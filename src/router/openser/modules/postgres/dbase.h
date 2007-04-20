/*
 * $Id: dbase.h,v 1.1.1.1 2005/06/13 16:47:44 bogdan_iancu Exp $
 *
 * POSTGRES module, portions of this code were templated using
 * the mysql module, thus it's similarity.
 *
 * Copyright (C) 2003 August.Net Services, LLC
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
 * ---
 *
 * History
 * -------
 * 2003-04-06 initial code written (Greg Fausak/Andy Fullford)
 *
 */


#ifndef DBASE_H
#define DBASE_H

#include "../../db/db_con.h"
#include "../../db/db_res.h"
#include "../../db/db_key.h"
#include "../../db/db_op.h"
#include "../../db/db_val.h"


/*
 * Initialize database connection
 */
db_con_t* db_init(const char* _sqlurl);

/*
 * Close a database connection
 */
void db_close(db_con_t* _h);

/*
 * Return result of previous query
 */
int get_result(db_con_t* _h, db_res_t** _r);

/*
 * create a new result anchored on parent memory
 */
db_res_t* new_result_pg(char *parent);


/*
 * Free all memory allocated by get_result
 */
int db_free_query(db_con_t* _h, db_res_t* _r);


/*
 * Do a query
 */
int db_query(db_con_t* _h, db_key_t* _k, db_op_t* _op, db_val_t* _v, db_key_t* _c, int _n, int _nc,
	     db_key_t _o, db_res_t** _r);


/*
 * Raw SQL query
 */
int db_raw_query(db_con_t* _h, char* _s, db_res_t** _r);


/*
 * Insert a row into table
 */
int db_insert(db_con_t* _h, db_key_t* _k, db_val_t* _v, int _n);


/*
 * Delete a row from table
 */
int db_delete(db_con_t* _h, db_key_t* _k, db_op_t* _o, db_val_t* _v, int _n);


/*
 * Update a row in table
 */
int db_update(db_con_t* _h, db_key_t* _k, db_op_t* _o, db_val_t* _v,
	      db_key_t* _uk, db_val_t* _uv, int _n, int _un);


/*
 * Store name of table that will be used by
 * subsequent database functions
 */
int use_table(db_con_t* _h, const char* _t);

int val2str(db_val_t* _v, char* _s, int* _len);

int free_result(db_res_t* _r);

int convert_result(db_con_t* _h, db_res_t* _r);

#endif /* DBASE_H */
