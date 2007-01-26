/*
 * $Id: dset.c,v 1.7 2005/10/24 16:54:52 bogdan_iancu Exp $
 *
 * destination set
 *
 * Copyright (C) 2001-2004 FhG FOKUS
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

#include <string.h>
#include "dprint.h"
#include "config.h"
#include "parser/parser_f.h"
#include "parser/msg_parser.h"
#include "ut.h"
#include "hash_func.h"
#include "error.h"
#include "dset.h"
#include "mem/mem.h"
#include "ip_addr.h"

#define CONTACT "Contact: "
#define CONTACT_LEN (sizeof(CONTACT) - 1)

#define CONTACT_DELIM ", "
#define CONTACT_DELIM_LEN (sizeof(CONTACT_DELIM) - 1)

#define Q_PARAM ">;q="
#define Q_PARAM_LEN (sizeof(Q_PARAM) - 1)

struct branch
{
	char uri[MAX_URI_SIZE];
	unsigned int len;

	/* Real destination of the request */
	char dst_uri[MAX_URI_SIZE];
	unsigned int dst_uri_len;

	int q; /* Preference of the contact among contact within the array */
	struct socket_info* force_send_socket;
	int flags;
};


/* 
 * Where we store URIs of additional transaction branches
 * (-1 because of the default branch, #0)
 */
static struct branch branches[MAX_BRANCHES - 1];

/* how many of them we have */
unsigned int nr_branches = 0;

/* The q parameter of the Request-URI */
static qvalue_t ruri_q = Q_UNSPECIFIED; 


/*
 * Return the next branch from the dset
 * array, 0 is returned if there are no
 * more branches
 */
char* get_branch(int idx, int* len, qvalue_t* q, str* dst_uri, int *flags,
		struct socket_info** force_socket)
{
	if (idx < nr_branches) {
		*len = branches[idx].len;
		*q = branches[idx].q;
		if (dst_uri) {
			dst_uri->len = branches[idx].dst_uri_len;
			dst_uri->s = (dst_uri->len)?branches[idx].dst_uri:0;
		}
		if (force_socket)
			*force_socket = branches[idx].force_send_socket;
		if (flags)
			*flags = branches[idx].flags;
		return branches[idx].uri;
	} else {
		*len = 0;
		*q = Q_UNSPECIFIED;
		if (dst_uri) {
			dst_uri->s = 0;
			dst_uri->len = 0;
		}
		if (force_socket)
			*force_socket = 0;
		if (flags)
			*flags = 0;
		return 0;
	}
}


/*
 * Empty the dset array
 */
void clear_branches(void)
{
	nr_branches = 0;
	ruri_q = Q_UNSPECIFIED;
}


/* 
 * Add a new branch to current transaction 
 */
int append_branch(struct sip_msg* msg, str* uri, str* dst_uri, qvalue_t q,
		int flags, struct socket_info* force_socket)
{
	str luri;

	/* if we have already set up the maximum number
	 * of branches, don't try new ones 
	 */
	if (nr_branches == MAX_BRANCHES - 1) {
		LOG(L_ERR, "ERROR: append_branch: max nr of branches exceeded\n");
		ser_error = E_TOO_MANY_BRANCHES;
		return -1;
	}

	/* if not parameterized, take current uri */
	if (uri==0 || uri->len==0 || uri->s==0) {
		if (msg->new_uri.s)
			luri = msg->new_uri;
		else
			luri = msg->first_line.u.request.uri;
	} else {
		luri = *uri;
	}

	if (luri.len > MAX_URI_SIZE - 1) {
		LOG(L_ERR, "ERROR: append_branch: too long uri: %.*s\n",
		    luri.len, luri.s);
		return -1;
	}

	memcpy(branches[nr_branches].uri, luri.s, luri.len);
	/* be safe -- add zero termination */
	branches[nr_branches].uri[luri.len] = 0;
	branches[nr_branches].len = luri.len;
	branches[nr_branches].q = q;

	if (dst_uri && dst_uri->len && dst_uri->s) {
		memcpy(branches[nr_branches].dst_uri, dst_uri->s, dst_uri->len);
		branches[nr_branches].dst_uri[dst_uri->len] = 0;
		branches[nr_branches].dst_uri_len = dst_uri->len;
	} else {
		branches[nr_branches].dst_uri[0] = '\0';
		branches[nr_branches].dst_uri_len = 0;
	}

	branches[nr_branches].force_send_socket = force_socket;
	branches[nr_branches].flags = flags;

	nr_branches++;
	return 1;
}


/*
 * Create a Contact header field from the dset
 * array
 */
char* print_dset(struct sip_msg* msg, int* len) 
{
	int cnt, i, idx;
	unsigned int qlen;
	qvalue_t q;
	str uri;
	char* p, *qbuf;
	static char dset[MAX_REDIRECTION_LEN];

	if (msg->new_uri.s) {
		cnt = 1;
		*len = msg->new_uri.len;
		if (ruri_q != Q_UNSPECIFIED) {
			*len += 1 + Q_PARAM_LEN + len_q(ruri_q);
		}
	} else {
		cnt = 0;
		*len = 0;
	}

	for( idx=0 ; (uri.s=get_branch(idx,&uri.len,&q,0,0,0))!=0 ; idx++ ) {
		cnt++;
		*len += uri.len;
		if (q != Q_UNSPECIFIED) {
			*len += 1 + Q_PARAM_LEN + len_q(q);
		}
	}

	if (cnt == 0) return 0;	

	*len += CONTACT_LEN + CRLF_LEN + (cnt - 1) * CONTACT_DELIM_LEN;

	if (*len + 1 > MAX_REDIRECTION_LEN) {
		LOG(L_ERR, "ERROR: redirection buffer length exceed\n");
		return 0;
	}

	memcpy(dset, CONTACT, CONTACT_LEN);
	p = dset + CONTACT_LEN;
	if (msg->new_uri.s) {
		if (ruri_q != Q_UNSPECIFIED) {
			*p++ = '<';
		}

		memcpy(p, msg->new_uri.s, msg->new_uri.len);
		p += msg->new_uri.len;

		if (ruri_q != Q_UNSPECIFIED) {
			memcpy(p, Q_PARAM, Q_PARAM_LEN);
			p += Q_PARAM_LEN;

			qbuf = q2str(ruri_q, &qlen);
			memcpy(p, qbuf, qlen);
			p += qlen;
		}
		i = 1;
	} else {
		i = 0;
	}

	for( idx=0 ; (uri.s=get_branch(idx,&uri.len,&q,0,0,0))!=0 ; idx++ ) {
		if (i) {
			memcpy(p, CONTACT_DELIM, CONTACT_DELIM_LEN);
			p += CONTACT_DELIM_LEN;
		}

		if (q != Q_UNSPECIFIED) {
			*p++ = '<';
		}

		memcpy(p, uri.s, uri.len);
		p += uri.len;
		if (q != Q_UNSPECIFIED) {
			memcpy(p, Q_PARAM, Q_PARAM_LEN);
			p += Q_PARAM_LEN;

			qbuf = q2str(q, &qlen);
			memcpy(p, qbuf, qlen);
			p += qlen;
		}
		i++;
	}

	memcpy(p, CRLF " ", CRLF_LEN + 1);
	return dset;
}


/*
 * Sets the q parameter of the Request-URI
 */
void set_ruri_q(qvalue_t q)
{
	ruri_q = q;
}


/*
 * Return the q value of the Request-URI
 */
qvalue_t get_ruri_q(void)
{
	return ruri_q;
}



/*
 * Get actual Request-URI
 */
int get_request_uri(struct sip_msg* _m, str* _u)
{
	/* Use new_uri if present */
	if (_m->new_uri.s) {
		_u->s = _m->new_uri.s;
		_u->len = _m->new_uri.len;
	} else {
		_u->s = _m->first_line.u.request.uri.s;
		_u->len = _m->first_line.u.request.uri.len;
	}

	return 0;
}


/*
 * Rewrite Request-URI
 */
int rewrite_uri(struct sip_msg* _m, str* _s)
{
        char* buf;

        buf = (char*)pkg_malloc(_s->len + 1);
        if (!buf) {
                LOG(L_ERR, "ERROR: rewrite_uri: No memory left\n");
                return -1;
        }

        memcpy(buf, _s->s, _s->len);
        buf[_s->len] = '\0';

        _m->parsed_uri_ok = 0;
        if (_m->new_uri.s) {
                pkg_free(_m->new_uri.s);
        }

        _m->new_uri.s = buf;
        _m->new_uri.len = _s->len;

        DBG("rewrite_uri: Rewriting Request-URI with '%.*s'\n", _s->len, 
																		   buf);
        return 1;
}

