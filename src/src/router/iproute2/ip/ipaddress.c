/*
 * ipaddress.c		"ip address".
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Changes:
 *	Laszlo Valko <valko@linux.karinthy.hu> 990223: address label must be zero terminated
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fnmatch.h>

#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/sockios.h>

#include "rt_names.h"
#include "utils.h"
#include "ll_map.h"
#include "ip_common.h"

static struct
{
	int ifindex;
	int family;
	int oneline;
	int showqueue;
	inet_prefix pfx;
	int scope, scopemask;
	int flags, flagmask;
	int up;
	char *label;
	int flushed;
	char *flushb;
	int flushp;
	int flushe;
	struct rtnl_handle *rth;
} filter;

static int do_link;

static void usage(void) __attribute__((noreturn));

static void usage(void)
{
	if (do_link) {
		iplink_usage();
	}
	fprintf(stderr, "Usage: ip addr {add|del} IFADDR dev STRING\n");
	fprintf(stderr, "       ip addr {show|flush} [ dev STRING ] [ scope SCOPE-ID ]\n");
	fprintf(stderr, "                            [ to PREFIX ] [ FLAG-LIST ] [ label PATTERN ]\n");
	fprintf(stderr, "IFADDR := PREFIX | ADDR peer PREFIX\n");
	fprintf(stderr, "          [ broadcast ADDR ] [ anycast ADDR ]\n");
	fprintf(stderr, "          [ label STRING ] [ scope SCOPE-ID ]\n");
	fprintf(stderr, "SCOPE-ID := [ host | link | global | NUMBER ]\n");
	fprintf(stderr, "FLAG-LIST := [ FLAG-LIST ] FLAG\n");
	fprintf(stderr, "FLAG  := [ permanent | dynamic | secondary | primary |\n");
	fprintf(stderr, "           tentative | deprecated ]\n");
	exit(-1);
}

void print_link_flags(FILE *fp, unsigned flags, unsigned mdown)
{
	fprintf(fp, "<");
	if (flags & IFF_UP && !(flags & IFF_RUNNING))
		fprintf(fp, "NO-CARRIER%s", flags ? "," : "");
	flags &= ~IFF_RUNNING;
#define _PF(f) if (flags&IFF_##f) { \
                  flags &= ~IFF_##f ; \
                  fprintf(fp, #f "%s", flags ? "," : ""); }
	_PF(LOOPBACK);
	_PF(BROADCAST);
	_PF(POINTOPOINT);
	_PF(MULTICAST);
	_PF(NOARP);
	_PF(ALLMULTI);
	_PF(PROMISC);
	_PF(MASTER);
	_PF(SLAVE);
	_PF(DEBUG);
	_PF(DYNAMIC);
	_PF(AUTOMEDIA);
	_PF(PORTSEL);
	_PF(NOTRAILERS);
	_PF(UP);
#undef _PF
        if (flags)
		fprintf(fp, "%x", flags);
	if (mdown)
		fprintf(fp, ",M-DOWN");
	fprintf(fp, "> ");
}

void print_queuelen(char *name)
{
	struct ifreq ifr;
	int s;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		return;

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, name);
	if (ioctl(s, SIOCGIFTXQLEN, &ifr) < 0) { 
		perror("SIOCGIFXQLEN");
		close(s);
		return;
	}
	close(s);

	if (ifr.ifr_qlen)
		printf("qlen %d", ifr.ifr_qlen);
}

int print_linkinfo(const struct sockaddr_nl *who, 
		   struct nlmsghdr *n, void *arg)
{
	FILE *fp = (FILE*)arg;
	struct ifinfomsg *ifi = NLMSG_DATA(n);
	struct rtattr * tb[IFLA_MAX+1];
	int len = n->nlmsg_len;
	unsigned m_flag = 0;

	if (n->nlmsg_type != RTM_NEWLINK && n->nlmsg_type != RTM_DELLINK)
		return 0;

	len -= NLMSG_LENGTH(sizeof(*ifi));
	if (len < 0)
		return -1;

	if (filter.ifindex && ifi->ifi_index != filter.ifindex)
		return 0;
	if (filter.up && !(ifi->ifi_flags&IFF_UP))
		return 0;

	parse_rtattr(tb, IFLA_MAX, IFLA_RTA(ifi), len);
	if (tb[IFLA_IFNAME] == NULL) {
		fprintf(stderr, "BUG: nil ifname\n");
		return -1;
	}
	if (filter.label &&
	    (!filter.family || filter.family == AF_PACKET) &&
	    fnmatch(filter.label, RTA_DATA(tb[IFLA_IFNAME]), 0))
		return 0;

	if (n->nlmsg_type == RTM_DELLINK)
		fprintf(fp, "Deleted ");

	fprintf(fp, "%d: %s", ifi->ifi_index,
		tb[IFLA_IFNAME] ? (char*)RTA_DATA(tb[IFLA_IFNAME]) : "<nil>");

	if (tb[IFLA_LINK]) {
		SPRINT_BUF(b1);
		int iflink = *(int*)RTA_DATA(tb[IFLA_LINK]);
		if (iflink == 0)
			fprintf(fp, "@NONE: ");
		else {
			fprintf(fp, "@%s: ", ll_idx_n2a(iflink, b1));
			m_flag = ll_index_to_flags(iflink);
			m_flag = !(m_flag & IFF_UP);
		}
	} else {
		fprintf(fp, ": ");
	}
	print_link_flags(fp, ifi->ifi_flags, m_flag);

	if (tb[IFLA_MTU])
		fprintf(fp, "mtu %u ", *(int*)RTA_DATA(tb[IFLA_MTU]));
	if (tb[IFLA_QDISC])
		fprintf(fp, "qdisc %s ", (char*)RTA_DATA(tb[IFLA_QDISC]));
#ifdef IFLA_MASTER
	if (tb[IFLA_MASTER]) {
		SPRINT_BUF(b1);
		fprintf(fp, "master %s ", ll_idx_n2a(*(int*)RTA_DATA(tb[IFLA_MASTER]), b1));
	}
#endif
	if (filter.showqueue)
		print_queuelen((char*)RTA_DATA(tb[IFLA_IFNAME]));
	
	if (!filter.family || filter.family == AF_PACKET) {
		SPRINT_BUF(b1);
		fprintf(fp, "%s", _SL_);
		fprintf(fp, "    link/%s ", ll_type_n2a(ifi->ifi_type, b1, sizeof(b1)));

		if (tb[IFLA_ADDRESS]) {
			fprintf(fp, "%s", ll_addr_n2a(RTA_DATA(tb[IFLA_ADDRESS]),
						      RTA_PAYLOAD(tb[IFLA_ADDRESS]),
						      ifi->ifi_type,
						      b1, sizeof(b1)));
		}
		if (tb[IFLA_BROADCAST]) {
			if (ifi->ifi_flags&IFF_POINTOPOINT)
				fprintf(fp, " peer ");
			else
				fprintf(fp, " brd ");
			fprintf(fp, "%s", ll_addr_n2a(RTA_DATA(tb[IFLA_BROADCAST]),
						      RTA_PAYLOAD(tb[IFLA_BROADCAST]),
						      ifi->ifi_type,
						      b1, sizeof(b1)));
		}
	}
	if (do_link && tb[IFLA_STATS] && show_stats) {
		struct rtnl_link_stats slocal;
		struct rtnl_link_stats *s = RTA_DATA(tb[IFLA_STATS]);
		if (((unsigned long)s) & (sizeof(unsigned long)-1)) {
			memcpy(&slocal, s, sizeof(slocal));
			s = &slocal;
		}
		fprintf(fp, "%s", _SL_);
		fprintf(fp, "    RX: bytes  packets  errors  dropped overrun mcast   %s%s",
			s->rx_compressed ? "compressed" : "", _SL_);
		fprintf(fp, "    %-10u %-8u %-7u %-7u %-7u %-7u",
			s->rx_bytes, s->rx_packets, s->rx_errors,
			s->rx_dropped, s->rx_over_errors,
			s->multicast
			);
		if (s->rx_compressed)
			fprintf(fp, " %-7u", s->rx_compressed);
		if (show_stats > 1) {
			fprintf(fp, "%s", _SL_);
			fprintf(fp, "    RX errors: length  crc     frame   fifo    missed%s", _SL_);
			fprintf(fp, "               %-7u  %-7u %-7u %-7u %-7u",
				s->rx_length_errors,
				s->rx_crc_errors,
				s->rx_frame_errors,
				s->rx_fifo_errors,
				s->rx_missed_errors
				);
		}
		fprintf(fp, "%s", _SL_);
		fprintf(fp, "    TX: bytes  packets  errors  dropped carrier collsns %s%s",
			s->tx_compressed ? "compressed" : "", _SL_);
		fprintf(fp, "    %-10u %-8u %-7u %-7u %-7u %-7u",
			s->tx_bytes, s->tx_packets, s->tx_errors,
			s->tx_dropped, s->tx_carrier_errors, s->collisions);
		if (s->tx_compressed)
			fprintf(fp, " %-7u", s->tx_compressed);
		if (show_stats > 1) {
			fprintf(fp, "%s", _SL_);
			fprintf(fp, "    TX errors: aborted fifo    window  heartbeat%s", _SL_);
			fprintf(fp, "               %-7u  %-7u %-7u %-7u",
				s->tx_aborted_errors,
				s->tx_fifo_errors,
				s->tx_window_errors,
				s->tx_heartbeat_errors
				);
		}
	}
	fprintf(fp, "\n");
	fflush(fp);
	return 0;
}

static int flush_update(void)
{
	if (rtnl_send(filter.rth, filter.flushb, filter.flushp) < 0) {
		perror("Failed to send flush request\n");
		return -1;
	}
	filter.flushp = 0;
	return 0;
}

int print_addrinfo(const struct sockaddr_nl *who, struct nlmsghdr *n, 
		   void *arg)
{
	FILE *fp = (FILE*)arg;
	struct ifaddrmsg *ifa = NLMSG_DATA(n);
	int len = n->nlmsg_len;
	struct rtattr * rta_tb[IFA_MAX+1];
	char abuf[256];
	SPRINT_BUF(b1);

	if (n->nlmsg_type != RTM_NEWADDR && n->nlmsg_type != RTM_DELADDR)
		return 0;
	len -= NLMSG_LENGTH(sizeof(*ifa));
	if (len < 0) {
		fprintf(stderr, "BUG: wrong nlmsg len %d\n", len);
		return -1;
	}

	if (filter.flushb && n->nlmsg_type != RTM_NEWADDR)
		return 0;

	parse_rtattr(rta_tb, IFA_MAX, IFA_RTA(ifa), n->nlmsg_len - NLMSG_LENGTH(sizeof(*ifa)));

	if (!rta_tb[IFA_LOCAL])
		rta_tb[IFA_LOCAL] = rta_tb[IFA_ADDRESS];
	if (!rta_tb[IFA_ADDRESS])
		rta_tb[IFA_ADDRESS] = rta_tb[IFA_LOCAL];

	if (filter.ifindex && filter.ifindex != ifa->ifa_index)
		return 0;
	if ((filter.scope^ifa->ifa_scope)&filter.scopemask)
		return 0;
	if ((filter.flags^ifa->ifa_flags)&filter.flagmask)
		return 0;
	if (filter.label) {
		SPRINT_BUF(b1);
		const char *label;
		if (rta_tb[IFA_LABEL])
			label = RTA_DATA(rta_tb[IFA_LABEL]);
		else
			label = ll_idx_n2a(ifa->ifa_index, b1);
		if (fnmatch(filter.label, label, 0) != 0)
			return 0;
	}
	if (filter.pfx.family) {
		if (rta_tb[IFA_LOCAL]) {
			inet_prefix dst;
			memset(&dst, 0, sizeof(dst));
			dst.family = ifa->ifa_family;
			memcpy(&dst.data, RTA_DATA(rta_tb[IFA_LOCAL]), RTA_PAYLOAD(rta_tb[IFA_LOCAL]));
			if (inet_addr_match(&dst, &filter.pfx, filter.pfx.bitlen))
				return 0;
		}
	}

	if (filter.family && filter.family != ifa->ifa_family)
		return 0;

	if (filter.flushb) {
		struct nlmsghdr *fn;
		if (NLMSG_ALIGN(filter.flushp) + n->nlmsg_len > filter.flushe) {
			if (flush_update())
				return -1;
		}
		fn = (struct nlmsghdr*)(filter.flushb + NLMSG_ALIGN(filter.flushp));
		memcpy(fn, n, n->nlmsg_len);
		fn->nlmsg_type = RTM_DELADDR;
		fn->nlmsg_flags = NLM_F_REQUEST;
		fn->nlmsg_seq = ++filter.rth->seq;
		filter.flushp = (((char*)fn) + n->nlmsg_len) - filter.flushb;
		filter.flushed++;
		if (show_stats < 2)
			return 0;
	}

	if (n->nlmsg_type == RTM_DELADDR)
		fprintf(fp, "Deleted ");

	if (filter.oneline || filter.flushb)
		fprintf(fp, "%u: %s", ifa->ifa_index, ll_index_to_name(ifa->ifa_index));
	if (ifa->ifa_family == AF_INET)
		fprintf(fp, "    inet ");
	else if (ifa->ifa_family == AF_INET6)
		fprintf(fp, "    inet6 ");
	else if (ifa->ifa_family == AF_DECnet)
		fprintf(fp, "    dnet ");
	else if (ifa->ifa_family == AF_IPX)
		fprintf(fp, "     ipx ");
	else
		fprintf(fp, "    family %d ", ifa->ifa_family);

	if (rta_tb[IFA_LOCAL]) {
		fprintf(fp, "%s", rt_addr_n2a(ifa->ifa_family,
					      RTA_PAYLOAD(rta_tb[IFA_LOCAL]),
					      RTA_DATA(rta_tb[IFA_LOCAL]),
					      abuf, sizeof(abuf)));

		if (rta_tb[IFA_ADDRESS] == NULL ||
		    memcmp(RTA_DATA(rta_tb[IFA_ADDRESS]), RTA_DATA(rta_tb[IFA_LOCAL]), 4) == 0) {
			fprintf(fp, "/%d ", ifa->ifa_prefixlen);
		} else {
			fprintf(fp, " peer %s/%d ",
				rt_addr_n2a(ifa->ifa_family,
					    RTA_PAYLOAD(rta_tb[IFA_ADDRESS]),
					    RTA_DATA(rta_tb[IFA_ADDRESS]),
					    abuf, sizeof(abuf)),
				ifa->ifa_prefixlen);
		}
	}

	if (rta_tb[IFA_BROADCAST]) {
		fprintf(fp, "brd %s ",
			rt_addr_n2a(ifa->ifa_family,
				    RTA_PAYLOAD(rta_tb[IFA_BROADCAST]),
				    RTA_DATA(rta_tb[IFA_BROADCAST]),
				    abuf, sizeof(abuf)));
	}
	if (rta_tb[IFA_ANYCAST]) {
		fprintf(fp, "any %s ",
			rt_addr_n2a(ifa->ifa_family,
				    RTA_PAYLOAD(rta_tb[IFA_ANYCAST]),
				    RTA_DATA(rta_tb[IFA_ANYCAST]),
				    abuf, sizeof(abuf)));
	}
	fprintf(fp, "scope %s ", rtnl_rtscope_n2a(ifa->ifa_scope, b1, sizeof(b1)));
	if (ifa->ifa_flags&IFA_F_SECONDARY) {
		ifa->ifa_flags &= ~IFA_F_SECONDARY;
		fprintf(fp, "secondary ");
	}
	if (ifa->ifa_flags&IFA_F_TENTATIVE) {
		ifa->ifa_flags &= ~IFA_F_TENTATIVE;
		fprintf(fp, "tentative ");
	}
	if (ifa->ifa_flags&IFA_F_DEPRECATED) {
		ifa->ifa_flags &= ~IFA_F_DEPRECATED;
		fprintf(fp, "deprecated ");
	}
	if (!(ifa->ifa_flags&IFA_F_PERMANENT)) {
		fprintf(fp, "dynamic ");
	} else
		ifa->ifa_flags &= ~IFA_F_PERMANENT;
	if (ifa->ifa_flags)
		fprintf(fp, "flags %02x ", ifa->ifa_flags);
	if (rta_tb[IFA_LABEL])
		fprintf(fp, "%s", (char*)RTA_DATA(rta_tb[IFA_LABEL]));
	if (rta_tb[IFA_CACHEINFO]) {
		struct ifa_cacheinfo *ci = RTA_DATA(rta_tb[IFA_CACHEINFO]);
		char buf[128];
		fprintf(fp, "%s", _SL_);
		if (ci->ifa_valid == 0xFFFFFFFFU)
			sprintf(buf, "valid_lft forever");
		else
			sprintf(buf, "valid_lft %dsec", ci->ifa_valid);
		if (ci->ifa_prefered == 0xFFFFFFFFU)
			sprintf(buf+strlen(buf), " preferred_lft forever");
		else
			sprintf(buf+strlen(buf), " preferred_lft %dsec", ci->ifa_prefered);
		fprintf(fp, "       %s", buf);
	}
	fprintf(fp, "\n");
	fflush(fp);
	return 0;
}


struct nlmsg_list
{
	struct nlmsg_list *next;
	struct nlmsghdr	  h;
};

int print_selected_addrinfo(int ifindex, struct nlmsg_list *ainfo, FILE *fp)
{
	for ( ;ainfo ;  ainfo = ainfo->next) {
		struct nlmsghdr *n = &ainfo->h;
		struct ifaddrmsg *ifa = NLMSG_DATA(n);

		if (n->nlmsg_type != RTM_NEWADDR)
			continue;

		if (n->nlmsg_len < NLMSG_LENGTH(sizeof(ifa)))
			return -1;

		if (ifa->ifa_index != ifindex || 
		    (filter.family && filter.family != ifa->ifa_family))
			continue;

		print_addrinfo(NULL, n, fp);
	}
	return 0;
}


static int store_nlmsg(const struct sockaddr_nl *who, struct nlmsghdr *n, 
		       void *arg)
{
	struct nlmsg_list **linfo = (struct nlmsg_list**)arg;
	struct nlmsg_list *h;
	struct nlmsg_list **lp;

	h = malloc(n->nlmsg_len+sizeof(void*));
	if (h == NULL)
		return -1;

	memcpy(&h->h, n, n->nlmsg_len);
	h->next = NULL;

	for (lp = linfo; *lp; lp = &(*lp)->next) /* NOTHING */;
	*lp = h;

	ll_remember_index(who, n, NULL);
	return 0;
}

int ipaddr_list_or_flush(int argc, char **argv, int flush)
{
	struct nlmsg_list *linfo = NULL;
	struct nlmsg_list *ainfo = NULL;
	struct nlmsg_list *l;
	struct rtnl_handle rth;
	char *filter_dev = NULL;
	int no_link = 0;

	ipaddr_reset_filter(oneline);
	filter.showqueue = 1;

	if (filter.family == AF_UNSPEC)
		filter.family = preferred_family;

	if (flush) {
		if (argc <= 0) {
			fprintf(stderr, "Flush requires arguments.\n");
			return -1;
		}
		if (filter.family == AF_PACKET) {
			fprintf(stderr, "Cannot flush link addresses.\n");
			return -1;
		}
	}

	while (argc > 0) {
		if (strcmp(*argv, "to") == 0) {
			NEXT_ARG();
			get_prefix(&filter.pfx, *argv, filter.family);
			if (filter.family == AF_UNSPEC)
				filter.family = filter.pfx.family;
		} else if (strcmp(*argv, "scope") == 0) {
			int scope = 0;
			NEXT_ARG();
			filter.scopemask = -1;
			if (rtnl_rtscope_a2n(&scope, *argv)) {
				if (strcmp(*argv, "all") != 0)
					invarg("invalid \"scope\"\n", *argv);
				scope = RT_SCOPE_NOWHERE;
				filter.scopemask = 0;
			}
			filter.scope = scope;
		} else if (strcmp(*argv, "up") == 0) {
			filter.up = 1;
		} else if (strcmp(*argv, "dynamic") == 0) {
			filter.flags &= ~IFA_F_PERMANENT;
			filter.flagmask |= IFA_F_PERMANENT;
		} else if (strcmp(*argv, "permanent") == 0) {
			filter.flags |= IFA_F_PERMANENT;
			filter.flagmask |= IFA_F_PERMANENT;
		} else if (strcmp(*argv, "secondary") == 0) {
			filter.flags |= IFA_F_SECONDARY;
			filter.flagmask |= IFA_F_SECONDARY;
		} else if (strcmp(*argv, "primary") == 0) {
			filter.flags &= ~IFA_F_SECONDARY;
			filter.flagmask |= IFA_F_SECONDARY;
		} else if (strcmp(*argv, "tentative") == 0) {
			filter.flags |= IFA_F_TENTATIVE;
			filter.flagmask |= IFA_F_TENTATIVE;
		} else if (strcmp(*argv, "deprecated") == 0) {
			filter.flags |= IFA_F_DEPRECATED;
			filter.flagmask |= IFA_F_DEPRECATED;
		} else if (strcmp(*argv, "label") == 0) {
			NEXT_ARG();
			filter.label = *argv;
		} else {
			if (strcmp(*argv, "dev") == 0) {
				NEXT_ARG();
			}
			if (matches(*argv, "help") == 0)
				usage();
			if (filter_dev)
				duparg2("dev", *argv);
			filter_dev = *argv;
		}
		argv++; argc--;
	}

	if (rtnl_open(&rth, 0) < 0)
		exit(1);

	if (rtnl_wilddump_request(&rth, preferred_family, RTM_GETLINK) < 0) {
		perror("Cannot send dump request");
		exit(1);
	}

	if (rtnl_dump_filter(&rth, store_nlmsg, &linfo, NULL, NULL) < 0) {
		fprintf(stderr, "Dump terminated\n");
		exit(1);
	}

	if (filter_dev) {
		filter.ifindex = ll_name_to_index(filter_dev);
		if (filter.ifindex <= 0) {
			fprintf(stderr, "Device \"%s\" does not exist.\n", filter_dev);
			return -1;
		}
	}

	if (flush) {
		int round = 0;
		char flushb[4096-512];

		filter.flushb = flushb;
		filter.flushp = 0;
		filter.flushe = sizeof(flushb);
		filter.rth = &rth;

		for (;;) {
			if (rtnl_wilddump_request(&rth, filter.family, RTM_GETADDR) < 0) {
				perror("Cannot send dump request");
				exit(1);
			}
			filter.flushed = 0;
			if (rtnl_dump_filter(&rth, print_addrinfo, stdout, NULL, NULL) < 0) {
				fprintf(stderr, "Flush terminated\n");
				exit(1);
			}
			if (filter.flushed == 0) {
				if (round == 0) {
					fprintf(stderr, "Nothing to flush.\n");
				} else if (show_stats)
					printf("*** Flush is complete after %d round%s ***\n", round, round>1?"s":"");
				fflush(stdout);
				return 0;
			}
			round++;
			if (flush_update() < 0)
				exit(1);
			if (show_stats) {
				printf("\n*** Round %d, deleting %d addresses ***\n", round, filter.flushed);
				fflush(stdout);
			}
		}
	}

	if (filter.family != AF_PACKET) {
		if (rtnl_wilddump_request(&rth, filter.family, RTM_GETADDR) < 0) {
			perror("Cannot send dump request");
			exit(1);
		}

		if (rtnl_dump_filter(&rth, store_nlmsg, &ainfo, NULL, NULL) < 0) {
			fprintf(stderr, "Dump terminated\n");
			exit(1);
		}
	}


	if (filter.family && filter.family != AF_PACKET) {
		struct nlmsg_list **lp;
		lp=&linfo;

		if (filter.oneline)
			no_link = 1;

		while ((l=*lp)!=NULL) {
			int ok = 0;
			struct ifinfomsg *ifi = NLMSG_DATA(&l->h);
			struct nlmsg_list *a;

			for (a=ainfo; a; a=a->next) {
				struct nlmsghdr *n = &a->h;
				struct ifaddrmsg *ifa = NLMSG_DATA(n);

				if (ifa->ifa_index != ifi->ifi_index || 
				    (filter.family && filter.family != ifa->ifa_family))
					continue;
				if ((filter.scope^ifa->ifa_scope)&filter.scopemask)
					continue;
				if ((filter.flags^ifa->ifa_flags)&filter.flagmask)
					continue;
				if (filter.pfx.family || filter.label) {
					struct rtattr *tb[IFA_MAX+1];
					parse_rtattr(tb, IFA_MAX, IFA_RTA(ifa), IFA_PAYLOAD(n));
					if (!tb[IFA_LOCAL])
						tb[IFA_LOCAL] = tb[IFA_ADDRESS];

					if (filter.pfx.family && tb[IFA_LOCAL]) {
						inet_prefix dst;
						memset(&dst, 0, sizeof(dst));
						dst.family = ifa->ifa_family;
						memcpy(&dst.data, RTA_DATA(tb[IFA_LOCAL]), RTA_PAYLOAD(tb[IFA_LOCAL]));
						if (inet_addr_match(&dst, &filter.pfx, filter.pfx.bitlen))
							continue;
					}
					if (filter.label) {
						SPRINT_BUF(b1);
						const char *label;
						if (tb[IFA_LABEL])
							label = RTA_DATA(tb[IFA_LABEL]);
						else
							label = ll_idx_n2a(ifa->ifa_index, b1);
						if (fnmatch(filter.label, label, 0) != 0)
							continue;
					}
				}

				ok = 1;
				break;
			}
			if (!ok)
				*lp = l->next;
			else
				lp = &l->next;
		}
	}

	for (l=linfo; l; l = l->next) {
		if (no_link || print_linkinfo(NULL, &l->h, stdout) == 0) {
			struct ifinfomsg *ifi = NLMSG_DATA(&l->h);
			if (filter.family != AF_PACKET)
				print_selected_addrinfo(ifi->ifi_index, ainfo, stdout);
		}
		fflush(stdout);
	}

	exit(0);
}

int ipaddr_list_link(int argc, char **argv)
{
	preferred_family = AF_PACKET;
	do_link = 1;
	return ipaddr_list_or_flush(argc, argv, 0);
}

void ipaddr_reset_filter(int oneline)
{
	memset(&filter, 0, sizeof(filter));
	filter.oneline = oneline;
}

int default_scope(inet_prefix *lcl)
{
	if (lcl->family == AF_INET) {
		if (lcl->bytelen >= 1 && *(__u8*)&lcl->data == 127)
			return RT_SCOPE_HOST;
	}
	return 0;
}

int ipaddr_modify(int cmd, int argc, char **argv)
{
	struct rtnl_handle rth;
	struct {
		struct nlmsghdr 	n;
		struct ifaddrmsg 	ifa;
		char   			buf[256];
	} req;
	char  *d = NULL;
	char  *l = NULL;
	char  *lcl_arg = NULL;
	inet_prefix lcl;
	inet_prefix peer;
	int local_len = 0;
	int peer_len = 0;
	int brd_len = 0;
	int any_len = 0;
	int scoped = 0;

	memset(&req, 0, sizeof(req));

	req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
	req.n.nlmsg_flags = NLM_F_REQUEST;
	req.n.nlmsg_type = cmd;
	req.ifa.ifa_family = preferred_family;

	while (argc > 0) {
		if (strcmp(*argv, "peer") == 0 ||
		    strcmp(*argv, "remote") == 0) {
			NEXT_ARG();

			if (peer_len)
				duparg("peer", *argv);
			get_prefix(&peer, *argv, req.ifa.ifa_family);
			peer_len = peer.bytelen;
			if (req.ifa.ifa_family == AF_UNSPEC)
				req.ifa.ifa_family = peer.family;
			addattr_l(&req.n, sizeof(req), IFA_ADDRESS, &peer.data, peer.bytelen);
			req.ifa.ifa_prefixlen = peer.bitlen;
		} else if (matches(*argv, "broadcast") == 0 ||
			   strcmp(*argv, "brd") == 0) {
			inet_prefix addr;
			NEXT_ARG();
			if (brd_len)
				duparg("broadcast", *argv);
			if (strcmp(*argv, "+") == 0)
				brd_len = -1;
			else if (strcmp(*argv, "-") == 0)
				brd_len = -2;
			else {
				get_addr(&addr, *argv, req.ifa.ifa_family);
				if (req.ifa.ifa_family == AF_UNSPEC)
					req.ifa.ifa_family = addr.family;
				addattr_l(&req.n, sizeof(req), IFA_BROADCAST, &addr.data, addr.bytelen);
				brd_len = addr.bytelen;
			}
		} else if (strcmp(*argv, "anycast") == 0) {
			inet_prefix addr;
			NEXT_ARG();
			if (any_len)
				duparg("anycast", *argv);
			get_addr(&addr, *argv, req.ifa.ifa_family);
			if (req.ifa.ifa_family == AF_UNSPEC)
				req.ifa.ifa_family = addr.family;
			addattr_l(&req.n, sizeof(req), IFA_ANYCAST, &addr.data, addr.bytelen);
			any_len = addr.bytelen;
		} else if (strcmp(*argv, "scope") == 0) {
			int scope = 0;
			NEXT_ARG();
			if (rtnl_rtscope_a2n(&scope, *argv))
				invarg(*argv, "invalid scope value.");
			req.ifa.ifa_scope = scope;
			scoped = 1;
		} else if (strcmp(*argv, "dev") == 0) {
			NEXT_ARG();
			d = *argv;
		} else if (strcmp(*argv, "label") == 0) {
			NEXT_ARG();
			l = *argv;
			addattr_l(&req.n, sizeof(req), IFA_LABEL, l, strlen(l)+1);
		} else {
			if (strcmp(*argv, "local") == 0) {
				NEXT_ARG();
			}
			if (matches(*argv, "help") == 0)
				usage();
			if (local_len)
				duparg2("local", *argv);
			lcl_arg = *argv;
			get_prefix(&lcl, *argv, req.ifa.ifa_family);
			if (req.ifa.ifa_family == AF_UNSPEC)
				req.ifa.ifa_family = lcl.family;
			addattr_l(&req.n, sizeof(req), IFA_LOCAL, &lcl.data, lcl.bytelen);
			local_len = lcl.bytelen;
		}
		argc--; argv++;
	}
	if (d == NULL) {
		fprintf(stderr, "Not enough information: \"dev\" argument is required.\n");
		return -1;
	}
	if (l && matches(d, l) != 0) {
		fprintf(stderr, "\"dev\" (%s) must match \"label\" (%s).\n", d, l);
		exit(1);
	}

	if (peer_len == 0 && local_len) {
		if (cmd == RTM_DELADDR && lcl.family == AF_INET && !(lcl.flags & PREFIXLEN_SPECIFIED)) {
			fprintf(stderr,
			    "Warning: Executing wildcard deletion to stay compatible with old scripts.\n" \
			    "         Explicitly specify the prefix length (%s/%d) to avoid this warning.\n" \
			    "         This special behaviour is likely to disappear in further releases,\n" \
			    "         fix your scripts!\n", lcl_arg, local_len*8);
		} else {
			peer = lcl;
			addattr_l(&req.n, sizeof(req), IFA_ADDRESS, &lcl.data, lcl.bytelen);
		}
	}
	if (req.ifa.ifa_prefixlen == 0)
		req.ifa.ifa_prefixlen = lcl.bitlen;

	if (brd_len < 0 && cmd != RTM_DELADDR) {
		inet_prefix brd;
		int i;
		if (req.ifa.ifa_family != AF_INET) {
			fprintf(stderr, "Broadcast can be set only for IPv4 addresses\n");
			return -1;
		}
		brd = peer;
		if (brd.bitlen <= 30) {
			for (i=31; i>=brd.bitlen; i--) {
				if (brd_len == -1)
					brd.data[0] |= htonl(1<<(31-i));
				else
					brd.data[0] &= ~htonl(1<<(31-i));
			}
			addattr_l(&req.n, sizeof(req), IFA_BROADCAST, &brd.data, brd.bytelen);
			brd_len = brd.bytelen;
		}
	}
	if (!scoped && cmd != RTM_DELADDR)
		req.ifa.ifa_scope = default_scope(&lcl);

	if (rtnl_open(&rth, 0) < 0)
		exit(1);

	ll_init_map(&rth);

	if ((req.ifa.ifa_index = ll_name_to_index(d)) == 0) {
		fprintf(stderr, "Cannot find device \"%s\"\n", d);
		return -1;
	}

	if (rtnl_talk(&rth, &req.n, 0, 0, NULL, NULL, NULL) < 0)
		exit(2);

	exit(0);
}

int do_ipaddr(int argc, char **argv)
{
	if (argc < 1)
		return ipaddr_list_or_flush(0, NULL, 0);
	if (matches(*argv, "add") == 0)
		return ipaddr_modify(RTM_NEWADDR, argc-1, argv+1);
	if (matches(*argv, "delete") == 0)
		return ipaddr_modify(RTM_DELADDR, argc-1, argv+1);
	if (matches(*argv, "list") == 0 || matches(*argv, "show") == 0
	    || matches(*argv, "lst") == 0)
		return ipaddr_list_or_flush(argc-1, argv+1, 0);
	if (matches(*argv, "flush") == 0)
		return ipaddr_list_or_flush(argc-1, argv+1, 1);
	if (matches(*argv, "help") == 0)
		usage();
	fprintf(stderr, "Command \"%s\" is unknown, try \"ip address help\".\n", *argv);
	exit(-1);
}

