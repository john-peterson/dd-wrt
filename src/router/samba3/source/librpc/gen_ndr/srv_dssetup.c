/*
 * Unix SMB/CIFS implementation.
 * server auto-generated by pidl. DO NOT MODIFY!
 */

#include "includes.h"
#ifdef AVM_VERY_SMALL
NTSTATUS rpc_dssetup_init(void) {AVM_VERY_SMALL_LOG  return NT_STATUS_OK; }
#else

#include "librpc/gen_ndr/srv_dssetup.h"

static bool api_dssetup_DsRoleGetPrimaryDomainInformation(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleGetPrimaryDomainInformation *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEGETPRIMARYDOMAININFORMATION];

	r = talloc(talloc_tos(), struct dssetup_DsRoleGetPrimaryDomainInformation);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleGetPrimaryDomainInformation, r);
	}

	ZERO_STRUCT(r->out);
	r->out.info = talloc_zero(r, union dssetup_DsRoleInfo);
	if (r->out.info == NULL) {
		talloc_free(r);
		return false;
	}

	r->out.result = _dssetup_DsRoleGetPrimaryDomainInformation(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleGetPrimaryDomainInformation, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleDnsNameToFlatName(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleDnsNameToFlatName *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEDNSNAMETOFLATNAME];

	r = talloc(talloc_tos(), struct dssetup_DsRoleDnsNameToFlatName);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleDnsNameToFlatName, r);
	}

	r->out.result = _dssetup_DsRoleDnsNameToFlatName(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleDnsNameToFlatName, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleDcAsDc(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleDcAsDc *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEDCASDC];

	r = talloc(talloc_tos(), struct dssetup_DsRoleDcAsDc);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleDcAsDc, r);
	}

	r->out.result = _dssetup_DsRoleDcAsDc(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleDcAsDc, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleDcAsReplica(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleDcAsReplica *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEDCASREPLICA];

	r = talloc(talloc_tos(), struct dssetup_DsRoleDcAsReplica);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleDcAsReplica, r);
	}

	r->out.result = _dssetup_DsRoleDcAsReplica(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleDcAsReplica, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleDemoteDc(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleDemoteDc *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEDEMOTEDC];

	r = talloc(talloc_tos(), struct dssetup_DsRoleDemoteDc);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleDemoteDc, r);
	}

	r->out.result = _dssetup_DsRoleDemoteDc(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleDemoteDc, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleGetDcOperationProgress(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleGetDcOperationProgress *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEGETDCOPERATIONPROGRESS];

	r = talloc(talloc_tos(), struct dssetup_DsRoleGetDcOperationProgress);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleGetDcOperationProgress, r);
	}

	r->out.result = _dssetup_DsRoleGetDcOperationProgress(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleGetDcOperationProgress, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleGetDcOperationResults(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleGetDcOperationResults *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEGETDCOPERATIONRESULTS];

	r = talloc(talloc_tos(), struct dssetup_DsRoleGetDcOperationResults);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleGetDcOperationResults, r);
	}

	r->out.result = _dssetup_DsRoleGetDcOperationResults(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleGetDcOperationResults, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleCancel(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleCancel *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLECANCEL];

	r = talloc(talloc_tos(), struct dssetup_DsRoleCancel);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleCancel, r);
	}

	r->out.result = _dssetup_DsRoleCancel(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleCancel, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleServerSaveStateForUpgrade(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleServerSaveStateForUpgrade *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLESERVERSAVESTATEFORUPGRADE];

	r = talloc(talloc_tos(), struct dssetup_DsRoleServerSaveStateForUpgrade);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleServerSaveStateForUpgrade, r);
	}

	r->out.result = _dssetup_DsRoleServerSaveStateForUpgrade(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleServerSaveStateForUpgrade, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleUpgradeDownlevelServer(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleUpgradeDownlevelServer *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEUPGRADEDOWNLEVELSERVER];

	r = talloc(talloc_tos(), struct dssetup_DsRoleUpgradeDownlevelServer);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleUpgradeDownlevelServer, r);
	}

	r->out.result = _dssetup_DsRoleUpgradeDownlevelServer(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleUpgradeDownlevelServer, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}

static bool api_dssetup_DsRoleAbortDownlevelServerUpgrade(pipes_struct *p)
{
	const struct ndr_interface_call *call;
	struct ndr_pull *pull;
	struct ndr_push *push;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dssetup_DsRoleAbortDownlevelServerUpgrade *r;

	call = &ndr_table_dssetup.calls[NDR_DSSETUP_DSROLEABORTDOWNLEVELSERVERUPGRADE];

	r = talloc(talloc_tos(), struct dssetup_DsRoleAbortDownlevelServerUpgrade);
	if (r == NULL) {
		return false;
	}

	if (!prs_data_blob(&p->in_data.data, &blob, r)) {
		talloc_free(r);
		return false;
	}

	pull = ndr_pull_init_blob(&blob, r);
	if (pull == NULL) {
		talloc_free(r);
		return false;
	}

	pull->flags |= LIBNDR_FLAG_REF_ALLOC;
	ndr_err = call->ndr_pull(pull, NDR_IN, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_IN_DEBUG(dssetup_DsRoleAbortDownlevelServerUpgrade, r);
	}

	r->out.result = _dssetup_DsRoleAbortDownlevelServerUpgrade(p, r);

	if (p->rng_fault_state) {
		talloc_free(r);
		/* Return true here, srv_pipe_hnd.c will take care */
		return true;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_OUT_DEBUG(dssetup_DsRoleAbortDownlevelServerUpgrade, r);
	}

	push = ndr_push_init_ctx(r);
	if (push == NULL) {
		talloc_free(r);
		return false;
	}

	ndr_err = call->ndr_push(push, NDR_OUT, r);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		talloc_free(r);
		return false;
	}

	blob = ndr_push_blob(push);
	if (!prs_copy_data_in(&p->out_data.rdata, (const char *)blob.data, (uint32_t)blob.length)) {
		talloc_free(r);
		return false;
	}

	talloc_free(r);

	return true;
}


/* Tables */
static struct api_struct api_dssetup_cmds[] = 
{
	{"DSSETUP_DSROLEGETPRIMARYDOMAININFORMATION", NDR_DSSETUP_DSROLEGETPRIMARYDOMAININFORMATION, api_dssetup_DsRoleGetPrimaryDomainInformation},
	{"DSSETUP_DSROLEDNSNAMETOFLATNAME", NDR_DSSETUP_DSROLEDNSNAMETOFLATNAME, api_dssetup_DsRoleDnsNameToFlatName},
	{"DSSETUP_DSROLEDCASDC", NDR_DSSETUP_DSROLEDCASDC, api_dssetup_DsRoleDcAsDc},
	{"DSSETUP_DSROLEDCASREPLICA", NDR_DSSETUP_DSROLEDCASREPLICA, api_dssetup_DsRoleDcAsReplica},
	{"DSSETUP_DSROLEDEMOTEDC", NDR_DSSETUP_DSROLEDEMOTEDC, api_dssetup_DsRoleDemoteDc},
	{"DSSETUP_DSROLEGETDCOPERATIONPROGRESS", NDR_DSSETUP_DSROLEGETDCOPERATIONPROGRESS, api_dssetup_DsRoleGetDcOperationProgress},
	{"DSSETUP_DSROLEGETDCOPERATIONRESULTS", NDR_DSSETUP_DSROLEGETDCOPERATIONRESULTS, api_dssetup_DsRoleGetDcOperationResults},
	{"DSSETUP_DSROLECANCEL", NDR_DSSETUP_DSROLECANCEL, api_dssetup_DsRoleCancel},
	{"DSSETUP_DSROLESERVERSAVESTATEFORUPGRADE", NDR_DSSETUP_DSROLESERVERSAVESTATEFORUPGRADE, api_dssetup_DsRoleServerSaveStateForUpgrade},
	{"DSSETUP_DSROLEUPGRADEDOWNLEVELSERVER", NDR_DSSETUP_DSROLEUPGRADEDOWNLEVELSERVER, api_dssetup_DsRoleUpgradeDownlevelServer},
	{"DSSETUP_DSROLEABORTDOWNLEVELSERVERUPGRADE", NDR_DSSETUP_DSROLEABORTDOWNLEVELSERVERUPGRADE, api_dssetup_DsRoleAbortDownlevelServerUpgrade},
};

void dssetup_get_pipe_fns(struct api_struct **fns, int *n_fns)
{
	*fns = api_dssetup_cmds;
	*n_fns = sizeof(api_dssetup_cmds) / sizeof(struct api_struct);
}

NTSTATUS rpc_dssetup_init(void)
{
	return rpc_pipe_register_commands(SMB_RPC_INTERFACE_VERSION, "dssetup", "dssetup", &ndr_table_dssetup.syntax_id, api_dssetup_cmds, sizeof(api_dssetup_cmds) / sizeof(struct api_struct));
}
#endif
