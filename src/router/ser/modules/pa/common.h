/*
 * $Id: common.h,v 1.1.1.1 2005/06/13 16:47:41 bogdan_iancu Exp $
 *
 * Presence Agent, common functions
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

#ifndef COMMON_H
#define COMMON_H

#include "../../str.h"

/*
 * Append a string app with length app_len
 * to the end of string str which is a str* pointer
 * the buffer must be large enough
 */
#define str_append(str, app, app_len)                    \
    do {                                                 \
        memcpy((str)->s + (str)->len, (app), (app_len)); \
        (str)->len += (app_len);                         \
    } while(0)


#endif /* COMMON_H */
