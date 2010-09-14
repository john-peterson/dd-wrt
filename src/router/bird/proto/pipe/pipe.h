/*
 *	BIRD -- Table-to-Table Routing Protocol a.k.a Pipe
 *
 *	(c) 1999 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#ifndef _BIRD_PIPE_H_
#define _BIRD_PIPE_H_

#define PIPE_OPAQUE 0
#define PIPE_TRANSPARENT 1

struct pipe_config {
  struct proto_config c;
  struct rtable_config *peer;		/* Table we're connected to */
  int mode;				/* PIPE_OPAQUE or PIPE_TRANSPARENT */
};

struct pipe_proto {
  struct proto p;
  struct rtable *peer;
  struct proto_stats peer_stats;	/* Statistics for the direction peer->primary */
  int mode;				/* PIPE_OPAQUE or PIPE_TRANSPARENT */
};


extern struct protocol proto_pipe;

static inline int proto_is_pipe(struct proto *p)
{ return p->proto == &proto_pipe; }

static inline struct rtable * pipe_get_peer_table(struct proto *P)
{ return ((struct pipe_proto *) P)->peer; }

static inline struct proto_stats * pipe_get_peer_stats(struct proto *P)
{ return &((struct pipe_proto *) P)->peer_stats; }

#endif
