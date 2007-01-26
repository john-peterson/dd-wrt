/*
 * $Id: common.c,v 1.2 2005/08/23 08:17:07 bogdan_iancu Exp $
 *
 * Common stuff
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
 *	
 * History
 * ------
 * 2003-02-14 un-escaping added (janakj)
 *       

*/

#include <string.h> 
#include "../../dprint.h"
#include "../../ut.h"      /* q_memchr */
#include "../../parser/parse_uri.h"
#include "rerrno.h"
#include "reg_mod.h"
#include "common.h"


#define MAX_AOR_LEN 256

/*
 * Extract Address of Record
 */
int extract_aor(str* _uri, str* _a)
{
	static char aor_buf[MAX_AOR_LEN];
	str tmp;
	struct sip_uri puri;
	int user_len;

	if (parse_uri(_uri->s, _uri->len, &puri) < 0) {
		rerrno = R_AOR_PARSE;
		LOG(L_ERR, "extract_aor(): Error while parsing Address of Record\n");
		return -1;
	}
	
	if ((puri.user.len + puri.host.len + 1) > MAX_AOR_LEN) {
		rerrno = R_AOR_LEN;
		LOG(L_ERR, "extract_aor(): Address Of Record too long\n");
		return -2;
	}

	_a->s = aor_buf;
	_a->len = puri.user.len;

	if (un_escape(&puri.user, _a) < 0) {
		rerrno = R_UNESCAPE;
		LOG(L_ERR, "extract_aor(): Error while unescaping username\n");
		return -3;
	}

	user_len = _a->len;

	if (use_domain) {
		if (user_len)
			aor_buf[_a->len++] = '@';
		/* strip prefix (if defined) */
		if (realm_prefix.len && realm_prefix.len<puri.host.len &&
		(memcmp(realm_prefix.s, puri.host.s, realm_prefix.len)==0) ) {
			memcpy(aor_buf + _a->len, puri.host.s + realm_prefix.len,
					puri.host.len - realm_prefix.len);
			_a->len += puri.host.len - realm_prefix.len;
		} else {
			memcpy(aor_buf + _a->len, puri.host.s, puri.host.len);
			_a->len += puri.host.len;
		}
	}

	if (case_sensitive && user_len) {
		tmp.s = _a->s + user_len + 1;
		tmp.len = _a->s + _a->len - tmp.s;
		strlower(&tmp);
	} else {
		strlower(_a);
	}

	return 0;
}
