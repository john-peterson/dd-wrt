/*
 *Copyright(c)2004,Cisco URP imburses and Network Information Center in Beijing University of Posts and Telecommunications researches.
 *
 *All right reserved
 *
 *File Name:traceRouteCtlTable.h
 *File Description:The head file of traceRouteCtlTable.c
 *
 *Current Version:1.0
 *Author:ChenJing
 *Date:2004.8.20
 */


/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.create-dataset.conf,v 5.3 2002/12/05 00:29:45 hardaker Exp $
 */
#ifndef TRACEROUTECTLTABLE_H
#define TRACEROUTECTLTABLE_H


config_require(header_complex);

struct traceRouteCtlTable_data {
    char           *traceRouteCtlOwnerIndex;    /* string */
    size_t          traceRouteCtlOwnerIndexLen;

    char           *traceRouteCtlTestName;      /* string */
    size_t          traceRouteCtlTestNameLen;


    long            traceRouteCtlTargetAddressType;

    char           *traceRouteCtlTargetAddress;
    size_t          traceRouteCtlTargetAddressLen;

    long            traceRouteCtlByPassRouteTable;

    unsigned long   traceRouteCtlDataSize;

    unsigned long   traceRouteCtlTimeOut;

    unsigned long   traceRouteCtlProbesPerHop;

    unsigned long   traceRouteCtlPort;

    unsigned long   traceRouteCtlMaxTtl;

    unsigned long   traceRouteCtlDSField;

    long            traceRouteCtlSourceAddressType;

    char           *traceRouteCtlSourceAddress;
    size_t          traceRouteCtlSourceAddressLen;

    long            traceRouteCtlIfIndex;

    char           *traceRouteCtlMiscOptions;
    size_t          traceRouteCtlMiscOptionsLen;

    unsigned long   traceRouteCtlMaxFailures;

    long            traceRouteCtlDontFragment;

    unsigned long   traceRouteCtlInitialTtl;

    unsigned long   traceRouteCtlFrequency;

    long            traceRouteCtlStorageType;

    long            traceRouteCtlAdminStatus;

    char           *traceRouteCtlDescr;
    size_t          traceRouteCtlDescrLen;

    unsigned long   traceRouteCtlMaxRows;

    char           *traceRouteCtlTrapGeneration;
    size_t          traceRouteCtlTrapGenerationLen;

    long            traceRouteCtlCreateHopsEntries;

    oid            *traceRouteCtlType;
    size_t          traceRouteCtlTypeLen;

    long            traceRouteCtlRowStatus;


    int             storageType;
    u_long          timer_id;
    unsigned long   traceRouteProbeHistoryMaxIndex;

    struct traceRouteResultsTable_data *traceRouteResults;
    struct traceRouteProbeHistoryTable_data *traceRouteProbeHis;
    struct traceRouteHopsTable_data *traceRouteHops;

};

struct traceRouteResultsTable_data {
    /* struct              pingResultsTable_data *next; */
    char           *traceRouteCtlOwnerIndex;    /* string */
    size_t          traceRouteCtlOwnerIndexLen;

    char           *traceRouteCtlTestName;      /* string */
    size_t          traceRouteCtlTestNameLen;

    long            traceRouteResultsOperStatus;
    unsigned long   traceRouteResultsCurHopCount;
    unsigned long   traceRouteResultsCurProbeCount;
    long            traceRouteResultsIpTgtAddrType;
    char           *traceRouteResultsIpTgtAddr;
    size_t          traceRouteResultsIpTgtAddrLen;
    unsigned long   traceRouteResultsTestAttempts;
    unsigned long   traceRouteResultsTestSuccesses;
    char           *traceRouteResultsLastGoodPath;
    size_t          traceRouteResultsLastGoodPathLen;

    int             storageType;

};

struct traceRouteProbeHistoryTable_data {
    struct traceRouteProbeHistoryTable_data *next;
    /* index */
    char           *traceRouteCtlOwnerIndex;    /* string */
    size_t          traceRouteCtlOwnerIndexLen;

    char           *traceRouteCtlTestName;      /* string */
    size_t          traceRouteCtlTestNameLen;
    unsigned long   traceRouteProbeHistoryIndex;
    unsigned long   traceRouteProbeHistoryHopIndex;
    unsigned long   traceRouteProbeHistoryProbeIndex;
    /* index          */
    long            traceRouteProbeHistoryHAddrType;
    char           *traceRouteProbeHistoryHAddr;
    size_t          traceRouteProbeHistoryHAddrLen;
    unsigned long   traceRouteProbeHistoryResponse;
    long            traceRouteProbeHistoryStatus;
    long            traceRouteProbeHistoryLastRC;
    char           *traceRouteProbeHistoryTime;
    size_t          traceRouteProbeHistoryTimeLen;

    int             storageType;

};

struct traceRouteHopsTable_data {
    struct traceRouteHopsTable_data *next;

    char           *traceRouteCtlOwnerIndex;    /* string */
    size_t          traceRouteCtlOwnerIndexLen;

    char           *traceRouteCtlTestName;      /* string */
    size_t          traceRouteCtlTestNameLen;

    unsigned long   traceRouteHopsHopIndex;
    long            traceRouteHopsIpTgtAddressType;
    char           *traceRouteHopsIpTgtAddress;
    size_t          traceRouteHopsIpTgtAddressLen;

    unsigned long   traceRouteHopsMinRtt;
    unsigned long   traceRouteHopsMaxRtt;
    unsigned long   traceRouteHopsAverageRtt;
    unsigned long   traceRouteHopsRttSumOfSquares;
    unsigned long   traceRouteHopsSentProbes;
    unsigned long   traceRouteHopsProbeResponses;
    char           *traceRouteHopsLastGoodProbe;
    size_t          traceRouteHopsLastGoodProbeLen;

    int             storageType;
};
/*
 * function declarations 
 */
void            init_traceRouteCtlTable(void);
FindVarMethod   var_traceRouteCtlTable;
void            parse_traceRouteCtlTable(const char *, char *);
SNMPCallback    store_traceRouteCtlTable;
void            init_trResultsTable(struct traceRouteCtlTable_data *);

WriteMethod     write_traceRouteCtlTargetAddressType;
WriteMethod     write_traceRouteCtlTargetAddress;
WriteMethod     write_traceRouteCtlByPassRouteTable;
WriteMethod     write_traceRouteCtlDataSize;
WriteMethod     write_traceRouteCtlTimeOut;
WriteMethod     write_traceRouteCtlProbesPerHop;
WriteMethod     write_traceRouteCtlPort;
WriteMethod     write_traceRouteCtlMaxTtl;
WriteMethod     write_traceRouteCtlDSField;
WriteMethod     write_traceRouteCtlSourceAddressType;
WriteMethod     write_traceRouteCtlSourceAddress;
WriteMethod     write_traceRouteCtlIfIndex;
WriteMethod     write_traceRouteCtlMiscOptions;
WriteMethod     write_traceRouteCtlMaxFailures;
WriteMethod     write_traceRouteCtlDontFragment;
WriteMethod     write_traceRouteCtlInitialTtl;
WriteMethod     write_traceRouteCtlFrequency;
WriteMethod     write_traceRouteCtlStorageType;
WriteMethod     write_traceRouteCtlAdminStatus;
WriteMethod     write_traceRouteCtlDescr;
WriteMethod     write_traceRouteCtlMaxRows;
WriteMethod     write_traceRouteCtlTrapGeneration;
WriteMethod     write_traceRouteCtlCreateHopsEntries;
WriteMethod     write_traceRouteCtlType;

WriteMethod     write_traceRouteCtlRowStatus;



#define TRACEROUTETRAPGENERATION_PATHCHANGE                 	0x80
#define TRACEROUTETRAPGENERATION_TESTFAILED                  	0x40
#define TRACEROUTETRAPGENERATION_TESTCOMPLETED          	0x20
#define TRACEROUTETRAPGENERATION_NULL					0x00


/*
 * column number definitions for table traceRouteCtlTable 
 */
#define COLUMN_TRACEROUTECTLOWNERINDEX		1
#define COLUMN_TRACEROUTECTLTESTNAME		2
#define COLUMN_TRACEROUTECTLTARGETADDRESSTYPE		3
#define COLUMN_TRACEROUTECTLTARGETADDRESS		4
#define COLUMN_TRACEROUTECTLBYPASSROUTETABLE		5
#define COLUMN_TRACEROUTECTLDATASIZE		6
#define COLUMN_TRACEROUTECTLTIMEOUT		7
#define COLUMN_TRACEROUTECTLPROBESPERHOP		8
#define COLUMN_TRACEROUTECTLPORT		9
#define COLUMN_TRACEROUTECTLMAXTTL		10
#define COLUMN_TRACEROUTECTLDSFIELD		11
#define COLUMN_TRACEROUTECTLSOURCEADDRESSTYPE		12
#define COLUMN_TRACEROUTECTLSOURCEADDRESS		13
#define COLUMN_TRACEROUTECTLIFINDEX		14
#define COLUMN_TRACEROUTECTLMISCOPTIONS		15
#define COLUMN_TRACEROUTECTLMAXFAILURES		16
#define COLUMN_TRACEROUTECTLDONTFRAGMENT		17
#define COLUMN_TRACEROUTECTLINITIALTTL		18
#define COLUMN_TRACEROUTECTLFREQUENCY		19
#define COLUMN_TRACEROUTECTLSTORAGETYPE		20
#define COLUMN_TRACEROUTECTLADMINSTATUS		21
#define COLUMN_TRACEROUTECTLDESCR		22
#define COLUMN_TRACEROUTECTLMAXROWS		23
#define COLUMN_TRACEROUTECTLTRAPGENERATION		24
#define COLUMN_TRACEROUTECTLCREATEHOPSENTRIES		25
#define COLUMN_TRACEROUTECTLTYPE		26
#define COLUMN_TRACEROUTECTLROWSTATUS		27
#endif                          /* TRACEROUTECTLTABLE_H */

/*
 * Copyright (c) 1988, 1989, 1991, 1994, 1995, 1996, 1997, 1998, 1999, 2000
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static const char copyright[] =
    "@(#) Copyright (c) 1988, 1989, 1991, 1994, 1995, 1996, 1997, 1998, 1999, 2000\n\
The Regents of the University of California.  All rights reserved.\n";
static const char rcsid[] =
    "@(#)$Id: traceRouteCtlTable.h,v 1.1 2004/08/24 15:15:03 dts12 Exp $ (LBL)";
#endif

/*
 * traceroute host  - trace the route ip packets follow going to "host".
 *
 * Attempt to trace the route an ip packet would follow to some
 * internet host.  We find out intermediate hops by launching probe
 * packets with a small ttl (time to live) then listening for an
 * icmp "time exceeded" reply from a gateway.  We start our probes
 * with a ttl of one and increase by one until we get an icmp "port
 * unreachable" (which means we got to "host") or hit a max (which
 * defaults to 30 hops & can be changed with the -m flag).  Three
 * probes (change with -q flag) are sent at each ttl setting and a
 * line is printed showing the ttl, address of the gateway and
 * round trip time of each probe.  If the probe answers come from
 * different gateways, the address of each responding system will
 * be printed.  If there is no response within a 5 sec. timeout
 * interval (changed with the -w flag), a "*" is printed for that
 * probe.
 *
 * Probe packets are UDP format.  We don't want the destination
 * host to process them so the destination port is set to an
 * unlikely value (if some clod on the destination is using that
 * value, it can be changed with the -p flag).
 *
 * A sample use might be:
 *
 *     [yak 71]% traceroute nis.nsf.net.
 *     traceroute to nis.nsf.net (35.1.1.48), 30 hops max, 56 byte packet
 *      1  helios.ee.lbl.gov (128.3.112.1)  19 ms  19 ms  0 ms
 *      2  lilac-dmc.Berkeley.EDU (128.32.216.1)  39 ms  39 ms  19 ms
 *      3  lilac-dmc.Berkeley.EDU (128.32.216.1)  39 ms  39 ms  19 ms
 *      4  ccngw-ner-cc.Berkeley.EDU (128.32.136.23)  39 ms  40 ms  39 ms
 *      5  ccn-nerif22.Berkeley.EDU (128.32.168.22)  39 ms  39 ms  39 ms
 *      6  128.32.197.4 (128.32.197.4)  40 ms  59 ms  59 ms
 *      7  131.119.2.5 (131.119.2.5)  59 ms  59 ms  59 ms
 *      8  129.140.70.13 (129.140.70.13)  99 ms  99 ms  80 ms
 *      9  129.140.71.6 (129.140.71.6)  139 ms  239 ms  319 ms
 *     10  129.140.81.7 (129.140.81.7)  220 ms  199 ms  199 ms
 *     11  nic.merit.edu (35.1.1.48)  239 ms  239 ms  239 ms
 *
 * Note that lines 2 & 3 are the same.  This is due to a buggy
 * kernel on the 2nd hop system -- lbl-csam.arpa -- that forwards
 * packets with a zero ttl.
 *
 * A more interesting example is:
 *
 *     [yak 72]% traceroute allspice.lcs.mit.edu.
 *     traceroute to allspice.lcs.mit.edu (18.26.0.115), 30 hops max
 *      1  helios.ee.lbl.gov (128.3.112.1)  0 ms  0 ms  0 ms
 *      2  lilac-dmc.Berkeley.EDU (128.32.216.1)  19 ms  19 ms  19 ms
 *      3  lilac-dmc.Berkeley.EDU (128.32.216.1)  39 ms  19 ms  19 ms
 *      4  ccngw-ner-cc.Berkeley.EDU (128.32.136.23)  19 ms  39 ms  39 ms
 *      5  ccn-nerif22.Berkeley.EDU (128.32.168.22)  20 ms  39 ms  39 ms
 *      6  128.32.197.4 (128.32.197.4)  59 ms  119 ms  39 ms
 *      7  131.119.2.5 (131.119.2.5)  59 ms  59 ms  39 ms
 *      8  129.140.70.13 (129.140.70.13)  80 ms  79 ms  99 ms
 *      9  129.140.71.6 (129.140.71.6)  139 ms  139 ms  159 ms
 *     10  129.140.81.7 (129.140.81.7)  199 ms  180 ms  300 ms
 *     11  129.140.72.17 (129.140.72.17)  300 ms  239 ms  239 ms
 *     12  * * *
 *     13  128.121.54.72 (128.121.54.72)  259 ms  499 ms  279 ms
 *     14  * * *
 *     15  * * *
 *     16  * * *
 *     17  * * *
 *     18  ALLSPICE.LCS.MIT.EDU (18.26.0.115)  339 ms  279 ms  279 ms
 *
 * (I start to see why I'm having so much trouble with mail to
 * MIT.)  Note that the gateways 12, 14, 15, 16 & 17 hops away
 * either don't send ICMP "time exceeded" messages or send them
 * with a ttl too small to reach us.  14 - 17 are running the
 * MIT C Gateway code that doesn't send "time exceeded"s.  God
 * only knows what's going on with 12.
 *
 * The silent gateway 12 in the above may be the result of a bug in
 * the 4.[23]BSD network code (and its derivatives):  4.x (x <= 3)
 * sends an unreachable message using whatever ttl remains in the
 * original datagram.  Since, for gateways, the remaining ttl is
 * zero, the icmp "time exceeded" is guaranteed to not make it back
 * to us.  The behavior of this bug is slightly more interesting
 * when it appears on the destination system:
 *
 *      1  helios.ee.lbl.gov (128.3.112.1)  0 ms  0 ms  0 ms
 *      2  lilac-dmc.Berkeley.EDU (128.32.216.1)  39 ms  19 ms  39 ms
 *      3  lilac-dmc.Berkeley.EDU (128.32.216.1)  19 ms  39 ms  19 ms
 *      4  ccngw-ner-cc.Berkeley.EDU (128.32.136.23)  39 ms  40 ms  19 ms
 *      5  ccn-nerif35.Berkeley.EDU (128.32.168.35)  39 ms  39 ms  39 ms
 *      6  csgw.Berkeley.EDU (128.32.133.254)  39 ms  59 ms  39 ms
 *      7  * * *
 *      8  * * *
 *      9  * * *
 *     10  * * *
 *     11  * * *
 *     12  * * *
 *     13  rip.Berkeley.EDU (128.32.131.22)  59 ms !  39 ms !  39 ms !
 *
 * Notice that there are 12 "gateways" (13 is the final
 * destination) and exactly the last half of them are "missing".
 * What's really happening is that rip (a Sun-3 running Sun OS3.5)
 * is using the ttl from our arriving datagram as the ttl in its
 * icmp reply.  So, the reply will time out on the return path
 * (with no notice sent to anyone since icmp's aren't sent for
 * icmp's) until we probe with a ttl that's at least twice the path
 * length.  I.e., rip is really only 7 hops away.  A reply that
 * returns with a ttl of 1 is a clue this problem exists.
 * Traceroute prints a "!" after the time if the ttl is <= 1.
 * Since vendors ship a lot of obsolete (DEC's Ultrix, Sun 3.x) or
 * non-standard (HPUX) software, expect to see this problem
 * frequently and/or take care picking the target host of your
 * probes.
 *
 * Other possible annotations after the time are !H, !N, !P (got a host,
 * network or protocol unreachable, respectively), !S or !F (source
 * route failed or fragmentation needed -- neither of these should
 * ever occur and the associated gateway is busted if you see one).  If
 * almost all the probes result in some kind of unreachable, traceroute
 * will give up and exit.
 *
 * Notes
 * -----
 * This program must be run by root or be setuid.  (I suggest that
 * you *don't* make it setuid -- casual use could result in a lot
 * of unnecessary traffic on our poor, congested nets.)
 *
 * This program requires a kernel mod that does not appear in any
 * system available from Berkeley:  A raw ip socket using proto
 * IPPROTO_RAW must interpret the data sent as an ip datagram (as
 * opposed to data to be wrapped in a ip datagram).  See the README
 * file that came with the source to this program for a description
 * of the mods I made to /sys/netinet/raw_ip.c.  Your mileage may
 * vary.  But, again, ANY 4.x (x < 4) BSD KERNEL WILL HAVE TO BE
 * MODIFIED TO RUN THIS PROGRAM.
 *
 * The udp port usage may appear bizarre (well, ok, it is bizarre).
 * The problem is that an icmp message only contains 8 bytes of
 * data from the original datagram.  8 bytes is the size of a udp
 * header so, if we want to associate replies with the original
 * datagram, the necessary information must be encoded into the
 * udp header (the ip id could be used but there's no way to
 * interlock with the kernel's assignment of ip id's and, anyway,
 * it would have taken a lot more kernel hacking to allow this
 * code to set the ip id).  So, to allow two or more users to
 * use traceroute simultaneously, we use this task's pid as the
 * source port (the high bit is set to move the port number out
 * of the "likely" range).  To keep track of which probe is being
 * replied to (so times and/or hop counts don't get confused by a
 * reply that was delayed in transit), we increment the destination
 * port number before each probe.
 *
 * Don't use this as a coding example.  I was trying to find a
 * routing problem and this code sort-of popped out after 48 hours
 * without sleep.  I was amazed it ever compiled, much less ran.
 *
 * I stole the idea for this program from Steve Deering.  Since
 * the first release, I've learned that had I attended the right
 * IETF working group meetings, I also could have stolen it from Guy
 * Almes or Matt Mathis.  I don't know (or care) who came up with
 * the idea first.  I envy the originators' perspicacity and I'm
 * glad they didn't keep the idea a secret.
 *
 * Tim Seaver, Ken Adelman and C. Philip Wood provided bug fixes and/or
 * enhancements to the original distribution.
 *
 * I've hacked up a round-trip-route version of this that works by
 * sending a loose-source-routed udp datagram through the destination
 * back to yourself.  Unfortunately, SO many gateways botch source
 * routing, the thing is almost worthless.  Maybe one day...
 *
 *  -- Van Jacobson (van@ee.lbl.gov)
 *     Tue Dec 20 03:50:13 PST 1988
 */

#include <sys/param.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#include <sys/socket.h>
#include <sys/time.h>

/* include <linux/ipv6.h> */
/* include <linux/in6.h> */

#include "in6.h"
#include "ipv6.h"

#include <linux/icmpv6.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "ip_var.h"
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include "udp_var.h"

/* include <linux/if.h> */
#include <arpa/inet.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <memory.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gnuc.h"
#ifdef HAVE_OS_PROTO_H
#include "os-proto.h"
#endif

#include <net/if.h>

#if __linux__
#include <endian.h>
#endif



/*
 * rfc1716 
 */
#ifndef ICMP_UNREACH_FILTER_PROHIB
#define ICMP_UNREACH_FILTER_PROHIB	13      /* admin prohibited filter */
#endif
#ifndef ICMP_UNREACH_HOST_PRECEDENCE
#define ICMP_UNREACH_HOST_PRECEDENCE	14      /* host precedence violation */
#endif
#ifndef ICMP_UNREACH_PRECEDENCE_CUTOFF
#define ICMP_UNREACH_PRECEDENCE_CUTOFF	15      /* precedence cutoff */
#endif

#define MAXPACKET	65535

/*
 * Maximum number of gateways (include room for one noop) 
 */
#define NGATEWAYS ((int)((MAX_IPOPTLEN - IPOPT_MINOFF - 1) / sizeof(u_int32_t)))

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	64
#endif

#ifndef FD_SET
#define NFDBITS         (8*sizeof(fd_set))
#define FD_SETSIZE      NFDBITS
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      bzero((char *)(p), sizeof(*(p)))
#endif

#define Fprintf (void)fprintf
#define Printf (void)printf

/*
 * Host name and address list 
 */
struct hostinfo {
    char           *name;
    int             n;
    u_int32_t      *addrs;
};

/*
 * Data section of the probe packet 
 */
struct outdata {
    u_char          seq;        /* sequence number of this packet */
    u_char          ttl;        /* ttl packet left with */
    struct timeval  tv;         /* time packet left */
};

#ifndef HAVE_ICMP_NEXTMTU
/*
 * Path MTU Discovery (RFC1191) 
 */
struct my_pmtu {
    u_short         ipm_void;
    u_short         ipm_nextmtu;
};
#endif

struct ifaddrlist {
    u_int32_t       addr;
    char           *device;
};




char           *prog;

struct pkt_format {
    __u32           ident;
    __u32           seq;
    struct timeval  tv;
};


extern int      optind;
extern int      opterr;
extern char    *optarg;

/*
 * Forwards 
 */
unsigned long   deltaT(struct timeval *, struct timeval *);
unsigned long   round(double);
void            freehostinfo(struct hostinfo *);
void            getaddr(u_int32_t *, char *);
struct hostinfo *gethostinfo(char *);
u_short         in_checksum(u_short *, int);
char           *inetname(struct in_addr);
void            run_traceRoute(unsigned int, void *);
int             packet_ok(u_char *, int, struct sockaddr_in *, int,
                          u_short, int, u_short);
char           *pr_type(u_char);
 /* void print(u_char *, int, struct sockaddr_in *); */
void            send_probe(struct sockaddr_in *, int, int,
                           struct timeval *, struct ip *, struct udphdr *,
                           int, int, char *, u_short, int, u_short,
                           struct outdata *);
int             str2val(const char *, const char *, int, int);
void            tvsub(struct timeval *, struct timeval *);
int             wait_for_reply(int, struct sockaddr_in *,
                               const struct timeval *, u_char *, int);
#ifndef HAVE_USLEEP
int             usleep(u_int);
#endif
void            setsin(struct sockaddr_in *, u_int32_t);
int             ifaddrlist(struct ifaddrlist **, char *);
const char     *findsaddr(const struct sockaddr_in *,
                          struct sockaddr_in *);
struct addrinfo *host_serv_tr(const char *, const char *, int, int);
char           *sock_ntop_host_tr(const struct sockaddr *, socklen_t);

void            send_probe_v6(int, int, char *, pid_t, struct timezone *,
                              int, int, struct sockaddr_in6 *, char *);
int             wait_for_reply_v6(int, struct sockaddr_in6 *, int, int,
                                  int, u_char *);
int             packet_ok_v6(u_char *, int, struct sockaddr_in6 *, int,
                             struct timeval *, pid_t);
void            print_v6(unsigned char *, int, struct sockaddr_in6 *);
