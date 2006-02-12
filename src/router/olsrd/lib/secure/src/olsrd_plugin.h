
/*
 * Secure OLSR plugin
 * http://www.olsr.org
 *
 * Copyright (c) 2004, Andreas T�nnesen(andreto@olsr.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are permitted provided that the following 
 * conditions are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of olsrd, olsr.org nor the names of its 
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
 * $Id: olsrd_plugin.h,v 1.11 2005/03/10 19:57:48 kattemat Exp $
 */


/*
 * olsr.org olsr daemon security plugin
 */

#ifndef _OLSRD_PLUGIN_DEFS
#define _OLSRD_PLUGIN_DEFS


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "olsr_plugin_io.h"
#include "olsr_types.h"
#include "interfaces.h"

/* Use this as PARSER_TYPE to receive ALL messages! */
#define PROMISCUOUS 0xffffffff



#define PLUGIN_NAME    "OLSRD signature plugin"
#define PLUGIN_VERSION "0.5"
#define PLUGIN_AUTHOR   "Andreas T�nnesen"
#define MOD_DESC PLUGIN_NAME " " PLUGIN_VERSION " by " PLUGIN_AUTHOR
#define PLUGIN_INTERFACE_VERSION 2

/* The type of message you will use */
#define MESSAGE_TYPE 10

#define MAXMESSAGESIZE 512

/* The type of messages we will receive - can be set to promiscuous */
#define PARSER_TYPE MESSAGE_TYPE

#define TYPE_CHALLENGE 11
#define TYPE_CRESPONSE 12
#define TYPE_RRESPONSE 13

#define TIMED_OUT(s1) \
        timercmp(s1, now, <)

char keyfile[FILENAME_MAX];

#define MAX_TTL               0xff

/*
 *Link Types
 */

#define UNSPEC_LINK           0
#define ASYM_LINK             1
#define SYM_LINK              2
#define LOST_LINK             3
#define HIDE_LINK             4
#define MAX_LINK              4


/*
 * Mantissa scaling factor
 */

#define VTIME_SCALE_FACTOR    0.0625


/*
 * Hashing
 */

#define	HASHSIZE	32
#define	HASHMASK	(HASHSIZE - 1)

#define MAXIFS         8 /* Maximum number of interfaces (from defs.h) in uOLSRd */

/* Ifchange actions */

#define IFCHG_IF_ADD           1
#define IFCHG_IF_REMOVE        2
#define IFCHG_IF_UPDATE        3

#ifdef USE_OPENSSL
#define SIGSIZE   20
#else
#define SIGSIZE   16
#endif

/****************************************************************************
 *                            PACKET SECTION                                *
 ****************************************************************************/

struct sig_msg
{
  olsr_u8_t     type;
  olsr_u8_t     algorithm;
  olsr_u16_t    reserved;

  time_t        timestamp;
  char          signature[SIGSIZE];
};

/*
 * OLSR message (several can exist in one OLSR packet)
 */

struct olsrmsg
{
  olsr_u8_t     olsr_msgtype;
  olsr_u8_t     olsr_vtime;
  olsr_u16_t    olsr_msgsize;
  olsr_u32_t    originator;
  olsr_u8_t     ttl;
  olsr_u8_t     hopcnt;
  olsr_u16_t    seqno;

  /* YOUR PACKET GOES HERE */
  struct sig_msg sig;

};


/*
 * Challenge response messages
 */

struct challengemsg
{
  olsr_u8_t     olsr_msgtype;
  olsr_u8_t     olsr_vtime;
  olsr_u16_t    olsr_msgsize;
  olsr_u32_t    originator;
  olsr_u8_t     ttl;
  olsr_u8_t     hopcnt;
  olsr_u16_t    seqno;

  olsr_u32_t    destination;
  olsr_u32_t    challenge;

  char          signature[SIGSIZE];

};



struct c_respmsg
{
  olsr_u8_t     olsr_msgtype;
  olsr_u8_t     olsr_vtime;
  olsr_u16_t    olsr_msgsize;
  olsr_u32_t    originator;
  olsr_u8_t     ttl;
  olsr_u8_t     hopcnt;
  olsr_u16_t    seqno;

  olsr_u32_t    destination;
  olsr_u32_t    challenge;
  time_t        timestamp;

  char          res_sig[SIGSIZE];

  char          signature[SIGSIZE];

};


struct r_respmsg
{
  olsr_u8_t     olsr_msgtype;
  olsr_u8_t     olsr_vtime;
  olsr_u16_t    olsr_msgsize;
  olsr_u32_t    originator;
  olsr_u8_t     ttl;
  olsr_u8_t     hopcnt;
  olsr_u16_t    seqno;

  olsr_u32_t    destination;
  time_t        timestamp;

  char          res_sig[SIGSIZE];

  char          signature[SIGSIZE];
};


/*
 *IPv6
 */

struct olsrmsg6
{
  olsr_u8_t        olsr_msgtype;
  olsr_u8_t        olsr_vtime;
  olsr_u16_t       olsr_msgsize;
  struct in6_addr  originator;
  olsr_u8_t        ttl;
  olsr_u8_t        hopcnt;
  olsr_u16_t       seqno;

  /* YOUR PACKET GOES HERE */
  struct sig_msg   sig;
};

/*
 * Generic OLSR packet - DO NOT ALTER
 */

struct olsr 
{
  olsr_u16_t	  olsr_packlen;		/* packet length */
  olsr_u16_t	  olsr_seqno;
  struct olsrmsg  olsr_msg[1];          /* variable messages */
};


struct olsr6
{
  olsr_u16_t	    olsr_packlen;        /* packet length */
  olsr_u16_t	    olsr_seqno;
  struct olsrmsg6   olsr_msg[1];         /* variable messages */
};


/* 
 * ALWAYS USE THESE WRAPPERS TO
 * ENSURE IPv4 <-> IPv6 compability 
 */

union olsr_message
{
  struct olsrmsg v4;
  struct olsrmsg6 v6;
};

union olsr_packet
{
  struct olsr v4;
  struct olsr6 v6;
};


/***************************************************************************
 *                 Functions provided by uolsrd_plugin.c                   *
 *                  Similar to their siblings in olsrd                     *
 ***************************************************************************/

char ipv6_buf[100]; /* buffer for IPv6 inet_htop */

/* All these could optionally be fetched from olsrd */

olsr_u32_t
olsr_hashing(union olsr_ip_addr *);

void
olsr_get_timestamp(olsr_u32_t, struct timeval *);

void
olsr_init_timer(olsr_u32_t, struct timeval *);

int
olsr_timed_out(struct timeval *);

char *
olsr_ip_to_string(union olsr_ip_addr *);



/****************************************************************************
 *                Function pointers to functions in olsrd                   *
 *              These allow direct access to olsrd functions                *
 ****************************************************************************/

/* The multi-purpose funtion. All other functions are fetched trough this */
int (*olsr_plugin_io)(int, void *, size_t);

/* add a prser function */
void (*olsr_parser_add_function)(void (*)(union olsr_message *, struct interface *, union olsr_ip_addr *), 
				 int, int);

/* Register a timeout function */
int (*olsr_register_timeout_function)(void (*)(void));

/* Register a scheduled event */
int (*olsr_register_scheduler_event)(void (*)(void), void *, float, float, olsr_u8_t *);

/* Get the next message seqno in line */
olsr_u16_t (*get_msg_seqno)(void);

/* Transmit package */
int (*net_output)(struct interface*);

/* Check the duplicate table for prior processing */
int (*check_dup_proc)(union olsr_ip_addr *, olsr_u16_t);

/* Default forward algorithm */
int (*default_fwd)(union olsr_message *, 
		   union olsr_ip_addr *, 
		   olsr_u16_t,  
		   struct interface *, 
		   union olsr_ip_addr *);

/* Add a socket to the main olsrd select loop */
void (*add_olsr_socket)(int, void(*)(int));

/* Remove a socket from the main olsrd select loop */
int (*remove_olsr_socket)(int, void(*)(int));

/* get the link status to a neighbor */
int (*check_neighbor_link)(union olsr_ip_addr *);

/* Mantissa/exponen conversions */
olsr_u8_t (*double_to_me)(double);

double (*me_to_double)(olsr_u8_t);

/* olsrd printf wrapper */
int (*olsr_printf)(int, char *, ...);

/* olsrd malloc wrapper */
void *(*olsr_malloc)(size_t, const char *);

/* Add a packet transform function */
int (*add_ptf)(int(*)(char *, int *));

/* Remove a packet transform function */
int (*del_ptf)(int(*)(char *, int *));

/* Socket input function */
void (*olsr_input)(int);

/* Packet parser function */
void (*parse_packet)(struct olsr *, int, struct interface *, union olsr_ip_addr *);

/* Map interface by socket */
struct interface * (*if_ifwithsock)(int);

/* Map interface by address */
struct interface * (*if_ifwithaddr)(union olsr_ip_addr *);

/* Add an ifchange function */
int (*add_ifchgf)(int(*)(struct interface *, int));

/* Remove an ifchange function */
int (*del_ifchgf)(int(*)(struct interface *, int));

int (*net_reserve_bufspace)(struct interface *, int);

int (*net_outbuffer_push_reserved)(struct interface *, olsr_u8_t *, olsr_u16_t);

int (*net_outbuffer_push)(struct interface *, olsr_u8_t *, olsr_u16_t);


/****************************************************************************
 *                             Data from olsrd                              *
 *           NOTE THAT POINTERS POINT TO THE DATA USED BY OLSRD!            *
 *               NEVER ALTER DATA POINTED TO BY THESE POINTERS              * 
 *                   UNLESS YOU KNOW WHAT YOU ARE DOING!!!                  *
 ****************************************************************************/
/**
 * The interface list from olsrd
 */

struct interface   *ifs;

/* These two are set automatically by olsrd at load time */
int                ipversion;  /* IPversion in use */
union olsr_ip_addr *main_addr; /* Main address */


size_t             ipsize;     /* Size of the ipadresses used */
struct timeval     *now;       /* the olsrds schedulers idea of current time */


/****************************************************************************
 *                Functions that the plugin MUST provide                    *
 ****************************************************************************/


/* Initialization function */
int
olsr_plugin_init(void);

/* IPC initialization function */
int
plugin_ipc_init(void);

int
register_olsr_param(char *, char *);

/* Destructor function */
void
olsr_plugin_exit(void);

/* Mulitpurpose funtion */
int
plugin_io(int, void *, size_t);

int 
get_plugin_interface_version(void);

#endif
