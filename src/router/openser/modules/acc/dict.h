/*
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
 * WARNING: Don't forget to update the dictionary if you update this file !!!
 *
 * History:
 * --------
 * 2005-06-28  multi leg call support added (bogdan)
 *
 */

#ifndef _DICT_H
#define _DICT_H

struct attr {
	const char *n;
	int v;
};

struct val {
	const char *n;
	int v;
};

#define	A_USER_NAME						0
#define	A_SERVICE_TYPE					1
#define	A_CALLED_STATION_ID				2
#define	A_CALLING_STATION_ID			3
#define	A_ACCT_STATUS_TYPE				4
#define	A_ACCT_SESSION_ID				5
#define	A_SIP_METHOD					6
#define	A_SIP_RESPONSE_CODE				7
#define	A_SIP_CSEQ						8
#define	A_SIP_TO_TAG					9
#define	A_SIP_FROM_TAG					10
#define	A_SIP_TRANSLATED_REQUEST_URI	11
#define	A_DIGEST_RESPONSE				12
#define	A_DIGEST_ATTRIBUTES				13
#define	A_SIP_URI_USER					14
#define	A_DIGEST_REALM					15
#define	A_DIGEST_NONCE					16
#define	A_DIGEST_METHOD					17
#define	A_DIGEST_URI					18
#define	A_DIGEST_QOP					19
#define	A_DIGEST_ALGORITHM				20
#define	A_DIGEST_BODY_DIGEST			21
#define	A_DIGEST_CNONCE					22
#define	A_DIGEST_NONCE_COUNT			23
#define	A_DIGEST_USER_NAME				24
#define	A_SIP_GROUP						25
#define	A_CISCO_AVPAIR					26
#define	A_SIP_AVP						27
#define	A_SRC_LEG						28
#define	A_DST_LEG						29
#define	A_MAX							30

#define	V_STATUS_START			0
#define	V_STATUS_STOP			1
#define	V_STATUS_FAILED			2
#define	V_CALL_CHECK			3
#define	V_EMERGENCY_CALL		4
#define	V_SIP_SESSION			5
#define	V_GROUP_CHECK			6
#define	V_SIP_CALLER_AVPS		7
#define	V_SIP_CALLEE_AVPS		8
#define	V_MAX				9

#define	INIT_AV(rh, at, nr_at, vl, fn, e1, e2)					\
{									\
	int i;								\
	DICT_ATTR *da;							\
	DICT_VALUE *dv;							\
									\
	for (i = 0; i < nr_at; i++) {					\
		if (at[i].n == NULL)					\
			continue;					\
		da = rc_dict_findattr(rh, at[i].n);			\
		if (da == NULL) {					\
			LOG(L_ERR, "ERROR: %s: can't get code for the "	\
				   "%s attribute\n", fn, at[i].n);	\
			return e1;					\
		}							\
		at[i].v = da->value;					\
	}								\
	for (i = 0; i < V_MAX; i++) {					\
		if (vl[i].n == NULL)					\
			continue;					\
		dv = rc_dict_findval(rh, vl[i].n);			\
		if (dv == NULL) {					\
			LOG(L_ERR, "ERROR: %s: can't get code for the "	\
				   "%s attribute value\n", fn, vl[i].n);\
			return e2;					\
		}							\
		vl[i].v = dv->value;					\
	}								\
}

#endif
