/*
   Unix SMB/CIFS implementation.

   Extract the user/system database from a remote SamSync server

   Copyright (C) Andrew Bartlett <abartlet@samba.org> 2004-2005
   Copyright (C) Guenther Deschner <gd@samba.org> 2008

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
#include "libnet/libnet.h"

/**
 * Decrypt and extract the user's passwords.
 *
 * The writes decrypted (no longer 'RID encrypted' or arcfour encrypted)
 * passwords back into the structure
 */

static NTSTATUS fix_user(TALLOC_CTX *mem_ctx,
			 DATA_BLOB *session_key,
			 enum netr_SamDatabaseID database_id,
			 struct netr_DELTA_ENUM *delta)
{

	uint32_t rid = delta->delta_id_union.rid;
	struct netr_DELTA_USER *user = delta->delta_union.user;
	struct samr_Password lm_hash;
	struct samr_Password nt_hash;
	unsigned char zero_buf[16];

	memset(zero_buf, '\0', sizeof(zero_buf));

	/* Note that win2000 may send us all zeros
	 * for the hashes if it doesn't
	 * think this channel is secure enough. */
	if (user->lm_password_present) {
		if (memcmp(user->lmpassword.hash, zero_buf, 16) != 0) {
			sam_pwd_hash(rid, user->lmpassword.hash, lm_hash.hash, 0);
		} else {
			memset(lm_hash.hash, '\0', sizeof(lm_hash.hash));
		}
		user->lmpassword = lm_hash;
	}

	if (user->nt_password_present) {
		if (memcmp(user->ntpassword.hash, zero_buf, 16) != 0) {
			sam_pwd_hash(rid, user->ntpassword.hash, nt_hash.hash, 0);
		} else {
			memset(nt_hash.hash, '\0', sizeof(nt_hash.hash));
		}
		user->ntpassword = nt_hash;
	}

	if (user->user_private_info.SensitiveData) {
		DATA_BLOB data;
		struct netr_USER_KEYS keys;
		enum ndr_err_code ndr_err;
		data.data = user->user_private_info.SensitiveData;
		data.length = user->user_private_info.DataLength;
		SamOEMhashBlob(data.data, data.length, session_key);
		user->user_private_info.SensitiveData = data.data;
		user->user_private_info.DataLength = data.length;

		ndr_err = ndr_pull_struct_blob(&data, mem_ctx, &keys,
			(ndr_pull_flags_fn_t)ndr_pull_netr_USER_KEYS);
		if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
			dump_data(10, data.data, data.length);
			return ndr_map_error2ntstatus(ndr_err);
		}

		/* Note that win2000 may send us all zeros
		 * for the hashes if it doesn't
		 * think this channel is secure enough. */
		if (keys.keys.keys2.lmpassword.length == 16) {
			if (memcmp(keys.keys.keys2.lmpassword.pwd.hash,
					zero_buf, 16) != 0) {
				sam_pwd_hash(rid,
					keys.keys.keys2.lmpassword.pwd.hash,
					lm_hash.hash, 0);
			} else {
				memset(lm_hash.hash, '\0', sizeof(lm_hash.hash));
			}
			user->lmpassword = lm_hash;
			user->lm_password_present = true;
		}
		if (keys.keys.keys2.ntpassword.length == 16) {
			if (memcmp(keys.keys.keys2.ntpassword.pwd.hash,
						zero_buf, 16) != 0) {
				sam_pwd_hash(rid,
					keys.keys.keys2.ntpassword.pwd.hash,
					nt_hash.hash, 0);
			} else {
				memset(nt_hash.hash, '\0', sizeof(nt_hash.hash));
			}
			user->ntpassword = nt_hash;
			user->nt_password_present = true;
		}
		/* TODO: rid decrypt history fields */
	}
	return NT_STATUS_OK;
}

/**
 * Decrypt and extract the secrets
 *
 * The writes decrypted secrets back into the structure
 */
static NTSTATUS fix_secret(TALLOC_CTX *mem_ctx,
			   DATA_BLOB *session_key,
			   enum netr_SamDatabaseID database_id,
			   struct netr_DELTA_ENUM *delta)
{
	struct netr_DELTA_SECRET *secret = delta->delta_union.secret;

	SamOEMhashBlob(secret->current_cipher.cipher_data,
		       secret->current_cipher.maxlen,
		       session_key);

	SamOEMhashBlob(secret->old_cipher.cipher_data,
		       secret->old_cipher.maxlen,
		       session_key);

	return NT_STATUS_OK;
}

/**
 * Fix up the delta, dealing with encryption issues so that the final
 * callback need only do the printing or application logic
 */

static NTSTATUS samsync_fix_delta(TALLOC_CTX *mem_ctx,
				  DATA_BLOB *session_key,
				  enum netr_SamDatabaseID database_id,
				  struct netr_DELTA_ENUM *delta)
{
	NTSTATUS status = NT_STATUS_OK;

	switch (delta->delta_type) {
		case NETR_DELTA_USER:

			status = fix_user(mem_ctx,
					  session_key,
					  database_id,
					  delta);
			break;
		case NETR_DELTA_SECRET:

			status = fix_secret(mem_ctx,
					    session_key,
					    database_id,
					    delta);
			break;
		default:
			break;
	}

	return status;
}

/**
 * Fix up the delta, dealing with encryption issues so that the final
 * callback need only do the printing or application logic
 */

static NTSTATUS samsync_fix_delta_array(TALLOC_CTX *mem_ctx,
					DATA_BLOB *session_key,
					enum netr_SamDatabaseID database_id,
					struct netr_DELTA_ENUM_ARRAY *r)
{
	NTSTATUS status;
	int i;

	for (i = 0; i < r->num_deltas; i++) {

		status = samsync_fix_delta(mem_ctx,
					   session_key,
					   database_id,
					   &r->delta_enum[i]);
		if (!NT_STATUS_IS_OK(status)) {
			return status;
		}
	}

	return NT_STATUS_OK;
}

/**
 * libnet_samsync_init_context
 */

NTSTATUS libnet_samsync_init_context(TALLOC_CTX *mem_ctx,
				     const struct dom_sid *domain_sid,
				     struct samsync_context **ctx_p)
{
	struct samsync_context *ctx;

	*ctx_p = NULL;

	ctx = TALLOC_ZERO_P(mem_ctx, struct samsync_context);
	NT_STATUS_HAVE_NO_MEMORY(ctx);

	if (domain_sid) {
		ctx->domain_sid = sid_dup_talloc(mem_ctx, domain_sid);
		NT_STATUS_HAVE_NO_MEMORY(ctx->domain_sid);

		ctx->domain_sid_str = sid_string_talloc(mem_ctx, ctx->domain_sid);
		NT_STATUS_HAVE_NO_MEMORY(ctx->domain_sid_str);
	}

	*ctx_p = ctx;

	return NT_STATUS_OK;
}

/**
 * samsync_database_str
 */

static const char *samsync_database_str(enum netr_SamDatabaseID database_id)
{

	switch (database_id) {
		case SAM_DATABASE_DOMAIN:
			return "DOMAIN";
		case SAM_DATABASE_BUILTIN:
			return "BUILTIN";
		case SAM_DATABASE_PRIVS:
			return "PRIVS";
		default:
			return "unknown";
	}
}

/**
 * samsync_debug_str
 */

static const char *samsync_debug_str(TALLOC_CTX *mem_ctx,
				     enum net_samsync_mode mode,
				     enum netr_SamDatabaseID database_id)
{
	const char *action = NULL;

	switch (mode) {
		case NET_SAMSYNC_MODE_DUMP:
			action = "Dumping (to stdout)";
			break;
		case NET_SAMSYNC_MODE_FETCH_PASSDB:
			action = "Fetching (to passdb)";
			break;
		case NET_SAMSYNC_MODE_FETCH_LDIF:
			action = "Fetching (to ldif)";
			break;
		case NET_SAMSYNC_MODE_FETCH_KEYTAB:
			action = "Fetching (to keytab)";
			break;
		default:
			action = "Unknown";
			break;
	}

	return talloc_asprintf(mem_ctx, "%s %s database",
				action, samsync_database_str(database_id));
}

/**
 * libnet_samsync
 */

static void libnet_init_netr_ChangeLogEntry(struct samsync_object *o,
					    struct netr_ChangeLogEntry *e)
{
	ZERO_STRUCTP(e);

	e->db_index		= o->database_id;
	e->delta_type		= o->object_type;

	switch (e->delta_type) {
		case NETR_DELTA_DOMAIN:
		case NETR_DELTA_DELETE_GROUP:
		case NETR_DELTA_RENAME_GROUP:
		case NETR_DELTA_DELETE_USER:
		case NETR_DELTA_RENAME_USER:
		case NETR_DELTA_DELETE_ALIAS:
		case NETR_DELTA_RENAME_ALIAS:
		case NETR_DELTA_DELETE_TRUST:
		case NETR_DELTA_DELETE_ACCOUNT:
		case NETR_DELTA_DELETE_SECRET:
		case NETR_DELTA_DELETE_GROUP2:
		case NETR_DELTA_DELETE_USER2:
		case NETR_DELTA_MODIFY_COUNT:
			break;
		case NETR_DELTA_USER:
		case NETR_DELTA_GROUP:
		case NETR_DELTA_GROUP_MEMBER:
		case NETR_DELTA_ALIAS:
		case NETR_DELTA_ALIAS_MEMBER:
			e->object_rid = o->object_identifier.rid;
			break;
		case NETR_DELTA_SECRET:
			e->object.object_name = o->object_identifier.name;
			e->flags = NETR_CHANGELOG_NAME_INCLUDED;
			break;
		case NETR_DELTA_TRUSTED_DOMAIN:
		case NETR_DELTA_ACCOUNT:
		case NETR_DELTA_POLICY:
			e->object.object_sid = o->object_identifier.sid;
			e->flags = NETR_CHANGELOG_SID_INCLUDED;
			break;
		default:
			break;
	}
}

/**
 * libnet_samsync_delta
 */

static NTSTATUS libnet_samsync_delta(TALLOC_CTX *mem_ctx,
				     enum netr_SamDatabaseID database_id,
				     uint64_t *sequence_num,
				     struct samsync_context *ctx,
				     struct netr_ChangeLogEntry *e)
{
	NTSTATUS result;
	NTSTATUS callback_status;
	const char *logon_server = ctx->cli->desthost;
	const char *computername = global_myname();
	struct netr_Authenticator credential;
	struct netr_Authenticator return_authenticator;
	uint16_t restart_state = 0;
	uint32_t sync_context = 0;
	DATA_BLOB session_key;

	ZERO_STRUCT(return_authenticator);

	do {
		struct netr_DELTA_ENUM_ARRAY *delta_enum_array = NULL;

		netlogon_creds_client_step(ctx->cli->dc, &credential);

		if (ctx->single_object_replication &&
		    !ctx->force_full_replication) {
			result = rpccli_netr_DatabaseRedo(ctx->cli, mem_ctx,
							  logon_server,
							  computername,
							  &credential,
							  &return_authenticator,
							  *e,
							  0,
							  &delta_enum_array);
		} else if (!ctx->force_full_replication &&
		           sequence_num && (*sequence_num > 0)) {
			result = rpccli_netr_DatabaseDeltas(ctx->cli, mem_ctx,
							    logon_server,
							    computername,
							    &credential,
							    &return_authenticator,
							    database_id,
							    sequence_num,
							    &delta_enum_array,
							    0xffff);
		} else {
			result = rpccli_netr_DatabaseSync2(ctx->cli, mem_ctx,
							   logon_server,
							   computername,
							   &credential,
							   &return_authenticator,
							   database_id,
							   restart_state,
							   &sync_context,
							   &delta_enum_array,
							   0xffff);
		}

		if (NT_STATUS_EQUAL(result, NT_STATUS_NOT_SUPPORTED)) {
			return result;
		}

		/* Check returned credentials. */
		if (!netlogon_creds_client_check(ctx->cli->dc,
						 &return_authenticator.cred)) {
			DEBUG(0,("credentials chain check failed\n"));
			return NT_STATUS_ACCESS_DENIED;
		}

		if (NT_STATUS_IS_ERR(result)) {
			break;
		}

		session_key = data_blob_const(ctx->cli->dc->sess_key, 16);

		samsync_fix_delta_array(mem_ctx,
					&session_key,
					database_id,
					delta_enum_array);

		/* Process results */
		callback_status = ctx->ops->process_objects(mem_ctx, database_id,
							    delta_enum_array,
							    sequence_num,
							    ctx);
		if (!NT_STATUS_IS_OK(callback_status)) {
			result = callback_status;
			goto out;
		}

		TALLOC_FREE(delta_enum_array);

	} while (NT_STATUS_EQUAL(result, STATUS_MORE_ENTRIES));

 out:

	return result;
}

/**
 * libnet_samsync
 */

NTSTATUS libnet_samsync(enum netr_SamDatabaseID database_id,
			struct samsync_context *ctx)
{
	NTSTATUS status = NT_STATUS_OK;
	NTSTATUS callback_status;
	TALLOC_CTX *mem_ctx;
	const char *debug_str;
	uint64_t sequence_num = 0;
	int i = 0;

	if (!(mem_ctx = talloc_new(ctx))) {
		return NT_STATUS_NO_MEMORY;
	}

	if (!ctx->ops) {
		return NT_STATUS_INVALID_PARAMETER;
	}

	if (ctx->ops->startup) {
		status = ctx->ops->startup(mem_ctx, ctx,
					   database_id, &sequence_num);
		if (!NT_STATUS_IS_OK(status)) {
			goto done;
		}
	}

	debug_str = samsync_debug_str(mem_ctx, ctx->mode, database_id);
	if (debug_str) {
		d_fprintf(stderr, "%s\n", debug_str);
	}

	if (!ctx->single_object_replication) {
		status = libnet_samsync_delta(mem_ctx, database_id,
					      &sequence_num, ctx, NULL);
		goto done;
	}

	for (i=0; i<ctx->num_objects; i++) {

		struct netr_ChangeLogEntry e;

		if (ctx->objects[i].database_id != database_id) {
			continue;
		}

		libnet_init_netr_ChangeLogEntry(&ctx->objects[i], &e);

		status = libnet_samsync_delta(mem_ctx, database_id,
					      &sequence_num, ctx, &e);
		if (!NT_STATUS_IS_OK(status)) {
			goto done;
		}
	}

 done:

	if (NT_STATUS_IS_OK(status) && ctx->ops->finish) {
		callback_status = ctx->ops->finish(mem_ctx, ctx,
						   database_id, sequence_num);
		if (!NT_STATUS_IS_OK(callback_status)) {
			status = callback_status;
		}
	}

	if (NT_STATUS_IS_ERR(status) && !ctx->error_message) {

		ctx->error_message = talloc_asprintf(ctx,
			"Failed to fetch %s database: %s",
			samsync_database_str(database_id),
			nt_errstr(status));

		if (NT_STATUS_EQUAL(status, NT_STATUS_NOT_SUPPORTED)) {

			ctx->error_message =
				talloc_asprintf_append(ctx->error_message,
					"\nPerhaps %s is a Windows native mode domain?",
					ctx->domain_name);
		}
	}

	talloc_destroy(mem_ctx);

	return status;
}

/**
 * pull_netr_AcctLockStr
 */

NTSTATUS pull_netr_AcctLockStr(TALLOC_CTX *mem_ctx,
			       struct lsa_BinaryString *r,
			       struct netr_AcctLockStr **str_p)
{
	struct netr_AcctLockStr *str;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;

	if (!mem_ctx || !r || !str_p) {
		return NT_STATUS_INVALID_PARAMETER;
	}

	*str_p = NULL;

	str = TALLOC_ZERO_P(mem_ctx, struct netr_AcctLockStr);
	if (!str) {
		return NT_STATUS_NO_MEMORY;
	}

	blob = data_blob_const(r->array, r->length);

	ndr_err = ndr_pull_struct_blob(&blob, mem_ctx, str,
		       (ndr_pull_flags_fn_t)ndr_pull_netr_AcctLockStr);

	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		return ndr_map_error2ntstatus(ndr_err);
	}

	*str_p = str;

	return NT_STATUS_OK;
}

