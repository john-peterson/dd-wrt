/*
 * $Id: xjab_load.h,v 1.1.1.1 2005/06/13 16:47:39 bogdan_iancu Exp $
 *
 * Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of openser, a free SIP server.
 *
 * openser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * openser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef _XJAB_LOAD_H_
#define _XJAB_LOAD_H_

#include "../../sr_module.h"
#include "../../str.h"
#include "xjab_base.h"

#define XJ_NO_SCRIPT_F	1

typedef int (*pa_register_watcher_f)(str*, str *, void*, void*);
typedef int (*pa_unregister_watcher_f)(str*, str *, void*, void*);

struct xjab_binds {
	pa_register_watcher_f register_watcher;
	pa_unregister_watcher_f unregister_watcher;
};

typedef int(*load_xjab_f)(struct xjab_binds*);

int load_xjab(struct xjab_binds*);
void xj_register_watcher(str*, str *, void*, void*);
void xj_unregister_watcher(str*, str *, void*, void*);

#endif

