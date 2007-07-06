/*
 * netlink/route/class.h       Classes
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_CLASS_H_
#define NETLINK_CLASS_H_

#include <netlink/netlink.h>
#include <netlink/route/tc.h>
#include <netlink/route/qdisc.h>

struct rtnl_class;

/* General */
extern struct rtnl_class *	rtnl_class_alloc(void);
extern void			rtnl_class_put(struct rtnl_class *);
extern void			rtnl_class_free(struct rtnl_class *);
extern struct nl_cache *	rtnl_class_alloc_cache(struct nl_handle *, int);

/* leaf qdisc access */
extern int			rtnl_class_has_leaf_qdisc(struct rtnl_class *);
extern struct rtnl_qdisc *	rtnl_class_leaf_qdisc(struct rtnl_class *,
						      struct nl_cache *);

/* class addition */
extern struct nl_msg * rtnl_class_build_add_request(struct rtnl_class *, int);
extern int rtnl_class_add(struct nl_handle *, struct rtnl_class *, int);

/* attribute modification */
extern void		rtnl_class_set_ifindex(struct rtnl_class *, int);
extern int		rtnl_class_get_ifindex(struct rtnl_class *);
extern void		rtnl_class_set_handle(struct rtnl_class *, uint32_t);
extern uint32_t		rtnl_class_get_handle(struct rtnl_class *);
extern void		rtnl_class_set_parent(struct rtnl_class *, uint32_t);
extern uint32_t		rtnl_class_get_parent(struct rtnl_class *);
extern void		rtnl_class_set_kind(struct rtnl_class *, const char *);
extern char *		rtnl_class_get_kind(struct rtnl_class *);
extern uint64_t		rtnl_class_get_stat(struct rtnl_class *,
					    enum rtnl_tc_stats_id);

/* iterators */
extern void		rtnl_class_foreach_child(struct rtnl_class *,
						 struct nl_cache *,
						 void (*cb)(struct nl_object *,
						 	    void *),
						 void *);
extern void		rtnl_class_foreach_cls(struct rtnl_class *,
					       struct nl_cache *,
					       void (*cb)(struct nl_object *,
							  void *),
					       void *);

#endif
