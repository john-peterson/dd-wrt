/*
 * $Id: cpl_utils.h,v 1.2 2003/09/03 17:42:10 bogdan Exp $
 *
 * Copyright (C) 2001-2003 Fhg Fokus
 *
 * This file is part of ser, a free SIP server.
 *
 * ser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * For a license to use the ser software under conditions
 * other than those described here, or to purchase support for this
 * software, please contact iptel.org by e-mail at the following addresses:
 *    info@iptel.org
 *
 * ser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * History:
 * -------
 * 2003-06-24: file created (bogdan)

 */

#ifndef _CPL_UTILS_H
#define _CPL_UTILS_H

#include <ctype.h>
#include "../../str.h"


/* looks for s2 into s1 */
static inline char *strcasestr_str(str *s1, str *s2)
{
	int i,j;
	for(i=0;i<s1->len-s2->len;i++) {
		for(j=0;j<s2->len;j++) {
			if ( !((s1->s[i+j]==s2->s[j]) ||
			( isalpha((int)s1->s[i+j]) && ((s1->s[i+j])^(s2->s[j]))==0x20 )) )
				break;
		}
		if (j==s2->len)
			return s1->s+i;
	}
	return 0;
}



#endif




