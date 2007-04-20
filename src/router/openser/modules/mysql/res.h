/* 
 * $Id: res.h,v 1.2 2005/06/16 12:41:52 bogdan_iancu Exp $ 
 *
 * MySQL module result related functions
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

#ifndef RES_H
#define RES_H

#include "../../db/db_res.h"
#include "../../db/db_con.h"


/*
 * Create a new result structure and initialize it
 */
db_res_t* new_result(void);


/*
 * Fill the structure with data from database
 */
int convert_result(db_con_t* _h, db_res_t* _r);


/*
 * Release memory used by a result structure
 */
int free_result(db_res_t* _r);


#endif /* RES_H */
