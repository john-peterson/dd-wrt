/* 
   Unix SMB/CIFS implementation.
   Manage connections_struct structures
   Copyright (C) Andrew Tridgell 1998
   Copyright (C) Alexander Bokovoy 2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "includes.h"

/* The connections bitmap is expanded in increments of BITMAP_BLOCK_SZ. The
 * maximum size of the bitmap is the largest positive integer, but you will hit
 * the "max connections" limit, looong before that.
 */
#define BITMAP_BLOCK_SZ 128

static connection_struct *Connections;

/* number of open connections */
static struct bitmap *bmap;
static int num_open;

/****************************************************************************
init the conn structures
****************************************************************************/
void conn_init(void)
{
	bmap = bitmap_allocate(BITMAP_BLOCK_SZ);
}

/****************************************************************************
return the number of open connections
****************************************************************************/
int conn_num_open(void)
{
	return num_open;
}


/****************************************************************************
check if a snum is in use
****************************************************************************/
bool conn_snum_used(int snum)
{
	connection_struct *conn;
	for (conn=Connections;conn;conn=conn->next) {
		if (conn->params->service == snum) {
			return(True);
		}
	}
	return(False);
}

/****************************************************************************
 Find a conn given a cnum.
****************************************************************************/

connection_struct *conn_find(unsigned cnum)
{
	int count=0;
	connection_struct *conn;

	for (conn=Connections;conn;conn=conn->next,count++) {
		if (conn->cnum == cnum) {
			if (count > 10) {
				DLIST_PROMOTE(Connections, conn);
			}
			return conn;
		}
	}

	return NULL;
}

/****************************************************************************
  find first available connection slot, starting from a random position.
The randomisation stops problems with the server dieing and clients
thinking the server is still available.
****************************************************************************/
connection_struct *conn_new(void)
{
	connection_struct *conn;
	int i;
        int find_offset = 1;

find_again:
	i = bitmap_find(bmap, find_offset);
	
	if (i == -1) {
                /* Expand the connections bitmap. */
                int             oldsz = bmap->n;
                int             newsz = bmap->n + BITMAP_BLOCK_SZ;
                struct bitmap * nbmap;

                if (newsz <= oldsz) {
                        /* Integer wrap. */
		        DEBUG(0,("ERROR! Out of connection structures\n"));
                        return NULL;
                }

		DEBUG(4,("resizing connections bitmap from %d to %d\n",
                        oldsz, newsz));

                nbmap = bitmap_allocate(newsz);
		if (!nbmap) {
			DEBUG(0,("ERROR! malloc fail.\n"));
			return NULL;
		}

                bitmap_copy(nbmap, bmap);
                bitmap_free(bmap);

                bmap = nbmap;
                find_offset = oldsz; /* Start next search in the new portion. */

                goto find_again;
	}

	/* The bitmap position is used below as the connection number
	 * conn->cnum). This ends up as the TID field in the SMB header,
	 * which is limited to 16 bits (we skip 0xffff which is the
	 * NULL TID).
	 */
	if (i > 65534) {
		DEBUG(0, ("Maximum connection limit reached\n"));
		return NULL;
	}

	if (!(conn=TALLOC_ZERO_P(NULL, connection_struct)) ||
	    !(conn->params = TALLOC_P(conn, struct share_params))) {
		DEBUG(0,("TALLOC_ZERO() failed!\n"));
		TALLOC_FREE(conn);
		return NULL;
	}
	conn->cnum = i;
	conn->force_group_gid = (gid_t)-1;

	bitmap_set(bmap, i);

	num_open++;

	string_set(&conn->dirpath,"");
	string_set(&conn->connectpath,"");
	string_set(&conn->origpath,"");
	
	DLIST_ADD(Connections, conn);

	return conn;
}

/****************************************************************************
 Close all conn structures.
return true if any were closed
****************************************************************************/
bool conn_close_all(void)
{
	connection_struct *conn, *next;
	bool ret = false;
	for (conn=Connections;conn;conn=next) {
		next=conn->next;
		set_current_service(conn, 0, True);
		close_cnum(conn, conn->vuid);
		ret = true;
	}
	return ret;
}

/****************************************************************************
 Idle inactive connections.
****************************************************************************/

bool conn_idle_all(time_t t)
{
	int deadtime = lp_deadtime()*60;
	pipes_struct *plist = NULL;
	connection_struct *conn;

	if (deadtime <= 0)
		deadtime = DEFAULT_SMBD_TIMEOUT;

	for (conn=Connections;conn;conn=conn->next) {

		time_t age = t - conn->lastused;

		/* Update if connection wasn't idle. */
		if (conn->lastused != conn->lastused_count) {
			conn->lastused = t;
			conn->lastused_count = t;
		}

		/* close dirptrs on connections that are idle */
		if (age > DPTR_IDLE_TIMEOUT) {
			dptr_idlecnum(conn);
		}

		if (conn->num_files_open > 0 || age < deadtime) {
			return False;
		}
	}

	/*
	 * Check all pipes for any open handles. We cannot
	 * idle with a handle open.
	 */

	for (plist = get_first_internal_pipe(); plist;
	     plist = get_next_internal_pipe(plist)) {
		if (plist->pipe_handles && plist->pipe_handles->count) {
			return False;
		}
	}
	
	return True;
}

/****************************************************************************
 Clear a vuid out of the validity cache, and as the 'owner' of a connection.
****************************************************************************/

void conn_clear_vuid_caches(uint16_t vuid)
{
	connection_struct *conn;

	for (conn=Connections;conn;conn=conn->next) {
		if (conn->vuid == vuid) {
			conn->vuid = UID_FIELD_INVALID;
		}
		conn_clear_vuid_cache(conn, vuid);
	}
}

/****************************************************************************
 Free a conn structure - internal part.
****************************************************************************/

void conn_free_internal(connection_struct *conn)
{
 	vfs_handle_struct *handle = NULL, *thandle = NULL;
	struct trans_state *state = NULL;

	/* Free vfs_connection_struct */
	handle = conn->vfs_handles;
	while(handle) {
		thandle = handle->next;
		DLIST_REMOVE(conn->vfs_handles, handle);
		if (handle->free_data)
			handle->free_data(&handle->data);
		handle = thandle;
	}

	/* Free any pending transactions stored on this conn. */
	for (state = conn->pending_trans; state; state = state->next) {
		/* state->setup is a talloc child of state. */
		SAFE_FREE(state->param);
		SAFE_FREE(state->data);
	}

	free_namearray(conn->veto_list);
	free_namearray(conn->hide_list);
	free_namearray(conn->veto_oplock_list);
	free_namearray(conn->aio_write_behind_list);
	
	string_free(&conn->dirpath);
	string_free(&conn->connectpath);
	string_free(&conn->origpath);

	ZERO_STRUCTP(conn);
	talloc_destroy(conn);
}

/****************************************************************************
 Free a conn structure.
****************************************************************************/

void conn_free(connection_struct *conn)
{
	DLIST_REMOVE(Connections, conn);

	bitmap_clear(bmap, conn->cnum);

	SMB_ASSERT(num_open > 0);
	num_open--;

	conn_free_internal(conn);
}
 
/****************************************************************************
receive a smbcontrol message to forcibly unmount a share
the message contains just a share name and all instances of that
share are unmounted
the special sharename '*' forces unmount of all shares
****************************************************************************/
void msg_force_tdis(struct messaging_context *msg,
		    void *private_data,
		    uint32_t msg_type,
		    struct server_id server_id,
		    DATA_BLOB *data)
{
	connection_struct *conn, *next;
	fstring sharename;

	fstrcpy(sharename, (const char *)data->data);

	if (strcmp(sharename, "*") == 0) {
		DEBUG(1,("Forcing close of all shares\n"));
		conn_close_all();
		return;
	}

	for (conn=Connections;conn;conn=next) {
		next=conn->next;
		if (strequal(lp_servicename(SNUM(conn)), sharename)) {
			DEBUG(1,("Forcing close of share %s cnum=%d\n",
				 sharename, conn->cnum));
			close_cnum(conn, (uint16)-1);
		}
	}
}
