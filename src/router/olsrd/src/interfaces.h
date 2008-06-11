/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas Tønnesen(andreto@olsr.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 */


#ifndef _OLSR_INTERFACE
#define _OLSR_INTERFACE

#include <sys/types.h>
#ifdef _MSC_VER
#include <WS2tcpip.h>
#undef interface
#else
#include <sys/socket.h>
#endif
#include <time.h>

#include "olsr_types.h"
#include "mantissa.h"

#define _PATH_PROCNET_IFINET6           "/proc/net/if_inet6"


#define IPV6_ADDR_ANY		0x0000U

#define IPV6_ADDR_UNICAST      	0x0001U
#define IPV6_ADDR_MULTICAST    	0x0002U
#define IPV6_ADDR_ANYCAST	0x0004U

#define IPV6_ADDR_LOOPBACK	0x0010U
#define IPV6_ADDR_LINKLOCAL	0x0020U
#define IPV6_ADDR_SITELOCAL	0x0040U

#define IPV6_ADDR_COMPATv4	0x0080U

#define IPV6_ADDR_SCOPE_MASK	0x00f0U

#define IPV6_ADDR_MAPPED	0x1000U
#define IPV6_ADDR_RESERVED	0x2000U


#define MAX_IF_METRIC           100

#define WEIGHT_LOWEST           0       /* No weight            */
#define WEIGHT_LOW              1       /* Low                  */
#define WEIGHT_ETHERNET_1GBP    2       /* Ethernet 1Gb+        */
#define WEIGHT_ETHERNET_1GB     4       /* Ethernet 1Gb         */
#define WEIGHT_ETHERNET_100MB   8       /* Ethernet 100Mb       */
#define WEIGHT_ETHERNET_10MB    16      /* Ethernet 10Mb        */
#define WEIGHT_ETHERNET_DEFAULT 32      /* Ethernet unknown rate*/
#define WEIGHT_WLAN_HIGH        64      /* >54Mb WLAN           */
#define WEIGHT_WLAN_54MB        128     /* 54Mb 802.11g         */
#define WEIGHT_WLAN_11MB        256     /* 11Mb 802.11b         */
#define WEIGHT_WLAN_LOW         512     /* <11Mb WLAN           */
#define WEIGHT_WLAN_DEFAULT     1024    /* WLAN unknown rate    */
#define WEIGHT_SERIAL           2048    /* Serial device        */
#define WEIGHT_HIGH             4096    /* High                 */
#define WEIGHT_HIGHEST          8192    /* Really high          */

struct if_gen_property
{
  olsr_u32_t             owner_id;
  void                   *data;
  struct if_gen_property *next;
};

struct vtimes
{
  olsr_u8_t hello;
  olsr_u8_t tc;
  olsr_u8_t mid;
  olsr_u8_t hna;
};

/* Output buffer structure. This should actually be in net_olsr.h but we have circular references then.
 */
struct olsr_netbuf
{
  olsr_u8_t *buff;     /* Pointer to the allocated buffer */
  int bufsize;    /* Size of the buffer */
  int maxsize;    /* Max bytes of payload that can be added to the buffer */
  int pending;    /* How much data is currently pending in the buffer */
  int reserved;   /* Plugins can reserve space in buffers */
};

/**
 *A struct containing all necessary information about each
 *interface participating in the OLSRD routing
 */
struct interface 
{
  /* IP version 4 */
  struct	sockaddr_in int_addr;		/* address */
  struct	sockaddr_in int_netmask;		/* netmask */
  struct	sockaddr_in int_broadaddr;         /* broadcast address */
  /* IP version 6 */
  struct        sockaddr_in6 int6_addr;         /* Address */
  struct        sockaddr_in6 int6_multaddr;     /* Multicast */
  /* IP independent */
  union         olsr_ip_addr ip_addr;
  int           is_hcif;                        /* Is this a emulated host-client if? */
  
  int           olsr_socket;        /* The broadcast socket for this interface */
  
  int	          int_metric;			/* metric of interface */
  int           int_mtu;        /* MTU of interface */
  int	          int_flags;			/* see below */
  int           if_index;       /* Kernels index of this interface */
  int           is_wireless;    /* wireless interface or not*/
  char	        *int_name;			/* from kernel if structure */
  olsr_u16_t    olsr_seqnum;    /* Olsr message seqno */

  /* Periodic message generation timers */
  struct timer_entry *hello_gen_timer;
  struct timer_entry *hna_gen_timer;
  struct timer_entry *mid_gen_timer;
  struct timer_entry *tc_gen_timer;

#ifdef linux
/* Struct used to store original redirect/ingress setting */
  struct nic_state
  {
    char redirect; /* The original state of icmp redirect */
    char spoof; /* The original state of the IP spoof filter */
  } nic_state;
#endif

  olsr_reltime  hello_etime;
  struct        vtimes valtimes;

  clock_t       fwdtimer;                       /* Timeout for OLSR forwarding on this if */

  struct olsr_netbuf netbuf;                    /* the buffer to construct the packet data */

  struct        if_gen_property *gen_properties;/* Generic interface properties */
  
  int           ttl_index; /* index in TTL array for fish-eye */
  
  olsr_bool	immediate_send_tc; /* Hello's are sent immediately normally, this flag prefers to send TC's */

  struct	interface *int_next;
};


#define OLSR_DEFAULT_MTU             1500

/* Ifchange actions */

#define IFCHG_IF_ADD           1
#define IFCHG_IF_REMOVE        2
#define IFCHG_IF_UPDATE        3

/* The interface linked-list */
extern struct interface *ifnet;

int
ifinit(void);

void
run_ifchg_cbs(struct interface *, int);

struct interface *
if_ifwithsock(int);

struct interface *
if_ifwithaddr(const union olsr_ip_addr *);

struct interface *
if_ifwithname(const char *);

const char *
if_ifwithindex_name(const int if_index);

struct interface *
if_ifwithindex(const int if_index);

struct olsr_if *
queue_if(const char *, int);

int
add_ifchgf(int (*f)(struct interface *, int));

int
del_ifchgf(int (*f)(struct interface *, int));

#endif

/*
 * Local Variables:
 * c-basic-offset: 2
 * End:
 */
