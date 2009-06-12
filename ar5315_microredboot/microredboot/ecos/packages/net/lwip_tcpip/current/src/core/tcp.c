/**
 * @file
 *
 * Transmission Control Protocol for IP
 *
 * This file contains common functions for the TCP implementation, such as functinos
 * for manipulating the data structures and the TCP timer functions. TCP functions
 * related to input and output is found in tcp_in.c and tcp_out.c respectively.
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"

#include "lwip/tcp.h"
#if LWIP_TCP

/* Incremented every coarse grained timer shot
   (typically every 500 ms, determined by TCP_COARSE_TIMEOUT). */
u32_t tcp_ticks;
const u8_t tcp_backoff[13] =
    { 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7};

/* The TCP PCB lists. */

/** List of all TCP PCBs in LISTEN state */
union tcp_listen_pcbs_t tcp_listen_pcbs;
/** List of all TCP PCBs that are in a state in which
 * they accept or send data. */
struct tcp_pcb *tcp_active_pcbs;  
/** List of all TCP PCBs in TIME-WAIT state */
struct tcp_pcb *tcp_tw_pcbs;

struct tcp_pcb *tcp_tmp_pcb;

static u8_t tcp_timer;
static u16_t tcp_new_port(void);

/**
 * Initializes the TCP layer.
 */
void
tcp_init(void)
{
  /* Clear globals. */
  tcp_listen_pcbs.listen_pcbs = NULL;
  tcp_active_pcbs = NULL;
  tcp_tw_pcbs = NULL;
  tcp_tmp_pcb = NULL;
  
  /* initialize timer */
  tcp_ticks = 0;
  tcp_timer = 0;
  
}

/**
 * Called periodically to dispatch TCP timers.
 *
 */
void
tcp_tmr(void)
{
  /* Call tcp_fasttmr() every 250 ms */
  tcp_fasttmr();

  if (++tcp_timer & 1) {
    /* Call tcp_tmr() every 500 ms, i.e., every other timer
       tcp_tmr() is called. */
    tcp_slowtmr();
  }
}

/**
 * Closes the connection held by the PCB.
 *
 */
err_t
tcp_close(struct tcp_pcb *pcb)
{
  err_t err;

#if TCP_DEBUG
  LWIP_DEBUGF(TCP_DEBUG, ("tcp_close: closing in state "));
  tcp_debug_print_state(pcb->state);
  LWIP_DEBUGF(TCP_DEBUG, ("\n"));
#endif /* TCP_DEBUG */
  switch (pcb->state) {
  case LISTEN:
    err = ERR_OK;
    tcp_pcb_remove((struct tcp_pcb **)&tcp_listen_pcbs.pcbs, pcb);
    memp_free(MEMP_TCP_PCB_LISTEN, pcb);
    pcb = NULL;
    break;
  case SYN_SENT:
    err = ERR_OK;
    tcp_pcb_remove(&tcp_active_pcbs, pcb);
    memp_free(MEMP_TCP_PCB, pcb);
    pcb = NULL;
    break;
  case SYN_RCVD:
  case ESTABLISHED:
    err = tcp_send_ctrl(pcb, TCP_FIN);
    if (err == ERR_OK) {
      pcb->state = FIN_WAIT_1;
    }
    break;
  case CLOSE_WAIT:
    err = tcp_send_ctrl(pcb, TCP_FIN);
    if (err == ERR_OK) {
      pcb->state = LAST_ACK;
    }
    break;
  default:
    /* Has already been closed, do nothing. */
    err = ERR_OK;
    pcb = NULL;
    break;
  }

  if (pcb != NULL && err == ERR_OK) {
    err = tcp_output(pcb);
  }
  return err;
}

/**
 * Aborts a connection by sending a RST to the remote host and deletes
 * the local protocol control block. This is done when a connection is
 * killed because of shortage of memory.
 *
 */
void
tcp_abort(struct tcp_pcb *pcb)
{
  u32_t seqno, ackno;
  u16_t remote_port, local_port;
  struct ip_addr remote_ip, local_ip;
#if LWIP_CALLBACK_API  
  void (* errf)(void *arg, err_t err);
#endif /* LWIP_CALLBACK_API */
  void *errf_arg;

  
  /* Figure out on which TCP PCB list we are, and remove us. If we
     are in an active state, call the receive function associated with
     the PCB with a NULL argument, and send an RST to the remote end. */
  if (pcb->state == TIME_WAIT) {
    tcp_pcb_remove(&tcp_tw_pcbs, pcb);
    memp_free(MEMP_TCP_PCB, pcb);
  } else {
    seqno = pcb->snd_nxt;
    ackno = pcb->rcv_nxt;
    ip_addr_set(&local_ip, &(pcb->local_ip));
    ip_addr_set(&remote_ip, &(pcb->remote_ip));
    local_port = pcb->local_port;
    remote_port = pcb->remote_port;
#if LWIP_CALLBACK_API
    errf = pcb->errf;
#endif /* LWIP_CALLBACK_API */
    errf_arg = pcb->callback_arg;
    tcp_pcb_remove(&tcp_active_pcbs, pcb);
    if (pcb->unacked != NULL) {
      tcp_segs_free(pcb->unacked);
    }
    if (pcb->unsent != NULL) {
      tcp_segs_free(pcb->unsent);
    }
#if TCP_QUEUE_OOSEQ    
    if (pcb->ooseq != NULL) {
      tcp_segs_free(pcb->ooseq);
    }
#endif /* TCP_QUEUE_OOSEQ */
    memp_free(MEMP_TCP_PCB, pcb);
    TCP_EVENT_ERR(errf, errf_arg, ERR_ABRT);
    LWIP_DEBUGF(TCP_RST_DEBUG, ("tcp_abort: sending RST\n"));
    tcp_rst(seqno, ackno, &local_ip, &remote_ip, local_port, remote_port);
  }
}

/**
 * Binds the connection to a local portnumber and IP address. If the
 * IP address is not given (i.e., ipaddr == NULL), the IP address of
 * the outgoing network interface is used instead.
 *
 */

err_t
tcp_bind(struct tcp_pcb *pcb, struct ip_addr *ipaddr, u16_t port)
{
  struct tcp_pcb *cpcb;
#if SO_REUSE
  int reuse_port_all_set = 1;
#endif /* SO_REUSE */

  if (port == 0) {
    port = tcp_new_port();
  }
#if SO_REUSE == 0
  /* Check if the address already is in use. */
  for(cpcb = (struct tcp_pcb *)tcp_listen_pcbs.pcbs;
      cpcb != NULL; cpcb = cpcb->next) {
    if (cpcb->local_port == port) {
      if (ip_addr_isany(&(cpcb->local_ip)) ||
        ip_addr_isany(ipaddr) ||
        ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
          return ERR_USE;
      }
    }
  }
  for(cpcb = tcp_active_pcbs;
      cpcb != NULL; cpcb = cpcb->next) {
    if (cpcb->local_port == port) {
      if (ip_addr_isany(&(cpcb->local_ip)) ||
   ip_addr_isany(ipaddr) ||
   ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
  return ERR_USE;
      }
    }
  }
#else /* SO_REUSE */
  /* Search through list of PCB's in LISTEN state. 
     
  If there is a PCB bound to specified port and IP_ADDR_ANY another PCB can be bound to the interface IP
  or to the loopback address on the same port if SOF_REUSEADDR is set. Any combination of PCB's bound to 
  the same local port, but to one address out of {IP_ADDR_ANY, 127.0.0.1, interface IP} at a time is valid.
  But no two PCB's bound to same local port and same local address is valid.
  
  If SOF_REUSEPORT is set several PCB's can be bound to same local port and same local address also. But then 
  all PCB's must have the SOF_REUSEPORT option set.
  
  When the two options aren't set and specified port is already bound, ERR_USE is returned saying that 
  address is already in use. */
  for(cpcb = (struct tcp_pcb *)tcp_listen_pcbs.pcbs; cpcb != NULL; cpcb = cpcb->next) {
    if(cpcb->local_port == port) {
      if(ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
        if(pcb->so_options & SOF_REUSEPORT) {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in listening PCB's: SO_REUSEPORT set and same address.\n"));
          reuse_port_all_set = (reuse_port_all_set && (cpcb->so_options & SOF_REUSEPORT));
        }
        else {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in listening PCB's: SO_REUSEPORT not set and same address.\n"));
          return ERR_USE;
        }
      }
      else if((ip_addr_isany(ipaddr) && !ip_addr_isany(&(cpcb->local_ip))) ||
              (!ip_addr_isany(ipaddr) && ip_addr_isany(&(cpcb->local_ip)))) {
        if(!(pcb->so_options & SOF_REUSEADDR) && !(pcb->so_options & SOF_REUSEPORT)) {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in listening PCB's SO_REUSEPORT or SO_REUSEADDR not set and not the same address.\n"));
          return ERR_USE;
        }      
        else {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in listening PCB's SO_REUSEPORT or SO_REUSEADDR set and not the same address.\n"));
        }     
      }
    }
  }

  /* Search through list of PCB's in a state in which they can accept or send data. Same decription as for 
     PCB's in state LISTEN applies to this PCB's regarding the options SOF_REUSEADDR and SOF_REUSEPORT. */
  for(cpcb = tcp_active_pcbs; cpcb != NULL; cpcb = cpcb->next) {
    if(cpcb->local_port == port) {
      if(ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
        if(pcb->so_options & SOF_REUSEPORT) {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in active PCB's SO_REUSEPORT set and same address.\n"));
          reuse_port_all_set = (reuse_port_all_set && (cpcb->so_options & SOF_REUSEPORT));
        }
        else {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in active PCB's SO_REUSEPORT not set and same address.\n"));
          return ERR_USE;
        }
      }
      else if((ip_addr_isany(ipaddr) && !ip_addr_isany(&(cpcb->local_ip))) ||
              (!ip_addr_isany(ipaddr) && ip_addr_isany(&(cpcb->local_ip)))) {
        if(!(pcb->so_options & SOF_REUSEADDR) && !(pcb->so_options & SOF_REUSEPORT)) {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in active PCB's SO_REUSEPORT or SO_REUSEADDR not set and not the same address.\n"));
          return ERR_USE;
        }   
        else {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in active PCB's SO_REUSEPORT or SO_REUSEADDR set and not the same address.\n"));
        }        
      }
    }
  }

  /* Search through list of PCB's in TIME_WAIT state. If SO_REUSEADDR is set a bound combination [IP, port} 
     can be rebound. The same applies when SOF_REUSEPORT is set. 
     
     If SOF_REUSEPORT is set several PCB's can be bound to same local port and same local address also. But then 
     all PCB's must have the SOF_REUSEPORT option set.
     
     When the two options aren't set and specified port is already bound, ERR_USE is returned saying that 
     address is already in use. */
  for(cpcb = tcp_tw_pcbs; cpcb != NULL; cpcb = cpcb->next) {
    if(cpcb->local_port == port) {
      if(ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
        if(!(pcb->so_options & SOF_REUSEADDR) && !(pcb->so_options & SOF_REUSEPORT)) {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in TIME_WAIT PCB's SO_REUSEPORT or SO_REUSEADDR not set and same address.\n"));
          return ERR_USE;
        }
        else if(pcb->so_options & SOF_REUSEPORT) {
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: in TIME_WAIT PCB's SO_REUSEPORT set and same address.\n"));
          reuse_port_all_set = (reuse_port_all_set && (cpcb->so_options & SOF_REUSEPORT));
        }
      }
    }
  }

  /* If SOF_REUSEPORT isn't set in all PCB's bound to specified port and local address specified then 
     {IP, port} can't be reused. */
  if(!reuse_port_all_set) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: not all sockets have SO_REUSEPORT set.\n"));
    return ERR_USE;
  }
#endif /* SO_REUSE */

  if (!ip_addr_isany(ipaddr)) {
    pcb->local_ip = *ipaddr;
  }
  pcb->local_port = port;
  LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: bind to port %u\n", port));
  return ERR_OK;
}
#if LWIP_CALLBACK_API
static err_t
tcp_accept_null(void *arg, struct tcp_pcb *pcb, err_t err)
{
  (void)arg;
  (void)pcb;
  (void)err;

  return ERR_ABRT;
}
#endif /* LWIP_CALLBACK_API */

/**
 * Set the state of the connection to be LISTEN, which means that it
 * is able to accept incoming connections. The protocol control block
 * is reallocated in order to consume less memory. Setting the
 * connection to LISTEN is an irreversible process.
 *
 */
struct tcp_pcb *
tcp_listen(struct tcp_pcb *pcb)
{
  struct tcp_pcb_listen *lpcb;

  /* already listening? */
  if (pcb->state == LISTEN) {
    return pcb;
  }
  lpcb = memp_malloc(MEMP_TCP_PCB_LISTEN);
  if (lpcb == NULL) {
    return NULL;
  }
  lpcb->callback_arg = pcb->callback_arg;
  lpcb->local_port = pcb->local_port;
  lpcb->state = LISTEN;
  lpcb->so_options = pcb->so_options;
  lpcb->so_options |= SOF_ACCEPTCONN;
  lpcb->ttl = pcb->ttl;
  lpcb->tos = pcb->tos;
  ip_addr_set(&lpcb->local_ip, &pcb->local_ip);
  memp_free(MEMP_TCP_PCB, pcb);
#if LWIP_CALLBACK_API
  lpcb->accept = tcp_accept_null;
#endif /* LWIP_CALLBACK_API */
  TCP_REG(&tcp_listen_pcbs.listen_pcbs, lpcb);
  return (struct tcp_pcb *)lpcb;
}

/**
 * This function should be called by the application when it has
 * processed the data. The purpose is to advertise a larger window
 * when the data has been processed.
 *
 */
void
tcp_recved(struct tcp_pcb *pcb, u16_t len)
{
  if ((u32_t)pcb->rcv_wnd + len > TCP_WND) {
    pcb->rcv_wnd = TCP_WND;
  } else {
    pcb->rcv_wnd += len;
  }
  if (!(pcb->flags & TF_ACK_DELAY) &&
     !(pcb->flags & TF_ACK_NOW)) {
    /*
     * We send an ACK here (if one is not already pending, hence
     * the above tests) as tcp_recved() implies that the application
     * has processed some data, and so we can open the receiver's
     * window to allow more to be transmitted.  This could result in
     * two ACKs being sent for each received packet in some limited cases
     * (where the application is only receiving data, and is slow to
     * process it) but it is necessary to guarantee that the sender can
     * continue to transmit.
     */
    tcp_ack(pcb);
  }

  LWIP_DEBUGF(TCP_DEBUG, ("tcp_recved: recveived %u bytes, wnd %u (%u).\n",
         len, pcb->rcv_wnd, TCP_WND - pcb->rcv_wnd));
}

/**
 * A nastly hack featuring 'goto' statements that allocates a
 * new TCP local port.
 */
static u16_t
tcp_new_port(void)
{
  struct tcp_pcb *pcb;
#ifndef TCP_LOCAL_PORT_RANGE_START
#define TCP_LOCAL_PORT_RANGE_START 4096
#define TCP_LOCAL_PORT_RANGE_END   0x7fff
#endif
  static u16_t port = TCP_LOCAL_PORT_RANGE_START;
  
 again:
  if (++port > TCP_LOCAL_PORT_RANGE_END) {
    port = TCP_LOCAL_PORT_RANGE_START;
  }
  
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->local_port == port) {
      goto again;
    }
  }
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->local_port == port) {
      goto again;
    }
  }
  for(pcb = (struct tcp_pcb *)tcp_listen_pcbs.pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->local_port == port) {
      goto again;
    }
  }
  return port;
}

/**
 * Connects to another host. The function given as the "connected"
 * argument will be called when the connection has been established.
 *
 */
err_t
tcp_connect(struct tcp_pcb *pcb, struct ip_addr *ipaddr, u16_t port,
      err_t (* connected)(void *arg, struct tcp_pcb *tpcb, err_t err))
{
  u32_t optdata;
  err_t ret;
  u32_t iss;

  LWIP_DEBUGF(TCP_DEBUG, ("tcp_connect to port %u\n", port));
  if (ipaddr != NULL) {
    pcb->remote_ip = *ipaddr;
  } else {
    return ERR_VAL;
  }
  pcb->remote_port = port;
  if (pcb->local_port == 0) {
    pcb->local_port = tcp_new_port();
  }
  iss = tcp_next_iss();
  pcb->rcv_nxt = 0;
  pcb->snd_nxt = iss;
  pcb->lastack = iss - 1;
  pcb->snd_lbb = iss - 1;
  pcb->rcv_wnd = TCP_WND;
  pcb->snd_wnd = TCP_WND;
  pcb->mss = TCP_MSS;
  pcb->cwnd = 1;
  pcb->ssthresh = pcb->mss * 10;
  pcb->state = SYN_SENT;
#if LWIP_CALLBACK_API  
  pcb->connected = connected;
#endif /* LWIP_CALLBACK_API */  
  TCP_REG(&tcp_active_pcbs, pcb);
  
  /* Build an MSS option */
  optdata = htonl(((u32_t)2 << 24) | 
      ((u32_t)4 << 16) | 
      (((u32_t)pcb->mss / 256) << 8) |
      (pcb->mss & 255));

  ret = tcp_enqueue(pcb, NULL, 0, TCP_SYN, 0, (u8_t *)&optdata, 4);
  if (ret == ERR_OK) { 
    tcp_output(pcb);
  }
  return ret;
} 

/**
 * Called every 500 ms and implements the retransmission timer and the timer that
 * removes PCBs that have been in TIME-WAIT for enough time. It also increments
 * various timers such as the inactivity timer in each PCB.
 */
void
tcp_slowtmr(void)
{
  struct tcp_pcb *pcb, *pcb2, *prev;
  u32_t eff_wnd;
  u8_t pcb_remove;      /* flag if a PCB should be removed */
  err_t err;

  err = ERR_OK;

  ++tcp_ticks;

  /* Steps through all of the active PCBs. */
  prev = NULL;
  pcb = tcp_active_pcbs;
  if (pcb == NULL) LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: no active pcbs\n"));
  while (pcb != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: processing active pcb\n"));
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != CLOSED\n", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != LISTEN\n", pcb->state != LISTEN);
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != TIME-WAIT\n", pcb->state != TIME_WAIT);

    pcb_remove = 0;

    if (pcb->state == SYN_SENT && pcb->nrtx == TCP_SYNMAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max SYN retries reached\n"));
    }
    else if (pcb->nrtx == TCP_MAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max DATA retries reached\n"));
    } else {
      ++pcb->rtime;
      if (pcb->unacked != NULL && pcb->rtime >= pcb->rto) {

        /* Time for a retransmission. */
        LWIP_DEBUGF(TCP_RTO_DEBUG, ("tcp_slowtmr: rtime %u pcb->rto %u\n",
          pcb->rtime, pcb->rto));

        /* Double retransmission time-out unless we are trying to
         * connect to somebody (i.e., we are in SYN_SENT). */
        if (pcb->state != SYN_SENT) {
          pcb->rto = ((pcb->sa >> 3) + pcb->sv) << tcp_backoff[pcb->nrtx];
        }
        tcp_rexmit(pcb);
        /* Reduce congestion window and ssthresh. */
        eff_wnd = LWIP_MIN(pcb->cwnd, pcb->snd_wnd);
        pcb->ssthresh = eff_wnd >> 1;
        if (pcb->ssthresh < pcb->mss) {
          pcb->ssthresh = pcb->mss * 2;
        }
        pcb->cwnd = pcb->mss;
        LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_slowtmr: cwnd %u ssthresh %u\n",
                                pcb->cwnd, pcb->ssthresh));
      }
    }
    /* Check if this PCB has stayed too long in FIN-WAIT-2 */
    if (pcb->state == FIN_WAIT_2) {
      if ((u32_t)(tcp_ticks - pcb->tmr) >
        TCP_FIN_WAIT_TIMEOUT / TCP_SLOW_INTERVAL) {
        ++pcb_remove;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in FIN-WAIT-2\n"));
      }
    }

   /* Check if KEEPALIVE should be sent */
   if((pcb->so_options & SOF_KEEPALIVE) && ((pcb->state == ESTABLISHED) || (pcb->state == CLOSE_WAIT))) {
      if((u32_t)(tcp_ticks - pcb->tmr) > (pcb->keepalive + TCP_MAXIDLE) / TCP_SLOW_INTERVAL)  {
         LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: KEEPALIVE timeout. Aborting connection to %u.%u.%u.%u.\n",
                                 ip4_addr1(&pcb->remote_ip), ip4_addr2(&pcb->remote_ip),
                                 ip4_addr3(&pcb->remote_ip), ip4_addr4(&pcb->remote_ip)));

         tcp_abort(pcb);
      }
      else if((u32_t)(tcp_ticks - pcb->tmr) > (pcb->keepalive + pcb->keep_cnt * TCP_KEEPINTVL) / TCP_SLOW_INTERVAL) {
         tcp_keepalive(pcb);
         pcb->keep_cnt++;
      }
   }

    /* If this PCB has queued out of sequence data, but has been
       inactive for too long, will drop the data (it will eventually
       be retransmitted). */
#if TCP_QUEUE_OOSEQ    
    if (pcb->ooseq != NULL &&
       (u32_t)tcp_ticks - pcb->tmr >=
       pcb->rto * TCP_OOSEQ_TIMEOUT) {
      tcp_segs_free(pcb->ooseq);
      pcb->ooseq = NULL;
      LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_slowtmr: dropping OOSEQ queued data\n"));
    }
#endif /* TCP_QUEUE_OOSEQ */

    /* Check if this PCB has stayed too long in SYN-RCVD */
    if (pcb->state == SYN_RCVD) {
      if ((u32_t)(tcp_ticks - pcb->tmr) >
   TCP_SYN_RCVD_TIMEOUT / TCP_SLOW_INTERVAL) {
        ++pcb_remove;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in SYN-RCVD\n"));
      }
    }


    /* If the PCB should be removed, do it. */
    if (pcb_remove) {
      tcp_pcb_purge(pcb);      
      /* Remove PCB from tcp_active_pcbs list. */
      if (prev != NULL) {
  LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_active_pcbs", pcb != tcp_active_pcbs);
        prev->next = pcb->next;
      } else {
        /* This PCB was the first. */
        LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_active_pcbs", tcp_active_pcbs == pcb);
        tcp_active_pcbs = pcb->next;
      }

      TCP_EVENT_ERR(pcb->errf, pcb->callback_arg, ERR_ABRT);

      pcb2 = pcb->next;
      memp_free(MEMP_TCP_PCB, pcb);
      pcb = pcb2;
    } else {

      /* We check if we should poll the connection. */
      ++pcb->polltmr;
      if (pcb->polltmr >= pcb->pollinterval) {
        pcb->polltmr = 0;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: polling application\n"));
        TCP_EVENT_POLL(pcb, err);
        if (err == ERR_OK) {
          tcp_output(pcb);
        }
      }
      
      prev = pcb;
      pcb = pcb->next;
    }
  }

  
  /* Steps through all of the TIME-WAIT PCBs. */
  prev = NULL;    
  pcb = tcp_tw_pcbs;
  while (pcb != NULL) {
    LWIP_ASSERT("tcp_slowtmr: TIME-WAIT pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
    pcb_remove = 0;

    /* Check if this PCB has stayed long enough in TIME-WAIT */
    if ((u32_t)(tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL) {
      ++pcb_remove;
    }
    


    /* If the PCB should be removed, do it. */
    if (pcb_remove) {
      tcp_pcb_purge(pcb);      
      /* Remove PCB from tcp_tw_pcbs list. */
      if (prev != NULL) {
  LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_tw_pcbs", pcb != tcp_tw_pcbs);
        prev->next = pcb->next;
      } else {
        /* This PCB was the first. */
        LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_tw_pcbs", tcp_tw_pcbs == pcb);
        tcp_tw_pcbs = pcb->next;
      }
      pcb2 = pcb->next;
      memp_free(MEMP_TCP_PCB, pcb);
      pcb = pcb2;
    } else {
      prev = pcb;
      pcb = pcb->next;
    }
  }
}

/**
 * Is called every TCP_FAST_INTERVAL (250 ms) and sends delayed ACKs.
 */
void
tcp_fasttmr(void)
{
  struct tcp_pcb *pcb;

  /* send delayed ACKs */  
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->flags & TF_ACK_DELAY) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_fasttmr: delayed ACK\n"));
      tcp_ack_now(pcb);
      pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
    }
  }
}

/**
 * Deallocates a list of TCP segments (tcp_seg structures).
 *
 */
u8_t
tcp_segs_free(struct tcp_seg *seg)
{
  u8_t count = 0;
  struct tcp_seg *next;
  while (seg != NULL) {
    next = seg->next;
    count += tcp_seg_free(seg);
    seg = next;
  }
  return count;
}

/**
 * Frees a TCP segment.
 *
 */
u8_t
tcp_seg_free(struct tcp_seg *seg)
{
  u8_t count = 0;
  
  if (seg != NULL) {
    if (seg->p != NULL) {
      count = pbuf_free(seg->p);
#if TCP_DEBUG
      seg->p = NULL;
#endif /* TCP_DEBUG */
    }
    memp_free(MEMP_TCP_SEG, seg);
  }
  return count;
}

/**
 * Sets the priority of a connection.
 *
 */
void
tcp_setprio(struct tcp_pcb *pcb, u8_t prio)
{
  pcb->prio = prio;
}
#if TCP_QUEUE_OOSEQ

/**
 * Returns a copy of the given TCP segment.
 *
 */ 
struct tcp_seg *
tcp_seg_copy(struct tcp_seg *seg)
{
  struct tcp_seg *cseg;

  cseg = memp_malloc(MEMP_TCP_SEG);
  if (cseg == NULL) {
    return NULL;
  }
  memcpy((char *)cseg, (const char *)seg, sizeof(struct tcp_seg)); 
  pbuf_ref(cseg->p);
  return cseg;
}
#endif

#if LWIP_CALLBACK_API
static err_t
tcp_recv_null(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  arg = arg;
  if (p != NULL) {
    pbuf_free(p);
  } else if (err == ERR_OK) {
    return tcp_close(pcb);
  }
  return ERR_OK;
}
#endif /* LWIP_CALLBACK_API */

static void
tcp_kill_prio(u8_t prio)
{
  struct tcp_pcb *pcb, *inactive;
  u32_t inactivity;
  u8_t mprio;


  mprio = TCP_PRIO_MAX;
  
  /* We kill the oldest active connection that has lower priority than
     prio. */
  inactivity = 0;
  inactive = NULL;
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->prio <= prio &&
       pcb->prio <= mprio &&
       (u32_t)(tcp_ticks - pcb->tmr) >= inactivity) {
      inactivity = tcp_ticks - pcb->tmr;
      inactive = pcb;
      mprio = pcb->prio;
    }
  }
  if (inactive != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_kill_prio: killing oldest PCB 0x%p (%ld)\n",
           (void *)inactive, inactivity));
    tcp_abort(inactive);
  }      
}


static void
tcp_kill_timewait(void)
{
  struct tcp_pcb *pcb, *inactive;
  u32_t inactivity;

  inactivity = 0;
  inactive = NULL;
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    if ((u32_t)(tcp_ticks - pcb->tmr) >= inactivity) {
      inactivity = tcp_ticks - pcb->tmr;
      inactive = pcb;
    }
  }
  if (inactive != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_kill_timewait: killing oldest TIME-WAIT PCB 0x%p (%ld)\n",
           (void *)inactive, inactivity));
    tcp_abort(inactive);
  }      
}



struct tcp_pcb *
tcp_alloc(u8_t prio)
{
  struct tcp_pcb *pcb;
  u32_t iss;
  
  pcb = memp_malloc(MEMP_TCP_PCB);
  if (pcb == NULL) {
    /* Try killing oldest connection in TIME-WAIT. */
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing off oldest TIME-WAIT connection\n"));
    tcp_kill_timewait();
    pcb = memp_malloc(MEMP_TCP_PCB);
    if (pcb == NULL) {
      tcp_kill_prio(prio);    
      pcb = memp_malloc(MEMP_TCP_PCB);
    }
  }
  if (pcb != NULL) {
    memset(pcb, 0, sizeof(struct tcp_pcb));
    pcb->prio = TCP_PRIO_NORMAL;
    pcb->snd_buf = TCP_SND_BUF;
    pcb->snd_queuelen = 0;
    pcb->rcv_wnd = TCP_WND;
    pcb->tos = 0;
    pcb->ttl = TCP_TTL;
    pcb->mss = TCP_MSS;
    pcb->rto = 3000 / TCP_SLOW_INTERVAL;
    pcb->sa = 0;
    pcb->sv = 3000 / TCP_SLOW_INTERVAL;
    pcb->rtime = 0;
    pcb->cwnd = 1;
    iss = tcp_next_iss();
    pcb->snd_wl2 = iss;
    pcb->snd_nxt = iss;
    pcb->snd_max = iss;
    pcb->lastack = iss;
    pcb->snd_lbb = iss;   
    pcb->tmr = tcp_ticks;

    pcb->polltmr = 0;

#if LWIP_CALLBACK_API
    pcb->recv = tcp_recv_null;
#endif /* LWIP_CALLBACK_API */  
    
    /* Init KEEPALIVE timer */
    pcb->keepalive = TCP_KEEPDEFAULT;
    pcb->keep_cnt = 0;
  }
  return pcb;
}

/**
 * Creates a new TCP protocol control block but doesn't place it on
 * any of the TCP PCB lists.
 *
 * @internal: Maybe there should be a idle TCP PCB list where these
 * PCBs are put on. We can then implement port reservation using
 * tcp_bind(). Currently, we lack this (BSD socket type of) feature.
 */

struct tcp_pcb *
tcp_new(void)
{
  return tcp_alloc(TCP_PRIO_NORMAL);
}

/*
 * tcp_arg():
 *
 * Used to specify the argument that should be passed callback
 * functions.
 *
 */ 

void
tcp_arg(struct tcp_pcb *pcb, void *arg)
{  
  pcb->callback_arg = arg;
}
#if LWIP_CALLBACK_API

/**
 * Used to specify the function that should be called when a TCP
 * connection receives data.
 *
 */ 
void
tcp_recv(struct tcp_pcb *pcb,
   err_t (* recv)(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err))
{
  pcb->recv = recv;
}

/**
 * Used to specify the function that should be called when TCP data
 * has been successfully delivered to the remote host.
 *
 */ 

void
tcp_sent(struct tcp_pcb *pcb,
   err_t (* sent)(void *arg, struct tcp_pcb *tpcb, u16_t len))
{
  pcb->sent = sent;
}

/**
 * Used to specify the function that should be called when a fatal error
 * has occured on the connection.
 *
 */ 
void
tcp_err(struct tcp_pcb *pcb,
   void (* errf)(void *arg, err_t err))
{
  pcb->errf = errf;
}

/**
 * Used for specifying the function that should be called when a
 * LISTENing connection has been connected to another host.
 *
 */ 
void
tcp_accept(struct tcp_pcb *pcb,
     err_t (* accept)(void *arg, struct tcp_pcb *newpcb, err_t err))
{
  ((struct tcp_pcb_listen *)pcb)->accept = accept;
}
#endif /* LWIP_CALLBACK_API */


/**
 * Used to specify the function that should be called periodically
 * from TCP. The interval is specified in terms of the TCP coarse
 * timer interval, which is called twice a second.
 *
 */ 
void
tcp_poll(struct tcp_pcb *pcb,
   err_t (* poll)(void *arg, struct tcp_pcb *tpcb), u8_t interval)
{
#if LWIP_CALLBACK_API
  pcb->poll = poll;
#endif /* LWIP_CALLBACK_API */  
  pcb->pollinterval = interval;
}

/**
 * Purges a TCP PCB. Removes any buffered data and frees the buffer memory.
 *
 */
void
tcp_pcb_purge(struct tcp_pcb *pcb)
{
  if (pcb->state != CLOSED &&
     pcb->state != TIME_WAIT &&
     pcb->state != LISTEN) {

    LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge\n"));
    
    if (pcb->unsent != NULL) {    
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: not all data sent\n"));
    }
    if (pcb->unacked != NULL) {    
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: data left on ->unacked\n"));
    }
#if TCP_QUEUE_OOSEQ /* LW */
    if (pcb->ooseq != NULL) {    
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: data left on ->ooseq\n"));
    }
    
    tcp_segs_free(pcb->ooseq);
    pcb->ooseq = NULL;
#endif /* TCP_QUEUE_OOSEQ */
    tcp_segs_free(pcb->unsent);
    tcp_segs_free(pcb->unacked);
    pcb->unacked = pcb->unsent = NULL;
  }
}

/**
 * Purges the PCB and removes it from a PCB list. Any delayed ACKs are sent first.
 *
 */
void
tcp_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb)
{
  TCP_RMV(pcblist, pcb);

  tcp_pcb_purge(pcb);
  
  /* if there is an outstanding delayed ACKs, send it */
  if (pcb->state != TIME_WAIT &&
     pcb->state != LISTEN &&
     pcb->flags & TF_ACK_DELAY) {
    pcb->flags |= TF_ACK_NOW;
    tcp_output(pcb);
  }  
  pcb->state = CLOSED;

  LWIP_ASSERT("tcp_pcb_remove: tcp_pcbs_sane()", tcp_pcbs_sane());
}

/**
 * Calculates a new initial sequence number for new connections.
 *
 */
u32_t
tcp_next_iss(void)
{
  static u32_t iss = 6510;
  
  iss += tcp_ticks;       /* XXX */
  return iss;
}

#if TCP_DEBUG || TCP_INPUT_DEBUG || TCP_OUTPUT_DEBUG
void
tcp_debug_print(struct tcp_hdr *tcphdr)
{
  LWIP_DEBUGF(TCP_DEBUG, ("TCP header:\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|    %5u      |    %5u      | (src port, dest port)\n",
         ntohs(tcphdr->src), ntohs(tcphdr->dest)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|           %010lu          | (seq no)\n",
          ntohl(tcphdr->seqno)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|           %010lu          | (ack no)\n",
         ntohl(tcphdr->ackno)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("| %2u |   |%u%u%u%u%u%u|     %5u     | (hdrlen, flags (",
       TCPH_HDRLEN(tcphdr),
         TCPH_FLAGS(tcphdr) >> 5 & 1,
         TCPH_FLAGS(tcphdr) >> 4 & 1,
         TCPH_FLAGS(tcphdr) >> 3 & 1,
         TCPH_FLAGS(tcphdr) >> 2 & 1,
         TCPH_FLAGS(tcphdr) >> 1 & 1,
         TCPH_FLAGS(tcphdr) & 1,
         ntohs(tcphdr->wnd)));
  tcp_debug_print_flags(TCPH_FLAGS(tcphdr));
  LWIP_DEBUGF(TCP_DEBUG, ("), win)\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|    0x%04x     |     %5u     | (chksum, urgp)\n",
         ntohs(tcphdr->chksum), ntohs(tcphdr->urgp)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
}

void
tcp_debug_print_state(enum tcp_state s)
{
  LWIP_DEBUGF(TCP_DEBUG, ("State: "));
  switch (s) {
  case CLOSED:
    LWIP_DEBUGF(TCP_DEBUG, ("CLOSED\n"));
    break;
 case LISTEN:
   LWIP_DEBUGF(TCP_DEBUG, ("LISTEN\n"));
   break;
  case SYN_SENT:
    LWIP_DEBUGF(TCP_DEBUG, ("SYN_SENT\n"));
    break;
  case SYN_RCVD:
    LWIP_DEBUGF(TCP_DEBUG, ("SYN_RCVD\n"));
    break;
  case ESTABLISHED:
    LWIP_DEBUGF(TCP_DEBUG, ("ESTABLISHED\n"));
    break;
  case FIN_WAIT_1:
    LWIP_DEBUGF(TCP_DEBUG, ("FIN_WAIT_1\n"));
    break;
  case FIN_WAIT_2:
    LWIP_DEBUGF(TCP_DEBUG, ("FIN_WAIT_2\n"));
    break;
  case CLOSE_WAIT:
    LWIP_DEBUGF(TCP_DEBUG, ("CLOSE_WAIT\n"));
    break;
  case CLOSING:
    LWIP_DEBUGF(TCP_DEBUG, ("CLOSING\n"));
    break;
  case LAST_ACK:
    LWIP_DEBUGF(TCP_DEBUG, ("LAST_ACK\n"));
    break;
  case TIME_WAIT:
    LWIP_DEBUGF(TCP_DEBUG, ("TIME_WAIT\n"));
   break;
  }
}

void
tcp_debug_print_flags(u8_t flags)
{
  if (flags & TCP_FIN) {
    LWIP_DEBUGF(TCP_DEBUG, ("FIN "));
  }
  if (flags & TCP_SYN) {
    LWIP_DEBUGF(TCP_DEBUG, ("SYN "));
  }
  if (flags & TCP_RST) {
    LWIP_DEBUGF(TCP_DEBUG, ("RST "));
  }
  if (flags & TCP_PSH) {
    LWIP_DEBUGF(TCP_DEBUG, ("PSH "));
  }
  if (flags & TCP_ACK) {
    LWIP_DEBUGF(TCP_DEBUG, ("ACK "));
  }
  if (flags & TCP_URG) {
    LWIP_DEBUGF(TCP_DEBUG, ("URG "));
  }
  if (flags & TCP_ECE) {
    LWIP_DEBUGF(TCP_DEBUG, ("ECE "));
  }
  if (flags & TCP_CWR) {
    LWIP_DEBUGF(TCP_DEBUG, ("CWR "));
  }
}

void
tcp_debug_print_pcbs(void)
{
  struct tcp_pcb *pcb;
  LWIP_DEBUGF(TCP_DEBUG, ("Active PCB states:\n"));
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %u, foreign port %u snd_nxt %lu rcv_nxt %lu ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }    
  LWIP_DEBUGF(TCP_DEBUG, ("Listen PCB states:\n"));
  for(pcb = (struct tcp_pcb *)tcp_listen_pcbs.pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %u, foreign port %u snd_nxt %lu rcv_nxt %lu ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }    
  LWIP_DEBUGF(TCP_DEBUG, ("TIME-WAIT PCB states:\n"));
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %u, foreign port %u snd_nxt %lu rcv_nxt %lu ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }    
}

int
tcp_pcbs_sane(void)
{
  struct tcp_pcb *pcb;
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != CLOSED", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != LISTEN", pcb->state != LISTEN);
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != TIME-WAIT", pcb->state != TIME_WAIT);
  }
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_ASSERT("tcp_pcbs_sane: tw pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
  }
  return 1;
}
#endif /* TCP_DEBUG */
#endif /* LWIP_TCP */










