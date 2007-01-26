/*
 * $Id: parse_rpid.h,v 1.2 2005/06/16 11:37:54 miconda Exp $
 * 
 * Copyright (C) 2001-2003 Juha Heinanen
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
 
 
#ifndef PARSE_RPID_H
#define PARSE_RPID_H
 
#include "msg_parser.h"
 
 
/* casting macro for accessing RPID body */
#define get_rpid(p_msg)  ((struct to_body*)(p_msg)->rpid->parsed)


/*
 * RPID header field parser
 */
int parse_rpid_header( struct sip_msg *msg);
 
#endif /* PARSE_RPID_H */
