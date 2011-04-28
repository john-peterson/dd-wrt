/* header auto-generated by pidl */

#include "librpc/ndr/libndr.h"
#include "librpc/gen_ndr/svcctl.h"

#ifndef _HEADER_NDR_svcctl
#define _HEADER_NDR_svcctl

#define NDR_SVCCTL_UUID "367abb81-9844-35f1-ad32-98f038001003"
#define NDR_SVCCTL_VERSION 2.0
#define NDR_SVCCTL_NAME "svcctl"
#define NDR_SVCCTL_HELPSTRING "Service Control"
extern const struct ndr_interface_table ndr_table_svcctl;
#define NDR_SVCCTL_CLOSESERVICEHANDLE (0x00)

#define NDR_SVCCTL_CONTROLSERVICE (0x01)

#define NDR_SVCCTL_DELETESERVICE (0x02)

#define NDR_SVCCTL_LOCKSERVICEDATABASE (0x03)

#define NDR_SVCCTL_QUERYSERVICEOBJECTSECURITY (0x04)

#define NDR_SVCCTL_SETSERVICEOBJECTSECURITY (0x05)

#define NDR_SVCCTL_QUERYSERVICESTATUS (0x06)

#define NDR_SVCCTL_SETSERVICESTATUS (0x07)

#define NDR_SVCCTL_UNLOCKSERVICEDATABASE (0x08)

#define NDR_SVCCTL_NOTIFYBOOTCONFIGSTATUS (0x09)

#define NDR_SVCCTL_SCSETSERVICEBITSW (0x0a)

#define NDR_SVCCTL_CHANGESERVICECONFIGW (0x0b)

#define NDR_SVCCTL_CREATESERVICEW (0x0c)

#define NDR_SVCCTL_ENUMDEPENDENTSERVICESW (0x0d)

#define NDR_SVCCTL_ENUMSERVICESSTATUSW (0x0e)

#define NDR_SVCCTL_OPENSCMANAGERW (0x0f)

#define NDR_SVCCTL_OPENSERVICEW (0x10)

#define NDR_SVCCTL_QUERYSERVICECONFIGW (0x11)

#define NDR_SVCCTL_QUERYSERVICELOCKSTATUSW (0x12)

#define NDR_SVCCTL_STARTSERVICEW (0x13)

#define NDR_SVCCTL_GETSERVICEDISPLAYNAMEW (0x14)

#define NDR_SVCCTL_GETSERVICEKEYNAMEW (0x15)

#define NDR_SVCCTL_SCSETSERVICEBITSA (0x16)

#define NDR_SVCCTL_CHANGESERVICECONFIGA (0x17)

#define NDR_SVCCTL_CREATESERVICEA (0x18)

#define NDR_SVCCTL_ENUMDEPENDENTSERVICESA (0x19)

#define NDR_SVCCTL_ENUMSERVICESSTATUSA (0x1a)

#define NDR_SVCCTL_OPENSCMANAGERA (0x1b)

#define NDR_SVCCTL_OPENSERVICEA (0x1c)

#define NDR_SVCCTL_QUERYSERVICECONFIGA (0x1d)

#define NDR_SVCCTL_QUERYSERVICELOCKSTATUSA (0x1e)

#define NDR_SVCCTL_STARTSERVICEA (0x1f)

#define NDR_SVCCTL_GETSERVICEDISPLAYNAMEA (0x20)

#define NDR_SVCCTL_GETSERVICEKEYNAMEA (0x21)

#define NDR_SVCCTL_GETCURRENTGROUPESTATEW (0x22)

#define NDR_SVCCTL_ENUMSERVICEGROUPW (0x23)

#define NDR_SVCCTL_CHANGESERVICECONFIG2A (0x24)

#define NDR_SVCCTL_CHANGESERVICECONFIG2W (0x25)

#define NDR_SVCCTL_QUERYSERVICECONFIG2A (0x26)

#define NDR_SVCCTL_QUERYSERVICECONFIG2W (0x27)

#define NDR_SVCCTL_QUERYSERVICESTATUSEX (0x28)

#define NDR_ENUMSERVICESSTATUSEXA (0x29)

#define NDR_ENUMSERVICESSTATUSEXW (0x2a)

#define NDR_SVCCTL_SCSENDTSMESSAGE (0x2b)

#define NDR_SVCCTL_CALL_COUNT (44)
void ndr_print_SERVICE_LOCK_STATUS(struct ndr_print *ndr, const char *name, const struct SERVICE_LOCK_STATUS *r);
void ndr_print_SERVICE_STATUS(struct ndr_print *ndr, const char *name, const struct SERVICE_STATUS *r);
void ndr_print_ENUM_SERVICE_STATUS(struct ndr_print *ndr, const char *name, const struct ENUM_SERVICE_STATUS *r);
enum ndr_err_code ndr_push_svcctl_ServerType(struct ndr_push *ndr, int ndr_flags, uint32_t r);
enum ndr_err_code ndr_pull_svcctl_ServerType(struct ndr_pull *ndr, int ndr_flags, uint32_t *r);
void ndr_print_svcctl_ServerType(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_svcctl_MgrAccessMask(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_svcctl_ServiceAccessMask(struct ndr_print *ndr, const char *name, uint32_t r);
enum ndr_err_code ndr_push_QUERY_SERVICE_CONFIG(struct ndr_push *ndr, int ndr_flags, const struct QUERY_SERVICE_CONFIG *r);
enum ndr_err_code ndr_pull_QUERY_SERVICE_CONFIG(struct ndr_pull *ndr, int ndr_flags, struct QUERY_SERVICE_CONFIG *r);
void ndr_print_QUERY_SERVICE_CONFIG(struct ndr_print *ndr, const char *name, const struct QUERY_SERVICE_CONFIG *r);
size_t ndr_size_QUERY_SERVICE_CONFIG(const struct QUERY_SERVICE_CONFIG *r, int flags);
void ndr_print_svcctl_CloseServiceHandle(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_CloseServiceHandle *r);
void ndr_print_svcctl_ControlService(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_ControlService *r);
void ndr_print_svcctl_DeleteService(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_DeleteService *r);
void ndr_print_svcctl_LockServiceDatabase(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_LockServiceDatabase *r);
void ndr_print_svcctl_QueryServiceObjectSecurity(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceObjectSecurity *r);
void ndr_print_svcctl_SetServiceObjectSecurity(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_SetServiceObjectSecurity *r);
void ndr_print_svcctl_QueryServiceStatus(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceStatus *r);
void ndr_print_svcctl_SetServiceStatus(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_SetServiceStatus *r);
void ndr_print_svcctl_UnlockServiceDatabase(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_UnlockServiceDatabase *r);
void ndr_print_svcctl_NotifyBootConfigStatus(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_NotifyBootConfigStatus *r);
void ndr_print_svcctl_SCSetServiceBitsW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_SCSetServiceBitsW *r);
void ndr_print_svcctl_ChangeServiceConfigW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_ChangeServiceConfigW *r);
void ndr_print_svcctl_CreateServiceW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_CreateServiceW *r);
void ndr_print_svcctl_EnumDependentServicesW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_EnumDependentServicesW *r);
void ndr_print_svcctl_EnumServicesStatusW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_EnumServicesStatusW *r);
void ndr_print_svcctl_OpenSCManagerW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_OpenSCManagerW *r);
void ndr_print_svcctl_OpenServiceW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_OpenServiceW *r);
void ndr_print_svcctl_QueryServiceConfigW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceConfigW *r);
void ndr_print_svcctl_QueryServiceLockStatusW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceLockStatusW *r);
void ndr_print_svcctl_StartServiceW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_StartServiceW *r);
void ndr_print_svcctl_GetServiceDisplayNameW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_GetServiceDisplayNameW *r);
void ndr_print_svcctl_GetServiceKeyNameW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_GetServiceKeyNameW *r);
void ndr_print_svcctl_SCSetServiceBitsA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_SCSetServiceBitsA *r);
void ndr_print_svcctl_ChangeServiceConfigA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_ChangeServiceConfigA *r);
void ndr_print_svcctl_CreateServiceA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_CreateServiceA *r);
void ndr_print_svcctl_EnumDependentServicesA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_EnumDependentServicesA *r);
void ndr_print_svcctl_EnumServicesStatusA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_EnumServicesStatusA *r);
void ndr_print_svcctl_OpenSCManagerA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_OpenSCManagerA *r);
void ndr_print_svcctl_OpenServiceA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_OpenServiceA *r);
void ndr_print_svcctl_QueryServiceConfigA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceConfigA *r);
void ndr_print_svcctl_QueryServiceLockStatusA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceLockStatusA *r);
void ndr_print_svcctl_StartServiceA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_StartServiceA *r);
void ndr_print_svcctl_GetServiceDisplayNameA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_GetServiceDisplayNameA *r);
void ndr_print_svcctl_GetServiceKeyNameA(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_GetServiceKeyNameA *r);
void ndr_print_svcctl_GetCurrentGroupeStateW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_GetCurrentGroupeStateW *r);
void ndr_print_svcctl_EnumServiceGroupW(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_EnumServiceGroupW *r);
void ndr_print_svcctl_ChangeServiceConfig2A(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_ChangeServiceConfig2A *r);
void ndr_print_svcctl_ChangeServiceConfig2W(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_ChangeServiceConfig2W *r);
void ndr_print_svcctl_QueryServiceConfig2A(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceConfig2A *r);
void ndr_print_svcctl_QueryServiceConfig2W(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceConfig2W *r);
void ndr_print_svcctl_QueryServiceStatusEx(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_QueryServiceStatusEx *r);
void ndr_print_EnumServicesStatusExA(struct ndr_print *ndr, const char *name, int flags, const struct EnumServicesStatusExA *r);
void ndr_print_EnumServicesStatusExW(struct ndr_print *ndr, const char *name, int flags, const struct EnumServicesStatusExW *r);
void ndr_print_svcctl_SCSendTSMessage(struct ndr_print *ndr, const char *name, int flags, const struct svcctl_SCSendTSMessage *r);
#endif /* _HEADER_NDR_svcctl */
