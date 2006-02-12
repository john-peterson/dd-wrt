/*
 *	BIRD -- OSPF
 *
 *	(c) 1999 - 2004 Ondrej Filip <feela@network.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#include "ospf.h"

char *ospf_ns[] = { "    down",
  " attempt",
  "    init",
  "    2way",
  " exstart",
  "exchange",
  " loading",
  "    full"
};

const char *ospf_inm[] =
  { "hello received", "neighbor start", "2-way received",
  "negotiation done", "exstart done", "bad ls request", "load done",
  "adjacency ok?", "sequence mismatch", "1-way received", "kill neighbor",
  "inactivity timer", "line down"
};


void neighbor_timer_hook(timer * timer);
void rxmt_timer_hook(timer * timer);
void ackd_timer_hook(timer * t);

struct ospf_neighbor *
ospf_neighbor_new(struct ospf_iface *ifa)
{
  struct proto *p = (struct proto *) (ifa->oa->po);
  struct pool *pool = rp_new(p->pool, "OSPF Neighbor");
  struct ospf_neighbor *n = mb_allocz(pool, sizeof(struct ospf_neighbor));

  n->pool = pool;
  n->ifa = ifa;
  add_tail(&ifa->neigh_list, NODE n);
  n->adj = 0;
  n->csn = 0;
  n->ldbdes = mb_allocz(pool, ifa->iface->mtu);
  n->state = NEIGHBOR_DOWN;

  n->inactim = tm_new(pool);
  n->inactim->data = n;
  n->inactim->randomize = 0;
  n->inactim->hook = neighbor_timer_hook;
  n->inactim->recurrent = 0;
  DBG("%s: Installing inactivity timer.\n", p->name);

  n->rxmt_timer = tm_new(pool);
  n->rxmt_timer->data = n;
  n->rxmt_timer->randomize = 0;
  n->rxmt_timer->hook = rxmt_timer_hook;
  n->rxmt_timer->recurrent = ifa->rxmtint;
  s_init_list(&(n->lsrql));
  n->lsrqh = ospf_top_new(pool);
  s_init_list(&(n->lsrtl));
  n->lsrth = ospf_top_new(pool);
  s_init(&(n->lsrqi), &(n->lsrql));
  s_init(&(n->lsrti), &(n->lsrtl));
  tm_start(n->rxmt_timer, n->ifa->rxmtint);
  DBG("%s: Installing rxmt timer.\n", p->name);

  n->ackd_timer = tm_new(pool);
  n->ackd_timer->data = n;
  n->ackd_timer->randomize = 0;
  n->ackd_timer->hook = ackd_timer_hook;
  n->ackd_timer->recurrent = ifa->rxmtint / 2;
  init_list(&n->ackl[ACKL_DIRECT]);
  init_list(&n->ackl[ACKL_DELAY]);
  tm_start(n->ackd_timer, n->ifa->rxmtint / 2);
  DBG("%s: Installing ackd timer.\n", p->name);

  return (n);
}

/**
 * neigh_chstate - handles changes related to new or lod state of neighbor
 * @n: OSPF neighbor
 * @state: new state
 *
 * Many actions have to be taken acording to a change of state of a neighbor. It
 * starts rxmt timers, call interface state machine etc.
 */

void
neigh_chstate(struct ospf_neighbor *n, u8 state)
{
  u8 oldstate;

  oldstate = n->state;

  if (oldstate != state)
  {
    struct ospf_iface *ifa = n->ifa;
    struct proto_ospf *po = ifa->oa->po;
    struct proto *p = &po->proto;

    n->state = state;

    OSPF_TRACE(D_EVENTS, "Neighbor %I changes state from \"%s\" to \"%s\".",
	       n->ip, ospf_ns[oldstate], ospf_ns[state]);

    if ((state == NEIGHBOR_2WAY) && (oldstate < NEIGHBOR_2WAY))
      ospf_iface_sm(ifa, ISM_NEICH);
    if ((state < NEIGHBOR_2WAY) && (oldstate >= NEIGHBOR_2WAY))
      ospf_iface_sm(ifa, ISM_NEICH);

    if (oldstate == NEIGHBOR_FULL)	/* Decrease number of adjacencies */
    {
      ifa->fadj--;
      schedule_rt_lsa(ifa->oa);
      if (ifa->type == OSPF_IT_VLINK) schedule_rt_lsa(ifa->voa);
      schedule_net_lsa(ifa);
    }

    if (state == NEIGHBOR_FULL)	/* Increase number of adjacencies */
    {
      ifa->fadj++;
      schedule_rt_lsa(ifa->oa);
      if (ifa->type == OSPF_IT_VLINK) schedule_rt_lsa(ifa->voa);
      schedule_net_lsa(ifa);
    }
    if (state == NEIGHBOR_EXSTART)
    {
      if (n->adj == 0)		/* First time adjacency */
      {
	n->dds = random_u32();
      }
      n->dds++;
      n->myimms.byte = 0;
      n->myimms.bit.ms = 1;
      n->myimms.bit.m = 1;
      n->myimms.bit.i = 1;
    }
    if (state > NEIGHBOR_EXSTART)
      n->myimms.bit.i = 0;
  }
}

struct ospf_neighbor *
electbdr(list nl)
{
  struct ospf_neighbor *neigh, *n1, *n2;

  n1 = NULL;
  n2 = NULL;
  WALK_LIST(neigh, nl)		/* First try those decl. themselves */
  {
    if (neigh->state >= NEIGHBOR_2WAY)	/* Higher than 2WAY */
      if (neigh->priority > 0)	/* Eligible */
	if (ipa_compare(neigh->ip, neigh->dr) != 0)	/* And not decl. itself DR */
	{
	  if (ipa_compare(neigh->ip, neigh->bdr) == 0)	/* Declaring BDR */
	  {
	    if (n1 != NULL)
	    {
	      if (neigh->priority > n1->priority)
		n1 = neigh;
	      else if (neigh->priority == n1->priority)
		if (neigh->rid > n1->rid)
		  n1 = neigh;
	    }
	    else
	    {
	      n1 = neigh;
	    }
	  }
	  else			/* And NOT declaring BDR */
	  {
	    if (n2 != NULL)
	    {
	      if (neigh->priority > n2->priority)
		n2 = neigh;
	      else if (neigh->priority == n2->priority)
		if (neigh->rid > n2->rid)
		  n2 = neigh;
	    }
	    else
	    {
	      n2 = neigh;
	    }
	  }
	}
  }
  if (n1 == NULL)
    n1 = n2;

  return (n1);
}

struct ospf_neighbor *
electdr(list nl)
{
  struct ospf_neighbor *neigh, *n;

  n = NULL;
  WALK_LIST(neigh, nl)		/* And now DR */
  {
    if (neigh->state >= NEIGHBOR_2WAY)	/* Higher than 2WAY */
      if (neigh->priority > 0)	/* Eligible */
	if (ipa_compare(neigh->ip, neigh->dr) == 0)	/* And declaring itself DR */
	{
	  if (n != NULL)
	  {
	    if (neigh->priority > n->priority)
	      n = neigh;
	    else if (neigh->priority == n->priority)
	      if (neigh->rid > n->rid)
		n = neigh;
	  }
	  else
	  {
	    n = neigh;
	  }
	}
  }

  return (n);
}

static int
can_do_adj(struct ospf_neighbor *n)
{
  struct ospf_iface *ifa;
  struct proto *p;
  int i;

  ifa = n->ifa;
  p = (struct proto *) (ifa->oa->po);
  i = 0;

  switch (ifa->type)
  {
  case OSPF_IT_PTP:
  case OSPF_IT_VLINK:
    i = 1;
    break;
  case OSPF_IT_BCAST:
  case OSPF_IT_NBMA:
    switch (ifa->state)
    {
    case OSPF_IS_DOWN:
      bug("%s: Iface %s in down state?", p->name, ifa->iface->name);
      break;
    case OSPF_IS_WAITING:
      DBG("%s: Neighbor? on iface %s\n", p->name, ifa->iface->name);
      break;
    case OSPF_IS_DROTHER:
      if (((n->rid == ifa->drid) || (n->rid == ifa->bdrid))
	  && (n->state >= NEIGHBOR_2WAY))
	i = 1;
      break;
    case OSPF_IS_PTP:
    case OSPF_IS_BACKUP:
    case OSPF_IS_DR:
      if (n->state >= NEIGHBOR_2WAY)
	i = 1;
      break;
    default:
      bug("%s: Iface %s in unknown state?", p->name, ifa->iface->name);
      break;
    }
    break;
  default:
    bug("%s: Iface %s is unknown type?", p->name, ifa->iface->name);
    break;
  }
  DBG("%s: Iface %s can_do_adj=%d\n", p->name, ifa->iface->name, i);
  return i;
}

/**
 * ospf_neigh_sm - ospf neighbor state machine
 * @n: neighor
 * @event: actual event
 *
 * This part implements the neighbor state machine as described in 10.3 of
 * RFC 2328. The only difference is that state %NEIGHBOR_ATTEMPT is not
 * used. We discover neighbors on nonbroadcast networks in the
 * same way as on broadcast networks. The only difference is in
 * sending hello packets. These are sent to IPs listed in
 * @ospf_iface->nbma_list .
 */
void
ospf_neigh_sm(struct ospf_neighbor *n, int event)
{
  struct proto_ospf *po = n->ifa->oa->po;
  struct proto *p = &po->proto;

  DBG("Neighbor state machine for neighbor %I, event \"%s\".", n->ip,
	     ospf_inm[event]);

  switch (event)
  {
  case INM_START:
    neigh_chstate(n, NEIGHBOR_ATTEMPT);
    /* NBMA are used different way */
    break;
  case INM_HELLOREC:
    switch (n->state)
    {
    case NEIGHBOR_ATTEMPT:
    case NEIGHBOR_DOWN:
      neigh_chstate(n, NEIGHBOR_INIT);
    default:
      tm_start(n->inactim, n->ifa->dead);	/* Restart inactivity timer */
      break;
    }
    break;
  case INM_2WAYREC:
    if (n->state < NEIGHBOR_2WAY)
      neigh_chstate(n, NEIGHBOR_2WAY);
    if ((n->state == NEIGHBOR_2WAY) && can_do_adj(n))
      neigh_chstate(n, NEIGHBOR_EXSTART);
    break;
  case INM_NEGDONE:
    if (n->state == NEIGHBOR_EXSTART)
    {
      neigh_chstate(n, NEIGHBOR_EXCHANGE);
      s_init(&(n->dbsi), &po->lsal);
      while (!EMPTY_LIST(n->ackl[ACKL_DELAY]))
      {
	struct lsah_n *no;
	no = (struct lsah_n *) HEAD(n->ackl[ACKL_DELAY]);
	rem_node(NODE no);
	mb_free(no);
      }
    }
    else
      bug("NEGDONE and I'm not in EXSTART?");
    break;
  case INM_EXDONE:
    neigh_chstate(n, NEIGHBOR_LOADING);
    break;
  case INM_LOADDONE:
    neigh_chstate(n, NEIGHBOR_FULL);
    break;
  case INM_ADJOK:
    switch (n->state)
    {
    case NEIGHBOR_2WAY:
      /* Can In build adjacency? */
      if (can_do_adj(n))
      {
	neigh_chstate(n, NEIGHBOR_EXSTART);
      }
      break;
    default:
      if (n->state >= NEIGHBOR_EXSTART)
	if (!can_do_adj(n))
	{
	  neigh_chstate(n, NEIGHBOR_2WAY);
	}
      break;
    }
    break;
  case INM_SEQMIS:
  case INM_BADLSREQ:
    if (n->state >= NEIGHBOR_EXCHANGE)
    {
      neigh_chstate(n, NEIGHBOR_EXSTART);
    }
    break;
  case INM_KILLNBR:
  case INM_LLDOWN:
  case INM_INACTTIM:
    neigh_chstate(n, NEIGHBOR_DOWN);
    break;
  case INM_1WAYREC:
    neigh_chstate(n, NEIGHBOR_INIT);
    break;
  default:
    bug("%s: INM - Unknown event?", p->name);
    break;
  }
}

/**
 * bdr_election - (Backup) Designed Router election
 * @ifa: actual interface
 *
 * When the wait timer fires, it is time to elect (Backup) Designated Router.
 * Structure describing me is added to this list so every electing router
 * has the same list. Backup Designated Router is elected before Designated
 * Router. This process is described in 9.4 of RFC 2328.
 */
void
bdr_election(struct ospf_iface *ifa)
{
  struct ospf_neighbor *neigh, *ndr, *nbdr, me;
  u32 myid;
  ip_addr ndrip, nbdrip;
  int doadj;
  struct proto *p = &ifa->oa->po->proto;

  DBG("(B)DR election.\n");

  myid = p->cf->global->router_id;

  me.state = NEIGHBOR_2WAY;
  me.rid = myid;
  me.priority = ifa->priority;
  me.dr = ifa->drip;
  me.bdr = ifa->bdrip;
  me.ip = ifa->iface->addr->ip;

  add_tail(&ifa->neigh_list, NODE & me);

  nbdr = electbdr(ifa->neigh_list);
  ndr = electdr(ifa->neigh_list);

  if (ndr == NULL)
    ndr = nbdr;

  if (((ifa->drid == myid) && (ndr != &me))
      || ((ifa->drid != myid) && (ndr == &me))
      || ((ifa->bdrid == myid) && (nbdr != &me))
      || ((ifa->bdrid != myid) && (nbdr == &me)))
  {
    if (ndr == NULL)
      ifa->drip = me.dr = IPA_NONE;
    else
      ifa->drip = me.dr = ndr->ip;

    if (nbdr == NULL)
      ifa->bdrip = me.bdr = IPA_NONE;
    else
      ifa->bdrip = me.bdr = nbdr->ip;

    nbdr = electbdr(ifa->neigh_list);
    ndr = electdr(ifa->neigh_list);
  }

  if (ndr == NULL)
    ndrip = IPA_NONE;
  else
    ndrip = ndr->ip;

  if (nbdr == NULL)
    nbdrip = IPA_NONE;
  else
    nbdrip = nbdr->ip;

  doadj = 0;
  if ((ipa_compare(ifa->drip, ndrip) != 0)
      || (ipa_compare(ifa->bdrip, nbdrip) != 0))
    doadj = 1;

  if (ndr == NULL)
  {
    ifa->drid = 0;
    ifa->drip = IPA_NONE;
  }
  else
  {
    ifa->drid = ndr->rid;
    ifa->drip = ndr->ip;
  }

  if (nbdr == NULL)
  {
    ifa->bdrid = 0;
    ifa->bdrip = IPA_NONE;
  }
  else
  {
    ifa->bdrid = nbdr->rid;
    ifa->bdrip = nbdr->ip;
  }

  DBG("DR=%I, BDR=%I\n", ifa->drid, ifa->bdrid);

  if (myid == ifa->drid)
    ospf_iface_chstate(ifa, OSPF_IS_DR);
  else
  {
    if (myid == ifa->bdrid)
      ospf_iface_chstate(ifa, OSPF_IS_BACKUP);
    else
      ospf_iface_chstate(ifa, OSPF_IS_DROTHER);
  }

  rem_node(NODE & me);

  if (doadj)
  {
    WALK_LIST(neigh, ifa->neigh_list)
    {
      ospf_neigh_sm(neigh, INM_ADJOK);
    }
  }
}

struct ospf_neighbor *
find_neigh(struct ospf_iface *ifa, u32 rid)
{
  struct ospf_neighbor *n;

  WALK_LIST(n, ifa->neigh_list) if (n->rid == rid)
    return n;
  return NULL;
}


/* Find a closest neighbor which is at least 2-Way */
struct ospf_neighbor *
find_neigh_noifa(struct proto_ospf *po, u32 rid)
{
  struct ospf_neighbor *n = NULL, *m;
  struct ospf_iface *ifa;

  WALK_LIST(ifa, po->iface_list) if ((m = find_neigh(ifa, rid)) != NULL)
  {
    if (m->state >= NEIGHBOR_2WAY)
    {
      if (n == NULL)
	n = m;
      else if (m->ifa->cost < n->ifa->cost)
	n = m;
    }
  }
  return n;
}

struct ospf_area *
ospf_find_area(struct proto_ospf *po, u32 aid)
{
  struct ospf_area *oa;
  WALK_LIST(oa, po->area_list)
    if (((struct ospf_area *) oa)->areaid == aid)
    return oa;
  return NULL;
}

/* Neighbor is inactive for a long time. Remove it. */
void
neighbor_timer_hook(timer * timer)
{
  struct ospf_neighbor *n = (struct ospf_neighbor *) timer->data;
  struct ospf_iface *ifa = n->ifa;
  struct proto *p = &ifa->oa->po->proto;

  OSPF_TRACE(D_EVENTS,
	     "Inactivity timer fired on interface %s for neighbor %I.",
	     ifa->iface->name, n->ip);
  ospf_neigh_remove(n);
}

void
ospf_neigh_remove(struct ospf_neighbor *n)
{
  struct ospf_iface *ifa = n->ifa;
  struct proto *p = &ifa->oa->po->proto;

  neigh_chstate(n, NEIGHBOR_DOWN);
  rem_node(NODE n);
  rfree(n->pool);
  OSPF_TRACE(D_EVENTS, "Deleting neigbor.");
}

void
ospf_sh_neigh_info(struct ospf_neighbor *n)
{
  struct ospf_iface *ifa = n->ifa;
  char *pos = "other";
  char etime[6];
  int exp, sec, min;

  exp = n->inactim->expires - now;
  sec = exp % 60;
  min = exp / 60;
  if (min > 59)
  {
    bsprintf(etime, "-Inf-");
  }
  else
  {
    bsprintf(etime, "%02u:%02u", min, sec);
  }

  if (n->rid == ifa->drid)
    pos = "dr   ";
  if (n->rid == ifa->bdrid)
    pos = "bdr  ";
  if ((n->ifa->type == OSPF_IT_PTP) || (n->ifa->type == OSPF_IT_VLINK))
    pos = "ptp  ";

  cli_msg(-1013, "%-1I\t%3u\t%s/%s\t%-5s\t%-1I\t%-10s", n->rid, n->priority,
	  ospf_ns[n->state], pos, etime, n->ip,
          (ifa->type == OSPF_IT_VLINK ? "vlink" : ifa->iface->name));
}

void
rxmt_timer_hook(timer * timer)
{
  struct ospf_neighbor *n = (struct ospf_neighbor *) timer->data;
  struct top_hash_entry *en;

  DBG("%s: RXMT timer fired on interface %s for neigh: %I.\n",
      p->name, ifa->iface->name, n->ip);

  if(n->state < NEIGHBOR_EXSTART) return;

  if (n->state == NEIGHBOR_EXSTART)
  {
    ospf_dbdes_send(n);
    return;
  }

  if ((n->state == NEIGHBOR_EXCHANGE) && n->myimms.bit.ms)	/* I'm master */
    ospf_dbdes_send(n);


  if (n->state < NEIGHBOR_FULL)	
    ospf_lsreq_send(n);	/* EXCHANGE or LOADING */
  else
  {
    if (!EMPTY_SLIST(n->lsrtl))	/* FULL */
    {
      list uplist;
      slab *upslab;
      struct l_lsr_head *llsh;

      init_list(&uplist);
      upslab = sl_new(n->pool, sizeof(struct l_lsr_head));

      WALK_SLIST(en, n->lsrtl)
      {
	if ((SNODE en)->next == (SNODE en))
	  bug("RTList is cycled");
	llsh = sl_alloc(upslab);
	llsh->lsh.id = en->lsa.id;
	llsh->lsh.rt = en->lsa.rt;
	llsh->lsh.type = en->lsa.type;
	DBG("Working on ID: %I, RT: %I, Type: %u\n",
	    en->lsa.id, en->lsa.rt, en->lsa.type);
	add_tail(&uplist, NODE llsh);
      }
      ospf_lsupd_send_list(n, &uplist);
      rfree(upslab);
    }
  }
}

void
ackd_timer_hook(timer * t)
{
  struct ospf_neighbor *n = t->data;
  ospf_lsack_send(n, ACKL_DELAY);
}
