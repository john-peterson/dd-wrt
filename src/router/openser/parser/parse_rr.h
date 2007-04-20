/*
 * $Id: parse_rr.h,v 1.2 2005/06/16 11:37:54 miconda Exp $
 *
 * Route & Record-Route Parser
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
 */


#ifndef PARSE_RR_H
#define PARSE_RR_H

#include <stdio.h>
#include "msg_parser.h"
#include "parse_nameaddr.h"
#include "parse_param.h"
#include "hf.h"


/*
 * Structure representing a Route & Record-Route HF body
 */
typedef struct rr {
	name_addr_t nameaddr; /* Name-addr part */
	param_t* r2;          /* Hook to r2 parameter */
	param_t* params;      /* Linked list of other parameters */
	int len;              /* Length of the whole route field */
	struct rr* next;      /* Next RR in the list */
} rr_t;


/*
 * Parse Route & Record-Route header fields
 */
int parse_rr(struct hdr_field* _r);

/*
 * Parse the body of Route & Record-Route headers
 */
int parse_rr_body(char *buf, int len, rr_t **head);

/*
 * Free list of rr
 * _c is head of the list
 */
void free_rr(rr_t** _r);


/*
 * Free list of rr
 * _c is head of the list
 */
void shm_free_rr(rr_t** _r);


/*
 * Print list of rrs, just for debugging
 */
void print_rr(FILE* _o, rr_t* _r);


/*
 * Duplicate a single rr_t structure using pkg_malloc
 */
int duplicate_rr(rr_t** _new, rr_t* _r);


/*
 * Duplicate a single rr_t structure using pkg_malloc
 */
int shm_duplicate_rr(rr_t** _new, rr_t* _r);


#endif /* PARSE_RR_H */
