/* 
   Unix SMB/CIFS implementation.
   ACL get/set utility
   
   Copyright (C) Andrew Tridgell 2000
   Copyright (C) Tim Potter      2000
   Copyright (C) Jeremy Allison  2000
   Copyright (C) Jelmer Vernooij 2003
   
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

static int test_args;

#define CREATE_ACCESS_READ READ_CONTROL_ACCESS

/* numeric is set when the user wants numeric SIDs and ACEs rather
   than going via LSA calls to resolve them */
static int numeric;

enum acl_mode {SMB_ACL_SET, SMB_ACL_DELETE, SMB_ACL_MODIFY, SMB_ACL_ADD };
enum chown_mode {REQUEST_NONE, REQUEST_CHOWN, REQUEST_CHGRP};
enum exit_values {EXIT_OK, EXIT_FAILED, EXIT_PARSE_ERROR};

struct perm_value {
	const char *perm;
	uint32 mask;
};

/* These values discovered by inspection */

static const struct perm_value special_values[] = {
	{ "R", 0x00120089 },
	{ "W", 0x00120116 },
	{ "X", 0x001200a0 },
	{ "D", 0x00010000 },
	{ "P", 0x00040000 },
	{ "O", 0x00080000 },
	{ NULL, 0 },
};

static const struct perm_value standard_values[] = {
	{ "READ",   0x001200a9 },
	{ "CHANGE", 0x001301bf },
	{ "FULL",   0x001f01ff },
	{ NULL, 0 },
};

/* Open cli connection and policy handle */

static NTSTATUS cli_lsa_lookup_sid(struct cli_state *cli,
				   const DOM_SID *sid,
				   TALLOC_CTX *mem_ctx,
				   enum lsa_SidType *type,
				   char **domain, char **name)
{
	uint16 orig_cnum = cli->cnum;
	struct rpc_pipe_client *p;
	struct policy_handle handle;
	NTSTATUS status;
	TALLOC_CTX *frame = talloc_stackframe();
	enum lsa_SidType *types;
	char **domains;
	char **names;

	if (!cli_send_tconX(cli, "IPC$", "?????", "", 0)) {
		return cli_nt_error(cli);
	}

	status = cli_rpc_pipe_open_noauth(cli, &ndr_table_lsarpc.syntax_id,
					  &p);
	if (!NT_STATUS_IS_OK(status)) {
		goto fail;
	}

	status = rpccli_lsa_open_policy(p, talloc_tos(), True,
					GENERIC_EXECUTE_ACCESS, &handle);
	if (!NT_STATUS_IS_OK(status)) {
		goto fail;
	}

	status = rpccli_lsa_lookup_sids(p, talloc_tos(), &handle, 1, sid,
					&domains, &names, &types);
	if (!NT_STATUS_IS_OK(status)) {
		goto fail;
	}

	*type = types[0];
	*domain = talloc_move(mem_ctx, &domains[0]);
	*name = talloc_move(mem_ctx, &names[0]);

	status = NT_STATUS_OK;
 fail:
	TALLOC_FREE(p);
	cli_tdis(cli);
	cli->cnum = orig_cnum;
	TALLOC_FREE(frame);
	return status;
}

static NTSTATUS cli_lsa_lookup_name(struct cli_state *cli,
				    const char *name,
				    enum lsa_SidType *type,
				    DOM_SID *sid)
{
	uint16 orig_cnum = cli->cnum;
	struct rpc_pipe_client *p;
	struct policy_handle handle;
	NTSTATUS status;
	TALLOC_CTX *frame = talloc_stackframe();
	DOM_SID *sids;
	enum lsa_SidType *types;

	if (!cli_send_tconX(cli, "IPC$", "?????", "", 0)) {
		return cli_nt_error(cli);
	}

	status = cli_rpc_pipe_open_noauth(cli, &ndr_table_lsarpc.syntax_id,
					  &p);
	if (!NT_STATUS_IS_OK(status)) {
		goto fail;
	}

	status = rpccli_lsa_open_policy(p, talloc_tos(), True,
					GENERIC_EXECUTE_ACCESS, &handle);
	if (!NT_STATUS_IS_OK(status)) {
		goto fail;
	}

	status = rpccli_lsa_lookup_names(p, talloc_tos(), &handle, 1, &name,
					 NULL, 1, &sids, &types);
	if (!NT_STATUS_IS_OK(status)) {
		goto fail;
	}

	*type = types[0];
	*sid = sids[0];

	status = NT_STATUS_OK;
 fail:
	TALLOC_FREE(p);
	cli_tdis(cli);
	cli->cnum = orig_cnum;
	TALLOC_FREE(frame);
	return status;
}

/* convert a SID to a string, either numeric or username/group */
static void SidToString(struct cli_state *cli, fstring str, const DOM_SID *sid)
{
	char *domain = NULL;
	char *name = NULL;
	enum lsa_SidType type;
	NTSTATUS status;

	sid_to_fstring(str, sid);

	if (numeric) {
		return;
	}

	status = cli_lsa_lookup_sid(cli, sid, talloc_tos(), &type,
				    &domain, &name);

	if (!NT_STATUS_IS_OK(status)) {
		return;
	}

	if (*domain) {
		slprintf(str, sizeof(fstring) - 1, "%s%s%s",
			domain, lp_winbind_separator(), name);
	} else {
		fstrcpy(str, name);
	}
}

/* convert a string to a SID, either numeric or username/group */
static bool StringToSid(struct cli_state *cli, DOM_SID *sid, const char *str)
{
	enum lsa_SidType type;

	if (strncmp(str, "S-", 2) == 0) {
		return string_to_sid(sid, str);
	}

	return NT_STATUS_IS_OK(cli_lsa_lookup_name(cli, str, &type, sid));
}

static void print_ace_flags(FILE *f, uint8_t flags)
{
	char *str = talloc_strdup(NULL, "");

	if (!str) {
		goto out;
	}

	if (flags & SEC_ACE_FLAG_OBJECT_INHERIT) {
		str = talloc_asprintf(str, "%s%s",
				str, "OI|");
		if (!str) {
			goto out;
		}
	}
	if (flags & SEC_ACE_FLAG_CONTAINER_INHERIT) {
		str = talloc_asprintf(str, "%s%s",
				str, "CI|");
		if (!str) {
			goto out;
		}
	}
	if (flags & SEC_ACE_FLAG_NO_PROPAGATE_INHERIT) {
		str = talloc_asprintf(str, "%s%s",
				str, "NP|");
		if (!str) {
			goto out;
		}
	}
	if (flags & SEC_ACE_FLAG_INHERIT_ONLY) {
		str = talloc_asprintf(str, "%s%s",
				str, "IO|");
		if (!str) {
			goto out;
		}
	}
	if (flags & SEC_ACE_FLAG_INHERITED_ACE) {
		str = talloc_asprintf(str, "%s%s",
				str, "I|");
		if (!str) {
			goto out;
		}
	}
	/* Ignore define SEC_ACE_FLAG_SUCCESSFUL_ACCESS ( 0x40 )
	   and SEC_ACE_FLAG_FAILED_ACCESS ( 0x80 ) as they're
	   audit ace flags. */

	if (str[strlen(str)-1] == '|') {
		str[strlen(str)-1] = '\0';
		fprintf(f, "/%s/", str);
	} else {
		fprintf(f, "/0x%x/", flags);
	}
	TALLOC_FREE(str);
	return;

  out:
	fprintf(f, "/0x%x/", flags);
}

/* print an ACE on a FILE, using either numeric or ascii representation */
static void print_ace(struct cli_state *cli, FILE *f, SEC_ACE *ace)
{
	const struct perm_value *v;
	fstring sidstr;
	int do_print = 0;
	uint32 got_mask;

	SidToString(cli, sidstr, &ace->trustee);

	fprintf(f, "%s:", sidstr);

	if (numeric) {
		fprintf(f, "%d/0x%x/0x%08x",
			ace->type, ace->flags, ace->access_mask);
		return;
	}

	/* Ace type */

	if (ace->type == SEC_ACE_TYPE_ACCESS_ALLOWED) {
		fprintf(f, "ALLOWED");
	} else if (ace->type == SEC_ACE_TYPE_ACCESS_DENIED) {
		fprintf(f, "DENIED");
	} else {
		fprintf(f, "%d", ace->type);
	}

	print_ace_flags(f, ace->flags);

	/* Standard permissions */

	for (v = standard_values; v->perm; v++) {
		if (ace->access_mask == v->mask) {
			fprintf(f, "%s", v->perm);
			return;
		}
	}

	/* Special permissions.  Print out a hex value if we have
	   leftover bits in the mask. */

	got_mask = ace->access_mask;

 again:
	for (v = special_values; v->perm; v++) {
		if ((ace->access_mask & v->mask) == v->mask) {
			if (do_print) {
				fprintf(f, "%s", v->perm);
			}
			got_mask &= ~v->mask;
		}
	}

	if (!do_print) {
		if (got_mask != 0) {
			fprintf(f, "0x%08x", ace->access_mask);
		} else {
			do_print = 1;
			goto again;
		}
	}
}

static bool parse_ace_flags(const char *str, unsigned int *pflags)
{
	const char *p = str;
	*pflags = 0;

	while (*p) {
		if (strnequal(p, "OI", 2)) {
			*pflags |= SEC_ACE_FLAG_OBJECT_INHERIT;
			p += 2;
		} else if (strnequal(p, "CI", 2)) {
			*pflags |= SEC_ACE_FLAG_CONTAINER_INHERIT;
			p += 2;
		} else if (strnequal(p, "NP", 2)) {
			*pflags |= SEC_ACE_FLAG_NO_PROPAGATE_INHERIT;
			p += 2;
		} else if (strnequal(p, "IO", 2)) {
			*pflags |= SEC_ACE_FLAG_INHERIT_ONLY;
			p += 2;
		} else if (*p == 'I') {
			*pflags |= SEC_ACE_FLAG_INHERITED_ACE;
			p += 1;
		} else if (*p) {
			return false;
		}

		if (*p != '|' && *p != '\0') {
			return false;
		}
	}
	return true;
}

/* parse an ACE in the same format as print_ace() */
static bool parse_ace(struct cli_state *cli, SEC_ACE *ace,
		      const char *orig_str)
{
	char *p;
	const char *cp;
	char *tok;
	unsigned int atype = 0;
	unsigned int aflags = 0;
	unsigned int amask = 0;
	DOM_SID sid;
	uint32_t mask;
	const struct perm_value *v;
	char *str = SMB_STRDUP(orig_str);
	TALLOC_CTX *frame = talloc_stackframe();

	if (!str) {
		TALLOC_FREE(frame);
		return False;
	}

	ZERO_STRUCTP(ace);
	p = strchr_m(str,':');
	if (!p) {
		printf("ACE '%s': missing ':'.\n", orig_str);
		SAFE_FREE(str);
		TALLOC_FREE(frame);
		return False;
	}
	*p = '\0';
	p++;
	/* Try to parse numeric form */

	if (sscanf(p, "%i/%i/%i", &atype, &aflags, &amask) == 3 &&
	    StringToSid(cli, &sid, str)) {
		goto done;
	}

	/* Try to parse text form */

	if (!StringToSid(cli, &sid, str)) {
		printf("ACE '%s': failed to convert '%s' to SID\n",
			orig_str, str);
		SAFE_FREE(str);
		TALLOC_FREE(frame);
		return False;
	}

	cp = p;
	if (!next_token_talloc(frame, &cp, &tok, "/")) {
		printf("ACE '%s': failed to find '/' character.\n",
			orig_str);
		SAFE_FREE(str);
		TALLOC_FREE(frame);
		return False;
	}

	if (strncmp(tok, "ALLOWED", strlen("ALLOWED")) == 0) {
		atype = SEC_ACE_TYPE_ACCESS_ALLOWED;
	} else if (strncmp(tok, "DENIED", strlen("DENIED")) == 0) {
		atype = SEC_ACE_TYPE_ACCESS_DENIED;
	} else {
		printf("ACE '%s': missing 'ALLOWED' or 'DENIED' entry at '%s'\n",
			orig_str, tok);
		SAFE_FREE(str);
		TALLOC_FREE(frame);
		return False;
	}

	/* Only numeric form accepted for flags at present */

	if (!next_token_talloc(frame, &cp, &tok, "/")) {
		printf("ACE '%s': bad flags entry at '%s'\n",
			orig_str, tok);
		SAFE_FREE(str);
		TALLOC_FREE(frame);
		return False;
	}

	if (tok[0] < '0' || tok[0] > '9') {
		if (!parse_ace_flags(tok, &aflags)) {
			printf("ACE '%s': bad named flags entry at '%s'\n",
				orig_str, tok);
			SAFE_FREE(str);
			TALLOC_FREE(frame);
			return False;
		}
	} else if (strnequal(tok, "0x", 2)) {
		if (!sscanf(tok, "%x", &aflags)) {
			printf("ACE '%s': bad hex flags entry at '%s'\n",
				orig_str, tok);
			SAFE_FREE(str);
			TALLOC_FREE(frame);
			return False;
		}
	} else {
		if (!sscanf(tok, "%i", &aflags)) {
			printf("ACE '%s': bad integer flags entry at '%s'\n",
				orig_str, tok);
			SAFE_FREE(str);
			TALLOC_FREE(frame);
			return False;
		}
	}

	if (!next_token_talloc(frame, &cp, &tok, "/")) {
		printf("ACE '%s': missing / at '%s'\n",
			orig_str, tok);
		SAFE_FREE(str);
		TALLOC_FREE(frame);
		return False;
	}

	if (strncmp(tok, "0x", 2) == 0) {
		if (sscanf(tok, "%i", &amask) != 1) {
			printf("ACE '%s': bad hex number at '%s'\n",
				orig_str, tok);
			SAFE_FREE(str);
			TALLOC_FREE(frame);
			return False;
		}
		goto done;
	}

	for (v = standard_values; v->perm; v++) {
		if (strcmp(tok, v->perm) == 0) {
			amask = v->mask;
			goto done;
		}
	}

	p = tok;

	while(*p) {
		bool found = False;

		for (v = special_values; v->perm; v++) {
			if (v->perm[0] == *p) {
				amask |= v->mask;
				found = True;
			}
		}

		if (!found) {
			printf("ACE '%s': bad permission value at '%s'\n",
				orig_str, p);
			SAFE_FREE(str);
			TALLOC_FREE(frame);
		 	return False;
		}
		p++;
	}

	if (*p) {
		TALLOC_FREE(frame);
		SAFE_FREE(str);
		return False;
	}

 done:
	mask = amask;
	init_sec_ace(ace, &sid, atype, mask, aflags);
	TALLOC_FREE(frame);
	SAFE_FREE(str);
	return True;
}

/* add an ACE to a list of ACEs in a SEC_ACL */
static bool add_ace(SEC_ACL **the_acl, SEC_ACE *ace)
{
	SEC_ACL *new_ace;
	SEC_ACE *aces;
	if (! *the_acl) {
		return (((*the_acl) = make_sec_acl(talloc_tos(), 3, 1, ace))
			!= NULL);
	}

	if (!(aces = SMB_CALLOC_ARRAY(SEC_ACE, 1+(*the_acl)->num_aces))) {
		return False;
	}
	memcpy(aces, (*the_acl)->aces, (*the_acl)->num_aces * sizeof(SEC_ACE));
	memcpy(aces+(*the_acl)->num_aces, ace, sizeof(SEC_ACE));
	new_ace = make_sec_acl(talloc_tos(),(*the_acl)->revision,1+(*the_acl)->num_aces, aces);
	SAFE_FREE(aces);
	(*the_acl) = new_ace;
	return True;
}

/* parse a ascii version of a security descriptor */
static SEC_DESC *sec_desc_parse(TALLOC_CTX *ctx, struct cli_state *cli, char *str)
{
	const char *p = str;
	char *tok;
	SEC_DESC *ret = NULL;
	size_t sd_size;
	DOM_SID *grp_sid=NULL, *owner_sid=NULL;
	SEC_ACL *dacl=NULL;
	int revision=1;

	while (next_token_talloc(ctx, &p, &tok, "\t,\r\n")) {
		if (strncmp(tok,"REVISION:", 9) == 0) {
			revision = strtol(tok+9, NULL, 16);
			continue;
		}

		if (strncmp(tok,"OWNER:", 6) == 0) {
			if (owner_sid) {
				printf("Only specify owner once\n");
				goto done;
			}
			owner_sid = SMB_CALLOC_ARRAY(DOM_SID, 1);
			if (!owner_sid ||
			    !StringToSid(cli, owner_sid, tok+6)) {
				printf("Failed to parse owner sid\n");
				goto done;
			}
			continue;
		}

		if (strncmp(tok,"GROUP:", 6) == 0) {
			if (grp_sid) {
				printf("Only specify group once\n");
				goto done;
			}
			grp_sid = SMB_CALLOC_ARRAY(DOM_SID, 1);
			if (!grp_sid ||
			    !StringToSid(cli, grp_sid, tok+6)) {
				printf("Failed to parse group sid\n");
				goto done;
			}
			continue;
		}

		if (strncmp(tok,"ACL:", 4) == 0) {
			SEC_ACE ace;
			if (!parse_ace(cli, &ace, tok+4)) {
				goto done;
			}
			if(!add_ace(&dacl, &ace)) {
				printf("Failed to add ACL %s\n", tok);
				goto done;
			}
			continue;
		}

		printf("Failed to parse token '%s' in security descriptor,\n", tok);
		goto done;
	}

	ret = make_sec_desc(ctx,revision, SEC_DESC_SELF_RELATIVE, owner_sid, grp_sid,
			    NULL, dacl, &sd_size);

  done:
	SAFE_FREE(grp_sid);
	SAFE_FREE(owner_sid);

	return ret;
}


/* print a ascii version of a security descriptor on a FILE handle */
static void sec_desc_print(struct cli_state *cli, FILE *f, SEC_DESC *sd)
{
	fstring sidstr;
	uint32 i;

	fprintf(f, "REVISION:%d\n", sd->revision);
	fprintf(f, "CONTROL:0x%x\n", sd->type);

	/* Print owner and group sid */

	if (sd->owner_sid) {
		SidToString(cli, sidstr, sd->owner_sid);
	} else {
		fstrcpy(sidstr, "");
	}

	fprintf(f, "OWNER:%s\n", sidstr);

	if (sd->group_sid) {
		SidToString(cli, sidstr, sd->group_sid);
	} else {
		fstrcpy(sidstr, "");
	}

	fprintf(f, "GROUP:%s\n", sidstr);

	/* Print aces */
	for (i = 0; sd->dacl && i < sd->dacl->num_aces; i++) {
		SEC_ACE *ace = &sd->dacl->aces[i];
		fprintf(f, "ACL:");
		print_ace(cli, f, ace);
		fprintf(f, "\n");
	}

}

/***************************************************** 
dump the acls for a file
*******************************************************/
static int cacl_dump(struct cli_state *cli, char *filename)
{
	int result = EXIT_FAILED;
	int fnum = -1;
	SEC_DESC *sd;

	if (test_args) 
		return EXIT_OK;

	fnum = cli_nt_create(cli, filename, CREATE_ACCESS_READ);

	if (fnum == -1) {
		printf("Failed to open %s: %s\n", filename, cli_errstr(cli));
		goto done;
	}

	sd = cli_query_secdesc(cli, fnum, talloc_tos());

	if (!sd) {
		printf("ERROR: secdesc query failed: %s\n", cli_errstr(cli));
		goto done;
	}

	sec_desc_print(cli, stdout, sd);

	result = EXIT_OK;

done:
	if (fnum != -1)
		cli_close(cli, fnum);

	return result;
}

/***************************************************** 
Change the ownership or group ownership of a file. Just
because the NT docs say this can't be done :-). JRA.
*******************************************************/

static int owner_set(struct cli_state *cli, enum chown_mode change_mode, 
			const char *filename, const char *new_username)
{
	int fnum;
	DOM_SID sid;
	SEC_DESC *sd, *old;
	size_t sd_size;

	fnum = cli_nt_create(cli, filename, CREATE_ACCESS_READ);

	if (fnum == -1) {
		printf("Failed to open %s: %s\n", filename, cli_errstr(cli));
		return EXIT_FAILED;
	}

	if (!StringToSid(cli, &sid, new_username))
		return EXIT_PARSE_ERROR;

	old = cli_query_secdesc(cli, fnum, talloc_tos());

	cli_close(cli, fnum);

	if (!old) {
		printf("owner_set: Failed to query old descriptor\n");
		return EXIT_FAILED;
	}

	sd = make_sec_desc(talloc_tos(),old->revision, old->type,
				(change_mode == REQUEST_CHOWN) ? &sid : NULL,
				(change_mode == REQUEST_CHGRP) ? &sid : NULL,
			   NULL, NULL, &sd_size);

	fnum = cli_nt_create(cli, filename, WRITE_OWNER_ACCESS);

	if (fnum == -1) {
		printf("Failed to open %s: %s\n", filename, cli_errstr(cli));
		return EXIT_FAILED;
	}

	if (!cli_set_secdesc(cli, fnum, sd)) {
		printf("ERROR: secdesc set failed: %s\n", cli_errstr(cli));
		cli_close(cli, fnum);
		return EXIT_FAILED;
	}

	cli_close(cli, fnum);

	return EXIT_OK;
}


/* The MSDN is contradictory over the ordering of ACE entries in an
   ACL.  However NT4 gives a "The information may have been modified
   by a computer running Windows NT 5.0" if denied ACEs do not appear
   before allowed ACEs. At
   http://technet.microsoft.com/en-us/library/cc781716.aspx the
   canonical order is specified as "Explicit Deny, Explicit Allow,
   Inherited ACEs unchanged" */

static int ace_compare(SEC_ACE *ace1, SEC_ACE *ace2)
{
	if (sec_ace_equal(ace1, ace2))
		return 0;

	if ((ace1->flags & SEC_ACE_FLAG_INHERITED_ACE) &&
			!(ace2->flags & SEC_ACE_FLAG_INHERITED_ACE))
		return 1;
	if (!(ace1->flags & SEC_ACE_FLAG_INHERITED_ACE) &&
			(ace2->flags & SEC_ACE_FLAG_INHERITED_ACE))
		return -1;
	if ((ace1->flags & SEC_ACE_FLAG_INHERITED_ACE) &&
			(ace2->flags & SEC_ACE_FLAG_INHERITED_ACE))
		return ace1 - ace2;

	if (ace1->type != ace2->type)
		return ace2->type - ace1->type;

	if (sid_compare(&ace1->trustee, &ace2->trustee))
		return sid_compare(&ace1->trustee, &ace2->trustee);

	if (ace1->flags != ace2->flags)
		return ace1->flags - ace2->flags;

	if (ace1->access_mask != ace2->access_mask)
		return ace1->access_mask - ace2->access_mask;

	if (ace1->size != ace2->size)
		return ace1->size - ace2->size;

	return memcmp(ace1, ace2, sizeof(SEC_ACE));
}

static void sort_acl(SEC_ACL *the_acl)
{
	uint32 i;
	if (!the_acl) return;

	qsort(the_acl->aces, the_acl->num_aces, sizeof(the_acl->aces[0]), QSORT_CAST ace_compare);

	for (i=1;i<the_acl->num_aces;) {
		if (sec_ace_equal(&the_acl->aces[i-1], &the_acl->aces[i])) {
			int j;
			for (j=i; j<the_acl->num_aces-1; j++) {
				the_acl->aces[j] = the_acl->aces[j+1];
			}
			the_acl->num_aces--;
		} else {
			i++;
		}
	}
}

/***************************************************** 
set the ACLs on a file given an ascii description
*******************************************************/

static int cacl_set(struct cli_state *cli, char *filename, 
		    char *the_acl, enum acl_mode mode)
{
	int fnum;
	SEC_DESC *sd, *old;
	uint32 i, j;
	size_t sd_size;
	int result = EXIT_OK;

	sd = sec_desc_parse(talloc_tos(), cli, the_acl);

	if (!sd) return EXIT_PARSE_ERROR;
	if (test_args) return EXIT_OK;

	/* The desired access below is the only one I could find that works
	   with NT4, W2KP and Samba */

	fnum = cli_nt_create(cli, filename, CREATE_ACCESS_READ);

	if (fnum == -1) {
		printf("cacl_set failed to open %s: %s\n", filename, cli_errstr(cli));
		return EXIT_FAILED;
	}

	old = cli_query_secdesc(cli, fnum, talloc_tos());

	if (!old) {
		printf("calc_set: Failed to query old descriptor\n");
		return EXIT_FAILED;
	}

	cli_close(cli, fnum);

	/* the logic here is rather more complex than I would like */
	switch (mode) {
	case SMB_ACL_DELETE:
		for (i=0;sd->dacl && i<sd->dacl->num_aces;i++) {
			bool found = False;

			for (j=0;old->dacl && j<old->dacl->num_aces;j++) {
				if (sec_ace_equal(&sd->dacl->aces[i],
						  &old->dacl->aces[j])) {
					uint32 k;
					for (k=j; k<old->dacl->num_aces-1;k++) {
						old->dacl->aces[k] = old->dacl->aces[k+1];
					}
					old->dacl->num_aces--;
					found = True;
					break;
				}
			}

			if (!found) {
				printf("ACL for ACE:");
				print_ace(cli, stdout, &sd->dacl->aces[i]);
				printf(" not found\n");
			}
		}
		break;

	case SMB_ACL_MODIFY:
		for (i=0;sd->dacl && i<sd->dacl->num_aces;i++) {
			bool found = False;

			for (j=0;old->dacl && j<old->dacl->num_aces;j++) {
				if (sid_equal(&sd->dacl->aces[i].trustee,
					      &old->dacl->aces[j].trustee)) {
					old->dacl->aces[j] = sd->dacl->aces[i];
					found = True;
				}
			}

			if (!found) {
				fstring str;

				SidToString(cli, str,
					    &sd->dacl->aces[i].trustee);
				printf("ACL for SID %s not found\n", str);
			}
		}

		if (sd->owner_sid) {
			old->owner_sid = sd->owner_sid;
		}

		if (sd->group_sid) {
			old->group_sid = sd->group_sid;
		}

		break;

	case SMB_ACL_ADD:
		for (i=0;sd->dacl && i<sd->dacl->num_aces;i++) {
			add_ace(&old->dacl, &sd->dacl->aces[i]);
		}
		break;

	case SMB_ACL_SET:
 		old = sd;
		break;
	}

	/* Denied ACE entries must come before allowed ones */
	sort_acl(old->dacl);

	/* Create new security descriptor and set it */

	/* We used to just have "WRITE_DAC_ACCESS" without WRITE_OWNER.
	   But if we're sending an owner, even if it's the same as the one
	   that already exists then W2K3 insists we open with WRITE_OWNER access.
	   I need to check that setting a SD with no owner set works against WNT
	   and W2K. JRA.
	*/

	sd = make_sec_desc(talloc_tos(),old->revision, old->type,
			   old->owner_sid, old->group_sid,
			   NULL, old->dacl, &sd_size);

	fnum = cli_nt_create(cli, filename, WRITE_DAC_ACCESS|WRITE_OWNER_ACCESS);

	if (fnum == -1) {
		printf("cacl_set failed to open %s: %s\n", filename, cli_errstr(cli));
		return EXIT_FAILED;
	}

	if (!cli_set_secdesc(cli, fnum, sd)) {
		printf("ERROR: secdesc set failed: %s\n", cli_errstr(cli));
		result = EXIT_FAILED;
	}

	/* Clean up */

	cli_close(cli, fnum);

	return result;
}


/*****************************************************
 Return a connection to a server.
*******************************************************/
static struct cli_state *connect_one(const char *server, const char *share)
{
	struct cli_state *c = NULL;
	struct sockaddr_storage ss;
	NTSTATUS nt_status;
	uint32_t flags = 0;

	zero_sockaddr(&ss);

	if (get_cmdline_auth_info_use_kerberos()) {
		flags |= CLI_FULL_CONNECTION_USE_KERBEROS |
			 CLI_FULL_CONNECTION_FALLBACK_AFTER_KERBEROS;
	}

	if (get_cmdline_auth_info_use_machine_account() &&
	    !set_cmdline_auth_info_machine_account_creds()) {
		return NULL;
	}

	if (!get_cmdline_auth_info_got_pass()) {
		char *pass = getpass("Password: ");
		if (pass) {
			set_cmdline_auth_info_password(pass);
		}
	}

	nt_status = cli_full_connection(&c, global_myname(), server, 
				&ss, 0,
				share, "?????",
				get_cmdline_auth_info_username(),
				lp_workgroup(),
				get_cmdline_auth_info_password(),
				flags,
				get_cmdline_auth_info_signing_state(),
				NULL);
	if (!NT_STATUS_IS_OK(nt_status)) {
		DEBUG(0,("cli_full_connection failed! (%s)\n", nt_errstr(nt_status)));
		return NULL;
	}

	if (get_cmdline_auth_info_smb_encrypt()) {
		nt_status = cli_cm_force_encryption(c,
					get_cmdline_auth_info_username(),
					get_cmdline_auth_info_password(),
					lp_workgroup(),
					share);
                if (!NT_STATUS_IS_OK(nt_status)) {
			cli_shutdown(c);
			c = NULL;
                }
	}

	return c;
}

/****************************************************************************
  main program
****************************************************************************/
 int main(int argc, const char *argv[])
{
	char *share;
	int opt;
	enum acl_mode mode = SMB_ACL_SET;
	static char *the_acl = NULL;
	enum chown_mode change_mode = REQUEST_NONE;
	int result;
	char *path;
	char *filename = NULL;
	poptContext pc;
	struct poptOption long_options[] = {
		POPT_AUTOHELP
		{ "delete", 'D', POPT_ARG_STRING, NULL, 'D', "Delete an acl", "ACL" },
		{ "modify", 'M', POPT_ARG_STRING, NULL, 'M', "Modify an acl", "ACL" },
		{ "add", 'a', POPT_ARG_STRING, NULL, 'a', "Add an acl", "ACL" },
		{ "set", 'S', POPT_ARG_STRING, NULL, 'S', "Set acls", "ACLS" },
		{ "chown", 'C', POPT_ARG_STRING, NULL, 'C', "Change ownership of a file", "USERNAME" },
		{ "chgrp", 'G', POPT_ARG_STRING, NULL, 'G', "Change group ownership of a file", "GROUPNAME" },
		{ "numeric", 0, POPT_ARG_NONE, &numeric, 1, "Don't resolve sids or masks to names" },
		{ "test-args", 't', POPT_ARG_NONE, &test_args, 1, "Test arguments"},
		POPT_COMMON_SAMBA
		POPT_COMMON_CONNECTION
		POPT_COMMON_CREDENTIALS
		POPT_TABLEEND
	};

	struct cli_state *cli;
	TALLOC_CTX *frame = talloc_stackframe();
	const char *owner_username = "";
	char *server;

	load_case_tables();


	/* set default debug level to 1 regardless of what smb.conf sets */
	setup_logging( "smbcacls", True );
	DEBUGLEVEL_CLASS[DBGC_ALL] = 1;
	dbf = x_stderr;
	x_setbuf( x_stderr, NULL );

	setlinebuf(stdout);

	lp_load(get_dyn_CONFIGFILE(),True,False,False,True);
	load_interfaces();

	pc = poptGetContext("smbcacls", argc, argv, long_options, 0);

	poptSetOtherOptionHelp(pc, "//server1/share1 filename\nACLs look like: "
		"'ACL:user:[ALLOWED|DENIED]/flags/permissions'");

	while ((opt = poptGetNextOpt(pc)) != -1) {
		switch (opt) {
		case 'S':
			the_acl = smb_xstrdup(poptGetOptArg(pc));
			mode = SMB_ACL_SET;
			break;

		case 'D':
			the_acl = smb_xstrdup(poptGetOptArg(pc));
			mode = SMB_ACL_DELETE;
			break;

		case 'M':
			the_acl = smb_xstrdup(poptGetOptArg(pc));
			mode = SMB_ACL_MODIFY;
			break;

		case 'a':
			the_acl = smb_xstrdup(poptGetOptArg(pc));
			mode = SMB_ACL_ADD;
			break;

		case 'C':
			owner_username = poptGetOptArg(pc);
			change_mode = REQUEST_CHOWN;
			break;

		case 'G':
			owner_username = poptGetOptArg(pc);
			change_mode = REQUEST_CHGRP;
			break;
		}
	}

	/* Make connection to server */
	if(!poptPeekArg(pc)) {
		poptPrintUsage(pc, stderr, 0);
		return -1;
	}

	path = talloc_strdup(frame, poptGetArg(pc));
	if (!path) {
		return -1;
	}

	if(!poptPeekArg(pc)) {
		poptPrintUsage(pc, stderr, 0);
		return -1;
	}

	filename = talloc_strdup(frame, poptGetArg(pc));
	if (!filename) {
		return -1;
	}

	string_replace(path,'/','\\');

	server = talloc_strdup(frame, path+2);
	if (!server) {
		return -1;
	}
	share = strchr_m(server,'\\');
	if (!share) {
		printf("Invalid argument: %s\n", share);
		return -1;
	}

	*share = 0;
	share++;

	if (!test_args) {
		cli = connect_one(server, share);
		if (!cli) {
			exit(EXIT_FAILED);
		}
	} else {
		exit(0);
	}

	string_replace(filename, '/', '\\');
	if (filename[0] != '\\') {
		filename = talloc_asprintf(frame,
				"\\%s",
				filename);
		if (!filename) {
			return -1;
		}
	}

	/* Perform requested action */

	if (change_mode != REQUEST_NONE) {
		result = owner_set(cli, change_mode, filename, owner_username);
	} else if (the_acl) {
		result = cacl_set(cli, filename, the_acl, mode);
	} else {
		result = cacl_dump(cli, filename);
	}

	TALLOC_FREE(frame);

	return result;
}
