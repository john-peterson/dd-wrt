/* 
 * $Id: db_val.h,v 1.4.2.1 2004/03/05 18:01:11 janakj Exp $ 
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
 */

#ifndef MY_DB_VAL_H
#define MY_DB_VAL_H

#include <mysql/mysql.h>
#include "../../db/db_val.h"

/*
 * Convert string to given type
 */
int str2val(db_type_t _t, db_val_t* _v, const char* _s, int _l);


/*
 * Convert given type to string
 */
int val2str(MYSQL* _c, db_val_t* _v, char* _s, int* _len);


#endif /* MY_DB_VAL_H */

