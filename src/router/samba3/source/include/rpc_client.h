/* 
   Unix SMB/CIFS implementation.
   SMB parameters and setup
   Copyright (C) Gerald (Jerry) Carter         2005.
   
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

#ifndef _RPC_CLIENT_H
#define _RPC_CLIENT_H

/* autogenerated client stubs */

#include "librpc/gen_ndr/cli_echo.h"
#include "librpc/gen_ndr/cli_lsa.h"
#include "librpc/gen_ndr/cli_svcctl.h"
#include "librpc/gen_ndr/cli_wkssvc.h"
#include "librpc/gen_ndr/cli_eventlog.h"
#include "librpc/gen_ndr/cli_dfs.h"
#include "librpc/gen_ndr/cli_initshutdown.h"
#include "librpc/gen_ndr/cli_winreg.h"
#include "librpc/gen_ndr/cli_srvsvc.h"
#include "librpc/gen_ndr/cli_samr.h"
#include "librpc/gen_ndr/cli_netlogon.h"
#include "librpc/gen_ndr/cli_dssetup.h"
#include "librpc/gen_ndr/cli_ntsvcs.h"
#include "librpc/gen_ndr/cli_epmapper.h"
#include "librpc/gen_ndr/cli_drsuapi.h"

#define prs_init_empty( _ps_, _ctx_, _io_ ) (void) prs_init((_ps_), 0, (_ctx_), (_io_))

/* macro to expand cookie-cutter code in cli_xxx() using rpc_api_pipe_req() */


#ifdef AVM_SMALL
#define CLI_DO_RPC_WERR( pcli, ctx, interface, opnum, q_in, r_out, \
                             q_ps, r_ps, q_io_fn, r_io_fn, default_error ) \
{\
	SMB_ASSERT(ndr_syntax_id_equal(&pcli->abstract_syntax, interface)); \
	if (!prs_init( &q_ps, RPC_MAX_PDU_FRAG_LEN, ctx, MARSHALL )) { \
		return WERR_NOMEM;\
	}\
	prs_init_empty( &r_ps, ctx, UNMARSHALL );\
	if ( q_io_fn("", &q_in, &q_ps, 0) ) {\
		NTSTATUS _smb_pipe_stat_ = NT_STATUS_NO_MEMORY; \
		prs_mem_free( &q_ps );\
		prs_mem_free( &r_ps );\
		return ntstatus_to_werror(_smb_pipe_stat_);\
	} else {\
		prs_mem_free( &q_ps );\
		prs_mem_free( &r_ps );\
		return default_error;\
	}\
	prs_mem_free( &q_ps );\
	prs_mem_free( &r_ps );\
}
#else
#define CLI_DO_RPC_WERR( pcli, ctx, interface, opnum, q_in, r_out, \
                             q_ps, r_ps, q_io_fn, r_io_fn, default_error ) \
{\
	SMB_ASSERT(ndr_syntax_id_equal(&pcli->abstract_syntax, interface)); \
	if (!prs_init( &q_ps, RPC_MAX_PDU_FRAG_LEN, ctx, MARSHALL )) { \
		return WERR_NOMEM;\
	}\
	prs_init_empty( &r_ps, ctx, UNMARSHALL );\
	if ( q_io_fn("", &q_in, &q_ps, 0) ) {\
		NTSTATUS _smb_pipe_stat_ = rpc_api_pipe_req(pcli, opnum, &q_ps, &r_ps); \
		if (!NT_STATUS_IS_OK(_smb_pipe_stat_)) {\
			prs_mem_free( &q_ps );\
			prs_mem_free( &r_ps );\
			return ntstatus_to_werror(_smb_pipe_stat_);\
		}\
		if (!r_io_fn("", &r_out, &r_ps, 0)) {\
			prs_mem_free( &q_ps );\
			prs_mem_free( &r_ps );\
			return default_error;\
		}\
	} else {\
		prs_mem_free( &q_ps );\
		prs_mem_free( &r_ps );\
		return default_error;\
	}\
	prs_mem_free( &q_ps );\
	prs_mem_free( &r_ps );\
}
#endif

#endif /* _RPC_CLIENT_H */
