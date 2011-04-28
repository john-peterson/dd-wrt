/* 
   Unix SMB/CIFS implementation.
   RPC pipe client

   Copyright (C) Tim Potter 2000-2001
   Copyright (C) Martin Pool 2003

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
#include "rpcclient.h"

DOM_SID domain_sid;

static enum pipe_auth_type pipe_default_auth_type = PIPE_AUTH_TYPE_NONE;
static enum pipe_auth_level pipe_default_auth_level = PIPE_AUTH_LEVEL_NONE;
static unsigned int timeout = 0;

/* List to hold groups of commands.
 *
 * Commands are defined in a list of arrays: arrays are easy to
 * statically declare, and lists are easier to dynamically extend.
 */

static struct cmd_list {
	struct cmd_list *prev, *next;
	struct cmd_set *cmd_set;
} *cmd_list;

/****************************************************************************
handle completion of commands for readline
****************************************************************************/
static char **completion_fn(const char *text, int start, int end)
{
#define MAX_COMPLETIONS 100
	char **matches;
	int i, count=0;
	struct cmd_list *commands = cmd_list;

#if 0	/* JERRY */
	/* FIXME!!!  -- what to do when completing argument? */
	/* for words not at the start of the line fallback 
	   to filename completion */
	if (start) 
		return NULL;
#endif

	/* make sure we have a list of valid commands */
	if (!commands) {
		return NULL;
	}

	matches = SMB_MALLOC_ARRAY(char *, MAX_COMPLETIONS);
	if (!matches) {
		return NULL;
	}

	matches[count++] = SMB_STRDUP(text);
	if (!matches[0]) {
		SAFE_FREE(matches);
		return NULL;
	}

	while (commands && count < MAX_COMPLETIONS-1) {
		if (!commands->cmd_set) {
			break;
		}
		
		for (i=0; commands->cmd_set[i].name; i++) {
			if ((strncmp(text, commands->cmd_set[i].name, strlen(text)) == 0) &&
				(( commands->cmd_set[i].returntype == RPC_RTYPE_NTSTATUS &&
                        commands->cmd_set[i].ntfn ) || 
                      ( commands->cmd_set[i].returntype == RPC_RTYPE_WERROR &&
                        commands->cmd_set[i].wfn))) {
				matches[count] = SMB_STRDUP(commands->cmd_set[i].name);
				if (!matches[count]) {
					for (i = 0; i < count; i++) {
						SAFE_FREE(matches[count]);
					}
					SAFE_FREE(matches);
					return NULL;
				}
				count++;
			}
		}
		commands = commands->next;
		
	}

	if (count == 2) {
		SAFE_FREE(matches[0]);
		matches[0] = SMB_STRDUP(matches[1]);
	}
	matches[count] = NULL;
	return matches;
}

static char *next_command (char **cmdstr)
{
	char *command;
	char			*p;
	
	if (!cmdstr || !(*cmdstr))
		return NULL;
	
	p = strchr_m(*cmdstr, ';');
	if (p)
		*p = '\0';
	command = SMB_STRDUP(*cmdstr);
	if (p)
		*cmdstr = p + 1;
	else
		*cmdstr = NULL;
	
	return command;
}

/* Fetch the SID for this computer */

static void fetch_machine_sid(struct cli_state *cli)
{
	POLICY_HND pol;
	NTSTATUS result = NT_STATUS_OK;
	static bool got_domain_sid;
	TALLOC_CTX *mem_ctx;
	struct rpc_pipe_client *lsapipe = NULL;
	union lsa_PolicyInformation *info = NULL;

	if (got_domain_sid) return;

	if (!(mem_ctx=talloc_init("fetch_machine_sid"))) {
		DEBUG(0,("fetch_machine_sid: talloc_init returned NULL!\n"));
		goto error;
	}

	result = cli_rpc_pipe_open_noauth(cli, &ndr_table_lsarpc.syntax_id,
					  &lsapipe);
	if (!NT_STATUS_IS_OK(result)) {
		fprintf(stderr, "could not initialise lsa pipe. Error was %s\n", nt_errstr(result) );
		goto error;
	}
	
	result = rpccli_lsa_open_policy(lsapipe, mem_ctx, True, 
				     SEC_RIGHTS_MAXIMUM_ALLOWED,
				     &pol);
	if (!NT_STATUS_IS_OK(result)) {
		goto error;
	}

	result = rpccli_lsa_QueryInfoPolicy(lsapipe, mem_ctx,
					    &pol,
					    LSA_POLICY_INFO_ACCOUNT_DOMAIN,
					    &info);
	if (!NT_STATUS_IS_OK(result)) {
		goto error;
	}

	got_domain_sid = True;
	sid_copy(&domain_sid, info->account_domain.sid);

	rpccli_lsa_Close(lsapipe, mem_ctx, &pol);
	TALLOC_FREE(lsapipe);
	talloc_destroy(mem_ctx);

	return;

 error:

	if (lsapipe) {
		TALLOC_FREE(lsapipe);
	}

	fprintf(stderr, "could not obtain sid for domain %s\n", cli->domain);

	if (!NT_STATUS_IS_OK(result)) {
		fprintf(stderr, "error: %s\n", nt_errstr(result));
	}

	exit(1);
}

/* List the available commands on a given pipe */

static NTSTATUS cmd_listcommands(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
				 int argc, const char **argv)
{
	struct cmd_list *tmp;
        struct cmd_set *tmp_set;
	int i;

        /* Usage */

        if (argc != 2) {
                printf("Usage: %s <pipe>\n", argv[0]);
                return NT_STATUS_OK;
        }

        /* Help on one command */

	for (tmp = cmd_list; tmp; tmp = tmp->next) 
	{
		tmp_set = tmp->cmd_set;
		
		if (!StrCaseCmp(argv[1], tmp_set->name))
		{
			printf("Available commands on the %s pipe:\n\n", tmp_set->name);

			i = 0;
			tmp_set++;
			while(tmp_set->name) {
				printf("%30s", tmp_set->name);
                                tmp_set++;
				i++;
				if (i%3 == 0)
					printf("\n");
			}
			
			/* drop out of the loop */
			break;
		}
        }
	printf("\n\n");

	return NT_STATUS_OK;
}

/* Display help on commands */

static NTSTATUS cmd_help(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
                         int argc, const char **argv)
{
	struct cmd_list *tmp;
        struct cmd_set *tmp_set;

        /* Usage */

        if (argc > 2) {
                printf("Usage: %s [command]\n", argv[0]);
                return NT_STATUS_OK;
        }

        /* Help on one command */

        if (argc == 2) {
                for (tmp = cmd_list; tmp; tmp = tmp->next) {
                        
                        tmp_set = tmp->cmd_set;

                        while(tmp_set->name) {
                                if (strequal(argv[1], tmp_set->name)) {
                                        if (tmp_set->usage &&
                                            tmp_set->usage[0])
                                                printf("%s\n", tmp_set->usage);
                                        else
                                                printf("No help for %s\n", tmp_set->name);

                                        return NT_STATUS_OK;
                                }

                                tmp_set++;
                        }
                }

                printf("No such command: %s\n", argv[1]);
                return NT_STATUS_OK;
        }

        /* List all commands */

	for (tmp = cmd_list; tmp; tmp = tmp->next) {

		tmp_set = tmp->cmd_set;

		while(tmp_set->name) {

			printf("%15s\t\t%s\n", tmp_set->name,
			       tmp_set->description ? tmp_set->description:
			       "");

			tmp_set++;
		}
	}

	return NT_STATUS_OK;
}

/* Change the debug level */

static NTSTATUS cmd_debuglevel(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
                               int argc, const char **argv)
{
	if (argc > 2) {
		printf("Usage: %s [debuglevel]\n", argv[0]);
		return NT_STATUS_OK;
	}

	if (argc == 2) {
		DEBUGLEVEL = atoi(argv[1]);
	}

	printf("debuglevel is %d\n", DEBUGLEVEL);

	return NT_STATUS_OK;
}

static NTSTATUS cmd_quit(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
                         int argc, const char **argv)
{
	exit(0);
	return NT_STATUS_OK; /* NOTREACHED */
}

static NTSTATUS cmd_set_ss_level(void)
{
	struct cmd_list *tmp;

	/* Close any existing connections not at this level. */

	for (tmp = cmd_list; tmp; tmp = tmp->next) {
        	struct cmd_set *tmp_set;

		for (tmp_set = tmp->cmd_set; tmp_set->name; tmp_set++) {
			if (tmp_set->rpc_pipe == NULL) {
				continue;
			}

			if ((tmp_set->rpc_pipe->auth->auth_type
			     != pipe_default_auth_type)
			    || (tmp_set->rpc_pipe->auth->auth_level
				!= pipe_default_auth_level)) {
				TALLOC_FREE(tmp_set->rpc_pipe);
				tmp_set->rpc_pipe = NULL;
			}
		}
	}
	return NT_STATUS_OK;
}

static NTSTATUS cmd_sign(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
                         int argc, const char **argv)
{
	const char *type = "NTLMSSP";

	pipe_default_auth_level = PIPE_AUTH_LEVEL_INTEGRITY;
	pipe_default_auth_type = PIPE_AUTH_TYPE_NTLMSSP;

	if (argc > 2) {
		printf("Usage: %s [NTLMSSP|NTLMSSP_SPNEGO|SCHANNEL]\n", argv[0]);
		return NT_STATUS_OK;
	}

	if (argc == 2) {
		type = argv[1];
		if (strequal(type, "NTLMSSP")) {
			pipe_default_auth_type = PIPE_AUTH_TYPE_NTLMSSP;
		} else if (strequal(type, "NTLMSSP_SPNEGO")) {
			pipe_default_auth_type = PIPE_AUTH_TYPE_SPNEGO_NTLMSSP;
		} else if (strequal(type, "SCHANNEL")) {
			pipe_default_auth_type = PIPE_AUTH_TYPE_SCHANNEL;
		} else {
			printf("unknown type %s\n", type);
			return NT_STATUS_INVALID_LEVEL;
		}
	}

	d_printf("Setting %s - sign\n", type);

	return cmd_set_ss_level();
}

static NTSTATUS cmd_seal(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
                         int argc, const char **argv)
{
	const char *type = "NTLMSSP";

	pipe_default_auth_level = PIPE_AUTH_LEVEL_PRIVACY;
	pipe_default_auth_type = PIPE_AUTH_TYPE_NTLMSSP;

	if (argc > 2) {
		printf("Usage: %s [NTLMSSP|NTLMSSP_SPNEGO|SCHANNEL]\n", argv[0]);
		return NT_STATUS_OK;
	}

	if (argc == 2) {
		type = argv[1];
		if (strequal(type, "NTLMSSP")) {
			pipe_default_auth_type = PIPE_AUTH_TYPE_NTLMSSP;
		} else if (strequal(type, "NTLMSSP_SPNEGO")) {
			pipe_default_auth_type = PIPE_AUTH_TYPE_SPNEGO_NTLMSSP;
		} else if (strequal(type, "SCHANNEL")) {
			pipe_default_auth_type = PIPE_AUTH_TYPE_SCHANNEL;
		} else {
			printf("unknown type %s\n", type);
			return NT_STATUS_INVALID_LEVEL;
		}
	}

	d_printf("Setting %s - sign and seal\n", type);

	return cmd_set_ss_level();
}

static NTSTATUS cmd_timeout(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
			    int argc, const char **argv)
{
	struct cmd_list *tmp;

	if (argc > 2) {
		printf("Usage: %s timeout\n", argv[0]);
		return NT_STATUS_OK;
	}

	if (argc == 2) {
		timeout = atoi(argv[1]);

		for (tmp = cmd_list; tmp; tmp = tmp->next) {
			
			struct cmd_set *tmp_set;

			for (tmp_set = tmp->cmd_set; tmp_set->name; tmp_set++) {
				if (tmp_set->rpc_pipe == NULL) {
					continue;
				}

				rpccli_set_timeout(tmp_set->rpc_pipe, timeout);
			}
		}
	}

	printf("timeout is %d\n", timeout);

	return NT_STATUS_OK;
}


static NTSTATUS cmd_none(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
                         int argc, const char **argv)
{
	pipe_default_auth_level = PIPE_AUTH_LEVEL_NONE;
	pipe_default_auth_type = PIPE_AUTH_TYPE_NONE;

	return cmd_set_ss_level();
}

static NTSTATUS cmd_schannel(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
			     int argc, const char **argv)
{
	d_printf("Setting schannel - sign and seal\n");
	pipe_default_auth_level = PIPE_AUTH_LEVEL_PRIVACY;
	pipe_default_auth_type = PIPE_AUTH_TYPE_SCHANNEL;

	return cmd_set_ss_level();
}

static NTSTATUS cmd_schannel_sign(struct rpc_pipe_client *cli, TALLOC_CTX *mem_ctx,
			     int argc, const char **argv)
{
	d_printf("Setting schannel - sign only\n");
	pipe_default_auth_level = PIPE_AUTH_LEVEL_INTEGRITY;
	pipe_default_auth_type = PIPE_AUTH_TYPE_SCHANNEL;

	return cmd_set_ss_level();
}


/* Built in rpcclient commands */

static struct cmd_set rpcclient_commands[] = {

	{ "GENERAL OPTIONS" },

	{ "help", RPC_RTYPE_NTSTATUS, cmd_help, NULL, 	  NULL, NULL,	"Get help on commands", "[command]" },
	{ "?", 	RPC_RTYPE_NTSTATUS, cmd_help, NULL,	  NULL, NULL,	"Get help on commands", "[command]" },
	{ "debuglevel", RPC_RTYPE_NTSTATUS, cmd_debuglevel, NULL,   NULL,	NULL, "Set debug level", "level" },
	{ "debug", RPC_RTYPE_NTSTATUS, cmd_debuglevel, NULL,   NULL,	NULL, "Set debug level", "level" },
	{ "list",	RPC_RTYPE_NTSTATUS, cmd_listcommands, NULL, NULL,	NULL, "List available commands on <pipe>", "pipe" },
	{ "exit", RPC_RTYPE_NTSTATUS, cmd_quit, NULL,   NULL,	NULL,	"Exit program", "" },
	{ "quit", RPC_RTYPE_NTSTATUS, cmd_quit, NULL,	  NULL,	NULL, "Exit program", "" },
	{ "sign", RPC_RTYPE_NTSTATUS, cmd_sign, NULL,	  NULL,	NULL, "Force RPC pipe connections to be signed", "" },
	{ "seal", RPC_RTYPE_NTSTATUS, cmd_seal, NULL,	  NULL,	NULL, "Force RPC pipe connections to be sealed", "" },
	{ "schannel", RPC_RTYPE_NTSTATUS, cmd_schannel, NULL,	  NULL, NULL,	"Force RPC pipe connections to be sealed with 'schannel'.  Assumes valid machine account to this domain controller.", "" },
	{ "schannelsign", RPC_RTYPE_NTSTATUS, cmd_schannel_sign, NULL,	  NULL, NULL, "Force RPC pipe connections to be signed (not sealed) with 'schannel'.  Assumes valid machine account to this domain controller.", "" },
	{ "timeout", RPC_RTYPE_NTSTATUS, cmd_timeout, NULL,	  NULL, NULL, "Set timeout (in milliseonds) for RPC operations", "" },
	{ "none", RPC_RTYPE_NTSTATUS, cmd_none, NULL,	  NULL, NULL, "Force RPC pipe connections to have no special properties", "" },

	{ NULL }
};

static struct cmd_set separator_command[] = {
	{ "---------------", MAX_RPC_RETURN_TYPE, NULL, NULL,	NULL, NULL, "----------------------" },
	{ NULL }
};


/* Various pipe commands */

extern struct cmd_set lsarpc_commands[];
extern struct cmd_set samr_commands[];
extern struct cmd_set spoolss_commands[];
extern struct cmd_set netlogon_commands[];
extern struct cmd_set srvsvc_commands[];
extern struct cmd_set dfs_commands[];
extern struct cmd_set ds_commands[];
extern struct cmd_set echo_commands[];
extern struct cmd_set shutdown_commands[];
extern struct cmd_set test_commands[];
extern struct cmd_set wkssvc_commands[];
extern struct cmd_set ntsvcs_commands[];
extern struct cmd_set drsuapi_commands[];

static struct cmd_set *rpcclient_command_list[] = {
	rpcclient_commands,
	lsarpc_commands,
	ds_commands,
	samr_commands,
	spoolss_commands,
	netlogon_commands,
	srvsvc_commands,
	dfs_commands,
	echo_commands,
	shutdown_commands,
 	test_commands,
	wkssvc_commands,
	ntsvcs_commands,
	drsuapi_commands,
	NULL
};

static void add_command_set(struct cmd_set *cmd_set)
{
	struct cmd_list *entry;

	if (!(entry = SMB_MALLOC_P(struct cmd_list))) {
		DEBUG(0, ("out of memory\n"));
		return;
	}

	ZERO_STRUCTP(entry);

	entry->cmd_set = cmd_set;
	DLIST_ADD(cmd_list, entry);
}


/**
 * Call an rpcclient function, passing an argv array.
 *
 * @param cmd Command to run, as a single string.
 **/
static NTSTATUS do_cmd(struct cli_state *cli,
		       struct cmd_set *cmd_entry,
		       int argc, char **argv)
{
	NTSTATUS ntresult;
	WERROR wresult;
	
	TALLOC_CTX *mem_ctx;

	/* Create mem_ctx */

	if (!(mem_ctx = talloc_init("do_cmd"))) {
		DEBUG(0, ("talloc_init() failed\n"));
		return NT_STATUS_NO_MEMORY;
	}

	/* Open pipe */

	if ((cmd_entry->interface != NULL) && (cmd_entry->rpc_pipe == NULL)) {
		switch (pipe_default_auth_type) {
			case PIPE_AUTH_TYPE_NONE:
				ntresult = cli_rpc_pipe_open_noauth(
					cli, cmd_entry->interface,
					&cmd_entry->rpc_pipe);
				break;
			case PIPE_AUTH_TYPE_SPNEGO_NTLMSSP:
				ntresult = cli_rpc_pipe_open_spnego_ntlmssp(
					cli, cmd_entry->interface,
					NCACN_NP,
					pipe_default_auth_level,
					lp_workgroup(),
					get_cmdline_auth_info_username(),
					get_cmdline_auth_info_password(),
					&cmd_entry->rpc_pipe);
				break;
			case PIPE_AUTH_TYPE_NTLMSSP:
				ntresult = cli_rpc_pipe_open_ntlmssp(
					cli, cmd_entry->interface,
					NCACN_NP,
					pipe_default_auth_level,
					lp_workgroup(),
					get_cmdline_auth_info_username(),
					get_cmdline_auth_info_password(),
					&cmd_entry->rpc_pipe);
				break;
			case PIPE_AUTH_TYPE_SCHANNEL:
				ntresult = cli_rpc_pipe_open_schannel(
					cli, cmd_entry->interface,
					NCACN_NP,
					pipe_default_auth_level,
					lp_workgroup(),
					&cmd_entry->rpc_pipe);
				break;
			default:
				DEBUG(0, ("Could not initialise %s. Invalid "
					  "auth type %u\n",
					  cli_get_pipe_name_from_iface(
						  debug_ctx(), cli,
						  cmd_entry->interface),
					  pipe_default_auth_type ));
				return NT_STATUS_UNSUCCESSFUL;
		}
		if (!NT_STATUS_IS_OK(ntresult)) {
			DEBUG(0, ("Could not initialise %s. Error was %s\n",
				  cli_get_pipe_name_from_iface(
					  debug_ctx(), cli,
					  cmd_entry->interface),
				  nt_errstr(ntresult) ));
			return ntresult;
		}

		if (ndr_syntax_id_equal(cmd_entry->interface,
					&ndr_table_netlogon.syntax_id)) {
			uint32_t neg_flags = NETLOGON_NEG_AUTH2_ADS_FLAGS;
			uint32 sec_channel_type;
			uchar trust_password[16];
	
			if (!secrets_fetch_trust_account_password(lp_workgroup(),
							trust_password,
							NULL, &sec_channel_type)) {
				return NT_STATUS_UNSUCCESSFUL;
			}
		
			ntresult = rpccli_netlogon_setup_creds(cmd_entry->rpc_pipe,
						cli->desthost,   /* server name */
						lp_workgroup(),  /* domain */
						global_myname(), /* client name */
						global_myname(), /* machine account name */
						trust_password,
						sec_channel_type,
						&neg_flags);

			if (!NT_STATUS_IS_OK(ntresult)) {
				DEBUG(0, ("Could not initialise credentials for %s.\n",
					  cli_get_pipe_name_from_iface(
						  debug_ctx(), cli,
						  cmd_entry->interface)));
				return ntresult;
			}
		}
	}

	/* Run command */

	if ( cmd_entry->returntype == RPC_RTYPE_NTSTATUS ) {
		ntresult = cmd_entry->ntfn(cmd_entry->rpc_pipe, mem_ctx, argc, (const char **) argv);
		if (!NT_STATUS_IS_OK(ntresult)) {
			printf("result was %s\n", nt_errstr(ntresult));
		}
	} else {
		wresult = cmd_entry->wfn(cmd_entry->rpc_pipe, mem_ctx, argc, (const char **) argv);
		/* print out the DOS error */
		if (!W_ERROR_IS_OK(wresult)) {
			printf( "result was %s\n", dos_errstr(wresult));
		}
		ntresult = W_ERROR_IS_OK(wresult)?NT_STATUS_OK:NT_STATUS_UNSUCCESSFUL;
	}

	/* Cleanup */

	talloc_destroy(mem_ctx);

	return ntresult;
}


/**
 * Process a command entered at the prompt or as part of -c
 *
 * @returns The NTSTATUS from running the command.
 **/
static NTSTATUS process_cmd(struct cli_state *cli, char *cmd)
{
	struct cmd_list *temp_list;
	NTSTATUS result = NT_STATUS_OK;
	int ret;
	int argc;
	char **argv = NULL;

	if ((ret = poptParseArgvString(cmd, &argc, (const char ***) &argv)) != 0) {
		fprintf(stderr, "rpcclient: %s\n", poptStrerror(ret));
		return NT_STATUS_UNSUCCESSFUL;
	}


	/* Walk through a dlist of arrays of commands. */
	for (temp_list = cmd_list; temp_list; temp_list = temp_list->next) {
		struct cmd_set *temp_set = temp_list->cmd_set;

		while (temp_set->name) {
			if (strequal(argv[0], temp_set->name)) {
				if (!(temp_set->returntype == RPC_RTYPE_NTSTATUS && temp_set->ntfn ) &&
                         !(temp_set->returntype == RPC_RTYPE_WERROR && temp_set->wfn )) {
					fprintf (stderr, "Invalid command\n");
					goto out_free;
				}

				result = do_cmd(cli, temp_set, argc, argv);

				goto out_free;
			}
			temp_set++;
		}
	}

	if (argv[0]) {
		printf("command not found: %s\n", argv[0]);
	}

out_free:
/* moved to do_cmd()
	if (!NT_STATUS_IS_OK(result)) {
		printf("result was %s\n", nt_errstr(result));
	}
*/

	/* NOTE: popt allocates the whole argv, including the
	 * strings, as a single block.  So a single free is
	 * enough to release it -- we don't free the
	 * individual strings.  rtfm. */
	free(argv);

	return result;
}


/* Main function */

 int main(int argc, char *argv[])
{
	int 			opt;
	static char		*cmdstr = NULL;
	const char *server;
	struct cli_state	*cli = NULL;
	static char 		*opt_ipaddr=NULL;
	struct cmd_set 		**cmd_set;
	struct sockaddr_storage server_ss;
	NTSTATUS 		nt_status;
	static int		opt_port = 0;
	fstring new_workgroup;
	int result = 0;
	TALLOC_CTX *frame = talloc_stackframe();
	uint32_t flags = 0;

	/* make sure the vars that get altered (4th field) are in
	   a fixed location or certain compilers complain */
	poptContext pc;
	struct poptOption long_options[] = {
		POPT_AUTOHELP
		{"command",	'c', POPT_ARG_STRING,	&cmdstr, 'c', "Execute semicolon separated cmds", "COMMANDS"},
		{"dest-ip", 'I', POPT_ARG_STRING,   &opt_ipaddr, 'I', "Specify destination IP address", "IP"},
		{"port", 'p', POPT_ARG_INT,   &opt_port, 'p', "Specify port number", "PORT"},
		POPT_COMMON_SAMBA
		POPT_COMMON_CONNECTION
		POPT_COMMON_CREDENTIALS
		POPT_TABLEEND
	};

	load_case_tables();

	zero_sockaddr(&server_ss);

	setlinebuf(stdout);

	/* the following functions are part of the Samba debugging
	   facilities.  See lib/debug.c */
	setup_logging("rpcclient", True);

	/* Parse options */

	pc = poptGetContext("rpcclient", argc, (const char **) argv,
			    long_options, 0);

	if (argc == 1) {
		poptPrintHelp(pc, stderr, 0);
		goto done;
	}

	while((opt = poptGetNextOpt(pc)) != -1) {
		switch (opt) {

		case 'I':
			if (!interpret_string_addr(&server_ss,
						opt_ipaddr,
						AI_NUMERICHOST)) {
				fprintf(stderr, "%s not a valid IP address\n",
					opt_ipaddr);
				result = 1;
				goto done;
			}
		}
	}

	/* Get server as remaining unparsed argument.  Print usage if more
	   than one unparsed argument is present. */

	server = poptGetArg(pc);

	if (!server || poptGetArg(pc)) {
		poptPrintHelp(pc, stderr, 0);
		result = 1;
		goto done;
	}

	poptFreeContext(pc);

	load_interfaces();

	if (!init_names()) {
		result = 1;
		goto done;
	}

	/* save the workgroup...

	   FIXME!! do we need to do this for other options as well
	   (or maybe a generic way to keep lp_load() from overwriting
	   everything)?  */

	fstrcpy( new_workgroup, lp_workgroup() );

	/* Load smb.conf file */

	if (!lp_load(get_dyn_CONFIGFILE(),True,False,False,True))
		fprintf(stderr, "Can't load %s\n", get_dyn_CONFIGFILE());

	if ( strlen(new_workgroup) != 0 )
		set_global_myworkgroup( new_workgroup );

	/*
	 * Get password
	 * from stdin if necessary
	 */

	if (get_cmdline_auth_info_use_machine_account() &&
	    !set_cmdline_auth_info_machine_account_creds()) {
		result = 1;
		goto done;
	}

	if (!get_cmdline_auth_info_got_pass()) {
		char *pass = getpass("Password:");
		if (pass) {
			set_cmdline_auth_info_password(pass);
		}
	}

	if ((server[0] == '/' && server[1] == '/') ||
			(server[0] == '\\' && server[1] ==  '\\')) {
		server += 2;
	}

	if (get_cmdline_auth_info_use_kerberos()) {
		flags |= CLI_FULL_CONNECTION_USE_KERBEROS |
			 CLI_FULL_CONNECTION_FALLBACK_AFTER_KERBEROS;
	}


	nt_status = cli_full_connection(&cli, global_myname(), server,
					opt_ipaddr ? &server_ss : NULL, opt_port,
					"IPC$", "IPC",
					get_cmdline_auth_info_username(),
					lp_workgroup(),
					get_cmdline_auth_info_password(),
					flags,
					get_cmdline_auth_info_signing_state(),NULL);

	if (!NT_STATUS_IS_OK(nt_status)) {
		DEBUG(0,("Cannot connect to server.  Error was %s\n", nt_errstr(nt_status)));
		result = 1;
		goto done;
	}

	if (get_cmdline_auth_info_smb_encrypt()) {
		nt_status = cli_cm_force_encryption(cli,
					get_cmdline_auth_info_username(),
					get_cmdline_auth_info_password(),
					lp_workgroup(),
					"IPC$");
		if (!NT_STATUS_IS_OK(nt_status)) {
			result = 1;
			goto done;
		}
	}

#if 0	/* COMMENT OUT FOR TESTING */
	memset(cmdline_auth_info.password,'X',sizeof(cmdline_auth_info.password));
#endif

	/* Load command lists */

	timeout = cli_set_timeout(cli, 10000);

	cmd_set = rpcclient_command_list;

	while(*cmd_set) {
		add_command_set(*cmd_set);
		add_command_set(separator_command);
		cmd_set++;
	}

	fetch_machine_sid(cli);

       /* Do anything specified with -c */
        if (cmdstr && cmdstr[0]) {
                char    *cmd;
                char    *p = cmdstr;

		result = 0;

                while((cmd=next_command(&p)) != NULL) {
                        NTSTATUS cmd_result = process_cmd(cli, cmd);
			SAFE_FREE(cmd);
			result = NT_STATUS_IS_ERR(cmd_result);
                }

		goto done;
        }

	/* Loop around accepting commands */

	while(1) {
		char *line = NULL;

		line = smb_readline("rpcclient $> ", NULL, completion_fn);

		if (line == NULL)
			break;

		if (line[0] != '\n')
			process_cmd(cli, line);
		SAFE_FREE(line);
	}

done:
	if (cli != NULL) {
		cli_shutdown(cli);
	}
	TALLOC_FREE(frame);
	return result;
}
