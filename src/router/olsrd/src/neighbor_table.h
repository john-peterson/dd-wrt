/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas T�nnesen(andreto@olsr.org)
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
 * $Id: neighbor_table.h,v 1.13 2005/02/20 18:52:18 kattemat Exp $
 */


#ifndef _OLSR_NEIGH_TBL
#define _OLSR_NEIGH_TBL

#include "olsr_types.h"
#include "hashing.h"


struct neighbor_2_list_entry 
{
  struct neighbor_2_entry      *neighbor_2;
  clock_t       	       neighbor_2_timer;
  struct neighbor_2_list_entry *next;
  struct neighbor_2_list_entry *prev;
};



struct neighbor_entry
{
  union olsr_ip_addr           neighbor_main_addr;
  olsr_u8_t                    status;
  olsr_u8_t                    willingness;
  olsr_bool                    is_mpr;
  olsr_bool                    was_mpr; /* Used to detect changes in MPR */
  olsr_bool                    skip;
  int                          neighbor_2_nocov;
  int                          linkcount;
  struct neighbor_2_list_entry neighbor_2_list; 
  struct neighbor_entry        *next;
  struct neighbor_entry        *prev;
};


#ifndef OLSR_PLUGIN

/*
 * The neighbor table
 */

struct neighbor_entry neighbortable[HASHSIZE];

void
olsr_init_neighbor_table(void);

int
olsr_delete_neighbor_2_pointer(struct neighbor_entry *, union olsr_ip_addr *);

struct neighbor_2_list_entry *
olsr_lookup_my_neighbors(struct neighbor_entry *, union olsr_ip_addr *);

int
olsr_delete_neighbor_table(union olsr_ip_addr *);

struct neighbor_entry *
olsr_insert_neighbor_table(union olsr_ip_addr *);

struct neighbor_entry *
olsr_lookup_neighbor_table(union olsr_ip_addr *);

struct neighbor_entry *
olsr_lookup_neighbor_table_alias(union olsr_ip_addr *);

void
olsr_time_out_two_hop_neighbors(struct neighbor_entry  *);

void
olsr_time_out_neighborhood_tables(void);

void
olsr_print_neighbor_table(void);


int
update_neighbor_status(struct neighbor_entry *, int);

#endif
#endif
