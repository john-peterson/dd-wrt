/*
 *  Unix SMB/CIFS implementation.
 *  Local SAM access routines
 *  Copyright (C) Volker Lendecke 2006
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#include "includes.h"
#include "utils/net.h"

/*
 * Set a user's data
 */

static int net_sam_userset(struct net_context *c, int argc, const char **argv,
			   const char *field,
			   bool (*fn)(struct samu *, const char *,
				      enum pdb_value_state))
{
	struct samu *sam_acct = NULL;
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;
	NTSTATUS status;

	if (argc != 2 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam set %s <user> <value>\n",
			  field);
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	if (type != SID_NAME_USER) {
		d_fprintf(stderr, "%s is a %s, not a user\n", argv[0],
			  sid_type_lookup(type));
		return -1;
	}

	if ( !(sam_acct = samu_new( NULL )) ) {
		d_fprintf(stderr, "Internal error\n");
		return -1;
	}

	if (!pdb_getsampwsid(sam_acct, &sid)) {
		d_fprintf(stderr, "Loading user %s failed\n", argv[0]);
		return -1;
	}

	if (!fn(sam_acct, argv[1], PDB_CHANGED)) {
		d_fprintf(stderr, "Internal error\n");
		return -1;
	}

	status = pdb_update_sam_account(sam_acct);
	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Updating sam account %s failed with %s\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	TALLOC_FREE(sam_acct);

	d_printf("Updated %s for %s\\%s to %s\n", field, dom, name, argv[1]);
	return 0;
}

static int net_sam_set_fullname(struct net_context *c, int argc,
				const char **argv)
{
	return net_sam_userset(c, argc, argv, "fullname",
			       pdb_set_fullname);
}

static int net_sam_set_logonscript(struct net_context *c, int argc,
				   const char **argv)
{
	return net_sam_userset(c, argc, argv, "logonscript",
			       pdb_set_logon_script);
}

static int net_sam_set_profilepath(struct net_context *c, int argc,
				   const char **argv)
{
	return net_sam_userset(c, argc, argv, "profilepath",
			       pdb_set_profile_path);
}

static int net_sam_set_homedrive(struct net_context *c, int argc,
				 const char **argv)
{
	return net_sam_userset(c, argc, argv, "homedrive",
			       pdb_set_dir_drive);
}

static int net_sam_set_homedir(struct net_context *c, int argc,
			       const char **argv)
{
	return net_sam_userset(c, argc, argv, "homedir",
			       pdb_set_homedir);
}

static int net_sam_set_workstations(struct net_context *c, int argc,
				    const char **argv)
{
	return net_sam_userset(c, argc, argv, "workstations",
			       pdb_set_workstations);
}

/*
 * Set account flags
 */

static int net_sam_set_userflag(struct net_context *c, int argc,
				const char **argv, const char *field,
				uint16 flag)
{
	struct samu *sam_acct = NULL;
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;
	NTSTATUS status;
	uint16 acct_flags;

	if ((argc != 2) || c->display_usage ||
	    (!strequal(argv[1], "yes") &&
	     !strequal(argv[1], "no"))) {
		d_fprintf(stderr, "usage: net sam set %s <user> [yes|no]\n",
			  field);
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	if (type != SID_NAME_USER) {
		d_fprintf(stderr, "%s is a %s, not a user\n", argv[0],
			  sid_type_lookup(type));
		return -1;
	}

	if ( !(sam_acct = samu_new( NULL )) ) {
		d_fprintf(stderr, "Internal error\n");
		return -1;
	}

	if (!pdb_getsampwsid(sam_acct, &sid)) {
		d_fprintf(stderr, "Loading user %s failed\n", argv[0]);
		return -1;
	}

	acct_flags = pdb_get_acct_ctrl(sam_acct);

	if (strequal(argv[1], "yes")) {
		acct_flags |= flag;
	} else {
		acct_flags &= ~flag;
	}

	pdb_set_acct_ctrl(sam_acct, acct_flags, PDB_CHANGED);

	status = pdb_update_sam_account(sam_acct);
	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Updating sam account %s failed with %s\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	TALLOC_FREE(sam_acct);

	d_fprintf(stderr, "Updated flag %s for %s\\%s to %s\n", field, dom,
		  name, argv[1]);
	return 0;
}

static int net_sam_set_disabled(struct net_context *c, int argc,
				const char **argv)
{
	return net_sam_set_userflag(c, argc, argv, "disabled", ACB_DISABLED);
}

static int net_sam_set_pwnotreq(struct net_context *c, int argc,
				const char **argv)
{
	return net_sam_set_userflag(c, argc, argv, "pwnotreq", ACB_PWNOTREQ);
}

static int net_sam_set_autolock(struct net_context *c, int argc,
				const char **argv)
{
	return net_sam_set_userflag(c, argc, argv, "autolock", ACB_AUTOLOCK);
}

static int net_sam_set_pwnoexp(struct net_context *c, int argc,
			       const char **argv)
{
	return net_sam_set_userflag(c, argc, argv, "pwnoexp", ACB_PWNOEXP);
}

/*
 * Set pass last change time, based on force pass change now
 */

static int net_sam_set_pwdmustchangenow(struct net_context *c, int argc,
					const char **argv)
{
	struct samu *sam_acct = NULL;
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;
	NTSTATUS status;

	if ((argc != 2) || c->display_usage ||
	    (!strequal(argv[1], "yes") &&
	     !strequal(argv[1], "no"))) {
		d_fprintf(stderr, "usage: net sam set pwdmustchangenow <user> [yes|no]\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	if (type != SID_NAME_USER) {
		d_fprintf(stderr, "%s is a %s, not a user\n", argv[0],
			  sid_type_lookup(type));
		return -1;
	}

	if ( !(sam_acct = samu_new( NULL )) ) {
		d_fprintf(stderr, "Internal error\n");
		return -1;
	}

	if (!pdb_getsampwsid(sam_acct, &sid)) {
		d_fprintf(stderr, "Loading user %s failed\n", argv[0]);
		return -1;
	}

	if (strequal(argv[1], "yes")) {
		pdb_set_pass_last_set_time(sam_acct, 0, PDB_CHANGED);
	} else {
		pdb_set_pass_last_set_time(sam_acct, time(NULL), PDB_CHANGED);
	}

	status = pdb_update_sam_account(sam_acct);
	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Updating sam account %s failed with %s\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	TALLOC_FREE(sam_acct);

	d_fprintf(stderr, "Updated 'user must change password at next logon' for %s\\%s to %s\n", dom,
		  name, argv[1]);
	return 0;
}


/*
 * Set a user's or a group's comment
 */

static int net_sam_set_comment(struct net_context *c, int argc,
			       const char **argv)
{
	GROUP_MAP map;
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;
	NTSTATUS status;

	if (argc != 2 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam set comment <name> "
			  "<comment>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	if (type == SID_NAME_USER) {
		return net_sam_userset(c, argc, argv, "comment",
				       pdb_set_acct_desc);
	}

	if ((type != SID_NAME_DOM_GRP) && (type != SID_NAME_ALIAS) &&
	    (type != SID_NAME_WKN_GRP)) {
		d_fprintf(stderr, "%s is a %s, not a group\n", argv[0],
			  sid_type_lookup(type));
		return -1;
	}

	if (!pdb_getgrsid(&map, sid)) {
		d_fprintf(stderr, "Could not load group %s\n", argv[0]);
		return -1;
	}

	fstrcpy(map.comment, argv[1]);

	status = pdb_update_group_mapping_entry(&map);

	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Updating group mapping entry failed with "
			  "%s\n", nt_errstr(status));
		return -1;
	}

	d_printf("Updated comment of group %s\\%s to %s\n", dom, name,
		 argv[1]);

	return 0;
}

static int net_sam_set(struct net_context *c, int argc, const char **argv)
{
	struct functable func[] = {
		{
			"homedir",
			net_sam_set_homedir,
			NET_TRANSPORT_LOCAL,
			"Change a user's home directory",
			"net sam set homedir\n"
			"    Change a user's home directory"
		},
		{
			"profilepath",
			net_sam_set_profilepath,
			NET_TRANSPORT_LOCAL,
			"Change a user's profile path",
			"net sam set profilepath\n"
			"    Change a user's profile path"
		},
		{
			"comment",
			net_sam_set_comment,
			NET_TRANSPORT_LOCAL,
			"Change a users or groups description",
			"net sam set comment\n"
			"    Change a users or groups description"
		},
		{
			"fullname",
			net_sam_set_fullname,
			NET_TRANSPORT_LOCAL,
			"Change a user's full name",
			"net sam set fullname\n"
			"    Change a user's full name"
		},
		{
			"logonscript",
			net_sam_set_logonscript,
			NET_TRANSPORT_LOCAL,
			"Change a user's logon script",
			"net sam set logonscript\n"
			"    Change a user's logon script"
		},
		{
			"homedrive",
			net_sam_set_homedrive,
			NET_TRANSPORT_LOCAL,
			"Change a user's home drive",
			"net sam set homedrive\n"
			"    Change a user's home drive"
		},
		{
			"workstations",
			net_sam_set_workstations,
			NET_TRANSPORT_LOCAL,
			"Change a user's allowed workstations",
			"net sam set workstations\n"
			"    Change a user's allowed workstations"
		},
		{
			"disabled",
			net_sam_set_disabled,
			NET_TRANSPORT_LOCAL,
			"Disable/Enable a user",
			"net sam set disable\n"
			"    Disable/Enable a user"
		},
		{
			"pwnotreq",
			net_sam_set_pwnotreq,
			NET_TRANSPORT_LOCAL,
			"Disable/Enable the password not required flag",
			"net sam set pwnotreq\n"
			"    Disable/Enable the password not required flag"
		},
		{
			"autolock",
			net_sam_set_autolock,
			NET_TRANSPORT_LOCAL,
			"Disable/Enable a user's lockout flag",
			"net sam set autolock\n"
			"    Disable/Enable a user's lockout flag"
		},
		{
			"pwnoexp",
			net_sam_set_pwnoexp,
			NET_TRANSPORT_LOCAL,
			"Disable/Enable whether a user's pw does not expire",
			"net sam set pwnoexp\n"
			"    Disable/Enable whether a user's pw does not expire"
		},
		{
			"pwdmustchangenow",
			net_sam_set_pwdmustchangenow,
			NET_TRANSPORT_LOCAL,
			"Force users password must change at next logon",
			"net sam set pwdmustchangenow\n"
			"    Force users password must change at next logon"
		},
		{NULL, NULL, 0, NULL, NULL}
	};

	return net_run_function(c, argc, argv, "net sam set", func);
}

/*
 * Manage account policies
 */

static int net_sam_policy_set(struct net_context *c, int argc, const char **argv)
{
	const char *account_policy = NULL;
	uint32 value = 0;
	uint32 old_value = 0;
	int field;
	char *endptr;

        if (argc != 2 || c->display_usage) {
                d_fprintf(stderr, "usage: net sam policy set " 
			  "\"<account policy>\" <value> \n");
                return -1;
        }

	account_policy = argv[0];
	field = account_policy_name_to_fieldnum(account_policy);

	if (strequal(argv[1], "forever") || strequal(argv[1], "never")
	    || strequal(argv[1], "off")) {
		value = -1;
	}
	else {
		value = strtoul(argv[1], &endptr, 10);

		if ((endptr == argv[1]) || (endptr[0] != '\0')) {
			d_printf("Unable to set policy \"%s\"! Invalid value "
				 "\"%s\".\n", 
				 account_policy, argv[1]); 
			return -1;
		}
	}

	if (field == 0) {
		const char **names;
                int i, count;

                account_policy_names_list(&names, &count);
		d_fprintf(stderr, "No account policy \"%s\"!\n\n", argv[0]);
		d_fprintf(stderr, "Valid account policies are:\n");

		for (i=0; i<count; i++) {
			d_fprintf(stderr, "%s\n", names[i]);
		}

		SAFE_FREE(names);
		return -1;
	}

	if (!pdb_get_account_policy(field, &old_value)) {
		d_fprintf(stderr, "Valid account policy, but unable to fetch "
			  "value!\n");
	} else {
		d_printf("Account policy \"%s\" value was: %d\n", account_policy,
			old_value);
	}

	if (!pdb_set_account_policy(field, value)) {
		d_fprintf(stderr, "Valid account policy, but unable to "
			  "set value!\n");
		return -1;
	} else {
		d_printf("Account policy \"%s\" value is now: %d\n", account_policy,
			value);
	}

	return 0;
}

static int net_sam_policy_show(struct net_context *c, int argc, const char **argv)
{
	const char *account_policy = NULL;
        uint32 old_value;
        int field;

        if (argc != 1 || c->display_usage) {
                d_fprintf(stderr, "usage: net sam policy show"
			  " \"<account policy>\" \n");
                return -1;
        }

	account_policy = argv[0];
        field = account_policy_name_to_fieldnum(account_policy);

        if (field == 0) {
		const char **names;
		int count;
		int i;
                account_policy_names_list(&names, &count);
                d_fprintf(stderr, "No account policy by that name!\n");
                if (count != 0) {
                        d_fprintf(stderr, "Valid account policies "
                                  "are:\n");
			for (i=0; i<count; i++) {
				d_fprintf(stderr, "%s\n", names[i]);
			}
                }
                SAFE_FREE(names);
                return -1;
        }

	if (!pdb_get_account_policy(field, &old_value)) {
                fprintf(stderr, "Valid account policy, but unable to "
                        "fetch value!\n");
                return -1;
        }

	printf("Account policy \"%s\" description: %s\n",
	       account_policy, account_policy_get_desc(field));
        printf("Account policy \"%s\" value is: %d\n", account_policy,
	       old_value);
        return 0;
}

static int net_sam_policy_list(struct net_context *c, int argc, const char **argv)
{
	const char **names;
	int count;
	int i;

	if (c->display_usage) {
		d_printf("Usage:\n"
			 "net sam policy list\n"
			 "    List account policies\n");
		return 0;
	}

	account_policy_names_list(&names, &count);
        if (count != 0) {
        	d_fprintf(stderr, "Valid account policies "
			  "are:\n");
		for (i = 0; i < count ; i++) {
			d_fprintf(stderr, "%s\n", names[i]);
		}
	}
        SAFE_FREE(names);
        return -1;
}

static int net_sam_policy(struct net_context *c, int argc, const char **argv)
{
        struct functable func[] = {
		{
			"list",
			net_sam_policy_list,
			NET_TRANSPORT_LOCAL,
			"List account policies",
			"net sam policy list\n"
			"    List account policies"
		},
                {
			"show",
			net_sam_policy_show,
			NET_TRANSPORT_LOCAL,
			"Show account policies",
			"net sam policy show\n"
			"    Show account policies"
		},
		{
			"set",
			net_sam_policy_set,
			NET_TRANSPORT_LOCAL,
			"Change account policies",
			"net sam policy set\n"
			"    Change account policies"
		},
                {NULL, NULL, 0, NULL, NULL}
        };

        return net_run_function(c, argc, argv, "net sam policy", func);
}

extern PRIVS privs[];

static int net_sam_rights_list(struct net_context *c, int argc,
			       const char **argv)
{
	SE_PRIV mask;

	if (argc > 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam rights list [privilege name]\n");
		return -1;
	}

	if (argc == 0) {
		int i;
		int num = count_all_privileges();

		for (i=0; i<num; i++) {
			d_printf("%s\n", privs[i].name);
		}
		return 0;
	}

	if (se_priv_from_name(argv[0], &mask)) {
		DOM_SID *sids;
		int i, num_sids;
		NTSTATUS status;

		status = privilege_enum_sids(&mask, talloc_tos(),
					     &sids, &num_sids);
		if (!NT_STATUS_IS_OK(status)) {
			d_fprintf(stderr, "Could not list rights: %s\n",
				  nt_errstr(status));
			return -1;
		}

		for (i=0; i<num_sids; i++) {
			const char *dom, *name;
			enum lsa_SidType type;

			if (lookup_sid(talloc_tos(), &sids[i], &dom, &name,
				       &type)) {
				d_printf("%s\\%s\n", dom, name);
			}
			else {
				d_printf("%s\n", sid_string_tos(&sids[i]));
			}
		}
		return 0;
	}

	return -1;
}

static int net_sam_rights_grant(struct net_context *c, int argc,
				const char **argv)
{
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;
	SE_PRIV mask;

	if (argc != 2 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam rights grant <name> "
			  "<right>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	if (!se_priv_from_name(argv[1], &mask)) {
		d_fprintf(stderr, "%s unknown\n", argv[1]);
		return -1;
	}

	if (!grant_privilege(&sid, &mask)) {
		d_fprintf(stderr, "Could not grant privilege\n");
		return -1;
	}

	d_printf("Granted %s to %s\\%s\n", argv[1], dom, name);
	return 0;
}

static int net_sam_rights_revoke(struct net_context *c, int argc, const char **argv)
{
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;
	SE_PRIV mask;

	if (argc != 2 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam rights revoke <name> "
			  "<right>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	if (!se_priv_from_name(argv[1], &mask)) {
		d_fprintf(stderr, "%s unknown\n", argv[1]);
		return -1;
	}

	if (!revoke_privilege(&sid, &mask)) {
		d_fprintf(stderr, "Could not revoke privilege\n");
		return -1;
	}

	d_printf("Revoked %s from %s\\%s\n", argv[1], dom, name);
	return 0;
}

static int net_sam_rights(struct net_context *c, int argc, const char **argv)
{
	struct functable func[] = {
		{
			"list",
			net_sam_rights_list,
			NET_TRANSPORT_LOCAL,
			"List possible user rights",
			"net sam rights list\n"
			"    List possible user rights"
		},
		{
			"grant",
			net_sam_rights_grant,
			NET_TRANSPORT_LOCAL,
			"Grant a right",
			"net sam rights grant\n"
			"    Grant a right"
		},
		{
			"revoke",
			net_sam_rights_revoke,
			NET_TRANSPORT_LOCAL,
			"Revoke a right",
			"net sam rights revoke\n"
			"    Revoke a right"
		},
		{NULL, NULL, 0, NULL, NULL}
	};
        return net_run_function(c, argc, argv, "net sam rights", func);
}

/*
 * Map a unix group to a domain group
 */

static NTSTATUS map_unix_group(const struct group *grp, GROUP_MAP *pmap)
{
	NTSTATUS status;
	GROUP_MAP map;
	const char *grpname, *dom, *name;
	uint32 rid;

	if (pdb_getgrgid(&map, grp->gr_gid)) {
		return NT_STATUS_GROUP_EXISTS;
	}

	map.gid = grp->gr_gid;
	grpname = grp->gr_name;

	if (lookup_name(talloc_tos(), grpname, LOOKUP_NAME_LOCAL,
			&dom, &name, NULL, NULL)) {

		const char *tmp = talloc_asprintf(
			talloc_tos(), "Unix Group %s", grp->gr_name);

		DEBUG(5, ("%s exists as %s\\%s, retrying as \"%s\"\n",
			  grpname, dom, name, tmp));
		grpname = tmp;
	}

	if (lookup_name(talloc_tos(), grpname, LOOKUP_NAME_LOCAL,
			NULL, NULL, NULL, NULL)) {
		DEBUG(3, ("\"%s\" exists, can't map it\n", grp->gr_name));
		return NT_STATUS_GROUP_EXISTS;
	}

	fstrcpy(map.nt_name, grpname);

	if (pdb_rid_algorithm()) {
		rid = algorithmic_pdb_gid_to_group_rid( grp->gr_gid );
	} else {
		if (!pdb_new_rid(&rid)) {
			DEBUG(3, ("Could not get a new RID for %s\n",
				  grp->gr_name));
			return NT_STATUS_ACCESS_DENIED;
		}
	}

	sid_compose(&map.sid, get_global_sam_sid(), rid);
	map.sid_name_use = SID_NAME_DOM_GRP;
	fstrcpy(map.comment, talloc_asprintf(talloc_tos(), "Unix Group %s",
					     grp->gr_name));

	status = pdb_add_group_mapping_entry(&map);
	if (NT_STATUS_IS_OK(status)) {
		*pmap = map;
	}
	return status;
}

static int net_sam_mapunixgroup(struct net_context *c, int argc, const char **argv)
{
	NTSTATUS status;
	GROUP_MAP map;
	struct group *grp;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam mapunixgroup <name>\n");
		return -1;
	}

	grp = getgrnam(argv[0]);
	if (grp == NULL) {
		d_fprintf(stderr, "Could not find group %s\n", argv[0]);
		return -1;
	}

	status = map_unix_group(grp, &map);

	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Mapping group %s failed with %s\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	d_printf("Mapped unix group %s to SID %s\n", argv[0],
		 sid_string_tos(&map.sid));

	return 0;
}

/*
 * Remove a group mapping
 */

static NTSTATUS unmap_unix_group(const struct group *grp, GROUP_MAP *pmap)
{
        NTSTATUS status;
        GROUP_MAP map;
        const char *grpname;
        DOM_SID dom_sid;

        map.gid = grp->gr_gid;
        grpname = grp->gr_name;

        if (!lookup_name(talloc_tos(), grpname, LOOKUP_NAME_LOCAL,
                        NULL, NULL, NULL, NULL)) {
                DEBUG(3, ("\"%s\" does not exist, can't unmap it\n", grp->gr_name));
                return NT_STATUS_NO_SUCH_GROUP;
        }

        fstrcpy(map.nt_name, grpname);

        if (!pdb_gid_to_sid(map.gid, &dom_sid)) {
                return NT_STATUS_UNSUCCESSFUL;
        }

        status = pdb_delete_group_mapping_entry(dom_sid);

        return status;
}

static int net_sam_unmapunixgroup(struct net_context *c, int argc, const char **argv)
{
	NTSTATUS status;
	GROUP_MAP map;
	struct group *grp;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam unmapunixgroup <name>\n");
		return -1;
	}

	grp = getgrnam(argv[0]);
	if (grp == NULL) {
		d_fprintf(stderr, "Could not find mapping for group %s.\n", argv[0]);
		return -1;
	}

	status = unmap_unix_group(grp, &map);

	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Unmapping group %s failed with %s.\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	d_printf("Unmapped unix group %s.\n", argv[0]);

	return 0;
}

/*
 * Create a local group
 */

static int net_sam_createlocalgroup(struct net_context *c, int argc, const char **argv)
{
	NTSTATUS status;
	uint32 rid;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam createlocalgroup <name>\n");
		return -1;
	}

	if (!winbind_ping()) {
		d_fprintf(stderr, "winbind seems not to run. createlocalgroup "
			  "only works when winbind runs.\n");
		return -1;
	}

	status = pdb_create_alias(argv[0], &rid);

	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Creating %s failed with %s\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	d_printf("Created local group %s with RID %d\n", argv[0], rid);

	return 0;
}

/*
 * Delete a local group
 */

static int net_sam_deletelocalgroup(struct net_context *c, int argc, const char **argv)
{
	DOM_SID sid;
        enum lsa_SidType type;
        const char *dom, *name;
	NTSTATUS status;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam deletelocalgroup <name>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find %s.\n", argv[0]);
		return -1;
	}

	if (type != SID_NAME_ALIAS) {
		d_fprintf(stderr, "%s is a %s, not a local group.\n", argv[0],
			  sid_type_lookup(type));
		return -1;
	}

	status = pdb_delete_alias(&sid);

	if (!NT_STATUS_IS_OK(status)) {
                d_fprintf(stderr, "Deleting local group %s failed with %s\n",
                          argv[0], nt_errstr(status));
                return -1;
        }

	d_printf("Deleted local group %s.\n", argv[0]);

	return 0;
}

/*
 * Create a builtin group
 */

static int net_sam_createbuiltingroup(struct net_context *c, int argc, const char **argv)
{
	NTSTATUS status;
	uint32 rid;
	enum lsa_SidType type;
	fstring groupname;
	DOM_SID sid;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam createbuiltingroup <name>\n");
		return -1;
	}

	if (!winbind_ping()) {
		d_fprintf(stderr, "winbind seems not to run. createbuiltingroup "
			  "only works when winbind runs.\n");
		return -1;
	}

	/* validate the name and get the group */

	fstrcpy( groupname, "BUILTIN\\" );
	fstrcat( groupname, argv[0] );

	if ( !lookup_name(talloc_tos(), groupname, LOOKUP_NAME_ALL, NULL,
			  NULL, &sid, &type)) {
		d_fprintf(stderr, "%s is not a BUILTIN group\n", argv[0]);
		return -1;
	}

	if ( !sid_peek_rid( &sid, &rid ) ) {
		d_fprintf(stderr, "Failed to get RID for %s\n", argv[0]);
		return -1;
	}

	status = pdb_create_builtin_alias( rid );

	if (!NT_STATUS_IS_OK(status)) {
		d_fprintf(stderr, "Creating %s failed with %s\n",
			  argv[0], nt_errstr(status));
		return -1;
	}

	d_printf("Created BUILTIN group %s with RID %d\n", argv[0], rid);

	return 0;
}

/*
 * Add a group member
 */

static int net_sam_addmem(struct net_context *c, int argc, const char **argv)
{
	const char *groupdomain, *groupname, *memberdomain, *membername;
	DOM_SID group, member;
	enum lsa_SidType grouptype, membertype;
	NTSTATUS status;

	if (argc != 2 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam addmem <group> <member>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &groupdomain, &groupname, &group, &grouptype)) {
		d_fprintf(stderr, "Could not find group %s\n", argv[0]);
		return -1;
	}

	/* check to see if the member to be added is a name or a SID */

	if (!lookup_name(talloc_tos(), argv[1], LOOKUP_NAME_LOCAL,
			 &memberdomain, &membername, &member, &membertype))
	{
		/* try it as a SID */

		if ( !string_to_sid( &member, argv[1] ) ) {
			d_fprintf(stderr, "Could not find member %s\n", argv[1]);
			return -1;
		}

		if ( !lookup_sid(talloc_tos(), &member, &memberdomain,
			&membername, &membertype) )
		{
			d_fprintf(stderr, "Could not resolve SID %s\n", argv[1]);
			return -1;
		}
	}

	if ((grouptype == SID_NAME_ALIAS) || (grouptype == SID_NAME_WKN_GRP)) {
		if ((membertype != SID_NAME_USER) &&
		    (membertype != SID_NAME_DOM_GRP)) {
			d_fprintf(stderr, "%s is a local group, only users "
				  "and domain groups can be added.\n"
				  "%s is a %s\n", argv[0], argv[1],
				  sid_type_lookup(membertype));
			return -1;
		}
		status = pdb_add_aliasmem(&group, &member);

		if (!NT_STATUS_IS_OK(status)) {
			d_fprintf(stderr, "Adding local group member failed "
				  "with %s\n", nt_errstr(status));
			return -1;
		}
	} else {
		d_fprintf(stderr, "Can only add members to local groups so "
			  "far, %s is a %s\n", argv[0],
			  sid_type_lookup(grouptype));
		return -1;
	}

	d_printf("Added %s\\%s to %s\\%s\n", memberdomain, membername,
		groupdomain, groupname);

	return 0;
}

/*
 * Delete a group member
 */

static int net_sam_delmem(struct net_context *c, int argc, const char **argv)
{
	const char *groupdomain, *groupname;
	const char *memberdomain = NULL;
	const char *membername = NULL;
	DOM_SID group, member;
	enum lsa_SidType grouptype;
	NTSTATUS status;

	if (argc != 2 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam delmem <group> <member>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &groupdomain, &groupname, &group, &grouptype)) {
		d_fprintf(stderr, "Could not find group %s\n", argv[0]);
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[1], LOOKUP_NAME_LOCAL,
			 &memberdomain, &membername, &member, NULL)) {
		if (!string_to_sid(&member, argv[1])) {
			d_fprintf(stderr, "Could not find member %s\n",
				  argv[1]);
			return -1;
		}
	}

	if ((grouptype == SID_NAME_ALIAS) ||
	    (grouptype == SID_NAME_WKN_GRP)) {
		status = pdb_del_aliasmem(&group, &member);

		if (!NT_STATUS_IS_OK(status)) {
			d_fprintf(stderr, "Deleting local group member failed "
				  "with %s\n", nt_errstr(status));
			return -1;
		}
	} else {
		d_fprintf(stderr, "Can only delete members from local groups "
			  "so far, %s is a %s\n", argv[0],
			  sid_type_lookup(grouptype));
		return -1;
	}

	if (membername != NULL) {
		d_printf("Deleted %s\\%s from %s\\%s\n",
			 memberdomain, membername, groupdomain, groupname);
	} else {
		d_printf("Deleted %s from %s\\%s\n",
			 sid_string_tos(&member), groupdomain, groupname);
	}

	return 0;
}

/*
 * List group members
 */

static int net_sam_listmem(struct net_context *c, int argc, const char **argv)
{
	const char *groupdomain, *groupname;
	DOM_SID group;
	enum lsa_SidType grouptype;
	NTSTATUS status;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam listmem <group>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &groupdomain, &groupname, &group, &grouptype)) {
		d_fprintf(stderr, "Could not find group %s\n", argv[0]);
		return -1;
	}

	if ((grouptype == SID_NAME_ALIAS) ||
	    (grouptype == SID_NAME_WKN_GRP)) {
		DOM_SID *members = NULL;
		size_t i, num_members = 0;

		status = pdb_enum_aliasmem(&group, &members, &num_members);

		if (!NT_STATUS_IS_OK(status)) {
			d_fprintf(stderr, "Listing group members failed with "
				  "%s\n", nt_errstr(status));
			return -1;
		}

		d_printf("%s\\%s has %u members\n", groupdomain, groupname,
			 (unsigned int)num_members);
		for (i=0; i<num_members; i++) {
			const char *dom, *name;
			if (lookup_sid(talloc_tos(), &members[i],
				       &dom, &name, NULL)) {
				d_printf(" %s\\%s\n", dom, name);
			} else {
				d_printf(" %s\n", sid_string_tos(&members[i]));
			}
		}

		TALLOC_FREE(members);
	} else {
		d_fprintf(stderr, "Can only list local group members so far.\n"
			  "%s is a %s\n", argv[0], sid_type_lookup(grouptype));
		return -1;
	}

	return 0;
}

/*
 * Do the listing
 */
static int net_sam_do_list(struct net_context *c, int argc, const char **argv,
			   struct pdb_search *search, const char *what)
{
	bool verbose = (argc == 1);

	if ((argc > 1) || c->display_usage ||
	    ((argc == 1) && !strequal(argv[0], "verbose"))) {
		d_fprintf(stderr, "usage: net sam list %s [verbose]\n", what);
		return -1;
	}

	if (search == NULL) {
		d_fprintf(stderr, "Could not start search\n");
		return -1;
	}

	while (true) {
		struct samr_displayentry entry;
		if (!search->next_entry(search, &entry)) {
			break;
		}
		if (verbose) {
			d_printf("%s:%d:%s\n",
				 entry.account_name,
				 entry.rid,
				 entry.description);
		} else {
			d_printf("%s\n", entry.account_name);
		}
	}

	pdb_search_destroy(search);
	return 0;
}

static int net_sam_list_users(struct net_context *c, int argc,
			      const char **argv)
{
	return net_sam_do_list(c, argc, argv, pdb_search_users(ACB_NORMAL),
			       "users");
}

static int net_sam_list_groups(struct net_context *c, int argc,
			       const char **argv)
{
	return net_sam_do_list(c, argc, argv, pdb_search_groups(), "groups");
}

static int net_sam_list_localgroups(struct net_context *c, int argc,
				    const char **argv)
{
	return net_sam_do_list(c, argc, argv,
			       pdb_search_aliases(get_global_sam_sid()),
			       "localgroups");
}

static int net_sam_list_builtin(struct net_context *c, int argc,
				const char **argv)
{
	return net_sam_do_list(c, argc, argv,
			       pdb_search_aliases(&global_sid_Builtin),
			       "builtin");
}

static int net_sam_list_workstations(struct net_context *c, int argc,
				     const char **argv)
{
	return net_sam_do_list(c, argc, argv,
			       pdb_search_users(ACB_WSTRUST),
			       "workstations");
}

/*
 * List stuff
 */

static int net_sam_list(struct net_context *c, int argc, const char **argv)
{
	struct functable func[] = {
		{
			"users",
			net_sam_list_users,
			NET_TRANSPORT_LOCAL,
			"List SAM users",
			"net sam list users\n"
			"    List SAM users"
		},
		{
			"groups",
			net_sam_list_groups,
			NET_TRANSPORT_LOCAL,
			"List SAM groups",
			"net sam list groups\n"
			"    List SAM groups"
		},
		{
			"localgroups",
			net_sam_list_localgroups,
			NET_TRANSPORT_LOCAL,
			"List SAM local groups",
			"net sam list localgroups\n"
			"    List SAM local groups"
		},
		{
			"builtin",
			net_sam_list_builtin,
			NET_TRANSPORT_LOCAL,
			"List builtin groups",
			"net sam list builtin\n"
			"    List builtin groups"
		},
		{
			"workstations",
			net_sam_list_workstations,
			NET_TRANSPORT_LOCAL,
			"List domain member workstations",
			"net sam list workstations\n"
			"    List domain member workstations"
		},
		{NULL, NULL, 0, NULL, NULL}
	};

	return net_run_function(c, argc, argv, "net sam list", func);
}

/*
 * Show details of SAM entries
 */

static int net_sam_show(struct net_context *c, int argc, const char **argv)
{
	DOM_SID sid;
	enum lsa_SidType type;
	const char *dom, *name;

	if (argc != 1 || c->display_usage) {
		d_fprintf(stderr, "usage: net sam show <name>\n");
		return -1;
	}

	if (!lookup_name(talloc_tos(), argv[0], LOOKUP_NAME_LOCAL,
			 &dom, &name, &sid, &type)) {
		d_fprintf(stderr, "Could not find name %s\n", argv[0]);
		return -1;
	}

	d_printf("%s\\%s is a %s with SID %s\n", dom, name,
		 sid_type_lookup(type), sid_string_tos(&sid));

	return 0;
}

#ifdef HAVE_LDAP

/*
 * Init an LDAP tree with default users and Groups
 * if ldapsam:editposix is enabled
 */

static int net_sam_provision(struct net_context *c, int argc, const char **argv)
{
	TALLOC_CTX *tc;
	char *ldap_bk;
	char *ldap_uri = NULL;
	char *p;
	struct smbldap_state *ls;
	GROUP_MAP gmap;
	DOM_SID gsid;
	gid_t domusers_gid = -1;
	gid_t domadmins_gid = -1;
	struct samu *samuser;
	struct passwd *pwd;

	if (c->display_usage) {
		d_printf("Usage:\n"
			 "net sam provision\n"
			 "    Init an LDAP tree with default users/groups\n");
		return 0;
	}

	tc = talloc_new(NULL);
	if (!tc) {
		d_fprintf(stderr, "Out of Memory!\n");
		return -1;
	}

	if ((ldap_bk = talloc_strdup(tc, lp_passdb_backend())) == NULL) {
		d_fprintf(stderr, "talloc failed\n");
		talloc_free(tc);
		return -1;
	}
	p = strchr(ldap_bk, ':');
	if (p) {
		*p = 0;
		ldap_uri = talloc_strdup(tc, p+1);
		trim_char(ldap_uri, ' ', ' ');
	}

	trim_char(ldap_bk, ' ', ' ');
	        
	if (strcmp(ldap_bk, "ldapsam") != 0) {
		d_fprintf(stderr, "Provisioning works only with ldapsam backend\n");
		goto failed;
	}
	
	if (!lp_parm_bool(-1, "ldapsam", "trusted", false) ||
	    !lp_parm_bool(-1, "ldapsam", "editposix", false)) {

		d_fprintf(stderr, "Provisioning works only if ldapsam:trusted"
				  " and ldapsam:editposix are enabled.\n");
		goto failed;
	}

	if (!winbind_ping()) {
		d_fprintf(stderr, "winbind seems not to run. Provisioning "
			  "LDAP only works when winbind runs.\n");
		goto failed;
	}

	if (!NT_STATUS_IS_OK(smbldap_init(tc, NULL, ldap_uri, &ls))) {
		d_fprintf(stderr, "Unable to connect to the LDAP server.\n");
		goto failed;
	}

	d_printf("Checking for Domain Users group.\n");

	sid_compose(&gsid, get_global_sam_sid(), DOMAIN_GROUP_RID_USERS);

	if (!pdb_getgrsid(&gmap, gsid)) {
		LDAPMod **mods = NULL;
		char *dn;
		char *uname;
		char *wname;
		char *gidstr;
		char *gtype;
		int rc;

		d_printf("Adding the Domain Users group.\n");

		/* lets allocate a new groupid for this group */
		if (!winbind_allocate_gid(&domusers_gid)) {
			d_fprintf(stderr, "Unable to allocate a new gid to create Domain Users group!\n");
			goto domu_done;
		}

		uname = talloc_strdup(tc, "domusers");
		wname = talloc_strdup(tc, "Domain Users");
		dn = talloc_asprintf(tc, "cn=%s,%s", "domusers", lp_ldap_group_suffix());
		gidstr = talloc_asprintf(tc, "%d", domusers_gid);
		gtype = talloc_asprintf(tc, "%d", SID_NAME_DOM_GRP);

		if (!uname || !wname || !dn || !gidstr || !gtype) {
			d_fprintf(stderr, "Out of Memory!\n");
			goto failed;
		}

		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_POSIXGROUP);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_GROUPMAP);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "cn", uname);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "displayName", wname);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "gidNumber", gidstr);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaSid",
				sid_string_talloc(tc, &gsid));
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaGroupType", gtype);

		talloc_autofree_ldapmod(tc, mods);

		rc = smbldap_add(ls, dn, mods);

		if (rc != LDAP_SUCCESS) {
			d_fprintf(stderr, "Failed to add Domain Users group to ldap directory\n");
		}
	} else {
		domusers_gid = gmap.gid;
		d_printf("found!\n");
	}	

domu_done:

	d_printf("Checking for Domain Admins group.\n");

	sid_compose(&gsid, get_global_sam_sid(), DOMAIN_GROUP_RID_ADMINS);

	if (!pdb_getgrsid(&gmap, gsid)) {
		LDAPMod **mods = NULL;
		char *dn;
		char *uname;
		char *wname;
		char *gidstr;
		char *gtype;
		int rc;

		d_printf("Adding the Domain Admins group.\n");

		/* lets allocate a new groupid for this group */
		if (!winbind_allocate_gid(&domadmins_gid)) {
			d_fprintf(stderr, "Unable to allocate a new gid to create Domain Admins group!\n");
			goto doma_done;
		}

		uname = talloc_strdup(tc, "domadmins");
		wname = talloc_strdup(tc, "Domain Admins");
		dn = talloc_asprintf(tc, "cn=%s,%s", "domadmins", lp_ldap_group_suffix());
		gidstr = talloc_asprintf(tc, "%d", domadmins_gid);
		gtype = talloc_asprintf(tc, "%d", SID_NAME_DOM_GRP);

		if (!uname || !wname || !dn || !gidstr || !gtype) {
			d_fprintf(stderr, "Out of Memory!\n");
			goto failed;
		}

		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_POSIXGROUP);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_GROUPMAP);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "cn", uname);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "displayName", wname);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "gidNumber", gidstr);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaSid",
				sid_string_talloc(tc, &gsid));
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaGroupType", gtype);

		talloc_autofree_ldapmod(tc, mods);

		rc = smbldap_add(ls, dn, mods);

		if (rc != LDAP_SUCCESS) {
			d_fprintf(stderr, "Failed to add Domain Admins group to ldap directory\n");
		}
	} else {
		domadmins_gid = gmap.gid;
		d_printf("found!\n");
	}

doma_done:

	d_printf("Check for Administrator account.\n");

	samuser = samu_new(tc);
	if (!samuser) {
		d_fprintf(stderr, "Out of Memory!\n");
		goto failed;
	}

	if (!pdb_getsampwnam(samuser, "Administrator")) {
		LDAPMod **mods = NULL;
		DOM_SID sid;
		char *dn;
		char *name;
		char *uidstr;
		char *gidstr;
		char *shell;
		char *dir;
		uid_t uid;
		int rc;
		
		d_printf("Adding the Administrator user.\n");

		if (domadmins_gid == -1) {
			d_fprintf(stderr, "Can't create Administrator user, Domain Admins group not available!\n");
			goto done;
		}
		if (!winbind_allocate_uid(&uid)) {
			d_fprintf(stderr, "Unable to allocate a new uid to create the Administrator user!\n");
			goto done;
		}
		name = talloc_strdup(tc, "Administrator");
		dn = talloc_asprintf(tc, "uid=Administrator,%s", lp_ldap_user_suffix());
		uidstr = talloc_asprintf(tc, "%d", uid);
		gidstr = talloc_asprintf(tc, "%d", domadmins_gid);
		dir = talloc_sub_specified(tc, lp_template_homedir(),
						"Administrator",
						get_global_sam_name(),
						uid, domadmins_gid);
		shell = talloc_sub_specified(tc, lp_template_shell(),
						"Administrator",
						get_global_sam_name(),
						uid, domadmins_gid);

		if (!name || !dn || !uidstr || !gidstr || !dir || !shell) {
			d_fprintf(stderr, "Out of Memory!\n");
			goto failed;
		}

		sid_compose(&sid, get_global_sam_sid(), DOMAIN_USER_RID_ADMIN);

		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_ACCOUNT);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_POSIXACCOUNT);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_SAMBASAMACCOUNT);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "uid", name);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "cn", name);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "displayName", name);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "uidNumber", uidstr);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "gidNumber", gidstr);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "homeDirectory", dir);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "loginShell", shell);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaSID",
				sid_string_talloc(tc, &sid));
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaAcctFlags",
				pdb_encode_acct_ctrl(ACB_NORMAL|ACB_DISABLED,
				NEW_PW_FORMAT_SPACE_PADDED_LEN));

		talloc_autofree_ldapmod(tc, mods);

		rc = smbldap_add(ls, dn, mods);

		if (rc != LDAP_SUCCESS) {
			d_fprintf(stderr, "Failed to add Administrator user to ldap directory\n");
		}
	} else {
		d_printf("found!\n");
	}

	d_printf("Checking for Guest user.\n");

	samuser = samu_new(tc);
	if (!samuser) {
		d_fprintf(stderr, "Out of Memory!\n");
		goto failed;
	}

	if (!pdb_getsampwnam(samuser, lp_guestaccount())) {
		LDAPMod **mods = NULL;
		DOM_SID sid;
		char *dn;
		char *uidstr;
		char *gidstr;
		int rc;
		
		d_printf("Adding the Guest user.\n");

		pwd = getpwnam_alloc(tc, lp_guestaccount());

		if (!pwd) {
			if (domusers_gid == -1) {
				d_fprintf(stderr, "Can't create Guest user, Domain Users group not available!\n");
				goto done;
			}
			if ((pwd = talloc(tc, struct passwd)) == NULL) {
				d_fprintf(stderr, "talloc failed\n");
				goto done;
			}
			pwd->pw_name = talloc_strdup(pwd, lp_guestaccount());
			if (!winbind_allocate_uid(&(pwd->pw_uid))) {
				d_fprintf(stderr, "Unable to allocate a new uid to create the Guest user!\n");
				goto done;
			}
			pwd->pw_gid = domusers_gid;
			pwd->pw_dir = talloc_strdup(tc, "/");
			pwd->pw_shell = talloc_strdup(tc, "/bin/false");
			if (!pwd->pw_dir || !pwd->pw_shell) {
				d_fprintf(stderr, "Out of Memory!\n");
				goto failed;
			}
		}

		sid_compose(&sid, get_global_sam_sid(), DOMAIN_USER_RID_GUEST);

		dn = talloc_asprintf(tc, "uid=%s,%s", pwd->pw_name, lp_ldap_user_suffix ());
		uidstr = talloc_asprintf(tc, "%d", pwd->pw_uid);
		gidstr = talloc_asprintf(tc, "%d", pwd->pw_gid);
		if (!dn || !uidstr || !gidstr) {
			d_fprintf(stderr, "Out of Memory!\n");
			goto failed;
		}

		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_ACCOUNT);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_POSIXACCOUNT);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_SAMBASAMACCOUNT);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "uid", pwd->pw_name);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "cn", pwd->pw_name);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "displayName", pwd->pw_name);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "uidNumber", uidstr);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "gidNumber", gidstr);
		if ((pwd->pw_dir != NULL) && (pwd->pw_dir[0] != '\0')) {
			smbldap_set_mod(&mods, LDAP_MOD_ADD, "homeDirectory", pwd->pw_dir);
		}
		if ((pwd->pw_shell != NULL) && (pwd->pw_shell[0] != '\0')) {
			smbldap_set_mod(&mods, LDAP_MOD_ADD, "loginShell", pwd->pw_shell);
		}
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaSID",
				sid_string_talloc(tc, &sid));
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaAcctFlags",
				pdb_encode_acct_ctrl(ACB_NORMAL|ACB_DISABLED,
				NEW_PW_FORMAT_SPACE_PADDED_LEN));

		talloc_autofree_ldapmod(tc, mods);

		rc = smbldap_add(ls, dn, mods);

		if (rc != LDAP_SUCCESS) {
			d_fprintf(stderr, "Failed to add Guest user to ldap directory\n");
		}
	} else {
		d_printf("found!\n");
	}

	d_printf("Checking Guest's group.\n");

	pwd = getpwnam_alloc(talloc_autofree_context(), lp_guestaccount());
	if (!pwd) {
		d_fprintf(stderr, "Failed to find just created Guest account!\n"
				  "   Is nss properly configured?!\n");
		goto failed;
	}

	if (pwd->pw_gid == domusers_gid) {
		d_printf("found!\n");
		goto done;
	}

	if (!pdb_getgrgid(&gmap, pwd->pw_gid)) {
		LDAPMod **mods = NULL;
		char *dn;
		char *uname;
		char *wname;
		char *gidstr;
		char *gtype;
		int rc;

		d_printf("Adding the Domain Guests group.\n");

		uname = talloc_strdup(tc, "domguests");
		wname = talloc_strdup(tc, "Domain Guests");
		dn = talloc_asprintf(tc, "cn=%s,%s", "domguests", lp_ldap_group_suffix());
		gidstr = talloc_asprintf(tc, "%d", pwd->pw_gid);
		gtype = talloc_asprintf(tc, "%d", SID_NAME_DOM_GRP);

		if (!uname || !wname || !dn || !gidstr || !gtype) {
			d_fprintf(stderr, "Out of Memory!\n");
			goto failed;
		}

		sid_compose(&gsid, get_global_sam_sid(), DOMAIN_GROUP_RID_GUESTS);

		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_POSIXGROUP);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "objectClass", LDAP_OBJ_GROUPMAP);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "cn", uname);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "displayName", wname);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "gidNumber", gidstr);
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaSid",
				sid_string_talloc(tc, &gsid));
		smbldap_set_mod(&mods, LDAP_MOD_ADD, "sambaGroupType", gtype);

		talloc_autofree_ldapmod(tc, mods);

		rc = smbldap_add(ls, dn, mods);

		if (rc != LDAP_SUCCESS) {
			d_fprintf(stderr, "Failed to add Domain Guests group to ldap directory\n");
		}
	} else {
		d_printf("found!\n");
	}


done:
	talloc_free(tc);
	return 0;

failed:
	talloc_free(tc);
	return -1;
}

#endif

/***********************************************************
 migrated functionality from smbgroupedit
 **********************************************************/
int net_sam(struct net_context *c, int argc, const char **argv)
{
	struct functable func[] = {
		{
			"createbuiltingroup",
			net_sam_createbuiltingroup,
			NET_TRANSPORT_LOCAL,
			"Create a new BUILTIN group",
			"net sam createbuiltingroup\n"
			"    Create a new BUILTIN group"
		},
		{
			"createlocalgroup",
			net_sam_createlocalgroup,
			NET_TRANSPORT_LOCAL,
			"Create a new local group",
			"net sam createlocalgroup\n"
			"    Create a new local group"
		},
		{
			"deletelocalgroup",
			net_sam_deletelocalgroup,
			NET_TRANSPORT_LOCAL,
			"Delete an existing local group",
			"net sam deletelocalgroup\n"
			"    Delete an existing local group"
		},
		{
			"mapunixgroup",
			net_sam_mapunixgroup,
			NET_TRANSPORT_LOCAL,
			"Map a unix group to a domain group",
			"net sam mapunixgroup\n"
			"    Map a unix group to a domain group"
		},
		{
			"unmapunixgroup",
			net_sam_unmapunixgroup,
			NET_TRANSPORT_LOCAL,
			"Remove a group mapping of an unix group to a domain "
			"group",
			"net sam unmapunixgroup\n"
			"    Remove a group mapping of an unix group to a "
			"domain group"
		},
		{
			"addmem",
			net_sam_addmem,
			NET_TRANSPORT_LOCAL,
			"Add a member to a group",
			"net sam addmem\n"
			"    Add a member to a group"
		},
		{
			"delmem",
			net_sam_delmem,
			NET_TRANSPORT_LOCAL,
			"Delete a member from a group",
			"net sam delmem\n"
			"    Delete a member from a group"
		},
		{
			"listmem",
			net_sam_listmem,
			NET_TRANSPORT_LOCAL,
			"List group members",
			"net sam listmem\n"
			"    List group members"
		},
		{
			"list",
			net_sam_list,
			NET_TRANSPORT_LOCAL,
			"List users, groups and local groups",
			"net sam list\n"
			"    List users, groups and local groups"
		},
		{
			"show",
			net_sam_show,
			NET_TRANSPORT_LOCAL,
			"Show details of a SAM entry",
			"net sam show\n"
			"    Show details of a SAM entry"
		},
		{
			"set",
			net_sam_set,
			NET_TRANSPORT_LOCAL,
			"Set details of a SAM account",
			"net sam set\n"
			"    Set details of a SAM account"
		},
		{
			"policy",
			net_sam_policy,
			NET_TRANSPORT_LOCAL,
			"Set account policies",
			"net sam policy\n"
			"    Set account policies"
		},
		{
			"rights",
			net_sam_rights,
			NET_TRANSPORT_LOCAL,
			"Manipulate user privileges",
			"net sam rights\n"
			"    Manipulate user privileges"
		},
#ifdef HAVE_LDAP
		{
			"provision",
			net_sam_provision,
			NET_TRANSPORT_LOCAL,
			"Provision a clean user database",
			"net sam privison\n"
			"    Provision a clear user database"
		},
#endif
		{NULL, NULL, 0, NULL, NULL}
	};

	if (getuid() != 0) {
		d_fprintf(stderr, "You are not root, most things won't "
			  "work\n");
	}

	return net_run_function(c, argc, argv, "net sam", func);
}

