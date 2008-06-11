/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2008 Henning Rogge <rogge@fgan.de>
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

#ifndef LQ_PLUGIN_DEFAULT_H_
#define LQ_PLUGIN_DEFAULT_H_

#include "olsr_types.h"
#include "lq_plugin.h"

#define LQ_ALGORITHM_ETX_FLOAT_NAME "etx_float"

#define LQ_PLUGIN_LC_MULTIPLIER 1024
#define LQ_PLUGIN_RELEVANT_COSTCHANGE 8

struct default_lq_float {
  float lq, nlq;
  olsr_u16_t quickstart;
};

void default_lq_initialize_float(void);

olsr_linkcost default_lq_calc_cost_float(const void *lq);

olsr_bool default_lq_is_relevant_costchange_float(olsr_linkcost c1, olsr_linkcost c2);

olsr_linkcost default_lq_packet_loss_worker_float(struct link_entry *link, void *lq, olsr_bool lost);
void default_lq_memorize_foreign_hello_float(void *local, void *foreign);

int default_lq_serialize_hello_lq_pair_float(unsigned char *buff, void *lq);
void default_lq_deserialize_hello_lq_pair_float(const olsr_u8_t **curr, void *lq);
int default_lq_serialize_tc_lq_pair_float(unsigned char *buff, void *lq);
void default_lq_deserialize_tc_lq_pair_float(const olsr_u8_t **curr, void *lq);

void default_lq_copy_link2tc_float(void *target, void *source);
void default_lq_clear_float(void *target);

const char *default_lq_print_float(void *ptr, struct lqtextbuffer *buffer);
const char *default_lq_print_cost_float(olsr_linkcost cost, struct lqtextbuffer *buffer);

extern struct lq_handler lq_etx_float_handler;

#endif /*LQ_PLUGIN_DEFAULT_H_*/
