/*
 * IS-IS Rout(e)ing protocol - isis_events.h   
 *
 * Copyright (C) 2001,2002   Sampo Saaristo
 *                           Tampere University of Technology      
 *                           Institute of Communications Engineering
 *
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public Licenseas published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
 * more details.

 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <zebra.h>
#include <net/ethernet.h>

#include "log.h"
#include "memory.h"
#include "if.h"
#include "linklist.h"
#include "command.h"
#include "thread.h"
#include "hash.h"
#include "prefix.h"
#include "stream.h"

#include "isisd/dict.h"
#include "isisd/include-netbsd/iso.h"
#include "isisd/isis_constants.h"
#include "isisd/isis_common.h"
#include "isisd/isis_circuit.h"
#include "isisd/isis_tlv.h"
#include "isisd/isis_lsp.h"
#include "isisd/isis_pdu.h"
#include "isisd/isis_network.h"
#include "isisd/isis_misc.h"
#include "isisd/isis_constants.h"
#include "isisd/isis_adjacency.h"
#include "isisd/isis_dr.h"
#include "isisd/isis_flags.h"
#include "isisd/isisd.h"
#include "isisd/isis_csm.h"
#include "isisd/isis_events.h"
#include "isisd/isis_spf.h"

extern struct thread_master *master;
extern struct isis *isis;

/* debug isis-spf spf-events 
 4w4d: ISIS-Spf (tlt): L2 SPF needed, new adjacency, from 0x609229F4
 4w4d: ISIS-Spf (tlt): L2, 0000.0000.0042.01-00 TLV contents changed, code 0x2
 4w4d: ISIS-Spf (tlt): L2, new LSP 0 DEAD.BEEF.0043.00-00
 4w5d: ISIS-Spf (tlt): L1 SPF needed, periodic SPF, from 0x6091C844
 4w5d: ISIS-Spf (tlt): L2 SPF needed, periodic SPF, from 0x6091C844
*/

void 
isis_event_circuit_state_change (struct isis_circuit *circuit, int up)
{
  struct isis_area *area;
  
  area = circuit->area;
  assert (area);
  area->circuit_state_changes++;
  
  if (isis->debugs & DEBUG_EVENTS) 
    zlog_info ("ISIS-Evt (%s) circuit %s", circuit->area->area_tag, 
	       up ? "up" : "down");
  
  /*
   * Regenerate LSPs this affects
   */
  lsp_regenerate_schedule (area);

  return;
}

void 
isis_event_system_type_change (struct isis_area *area, int newtype)
{
  struct listnode *node;
  struct isis_circuit *circuit;

  if (isis->debugs & DEBUG_EVENTS) 
    zlog_info ("ISIS-Evt (%s) system type change %s -> %s", area->area_tag, 
	       circuit_t2string (area->is_type), circuit_t2string (newtype));
  
  if (area->is_type == newtype)
    return; /* No change */
  
  switch (area->is_type) {
  case IS_LEVEL_1:
    if (area->lspdb[1] == NULL)
      area->lspdb[1] = lsp_db_init (); 
    lsp_l2_generate (area);
    break;
  case IS_LEVEL_1_AND_2:
    if (newtype == IS_LEVEL_1) {
      lsp_db_destroy (area->lspdb[1]);
    } 
    else {
      lsp_db_destroy (area->lspdb[0]);
    } 
    break;
  case IS_LEVEL_2:
    if (area->lspdb[0] == NULL)
      area->lspdb[0] = lsp_db_init (); 
    lsp_l1_generate (area);
    break;
  default:
    break;
  }
  
  area->is_type = newtype;
  for (node = listhead (area->circuit_list); node; nextnode (node)) {
    circuit = getdata (node);
    isis_event_circuit_type_change (circuit, newtype);
  }

  spftree_area_init (area);
  lsp_regenerate_schedule (area);

  return;
}



void 
isis_event_area_addr_change (struct isis_area *area)
{

}

void
circuit_commence_level (struct isis_circuit *circuit, int level)
{
  uint32_t interval;

  if (level == 1) {
    THREAD_TIMER_ON(master, circuit->t_send_psnp[0], send_l1_psnp, circuit,
        isis_jitter(circuit->psnp_interval[0], PSNP_JITTER));

    if (circuit->circ_type == CIRCUIT_T_BROADCAST) {
      interval = circuit->hello_multiplier[0] * (circuit->hello_interval[0]); 
      
      THREAD_TIMER_ON(master, circuit->u.bc.t_run_dr[0], isis_run_dr_l1,
						     circuit, interval);
      
      THREAD_TIMER_ON(master, circuit->u.bc.t_send_lan_hello[0],
          send_lan_l1_hello, circuit,
          isis_jitter(circuit->hello_interval[0], IIH_JITTER));
      
      circuit->u.bc.lan_neighs[0] = list_new ();
    }
  } else {
    THREAD_TIMER_ON(master, circuit->t_send_psnp[1], send_l2_psnp, circuit,
        isis_jitter(circuit->psnp_interval[1], PSNP_JITTER));

    if (circuit->circ_type == CIRCUIT_T_BROADCAST) {
      interval = circuit->hello_multiplier[1] * (circuit->hello_interval[1]); 
      
      THREAD_TIMER_ON(master, circuit->u.bc.t_run_dr[1], isis_run_dr_l2,
						     circuit, interval);
      
      THREAD_TIMER_ON(master, circuit->u.bc.t_send_lan_hello[1],
          send_lan_l2_hello, circuit,
          isis_jitter(circuit->hello_interval[1], IIH_JITTER));
      
      circuit->u.bc.lan_neighs[1] = list_new ();
    }
  }
  
  return;
}

void
circuit_resign_level (struct isis_circuit *circuit, int level)
{
  int idx = level - 1;
 
  THREAD_TIMER_OFF(circuit->t_send_csnp[idx]);
  THREAD_TIMER_OFF(circuit->t_send_psnp[idx]);

  if (circuit->circ_type == CIRCUIT_T_BROADCAST) {
    THREAD_TIMER_OFF(circuit->u.bc.t_send_lan_hello[idx]);
    THREAD_TIMER_OFF(circuit->u.bc.t_run_dr[idx]);
    circuit->u.bc.run_dr_elect[idx] = 0;
  }
  
  return;
}

void 
isis_event_circuit_type_change (struct isis_circuit *circuit, int newtype) 
{
  
  if (isis->debugs & DEBUG_EVENTS) 
    zlog_info ("ISIS-Evt (%s) circuit type change %s -> %s", 
	       circuit->area->area_tag, 
	       circuit_t2string (circuit->circuit_is_type), 
	       circuit_t2string (newtype));

  if (circuit->circuit_is_type == newtype)
    return; /* No change */

  if (!(newtype & circuit->area->is_type)) {
    zlog_err ("ISIS-Evt (%s) circuit type change - invalid level %s because"
	      " area is %s", circuit->area->area_tag, 
	      circuit_t2string (newtype), 
	      circuit_t2string (circuit->area->is_type));
    return;
  }
    
  switch (circuit->circuit_is_type) {
  case IS_LEVEL_1:
    if (newtype == IS_LEVEL_2)
      circuit_resign_level (circuit, 1);
    circuit_commence_level (circuit, 2);
    break;
  case IS_LEVEL_1_AND_2:
    if (newtype == IS_LEVEL_1)
      circuit_resign_level (circuit, 2);
    else 
      circuit_resign_level (circuit, 1);
    break;
  case IS_LEVEL_2:
    if (newtype == IS_LEVEL_1)
      circuit_resign_level (circuit, 2);
    circuit_commence_level (circuit, 1);
    break;
  default:
    break;
  }
  
  circuit->circuit_is_type = newtype;
  lsp_regenerate_schedule (circuit->area);

  return;
}

 /* 04/18/2002 by Gwak. */
 /**************************************************************************
  *
  * EVENTS for LSP generation
  *
  * 1) an Adajacency or Circuit Up/Down event
  * 2) a chnage in Circuit metric
  * 3) a change in Reachable Address metric
  * 4) a change in manualAreaAddresses
  * 5) a change in systemID
  * 6) a change in DIS status
  * 7) a chnage in the waiting status
  *
  * ***********************************************************************
  *
  * current support event
  *
  * 1) Adjacency Up/Down event
  * 6) a change in DIS status
  *
  * ***********************************************************************/

void 
isis_event_adjacency_state_change (struct isis_adjacency *adj, int newstate)
{
  /* adjacency state change event. 
   * - the only proto-type was supported */
  
  /* invalid arguments */ 
  if ( !adj || !adj->circuit || !adj->circuit->area ) return;     
  
  zlog_info ("ISIS-Evt (%s) Adjacency State change", 
	     adj->circuit->area->area_tag );
  
  /* LSP generation again */
  lsp_regenerate_schedule (adj->circuit->area);

  return;
}

/* events supporting code */

int
isis_event_dis_status_change (struct thread *thread)
{
  struct isis_circuit *circuit;
  
  circuit = THREAD_ARG (thread);
  
  /* invalid arguments */
  if (!circuit || !circuit->area) return 0;
  
  zlog_info ("ISIS-Evt (%s) DIS status change", circuit->area->area_tag);

  /* LSP generation again */
  lsp_regenerate_schedule (circuit->area);
  
  return 0;
}


void 
isis_event_auth_failure (char *area_tag, char *error_string, char *sysid) 
{
  zlog_info ("ISIS-Evt (%s) Authentication failure %s from %s",
	     area_tag, error_string, sysid_print (sysid));
  
  return;
}

