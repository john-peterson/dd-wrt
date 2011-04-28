/* header auto-generated by pidl */

#include "librpc/ndr/libndr.h"
#include "librpc/gen_ndr/misc.h"

#ifndef _HEADER_NDR_misc
#define _HEADER_NDR_misc

#define NDR_MISC_CALL_COUNT (0)
enum ndr_err_code ndr_push_GUID(struct ndr_push *ndr, int ndr_flags, const struct GUID *r);
enum ndr_err_code ndr_pull_GUID(struct ndr_pull *ndr, int ndr_flags, struct GUID *r);
void ndr_print_GUID(struct ndr_print *ndr, const char *name, const struct GUID *r);
size_t ndr_size_GUID(const struct GUID *r, int flags);
enum ndr_err_code ndr_push_ndr_syntax_id(struct ndr_push *ndr, int ndr_flags, const struct ndr_syntax_id *r);
enum ndr_err_code ndr_pull_ndr_syntax_id(struct ndr_pull *ndr, int ndr_flags, struct ndr_syntax_id *r);
void ndr_print_ndr_syntax_id(struct ndr_print *ndr, const char *name, const struct ndr_syntax_id *r);
enum ndr_err_code ndr_push_policy_handle(struct ndr_push *ndr, int ndr_flags, const struct policy_handle *r);
enum ndr_err_code ndr_pull_policy_handle(struct ndr_pull *ndr, int ndr_flags, struct policy_handle *r);
void ndr_print_policy_handle(struct ndr_print *ndr, const char *name, const struct policy_handle *r);
enum ndr_err_code ndr_push_netr_SchannelType(struct ndr_push *ndr, int ndr_flags, enum netr_SchannelType r);
enum ndr_err_code ndr_pull_netr_SchannelType(struct ndr_pull *ndr, int ndr_flags, enum netr_SchannelType *r);
void ndr_print_netr_SchannelType(struct ndr_print *ndr, const char *name, enum netr_SchannelType r);
enum ndr_err_code ndr_push_netr_SamDatabaseID(struct ndr_push *ndr, int ndr_flags, enum netr_SamDatabaseID r);
enum ndr_err_code ndr_pull_netr_SamDatabaseID(struct ndr_pull *ndr, int ndr_flags, enum netr_SamDatabaseID *r);
void ndr_print_netr_SamDatabaseID(struct ndr_print *ndr, const char *name, enum netr_SamDatabaseID r);
enum ndr_err_code ndr_push_samr_RejectReason(struct ndr_push *ndr, int ndr_flags, enum samr_RejectReason r);
enum ndr_err_code ndr_pull_samr_RejectReason(struct ndr_pull *ndr, int ndr_flags, enum samr_RejectReason *r);
void ndr_print_samr_RejectReason(struct ndr_print *ndr, const char *name, enum samr_RejectReason r);
#endif /* _HEADER_NDR_misc */
