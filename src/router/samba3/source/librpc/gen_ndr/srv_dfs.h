#include "librpc/gen_ndr/ndr_dfs.h"
#ifndef __SRV_NETDFS__
#define __SRV_NETDFS__
void _dfs_GetManagerVersion(pipes_struct *p, struct dfs_GetManagerVersion *r);
WERROR _dfs_Add(pipes_struct *p, struct dfs_Add *r);
WERROR _dfs_Remove(pipes_struct *p, struct dfs_Remove *r);
WERROR _dfs_SetInfo(pipes_struct *p, struct dfs_SetInfo *r);
WERROR _dfs_GetInfo(pipes_struct *p, struct dfs_GetInfo *r);
WERROR _dfs_Enum(pipes_struct *p, struct dfs_Enum *r);
WERROR _dfs_Rename(pipes_struct *p, struct dfs_Rename *r);
WERROR _dfs_Move(pipes_struct *p, struct dfs_Move *r);
WERROR _dfs_ManagerGetConfigInfo(pipes_struct *p, struct dfs_ManagerGetConfigInfo *r);
WERROR _dfs_ManagerSendSiteInfo(pipes_struct *p, struct dfs_ManagerSendSiteInfo *r);
WERROR _dfs_AddFtRoot(pipes_struct *p, struct dfs_AddFtRoot *r);
WERROR _dfs_RemoveFtRoot(pipes_struct *p, struct dfs_RemoveFtRoot *r);
WERROR _dfs_AddStdRoot(pipes_struct *p, struct dfs_AddStdRoot *r);
WERROR _dfs_RemoveStdRoot(pipes_struct *p, struct dfs_RemoveStdRoot *r);
WERROR _dfs_ManagerInitialize(pipes_struct *p, struct dfs_ManagerInitialize *r);
WERROR _dfs_AddStdRootForced(pipes_struct *p, struct dfs_AddStdRootForced *r);
WERROR _dfs_GetDcAddress(pipes_struct *p, struct dfs_GetDcAddress *r);
WERROR _dfs_SetDcAddress(pipes_struct *p, struct dfs_SetDcAddress *r);
WERROR _dfs_FlushFtTable(pipes_struct *p, struct dfs_FlushFtTable *r);
WERROR _dfs_Add2(pipes_struct *p, struct dfs_Add2 *r);
WERROR _dfs_Remove2(pipes_struct *p, struct dfs_Remove2 *r);
WERROR _dfs_EnumEx(pipes_struct *p, struct dfs_EnumEx *r);
WERROR _dfs_SetInfo2(pipes_struct *p, struct dfs_SetInfo2 *r);
void netdfs_get_pipe_fns(struct api_struct **fns, int *n_fns);
NTSTATUS rpc_netdfs_init(void);
#endif /* __SRV_NETDFS__ */
