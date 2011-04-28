/* 
   Unix SMB/CIFS implementation.

   Winbind daemon for ntdom nss module

   Copyright (C) Tim Potter 2000
   Copyright (C) Jim McDonough <jmcd@us.ibm.com> 2003
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _WINBINDD_H
#define _WINBINDD_H

#include "nsswitch/winbind_struct_protocol.h"
#include "nsswitch/libwbclient/wbclient.h"

#ifdef HAVE_LIBNSCD
#include <libnscd.h>
#endif

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#undef DBGC_CLASS
#define DBGC_CLASS DBGC_WINBIND

#define WB_REPLACE_CHAR		'_'

struct winbindd_fd_event {
	struct winbindd_fd_event *next, *prev;
	int fd;
	int flags; /* see EVENT_FD_* flags */
	void (*handler)(struct winbindd_fd_event *fde, int flags);
	void *data;
	size_t length, done;
	void (*finished)(void *private_data, bool success);
	void *private_data;
};

struct sid_ctr {
	DOM_SID *sid;
	bool finished;
	const char *domain;
	const char *name;
	enum lsa_SidType type;
};

struct winbindd_cli_state {
	struct winbindd_cli_state *prev, *next;   /* Linked list pointers */
	int sock;                                 /* Open socket from client */
	struct winbindd_fd_event fd_event;
	pid_t pid;                                /* pid of client */
	bool finished;                            /* Can delete from list */
	bool write_extra_data;                    /* Write extra_data field */
	time_t last_access;                       /* Time of last access (read or write) */
	bool privileged;                           /* Is the client 'privileged' */

	TALLOC_CTX *mem_ctx;			  /* memory per request */
	struct winbindd_request request;          /* Request from client */
	struct winbindd_response response;        /* Respose to client */
	bool getpwent_initialized;                /* Has getpwent_state been
						   * initialized? */
	bool getgrent_initialized;                /* Has getgrent_state been
						   * initialized? */
	struct getent_state *getpwent_state;      /* State for getpwent() */
	struct getent_state *getgrent_state;      /* State for getgrent() */
};

/* State between get{pw,gr}ent() calls */

struct getent_state {
	struct getent_state *prev, *next;
	void *sam_entries;
	uint32 sam_entry_index, num_sam_entries;
	bool got_sam_entries;
	fstring domain_name;
};

/* Storage for cached getpwent() user entries */

struct getpwent_user {
	fstring name;                        /* Account name */
	fstring gecos;                       /* User information */
	fstring homedir;                     /* User Home Directory */
	fstring shell;                       /* User Login Shell */
	DOM_SID user_sid;                    /* NT user and primary group SIDs */
	DOM_SID group_sid;
};

/* Server state structure */

typedef struct {
	char *acct_name;
	char *full_name;
	char *homedir;
	char *shell;
	gid_t primary_gid;                   /* allow the nss_info
						backend to set the primary group */
	DOM_SID user_sid;                    /* NT user and primary group SIDs */
	DOM_SID group_sid;
} WINBIND_USERINFO;

/* Our connection to the DC */

struct winbindd_cm_conn {
	struct cli_state *cli;

	struct rpc_pipe_client *samr_pipe;
	POLICY_HND sam_connect_handle, sam_domain_handle;

	struct rpc_pipe_client *lsa_pipe;
	struct rpc_pipe_client *lsa_pipe_tcp;
	POLICY_HND lsa_policy;

	struct rpc_pipe_client *netlogon_pipe;
};

struct winbindd_async_request;

/* Async child */

struct winbindd_domain;

struct winbindd_child_dispatch_table {
	const char *name;
	enum winbindd_cmd struct_cmd;
	enum winbindd_result (*struct_fn)(struct winbindd_domain *domain,
					  struct winbindd_cli_state *state);
};

struct winbindd_child {
	struct winbindd_child *next, *prev;

	pid_t pid;
	struct winbindd_domain *domain;
	char *logfilename;

	struct winbindd_fd_event event;
	struct timed_event *lockout_policy_event;
	struct timed_event *machine_password_change_event;
	struct winbindd_async_request *requests;

	const struct winbindd_child_dispatch_table *table;
};

/* Structures to hold per domain information */

struct winbindd_domain {
	fstring name;                          /* Domain name (NetBIOS) */
	fstring alt_name;                      /* alt Domain name, if any (FQDN for ADS) */
	fstring forest_name;                   /* Name of the AD forest we're in */
	DOM_SID sid;                           /* SID for this domain */
	uint32 domain_flags;                   /* Domain flags from netlogon.h */
	uint32 domain_type;                    /* Domain type from netlogon.h */
	uint32 domain_trust_attribs;           /* Trust attribs from netlogon.h */
	bool initialized;		       /* Did we already ask for the domain mode? */
	bool native_mode;                      /* is this a win2k domain in native mode ? */
	bool active_directory;                 /* is this a win2k active directory ? */
	bool primary;                          /* is this our primary domain ? */
	bool internal;                         /* BUILTIN and member SAM */
	bool online;			       /* is this domain available ? */
	time_t startup_time;		       /* When we set "startup" true. */
	bool startup;                          /* are we in the first 30 seconds after startup_time ? */

	bool can_do_samlogon_ex; /* Due to the lack of finer control what type
				  * of DC we have, let us try to do a
				  * credential-chain less samlogon_ex call
				  * with AD and schannel. If this fails with
				  * DCERPC_FAULT_OP_RNG_ERROR, then set this
				  * to False. This variable is around so that
				  * we don't have to try _ex every time. */

	bool can_do_ncacn_ip_tcp;

	/* Lookup methods for this domain (LDAP or RPC) */
	struct winbindd_methods *methods;

	/* the backend methods are used by the cache layer to find the right
	   backend */
	struct winbindd_methods *backend;

        /* Private data for the backends (used for connection cache) */

	void *private_data;

	/*
	 * idmap config settings, used to tell the idmap child which
	 * special domain config to use for a mapping
	 */
	bool have_idmap_config;
	uint32_t id_range_low, id_range_high;

	/* A working DC */
	pid_t dc_probe_pid; /* Child we're using to detect the DC. */
	fstring dcname;
	struct sockaddr_storage dcaddr;

	/* Sequence number stuff */

	time_t last_seq_check;
	uint32 sequence_number;
	NTSTATUS last_status;

	/* The smb connection */

	struct winbindd_cm_conn conn;

	/* The child pid we're talking to */

	struct winbindd_child child;

	/* Callback we use to try put us back online. */

	uint32 check_online_timeout;
	struct timed_event *check_online_event;

	/* Linked list info */

	struct winbindd_domain *prev, *next;
};

/* per-domain methods. This is how LDAP vs RPC is selected
 */
struct winbindd_methods {
	/* does this backend provide a consistent view of the data? (ie. is the primary group
	   always correct) */
	bool consistent;

	/* get a list of users, returning a WINBIND_USERINFO for each one */
	NTSTATUS (*query_user_list)(struct winbindd_domain *domain,
				   TALLOC_CTX *mem_ctx,
				   uint32 *num_entries, 
				   WINBIND_USERINFO **info);

	/* get a list of domain groups */
	NTSTATUS (*enum_dom_groups)(struct winbindd_domain *domain,
				    TALLOC_CTX *mem_ctx,
				    uint32 *num_entries, 
				    struct acct_info **info);

	/* get a list of domain local groups */
	NTSTATUS (*enum_local_groups)(struct winbindd_domain *domain,
				    TALLOC_CTX *mem_ctx,
				    uint32 *num_entries, 
				    struct acct_info **info);
				    
	/* convert one user or group name to a sid */
	NTSTATUS (*name_to_sid)(struct winbindd_domain *domain,
				TALLOC_CTX *mem_ctx,
				enum winbindd_cmd orig_cmd,
				const char *domain_name,
				const char *name,
				DOM_SID *sid,
				enum lsa_SidType *type);

	/* convert a sid to a user or group name */
	NTSTATUS (*sid_to_name)(struct winbindd_domain *domain,
				TALLOC_CTX *mem_ctx,
				const DOM_SID *sid,
				char **domain_name,
				char **name,
				enum lsa_SidType *type);

	NTSTATUS (*rids_to_names)(struct winbindd_domain *domain,
				  TALLOC_CTX *mem_ctx,
				  const DOM_SID *domain_sid,
				  uint32 *rids,
				  size_t num_rids,
				  char **domain_name,
				  char ***names,
				  enum lsa_SidType **types);

	/* lookup user info for a given SID */
	NTSTATUS (*query_user)(struct winbindd_domain *domain, 
			       TALLOC_CTX *mem_ctx, 
			       const DOM_SID *user_sid,
			       WINBIND_USERINFO *user_info);

	/* lookup all groups that a user is a member of. The backend
	   can also choose to lookup by username or rid for this
	   function */
	NTSTATUS (*lookup_usergroups)(struct winbindd_domain *domain,
				      TALLOC_CTX *mem_ctx,
				      const DOM_SID *user_sid,
				      uint32 *num_groups, DOM_SID **user_gids);

	/* Lookup all aliases that the sids delivered are member of. This is
	 * to implement 'domain local groups' correctly */
	NTSTATUS (*lookup_useraliases)(struct winbindd_domain *domain,
				       TALLOC_CTX *mem_ctx,
				       uint32 num_sids,
				       const DOM_SID *sids,
				       uint32 *num_aliases,
				       uint32 **alias_rids);

	/* find all members of the group with the specified group_rid */
	NTSTATUS (*lookup_groupmem)(struct winbindd_domain *domain,
				    TALLOC_CTX *mem_ctx,
				    const DOM_SID *group_sid,
				    uint32 *num_names, 
				    DOM_SID **sid_mem, char ***names, 
				    uint32 **name_types);

	/* return the current global sequence number */
	NTSTATUS (*sequence_number)(struct winbindd_domain *domain, uint32 *seq);

	/* return the lockout policy */
	NTSTATUS (*lockout_policy)(struct winbindd_domain *domain,
 				   TALLOC_CTX *mem_ctx,
				   struct samr_DomInfo12 *lockout_policy);

	/* return the lockout policy */
	NTSTATUS (*password_policy)(struct winbindd_domain *domain,
				    TALLOC_CTX *mem_ctx,
				    struct samr_DomInfo1 *password_policy);

	/* enumerate trusted domains */
	NTSTATUS (*trusted_domains)(struct winbindd_domain *domain,
				    TALLOC_CTX *mem_ctx,
				    uint32 *num_domains,
				    char ***names,
				    char ***alt_names,
				    DOM_SID **dom_sids);
};

/* Used to glue a policy handle and cli_state together */

typedef struct {
	struct cli_state *cli;
	POLICY_HND pol;
} CLI_POLICY_HND;

/* Filled out by IDMAP backends */
struct winbindd_idmap_methods {
  /* Called when backend is first loaded */
  bool (*init)(void);

  bool (*get_sid_from_uid)(uid_t uid, DOM_SID *sid);
  bool (*get_sid_from_gid)(gid_t gid, DOM_SID *sid);

  bool (*get_uid_from_sid)(DOM_SID *sid, uid_t *uid);
  bool (*get_gid_from_sid)(DOM_SID *sid, gid_t *gid);

  /* Called when backend is unloaded */
  bool (*close)(void);
  /* Called to dump backend status */
  void (*status)(void);
};

/* Data structures for dealing with the trusted domain cache */

struct winbindd_tdc_domain {
	const char *domain_name;
	const char *dns_name;
        DOM_SID sid;
	uint32 trust_flags;
	uint32 trust_attribs;
	uint32 trust_type;
};

/* Switch for listing users or groups */
enum ent_type {
	LIST_USERS = 0,
	LIST_GROUPS,
};

struct WINBINDD_MEMORY_CREDS {
	struct WINBINDD_MEMORY_CREDS *next, *prev;
	const char *username; /* lookup key. */
	uid_t uid;
	int ref_count;
	size_t len;
	uint8_t *nt_hash; /* Base pointer for the following 2 */
	uint8_t *lm_hash;
	char *pass;
};

struct WINBINDD_CCACHE_ENTRY {
	struct WINBINDD_CCACHE_ENTRY *next, *prev;
	const char *principal_name;
	const char *ccname;
	const char *service;
	const char *username;
	const char *realm;
	struct WINBINDD_MEMORY_CREDS *cred_ptr;
	int ref_count;
	uid_t uid;
	time_t create_time;
	time_t renew_until;
	time_t refresh_time;
	struct timed_event *event;
};

#include "winbindd/winbindd_proto.h"

#define WINBINDD_ESTABLISH_LOOP 30
#define WINBINDD_RESCAN_FREQ lp_winbind_cache_time()
#define WINBINDD_PAM_AUTH_KRB5_RENEW_TIME 2592000 /* one month */
#define DOM_SEQUENCE_NONE ((uint32)-1)

#define IS_DOMAIN_OFFLINE(x) ( lp_winbind_offline_logon() && \
			       ( get_global_winbindd_state_offline() \
				 || !(x)->online ) )
#define IS_DOMAIN_ONLINE(x) (!IS_DOMAIN_OFFLINE(x))

#endif /* _WINBINDD_H */
