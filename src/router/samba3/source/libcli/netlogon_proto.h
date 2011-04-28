#ifndef _____LIBCLI_NETLOGON_PROTO_H__
#define _____LIBCLI_NETLOGON_PROTO_H__

#undef _PRINTF_ATTRIBUTE
#define _PRINTF_ATTRIBUTE(a1, a2) PRINTF_ATTRIBUTE(a1, a2)
/* This file was automatically generated by mkproto.pl. DO NOT EDIT */

/* this file contains prototypes for functions that are private
 * to this subsystem or library. These functions should not be
 * used outside this particular subsystem! */


/* The following definitions come from ../libcli/netlogon.c  */

NTSTATUS push_netlogon_samlogon_response(DATA_BLOB *data, TALLOC_CTX *mem_ctx,
					 struct netlogon_samlogon_response *response);
NTSTATUS pull_netlogon_samlogon_response(DATA_BLOB *data, TALLOC_CTX *mem_ctx,
					 struct netlogon_samlogon_response *response);
void map_netlogon_samlogon_response(struct netlogon_samlogon_response *response);
NTSTATUS push_nbt_netlogon_response(DATA_BLOB *data, TALLOC_CTX *mem_ctx,
				    struct nbt_netlogon_response *response);
NTSTATUS pull_nbt_netlogon_response(DATA_BLOB *data, TALLOC_CTX *mem_ctx,
					 struct nbt_netlogon_response *response);
#undef _PRINTF_ATTRIBUTE
#define _PRINTF_ATTRIBUTE(a1, a2)

#endif /* _____LIBCLI_NETLOGON_PROTO_H__ */

