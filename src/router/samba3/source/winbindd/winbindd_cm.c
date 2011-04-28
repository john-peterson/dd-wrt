/* 
   Unix SMB/CIFS implementation.

   Winbind daemon connection manager

   Copyright (C) Tim Potter                2001
   Copyright (C) Andrew Bartlett           2002
   Copyright (C) Gerald (Jerry) Carter     2003-2005.
   Copyright (C) Volker Lendecke           2004-2005
   Copyright (C) Jeremy Allison		   2006
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
   We need to manage connections to domain controllers without having to
   mess up the main winbindd code with other issues.  The aim of the
   connection manager is to:
  
       - make connections to domain controllers and cache them
       - re-establish connections when networks or servers go down
       - centralise the policy on connection timeouts, domain controller
	 selection etc
       - manage re-entrancy for when winbindd becomes able to handle
	 multiple outstanding rpc requests
  
   Why not have connection management as part of the rpc layer like tng?
   Good question.  This code may morph into libsmb/rpc_cache.c or something
   like that but at the moment it's simply staying as part of winbind.	I
   think the TNG architecture of forcing every user of the rpc layer to use
   the connection caching system is a bad idea.	 It should be an optional
   method of using the routines.

   The TNG design is quite good but I disagree with some aspects of the
   implementation. -tpot

 */

/*
   TODO:

     - I'm pretty annoyed by all the make_nmb_name() stuff.  It should be
       moved down into another function.

     - Take care when destroying cli_structs as they can be shared between
       various sam handles.

 */

#include "includes.h"
#include "winbindd.h"

#undef DBGC_CLASS
#define DBGC_CLASS DBGC_WINBIND

struct dc_name_ip {
	fstring name;
	struct sockaddr_storage ss;
};

extern struct winbindd_methods reconnect_methods;
extern bool override_logfile;

static NTSTATUS init_dc_connection_network(struct winbindd_domain *domain);
static void set_dc_type_and_flags( struct winbindd_domain *domain );
static bool get_dcs(TALLOC_CTX *mem_ctx, struct winbindd_domain *domain,
		    struct dc_name_ip **dcs, int *num_dcs);

/****************************************************************
 Child failed to find DC's. Reschedule check.
****************************************************************/

static void msg_failed_to_go_online(struct messaging_context *msg,
				    void *private_data,
				    uint32_t msg_type,
				    struct server_id server_id,
				    DATA_BLOB *data)
{
	struct winbindd_domain *domain;
	const char *domainname = (const char *)data->data;

	if (data->data == NULL || data->length == 0) {
		return;
	}

	DEBUG(5,("msg_fail_to_go_online: received for domain %s.\n", domainname));

	for (domain = domain_list(); domain; domain = domain->next) {
		if (domain->internal) {
			continue;
		}

		if (strequal(domain->name, domainname)) {
			if (domain->online) {
				/* We're already online, ignore. */
				DEBUG(5,("msg_fail_to_go_online: domain %s "
					"already online.\n", domainname));
				continue;
			}

			/* Reschedule the online check. */
			set_domain_offline(domain);
			break;
		}
	}
}

/****************************************************************
 Actually cause a reconnect from a message.
****************************************************************/

static void msg_try_to_go_online(struct messaging_context *msg,
				 void *private_data,
				 uint32_t msg_type,
				 struct server_id server_id,
				 DATA_BLOB *data)
{
	struct winbindd_domain *domain;
	const char *domainname = (const char *)data->data;

	if (data->data == NULL || data->length == 0) {
		return;
	}

	DEBUG(5,("msg_try_to_go_online: received for domain %s.\n", domainname));

	for (domain = domain_list(); domain; domain = domain->next) {
		if (domain->internal) {
			continue;
		}

		if (strequal(domain->name, domainname)) {

			if (domain->online) {
				/* We're already online, ignore. */
				DEBUG(5,("msg_try_to_go_online: domain %s "
					"already online.\n", domainname));
				continue;
			}

			/* This call takes care of setting the online
			   flag to true if we connected, or re-adding
			   the offline handler if false. Bypasses online
			   check so always does network calls. */

			init_dc_connection_network(domain);
			break;
		}
	}
}

/****************************************************************
 Fork a child to try and contact a DC. Do this as contacting a
 DC requires blocking lookups and we don't want to block our
 parent.
****************************************************************/

static bool fork_child_dc_connect(struct winbindd_domain *domain)
{
	struct dc_name_ip *dcs = NULL;
	int num_dcs = 0;
	TALLOC_CTX *mem_ctx = NULL;
	pid_t parent_pid = sys_getpid();
	char *lfile = NULL;

	/* Stop zombies */
	CatchChild();

	if (domain->dc_probe_pid != (pid_t)-1) {
		/*
		 * We might already have a DC probe
		 * child working, check.
		 */
		if (process_exists_by_pid(domain->dc_probe_pid)) {
			DEBUG(10,("fork_child_dc_connect: pid %u already "
				"checking for DC's.\n",
				(unsigned int)domain->dc_probe_pid));
			return true;
		}
		domain->dc_probe_pid = (pid_t)-1;
	}

	domain->dc_probe_pid = sys_fork();

	if (domain->dc_probe_pid == (pid_t)-1) {
		DEBUG(0, ("fork_child_dc_connect: Could not fork: %s\n", strerror(errno)));
		return False;
	}

	if (domain->dc_probe_pid != (pid_t)0) {
		/* Parent */
		messaging_register(winbind_messaging_context(), NULL,
				   MSG_WINBIND_TRY_TO_GO_ONLINE,
				   msg_try_to_go_online);
		messaging_register(winbind_messaging_context(), NULL,
				   MSG_WINBIND_FAILED_TO_GO_ONLINE,
				   msg_failed_to_go_online);
		return True;
	}

	/* Child. */

	/* Leave messages blocked - we will never process one. */

	if (!override_logfile) {
		if (asprintf(&lfile, "%s/log.winbindd-dc-connect", get_dyn_LOGFILEBASE()) == -1) {
			DEBUG(0, ("fork_child_dc_connect: out of memory.\n"));
			_exit(1);
		}
	}

	if (!winbindd_reinit_after_fork(lfile)) {
		messaging_send_buf(winbind_messaging_context(),
				   pid_to_procid(parent_pid),
				   MSG_WINBIND_FAILED_TO_GO_ONLINE,
				   (uint8 *)domain->name,
				   strlen(domain->name)+1);
		_exit(1);
	}
	SAFE_FREE(lfile);

	mem_ctx = talloc_init("fork_child_dc_connect");
	if (!mem_ctx) {
		DEBUG(0,("talloc_init failed.\n"));
		messaging_send_buf(winbind_messaging_context(),
				   pid_to_procid(parent_pid),
				   MSG_WINBIND_FAILED_TO_GO_ONLINE,
				   (uint8 *)domain->name,
				   strlen(domain->name)+1);
		_exit(1);
	}

	if ((!get_dcs(mem_ctx, domain, &dcs, &num_dcs)) || (num_dcs == 0)) {
		/* Still offline ? Can't find DC's. */
		messaging_send_buf(winbind_messaging_context(),
				   pid_to_procid(parent_pid),
				   MSG_WINBIND_FAILED_TO_GO_ONLINE,
				   (uint8 *)domain->name,
				   strlen(domain->name)+1);
		_exit(0);
	}

	/* We got a DC. Send a message to our parent to get it to
	   try and do the same. */

	messaging_send_buf(winbind_messaging_context(),
			   pid_to_procid(parent_pid),
			   MSG_WINBIND_TRY_TO_GO_ONLINE,
			   (uint8 *)domain->name,
			   strlen(domain->name)+1);
	_exit(0);
}

/****************************************************************
 Handler triggered if we're offline to try and detect a DC.
****************************************************************/

static void check_domain_online_handler(struct event_context *ctx,
					struct timed_event *te,
					struct timeval now,
					void *private_data)
{
        struct winbindd_domain *domain =
                (struct winbindd_domain *)private_data;

	DEBUG(10,("check_domain_online_handler: called for domain "
		  "%s (online = %s)\n", domain->name, 
		  domain->online ? "True" : "False" ));

	TALLOC_FREE(domain->check_online_event);

	/* Are we still in "startup" mode ? */

	if (domain->startup && (now.tv_sec > domain->startup_time + 30)) {
		/* No longer in "startup" mode. */
		DEBUG(10,("check_domain_online_handler: domain %s no longer in 'startup' mode.\n",
			domain->name ));
		domain->startup = False;
	}

	/* We've been told to stay offline, so stay
	   that way. */

	if (get_global_winbindd_state_offline()) {
		DEBUG(10,("check_domain_online_handler: domain %s remaining globally offline\n",
			domain->name ));
		return;
	}

	/* Fork a child to test if it can contact a DC. 
	   If it can then send ourselves a message to
	   cause a reconnect. */

	fork_child_dc_connect(domain);
}

/****************************************************************
 If we're still offline setup the timeout check.
****************************************************************/

static void calc_new_online_timeout_check(struct winbindd_domain *domain)
{
	int wbr = lp_winbind_reconnect_delay();

	if (domain->startup) {
		domain->check_online_timeout = 10;
	} else if (domain->check_online_timeout < wbr) {
		domain->check_online_timeout = wbr;
	}
}

/****************************************************************
 Set domain offline and also add handler to put us back online
 if we detect a DC.
****************************************************************/

void set_domain_offline(struct winbindd_domain *domain)
{
	DEBUG(10,("set_domain_offline: called for domain %s\n",
		domain->name ));

	TALLOC_FREE(domain->check_online_event);

	if (domain->internal) {
		DEBUG(3,("set_domain_offline: domain %s is internal - logic error.\n",
			domain->name ));
		return;
	}

	domain->online = False;

	/* Offline domains are always initialized. They're
	   re-initialized when they go back online. */

	domain->initialized = True;

	/* We only add the timeout handler that checks and
	   allows us to go back online when we've not
	   been told to remain offline. */

	if (get_global_winbindd_state_offline()) {
		DEBUG(10,("set_domain_offline: domain %s remaining globally offline\n",
			domain->name ));
		return;
	}

	/* If we're in statup mode, check again in 10 seconds, not in
	   lp_winbind_reconnect_delay() seconds (which is 30 seconds by default). */

	calc_new_online_timeout_check(domain);

	domain->check_online_event = event_add_timed(winbind_event_context(),
						NULL,
						timeval_current_ofs(domain->check_online_timeout,0),
						check_domain_online_handler,
						domain);

	/* The above *has* to succeed for winbindd to work. */
	if (!domain->check_online_event) {
		smb_panic("set_domain_offline: failed to add online handler");
	}

	DEBUG(10,("set_domain_offline: added event handler for domain %s\n",
		domain->name ));

	/* Send an offline message to the idmap child when our
	   primary domain goes offline */

	if ( domain->primary ) {
		struct winbindd_child *idmap = idmap_child();
		
		if ( idmap->pid != 0 ) {
			messaging_send_buf(winbind_messaging_context(),
					   pid_to_procid(idmap->pid), 
					   MSG_WINBIND_OFFLINE, 
					   (uint8 *)domain->name, 
					   strlen(domain->name)+1);
		}			
	}

	return;	
}

/****************************************************************
 Set domain online - if allowed.
****************************************************************/

static void set_domain_online(struct winbindd_domain *domain)
{
	DEBUG(10,("set_domain_online: called for domain %s\n",
		domain->name ));

	if (domain->internal) {
		DEBUG(3,("set_domain_online: domain %s is internal - logic error.\n",
			domain->name ));
		return;
	}

	if (get_global_winbindd_state_offline()) {
		DEBUG(10,("set_domain_online: domain %s remaining globally offline\n",
			domain->name ));
		return;
	}

	winbindd_set_locator_kdc_envs(domain);

	/* If we are waiting to get a krb5 ticket, trigger immediately. */
	ccache_regain_all_now();

	/* Ok, we're out of any startup mode now... */
	domain->startup = False;

	if (domain->online == False) {
		/* We were offline - now we're online. We default to
		   using the MS-RPC backend if we started offline,
		   and if we're going online for the first time we
		   should really re-initialize the backends and the
		   checks to see if we're talking to an AD or NT domain.
		*/

		domain->initialized = False;

		/* 'reconnect_methods' is the MS-RPC backend. */
		if (domain->backend == &reconnect_methods) {
			domain->backend = NULL;
		}
	}

	/* Ensure we have no online timeout checks. */
	domain->check_online_timeout = 0;
	TALLOC_FREE(domain->check_online_event);

	/* Ensure we ignore any pending child messages. */
	messaging_deregister(winbind_messaging_context(),
			     MSG_WINBIND_TRY_TO_GO_ONLINE, NULL);
	messaging_deregister(winbind_messaging_context(),
			     MSG_WINBIND_FAILED_TO_GO_ONLINE, NULL);

	domain->online = True;

	/* Send an online message to the idmap child when our
	   primary domain comes online */

	if ( domain->primary ) {
		struct winbindd_child *idmap = idmap_child();
		
		if ( idmap->pid != 0 ) {
			messaging_send_buf(winbind_messaging_context(),
					   pid_to_procid(idmap->pid), 
					   MSG_WINBIND_ONLINE, 
					   (uint8 *)domain->name, 
					   strlen(domain->name)+1);
		}			
	}

	return;	
}

/****************************************************************
 Requested to set a domain online.
****************************************************************/

void set_domain_online_request(struct winbindd_domain *domain)
{
	struct timeval tev;

	DEBUG(10,("set_domain_online_request: called for domain %s\n",
		domain->name ));

	if (get_global_winbindd_state_offline()) {
		DEBUG(10,("set_domain_online_request: domain %s remaining globally offline\n",
			domain->name ));
		return;
	}

	/* We've been told it's safe to go online and
	   try and connect to a DC. But I don't believe it
	   because network manager seems to lie.
	   Wait at least 5 seconds. Heuristics suck... */


	GetTimeOfDay(&tev);

	/* Go into "startup" mode again. */
	domain->startup_time = tev.tv_sec;
	domain->startup = True;

	tev.tv_sec += 5;

	if (!domain->check_online_event) {
		/* If we've come from being globally offline we
		   don't have a check online event handler set.
		   We need to add one now we're trying to go
		   back online. */

		DEBUG(10,("set_domain_online_request: domain %s was globally offline.\n",
			domain->name ));
	}

	TALLOC_FREE(domain->check_online_event);

	domain->check_online_event = event_add_timed(winbind_event_context(),
						     NULL,
						     tev,
						     check_domain_online_handler,
						     domain);

	/* The above *has* to succeed for winbindd to work. */
	if (!domain->check_online_event) {
		smb_panic("set_domain_online_request: failed to add online handler");
	}
}

/****************************************************************
 Add -ve connection cache entries for domain and realm.
****************************************************************/

void winbind_add_failed_connection_entry(const struct winbindd_domain *domain,
					const char *server,
					NTSTATUS result)
{
	add_failed_connection_entry(domain->name, server, result);
	/* If this was the saf name for the last thing we talked to,
	   remove it. */
	saf_delete(domain->name);
	if (*domain->alt_name) {
		add_failed_connection_entry(domain->alt_name, server, result);
		saf_delete(domain->alt_name);
	}
	winbindd_unset_locator_kdc_env(domain);
}

/* Choose between anonymous or authenticated connections.  We need to use
   an authenticated connection if DCs have the RestrictAnonymous registry
   entry set > 0, or the "Additional restrictions for anonymous
   connections" set in the win2k Local Security Policy. 
   
   Caller to free() result in domain, username, password
*/

static void cm_get_ipc_userpass(char **username, char **domain, char **password)
{
	*username = (char *)secrets_fetch(SECRETS_AUTH_USER, NULL);
	*domain = (char *)secrets_fetch(SECRETS_AUTH_DOMAIN, NULL);
	*password = (char *)secrets_fetch(SECRETS_AUTH_PASSWORD, NULL);
	
	if (*username && **username) {

		if (!*domain || !**domain)
			*domain = smb_xstrdup(lp_workgroup());
		
		if (!*password || !**password)
			*password = smb_xstrdup("");

		DEBUG(3, ("cm_get_ipc_userpass: Retrieved auth-user from secrets.tdb [%s\\%s]\n", 
			  *domain, *username));

	} else {
		DEBUG(3, ("cm_get_ipc_userpass: No auth-user defined\n"));
		*username = smb_xstrdup("");
		*domain = smb_xstrdup("");
		*password = smb_xstrdup("");
	}
}

static bool get_dc_name_via_netlogon(struct winbindd_domain *domain,
				     fstring dcname,
				     struct sockaddr_storage *dc_ss)
{
	struct winbindd_domain *our_domain = NULL;
	struct rpc_pipe_client *netlogon_pipe = NULL;
	NTSTATUS result;
	WERROR werr;
	TALLOC_CTX *mem_ctx;
	unsigned int orig_timeout;
	const char *tmp = NULL;
	const char *p;

	/* Hmmmm. We can only open one connection to the NETLOGON pipe at the
	 * moment.... */

	if (IS_DC) {
		return False;
	}

	if (domain->primary) {
		return False;
	}

	our_domain = find_our_domain();

	if ((mem_ctx = talloc_init("get_dc_name_via_netlogon")) == NULL) {
		return False;
	}

	result = cm_connect_netlogon(our_domain, &netlogon_pipe);
	if (!NT_STATUS_IS_OK(result)) {
		talloc_destroy(mem_ctx);
		return False;
	}

	/* This call can take a long time - allow the server to time out.
	   35 seconds should do it. */

	orig_timeout = rpccli_set_timeout(netlogon_pipe, 35000);

	if (our_domain->active_directory) {
		struct netr_DsRGetDCNameInfo *domain_info = NULL;

		result = rpccli_netr_DsRGetDCName(netlogon_pipe,
						  mem_ctx,
						  our_domain->dcname,
						  domain->name,
						  NULL,
						  NULL,
						  DS_RETURN_DNS_NAME,
						  &domain_info,
						  &werr);
		if (NT_STATUS_IS_OK(result) && W_ERROR_IS_OK(werr)) {
			tmp = talloc_strdup(
				mem_ctx, domain_info->dc_unc);
			if (tmp == NULL) {
				DEBUG(0, ("talloc_strdup failed\n"));
				talloc_destroy(mem_ctx);
				return false;
			}
			if (strlen(domain->alt_name) == 0) {
				fstrcpy(domain->alt_name,
					domain_info->domain_name);
			}
			if (strlen(domain->forest_name) == 0) {
				fstrcpy(domain->forest_name,
					domain_info->forest_name);
			}
		}
	} else {
		result = rpccli_netr_GetAnyDCName(netlogon_pipe, mem_ctx,
						  our_domain->dcname,
						  domain->name,
						  &tmp,
						  &werr);
	}

	/* And restore our original timeout. */
	rpccli_set_timeout(netlogon_pipe, orig_timeout);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(10,("rpccli_netr_GetAnyDCName failed: %s\n",
			nt_errstr(result)));
		talloc_destroy(mem_ctx);
		return false;
	}

	if (!W_ERROR_IS_OK(werr)) {
		DEBUG(10,("rpccli_netr_GetAnyDCName failed: %s\n",
			   dos_errstr(werr)));
		talloc_destroy(mem_ctx);
		return false;
	}

	/* rpccli_netr_GetAnyDCName gives us a name with \\ */
	p = strip_hostname(tmp);

	fstrcpy(dcname, p);

	talloc_destroy(mem_ctx);

	DEBUG(10,("rpccli_netr_GetAnyDCName returned %s\n", dcname));

	if (!resolve_name(dcname, dc_ss, 0x20)) {
		return False;
	}

	return True;
}

/**
 * Helper function to assemble trust password and account name
 */
static NTSTATUS get_trust_creds(const struct winbindd_domain *domain,
				char **machine_password,
				char **machine_account,
				char **machine_krb5_principal)
{
	const char *account_name;
	const char *name = NULL;
	
	/* If we are a DC and this is not our own domain */

	if (IS_DC) {
		name = domain->name;
	} else {
		struct winbindd_domain *our_domain = find_our_domain();

		if (!our_domain)
			return NT_STATUS_INVALID_SERVER_STATE;		
		
		name = our_domain->name;		
	}	
	
	if (!get_trust_pw_clear(name, machine_password,
				&account_name, NULL))
	{
		return NT_STATUS_CANT_ACCESS_DOMAIN_INFO;
	}

	if ((machine_account != NULL) &&
	    (asprintf(machine_account, "%s$", account_name) == -1))
	{
		return NT_STATUS_NO_MEMORY;
	}

	/* For now assume our machine account only exists in our domain */

	if (machine_krb5_principal != NULL)
	{
		struct winbindd_domain *our_domain = find_our_domain();

		if (!our_domain) {
			return NT_STATUS_CANT_ACCESS_DOMAIN_INFO;			
		}
		
		if (asprintf(machine_krb5_principal, "%s$@%s",
			     account_name, our_domain->alt_name) == -1)
		{
			return NT_STATUS_NO_MEMORY;
		}

		strupper_m(*machine_krb5_principal);
	}

	return NT_STATUS_OK;
}

/************************************************************************
 Given a fd with a just-connected TCP connection to a DC, open a connection
 to the pipe.
************************************************************************/

static NTSTATUS cm_prepare_connection(const struct winbindd_domain *domain,
				      const int sockfd,
				      const char *controller,
				      struct cli_state **cli,
				      bool *retry)
{
	char *machine_password = NULL;
	char *machine_krb5_principal = NULL;
	char *machine_account = NULL;
	char *ipc_username = NULL;
	char *ipc_domain = NULL;
	char *ipc_password = NULL;

	struct named_mutex *mutex;

	NTSTATUS result = NT_STATUS_UNSUCCESSFUL;

	struct sockaddr peeraddr;
	socklen_t peeraddr_len;

	struct sockaddr_in *peeraddr_in = (struct sockaddr_in *)&peeraddr;

	DEBUG(10,("cm_prepare_connection: connecting to DC %s for domain %s\n",
		controller, domain->name ));

	*retry = True;

	mutex = grab_named_mutex(talloc_tos(), controller,
				 WINBIND_SERVER_MUTEX_WAIT_TIME);
	if (mutex == NULL) {
		DEBUG(0,("cm_prepare_connection: mutex grab failed for %s\n",
			 controller));
		result = NT_STATUS_POSSIBLE_DEADLOCK;
		goto done;
	}

	if ((*cli = cli_initialise()) == NULL) {
		DEBUG(1, ("Could not cli_initialize\n"));
		result = NT_STATUS_NO_MEMORY;
		goto done;
	}

	(*cli)->timeout = 10000; 	/* 10 seconds */
	(*cli)->fd = sockfd;
	fstrcpy((*cli)->desthost, controller);
	(*cli)->use_kerberos = True;

	peeraddr_len = sizeof(peeraddr);

	if ((getpeername((*cli)->fd, &peeraddr, &peeraddr_len) != 0) ||
	    (peeraddr_len != sizeof(struct sockaddr_in)) ||
	    (peeraddr_in->sin_family != PF_INET))
	{
		DEBUG(0,("cm_prepare_connection: %s\n", strerror(errno)));
		result = NT_STATUS_UNSUCCESSFUL;
		goto done;
	}

	if (ntohs(peeraddr_in->sin_port) == 139) {
		struct nmb_name calling;
		struct nmb_name called;

		make_nmb_name(&calling, global_myname(), 0x0);
		make_nmb_name(&called, "*SMBSERVER", 0x20);

		if (!cli_session_request(*cli, &calling, &called)) {
			DEBUG(8, ("cli_session_request failed for %s\n",
				  controller));
			result = NT_STATUS_UNSUCCESSFUL;
			goto done;
		}
	}

	cli_setup_signing_state(*cli, Undefined);

	if (!cli_negprot(*cli)) {
		DEBUG(1, ("cli_negprot failed\n"));
		result = NT_STATUS_UNSUCCESSFUL;
		goto done;
	}

	if (!is_dc_trusted_domain_situation(domain->name) &&
	    (*cli)->protocol >= PROTOCOL_NT1 &&
	    (*cli)->capabilities & CAP_EXTENDED_SECURITY)
	{
		ADS_STATUS ads_status;

		result = get_trust_creds(domain, &machine_password,
					 &machine_account,
					 &machine_krb5_principal);
		if (!NT_STATUS_IS_OK(result)) {
			goto anon_fallback;
		}

		if (lp_security() == SEC_ADS) {

			/* Try a krb5 session */

			(*cli)->use_kerberos = True;
			DEBUG(5, ("connecting to %s from %s with kerberos principal "
				  "[%s] and realm [%s]\n", controller, global_myname(),
				  machine_krb5_principal, domain->alt_name));

			winbindd_set_locator_kdc_envs(domain);

			ads_status = cli_session_setup_spnego(*cli,
							      machine_krb5_principal, 
							      machine_password,
							      lp_workgroup(),
							      domain->alt_name);

			if (!ADS_ERR_OK(ads_status)) {
				DEBUG(4,("failed kerberos session setup with %s\n",
					 ads_errstr(ads_status)));
			}

			result = ads_ntstatus(ads_status);
			if (NT_STATUS_IS_OK(result)) {
				/* Ensure creds are stored for NTLMSSP authenticated pipe access. */
				cli_init_creds(*cli, machine_account, lp_workgroup(), machine_password);
				goto session_setup_done;
			}
		}

		/* Fall back to non-kerberos session setup using NTLMSSP SPNEGO with the machine account. */
		(*cli)->use_kerberos = False;

		DEBUG(5, ("connecting to %s from %s with username "
			  "[%s]\\[%s]\n",  controller, global_myname(),
			  lp_workgroup(), machine_account));

		ads_status = cli_session_setup_spnego(*cli,
						      machine_account, 
						      machine_password, 
						      lp_workgroup(),
						      NULL);
		if (!ADS_ERR_OK(ads_status)) {
			DEBUG(4, ("authenticated session setup failed with %s\n",
				ads_errstr(ads_status)));
		}

		result = ads_ntstatus(ads_status);
		if (NT_STATUS_IS_OK(result)) {
			/* Ensure creds are stored for NTLMSSP authenticated pipe access. */
			cli_init_creds(*cli, machine_account, lp_workgroup(), machine_password);
			goto session_setup_done;
		}
	}

	/* Fall back to non-kerberos session setup with auth_user */

	(*cli)->use_kerberos = False;

	cm_get_ipc_userpass(&ipc_username, &ipc_domain, &ipc_password);

	if ((((*cli)->sec_mode & NEGOTIATE_SECURITY_CHALLENGE_RESPONSE) != 0) &&
	    (strlen(ipc_username) > 0)) {

		/* Only try authenticated if we have a username */

		DEBUG(5, ("connecting to %s from %s with username "
			  "[%s]\\[%s]\n",  controller, global_myname(),
			  ipc_domain, ipc_username));

		if (NT_STATUS_IS_OK(cli_session_setup(
					    *cli, ipc_username,
					    ipc_password, strlen(ipc_password)+1,
					    ipc_password, strlen(ipc_password)+1,
					    ipc_domain))) {
			/* Successful logon with given username. */
			cli_init_creds(*cli, ipc_username, ipc_domain, ipc_password);
			goto session_setup_done;
		} else {
			DEBUG(4, ("authenticated session setup with user %s\\%s failed.\n",
				ipc_domain, ipc_username ));
		}
	}

 anon_fallback:

	/* Fall back to anonymous connection, this might fail later */
	DEBUG(10,("cm_prepare_connection: falling back to anonymous "
		"connection for DC %s\n",
		controller ));

	if (NT_STATUS_IS_OK(cli_session_setup(*cli, "", NULL, 0,
					      NULL, 0, ""))) {
		DEBUG(5, ("Connected anonymously\n"));
		cli_init_creds(*cli, "", "", "");
		goto session_setup_done;
	}

	result = cli_nt_error(*cli);

	if (NT_STATUS_IS_OK(result))
		result = NT_STATUS_UNSUCCESSFUL;

	/* We can't session setup */

	goto done;

 session_setup_done:

	/* cache the server name for later connections */

	saf_store( domain->name, (*cli)->desthost );
	if (domain->alt_name && (*cli)->use_kerberos) {
		saf_store( domain->alt_name, (*cli)->desthost );
	}

	winbindd_set_locator_kdc_envs(domain);

	if (!cli_send_tconX(*cli, "IPC$", "IPC", "", 0)) {

		result = cli_nt_error(*cli);

		DEBUG(1,("failed tcon_X with %s\n", nt_errstr(result)));

		if (NT_STATUS_IS_OK(result))
			result = NT_STATUS_UNSUCCESSFUL;

		goto done;
	}

	TALLOC_FREE(mutex);
	*retry = False;

	/* set the domain if empty; needed for schannel connections */
	if ( !*(*cli)->domain ) {
		fstrcpy( (*cli)->domain, domain->name );
	}

	result = NT_STATUS_OK;

 done:
	TALLOC_FREE(mutex);
	SAFE_FREE(machine_account);
	SAFE_FREE(machine_password);
	SAFE_FREE(machine_krb5_principal);
	SAFE_FREE(ipc_username);
	SAFE_FREE(ipc_domain);
	SAFE_FREE(ipc_password);

	if (!NT_STATUS_IS_OK(result)) {
		winbind_add_failed_connection_entry(domain, controller, result);
		if ((*cli) != NULL) {
			cli_shutdown(*cli);
			*cli = NULL;
		}
	}

	return result;
}

/*******************************************************************
 Add a dcname and sockaddr_storage pair to the end of a dc_name_ip
 array.

 Keeps the list unique by not adding duplicate entries.

 @param[in] mem_ctx talloc memory context to allocate from
 @param[in] domain_name domain of the DC
 @param[in] dcname name of the DC to add to the list
 @param[in] pss Internet address and port pair to add to the list
 @param[in,out] dcs array of dc_name_ip structures to add to
 @param[in,out] num_dcs number of dcs returned in the dcs array
 @return true if the list was added to, false otherwise
*******************************************************************/

static bool add_one_dc_unique(TALLOC_CTX *mem_ctx, const char *domain_name,
			      const char *dcname, struct sockaddr_storage *pss,
			      struct dc_name_ip **dcs, int *num)
{
	int i = 0;

	if (!NT_STATUS_IS_OK(check_negative_conn_cache(domain_name, dcname))) {
		DEBUG(10, ("DC %s was in the negative conn cache\n", dcname));
		return False;
	}

	/* Make sure there's no duplicates in the list */
	for (i=0; i<*num; i++)
		if (sockaddr_equal(&(*dcs)[i].ss, pss))
			return False;

	*dcs = TALLOC_REALLOC_ARRAY(mem_ctx, *dcs, struct dc_name_ip, (*num)+1);

	if (*dcs == NULL)
		return False;

	fstrcpy((*dcs)[*num].name, dcname);
	(*dcs)[*num].ss = *pss;
	*num += 1;
	return True;
}

static bool add_sockaddr_to_array(TALLOC_CTX *mem_ctx,
				  struct sockaddr_storage *pss, uint16 port,
				  struct sockaddr_storage **addrs, int *num)
{
	*addrs = TALLOC_REALLOC_ARRAY(mem_ctx, *addrs, struct sockaddr_storage, (*num)+1);

	if (*addrs == NULL) {
		*num = 0;
		return False;
	}

	(*addrs)[*num] = *pss;
	set_sockaddr_port(&(*addrs)[*num], port);

	*num += 1;
	return True;
}

/*******************************************************************
 convert an ip to a name
*******************************************************************/

static bool dcip_to_name(TALLOC_CTX *mem_ctx,
		const struct winbindd_domain *domain,
		struct sockaddr_storage *pss,
		fstring name )
{
	struct ip_service ip_list;
	uint32_t nt_version = NETLOGON_NT_VERSION_1;

	ip_list.ss = *pss;
	ip_list.port = 0;

#ifdef WITH_ADS
	/* For active directory servers, try to get the ldap server name.
	   None of these failures should be considered critical for now */

	if (lp_security() == SEC_ADS) {
		ADS_STRUCT *ads;
		ADS_STATUS ads_status;
		char addr[INET6_ADDRSTRLEN];

		print_sockaddr(addr, sizeof(addr), pss);

		ads = ads_init(domain->alt_name, domain->name, addr);
		ads->auth.flags |= ADS_AUTH_NO_BIND;

		ads_status = ads_connect(ads);
		if (ADS_ERR_OK(ads_status)) {
			/* We got a cldap packet. */
			fstrcpy(name, ads->config.ldap_server_name);
			namecache_store(name, 0x20, 1, &ip_list);

			DEBUG(10,("dcip_to_name: flags = 0x%x\n", (unsigned int)ads->config.flags));

			if (domain->primary && (ads->config.flags & NBT_SERVER_KDC)) {
				if (ads_closest_dc(ads)) {
					char *sitename = sitename_fetch(ads->config.realm);

					/* We're going to use this KDC for this realm/domain.
					   If we are using sites, then force the krb5 libs
					   to use this KDC. */

					create_local_private_krb5_conf_for_domain(domain->alt_name,
									domain->name,
									sitename,
									pss);

					SAFE_FREE(sitename);
				} else {
					/* use an off site KDC */
					create_local_private_krb5_conf_for_domain(domain->alt_name,
									domain->name,
									NULL,
									pss);
				}
				winbindd_set_locator_kdc_envs(domain);

				/* Ensure we contact this DC also. */
				saf_store( domain->name, name);
				saf_store( domain->alt_name, name);
			}

			ads_destroy( &ads );
			return True;
		}

		ads_destroy( &ads );
	}
#endif

	/* try GETDC requests next */

	if (send_getdc_request(mem_ctx, winbind_messaging_context(),
			       pss, domain->name, &domain->sid,
			       nt_version)) {
		const char *dc_name = NULL;
		int i;
		smb_msleep(100);
		for (i=0; i<5; i++) {
			if (receive_getdc_response(mem_ctx, pss, domain->name,
						   &nt_version,
						   &dc_name, NULL)) {
				fstrcpy(name, dc_name);
				namecache_store(name, 0x20, 1, &ip_list);
				return True;
			}
			smb_msleep(500);
		}
	}

	/* try node status request */

	if ( name_status_find(domain->name, 0x1c, 0x20, pss, name) ) {
		namecache_store(name, 0x20, 1, &ip_list);
		return True;
	}
	return False;
}

/*******************************************************************
 Retrieve a list of IP addresses for domain controllers.

 The array is sorted in the preferred connection order.

 @param[in] mem_ctx talloc memory context to allocate from
 @param[in] domain domain to retrieve DCs for
 @param[out] dcs array of dcs that will be returned
 @param[out] num_dcs number of dcs returned in the dcs array
 @return always true
*******************************************************************/

static bool get_dcs(TALLOC_CTX *mem_ctx, struct winbindd_domain *domain,
		    struct dc_name_ip **dcs, int *num_dcs)
{
	fstring dcname;
	struct  sockaddr_storage ss;
	struct  ip_service *ip_list = NULL;
	int     iplist_size = 0;
	int     i;
	bool    is_our_domain;
	enum security_types sec = (enum security_types)lp_security();

	is_our_domain = strequal(domain->name, lp_workgroup());

	/* If not our domain, get the preferred DC, by asking our primary DC */
	if ( !is_our_domain
		&& get_dc_name_via_netlogon(domain, dcname, &ss)
		&& add_one_dc_unique(mem_ctx, domain->name, dcname, &ss, dcs,
		       num_dcs) )
	{
		char addr[INET6_ADDRSTRLEN];
		print_sockaddr(addr, sizeof(addr), &ss);
		DEBUG(10, ("Retrieved DC %s at %s via netlogon\n",
			   dcname, addr));
		return True;
	}

	if (sec == SEC_ADS) {
		char *sitename = NULL;

		/* We need to make sure we know the local site before
		   doing any DNS queries, as this will restrict the
		   get_sorted_dc_list() call below to only fetching
		   DNS records for the correct site. */

		/* Find any DC to get the site record.
		   We deliberately don't care about the
		   return here. */

		get_dc_name(domain->name, domain->alt_name, dcname, &ss);

		sitename = sitename_fetch(domain->alt_name);
		if (sitename) {

			/* Do the site-specific AD dns lookup first. */
			get_sorted_dc_list(domain->alt_name, sitename, &ip_list,
			       &iplist_size, True);

			/* Add ips to the DC array.  We don't look up the name
			   of the DC in this function, but we fill in the char*
			   of the ip now to make the failed connection cache
			   work */
			for ( i=0; i<iplist_size; i++ ) {
				char addr[INET6_ADDRSTRLEN];
				print_sockaddr(addr, sizeof(addr),
						&ip_list[i].ss);
				add_one_dc_unique(mem_ctx,
						domain->name,
						addr,
						&ip_list[i].ss,
						dcs,
						num_dcs);
			}

			SAFE_FREE(ip_list);
			SAFE_FREE(sitename);
			iplist_size = 0;
		}

		/* Now we add DCs from the main AD DNS lookup. */
		get_sorted_dc_list(domain->alt_name, NULL, &ip_list,
			&iplist_size, True);

		for ( i=0; i<iplist_size; i++ ) {
			char addr[INET6_ADDRSTRLEN];
			print_sockaddr(addr, sizeof(addr),
					&ip_list[i].ss);
			add_one_dc_unique(mem_ctx,
					domain->name,
					addr,
					&ip_list[i].ss,
					dcs,
					num_dcs);
		}

		SAFE_FREE(ip_list);
		iplist_size = 0;
        }

	/* Try standard netbios queries if no ADS */
	if (*num_dcs == 0) {
		get_sorted_dc_list(domain->name, NULL, &ip_list, &iplist_size,
		       False);

		for ( i=0; i<iplist_size; i++ ) {
			char addr[INET6_ADDRSTRLEN];
			print_sockaddr(addr, sizeof(addr),
					&ip_list[i].ss);
			add_one_dc_unique(mem_ctx,
					domain->name,
					addr,
					&ip_list[i].ss,
					dcs,
					num_dcs);
		}

		SAFE_FREE(ip_list);
		iplist_size = 0;
	}

	return True;
}

/*******************************************************************
 Find and make a connection to a DC in the given domain.

 @param[in] mem_ctx talloc memory context to allocate from
 @param[in] domain domain to find a dc in
 @param[out] dcname NetBIOS or FQDN of DC that's connected to
 @param[out] pss DC Internet address and port
 @param[out] fd fd of the open socket connected to the newly found dc
 @return true when a DC connection is made, false otherwise
*******************************************************************/

static bool find_new_dc(TALLOC_CTX *mem_ctx,
			struct winbindd_domain *domain,
			fstring dcname, struct sockaddr_storage *pss, int *fd)
{
	struct dc_name_ip *dcs = NULL;
	int num_dcs = 0;

	const char **dcnames = NULL;
	int num_dcnames = 0;

	struct sockaddr_storage *addrs = NULL;
	int num_addrs = 0;

	int i, fd_index;

	*fd = -1;

 again:
	if (!get_dcs(mem_ctx, domain, &dcs, &num_dcs) || (num_dcs == 0))
		return False;

	for (i=0; i<num_dcs; i++) {

		if (!add_string_to_array(mem_ctx, dcs[i].name,
				    &dcnames, &num_dcnames)) {
			return False;
		}
		if (!add_sockaddr_to_array(mem_ctx, &dcs[i].ss, 445,
				      &addrs, &num_addrs)) {
			return False;
		}

		if (!add_string_to_array(mem_ctx, dcs[i].name,
				    &dcnames, &num_dcnames)) {
			return False;
		}
		if (!add_sockaddr_to_array(mem_ctx, &dcs[i].ss, 139,
				      &addrs, &num_addrs)) {
			return False;
		}
	}

	if ((num_dcnames == 0) || (num_dcnames != num_addrs))
		return False;

	if ((addrs == NULL) || (dcnames == NULL))
		return False;

	/* 5 second timeout. */
	if (!open_any_socket_out(addrs, num_addrs, 5000, &fd_index, fd) ) {
		for (i=0; i<num_dcs; i++) {
			char ab[INET6_ADDRSTRLEN];
			print_sockaddr(ab, sizeof(ab), &dcs[i].ss);
			DEBUG(10, ("find_new_dc: open_any_socket_out failed for "
				"domain %s address %s. Error was %s\n",
				domain->name, ab, strerror(errno) ));
			winbind_add_failed_connection_entry(domain,
				dcs[i].name, NT_STATUS_UNSUCCESSFUL);
		}
		return False;
	}

	*pss = addrs[fd_index];

	if (*dcnames[fd_index] != '\0' && !is_ipaddress(dcnames[fd_index])) {
		/* Ok, we've got a name for the DC */
		fstrcpy(dcname, dcnames[fd_index]);
		return True;
	}

	/* Try to figure out the name */
	if (dcip_to_name(mem_ctx, domain, pss, dcname)) {
		return True;
	}

	/* We can not continue without the DC's name */
	winbind_add_failed_connection_entry(domain, dcs[fd_index].name,
				    NT_STATUS_UNSUCCESSFUL);

	/* Throw away all arrays as we're doing this again. */
	TALLOC_FREE(dcs);
	num_dcs = 0;

	TALLOC_FREE(dcnames);
	num_dcnames = 0;
	
	TALLOC_FREE(addrs);
	num_addrs = 0;

	close(*fd);
	*fd = -1;

	goto again;
}

static NTSTATUS cm_open_connection(struct winbindd_domain *domain,
				   struct winbindd_cm_conn *new_conn)
{
	TALLOC_CTX *mem_ctx;
	NTSTATUS result;
	char *saf_servername = saf_fetch( domain->name );
	int retries;

	if ((mem_ctx = talloc_init("cm_open_connection")) == NULL) {
		SAFE_FREE(saf_servername);
		set_domain_offline(domain);
		return NT_STATUS_NO_MEMORY;
	}

	/* we have to check the server affinity cache here since 
	   later we selecte a DC based on response time and not preference */
	   
	/* Check the negative connection cache
	   before talking to it. It going down may have
	   triggered the reconnection. */

	if ( saf_servername && NT_STATUS_IS_OK(check_negative_conn_cache( domain->name, saf_servername))) {

		DEBUG(10,("cm_open_connection: saf_servername is '%s' for domain %s\n",
			saf_servername, domain->name ));

		/* convert an ip address to a name */
		if (is_ipaddress( saf_servername ) ) {
			fstring saf_name;
			struct sockaddr_storage ss;

			if (!interpret_string_addr(&ss, saf_servername,
						AI_NUMERICHOST)) {
				return NT_STATUS_UNSUCCESSFUL;
			}
			if (dcip_to_name(mem_ctx, domain, &ss, saf_name )) {
				fstrcpy( domain->dcname, saf_name );
			} else {
				winbind_add_failed_connection_entry(
					domain, saf_servername,
					NT_STATUS_UNSUCCESSFUL);
			}
		} else {
			fstrcpy( domain->dcname, saf_servername );
		}

		SAFE_FREE( saf_servername );
	}

	for (retries = 0; retries < 3; retries++) {
		int fd = -1;
		bool retry = False;

		result = NT_STATUS_DOMAIN_CONTROLLER_NOT_FOUND;

		DEBUG(10,("cm_open_connection: dcname is '%s' for domain %s\n",
			domain->dcname, domain->name ));

		if (*domain->dcname 
			&& NT_STATUS_IS_OK(check_negative_conn_cache( domain->name, domain->dcname))
			&& (resolve_name(domain->dcname, &domain->dcaddr, 0x20)))
		{
			struct sockaddr_storage *addrs = NULL;
			int num_addrs = 0;
			int dummy = 0;

			if (!add_sockaddr_to_array(mem_ctx, &domain->dcaddr, 445, &addrs, &num_addrs)) {
				set_domain_offline(domain);
				talloc_destroy(mem_ctx);
				return NT_STATUS_NO_MEMORY;
			}
			if (!add_sockaddr_to_array(mem_ctx, &domain->dcaddr, 139, &addrs, &num_addrs)) {
				set_domain_offline(domain);
				talloc_destroy(mem_ctx);
				return NT_STATUS_NO_MEMORY;
			}

			/* 5 second timeout. */
			if (!open_any_socket_out(addrs, num_addrs, 5000, &dummy, &fd)) {
				fd = -1;
			}
		}

		if ((fd == -1) 
			&& !find_new_dc(mem_ctx, domain, domain->dcname, &domain->dcaddr, &fd))
		{
			/* This is the one place where we will
			   set the global winbindd offline state
			   to true, if a "WINBINDD_OFFLINE" entry
			   is found in the winbindd cache. */
			set_global_winbindd_state_offline();
			break;
		}

		new_conn->cli = NULL;

		result = cm_prepare_connection(domain, fd, domain->dcname,
			&new_conn->cli, &retry);

		if (!retry)
			break;
	}

	if (NT_STATUS_IS_OK(result)) {

		winbindd_set_locator_kdc_envs(domain);

		if (domain->online == False) {
			/* We're changing state from offline to online. */
			set_global_winbindd_state_online();
		}
		set_domain_online(domain);
	} else {
		/* Ensure we setup the retry handler. */
		set_domain_offline(domain);
	}

	talloc_destroy(mem_ctx);
	return result;
}

/* Close down all open pipes on a connection. */

void invalidate_cm_connection(struct winbindd_cm_conn *conn)
{
	/* We're closing down a possibly dead
	   connection. Don't have impossibly long (10s) timeouts. */

	if (conn->cli) {
		cli_set_timeout(conn->cli, 1000); /* 1 second. */
	}

	if (conn->samr_pipe != NULL) {
		TALLOC_FREE(conn->samr_pipe);
		/* Ok, it must be dead. Drop timeout to 0.5 sec. */
		if (conn->cli) {
			cli_set_timeout(conn->cli, 500);
		}
	}

	if (conn->lsa_pipe != NULL) {
		TALLOC_FREE(conn->lsa_pipe);
		/* Ok, it must be dead. Drop timeout to 0.5 sec. */
		if (conn->cli) {
			cli_set_timeout(conn->cli, 500);
		}
	}

	if (conn->lsa_pipe_tcp != NULL) {
		TALLOC_FREE(conn->lsa_pipe_tcp);
		/* Ok, it must be dead. Drop timeout to 0.5 sec. */
		if (conn->cli) {
			cli_set_timeout(conn->cli, 500);
		}
	}

	if (conn->netlogon_pipe != NULL) {
		TALLOC_FREE(conn->netlogon_pipe);
		/* Ok, it must be dead. Drop timeout to 0.5 sec. */
		if (conn->cli) {
			cli_set_timeout(conn->cli, 500);
		}
	}

	if (conn->cli) {
		cli_shutdown(conn->cli);
	}

	conn->cli = NULL;
}

void close_conns_after_fork(void)
{
	struct winbindd_domain *domain;

	for (domain = domain_list(); domain; domain = domain->next) {
		if (domain->conn.cli == NULL)
			continue;

		if (domain->conn.cli->fd == -1)
			continue;

		close(domain->conn.cli->fd);
		domain->conn.cli->fd = -1;
	}
}

static bool connection_ok(struct winbindd_domain *domain)
{
	if (domain->conn.cli == NULL) {
		DEBUG(8, ("connection_ok: Connection to %s for domain %s has NULL "
			  "cli!\n", domain->dcname, domain->name));
		return False;
	}

	if (!domain->conn.cli->initialised) {
		DEBUG(3, ("connection_ok: Connection to %s for domain %s was never "
			  "initialised!\n", domain->dcname, domain->name));
		return False;
	}

	if (domain->conn.cli->fd == -1) {
		DEBUG(3, ("connection_ok: Connection to %s for domain %s has died or was "
			  "never started (fd == -1)\n", 
			  domain->dcname, domain->name));
		return False;
	}

	if (domain->online == False) {
		DEBUG(3, ("connection_ok: Domain %s is offline\n", domain->name));
		return False;
	}

	return True;
}

/* Initialize a new connection up to the RPC BIND.
   Bypass online status check so always does network calls. */

static NTSTATUS init_dc_connection_network(struct winbindd_domain *domain)
{
	NTSTATUS result;

	/* Internal connections never use the network. */
	if (domain->internal || !winbindd_can_contact_domain(domain)) {
		domain->initialized = True;
		return NT_STATUS_OK;
	}

	if (connection_ok(domain)) {
		if (!domain->initialized) {
			set_dc_type_and_flags(domain);
		}
		return NT_STATUS_OK;
	}

	invalidate_cm_connection(&domain->conn);

	result = cm_open_connection(domain, &domain->conn);

	if (NT_STATUS_IS_OK(result) && !domain->initialized) {
		set_dc_type_and_flags(domain);
	}

	return result;
}

NTSTATUS init_dc_connection(struct winbindd_domain *domain)
{
	if (domain->initialized && !domain->online) {
		/* We check for online status elsewhere. */
		return NT_STATUS_DOMAIN_CONTROLLER_NOT_FOUND;
	}

	return init_dc_connection_network(domain);
}

/******************************************************************************
 Set the trust flags (direction and forest location) for a domain
******************************************************************************/

static bool set_dc_type_and_flags_trustinfo( struct winbindd_domain *domain )
{
	struct winbindd_domain *our_domain;
	NTSTATUS result = NT_STATUS_UNSUCCESSFUL;
	struct netr_DomainTrustList trusts;
	int i;
	uint32 flags = (NETR_TRUST_FLAG_IN_FOREST |
			NETR_TRUST_FLAG_OUTBOUND |
			NETR_TRUST_FLAG_INBOUND);
	struct rpc_pipe_client *cli;
	TALLOC_CTX *mem_ctx = NULL;

	DEBUG(5, ("set_dc_type_and_flags_trustinfo: domain %s\n", domain->name ));
	
	/* Our primary domain doesn't need to worry about trust flags.
	   Force it to go through the network setup */
	if ( domain->primary ) {		
		return False;		
	}
	
	our_domain = find_our_domain();
	
	if ( !connection_ok(our_domain) ) {
		DEBUG(3,("set_dc_type_and_flags_trustinfo: No connection to our domain!\n"));		
		return False;
	}

	/* This won't work unless our domain is AD */
	 
	if ( !our_domain->active_directory ) {
		return False;
	}
	
	/* Use DsEnumerateDomainTrusts to get us the trust direction
	   and type */

	result = cm_connect_netlogon(our_domain, &cli);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(5, ("set_dc_type_and_flags_trustinfo: Could not open "
			  "a connection to %s for PIPE_NETLOGON (%s)\n", 
			  domain->name, nt_errstr(result)));
		return False;
	}

	if ( (mem_ctx = talloc_init("set_dc_type_and_flags_trustinfo")) == NULL ) {
		DEBUG(0,("set_dc_type_and_flags_trustinfo: talloc_init() failed!\n"));
		return False;
	}	

	result = rpccli_netr_DsrEnumerateDomainTrusts(cli, mem_ctx,
						      cli->desthost,
						      flags,
						      &trusts,
						      NULL);
	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(0,("set_dc_type_and_flags_trustinfo: "
			"failed to query trusted domain list: %s\n",
			nt_errstr(result)));
		talloc_destroy(mem_ctx);
		return false;
	}

	/* Now find the domain name and get the flags */

	for ( i=0; i<trusts.count; i++ ) {
		if ( strequal( domain->name, trusts.array[i].netbios_name) ) {
			domain->domain_flags          = trusts.array[i].trust_flags;
			domain->domain_type           = trusts.array[i].trust_type;
			domain->domain_trust_attribs  = trusts.array[i].trust_attributes;

			if ( domain->domain_type == NETR_TRUST_TYPE_UPLEVEL )
				domain->active_directory = True;

			/* This flag is only set if the domain is *our* 
			   primary domain and the primary domain is in
			   native mode */

			domain->native_mode = (domain->domain_flags & NETR_TRUST_FLAG_NATIVE);

			DEBUG(5, ("set_dc_type_and_flags_trustinfo: domain %s is %sin "
				  "native mode.\n", domain->name, 
				  domain->native_mode ? "" : "NOT "));

			DEBUG(5,("set_dc_type_and_flags_trustinfo: domain %s is %s"
				 "running active directory.\n", domain->name, 
				 domain->active_directory ? "" : "NOT "));


			domain->initialized = True;

			break;
		}		
	}
	
	talloc_destroy( mem_ctx );
	
	return domain->initialized;	
}

/******************************************************************************
 We can 'sense' certain things about the DC by it's replies to certain
 questions.

 This tells us if this particular remote server is Active Directory, and if it
 is native mode.
******************************************************************************/

static void set_dc_type_and_flags_connect( struct winbindd_domain *domain )
{
	NTSTATUS 		result;
	WERROR werr;
	TALLOC_CTX              *mem_ctx = NULL;
	struct rpc_pipe_client  *cli = NULL;
	POLICY_HND pol;
	union dssetup_DsRoleInfo info;
	union lsa_PolicyInformation *lsa_info = NULL;

	if (!connection_ok(domain)) {
		return;
	}

	mem_ctx = talloc_init("set_dc_type_and_flags on domain %s\n",
			      domain->name);
	if (!mem_ctx) {
		DEBUG(1, ("set_dc_type_and_flags_connect: talloc_init() failed\n"));
		return;
	}

	DEBUG(5, ("set_dc_type_and_flags_connect: domain %s\n", domain->name ));

	result = cli_rpc_pipe_open_noauth(domain->conn.cli,
					  &ndr_table_dssetup.syntax_id,
					  &cli);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(5, ("set_dc_type_and_flags_connect: Could not bind to "
			  "PI_DSSETUP on domain %s: (%s)\n",
			  domain->name, nt_errstr(result)));

		/* if this is just a non-AD domain we need to continue
		 * identifying so that we can in the end return with
		 * domain->initialized = True - gd */

		goto no_dssetup;
	}

	result = rpccli_dssetup_DsRoleGetPrimaryDomainInformation(cli, mem_ctx,
								  DS_ROLE_BASIC_INFORMATION,
								  &info,
								  &werr);
	TALLOC_FREE(cli);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(5, ("set_dc_type_and_flags_connect: rpccli_ds_getprimarydominfo "
			  "on domain %s failed: (%s)\n",
			  domain->name, nt_errstr(result)));

		/* older samba3 DCs will return DCERPC_FAULT_OP_RNG_ERROR for
		 * every opcode on the DSSETUP pipe, continue with
		 * no_dssetup mode here as well to get domain->initialized
		 * set - gd */

		if (NT_STATUS_V(result) == DCERPC_FAULT_OP_RNG_ERROR) {
			goto no_dssetup;
		}

		TALLOC_FREE(mem_ctx);
		return;
	}

	if ((info.basic.flags & DS_ROLE_PRIMARY_DS_RUNNING) &&
	    !(info.basic.flags & DS_ROLE_PRIMARY_DS_MIXED_MODE)) {
		domain->native_mode = True;
	} else {
		domain->native_mode = False;
	}

no_dssetup:
	result = cli_rpc_pipe_open_noauth(domain->conn.cli,
					  &ndr_table_lsarpc.syntax_id, &cli);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(5, ("set_dc_type_and_flags_connect: Could not bind to "
			  "PI_LSARPC on domain %s: (%s)\n",
			  domain->name, nt_errstr(result)));
		TALLOC_FREE(cli);
		TALLOC_FREE(mem_ctx);
		return;
	}

	result = rpccli_lsa_open_policy2(cli, mem_ctx, True, 
					 SEC_RIGHTS_MAXIMUM_ALLOWED, &pol);
		
	if (NT_STATUS_IS_OK(result)) {
		/* This particular query is exactly what Win2k clients use 
		   to determine that the DC is active directory */
		result = rpccli_lsa_QueryInfoPolicy2(cli, mem_ctx,
						     &pol,
						     LSA_POLICY_INFO_DNS,
						     &lsa_info);
	}

	if (NT_STATUS_IS_OK(result)) {
		domain->active_directory = True;

		if (lsa_info->dns.name.string) {
			fstrcpy(domain->name, lsa_info->dns.name.string);
		}

		if (lsa_info->dns.dns_domain.string) {
			fstrcpy(domain->alt_name,
				lsa_info->dns.dns_domain.string);
		}

		/* See if we can set some domain trust flags about
		   ourself */

		if (lsa_info->dns.dns_forest.string) {
			fstrcpy(domain->forest_name,
				lsa_info->dns.dns_forest.string);

			if (strequal(domain->forest_name, domain->alt_name)) {
				domain->domain_flags |= NETR_TRUST_FLAG_TREEROOT;
			}
		}

		if (lsa_info->dns.sid) {
			sid_copy(&domain->sid, lsa_info->dns.sid);
		}
	} else {
		domain->active_directory = False;

		result = rpccli_lsa_open_policy(cli, mem_ctx, True, 
						SEC_RIGHTS_MAXIMUM_ALLOWED,
						&pol);

		if (!NT_STATUS_IS_OK(result)) {
			goto done;
		}

		result = rpccli_lsa_QueryInfoPolicy(cli, mem_ctx,
						    &pol,
						    LSA_POLICY_INFO_ACCOUNT_DOMAIN,
						    &lsa_info);

		if (NT_STATUS_IS_OK(result)) {

			if (lsa_info->account_domain.name.string) {
				fstrcpy(domain->name,
					lsa_info->account_domain.name.string);
			}

			if (lsa_info->account_domain.sid) {
				sid_copy(&domain->sid, lsa_info->account_domain.sid);
			}
		}
	}
done:

	DEBUG(5, ("set_dc_type_and_flags_connect: domain %s is %sin native mode.\n",
		  domain->name, domain->native_mode ? "" : "NOT "));

	DEBUG(5,("set_dc_type_and_flags_connect: domain %s is %srunning active directory.\n",
		  domain->name, domain->active_directory ? "" : "NOT "));

	domain->can_do_ncacn_ip_tcp = domain->active_directory;

	TALLOC_FREE(cli);

	TALLOC_FREE(mem_ctx);

	domain->initialized = True;
}

/**********************************************************************
 Set the domain_flags (trust attributes, domain operating modes, etc... 
***********************************************************************/

static void set_dc_type_and_flags( struct winbindd_domain *domain )
{
	/* we always have to contact our primary domain */

	if ( domain->primary ) {
		DEBUG(10,("set_dc_type_and_flags: setting up flags for "
			  "primary domain\n"));
		set_dc_type_and_flags_connect( domain );
		return;		
	}

	/* Use our DC to get the information if possible */

	if ( !set_dc_type_and_flags_trustinfo( domain ) ) {
		/* Otherwise, fallback to contacting the 
		   domain directly */
		set_dc_type_and_flags_connect( domain );
	}

	return;
}



/**********************************************************************
***********************************************************************/

static bool cm_get_schannel_dcinfo(struct winbindd_domain *domain,
				   struct dcinfo **ppdc)
{
	NTSTATUS result;
	struct rpc_pipe_client *netlogon_pipe;

	if (lp_client_schannel() == False) {
		return False;
	}

	result = cm_connect_netlogon(domain, &netlogon_pipe);
	if (!NT_STATUS_IS_OK(result)) {
		return False;
	}

	/* Return a pointer to the struct dcinfo from the
	   netlogon pipe. */

	if (!domain->conn.netlogon_pipe->dc) {
		return false;
	}

	*ppdc = domain->conn.netlogon_pipe->dc;
	return True;
}

NTSTATUS cm_connect_sam(struct winbindd_domain *domain, TALLOC_CTX *mem_ctx,
			struct rpc_pipe_client **cli, POLICY_HND *sam_handle)
{
	struct winbindd_cm_conn *conn;
	NTSTATUS result = NT_STATUS_UNSUCCESSFUL;
	fstring conn_pwd;
	struct dcinfo *p_dcinfo;
	char *machine_password = NULL;
	char *machine_account = NULL;
	char *domain_name = NULL;

	result = init_dc_connection(domain);
	if (!NT_STATUS_IS_OK(result)) {
		return result;
	}

	conn = &domain->conn;

	if (conn->samr_pipe != NULL) {
		goto done;
	}


	/*
	 * No SAMR pipe yet. Attempt to get an NTLMSSP SPNEGO authenticated
	 * sign and sealed pipe using the machine account password by
	 * preference. If we can't - try schannel, if that fails, try
	 * anonymous.
	 */

	pwd_get_cleartext(&conn->cli->pwd, conn_pwd);
	if ((conn->cli->user_name[0] == '\0') ||
	    (conn->cli->domain[0] == '\0') || 
	    (conn_pwd[0] == '\0'))
	{
		result = get_trust_creds(domain, &machine_password,
					 &machine_account, NULL);
		if (!NT_STATUS_IS_OK(result)) {
			DEBUG(10, ("cm_connect_sam: No no user available for "
				   "domain %s, trying schannel\n", conn->cli->domain));
			goto schannel;
		}
		domain_name = domain->name;
	} else {
		machine_password = SMB_STRDUP(conn_pwd);		
		machine_account = SMB_STRDUP(conn->cli->user_name);
		domain_name = conn->cli->domain;
	}

	if (!machine_password || !machine_account) {
		result = NT_STATUS_NO_MEMORY;
		goto done;
	}

	/* We have an authenticated connection. Use a NTLMSSP SPNEGO
	   authenticated SAMR pipe with sign & seal. */
	result = cli_rpc_pipe_open_spnego_ntlmssp(conn->cli,
						  &ndr_table_samr.syntax_id,
						  NCACN_NP,
						  PIPE_AUTH_LEVEL_PRIVACY,
						  domain_name,
						  machine_account,
						  machine_password,
						  &conn->samr_pipe);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(10,("cm_connect_sam: failed to connect to SAMR "
			  "pipe for domain %s using NTLMSSP "
			  "authenticated pipe: user %s\\%s. Error was "
			  "%s\n", domain->name, domain_name,
			  machine_account, nt_errstr(result)));
		goto schannel;
	}

	DEBUG(10,("cm_connect_sam: connected to SAMR pipe for "
		  "domain %s using NTLMSSP authenticated "
		  "pipe: user %s\\%s\n", domain->name,
		  domain_name, machine_account));

	result = rpccli_samr_Connect2(conn->samr_pipe, mem_ctx,
				      conn->samr_pipe->desthost,
				      SEC_RIGHTS_MAXIMUM_ALLOWED,
				      &conn->sam_connect_handle);
	if (NT_STATUS_IS_OK(result)) {
		goto open_domain;
	}
	DEBUG(10,("cm_connect_sam: ntlmssp-sealed rpccli_samr_Connect2 "
		  "failed for domain %s, error was %s. Trying schannel\n",
		  domain->name, nt_errstr(result) ));
	TALLOC_FREE(conn->samr_pipe);

 schannel:

	/* Fall back to schannel if it's a W2K pre-SP1 box. */

	if (!cm_get_schannel_dcinfo(domain, &p_dcinfo)) {
		/* If this call fails - conn->cli can now be NULL ! */
		DEBUG(10, ("cm_connect_sam: Could not get schannel auth info "
			   "for domain %s, trying anon\n", domain->name));
		goto anonymous;
	}
	result = cli_rpc_pipe_open_schannel_with_key
		(conn->cli, &ndr_table_samr.syntax_id, NCACN_NP,
		PIPE_AUTH_LEVEL_PRIVACY,
		 domain->name, p_dcinfo, &conn->samr_pipe);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(10,("cm_connect_sam: failed to connect to SAMR pipe for "
			  "domain %s using schannel. Error was %s\n",
			  domain->name, nt_errstr(result) ));
		goto anonymous;
	}
	DEBUG(10,("cm_connect_sam: connected to SAMR pipe for domain %s using "
		  "schannel.\n", domain->name ));

	result = rpccli_samr_Connect2(conn->samr_pipe, mem_ctx,
				      conn->samr_pipe->desthost,
				      SEC_RIGHTS_MAXIMUM_ALLOWED,
				      &conn->sam_connect_handle);
	if (NT_STATUS_IS_OK(result)) {
		goto open_domain;
	}
	DEBUG(10,("cm_connect_sam: schannel-sealed rpccli_samr_Connect2 failed "
		  "for domain %s, error was %s. Trying anonymous\n",
		  domain->name, nt_errstr(result) ));
	TALLOC_FREE(conn->samr_pipe);

 anonymous:

	/* Finally fall back to anonymous. */
	result = cli_rpc_pipe_open_noauth(conn->cli, &ndr_table_samr.syntax_id,
					  &conn->samr_pipe);

	if (!NT_STATUS_IS_OK(result)) {
		goto done;
	}

	result = rpccli_samr_Connect2(conn->samr_pipe, mem_ctx,
				      conn->samr_pipe->desthost,
				      SEC_RIGHTS_MAXIMUM_ALLOWED,
				      &conn->sam_connect_handle);
	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(10,("cm_connect_sam: rpccli_samr_Connect2 failed "
			  "for domain %s Error was %s\n",
			  domain->name, nt_errstr(result) ));
		goto done;
	}

 open_domain:
	result = rpccli_samr_OpenDomain(conn->samr_pipe,
					mem_ctx,
					&conn->sam_connect_handle,
					SEC_RIGHTS_MAXIMUM_ALLOWED,
					&domain->sid,
					&conn->sam_domain_handle);

 done:

	if (NT_STATUS_EQUAL(result, NT_STATUS_ACCESS_DENIED)) {
		/*
		 * if we got access denied, we might just have no access rights
		 * to talk to the remote samr server server (e.g. when we are a
		 * PDC and we are connecting a w2k8 pdc via an interdomain
		 * trust). In that case do not invalidate the whole connection
		 * stack
		 */
		TALLOC_FREE(conn->samr_pipe);
		ZERO_STRUCT(conn->sam_domain_handle);
		return result;
	} else if (!NT_STATUS_IS_OK(result)) {
		invalidate_cm_connection(conn);
		return result;
	}

	*cli = conn->samr_pipe;
	*sam_handle = conn->sam_domain_handle;
	SAFE_FREE(machine_password);
	SAFE_FREE(machine_account);
	return result;
}

/**********************************************************************
 open an schanneld ncacn_ip_tcp connection to LSA
***********************************************************************/

NTSTATUS cm_connect_lsa_tcp(struct winbindd_domain *domain,
			    TALLOC_CTX *mem_ctx,
			    struct rpc_pipe_client **cli)
{
	struct winbindd_cm_conn *conn;
	NTSTATUS status;

	DEBUG(10,("cm_connect_lsa_tcp\n"));

	status = init_dc_connection(domain);
	if (!NT_STATUS_IS_OK(status)) {
		return status;
	}

	conn = &domain->conn;

	if (conn->lsa_pipe_tcp &&
	    conn->lsa_pipe_tcp->transport_type == NCACN_IP_TCP &&
	    conn->lsa_pipe_tcp->auth->auth_level == PIPE_AUTH_LEVEL_PRIVACY) {
		goto done;
	}

	TALLOC_FREE(conn->lsa_pipe_tcp);

	status = cli_rpc_pipe_open_schannel(conn->cli,
					    &ndr_table_lsarpc.syntax_id,
					    NCACN_IP_TCP,
					    PIPE_AUTH_LEVEL_PRIVACY,
					    domain->name,
					    &conn->lsa_pipe_tcp);
	if (!NT_STATUS_IS_OK(status)) {
		DEBUG(10,("cli_rpc_pipe_open_schannel failed: %s\n",
			nt_errstr(status)));
		goto done;
	}

 done:
	if (!NT_STATUS_IS_OK(status)) {
		TALLOC_FREE(conn->lsa_pipe_tcp);
		return status;
	}

	*cli = conn->lsa_pipe_tcp;

	return status;
}

NTSTATUS cm_connect_lsa(struct winbindd_domain *domain, TALLOC_CTX *mem_ctx,
			struct rpc_pipe_client **cli, POLICY_HND *lsa_policy)
{
	struct winbindd_cm_conn *conn;
	NTSTATUS result = NT_STATUS_UNSUCCESSFUL;
	fstring conn_pwd;
	struct dcinfo *p_dcinfo;

	result = init_dc_connection(domain);
	if (!NT_STATUS_IS_OK(result))
		return result;

	conn = &domain->conn;

	if (conn->lsa_pipe != NULL) {
		goto done;
	}

	pwd_get_cleartext(&conn->cli->pwd, conn_pwd);
	if ((conn->cli->user_name[0] == '\0') ||
	    (conn->cli->domain[0] == '\0') || 
	    (conn_pwd[0] == '\0')) {
		DEBUG(10, ("cm_connect_lsa: No no user available for "
			   "domain %s, trying schannel\n", conn->cli->domain));
		goto schannel;
	}

	/* We have an authenticated connection. Use a NTLMSSP SPNEGO
	 * authenticated LSA pipe with sign & seal. */
	result = cli_rpc_pipe_open_spnego_ntlmssp
		(conn->cli, &ndr_table_lsarpc.syntax_id, NCACN_NP,
		 PIPE_AUTH_LEVEL_PRIVACY,
		 conn->cli->domain, conn->cli->user_name, conn_pwd,
		 &conn->lsa_pipe);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(10,("cm_connect_lsa: failed to connect to LSA pipe for "
			  "domain %s using NTLMSSP authenticated pipe: user "
			  "%s\\%s. Error was %s. Trying schannel.\n",
			  domain->name, conn->cli->domain,
			  conn->cli->user_name, nt_errstr(result)));
		goto schannel;
	}

	DEBUG(10,("cm_connect_lsa: connected to LSA pipe for domain %s using "
		  "NTLMSSP authenticated pipe: user %s\\%s\n",
		  domain->name, conn->cli->domain, conn->cli->user_name ));

	result = rpccli_lsa_open_policy(conn->lsa_pipe, mem_ctx, True,
					SEC_RIGHTS_MAXIMUM_ALLOWED,
					&conn->lsa_policy);
	if (NT_STATUS_IS_OK(result)) {
		goto done;
	}

	DEBUG(10,("cm_connect_lsa: rpccli_lsa_open_policy failed, trying "
		  "schannel\n"));

	TALLOC_FREE(conn->lsa_pipe);

 schannel:

	/* Fall back to schannel if it's a W2K pre-SP1 box. */

	if (!cm_get_schannel_dcinfo(domain, &p_dcinfo)) {
		/* If this call fails - conn->cli can now be NULL ! */
		DEBUG(10, ("cm_connect_lsa: Could not get schannel auth info "
			   "for domain %s, trying anon\n", domain->name));
		goto anonymous;
	}
	result = cli_rpc_pipe_open_schannel_with_key
		(conn->cli, &ndr_table_lsarpc.syntax_id, NCACN_NP,
		 PIPE_AUTH_LEVEL_PRIVACY,
		 domain->name, p_dcinfo, &conn->lsa_pipe);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(10,("cm_connect_lsa: failed to connect to LSA pipe for "
			  "domain %s using schannel. Error was %s\n",
			  domain->name, nt_errstr(result) ));
		goto anonymous;
	}
	DEBUG(10,("cm_connect_lsa: connected to LSA pipe for domain %s using "
		  "schannel.\n", domain->name ));

	result = rpccli_lsa_open_policy(conn->lsa_pipe, mem_ctx, True,
					SEC_RIGHTS_MAXIMUM_ALLOWED,
					&conn->lsa_policy);
	if (NT_STATUS_IS_OK(result)) {
		goto done;
	}

	DEBUG(10,("cm_connect_lsa: rpccli_lsa_open_policy failed, trying "
		  "anonymous\n"));

	TALLOC_FREE(conn->lsa_pipe);

 anonymous:

	result = cli_rpc_pipe_open_noauth(conn->cli,
					  &ndr_table_lsarpc.syntax_id,
					  &conn->lsa_pipe);
	if (!NT_STATUS_IS_OK(result)) {
		result = NT_STATUS_PIPE_NOT_AVAILABLE;
		goto done;
	}

	result = rpccli_lsa_open_policy(conn->lsa_pipe, mem_ctx, True,
					SEC_RIGHTS_MAXIMUM_ALLOWED,
					&conn->lsa_policy);
 done:
	if (!NT_STATUS_IS_OK(result)) {
		invalidate_cm_connection(conn);
		return result;
	}

	*cli = conn->lsa_pipe;
	*lsa_policy = conn->lsa_policy;
	return result;
}

/****************************************************************************
 Open the netlogon pipe to this DC. Use schannel if specified in client conf.
 session key stored in conn->netlogon_pipe->dc->sess_key.
****************************************************************************/

NTSTATUS cm_connect_netlogon(struct winbindd_domain *domain,
			     struct rpc_pipe_client **cli)
{
	struct winbindd_cm_conn *conn;
	NTSTATUS result;

	uint32_t neg_flags = NETLOGON_NEG_AUTH2_ADS_FLAGS;
	uint8  mach_pwd[16];
	uint32  sec_chan_type;
	const char *account_name;
	struct rpc_pipe_client *netlogon_pipe = NULL;

	*cli = NULL;

	result = init_dc_connection(domain);
	if (!NT_STATUS_IS_OK(result)) {
		return result;
	}

	conn = &domain->conn;

	if (conn->netlogon_pipe != NULL) {
		*cli = conn->netlogon_pipe;
		return NT_STATUS_OK;
	}

	result = cli_rpc_pipe_open_noauth(conn->cli,
					  &ndr_table_netlogon.syntax_id,
					  &netlogon_pipe);
	if (!NT_STATUS_IS_OK(result)) {
		return result;
	}

	if ((!IS_DC) && (!domain->primary)) {
		/* Clear the schannel request bit and drop down */
		neg_flags &= ~NETLOGON_NEG_SCHANNEL;		
		goto no_schannel;
	}

	if (lp_client_schannel() != False) {
		neg_flags |= NETLOGON_NEG_SCHANNEL;
	}

	if (!get_trust_pw_hash(domain->name, mach_pwd, &account_name,
			       &sec_chan_type))
	{
		TALLOC_FREE(netlogon_pipe);
		return NT_STATUS_CANT_ACCESS_DOMAIN_INFO;
	}

	result = rpccli_netlogon_setup_creds(
		 netlogon_pipe,
		 domain->dcname, /* server name. */
		 domain->name,   /* domain name */
		 global_myname(), /* client name */
		 account_name,   /* machine account */
		 mach_pwd,       /* machine password */
		 sec_chan_type,  /* from get_trust_pw */
		 &neg_flags);

	if (!NT_STATUS_IS_OK(result)) {
		TALLOC_FREE(netlogon_pipe);
		return result;
	}

	if ((lp_client_schannel() == True) &&
			((neg_flags & NETLOGON_NEG_SCHANNEL) == 0)) {
		DEBUG(3, ("Server did not offer schannel\n"));
		TALLOC_FREE(netlogon_pipe);
		return NT_STATUS_ACCESS_DENIED;
	}

 no_schannel:
	if ((lp_client_schannel() == False) ||
			((neg_flags & NETLOGON_NEG_SCHANNEL) == 0)) {
		/*
		 * NetSamLogonEx only works for schannel
		 */
		domain->can_do_samlogon_ex = False;

		/* We're done - just keep the existing connection to NETLOGON
		 * open */
		conn->netlogon_pipe = netlogon_pipe;
		*cli = conn->netlogon_pipe;
		return NT_STATUS_OK;
	}

	/* Using the credentials from the first pipe, open a signed and sealed
	   second netlogon pipe. The session key is stored in the schannel
	   part of the new pipe auth struct.
	*/

	result = cli_rpc_pipe_open_schannel_with_key(
		conn->cli, &ndr_table_netlogon.syntax_id, NCACN_NP,
		PIPE_AUTH_LEVEL_PRIVACY, domain->name, netlogon_pipe->dc,
		&conn->netlogon_pipe);

	/* We can now close the initial netlogon pipe. */
	TALLOC_FREE(netlogon_pipe);

	if (!NT_STATUS_IS_OK(result)) {
		DEBUG(3, ("Could not open schannel'ed NETLOGON pipe. Error "
			  "was %s\n", nt_errstr(result)));
			  
		/* make sure we return something besides OK */
		return !NT_STATUS_IS_OK(result) ? result : NT_STATUS_PIPE_NOT_AVAILABLE;
	}

	/*
	 * Try NetSamLogonEx for AD domains
	 */
	domain->can_do_samlogon_ex = domain->active_directory;

	*cli = conn->netlogon_pipe;
	return NT_STATUS_OK;
}
