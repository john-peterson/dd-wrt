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

#include "tc_set.h"
#include "link_set.h"
#include "lq_plugin.h"
#include "olsr_spf.h"
#include "lq_packet.h"
#include "packet.h"
#include "olsr.h"
#include "lq_plugin_default_fpm.h"
#include "fpm.h"

/* etx lq plugin (fpm version) settings */
struct lq_handler lq_etx_fpm_handler = {
    &default_lq_initialize_fpm,
    
    &default_lq_calc_cost_fpm,
    &default_lq_calc_cost_fpm,
    
    &default_lq_is_relevant_costchange_fpm,
    
    &default_lq_packet_loss_worker_fpm,
    &default_lq_memorize_foreign_hello_fpm,
    &default_lq_copy_link2tc_fpm,
    &default_lq_clear_fpm,
    &default_lq_clear_fpm,
    
    &default_lq_serialize_hello_lq_pair_fpm,
    &default_lq_serialize_tc_lq_pair_fpm,
    &default_lq_deserialize_hello_lq_pair_fpm,
    &default_lq_deserialize_tc_lq_pair_fpm,
    
    &default_lq_print_fpm,
    &default_lq_print_fpm,
    &default_lq_print_cost_fpm, 
    
    sizeof(struct default_lq_fpm),
    sizeof(struct default_lq_fpm)
};

fpm aging_factor_new, aging_factor_old;
fpm aging_quickstart_new, aging_quickstart_old;

void default_lq_initialize_fpm(void) {
  aging_factor_new = ftofpm(olsr_cnf->lq_aging);
  aging_factor_old = fpmsub(itofpm(1), aging_factor_new);
  
  aging_quickstart_new = ftofpm(LQ_QUICKSTART_AGING);
  aging_quickstart_old = fpmsub(itofpm(1), aging_quickstart_new);
}

olsr_linkcost default_lq_calc_cost_fpm(const void *ptr) {
  const struct default_lq_fpm *lq = ptr;
  olsr_linkcost cost;
  
  if (lq->valueLq < (unsigned int)(255 * MINIMAL_USEFUL_LQ) || lq->valueNlq < (unsigned int)(255 * MINIMAL_USEFUL_LQ)) {
    return LINK_COST_BROKEN;
  }
  
  cost = fpmidiv(itofpm(255 * 255), (int)lq->valueLq * (int)lq->valueNlq);

  if (cost > LINK_COST_BROKEN)
    return LINK_COST_BROKEN;
  if (cost == 0)
    return 1;
  return cost;
}

int default_lq_serialize_hello_lq_pair_fpm(unsigned char *buff, void *ptr) {
  struct default_lq_fpm *lq = ptr;
  
  buff[0] = (unsigned char)lq->valueLq;
  buff[1] = (unsigned char)lq->valueNlq;
  buff[2] = (unsigned char)(0);
  buff[3] = (unsigned char)(0);
  
  return 4;
}

void default_lq_deserialize_hello_lq_pair_fpm(const olsr_u8_t **curr, void *ptr) {
  struct default_lq_fpm *lq = ptr;
  
  pkt_get_u8(curr, &lq->valueLq);
  pkt_get_u8(curr, &lq->valueNlq);
  pkt_ignore_u16(curr);
}

olsr_bool default_lq_is_relevant_costchange_fpm(olsr_linkcost c1, olsr_linkcost c2) {
  if (c1 > c2) {
    return c2 - c1 > LQ_PLUGIN_RELEVANT_COSTCHANGE_FPM;
  }
  return c1 - c2 > LQ_PLUGIN_RELEVANT_COSTCHANGE_FPM;
}

int default_lq_serialize_tc_lq_pair_fpm(unsigned char *buff, void *ptr) {
  struct default_lq_fpm *lq = ptr;
  
  buff[0] = (unsigned char)lq->valueLq;
  buff[1] = (unsigned char)lq->valueNlq;
  buff[2] = (unsigned char)(0);
  buff[3] = (unsigned char)(0);
  
  return 4;
}

void default_lq_deserialize_tc_lq_pair_fpm(const olsr_u8_t **curr, void *ptr) {
  struct default_lq_fpm *lq = ptr;
  
  pkt_get_u8(curr, &lq->valueLq);
  pkt_get_u8(curr, &lq->valueNlq);
  pkt_ignore_u16(curr);
}

olsr_linkcost default_lq_packet_loss_worker_fpm(struct link_entry *link, void *ptr, olsr_bool lost) {
  struct default_lq_fpm *tlq = ptr;
  fpm alpha_old = aging_factor_old;
  fpm alpha_new = aging_factor_new;
  
  fpm link_loss_factor = fpmidiv(itofpm(link->loss_link_multiplier), 65536);
  
  if (tlq->quickstart < LQ_QUICKSTART_STEPS) {
    alpha_new = aging_quickstart_new;
    alpha_old = aging_quickstart_old;
    tlq->quickstart++;
  }
  
  // exponential moving average
  tlq->valueLq = fpmmul(tlq->valueLq, alpha_old);
  if (lost == 0) {
    tlq->valueLq += fpmtoi(fpmmuli(fpmmul(alpha_new, link_loss_factor), 255));
  }
  return default_lq_calc_cost_fpm(ptr);
}

void default_lq_memorize_foreign_hello_fpm(void *ptrLocal, void *ptrForeign) {
  struct default_lq_fpm *local = ptrLocal;
  struct default_lq_fpm *foreign = ptrForeign;
  
  if (foreign) {
    local->valueNlq = foreign->valueLq;
  }
  else {
    local->valueNlq = 0;
  }
}

void default_lq_copy_link2tc_fpm(void *target, void *source) {
  memcpy(target, source, sizeof(struct default_lq_fpm));
}

void default_lq_clear_fpm(void *target) {
  memset(target, 0, sizeof(struct default_lq_fpm));
}

const char *default_lq_print_fpm(void *ptr, struct lqtextbuffer *buffer) {
  struct default_lq_fpm *lq = ptr;
  
  sprintf(buffer->buf, "%s/%s",
    fpmtoa(fpmidiv(itofpm((int)lq->valueLq), 255)),
    fpmtoa(fpmidiv(itofpm((int)lq->valueNlq), 255)));
  return buffer->buf;
}

const char *default_lq_print_cost_fpm(olsr_linkcost cost, struct lqtextbuffer *buffer) {
  sprintf(buffer->buf, "%s", fpmtoa(cost));
  return buffer->buf;
}
