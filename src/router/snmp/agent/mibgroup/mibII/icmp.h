/*
 *  ICMP MIB group interface - icmp.h
 *
 */
#ifndef _MIBGROUP_ICMP_H
#define _MIBGROUP_ICMP_H

config_arch_require(solaris2, kernel_sunos5)
config_arch_require(linux, mibII/kernel_linux)

#include <net-snmp/agent/cache_handler.h>

extern void     init_icmp(void);
extern Netsnmp_Node_Handler icmp_handler;
extern NetsnmpCacheLoad icmp_load;
extern NetsnmpCacheFree icmp_free;

#define ICMPINMSGS	      1
#define ICMPINERRORS	      2
#define ICMPINDESTUNREACHS    3
#define ICMPINTIMEEXCDS       4
#define ICMPINPARMPROBS       5
#define ICMPINSRCQUENCHS      6
#define ICMPINREDIRECTS       7
#define ICMPINECHOS	      8
#define ICMPINECHOREPS	      9
#define ICMPINTIMESTAMPS     10
#define ICMPINTIMESTAMPREPS  11
#define ICMPINADDRMASKS      12
#define ICMPINADDRMASKREPS   13
#define ICMPOUTMSGS	     14
#define ICMPOUTERRORS	     15
#define ICMPOUTDESTUNREACHS  16
#define ICMPOUTTIMEEXCDS     17
#define ICMPOUTPARMPROBS     18
#define ICMPOUTSRCQUENCHS    19
#define ICMPOUTREDIRECTS     20
#define ICMPOUTECHOS	     21
#define ICMPOUTECHOREPS      22
#define ICMPOUTTIMESTAMPS    23
#define ICMPOUTTIMESTAMPREPS 24
#define ICMPOUTADDRMASKS     25
#define ICMPOUTADDRMASKREPS  26

#endif                          /* _MIBGROUP_ICMP_H */
