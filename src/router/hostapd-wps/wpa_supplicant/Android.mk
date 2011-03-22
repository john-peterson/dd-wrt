#
# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

LOCAL_PATH := $(call my-dir)

WPA_BUILD_SUPPLICANT := false
ifneq ($(TARGET_SIMULATOR),true)
  ifneq ($(BOARD_WPA_SUPPLICANT_DRIVER),)
    WPA_BUILD_SUPPLICANT := true
    CONFIG_DRIVER_$(BOARD_WPA_SUPPLICANT_DRIVER) := y
  endif
endif

include $(LOCAL_PATH)/.config

# To ignore possible wrong network configurations
L_CFLAGS = -DWPA_IGNORE_CONFIG_ERRORS

# Use Android specific directory for control interface sockets
L_CFLAGS += -DCONFIG_CTRL_IFACE_CLIENT_DIR=\"/data/misc/wifi/sockets\"
L_CFLAGS += -DCONFIG_CTRL_IFACE_DIR=\"/data/system/wpa_supplicant\"

# To force sizeof(enum) = 4
ifeq ($(TARGET_ARCH),arm)
L_CFLAGS += -mabi=aapcs-linux
endif

# To allow non-ASCII characters in SSID
L_CFLAGS += -DWPA_UNICODE_SSID

# OpenSSL is configured without engines on Android
L_CFLAGS += -DOPENSSL_NO_ENGINE

INCLUDES = $(LOCAL_PATH)
INCLUDES += $(LOCAL_PATH)/src
INCLUDES += $(LOCAL_PATH)/src/common
# INCLUDES += $(LOCAL_PATH)/src/crypto # To force proper includes
INCLUDES += $(LOCAL_PATH)/src/drivers
INCLUDES += $(LOCAL_PATH)/src/eap_common
INCLUDES += $(LOCAL_PATH)/src/eapol_supp
INCLUDES += $(LOCAL_PATH)/src/eap_peer
INCLUDES += $(LOCAL_PATH)/src/eap_server
INCLUDES += $(LOCAL_PATH)/src/hlr_auc_gw
INCLUDES += $(LOCAL_PATH)/src/l2_packet
INCLUDES += $(LOCAL_PATH)/src/radius
INCLUDES += $(LOCAL_PATH)/src/rsn_supp
INCLUDES += $(LOCAL_PATH)/src/tls
INCLUDES += $(LOCAL_PATH)/src/utils
INCLUDES += $(LOCAL_PATH)/src/wps
INCLUDES += external/openssl/include
INCLUDES += frameworks/base/cmds/keystore
ifdef CONFIG_DRIVER_NL80211
INCLUDES += external/libnl_2/include
endif

OBJS = config.c
OBJS += notify.c
OBJS += bss.c
OBJS += eap_register.c
OBJS += src/utils/common.c
OBJS += src/utils/wpa_debug.c
OBJS += src/utils/wpabuf.c
OBJS_p = wpa_passphrase.c
OBJS_p += src/utils/common.c
OBJS_p += src/utils/wpa_debug.c
OBJS_p += src/utils/wpabuf.c
OBJS_c = wpa_cli.c src/common/wpa_ctrl.c
OBJS_c += src/utils/wpa_debug.c
OBJS_d =
OBJS_priv =

ifndef CONFIG_OS
ifdef CONFIG_NATIVE_WINDOWS
CONFIG_OS=win32
else
CONFIG_OS=unix
endif
endif

ifeq ($(CONFIG_OS), internal)
L_CFLAGS += -DOS_NO_C_LIB_DEFINES
endif

OBJS += src/utils/os_$(CONFIG_OS).c
OBJS_p += src/utils/os_$(CONFIG_OS).c
OBJS_c += src/utils/os_$(CONFIG_OS).c

ifdef CONFIG_WPA_TRACE
L_CFLAGS += -DWPA_TRACE
OBJS += src/utils/trace.c
OBJS_p += src/utils/trace.c
OBJS_c += src/utils/trace.c
LDFLAGS += -rdynamic
L_CFLAGS += -funwind-tables
ifdef CONFIG_WPA_TRACE_BFD
L_CFLAGS += -DWPA_TRACE_BFD
LIBS += -lbfd
LIBS_p += -lbfd
LIBS_c += -lbfd
endif
endif

ifndef CONFIG_ELOOP
CONFIG_ELOOP=eloop
endif
OBJS += src/utils/$(CONFIG_ELOOP).c
OBJS_c += src/utils/$(CONFIG_ELOOP).c


ifdef CONFIG_EAPOL_TEST
L_CFLAGS += -Werror -DEAPOL_TEST
endif

ifndef CONFIG_BACKEND
CONFIG_BACKEND=file
endif

ifeq ($(CONFIG_BACKEND), file)
OBJS += config_file.c
ifndef CONFIG_NO_CONFIG_BLOBS
NEED_BASE64=y
endif
L_CFLAGS += -DCONFIG_BACKEND_FILE
endif

ifeq ($(CONFIG_BACKEND), winreg)
OBJS += config_winreg.c
endif

ifeq ($(CONFIG_BACKEND), none)
OBJS += config_none.c
endif

ifdef CONFIG_NO_CONFIG_WRITE
L_CFLAGS += -DCONFIG_NO_CONFIG_WRITE
endif

ifdef CONFIG_NO_CONFIG_BLOBS
L_CFLAGS += -DCONFIG_NO_CONFIG_BLOBS
endif

ifdef CONFIG_NO_SCAN_PROCESSING
L_CFLAGS += -DCONFIG_NO_SCAN_PROCESSING
endif

ifdef CONFIG_IEEE80211W
L_CFLAGS += -DCONFIG_IEEE80211W
NEED_SHA256=y
NEED_AES_OMAC1=y
endif

ifdef CONFIG_IEEE80211R
L_CFLAGS += -DCONFIG_IEEE80211R
OBJS += src/rsn_supp/wpa_ft.c
NEED_80211_COMMON=y
NEED_SHA256=y
NEED_AES_OMAC1=y
endif

ifdef CONFIG_PEERKEY
L_CFLAGS += -DCONFIG_PEERKEY
endif

ifndef CONFIG_NO_WPA
OBJS += src/rsn_supp/wpa.c
OBJS += src/rsn_supp/preauth.c
OBJS += src/rsn_supp/pmksa_cache.c
OBJS += src/rsn_supp/peerkey.c
OBJS += src/rsn_supp/wpa_ie.c
OBJS += src/common/wpa_common.c
NEED_AES=y
NEED_SHA1=y
NEED_MD5=y
NEED_RC4=y
else
L_CFLAGS += -DCONFIG_NO_WPA -DCONFIG_NO_WPA2
endif

ifdef CONFIG_IBSS_RSN
NEED_RSN_AUTHENTICATOR=y
L_CFLAGS += -DCONFIG_IBSS_RSN
OBJS += ibss_rsn.c
endif

ifdef CONFIG_P2P
OBJS += p2p_supplicant.c
OBJS += src/p2p/p2p.c
OBJS += src/p2p/p2p_utils.c
OBJS += src/p2p/p2p_parse.c
OBJS += src/p2p/p2p_build.c
OBJS += src/p2p/p2p_go_neg.c
OBJS += src/p2p/p2p_sd.c
OBJS += src/p2p/p2p_pd.c
OBJS += src/p2p/p2p_invitation.c
OBJS += src/p2p/p2p_dev_disc.c
OBJS += src/p2p/p2p_group.c
OBJS += src/ap/p2p_hostapd.c
L_CFLAGS += -DCONFIG_P2P
NEED_80211_COMMON=y
ifdef CONFIG_P2P_STRICT
L_CFLAGS += -DCONFIG_P2P_STRICT
endif
endif

ifdef CONFIG_NO_WPA2
L_CFLAGS += -DCONFIG_NO_WPA2
endif

include $(LOCAL_PATH)/src/drivers/drivers.mk

ifdef CONFIG_AP
OBJS_d += $(DRV_BOTH_OBJS)
L_CFLAGS += $(DRV_BOTH_CFLAGS)
LDFLAGS += $(DRV_BOTH_LDFLAGS)
LIBS += $(DRV_BOTH_LIBS)
else
NEED_AP_MLME=
OBJS_d += $(DRV_WPA_OBJS)
L_CFLAGS += $(DRV_WPA_CFLAGS)
LDFLAGS += $(DRV_WPA_LDFLAGS)
LIBS += $(DRV_WPA_LIBS)
endif

ifndef CONFIG_L2_PACKET
CONFIG_L2_PACKET=linux
endif

OBJS_l2 += src/l2_packet/l2_packet_$(CONFIG_L2_PACKET).c

ifeq ($(CONFIG_L2_PACKET), pcap)
ifdef CONFIG_WINPCAP
L_CFLAGS += -DCONFIG_WINPCAP
LIBS += -lwpcap -lpacket
LIBS_w += -lwpcap
else
LIBS += -ldnet -lpcap
endif
endif

ifeq ($(CONFIG_L2_PACKET), winpcap)
LIBS += -lwpcap -lpacket
LIBS_w += -lwpcap
endif

ifeq ($(CONFIG_L2_PACKET), freebsd)
LIBS += -lpcap
endif

ifdef CONFIG_EAP_TLS
# EAP-TLS
ifeq ($(CONFIG_EAP_TLS), dyn)
L_CFLAGS += -DEAP_TLS_DYNAMIC
EAPDYN += src/eap_peer/eap_tls.so
else
L_CFLAGS += -DEAP_TLS
OBJS += src/eap_peer/eap_tls.c
OBJS_h += src/eap_server/eap_server_tls.c
endif
TLS_FUNCS=y
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_PEAP
# EAP-PEAP
ifeq ($(CONFIG_EAP_PEAP), dyn)
L_CFLAGS += -DEAP_PEAP_DYNAMIC
EAPDYN += src/eap_peer/eap_peap.so
else
L_CFLAGS += -DEAP_PEAP
OBJS += src/eap_peer/eap_peap.c
OBJS += src/eap_common/eap_peap_common.c
OBJS_h += src/eap_server/eap_server_peap.c
endif
TLS_FUNCS=y
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_TTLS
# EAP-TTLS
ifeq ($(CONFIG_EAP_TTLS), dyn)
L_CFLAGS += -DEAP_TTLS_DYNAMIC
EAPDYN += src/eap_peer/eap_ttls.so
else
L_CFLAGS += -DEAP_TTLS
OBJS += src/eap_peer/eap_ttls.c
OBJS_h += src/eap_server/eap_server_ttls.c
endif
MS_FUNCS=y
TLS_FUNCS=y
CHAP=y
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_MD5
# EAP-MD5
ifeq ($(CONFIG_EAP_MD5), dyn)
L_CFLAGS += -DEAP_MD5_DYNAMIC
EAPDYN += src/eap_peer/eap_md5.so
else
L_CFLAGS += -DEAP_MD5
OBJS += src/eap_peer/eap_md5.c
OBJS_h += src/eap_server/eap_server_md5.c
endif
CHAP=y
CONFIG_IEEE8021X_EAPOL=y
endif

# backwards compatibility for old spelling
ifdef CONFIG_MSCHAPV2
ifndef CONFIG_EAP_MSCHAPV2
CONFIG_EAP_MSCHAPV2=y
endif
endif

ifdef CONFIG_EAP_MSCHAPV2
# EAP-MSCHAPv2
ifeq ($(CONFIG_EAP_MSCHAPV2), dyn)
L_CFLAGS += -DEAP_MSCHAPv2_DYNAMIC
EAPDYN += src/eap_peer/eap_mschapv2.so
EAPDYN += src/eap_peer/mschapv2.so
else
L_CFLAGS += -DEAP_MSCHAPv2
OBJS += src/eap_peer/eap_mschapv2.c
OBJS += src/eap_peer/mschapv2.c
OBJS_h += src/eap_server/eap_server_mschapv2.c
endif
MS_FUNCS=y
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_GTC
# EAP-GTC
ifeq ($(CONFIG_EAP_GTC), dyn)
L_CFLAGS += -DEAP_GTC_DYNAMIC
EAPDYN += src/eap_peer/eap_gtc.so
else
L_CFLAGS += -DEAP_GTC
OBJS += src/eap_peer/eap_gtc.c
OBJS_h += src/eap_server/eap_server_gtc.c
endif
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_OTP
# EAP-OTP
ifeq ($(CONFIG_EAP_OTP), dyn)
L_CFLAGS += -DEAP_OTP_DYNAMIC
EAPDYN += src/eap_peer/eap_otp.so
else
L_CFLAGS += -DEAP_OTP
OBJS += src/eap_peer/eap_otp.c
endif
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_SIM
# EAP-SIM
ifeq ($(CONFIG_EAP_SIM), dyn)
L_CFLAGS += -DEAP_SIM_DYNAMIC
EAPDYN += src/eap_peer/eap_sim.so
else
L_CFLAGS += -DEAP_SIM
OBJS += src/eap_peer/eap_sim.c
OBJS_h += src/eap_server/eap_server_sim.c
endif
CONFIG_IEEE8021X_EAPOL=y
CONFIG_EAP_SIM_COMMON=y
NEED_AES_CBC=y
endif

ifdef CONFIG_EAP_LEAP
# EAP-LEAP
ifeq ($(CONFIG_EAP_LEAP), dyn)
L_CFLAGS += -DEAP_LEAP_DYNAMIC
EAPDYN += src/eap_peer/eap_leap.so
else
L_CFLAGS += -DEAP_LEAP
OBJS += src/eap_peer/eap_leap.c
endif
MS_FUNCS=y
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_PSK
# EAP-PSK
ifeq ($(CONFIG_EAP_PSK), dyn)
L_CFLAGS += -DEAP_PSK_DYNAMIC
EAPDYN += src/eap_peer/eap_psk.so
else
L_CFLAGS += -DEAP_PSK
OBJS += src/eap_peer/eap_psk.c src/eap_common/eap_psk_common.c
OBJS_h += src/eap_server/eap_server_psk.c
endif
CONFIG_IEEE8021X_EAPOL=y
NEED_AES=y
NEED_AES_OMAC1=y
NEED_AES_ENCBLOCK=y
NEED_AES_EAX=y
endif

ifdef CONFIG_EAP_AKA
# EAP-AKA
ifeq ($(CONFIG_EAP_AKA), dyn)
L_CFLAGS += -DEAP_AKA_DYNAMIC
EAPDYN += src/eap_peer/eap_aka.so
else
L_CFLAGS += -DEAP_AKA
OBJS += src/eap_peer/eap_aka.c
OBJS_h += src/eap_server/eap_server_aka.c
endif
CONFIG_IEEE8021X_EAPOL=y
CONFIG_EAP_SIM_COMMON=y
NEED_AES_CBC=y
endif

ifdef CONFIG_EAP_AKA_PRIME
# EAP-AKA'
ifeq ($(CONFIG_EAP_AKA_PRIME), dyn)
L_CFLAGS += -DEAP_AKA_PRIME_DYNAMIC
else
L_CFLAGS += -DEAP_AKA_PRIME
endif
NEED_SHA256=y
endif

ifdef CONFIG_EAP_SIM_COMMON
OBJS += src/eap_common/eap_sim_common.c
OBJS_h += src/eap_server/eap_sim_db.c
NEED_AES=y
NEED_FIPS186_2_PRF=y
endif

ifdef CONFIG_EAP_FAST
# EAP-FAST
ifeq ($(CONFIG_EAP_FAST), dyn)
L_CFLAGS += -DEAP_FAST_DYNAMIC
EAPDYN += src/eap_peer/eap_fast.so
EAPDYN += src/eap_common/eap_fast_common.c
else
L_CFLAGS += -DEAP_FAST
OBJS += src/eap_peer/eap_fast.c src/eap_peer/eap_fast_pac.c
OBJS += src/eap_common/eap_fast_common.c
OBJS_h += src/eap_server/eap_server_fast.c
endif
TLS_FUNCS=y
CONFIG_IEEE8021X_EAPOL=y
NEED_T_PRF=y
endif

ifdef CONFIG_EAP_PAX
# EAP-PAX
ifeq ($(CONFIG_EAP_PAX), dyn)
L_CFLAGS += -DEAP_PAX_DYNAMIC
EAPDYN += src/eap_peer/eap_pax.so
else
L_CFLAGS += -DEAP_PAX
OBJS += src/eap_peer/eap_pax.c src/eap_common/eap_pax_common.c
OBJS_h += src/eap_server/eap_server_pax.c
endif
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_SAKE
# EAP-SAKE
ifeq ($(CONFIG_EAP_SAKE), dyn)
L_CFLAGS += -DEAP_SAKE_DYNAMIC
EAPDYN += src/eap_peer/eap_sake.so
else
L_CFLAGS += -DEAP_SAKE
OBJS += src/eap_peer/eap_sake.c src/eap_common/eap_sake_common.c
OBJS_h += src/eap_server/eap_server_sake.c
endif
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_GPSK
# EAP-GPSK
ifeq ($(CONFIG_EAP_GPSK), dyn)
L_CFLAGS += -DEAP_GPSK_DYNAMIC
EAPDYN += src/eap_peer/eap_gpsk.so
else
L_CFLAGS += -DEAP_GPSK
OBJS += src/eap_peer/eap_gpsk.c src/eap_common/eap_gpsk_common.c
OBJS_h += src/eap_server/eap_server_gpsk.c
endif
CONFIG_IEEE8021X_EAPOL=y
ifdef CONFIG_EAP_GPSK_SHA256
L_CFLAGS += -DEAP_GPSK_SHA256
endif
NEED_SHA256=y
NEED_AES_OMAC1=y
endif

ifdef CONFIG_EAP_PWD
L_CFLAGS += -DEAP_PWD
OBJS += src/eap_peer/eap_pwd.c src/eap_common/eap_pwd_common.c
OBJS_h += src/eap_server/eap_pwd.c
CONFIG_IEEE8021X_EAPOL=y
NEED_SHA256=y
endif

ifdef CONFIG_WPS
ifdef CONFIG_WPS2
L_CFLAGS += -DCONFIG_WPS2
endif

# EAP-WSC
L_CFLAGS += -DCONFIG_WPS -DEAP_WSC
OBJS += wps_supplicant.c
OBJS += src/utils/uuid.c
OBJS += src/eap_peer/eap_wsc.c src/eap_common/eap_wsc_common.c
OBJS += src/wps/wps.c
OBJS += src/wps/wps_common.c
OBJS += src/wps/wps_attr_parse.c
OBJS += src/wps/wps_attr_build.c
OBJS += src/wps/wps_attr_process.c
OBJS += src/wps/wps_dev_attr.c
OBJS += src/wps/wps_enrollee.c
OBJS += src/wps/wps_registrar.c
OBJS_h += src/eap_server/eap_server_wsc.c
CONFIG_IEEE8021X_EAPOL=y
NEED_DH_GROUPS=y
NEED_SHA256=y
NEED_BASE64=y
NEED_80211_COMMON=y
NEED_AES_CBC=y
NEED_MODEXP=y

ifdef CONFIG_WPS_UFD
L_CFLAGS += -DCONFIG_WPS_UFD
OBJS += src/wps/wps_ufd.c
NEED_WPS_OOB=y
endif

ifdef CONFIG_WPS_NFC
L_CFLAGS += -DCONFIG_WPS_NFC
OBJS += src/wps/ndef.c
OBJS += src/wps/wps_nfc.c
NEED_WPS_OOB=y
ifdef CONFIG_WPS_NFC_PN531
PN531_PATH ?= /usr/local/src/nfc
L_CFLAGS += -DCONFIG_WPS_NFC_PN531
L_CFLAGS += -I${PN531_PATH}/inc
OBJS += src/wps/wps_nfc_pn531.c
LIBS += ${PN531_PATH}/lib/wpsnfc.dll
LIBS += ${PN531_PATH}/lib/libnfc_mapping_pn53x.dll
endif
endif

ifdef NEED_WPS_OOB
L_CFLAGS += -DCONFIG_WPS_OOB
endif

ifdef CONFIG_WPS_ER
CONFIG_WPS_UPNP=y
L_CFLAGS += -DCONFIG_WPS_ER
OBJS += src/wps/wps_er.c
OBJS += src/wps/wps_er_ssdp.c
endif

ifdef CONFIG_WPS_UPNP
L_CFLAGS += -DCONFIG_WPS_UPNP
OBJS += src/wps/wps_upnp.c
OBJS += src/wps/wps_upnp_ssdp.c
OBJS += src/wps/wps_upnp_web.c
OBJS += src/wps/wps_upnp_event.c
OBJS += src/wps/wps_upnp_ap.c
OBJS += src/wps/upnp_xml.c
OBJS += src/wps/httpread.c
OBJS += src/wps/http_client.c
OBJS += src/wps/http_server.c
endif

ifdef CONFIG_WPS_STRICT
L_CFLAGS += -DCONFIG_WPS_STRICT
OBJS += src/wps/wps_validate.c
endif

ifdef CONFIG_WPS_TESTING
L_CFLAGS += -DCONFIG_WPS_TESTING
endif

ifdef CONFIG_WPS_REG_DISABLE_OPEN
L_CFLAGS += -DCONFIG_WPS_REG_DISABLE_OPEN
endif

endif

ifdef CONFIG_EAP_IKEV2
# EAP-IKEv2
ifeq ($(CONFIG_EAP_IKEV2), dyn)
L_CFLAGS += -DEAP_IKEV2_DYNAMIC
EAPDYN += src/eap_peer/eap_ikev2.so src/eap_peer/ikev2.c
EAPDYN += src/eap_common/eap_ikev2_common.c src/eap_common/ikev2_common.c
else
L_CFLAGS += -DEAP_IKEV2
OBJS += src/eap_peer/eap_ikev2.c src/eap_peer/ikev2.c
OBJS += src/eap_common/eap_ikev2_common.c src/eap_common/ikev2_common.c
OBJS_h += src/eap_server/eap_server_ikev2.c
OBJS_h += src/eap_server/ikev2.c
endif
CONFIG_IEEE8021X_EAPOL=y
NEED_DH_GROUPS=y
NEED_DH_GROUPS_ALL=y
NEED_MODEXP=y
NEED_CIPHER=y
endif

ifdef CONFIG_EAP_VENDOR_TEST
ifeq ($(CONFIG_EAP_VENDOR_TEST), dyn)
L_CFLAGS += -DEAP_VENDOR_TEST_DYNAMIC
EAPDYN += src/eap_peer/eap_vendor_test.so
else
L_CFLAGS += -DEAP_VENDOR_TEST
OBJS += src/eap_peer/eap_vendor_test.c
OBJS_h += src/eap_server/eap_server_vendor_test.c
endif
CONFIG_IEEE8021X_EAPOL=y
endif

ifdef CONFIG_EAP_TNC
# EAP-TNC
L_CFLAGS += -DEAP_TNC
OBJS += src/eap_peer/eap_tnc.c
OBJS += src/eap_peer/tncc.c
OBJS_h += src/eap_server/eap_server_tnc.c
OBJS_h += src/eap_server/tncs.c
NEED_BASE64=y
ifndef CONFIG_NATIVE_WINDOWS
ifndef CONFIG_DRIVER_BSD
LIBS += -ldl
endif
endif
endif

ifdef CONFIG_IEEE8021X_EAPOL
# IEEE 802.1X/EAPOL state machines (e.g., for RADIUS authentication)
L_CFLAGS += -DIEEE8021X_EAPOL
OBJS += src/eapol_supp/eapol_supp_sm.c
OBJS += src/eap_peer/eap.c src/eap_peer/eap_methods.c
NEED_EAP_COMMON=y
ifdef CONFIG_DYNAMIC_EAP_METHODS
L_CFLAGS += -DCONFIG_DYNAMIC_EAP_METHODS
LIBS += -ldl -rdynamic
endif
endif

ifdef CONFIG_AP
NEED_80211_COMMON=y
NEED_EAP_COMMON=y
NEED_RSN_AUTHENTICATOR=y
L_CFLAGS += -DCONFIG_AP
OBJS += ap.c
L_CFLAGS += -DCONFIG_NO_RADIUS
L_CFLAGS += -DCONFIG_NO_ACCOUNTING
L_CFLAGS += -DCONFIG_NO_VLAN
OBJS += src/ap/hostapd.c
OBJS += src/ap/wpa_auth_glue.c
OBJS += src/ap/utils.c
OBJS += src/ap/authsrv.c
OBJS += src/ap/ap_config.c
OBJS += src/utils/ip_addr.c
OBJS += src/ap/sta_info.c
OBJS += src/ap/tkip_countermeasures.c
OBJS += src/ap/ap_mlme.c
OBJS += src/ap/ieee802_1x.c
OBJS += src/eapol_auth/eapol_auth_sm.c
OBJS += src/ap/ieee802_11_auth.c
OBJS += src/ap/drv_callbacks.c
OBJS += src/ap/ap_drv_ops.c
ifdef CONFIG_IEEE80211N
OBJS += src/ap/ieee802_11_ht.c
endif
ifdef CONFIG_CTRL_IFACE
OBJS += src/ap/ctrl_iface_ap.c
endif

L_CFLAGS += -DEAP_SERVER -DEAP_SERVER_IDENTITY
OBJS += src/eap_server/eap_server.c
OBJS += src/eap_server/eap_server_identity.c
OBJS += src/eap_server/eap_server_methods.c

ifdef CONFIG_IEEE80211N
L_CFLAGS += -DCONFIG_IEEE80211N
endif

ifdef NEED_AP_MLME
OBJS += src/ap/beacon.c
OBJS += src/ap/wmm.c
OBJS += src/ap/ap_list.c
OBJS += src/ap/ieee802_11.c
OBJS += src/ap/hw_features.c
L_CFLAGS += -DNEED_AP_MLME
endif
ifdef CONFIG_WPS
L_CFLAGS += -DEAP_SERVER_WSC
OBJS += src/ap/wps_hostapd.c
OBJS += src/eap_server/eap_server_wsc.c
endif
endif

ifdef NEED_RSN_AUTHENTICATOR
L_CFLAGS += -DCONFIG_NO_RADIUS
NEED_AES_WRAP=y
OBJS += src/ap/wpa_auth.c
OBJS += src/ap/wpa_auth_ie.c
OBJS += src/ap/pmksa_cache_auth.c
ifdef CONFIG_IEEE80211R
OBJS += src/ap/wpa_auth_ft.c
endif
ifdef CONFIG_PEERKEY
OBJS += src/ap/peerkey_auth.c
endif
endif

ifdef CONFIG_EAP_SERVER
L_CFLAGS += -DEAP_SERVER
OBJS_h += src/eap_server/eap_server.c
OBJS_h += src/eap_server/eap_server_identity.c
OBJS_h += src/eap_server/eap_server_methods.c
endif

ifdef CONFIG_RADIUS_CLIENT
OBJS_h += src/utils/ip_addr.c
OBJS_h += src/radius/radius.c
OBJS_h += src/radius/radius_client.c
endif

ifdef CONFIG_AUTHENTICATOR
OBJS_h += src/eapol_auth/eapol_auth_sm.c
OBJS_h += src/ap/ieee802_1x.c
endif

ifdef CONFIG_WPA_AUTHENTICATOR
OBJS_h += src/ap/wpa_auth.c
OBJS_h += src/ap/wpa_auth_ie.c
OBJS_h += src/ap/pmksa_cache_auth.c
ifdef CONFIG_IEEE80211R
OBJS_h += src/ap/wpa_auth_ft.c
endif
ifdef CONFIG_PEERKEY
OBJS_h += src/ap/peerkey_auth.c
endif
endif

ifdef CONFIG_PCSC
# PC/SC interface for smartcards (USIM, GSM SIM)
L_CFLAGS += -DPCSC_FUNCS -I/usr/include/PCSC
OBJS += src/utils/pcsc_funcs.c
# -lpthread may not be needed depending on how pcsc-lite was configured
ifdef CONFIG_NATIVE_WINDOWS
#Once MinGW gets support for WinScard, -lwinscard could be used instead of the
#dynamic symbol loading that is now used in pcsc_funcs.c
#LIBS += -lwinscard
else
LIBS += -lpcsclite -lpthread
endif
endif

ifdef CONFIG_SIM_SIMULATOR
L_CFLAGS += -DCONFIG_SIM_SIMULATOR
NEED_MILENAGE=y
endif

ifdef CONFIG_USIM_SIMULATOR
L_CFLAGS += -DCONFIG_USIM_SIMULATOR
NEED_MILENAGE=y
endif

ifdef NEED_MILENAGE
OBJS += src/crypto/milenage.c
endif

ifdef CONFIG_PKCS12
L_CFLAGS += -DPKCS12_FUNCS
endif

ifdef CONFIG_SMARTCARD
L_CFLAGS += -DCONFIG_SMARTCARD
endif

ifdef MS_FUNCS
OBJS += src/crypto/ms_funcs.c
NEED_DES=y
NEED_MD4=y
endif

ifdef CHAP
OBJS += src/eap_common/chap.c
endif

ifdef TLS_FUNCS
NEED_DES=y
# Shared TLS functions (needed for EAP_TLS, EAP_PEAP, EAP_TTLS, and EAP_FAST)
OBJS += src/eap_peer/eap_tls_common.c
OBJS_h += src/eap_server/eap_server_tls_common.c
NEED_TLS_PRF=y
endif

ifndef CONFIG_TLS
CONFIG_TLS=openssl
endif

ifeq ($(CONFIG_TLS), openssl)
ifdef TLS_FUNCS
L_CFLAGS += -DEAP_TLS_OPENSSL
OBJS += src/crypto/tls_openssl.c
LIBS += -lssl
endif
OBJS += src/crypto/crypto_openssl.c
OBJS_p += src/crypto/crypto_openssl.c
ifdef NEED_FIPS186_2_PRF
OBJS += src/crypto/fips_prf_openssl.c
endif
LIBS += -lcrypto
LIBS_p += -lcrypto
endif

ifeq ($(CONFIG_TLS), gnutls)
ifdef TLS_FUNCS
OBJS += src/crypto/tls_gnutls.c
LIBS += -lgnutls -lgpg-error
ifdef CONFIG_GNUTLS_EXTRA
L_CFLAGS += -DCONFIG_GNUTLS_EXTRA
LIBS += -lgnutls-extra
endif
endif
OBJS += src/crypto/crypto_gnutls.c
OBJS_p += src/crypto/crypto_gnutls.c
ifdef NEED_FIPS186_2_PRF
OBJS += src/crypto/fips_prf_gnutls.c
endif
LIBS += -lgcrypt
LIBS_p += -lgcrypt
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif

ifeq ($(CONFIG_TLS), schannel)
ifdef TLS_FUNCS
OBJS += src/crypto/tls_schannel.c
endif
OBJS += src/crypto/crypto_cryptoapi.c
OBJS_p += src/crypto/crypto_cryptoapi.c
ifdef NEED_FIPS186_2_PRF
OBJS += src/crypto/fips_prf_cryptoapi.c
endif
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif

ifeq ($(CONFIG_TLS), nss)
ifdef TLS_FUNCS
OBJS += src/crypto/tls_nss.c
LIBS += -lssl3
endif
OBJS += src/crypto/crypto_nss.c
OBJS_p += src/crypto/crypto_nss.c
ifdef NEED_FIPS186_2_PRF
OBJS += src/crypto/fips_prf_nss.c
endif
LIBS += -lnss3
LIBS_p += -lnss3
CONFIG_INTERNAL_MD4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif

ifeq ($(CONFIG_TLS), internal)
ifndef CONFIG_CRYPTO
CONFIG_CRYPTO=internal
endif
ifdef TLS_FUNCS
OBJS += src/crypto/crypto_internal-rsa.c
OBJS += src/crypto/tls_internal.c
OBJS += src/tls/tlsv1_common.c
OBJS += src/tls/tlsv1_record.c
OBJS += src/tls/tlsv1_cred.c
OBJS += src/tls/tlsv1_client.c
OBJS += src/tls/tlsv1_client_write.c
OBJS += src/tls/tlsv1_client_read.c
OBJS += src/tls/asn1.c
OBJS += src/tls/rsa.c
OBJS += src/tls/x509v3.c
OBJS += src/tls/pkcs1.c
OBJS += src/tls/pkcs5.c
OBJS += src/tls/pkcs8.c
NEED_SHA256=y
NEED_BASE64=y
NEED_TLS_PRF=y
NEED_MODEXP=y
NEED_CIPHER=y
L_CFLAGS += -DCONFIG_TLS_INTERNAL_CLIENT
endif
ifdef NEED_CIPHER
NEED_DES=y
OBJS += src/crypto/crypto_internal-cipher.c
endif
ifdef NEED_MODEXP
OBJS += src/crypto/crypto_internal-modexp.c
OBJS += src/tls/bignum.c
endif
ifeq ($(CONFIG_CRYPTO), libtomcrypt)
OBJS += src/crypto/crypto_libtomcrypt.c
OBJS_p += src/crypto/crypto_libtomcrypt.c
LIBS += -ltomcrypt -ltfm
LIBS_p += -ltomcrypt -ltfm
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif
ifeq ($(CONFIG_CRYPTO), internal)
OBJS += src/crypto/crypto_internal.c
OBJS_p += src/crypto/crypto_internal.c
NEED_AES_ENC=y
L_CFLAGS += -DCONFIG_CRYPTO_INTERNAL
ifdef CONFIG_INTERNAL_LIBTOMMATH
L_CFLAGS += -DCONFIG_INTERNAL_LIBTOMMATH
ifdef CONFIG_INTERNAL_LIBTOMMATH_FAST
L_CFLAGS += -DLTM_FAST
endif
else
LIBS += -ltommath
LIBS_p += -ltommath
endif
CONFIG_INTERNAL_AES=y
CONFIG_INTERNAL_DES=y
CONFIG_INTERNAL_SHA1=y
CONFIG_INTERNAL_MD4=y
CONFIG_INTERNAL_MD5=y
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
CONFIG_INTERNAL_DH_GROUP5=y
endif
ifeq ($(CONFIG_CRYPTO), cryptoapi)
OBJS += src/crypto/crypto_cryptoapi.c
OBJS_p += src/crypto/crypto_cryptoapi.c
L_CFLAGS += -DCONFIG_CRYPTO_CRYPTOAPI
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
endif
endif

ifeq ($(CONFIG_TLS), none)
ifdef TLS_FUNCS
OBJS += src/crypto/tls_none.c
L_CFLAGS += -DEAP_TLS_NONE
CONFIG_INTERNAL_AES=y
CONFIG_INTERNAL_SHA1=y
CONFIG_INTERNAL_MD5=y
endif
OBJS += src/crypto/crypto_none.c
OBJS_p += src/crypto/crypto_none.c
CONFIG_INTERNAL_SHA256=y
CONFIG_INTERNAL_RC4=y
endif

ifdef TLS_FUNCS
ifdef CONFIG_SMARTCARD
ifndef CONFIG_NATIVE_WINDOWS
ifneq ($(CONFIG_L2_PACKET), freebsd)
LIBS += -ldl
endif
endif
endif
endif

ifndef TLS_FUNCS
OBJS += src/crypto/tls_none.c
ifeq ($(CONFIG_TLS), internal)
CONFIG_INTERNAL_AES=y
CONFIG_INTERNAL_SHA1=y
CONFIG_INTERNAL_MD5=y
CONFIG_INTERNAL_RC4=y
endif
endif

AESOBJS = # none so far (see below)
ifdef CONFIG_INTERNAL_AES
AESOBJS += src/crypto/aes-internal.c src/crypto/aes-internal-dec.c
endif

AESOBJS += src/crypto/aes-unwrap.c
ifdef NEED_AES_EAX
AESOBJS += src/crypto/aes-eax.c
NEED_AES_CTR=y
endif
ifdef NEED_AES_CTR
AESOBJS += src/crypto/aes-ctr.c
endif
ifdef NEED_AES_ENCBLOCK
AESOBJS += src/crypto/aes-encblock.c
endif
ifdef NEED_AES_OMAC1
NEED_AES_ENC=y
AESOBJS += src/crypto/aes-omac1.c
endif
ifdef NEED_AES_WRAP
NEED_AES_ENC=y
AESOBJS += src/crypto/aes-wrap.c
endif
ifdef NEED_AES_CBC
NEED_AES_ENC=y
AESOBJS += src/crypto/aes-cbc.c
endif
ifdef NEED_AES_ENC
ifdef CONFIG_INTERNAL_AES
AESOBJS += src/crypto/aes-internal-enc.c
endif
endif
ifdef NEED_AES
OBJS += $(AESOBJS)
endif

SHA1OBJS =
ifdef NEED_SHA1
SHA1OBJS += src/crypto/sha1.c
ifdef CONFIG_INTERNAL_SHA1
SHA1OBJS += src/crypto/sha1-internal.c
ifdef NEED_FIPS186_2_PRF
SHA1OBJS += src/crypto/fips_prf_internal.c
endif
endif
ifndef CONFIG_NO_WPA_PASSPHRASE
SHA1OBJS += src/crypto/sha1-pbkdf2.c
endif
ifdef NEED_T_PRF
SHA1OBJS += src/crypto/sha1-tprf.c
endif
ifdef NEED_TLS_PRF
SHA1OBJS += src/crypto/sha1-tlsprf.c
endif
endif

MD5OBJS = src/crypto/md5.c
ifdef NEED_MD5
ifdef CONFIG_INTERNAL_MD5
MD5OBJS += src/crypto/md5-internal.c
endif
ifdef CONFIG_FIPS
MD5OBJS += src/crypto/md5-non-fips.c
endif
OBJS += $(MD5OBJS)
OBJS_p += $(MD5OBJS)
endif

ifdef NEED_MD4
ifdef CONFIG_INTERNAL_MD4
OBJS += src/crypto/md4-internal.c
endif
endif

DESOBJS = # none needed when not internal
ifdef NEED_DES
ifdef CONFIG_INTERNAL_DES
DESOBJS += src/crypto/des-internal.c
endif
endif

ifdef NEED_RC4
ifdef CONFIG_INTERNAL_RC4
OBJS += src/crypto/rc4.c
endif
endif

SHA256OBJS = # none by default
ifdef NEED_SHA256
L_CFLAGS += -DCONFIG_SHA256
SHA256OBJS += src/crypto/sha256.c
ifdef CONFIG_INTERNAL_SHA256
SHA256OBJS += src/crypto/sha256-internal.c
endif
OBJS += $(SHA256OBJS)
endif

ifdef NEED_DH_GROUPS
OBJS += src/crypto/dh_groups.c
endif
ifdef NEED_DH_GROUPS_ALL
L_CFLAGS += -DALL_DH_GROUPS
endif
ifdef CONFIG_INTERNAL_DH_GROUP5
ifdef NEED_DH_GROUPS
OBJS += src/crypto/dh_group5.c
endif
endif

ifdef CONFIG_NO_RANDOM_POOL
L_CFLAGS += -DCONFIG_NO_RANDOM_POOL
else
OBJS += src/crypto/random.c
endif

ifdef CONFIG_CTRL_IFACE
ifeq ($(CONFIG_CTRL_IFACE), y)
ifdef CONFIG_NATIVE_WINDOWS
CONFIG_CTRL_IFACE=named_pipe
else
CONFIG_CTRL_IFACE=unix
endif
endif
L_CFLAGS += -DCONFIG_CTRL_IFACE
ifeq ($(CONFIG_CTRL_IFACE), unix)
L_CFLAGS += -DCONFIG_CTRL_IFACE_UNIX
endif
ifeq ($(CONFIG_CTRL_IFACE), udp)
L_CFLAGS += -DCONFIG_CTRL_IFACE_UDP
endif
ifeq ($(CONFIG_CTRL_IFACE), named_pipe)
L_CFLAGS += -DCONFIG_CTRL_IFACE_NAMED_PIPE
endif
OBJS += ctrl_iface.c ctrl_iface_$(CONFIG_CTRL_IFACE).c
endif

ifdef CONFIG_CTRL_IFACE_DBUS
DBUS=y
DBUS_CFLAGS += -DCONFIG_CTRL_IFACE_DBUS -DDBUS_API_SUBJECT_TO_CHANGE
DBUS_OBJS += dbus/dbus_old.c dbus/dbus_old_handlers.c
ifdef CONFIG_WPS
DBUS_OBJS += dbus/dbus_old_handlers_wps.c
endif
DBUS_OBJS += dbus/dbus_dict_helpers.c
ifndef DBUS_LIBS
DBUS_LIBS := $(shell pkg-config --libs dbus-1)
endif
ifndef DBUS_INCLUDE
DBUS_INCLUDE := $(shell pkg-config --cflags dbus-1)
endif
dbus_version=$(subst ., ,$(shell pkg-config --modversion dbus-1))
DBUS_VERSION_MAJOR=$(word 1,$(dbus_version))
DBUS_VERSION_MINOR=$(word 2,$(dbus_version))
ifeq ($(DBUS_VERSION_MAJOR),)
DBUS_VERSION_MAJOR=0
endif
ifeq ($(DBUS_VERSION_MINOR),)
DBUS_VERSION_MINOR=0
endif
DBUS_INCLUDE += -DDBUS_VERSION_MAJOR=$(DBUS_VERSION_MAJOR)
DBUS_INCLUDE += -DDBUS_VERSION_MINOR=$(DBUS_VERSION_MINOR)
DBUS_CFLAGS += $(DBUS_INCLUDE)
endif

ifdef CONFIG_CTRL_IFACE_DBUS_NEW
DBUS=y
DBUS_CFLAGS += -DCONFIG_CTRL_IFACE_DBUS_NEW
DBUS_OBJS ?= dbus/dbus_dict_helpers.c
DBUS_OBJS += dbus/dbus_new_helpers.c
DBUS_OBJS += dbus/dbus_new.c dbus/dbus_new_handlers.c
ifdef CONFIG_WPS
DBUS_OBJS += dbus/dbus_new_handlers_wps.c
endif
ifndef DBUS_LIBS
DBUS_LIBS := $(shell pkg-config --libs dbus-1)
endif
ifndef DBUS_INCLUDE
DBUS_INCLUDE := $(shell pkg-config --cflags dbus-1)
endif
ifdef CONFIG_CTRL_IFACE_DBUS_INTRO
DBUS_OBJS += dbus/dbus_new_introspect.c
DBUS_CFLAGS += -DCONFIG_CTRL_IFACE_DBUS_INTRO
endif
DBUS_CFLAGS += $(DBUS_INCLUDE)
endif

ifdef DBUS
DBUS_CFLAGS += -DCONFIG_DBUS
DBUS_OBJS += dbus/dbus_common.c
endif

OBJS += $(DBUS_OBJS)
L_CFLAGS += $(DBUS_CFLAGS)
LIBS += $(DBUS_LIBS)

ifdef CONFIG_READLINE
OBJS_c += src/utils/edit_readline.c
LIBS_c += -lncurses -lreadline
else
ifdef CONFIG_WPA_CLI_EDIT
OBJS_c += src/utils/edit.c
else
OBJS_c += src/utils/edit_simple.c
endif
endif

ifdef CONFIG_NATIVE_WINDOWS
L_CFLAGS += -DCONFIG_NATIVE_WINDOWS
LIBS += -lws2_32 -lgdi32 -lcrypt32
LIBS_c += -lws2_32
LIBS_p += -lws2_32 -lgdi32
ifeq ($(CONFIG_CRYPTO), cryptoapi)
LIBS_p += -lcrypt32
endif
endif

ifdef CONFIG_NO_STDOUT_DEBUG
L_CFLAGS += -DCONFIG_NO_STDOUT_DEBUG
ifndef CONFIG_CTRL_IFACE
L_CFLAGS += -DCONFIG_NO_WPA_MSG
endif
endif

ifdef CONFIG_ANDROID_LOG
L_CFLAGS += -DCONFIG_ANDROID_LOG
endif

ifdef CONFIG_IPV6
# for eapol_test only
L_CFLAGS += -DCONFIG_IPV6
endif

ifdef NEED_BASE64
OBJS += src/utils/base64.c
endif

ifdef NEED_SME
NEED_80211_COMMON=y
OBJS += sme.c
L_CFLAGS += -DCONFIG_SME
endif

ifdef CONFIG_CLIENT_MLME
OBJS += mlme.c
L_CFLAGS += -DCONFIG_CLIENT_MLME
NEED_80211_COMMON=y
endif

ifdef NEED_80211_COMMON
OBJS += src/common/ieee802_11_common.c
endif

ifdef NEED_EAP_COMMON
OBJS += src/eap_common/eap_common.c
endif

ifndef CONFIG_MAIN
CONFIG_MAIN=main
endif

ifdef CONFIG_DEBUG_SYSLOG
L_CFLAGS += -DCONFIG_DEBUG_SYSLOG
endif

ifdef CONFIG_DEBUG_FILE
L_CFLAGS += -DCONFIG_DEBUG_FILE
endif

ifdef CONFIG_DELAYED_MIC_ERROR_REPORT
L_CFLAGS += -DCONFIG_DELAYED_MIC_ERROR_REPORT
endif

ifdef CONFIG_FIPS
L_CFLAGS += -DCONFIG_FIPS
endif

OBJS += $(SHA1OBJS) $(DESOBJS)

OBJS_p += $(SHA1OBJS)

ifdef CONFIG_BGSCAN_SIMPLE
L_CFLAGS += -DCONFIG_BGSCAN_SIMPLE
OBJS += bgscan_simple.c
NEED_BGSCAN=y
endif

ifdef CONFIG_BGSCAN_LEARN
L_CFLAGS += -DCONFIG_BGSCAN_LEARN
OBJS += bgscan_learn.c
NEED_BGSCAN=y
endif

ifdef NEED_BGSCAN
L_CFLAGS += -DCONFIG_BGSCAN
OBJS += bgscan.c
endif

OBJS_wpa_rm := ctrl_iface.c mlme.c ctrl_iface_unix.c
OBJS_wpa := $(filter-out $(OBJS_wpa_rm),$(OBJS)) $(OBJS_h) tests/test_wpa.c
ifdef CONFIG_AUTHENTICATOR
OBJS_wpa += tests/link_test.c
endif
OBJS_wpa += $(OBJS_l2)
OBJS += wpa_supplicant.c events.c blacklist.c wpas_glue.c scan.c
OBJS_t := $(OBJS) $(OBJS_l2) eapol_test.c
OBJS_t += src/radius/radius_client.c
OBJS_t += src/radius/radius.c
ifndef CONFIG_AP
OBJS_t += src/utils/ip_addr.c
endif
OBJS_t2 := $(OBJS) $(OBJS_l2) preauth_test.c
OBJS += $(CONFIG_MAIN).c

ifdef CONFIG_PRIVSEP
OBJS_priv += $(OBJS_d) src/drivers/drivers.c
OBJS_priv += $(OBJS_l2)
OBJS_priv += src/utils/os_$(CONFIG_OS).c
OBJS_priv += src/utils/$(CONFIG_ELOOP).c
OBJS_priv += src/utils/common.c
OBJS_priv += src/utils/wpa_debug.c
OBJS_priv += src/utils/wpabuf.c
OBJS_priv += wpa_priv.c
ifdef CONFIG_DRIVER_TEST
OBJS_priv += $(SHA1OBJS)
OBJS_priv += $(MD5OBJS)
ifeq ($(CONFIG_TLS), openssl)
OBJS_priv += src/crypto/crypto_openssl.c
endif
ifeq ($(CONFIG_TLS), gnutls)
OBJS_priv += src/crypto/crypto_gnutls.c
endif
ifeq ($(CONFIG_TLS), nss)
OBJS_priv += src/crypto/crypto_nss.c
endif
ifeq ($(CONFIG_TLS), internal)
ifeq ($(CONFIG_CRYPTO), libtomcrypt)
OBJS_priv += src/crypto/crypto_libtomcrypt.c
else
OBJS_priv += src/crypto/crypto_internal.c
endif
endif
endif # CONFIG_DRIVER_TEST
OBJS += src/l2_packet/l2_packet_privsep.c
OBJS += src/drivers/driver_privsep.c
EXTRA_progs += wpa_priv
else
OBJS += $(OBJS_d) src/drivers/drivers.c
OBJS += $(OBJS_l2)
endif

ifdef CONFIG_NDIS_EVENTS_INTEGRATED
L_CFLAGS += -DCONFIG_NDIS_EVENTS_INTEGRATED
OBJS += src/drivers/ndis_events.c
EXTRALIBS += -loleaut32 -lole32 -luuid
ifdef PLATFORMSDKLIB
EXTRALIBS += $(PLATFORMSDKLIB)/WbemUuid.Lib
else
EXTRALIBS += WbemUuid.Lib
endif
endif

ifndef LDO
LDO=$(CC)
endif

ifeq ($(WPA_BUILD_SUPPLICANT),true)

########################

include $(CLEAR_VARS)
LOCAL_MODULE := wpa_cli
LOCAL_MODULE_TAGS := debug
LOCAL_SHARED_LIBRARIES := libc libcutils
LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_SRC_FILES := $(OBJS_c)
LOCAL_C_INCLUDES := $(INCLUDES)
include $(BUILD_EXECUTABLE)

########################
include $(CLEAR_VARS)
LOCAL_MODULE := wpa_supplicant
ifdef CONFIG_DRIVER_CUSTOM
LOCAL_STATIC_LIBRARIES := libCustomWifi
endif
ifneq ($(BOARD_WPA_SUPPLICANT_PRIVATE_LIB),)
LOCAL_STATIC_LIBRARIES += $(BOARD_WPA_SUPPLICANT_PRIVATE_LIB)
endif
LOCAL_SHARED_LIBRARIES := libc libcutils libcrypto libssl
ifdef CONFIG_DRIVER_NL80211
LOCAL_SHARED_LIBRARIES += libnl_2
endif
LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_SRC_FILES := $(OBJS)
LOCAL_C_INCLUDES := $(INCLUDES)
include $(BUILD_EXECUTABLE)

########################
#
#include $(CLEAR_VARS)
#LOCAL_MODULE := eapol_test
#ifdef CONFIG_DRIVER_CUSTOM
#LOCAL_STATIC_LIBRARIES := libCustomWifi
#endif
#LOCAL_SHARED_LIBRARIES := libc libcrypto libssl
#LOCAL_CFLAGS := $(L_CFLAGS)
#LOCAL_SRC_FILES := $(OBJS_t)
#LOCAL_C_INCLUDES := $(INCLUDES)
#include $(BUILD_EXECUTABLE)
#
########################
#
#local_target_dir := $(TARGET_OUT)/etc/wifi
#
#include $(CLEAR_VARS)
#LOCAL_MODULE := wpa_supplicant.conf
#LOCAL_MODULE_TAGS := user
#LOCAL_MODULE_CLASS := ETC
#LOCAL_MODULE_PATH := $(local_target_dir)
#LOCAL_SRC_FILES := $(LOCAL_MODULE)
#include $(BUILD_PREBUILT)
#
########################

endif # ifeq ($(WPA_BUILD_SUPPLICANT),true)

include $(CLEAR_VARS)
LOCAL_MODULE = libwpa_client
LOCAL_CFLAGS = $(L_CFLAGS)
LOCAL_SRC_FILES = src/common/wpa_ctrl.c src/utils/os_$(CONFIG_OS).c
LOCAL_C_INCLUDES = $(INCLUDES)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_COPY_HEADERS_TO := libwpa_client
LOCAL_COPY_HEADERS := src/common/wpa_ctrl.h
include $(BUILD_SHARED_LIBRARY)
