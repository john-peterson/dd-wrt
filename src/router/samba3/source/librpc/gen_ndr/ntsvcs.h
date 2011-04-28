/* header auto-generated by pidl */

#include <stdint.h>

#ifndef _HEADER_ntsvcs
#define _HEADER_ntsvcs

#define DEV_REGPROP_DESC	( 1 )
struct PNP_HwProfInfo {
	uint32_t profile_handle;
	uint16_t friendly_name[80];
	uint32_t flags;
};


struct PNP_Disconnect {
	struct {
		WERROR result;
	} out;

};


struct PNP_Connect {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetVersion {
	struct {
		uint16_t *version;/* [ref] */
		WERROR result;
	} out;

};


struct PNP_GetGlobalState {
	struct {
		WERROR result;
	} out;

};


struct PNP_InitDetection {
	struct {
		WERROR result;
	} out;

};


struct PNP_ReportLogOn {
	struct {
		WERROR result;
	} out;

};


struct PNP_ValidateDeviceInstance {
	struct {
		const char *devicepath;/* [ref,charset(UTF16)] */
		uint32_t flags;
	} in;

	struct {
		WERROR result;
	} out;

};


struct PNP_GetRootDeviceInstance {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetRelatedDeviceInstance {
	struct {
		WERROR result;
	} out;

};


struct PNP_EnumerateSubKeys {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetDeviceList {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetDeviceListSize {
	struct {
		const char *devicename;/* [unique,charset(UTF16)] */
		uint32_t flags;
	} in;

	struct {
		uint32_t *size;/* [ref] */
		WERROR result;
	} out;

};


struct PNP_GetDepth {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetDeviceRegProp {
	struct {
		const char *devicepath;/* [ref,charset(UTF16)] */
		uint32_t property;
		uint32_t unknown3;
		uint32_t *unknown1;/* [ref] */
		uint32_t *buffer_size;/* [ref] */
		uint32_t *needed;/* [ref] */
	} in;

	struct {
		uint8_t *buffer;/* [ref,length_is(*buffer_size),size_is(*buffer_size)] */
		uint32_t *unknown1;/* [ref] */
		uint32_t *buffer_size;/* [ref] */
		uint32_t *needed;/* [ref] */
		WERROR result;
	} out;

};


struct PNP_SetDeviceRegProp {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetClassInstance {
	struct {
		WERROR result;
	} out;

};


struct PNP_CreateKey {
	struct {
		WERROR result;
	} out;

};


struct PNP_DeleteRegistryKey {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetClassCount {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetClassName {
	struct {
		WERROR result;
	} out;

};


struct PNP_DeleteClassKey {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetInterfaceDeviceAlias {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetInterfaceDeviceList {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetInterfaceDeviceListSize {
	struct {
		WERROR result;
	} out;

};


struct PNP_RegisterDeviceClassAssociation {
	struct {
		WERROR result;
	} out;

};


struct PNP_UnregisterDeviceClassAssociation {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetClassRegProp {
	struct {
		WERROR result;
	} out;

};


struct PNP_SetClassRegProp {
	struct {
		WERROR result;
	} out;

};


struct PNP_CreateDevInst {
	struct {
		WERROR result;
	} out;

};


struct PNP_DeviceInstanceAction {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetDeviceStatus {
	struct {
		WERROR result;
	} out;

};


struct PNP_SetDeviceProblem {
	struct {
		WERROR result;
	} out;

};


struct PNP_DisableDevInst {
	struct {
		WERROR result;
	} out;

};


struct PNP_UninstallDevInst {
	struct {
		WERROR result;
	} out;

};


struct PNP_AddID {
	struct {
		WERROR result;
	} out;

};


struct PNP_RegisterDriver {
	struct {
		WERROR result;
	} out;

};


struct PNP_QueryRemove {
	struct {
		WERROR result;
	} out;

};


struct PNP_RequestDeviceEject {
	struct {
		WERROR result;
	} out;

};


struct PNP_IsDockStationPresent {
	struct {
		WERROR result;
	} out;

};


struct PNP_RequestEjectPC {
	struct {
		WERROR result;
	} out;

};


struct PNP_HwProfFlags {
	struct {
		uint32_t unknown1;
		const char *devicepath;/* [ref,charset(UTF16)] */
		uint32_t unknown2;
		const char *unknown5;/* [unique,charset(UTF16)] */
		uint32_t unknown6;
		uint32_t unknown7;
		uint32_t *unknown3;/* [ref] */
		uint16_t *unknown4;/* [unique] */
	} in;

	struct {
		const char **unknown5a;/* [unique,charset(UTF16)] */
		uint32_t *unknown3;/* [ref] */
		uint16_t *unknown4;/* [unique] */
		WERROR result;
	} out;

};


struct PNP_GetHwProfInfo {
	struct {
		uint32_t idx;
		uint32_t size;
		uint32_t flags;
		struct PNP_HwProfInfo *info;/* [ref] */
	} in;

	struct {
		struct PNP_HwProfInfo *info;/* [ref] */
		WERROR result;
	} out;

};


struct PNP_AddEmptyLogConf {
	struct {
		WERROR result;
	} out;

};


struct PNP_FreeLogConf {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetFirstLogConf {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetNextLogConf {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetLogConfPriority {
	struct {
		WERROR result;
	} out;

};


struct PNP_AddResDes {
	struct {
		WERROR result;
	} out;

};


struct PNP_FreeResDes {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetNextResDes {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetResDesData {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetResDesDataSize {
	struct {
		WERROR result;
	} out;

};


struct PNP_ModifyResDes {
	struct {
		WERROR result;
	} out;

};


struct PNP_DetectResourceLimit {
	struct {
		WERROR result;
	} out;

};


struct PNP_QueryResConfList {
	struct {
		WERROR result;
	} out;

};


struct PNP_SetHwProf {
	struct {
		WERROR result;
	} out;

};


struct PNP_QueryArbitratorFreeData {
	struct {
		WERROR result;
	} out;

};


struct PNP_QueryArbitratorFreeSize {
	struct {
		WERROR result;
	} out;

};


struct PNP_RunDetection {
	struct {
		WERROR result;
	} out;

};


struct PNP_RegisterNotification {
	struct {
		WERROR result;
	} out;

};


struct PNP_UnregisterNotification {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetCustomDevProp {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetVersionInternal {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetBlockedDriverInfo {
	struct {
		WERROR result;
	} out;

};


struct PNP_GetServerSideDeviceInstallFlags {
	struct {
		WERROR result;
	} out;

};

#endif /* _HEADER_ntsvcs */
