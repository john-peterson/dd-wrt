/*
 *	BIRD -- OSPF
 *
 *	(c) 2000--2004 Ondrej Filip <feela@network.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#include "ospf.h"

void
ospf_lsreq_send(struct ospf_neighbor *n)
{
  snode *sn;
  struct top_hash_entry *en;
  struct ospf_lsreq_packet *pk;
  struct ospf_packet *op;
  struct ospf_lsreq_header *lsh;
  u16 length;
  int i, j;
  struct proto *p = &n->ifa->oa->po->proto;

  pk = (struct ospf_lsreq_packet *) n->ifa->ip_sk->tbuf;
  op = (struct ospf_packet *) n->ifa->ip_sk->tbuf;

  ospf_pkt_fill_hdr(n->ifa, pk, LSREQ_P);

  sn = SHEAD(n->lsrql);
  if (EMPTY_SLIST(n->lsrql))
  {
    if (n->state == NEIGHBOR_LOADING)
      ospf_neigh_sm(n, INM_LOADDONE);
    return;
  }

  i = j = (ospf_pkt_maxsize(n->ifa) - sizeof(struct ospf_lsreq_packet)) /
    sizeof(struct ospf_lsreq_header);
  lsh = (struct ospf_lsreq_header *) (pk + 1);

  for (; i > 0; i--)
  {
    en = (struct top_hash_entry *) sn;
    lsh->padd1 = 0;
    lsh->padd2 = 0;
    lsh->type = en->lsa.type;
    lsh->rt = htonl(en->lsa.rt);
    lsh->id = htonl(en->lsa.id);
    DBG("Requesting %uth LSA: Type: %u, Id: %I, RT: %I\n", i, en->lsa.type,
	en->lsa.id, en->lsa.rt);
    lsh++;
    if (sn == STAIL(n->lsrql))
      break;
    sn = sn->next;
  }
  if (i != 0)
    i--;

  length =
    sizeof(struct ospf_lsreq_packet) + (j -
					i) * sizeof(struct ospf_lsreq_header);
  op->length = htons(length);
  ospf_send_to(n->ifa->ip_sk, n->ip, n->ifa);
  OSPF_TRACE(D_PACKETS, "LS request sent to: %I", n->rid);
}

void
ospf_lsreq_receive(struct ospf_lsreq_packet *ps,
		   struct ospf_iface *ifa, struct ospf_neighbor *n)
{
  struct ospf_lsreq_header *lsh;
  struct l_lsr_head *llsh;
  list uplist;
  slab *upslab;
  unsigned int size = ntohs(ps->ospf_packet.length);
  int i, lsano;
  struct ospf_area *oa = ifa->oa;
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;

  if (n->state < NEIGHBOR_EXCHANGE)
    return;

  OSPF_TRACE(D_EVENTS, "Received LS req from neighbor: %I", n->ip);
  ospf_neigh_sm(n, INM_HELLOREC);

  lsh = (void *) (ps + 1);
  init_list(&uplist);
  upslab = sl_new(n->pool, sizeof(struct l_lsr_head));

  lsano = (size - sizeof(struct ospf_lsreq_packet)) /
    sizeof(struct ospf_lsreq_header);
  for (i = 0; i < lsano; lsh++, i++)
  {
    DBG("Processing LSA: ID=%I, Type=%u, Router=%I\n", ntohl(lsh->id),
	lsh->type, ntohl(lsh->rt));
    llsh = sl_alloc(upslab);
    llsh->lsh.id = ntohl(lsh->id);
    llsh->lsh.rt = ntohl(lsh->rt);
    llsh->lsh.type = lsh->type;
    add_tail(&uplist, NODE llsh);
    if (ospf_hash_find(po->gr, oa->areaid, llsh->lsh.id, llsh->lsh.rt,
		       llsh->lsh.type) == NULL)
    {
      log(L_WARN
	  "Received bad LS req from: %I looking: RT: %I, ID: %I, Type: %u",
	  n->ip, lsh->rt, lsh->id, lsh->type);
      ospf_neigh_sm(n, INM_BADLSREQ);
      rfree(upslab);
      return;
    }
  }
  ospf_lsupd_send_list(n, &uplist);
  rfree(upslab);
}
