/*
 * hostapd / Configuration file
 * Copyright (c) 2003-2006, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "config_types.h"

typedef u8 macaddr[ETH_ALEN];

struct hostapd_radius_servers;

#define HOSTAPD_MAX_SSID_LEN 32

#define NUM_WEP_KEYS 4
struct hostapd_wep_keys {
	u8 idx;
	u8 *key[NUM_WEP_KEYS];
	size_t len[NUM_WEP_KEYS];
	int keys_set;
	size_t default_len; /* key length used for dynamic key generation */
};

typedef enum hostap_security_policy {
	SECURITY_PLAINTEXT = 0,
	SECURITY_STATIC_WEP = 1,
	SECURITY_IEEE_802_1X = 2,
	SECURITY_WPA_PSK = 3,
	SECURITY_WPA = 4
} secpolicy;

struct hostapd_ssid {
	char ssid[HOSTAPD_MAX_SSID_LEN + 1];
	size_t ssid_len;
	int ssid_set;

	char vlan[IFNAMSIZ + 1];
	secpolicy security_policy;

	struct hostapd_wpa_psk *wpa_psk;
	char *wpa_passphrase;
	char *wpa_psk_file;

	struct hostapd_wep_keys wep;

#define DYNAMIC_VLAN_DISABLED 0
#define DYNAMIC_VLAN_OPTIONAL 1
#define DYNAMIC_VLAN_REQUIRED 2
	int dynamic_vlan;
};


#define PMK_LEN 32
struct hostapd_wpa_psk {
	struct hostapd_wpa_psk *next;
	int group;
	u8 psk[PMK_LEN];
	u8 addr[ETH_ALEN];
};

#define EAP_USER_MAX_METHODS 8
struct hostapd_eap_user {
	struct hostapd_eap_user *next;
	u8 *identity;
	size_t identity_len;
	struct {
		int vendor;
		u32 method;
	} methods[EAP_USER_MAX_METHODS];
	u8 *password;
	size_t password_len;
	int phase2;
	int force_version;
	unsigned int wildcard_prefix:1;
	unsigned int password_hash:1; /* whether password is hashed with
				       * nt_password_hash() */
};

/**
 * struct hostapd_bss_config - Per-BSS configuration
 */
struct hostapd_bss_config {
	char iface[IFNAMSIZ + 1];
	char bridge[IFNAMSIZ + 1];

	enum {
		HOSTAPD_LEVEL_DEBUG_VERBOSE = 0,
		HOSTAPD_LEVEL_DEBUG = 1,
		HOSTAPD_LEVEL_INFO = 2,
		HOSTAPD_LEVEL_NOTICE = 3,
		HOSTAPD_LEVEL_WARNING = 4
	} logger_syslog_level, logger_stdout_level;

#define HOSTAPD_MODULE_IEEE80211 BIT(0)
#define HOSTAPD_MODULE_IEEE8021X BIT(1)
#define HOSTAPD_MODULE_RADIUS BIT(2)
#define HOSTAPD_MODULE_WPA BIT(3)
#define HOSTAPD_MODULE_DRIVER BIT(4)
#define HOSTAPD_MODULE_IAPP BIT(5)
	unsigned int logger_syslog; /* module bitfield */
	unsigned int logger_stdout; /* module bitfield */

	enum { HOSTAPD_DEBUG_NO = 0, HOSTAPD_DEBUG_MINIMAL = 1,
	       HOSTAPD_DEBUG_VERBOSE = 2,
	       HOSTAPD_DEBUG_MSGDUMPS = 3,
	       HOSTAPD_DEBUG_EXCESSIVE = 4 } debug; /* debug verbosity level */
	char *dump_log_name; /* file name for state dump (SIGUSR1) */

	int ieee802_1x; /* use IEEE 802.1X */
	int eapol_version;
	int eap_server; /* Use internal EAP server instead of external
			 * RADIUS server */
	struct hostapd_eap_user *eap_user;
	char *eap_sim_db;
	struct hostapd_ip_addr own_ip_addr;
	char *nas_identifier;
	struct hostapd_radius_servers *radius;

	struct hostapd_ssid ssid;

	char *eap_req_id_text; /* optional displayable message sent with
				* EAP Request-Identity */
	size_t eap_req_id_text_len;
	int eapol_key_index_workaround;

	size_t default_wep_key_len;
	int individual_wep_key_len;
	int wep_rekeying_period;
	int eap_reauth_period;

	int ieee802_11f; /* use IEEE 802.11f (IAPP) */
	char iapp_iface[IFNAMSIZ + 1]; /* interface used with IAPP broadcast
					* frames */

	u8 assoc_ap_addr[ETH_ALEN];
	int assoc_ap; /* whether assoc_ap_addr is set */

	enum {
		ACCEPT_UNLESS_DENIED = 0,
		DENY_UNLESS_ACCEPTED = 1,
		USE_EXTERNAL_RADIUS_AUTH = 2
	} macaddr_acl;
	macaddr *accept_mac;
	int num_accept_mac;
	macaddr *deny_mac;
	int num_deny_mac;

#define HOSTAPD_AUTH_OPEN BIT(0)
#define HOSTAPD_AUTH_SHARED_KEY BIT(1)
	int auth_algs; /* bitfield of allowed IEEE 802.11 authentication
			* algorithms */

#define HOSTAPD_WPA_VERSION_WPA BIT(0)
#define HOSTAPD_WPA_VERSION_WPA2 BIT(1)
	int wpa;
#define WPA_KEY_MGMT_IEEE8021X BIT(0)
#define WPA_KEY_MGMT_PSK BIT(1)
	int wpa_key_mgmt;
#define WPA_CIPHER_NONE BIT(0)
#define WPA_CIPHER_WEP40 BIT(1)
#define WPA_CIPHER_WEP104 BIT(2)
#define WPA_CIPHER_TKIP BIT(3)
#define WPA_CIPHER_CCMP BIT(4)
	int wpa_pairwise;
	int wpa_group;
	int wpa_group_rekey;
	int wpa_strict_rekey;
	int wpa_gmk_rekey;
	int rsn_preauth;
	char *rsn_preauth_interfaces;
	int stakey;

	char *ctrl_interface; /* directory for UNIX domain sockets */
	gid_t ctrl_interface_gid;
	int ctrl_interface_gid_set;

	char *ca_cert;
	char *server_cert;
	char *private_key;
	char *private_key_passwd;
	int check_crl;

	char *radius_server_clients;
	int radius_server_auth_port;
	int radius_server_ipv6;

	char *test_socket; /* UNIX domain socket path for driver_test */

	int use_pae_group_addr; /* Whether to send EAPOL frames to PAE group
				 * address instead of individual address
				 * (for driver_wired.c).
				 */

	int ap_max_inactivity;
	int ignore_broadcast_ssid;
};


typedef enum {
	HOSTAPD_MODE_IEEE80211B,
	HOSTAPD_MODE_IEEE80211G,
	HOSTAPD_MODE_IEEE80211A,
	NUM_HOSTAPD_MODES
} hostapd_hw_mode;


/**
 * struct hostapd_config - Per-radio interface configuration
 */
struct hostapd_config {
	struct hostapd_bss_config *bss, *last_bss;
	size_t num_bss;

	u16 beacon_int;
	u8 send_probe_response;
	u8 channel;
	hostapd_hw_mode hw_mode; /* HOSTAPD_MODE_IEEE80211A, .. */

	int *supported_rates;
	int *basic_rates;

	const struct driver_ops *driver;
};


struct hostapd_config * hostapd_config_read(const char *fname);
void hostapd_config_free(struct hostapd_config *conf);
int hostapd_maclist_found(macaddr *list, int num_entries, const u8 *addr);
int hostapd_rate_found(int *list, int rate);
const u8 * hostapd_get_psk(const struct hostapd_bss_config *conf,
			   const u8 *addr, const u8 *prev_psk);
int hostapd_setup_wpa_psk(struct hostapd_bss_config *conf);
const struct hostapd_eap_user *
hostapd_get_eap_user(const struct hostapd_bss_config *conf, const u8 *identity,
		     size_t identity_len, int phase2);

#endif /* CONFIG_H */
