#include "librpc/gen_ndr/ndr_wkssvc.h"
#ifndef __SRV_WKSSVC__
#define __SRV_WKSSVC__
WERROR _wkssvc_NetWkstaGetInfo(pipes_struct *p, struct wkssvc_NetWkstaGetInfo *r);
WERROR _wkssvc_NetWkstaSetInfo(pipes_struct *p, struct wkssvc_NetWkstaSetInfo *r);
WERROR _wkssvc_NetWkstaEnumUsers(pipes_struct *p, struct wkssvc_NetWkstaEnumUsers *r);
WERROR _wkssvc_NetrWkstaUserGetInfo(pipes_struct *p, struct wkssvc_NetrWkstaUserGetInfo *r);
WERROR _wkssvc_NetrWkstaUserSetInfo(pipes_struct *p, struct wkssvc_NetrWkstaUserSetInfo *r);
WERROR _wkssvc_NetWkstaTransportEnum(pipes_struct *p, struct wkssvc_NetWkstaTransportEnum *r);
WERROR _wkssvc_NetrWkstaTransportAdd(pipes_struct *p, struct wkssvc_NetrWkstaTransportAdd *r);
WERROR _wkssvc_NetrWkstaTransportDel(pipes_struct *p, struct wkssvc_NetrWkstaTransportDel *r);
WERROR _wkssvc_NetrUseAdd(pipes_struct *p, struct wkssvc_NetrUseAdd *r);
WERROR _wkssvc_NetrUseGetInfo(pipes_struct *p, struct wkssvc_NetrUseGetInfo *r);
WERROR _wkssvc_NetrUseDel(pipes_struct *p, struct wkssvc_NetrUseDel *r);
WERROR _wkssvc_NetrUseEnum(pipes_struct *p, struct wkssvc_NetrUseEnum *r);
WERROR _wkssvc_NetrMessageBufferSend(pipes_struct *p, struct wkssvc_NetrMessageBufferSend *r);
WERROR _wkssvc_NetrWorkstationStatisticsGet(pipes_struct *p, struct wkssvc_NetrWorkstationStatisticsGet *r);
WERROR _wkssvc_NetrLogonDomainNameAdd(pipes_struct *p, struct wkssvc_NetrLogonDomainNameAdd *r);
WERROR _wkssvc_NetrLogonDomainNameDel(pipes_struct *p, struct wkssvc_NetrLogonDomainNameDel *r);
WERROR _wkssvc_NetrJoinDomain(pipes_struct *p, struct wkssvc_NetrJoinDomain *r);
WERROR _wkssvc_NetrUnjoinDomain(pipes_struct *p, struct wkssvc_NetrUnjoinDomain *r);
WERROR _wkssvc_NetrRenameMachineInDomain(pipes_struct *p, struct wkssvc_NetrRenameMachineInDomain *r);
WERROR _wkssvc_NetrValidateName(pipes_struct *p, struct wkssvc_NetrValidateName *r);
WERROR _wkssvc_NetrGetJoinInformation(pipes_struct *p, struct wkssvc_NetrGetJoinInformation *r);
WERROR _wkssvc_NetrGetJoinableOus(pipes_struct *p, struct wkssvc_NetrGetJoinableOus *r);
WERROR _wkssvc_NetrJoinDomain2(pipes_struct *p, struct wkssvc_NetrJoinDomain2 *r);
WERROR _wkssvc_NetrUnjoinDomain2(pipes_struct *p, struct wkssvc_NetrUnjoinDomain2 *r);
WERROR _wkssvc_NetrRenameMachineInDomain2(pipes_struct *p, struct wkssvc_NetrRenameMachineInDomain2 *r);
WERROR _wkssvc_NetrValidateName2(pipes_struct *p, struct wkssvc_NetrValidateName2 *r);
WERROR _wkssvc_NetrGetJoinableOus2(pipes_struct *p, struct wkssvc_NetrGetJoinableOus2 *r);
WERROR _wkssvc_NetrAddAlternateComputerName(pipes_struct *p, struct wkssvc_NetrAddAlternateComputerName *r);
WERROR _wkssvc_NetrRemoveAlternateComputerName(pipes_struct *p, struct wkssvc_NetrRemoveAlternateComputerName *r);
WERROR _wkssvc_NetrSetPrimaryComputername(pipes_struct *p, struct wkssvc_NetrSetPrimaryComputername *r);
WERROR _wkssvc_NetrEnumerateComputerNames(pipes_struct *p, struct wkssvc_NetrEnumerateComputerNames *r);
void wkssvc_get_pipe_fns(struct api_struct **fns, int *n_fns);
NTSTATUS rpc_wkssvc_init(void);
#endif /* __SRV_WKSSVC__ */
