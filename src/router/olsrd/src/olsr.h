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
 * $Id: olsr.h,v 1.21 2005/03/06 19:33:35 kattemat Exp $
 */


#ifndef _OLSR_FUNCTIONS
#define _OLSR_FUNCTIONS

#include "olsr_protocol.h"
#include "interfaces.h"

olsr_bool changes_topology;
olsr_bool changes_neighborhood;
olsr_bool changes_hna;

void
register_pcf(int (*)(int, int, int));

void
olsr_process_changes(void);

void
init_msg_seqno(void);

inline olsr_u16_t
get_msg_seqno(void);

int
olsr_forward_message(union olsr_message *, 
		     union olsr_ip_addr *, 
		     olsr_u16_t, 
		     struct interface *, 
		     union olsr_ip_addr *);

void
set_buffer_timer(struct interface *);

void
olsr_init_tables(void);

void
olsr_init_willingness(void);

void
olsr_update_willingness(void *);

olsr_u8_t
olsr_calculate_willingness(void);

void
olsr_exit(const char *, int);

void *
olsr_malloc(size_t, const char *);

int
olsr_printf(int, char *, ...);

#endif
