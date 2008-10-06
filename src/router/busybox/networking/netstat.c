/* vi: set sw=4 ts=4: */
/*
 * Mini netstat implementation(s) for busybox
 * based in part on the netstat implementation from net-tools.
 *
 * Copyright (C) 2002 by Bart Visscher <magick@linux-fan.com>
 *
 * 2002-04-20
 * IPV6 support added by Bart Visscher <magick@linux-fan.com>
 *
 * 2008-07-10
 * optional '-p' flag support ported from net-tools by G. Somlo <somlo@cmu.edu>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"
#include "inet_common.h"

#define NETSTAT_OPTS "laentuwx" \
	USE_ROUTE(               "r") \
	USE_FEATURE_NETSTAT_WIDE("W") \
	USE_FEATURE_NETSTAT_PRG( "p")

enum {
	OPTBIT_KEEP_OLD = 7,
	USE_ROUTE(               OPTBIT_ROUTE,)
	USE_FEATURE_NETSTAT_WIDE(OPTBIT_WIDE ,)
	USE_FEATURE_NETSTAT_PRG( OPTBIT_PRG  ,)
	OPT_sock_listen = 1 << 0, // l
	OPT_sock_all    = 1 << 1, // a
	OPT_extended    = 1 << 2, // e
	OPT_noresolve   = 1 << 3, // n
	OPT_sock_tcp    = 1 << 4, // t
	OPT_sock_udp    = 1 << 5, // u
	OPT_sock_raw    = 1 << 6, // w
	OPT_sock_unix   = 1 << 7, // x
	OPT_route       = USE_ROUTE(               (1 << OPTBIT_ROUTE)) + 0, // r
	OPT_wide        = USE_FEATURE_NETSTAT_WIDE((1 << OPTBIT_WIDE )) + 0, // W
	OPT_prg         = USE_FEATURE_NETSTAT_PRG( (1 << OPTBIT_PRG  )) + 0, // p
};

#define NETSTAT_CONNECTED 0x01
#define NETSTAT_LISTENING 0x02
#define NETSTAT_NUMERIC   0x04
/* Must match getopt32 option string */
#define NETSTAT_TCP       0x10
#define NETSTAT_UDP       0x20
#define NETSTAT_RAW       0x40
#define NETSTAT_UNIX      0x80
#define NETSTAT_ALLPROTO  (NETSTAT_TCP|NETSTAT_UDP|NETSTAT_RAW|NETSTAT_UNIX)


enum {
	TCP_ESTABLISHED = 1,
	TCP_SYN_SENT,
	TCP_SYN_RECV,
	TCP_FIN_WAIT1,
	TCP_FIN_WAIT2,
	TCP_TIME_WAIT,
	TCP_CLOSE,
	TCP_CLOSE_WAIT,
	TCP_LAST_ACK,
	TCP_LISTEN,
	TCP_CLOSING, /* now a valid state */
};

static const char *const tcp_state[] = {
	"",
	"ESTABLISHED",
	"SYN_SENT",
	"SYN_RECV",
	"FIN_WAIT1",
	"FIN_WAIT2",
	"TIME_WAIT",
	"CLOSE",
	"CLOSE_WAIT",
	"LAST_ACK",
	"LISTEN",
	"CLOSING"
};

typedef enum {
	SS_FREE = 0,     /* not allocated                */
	SS_UNCONNECTED,  /* unconnected to any socket    */
	SS_CONNECTING,   /* in process of connecting     */
	SS_CONNECTED,    /* connected to socket          */
	SS_DISCONNECTING /* in process of disconnecting  */
} socket_state;

#define SO_ACCEPTCON (1<<16)	/* performed a listen           */
#define SO_WAITDATA  (1<<17)	/* wait data to read            */
#define SO_NOSPACE   (1<<18)	/* no space to write            */

/* Standard printout size */
#define PRINT_IP_MAX_SIZE           23
#define PRINT_NET_CONN              "%s   %6ld %6ld %-23s %-23s %-12s"
#define PRINT_NET_CONN_HEADER       "\nProto Recv-Q Send-Q %-23s %-23s State       "

/* When there are IPv6 connections the IPv6 addresses will be
 * truncated to none-recognition. The '-W' option makes the
 * address columns wide enough to accomodate for longest possible
 * IPv6 addresses, i.e. addresses of the form
 * xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd
 */
#define PRINT_IP_MAX_SIZE_WIDE      51  /* INET6_ADDRSTRLEN + 5 for the port number */
#define PRINT_NET_CONN_WIDE         "%s   %6ld %6ld %-51s %-51s %-12s"
#define PRINT_NET_CONN_HEADER_WIDE  "\nProto Recv-Q Send-Q %-51s %-51s State       "


#define PROGNAME_WIDTH     20
#define PROGNAME_WIDTH_STR "20"
/* PROGNAME_WIDTH chars: 12345678901234567890 */
#define PROGNAME_BANNER "PID/Program name    "

struct prg_node {
	struct prg_node *next;
	long inode;
	char name[PROGNAME_WIDTH];
};

#define PRG_HASH_SIZE 211


struct globals {
	const char *net_conn_line;
	smallint flags;
#if ENABLE_FEATURE_NETSTAT_PRG
	smallint prg_cache_loaded;
	struct prg_node *prg_hash[PRG_HASH_SIZE];
#endif
};
#define G (*ptr_to_globals)
#define flags            (G.flags           )
#define net_conn_line    (G.net_conn_line   )
#define prg_hash         (G.prg_hash        )
#define prg_cache_loaded (G.prg_cache_loaded)
#define INIT_G() do { \
	SET_PTR_TO_GLOBALS(xzalloc(sizeof(G))); \
	flags = NETSTAT_CONNECTED | NETSTAT_ALLPROTO; \
	net_conn_line = PRINT_NET_CONN; \
} while (0)


#if ENABLE_FEATURE_NETSTAT_PRG

/* Deliberately truncating long to unsigned *int* */
#define PRG_HASHIT(x) ((unsigned)(x) % PRG_HASH_SIZE)

#define print_progname_banner() do { \
	if (option_mask32 & OPT_prg) printf(PROGNAME_BANNER); \
} while (0)

static void prg_cache_add(long inode, char *name)
{
	unsigned hi = PRG_HASHIT(inode);
	struct prg_node **pnp, *pn;

	prg_cache_loaded = 2;
	for (pnp = prg_hash + hi; (pn = *pnp) != NULL; pnp = &pn->next) {
		if (pn->inode == inode) {
			/* Some warning should be appropriate here
			   as we got multiple processes for one i-node */
			return;
		}
	}
	*pnp = xzalloc(sizeof(struct prg_node));
	pn = *pnp;
	pn->inode = inode;
	safe_strncpy(pn->name, name, PROGNAME_WIDTH);
}

static const char *prg_cache_get(long inode)
{
	unsigned hi = PRG_HASHIT(inode);
	struct prg_node *pn;

	for (pn = prg_hash[hi]; pn; pn = pn->next)
		if (pn->inode == inode)
			return pn->name;
	return "-";
}

#if ENABLE_FEATURE_CLEAN_UP
static void prg_cache_clear(void)
{
	struct prg_node **pnp, *pn;

	for (pnp = prg_hash; pnp < prg_hash + PRG_HASH_SIZE; pnp++) {
		while ((pn = *pnp) != NULL) {
			*pnp = pn->next;
			free(pn);
		}
	}
}
#else
#define prg_cache_clear() ((void)0)
#endif

static long extract_socket_inode(const char *lname)
{
	long inode = -1;

	if (strncmp(lname, "socket:[", sizeof("socket:[")-1) == 0) {
		/* "socket:[12345]", extract the "12345" as inode */
		inode = bb_strtol(lname + sizeof("socket:[")-1, (char**)&lname, 0);
		if (*lname != ']')
			inode = -1;
	} else if (strncmp(lname, "[0000]:", sizeof("[0000]:")-1) == 0) {
		/* "[0000]:12345", extract the "12345" as inode */
		inode = bb_strtol(lname + sizeof("[0000]:")-1, NULL, 0);
		if (errno) /* not NUL terminated? */
			inode = -1;
	}

#if 0 /* bb_strtol returns all-ones bit pattern on ERANGE anyway */
	if (errno == ERANGE)
		inode = -1;
#endif
	return inode;
}

static int FAST_FUNC file_act(const char *fileName,
		struct stat *statbuf UNUSED_PARAM,
		void *userData,
		int depth UNUSED_PARAM)
{
	char *linkname;
	long inode;

	linkname = xmalloc_readlink(fileName);
	if (linkname != NULL) {
		inode = extract_socket_inode(linkname);
		free(linkname);
		if (inode >= 0)
			prg_cache_add(inode, (char *)userData);
	}
	return TRUE;
}

static int FAST_FUNC dir_act(const char *fileName,
		struct stat *statbuf UNUSED_PARAM,
		void *userData UNUSED_PARAM,
		int depth)
{
	const char *shortName;
	char *p, *q;
	char cmdline_buf[512];
	int i;

	if (depth == 0) /* "/proc" itself */
		return TRUE; /* continue looking one level below /proc */

	shortName = fileName + sizeof("/proc/")-1; /* point after "/proc/" */
	if (!isdigit(shortName[0])) /* skip /proc entries whic aren't processes */
		return SKIP;

	p = concat_path_file(fileName, "cmdline"); /* "/proc/PID/cmdline" */
	i = open_read_close(p, cmdline_buf, sizeof(cmdline_buf) - 1);
	free(p);
	if (i < 0)
		return FALSE;
	cmdline_buf[i] = '\0';
	q = concat_path_file(shortName, bb_basename(cmdline_buf)); /* "PID/argv0" */

	/* go through all files in /proc/PID/fd */
	p = concat_path_file(fileName, "fd");
	i = recursive_action(p, ACTION_RECURSE | ACTION_QUIET,
				file_act, NULL, (void *)q, 0);

	free(p);
	free(q);

	if (!i)
		return FALSE;	/* signal permissions error to caller */

	return SKIP;		/* caller should not recurse further into this dir. */
}

static void prg_cache_load(void)
{
	int load_ok;

	prg_cache_loaded = 1;
	load_ok = recursive_action("/proc", ACTION_RECURSE | ACTION_QUIET,
				NULL, dir_act, NULL, 0);
	if (load_ok)
		return;

	if (prg_cache_loaded == 1)
		bb_error_msg("can't scan /proc - are you root?");
	else
		bb_error_msg("showing only processes with your user ID");
}

#else

#define prg_cache_clear()       ((void)0)
#define print_progname_banner() ((void)0)

#endif //ENABLE_FEATURE_NETSTAT_PRG


#if ENABLE_FEATURE_IPV6
static void build_ipv6_addr(char* local_addr, struct sockaddr_in6* localaddr)
{
	char addr6[INET6_ADDRSTRLEN];
	struct in6_addr in6;

	sscanf(local_addr, "%08X%08X%08X%08X",
		   &in6.s6_addr32[0], &in6.s6_addr32[1],
		   &in6.s6_addr32[2], &in6.s6_addr32[3]);
	inet_ntop(AF_INET6, &in6, addr6, sizeof(addr6));
	inet_pton(AF_INET6, addr6, (struct sockaddr *) &localaddr->sin6_addr);

	localaddr->sin6_family = AF_INET6;
}
#endif

#if ENABLE_FEATURE_IPV6
static void build_ipv4_addr(char* local_addr, struct sockaddr_in6* localaddr)
#else
static void build_ipv4_addr(char* local_addr, struct sockaddr_in* localaddr)
#endif
{
	sscanf(local_addr, "%X",
		   &((struct sockaddr_in *) localaddr)->sin_addr.s_addr);
	((struct sockaddr *) localaddr)->sa_family = AF_INET;
}

static const char *get_sname(int port, const char *proto, int numeric)
{
	if (!port)
		return "*";
	if (!numeric) {
		struct servent *se = getservbyport(port, proto);
		if (se)
			return se->s_name;
	}
	/* hummm, we may return static buffer here!! */
	return itoa(ntohs(port));
}

static char *ip_port_str(struct sockaddr *addr, int port, const char *proto, int numeric)
{
	char *host, *host_port;

	/* Code which used "*" for INADDR_ANY is removed: it's ambiguous
	 * in IPv6, while "0.0.0.0" is not. */

	host = numeric ? xmalloc_sockaddr2dotted_noport(addr)
	               : xmalloc_sockaddr2host_noport(addr);

	host_port = xasprintf("%s:%s", host, get_sname(htons(port), proto, numeric));
	free(host);
	return host_port;
}

struct inet_params {
	int local_port, rem_port, state, uid;
#if ENABLE_FEATURE_IPV6
	struct sockaddr_in6 localaddr, remaddr;
#else
	struct sockaddr_in localaddr, remaddr;
#endif
	unsigned long rxq, txq, inode;
};

static int scan_inet_proc_line(struct inet_params *param, char *line)
{
	int num;
	char local_addr[64], rem_addr[64];

	num = sscanf(line,
			"%*d: %64[0-9A-Fa-f]:%X "
			"%64[0-9A-Fa-f]:%X %X "
			"%lX:%lX %*X:%*X "
			"%*X %d %*d %ld ",
			local_addr, &param->local_port,
			rem_addr, &param->rem_port, &param->state,
			&param->txq, &param->rxq,
			&param->uid, &param->inode);
	if (num < 9) {
		return 1; /* error */
	}

	if (strlen(local_addr) > 8) {
#if ENABLE_FEATURE_IPV6
		build_ipv6_addr(local_addr, &param->localaddr);
		build_ipv6_addr(rem_addr, &param->remaddr);
#endif
	} else {
		build_ipv4_addr(local_addr, &param->localaddr);
		build_ipv4_addr(rem_addr, &param->remaddr);
	}
	return 0;
}

static void print_inet_line(struct inet_params *param,
		const char *state_str, const char *proto, int is_connected)
{
	if ((is_connected && (flags & NETSTAT_CONNECTED))
	 || (!is_connected && (flags & NETSTAT_LISTENING))
	) {
		char *l = ip_port_str(
				(struct sockaddr *) &param->localaddr, param->local_port,
				proto, flags & NETSTAT_NUMERIC);
		char *r = ip_port_str(
				(struct sockaddr *) &param->remaddr, param->rem_port,
				proto, flags & NETSTAT_NUMERIC);
		printf(net_conn_line,
			proto, param->rxq, param->txq, l, r, state_str);
#if ENABLE_FEATURE_NETSTAT_PRG
		if (option_mask32 & OPT_prg)
			printf("%."PROGNAME_WIDTH_STR"s", prg_cache_get(param->inode));
#endif
		bb_putchar('\n');
		free(l);
		free(r);
	}
}

static int FAST_FUNC tcp_do_one(char *line)
{
	struct inet_params param;

	if (scan_inet_proc_line(&param, line))
		return 1;

	print_inet_line(&param, tcp_state[param.state], "tcp", param.rem_port);
	return 0;
}

#if ENABLE_FEATURE_IPV6
# define notnull(A) ( \
	( (A.sin6_family == AF_INET6)                               \
	  && (A.sin6_addr.s6_addr32[0] | A.sin6_addr.s6_addr32[1] | \
	      A.sin6_addr.s6_addr32[2] | A.sin6_addr.s6_addr32[3])  \
	) || (                                                      \
	  (A.sin6_family == AF_INET)                                \
	  && ((struct sockaddr_in*)&A)->sin_addr.s_addr             \
	)                                                           \
)
#else
# define notnull(A) (A.sin_addr.s_addr)
#endif

static int FAST_FUNC udp_do_one(char *line)
{
	int have_remaddr;
	const char *state_str;
	struct inet_params param;

	if (scan_inet_proc_line(&param, line))
		return 1;

	state_str = "UNKNOWN";
	switch (param.state) {
	case TCP_ESTABLISHED:
		state_str = "ESTABLISHED";
		break;
	case TCP_CLOSE:
		state_str = "";
		break;
	}

	have_remaddr = notnull(param.remaddr);
	print_inet_line(&param, state_str, "udp", have_remaddr);
	return 0;
}

static int FAST_FUNC raw_do_one(char *line)
{
	int have_remaddr;
	struct inet_params param;

	if (scan_inet_proc_line(&param, line))
		return 1;

	have_remaddr = notnull(param.remaddr);
	print_inet_line(&param, itoa(param.state), "raw", have_remaddr);
	return 0;
}

static int FAST_FUNC unix_do_one(char *line)
{
	unsigned long refcnt, proto, unix_flags;
	unsigned long inode;
	int type, state;
	int num, path_ofs;
	const char *ss_proto, *ss_state, *ss_type;
	char ss_flags[32];

	/* 2.6.15 may report lines like "... @/tmp/fam-user-^@^@^@^@^@^@^@..."
	 * Other users report long lines filled by NUL bytes.
	 * (those ^@ are NUL bytes too). We see them as empty lines. */
	if (!line[0])
		return 0;

	path_ofs = 0; /* paranoia */
	num = sscanf(line, "%*p: %lX %lX %lX %X %X %lu %n",
			&refcnt, &proto, &unix_flags, &type, &state, &inode, &path_ofs);
	if (num < 6) {
		return 1; /* error */
	}
	if ((flags & (NETSTAT_LISTENING|NETSTAT_CONNECTED)) != (NETSTAT_LISTENING|NETSTAT_CONNECTED)) {
		if ((state == SS_UNCONNECTED) && (unix_flags & SO_ACCEPTCON)) {
			if (!(flags & NETSTAT_LISTENING))
				return 0;
		} else {
			if (!(flags & NETSTAT_CONNECTED))
				return 0;
		}
	}

	switch (proto) {
		case 0:
			ss_proto = "unix";
			break;
		default:
			ss_proto = "??";
	}

	switch (type) {
		case SOCK_STREAM:
			ss_type = "STREAM";
			break;
		case SOCK_DGRAM:
			ss_type = "DGRAM";
			break;
		case SOCK_RAW:
			ss_type = "RAW";
			break;
		case SOCK_RDM:
			ss_type = "RDM";
			break;
		case SOCK_SEQPACKET:
			ss_type = "SEQPACKET";
			break;
		default:
			ss_type = "UNKNOWN";
	}

	switch (state) {
		case SS_FREE:
			ss_state = "FREE";
			break;
		case SS_UNCONNECTED:
			/*
			 * Unconnected sockets may be listening
			 * for something.
			 */
			if (unix_flags & SO_ACCEPTCON) {
				ss_state = "LISTENING";
			} else {
				ss_state = "";
			}
			break;
		case SS_CONNECTING:
			ss_state = "CONNECTING";
			break;
		case SS_CONNECTED:
			ss_state = "CONNECTED";
			break;
		case SS_DISCONNECTING:
			ss_state = "DISCONNECTING";
			break;
		default:
			ss_state = "UNKNOWN";
	}

	strcpy(ss_flags, "[ ");
	if (unix_flags & SO_ACCEPTCON)
		strcat(ss_flags, "ACC ");
	if (unix_flags & SO_WAITDATA)
		strcat(ss_flags, "W ");
	if (unix_flags & SO_NOSPACE)
		strcat(ss_flags, "N ");
	strcat(ss_flags, "]");

	printf("%-5s %-6ld %-11s %-10s %-13s %6lu ",
		ss_proto, refcnt, ss_flags, ss_type, ss_state, inode
		);

#if ENABLE_FEATURE_NETSTAT_PRG
	if (option_mask32 & OPT_prg)
		printf("%-"PROGNAME_WIDTH_STR"s", prg_cache_get(inode));
#endif

	/* TODO: currently we stop at first NUL byte. Is it a problem? */
	line += path_ofs;
	*strchrnul(line, '\n') = '\0';
	while (*line)
		fputc_printable(*line++, stdout);
	bb_putchar('\n');
	return 0;
}

static void do_info(const char *file, int FAST_FUNC (*proc)(char *))
{
	int lnr;
	FILE *procinfo;
	char *buffer;

	/* _stdin is just to save "r" param */
	procinfo = fopen_or_warn_stdin(file);
	if (procinfo == NULL) {
		return;
	}
	lnr = 0;
	/* Why xmalloc_fgets_str? because it doesn't stop on NULs */
	while ((buffer = xmalloc_fgets_str(procinfo, "\n")) != NULL) {
		/* line 0 is skipped */
		if (lnr && proc(buffer))
			bb_error_msg("%s: bogus data on line %d", file, lnr + 1);
		lnr++;
		free(buffer);
	}
	fclose(procinfo);
}

int netstat_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int netstat_main(int argc UNUSED_PARAM, char **argv)
{
	const char *net_conn_line_header = PRINT_NET_CONN_HEADER;
	unsigned opt;

	INIT_G();

	/* Option string must match NETSTAT_xxx constants */
	opt = getopt32(argv, NETSTAT_OPTS);
	if (opt & 0x1) { // -l
		flags &= ~NETSTAT_CONNECTED;
		flags |= NETSTAT_LISTENING;
	}
	if (opt & 0x2) flags |= NETSTAT_LISTENING | NETSTAT_CONNECTED; // -a
	//if (opt & 0x4) // -e
	if (opt & 0x8) flags |= NETSTAT_NUMERIC; // -n
	//if (opt & 0x10) // -t: NETSTAT_TCP
	//if (opt & 0x20) // -u: NETSTAT_UDP
	//if (opt & 0x40) // -w: NETSTAT_RAW
	//if (opt & 0x80) // -x: NETSTAT_UNIX
	if (opt & OPT_route) { // -r
#if ENABLE_ROUTE
		bb_displayroutes(flags & NETSTAT_NUMERIC, !(opt & OPT_extended));
		return 0;
#else
		bb_show_usage();
#endif
	}
	if (opt & OPT_wide) { // -W
		net_conn_line = PRINT_NET_CONN_WIDE;
		net_conn_line_header = PRINT_NET_CONN_HEADER_WIDE;
	}
#if ENABLE_FEATURE_NETSTAT_PRG
	if (opt & OPT_prg) { // -p
		prg_cache_load();
	}
#endif

	opt &= NETSTAT_ALLPROTO;
	if (opt) {
		flags &= ~NETSTAT_ALLPROTO;
		flags |= opt;
	}
	if (flags & (NETSTAT_TCP|NETSTAT_UDP|NETSTAT_RAW)) {
		printf("Active Internet connections ");	/* xxx */

		if ((flags & (NETSTAT_LISTENING|NETSTAT_CONNECTED)) == (NETSTAT_LISTENING|NETSTAT_CONNECTED))
			printf("(servers and established)");
		else if (flags & NETSTAT_LISTENING)
			printf("(only servers)");
		else
			printf("(w/o servers)");
		printf(net_conn_line_header, "Local Address", "Foreign Address");
		print_progname_banner();
		bb_putchar('\n');
	}
	if (flags & NETSTAT_TCP) {
		do_info("/proc/net/tcp", tcp_do_one);
#if ENABLE_FEATURE_IPV6
		do_info("/proc/net/tcp6", tcp_do_one);
#endif
	}
	if (flags & NETSTAT_UDP) {
		do_info("/proc/net/udp", udp_do_one);
#if ENABLE_FEATURE_IPV6
		do_info("/proc/net/udp6", udp_do_one);
#endif
	}
	if (flags & NETSTAT_RAW) {
		do_info("/proc/net/raw", raw_do_one);
#if ENABLE_FEATURE_IPV6
		do_info("/proc/net/raw6", raw_do_one);
#endif
	}
	if (flags & NETSTAT_UNIX) {
		printf("Active UNIX domain sockets ");
		if ((flags & (NETSTAT_LISTENING|NETSTAT_CONNECTED)) == (NETSTAT_LISTENING|NETSTAT_CONNECTED))
			printf("(servers and established)");
		else if (flags & NETSTAT_LISTENING)
			printf("(only servers)");
		else
			printf("(w/o servers)");
		printf("\nProto RefCnt Flags       Type       State         I-Node ");
		print_progname_banner();
		printf("Path\n");
		do_info("/proc/net/unix", unix_do_one);
	}
	prg_cache_clear();
	return 0;
}
