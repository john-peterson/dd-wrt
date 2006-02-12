/*
 * Copyright (C) 1999 Yasuhiro Ohara
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the 
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
 * Boston, MA 02111-1307, USA.  
 */

/* Interface State Machine */

#include "ospf6d.h"

int
ifs_change (state_t ifs_next, char *reason, struct ospf6_interface *ospf6_interface)
{
  state_t ifs_prev;

  ifs_prev = ospf6_interface->state;

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("I/F [%s] %s -> %s (%s)",
               ospf6_interface->interface->name,
               ospf6_interface_state_string[ifs_prev],
               ospf6_interface_state_string[ifs_next], reason);

  switch (ifs_prev)
    {
    case IFS_DR:
    case IFS_BDR:
      switch (ifs_next)
        {
        case IFS_DR:
        case IFS_BDR:
          break;
        default:
          ospf6_leave_alldrouters (ospf6_interface->interface->ifindex);
          break;
        }
      break;
    default:
      switch (ifs_next)
        {
        case IFS_DR:
        case IFS_BDR:
          ospf6_join_alldrouters (ospf6_interface->interface->ifindex);
          break;
        default:
          break;
        }
      break;
    }

  ospf6_interface->state = ifs_next;

  /* construct Router-LSA */
  ospf6_lsa_update_router (ospf6_interface->area->area_id);

  dr_change (ospf6_interface);

  return 0;
}

int
dr_change (struct ospf6_interface *ospf6_interface)
{
  if (ospf6_interface->prevdr == ospf6_interface->dr
      && ospf6_interface->prevbdr == ospf6_interface->bdr)
    return 0; /* Nothing has been changed */

  if (IS_OSPF6_DUMP_INTERFACE)
    {
      char dr[16], bdr[16], prevdr[16], prevbdr[16];
      inet_ntop (AF_INET, &ospf6_interface->prevdr, prevdr, sizeof (prevdr));
      inet_ntop (AF_INET, &ospf6_interface->prevbdr, prevbdr, sizeof (prevbdr));
      inet_ntop (AF_INET, &ospf6_interface->dr, dr, sizeof (dr));
      inet_ntop (AF_INET, &ospf6_interface->bdr, bdr, sizeof (bdr));
      zlog_info ("I/F [%s] {dr:%s,bdr:%s} -> {dr:%s,bdr:%s}",
                 ospf6_interface->interface->name, prevdr, prevbdr, dr, bdr);
    }

  /* construct LSAs */
  ospf6_lsa_update_router (ospf6_interface->area->area_id);
  if (ospf6_interface->state == IFS_DR)
    {
      ospf6_lsa_update_network (ospf6_interface->interface->name);
      ospf6_lsa_update_intra_prefix_transit (ospf6_interface->interface->name);
    }
  ospf6_lsa_update_intra_prefix_stub (ospf6_interface->area->area_id);

  return 0;
}


/* Interface State Machine */
int
interface_up (struct thread *thread)
{
  struct ospf6_interface *ospf6_interface;

  ospf6_interface = (struct ospf6_interface *)THREAD_ARG (thread);

  assert (ospf6_interface);
  assert (ospf6_interface->interface);

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("Interface Event %s: InterfaceUp",
               ospf6_interface->interface->name);

  /* check physical interface is up */
  if (!if_is_up (ospf6_interface->interface))
    {
      zlog_warn ("*** Interface %s is down, can't execute InterfaceUp event",
                ospf6_interface->interface->name);
      return -1;
    }

  /* if already enabled, do nothing */
  if (ospf6_interface->state > IFS_DOWN)
    {
      zlog_warn ("*** Interface %s is already up",
                 ospf6_interface->interface->name);
      return 0;
    }

  /* ifid of this interface */
  ospf6_interface->if_id = ospf6_interface->interface->ifindex;

#ifdef FREEBSD_32
  /* FreeBSD3.2's ep driver ignores multicast ethernet frame.
     saving this (very roughly) by make I/F promiscous mode */
  {
    struct ifreq ifr;

    strncpy (ifr.ifr_name, ospf6_interface->interface->name, sizeof (ifr.ifr_name));
    if (ioctl (ospf6_sock, SIOCGIFFLAGS, &ifr) < 0)
      zlog_warn ("fbsd3.2: get I/F flags failed: %s", strerror (errno));
    ifr.ifr_flags |= IFF_PROMISC;
    if (ioctl (ospf6_sock, SIOCSIFFLAGS, &ifr) < 0)
      zlog_warn ("fbsd3.2: set I/F flags failed: %s", strerror (errno));
  }
#endif /*FREEBSD_32*/

  /* Join AllSPFRouters */
  ospf6_join_allspfrouters (ospf6_interface->interface->ifindex);

  /* set socket options */
  ospf6_set_reuseaddr ();
  ospf6_reset_mcastloop ();
  ospf6_set_pktinfo ();
  ospf6_set_checksum ();

  /* Schedule Hello */
  if (! ospf6_interface->is_passive)
    thread_add_event (master, ospf6_send_hello, ospf6_interface, 0);

  /* decide next interface state */
  if (if_is_pointopoint (ospf6_interface->interface))
    ifs_change (IFS_PTOP, "IF Type PointToPoint", ospf6_interface);
  else if (ospf6_interface->priority == 0)
    ifs_change (IFS_DROTHER, "Router Priority = 0", ospf6_interface);
  else
    {
      ifs_change (IFS_WAITING, "Priority > 0", ospf6_interface);
      thread_add_timer (master, wait_timer, ospf6_interface,
                        ospf6_interface->dead_interval);
    }

  /* construct LSAs */
  ospf6_lsa_update_link (ospf6_interface->interface->name);
  if (ospf6_interface->state == IFS_DR)
    ospf6_lsa_update_intra_prefix_transit (ospf6_interface->interface->name);
  ospf6_lsa_update_intra_prefix_stub (ospf6_interface->area->area_id);

  return 0;
}

int
wait_timer (struct thread *thread)
{
  struct ospf6_interface *ospf6_interface;

  ospf6_interface = (struct ospf6_interface *)THREAD_ARG  (thread);
  assert (ospf6_interface);

  if (ospf6_interface->state != IFS_WAITING)
    return 0;

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("I/F [%s] WaitTimer", ospf6_interface->interface->name);

  ifs_change (dr_election (ospf6_interface), "WaitTimer:DR Election", ospf6_interface);
  return 0;
}

int backup_seen (struct thread *thread)
{
  struct ospf6_interface *ospf6_interface;

  ospf6_interface = (struct ospf6_interface *)THREAD_ARG  (thread);
  assert (ospf6_interface);

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("I/F [%s] BackupSeen", ospf6_interface->interface->name);

  if (ospf6_interface->state == IFS_WAITING)
    ifs_change (dr_election (ospf6_interface), "BackupSeen:DR Election", ospf6_interface);

  return 0;
}

int neighbor_change (struct thread *thread)
{
  struct ospf6_interface *ospf6_interface;

  ospf6_interface = (struct ospf6_interface *)THREAD_ARG  (thread);
  assert (ospf6_interface);

  if (ospf6_interface->state != IFS_DROTHER &&
      ospf6_interface->state != IFS_BDR &&
      ospf6_interface->state != IFS_DR)
    return 0;

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("I/F [%s] NeighborChange", ospf6_interface->interface->name);

  ifs_change (dr_election (ospf6_interface), "NeighborChange:DR Election", ospf6_interface);

  return 0;
}

int
loopind (struct thread *thread)
{
  struct ospf6_interface *ospf6_interface;

  ospf6_interface = (struct ospf6_interface *)THREAD_ARG (thread);
  assert (ospf6_interface);

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("I/F [%s] LoopInd", ospf6_interface->interface->name);

  return 0;
}

int
interface_down (struct thread *thread)
{
  struct ospf6_interface *ospf6_interface;

  ospf6_interface = (struct ospf6_interface *)THREAD_ARG (thread);
  assert (ospf6_interface);

  if (IS_OSPF6_DUMP_INTERFACE)
    zlog_info ("I/F [%s] InterfaceDown", ospf6_interface->interface->name);

  if (ospf6_interface->state == IFS_NONE)
    return 1;

  /* Leave AllSPFRouters */
  ospf6_leave_allspfrouters (ospf6_interface->interface->ifindex);

  ifs_change (IFS_DOWN, "Configured", ospf6_interface);

  return 0;
}


/* 9.4 of RFC2328 */
int
dr_election (struct ospf6_interface *ospf6_interface)
{
  list candidate_list = list_new ();
  listnode i, j, n;
  ifid_t prevdr, prevbdr, dr = 0, bdr;
  struct ospf6_neighbor *nbpi, *nbpj, myself, *nbr;
  int declare = 0;
  int gofive = 0;

  /* statistics */
  ospf6_interface->ospf6_stat_dr_election++;

  /* pseudo neighbor "myself" */
  memset (&myself, 0, sizeof (myself));
  myself.state = NBS_TWOWAY;
  myself.dr = ospf6_interface->dr;
  myself.bdr = ospf6_interface->bdr;
  myself.priority = ospf6_interface->priority;
  myself.ifid = ospf6_interface->if_id;
  myself.router_id = ospf6_interface->area->ospf6->router_id;

/* step_one: */

  ospf6_interface->prevdr = prevdr = ospf6_interface->dr;
  ospf6_interface->prevbdr = prevbdr = ospf6_interface->bdr;

step_two:

  /* Calculate Backup Designated Router. */
  /* Make Candidate list */
  if (!list_isempty (candidate_list))
    list_delete_all_node (candidate_list);
  declare = 0;
  for (i = listhead (ospf6_interface->neighbor_list); i; nextnode (i))
    {
      nbpi = (struct ospf6_neighbor *)getdata (i);
      if (nbpi->priority == 0)
        continue;
      if (nbpi->state < NBS_TWOWAY)
        continue;
      if (nbpi->dr == nbpi->router_id)
        continue;
      if (nbpi->bdr == nbpi->router_id)
        declare++;
      listnode_add (candidate_list, nbpi);
    }

  if (myself.priority)
    {
      if (myself.dr != myself.router_id)
        {
          if (myself.bdr == myself.router_id)
            declare++;
          listnode_add (candidate_list, &myself);
        }
    }

  /* Elect BDR */
  for (i = listhead (candidate_list);
       candidate_list->count > 1;
       i = listhead (candidate_list))
    {
      j = i;
      nextnode(j);
      assert (j);
      nbpi = (struct ospf6_neighbor *)getdata (i);
      nbpj = (struct ospf6_neighbor *)getdata (j);
      if (declare)
        {
          int deleted = 0;
          if (nbpi->bdr != nbpi->router_id)
            {
              listnode_delete (candidate_list, nbpi);
              deleted++;
            }
          if (nbpj->bdr != nbpj->router_id)
            {
              listnode_delete (candidate_list, nbpj);
              deleted++;
            }
          if (deleted)
            continue;
        }
      if (nbpi->priority > nbpj->priority)
        {
          listnode_delete (candidate_list, nbpj);
          continue;
        }
      else if (nbpi->priority < nbpj->priority)
        {
          listnode_delete (candidate_list, nbpi);
          continue;
        }
      else /* equal, case of tie */
        {
          if (ntohl (nbpi->router_id) > ntohl (nbpj->router_id))
            {
              listnode_delete (candidate_list, nbpj);
              continue;
            }
          else if (ntohl (nbpi->router_id) < ntohl (nbpj->router_id))
            {
              listnode_delete (candidate_list, nbpi);
              continue;
            }
          else
            assert (0);
        }
    }

  if (!list_isempty (candidate_list))
    {
      assert (candidate_list->count == 1);
      n = listhead (candidate_list);
      nbr = (struct ospf6_neighbor *)getdata (n);
      bdr = nbr->router_id;
    }
  else
    bdr = 0;

/* step_three: */

  /* Calculate Designated Router. */
  /* Make Candidate list */
  if (!list_isempty (candidate_list))
    list_delete_all_node (candidate_list);
  declare = 0;
  for (i = listhead (ospf6_interface->neighbor_list); i; nextnode (i))
    {
      nbpi = (struct ospf6_neighbor *)getdata (i);
      if (nbpi->priority == 0)
        continue;
      if (nbpi->state < NBS_TWOWAY)
        continue;
      if (nbpi->dr == nbpi->router_id)
        {
          declare++;
          listnode_add (candidate_list, nbpi);
        }
    }
  if (myself.priority)
    {
      if (myself.dr == myself.router_id)
        {
          declare++;
          listnode_add (candidate_list, &myself);
        }
    }

  /* Elect DR */
  if (declare == 0)
    {
      assert (list_isempty (candidate_list));
      /* No one declare but candidate_list not empty */
      dr = bdr;
    }
  else
    {
      assert (!list_isempty (candidate_list));
      for (i = listhead (candidate_list);
           candidate_list->count > 1;
           i = listhead (candidate_list))
        {
          j = i;
          nextnode (j);
          assert (j);
          nbpi = (struct ospf6_neighbor *)getdata (i);
          nbpj = (struct ospf6_neighbor *)getdata (j);

          if (nbpi->dr != nbpi->router_id)
            {
              list_delete_node (candidate_list, i);
              continue;
            }
          if (nbpj->dr != nbpj->router_id)
            {
              list_delete_node (candidate_list, j);
              continue;
            }

          if (nbpi->priority > nbpj->priority)
            {
              list_delete_node (candidate_list, j);
              continue;
            }
          else if (nbpi->priority < nbpj->priority)
            {
              list_delete_node (candidate_list, i);
              continue;
            }
          else /* equal, case of tie */
            {
              if (nbpi->router_id > nbpj->router_id)
                {
                  list_delete_node (candidate_list, j);
                  continue;
                }
              else if (nbpi->router_id < nbpj->router_id)
                {
                  list_delete_node (candidate_list, i);
                  continue;
                }
              else
                {
                  zlog_warn ("!!!THE SAME ROUTER ID FOR DIFFERENT NEIGHBOR");
                  zlog_warn ("!!!MISCONFIGURATION?");
                  list_delete_node (candidate_list, i);
                  continue;
                }
            }
        }
      if (!list_isempty (candidate_list))
        {
          assert (candidate_list->count == 1);
          n = listhead (candidate_list);
          nbr = (struct ospf6_neighbor *)getdata (n);
          dr = nbr->router_id;
        }
      else
        assert (0);
    }

/* step_four: */

  if (gofive)
    goto step_five;

  if (dr != prevdr)
    {
      if ((dr == myself.router_id || prevdr == myself.router_id)
          && !(dr == myself.router_id && prevdr == myself.router_id))
        {
          myself.dr = dr;
          myself.bdr = bdr;
          gofive++;
          goto step_two;
        }
    }
  if (bdr != prevbdr)
    {
      if ((bdr == myself.router_id || prevbdr == myself.router_id)
          && !(bdr == myself.router_id && prevbdr == myself.router_id))
        {
          myself.dr = dr;
          myself.bdr = bdr;
          gofive++;
          goto step_two;
        }
    }

step_five:

  ospf6_interface->dr = dr;
  ospf6_interface->bdr = bdr;

  if (prevdr != dr || prevbdr != bdr)
    {
      for (i = listhead (ospf6_interface->neighbor_list); i; nextnode (i))
        {
          nbpi = getdata (i);
          if (nbpi->state < NBS_TWOWAY)
            continue;
          /* Schedule or Execute AdjOK. which does "invoke" mean? */
          thread_add_event (master, adj_ok, nbpi, 0);
        }
    }

  list_delete (candidate_list);

  if (dr == myself.router_id)
    {
      assert (bdr != myself.router_id);
      return IFS_DR;
    }
  else if (bdr == myself.router_id)
    {
      assert (dr != myself.router_id);
      return IFS_BDR;
    }
  else
    return IFS_DROTHER;
}

