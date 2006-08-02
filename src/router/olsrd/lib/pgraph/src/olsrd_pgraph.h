/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas T�nnesen(andreto@olsr.org)
 *                     includes code by Bruno Randolf
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
 * $Id: olsrd_pgraph.h,v 1.1 2005/07/13 21:45:08 kattemat Exp $
 */

/*
 * Dynamic linked library for the olsr.org olsr daemon
 */

#ifndef _OLSRD_PGRAPH_PLUGIN
#define _OLSRD_PGRAPH_PLUGIN

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "olsrd_plugin.h"
#include "olsr_types.h"

#include "olsr.h"
#include "neighbor_table.h"
#include "two_hop_neighbor_table.h"
#include "tc_set.h"
#include "hna_set.h"
#include "mid_set.h"
#include "link_set.h"

extern struct in_addr ipc_accept_ip;
extern int ipc_port;

char netmask[5];

/* Event function to register with the sceduler */
int
pcf_event(int, int, int);

void
ipc_action(int);

static void inline
ipc_print_neigh_link(struct neighbor_entry *neighbor);

static void inline
ipc_print_tc_link(struct tc_entry *entry, struct topo_dst *dst_entry);

static void inline
ipc_print_net(union olsr_ip_addr *, union olsr_ip_addr *, union hna_netmask *);

int
ipc_send(char *, int);

char *
olsr_netmask_to_string(union hna_netmask *);

struct link_entry *olsr_neighbor_best_link(union olsr_ip_addr *main);

#endif
