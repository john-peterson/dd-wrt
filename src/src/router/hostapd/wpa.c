/*
 * hostapd - IEEE 802.11i-2004 / WPA Authenticator
 * Copyright (c) 2004-2006, Jouni Malinen <jkmaline@cc.hut.fi>
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

#include "includes.h"

#ifndef CONFIG_NATIVE_WINDOWS

#include "hostapd.h"
#include "eapol_sm.h"
#include "wpa.h"
#include "sha1.h"
#include "md5.h"
#include "rc4.h"
#include "aes_wrap.h"
#include "eloop.h"
#include "hostap_common.h"
#include "pmksa_cache.h"
#include "state_machine.h"

#define STATE_MACHINE_DATA struct wpa_state_machine
#define STATE_MACHINE_DEBUG_PREFIX "WPA"
#define STATE_MACHINE_ADDR sm->addr


struct wpa_stakey_negotiation {
	struct wpa_stakey_negotiation *next;
	u8 initiator[ETH_ALEN];
	u8 peer[ETH_ALEN];
	enum { PEER, INITIATOR } state;
	unsigned int num_retries;
	u8 key[32];
	size_t key_len;
	int alg;
};


struct wpa_state_machine {
	struct wpa_authenticator *wpa_auth;

	u8 addr[ETH_ALEN];

	enum {
		WPA_PTK_INITIALIZE, WPA_PTK_DISCONNECT, WPA_PTK_DISCONNECTED,
		WPA_PTK_AUTHENTICATION, WPA_PTK_AUTHENTICATION2,
		WPA_PTK_INITPMK, WPA_PTK_INITPSK, WPA_PTK_PTKSTART,
		WPA_PTK_PTKCALCNEGOTIATING, WPA_PTK_PTKCALCNEGOTIATING2,
		WPA_PTK_PTKINITNEGOTIATING, WPA_PTK_PTKINITDONE
	} wpa_ptk_state;

	enum {
		WPA_PTK_GROUP_IDLE = 0,
		WPA_PTK_GROUP_REKEYNEGOTIATING,
		WPA_PTK_GROUP_REKEYESTABLISHED,
		WPA_PTK_GROUP_KEYERROR
	} wpa_ptk_group_state;

	Boolean Init;
	Boolean DeauthenticationRequest;
	Boolean AuthenticationRequest;
	Boolean ReAuthenticationRequest;
	Boolean Disconnect;
	int TimeoutCtr;
	int GTimeoutCtr;
	Boolean TimeoutEvt;
	Boolean EAPOLKeyReceived;
	Boolean EAPOLKeyPairwise;
	Boolean EAPOLKeyRequest;
	Boolean MICVerified;
	Boolean GUpdateStationKeys;
	u8 ANonce[WPA_NONCE_LEN];
	u8 SNonce[WPA_NONCE_LEN];
	u8 PMK[WPA_PMK_LEN];
	struct wpa_ptk PTK;
	Boolean PTK_valid;
	Boolean pairwise_set;
	int keycount;
	Boolean Pair;
	u8 key_replay_counter[WPA_REPLAY_COUNTER_LEN];
	Boolean key_replay_counter_valid;
	Boolean PInitAKeys; /* WPA only, not in IEEE 802.11i */
	Boolean PTKRequest; /* not in IEEE 802.11i state machine */
	Boolean has_GTK;

	u8 *last_rx_eapol_key; /* starting from IEEE 802.1X header */
	size_t last_rx_eapol_key_len;

	unsigned int changed:1;
	unsigned int in_step_loop:1;
	unsigned int pending_deinit:1;
	unsigned int started:1;

	u8 req_replay_counter[WPA_REPLAY_COUNTER_LEN];
	int req_replay_counter_used;

	u8 *wpa_ie;
	size_t wpa_ie_len;

	enum {
		WPA_VERSION_NO_WPA = 0 /* WPA not used */,
		WPA_VERSION_WPA = 1 /* WPA / IEEE 802.11i/D3.0 */,
		WPA_VERSION_WPA2 = 2 /* WPA2 / IEEE 802.11i */
	} wpa;
	int pairwise; /* Pairwise cipher suite, WPA_CIPHER_* */
	int wpa_key_mgmt; /* the selected WPA_KEY_MGMT_* */
	struct rsn_pmksa_cache_entry *pmksa;

	u32 dot11RSNAStatsTKIPLocalMICFailures;
	u32 dot11RSNAStatsTKIPRemoteMICFailures;
};


/* per authenticator data */
struct wpa_authenticator {
	Boolean GInit;
	int GNoStations;
	int GKeyDoneStations;
	Boolean GTKReKey;
	int GTK_len;
	int GN, GM;
	Boolean GTKAuthenticator;
	u8 Counter[WPA_NONCE_LEN];

	enum {
		WPA_GROUP_GTK_INIT = 0,
		WPA_GROUP_SETKEYS, WPA_GROUP_SETKEYSDONE
	} wpa_group_state;

	u8 GMK[WPA_GMK_LEN];
	u8 GTK[2][WPA_GTK_MAX_LEN];
	u8 GNonce[WPA_NONCE_LEN];
	Boolean changed;

	unsigned int dot11RSNAStatsTKIPRemoteMICFailures;
	u8 dot11RSNAAuthenticationSuiteSelected[4];
	u8 dot11RSNAPairwiseCipherSelected[4];
	u8 dot11RSNAGroupCipherSelected[4];
	u8 dot11RSNAPMKIDUsed[PMKID_LEN];
	u8 dot11RSNAAuthenticationSuiteRequested[4]; /* FIX: update */
	u8 dot11RSNAPairwiseCipherRequested[4]; /* FIX: update */
	u8 dot11RSNAGroupCipherRequested[4]; /* FIX: update */
	unsigned int dot11RSNATKIPCounterMeasuresInvoked;
	unsigned int dot11RSNA4WayHandshakeFailures;

	struct wpa_stakey_negotiation *stakey_negotiations;

	struct wpa_auth_config conf;
	struct wpa_auth_callbacks cb;

	u8 *wpa_ie;
	size_t wpa_ie_len;

	u8 addr[ETH_ALEN];

	struct rsn_pmksa_cache *pmksa;
};


static void wpa_send_eapol_timeout(void *eloop_ctx, void *timeout_ctx);
static void wpa_sm_step(struct wpa_state_machine *sm);
static int wpa_verify_key_mic(struct wpa_ptk *PTK, u8 *data, size_t data_len);
static void wpa_sm_call_step(void *eloop_ctx, void *timeout_ctx);
static void wpa_group_sm_step(struct wpa_authenticator *wpa_auth);
static int wpa_stakey_remove(struct wpa_authenticator *wpa_auth,
			     struct wpa_stakey_negotiation *neg);
static void __wpa_send_eapol(struct wpa_authenticator *wpa_auth,
			     struct wpa_state_machine *sm,
			     int secure, int mic, int ack, int install,
			     int pairwise, u8 *key_rsc, u8 *nonce,
			     u8 *ie, size_t ie_len, u8 *gtk, size_t gtk_len,
			     int keyidx, int encr, int force_version);

/* Default timeouts are 100 ms, but this seems to be a bit too fast for most
 * WPA Supplicants, so use a bit longer timeout. */
static const u32 dot11RSNAConfigGroupUpdateTimeOut = 1000; /* ms */
static const u32 dot11RSNAConfigGroupUpdateCount = 3;
static const u32 dot11RSNAConfigPairwiseUpdateTimeOut = 1000; /* ms */
static const u32 dot11RSNAConfigPairwiseUpdateCount = 3;

/* TODO: make these configurable */
static const int dot11RSNAConfigPMKLifetime = 43200;
static const int dot11RSNAConfigPMKReauthThreshold = 70;
static const int dot11RSNAConfigSATimeout = 60;


static const int WPA_SELECTOR_LEN = 4;
static const u8 WPA_OUI_TYPE[] = { 0x00, 0x50, 0xf2, 1 };
static const u16 WPA_VERSION = 1;
static const u8 WPA_AUTH_KEY_MGMT_UNSPEC_802_1X[] = { 0x00, 0x50, 0xf2, 1 };
static const u8 WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X[] = { 0x00, 0x50, 0xf2, 2 };
static const u8 WPA_CIPHER_SUITE_NONE[] = { 0x00, 0x50, 0xf2, 0 };
static const u8 WPA_CIPHER_SUITE_WEP40[] = { 0x00, 0x50, 0xf2, 1 };
static const u8 WPA_CIPHER_SUITE_TKIP[] = { 0x00, 0x50, 0xf2, 2 };
static const u8 WPA_CIPHER_SUITE_WRAP[] = { 0x00, 0x50, 0xf2, 3 };
static const u8 WPA_CIPHER_SUITE_CCMP[] = { 0x00, 0x50, 0xf2, 4 };
static const u8 WPA_CIPHER_SUITE_WEP104[] = { 0x00, 0x50, 0xf2, 5 };

static const int RSN_SELECTOR_LEN = 4;
static const u16 RSN_VERSION = 1;
static const u8 RSN_AUTH_KEY_MGMT_UNSPEC_802_1X[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X[] = { 0x00, 0x0f, 0xac, 2 };
static const u8 RSN_CIPHER_SUITE_NONE[] = { 0x00, 0x0f, 0xac, 0 };
static const u8 RSN_CIPHER_SUITE_WEP40[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_CIPHER_SUITE_TKIP[] = { 0x00, 0x0f, 0xac, 2 };
static const u8 RSN_CIPHER_SUITE_WRAP[] = { 0x00, 0x0f, 0xac, 3 };
static const u8 RSN_CIPHER_SUITE_CCMP[] = { 0x00, 0x0f, 0xac, 4 };
static const u8 RSN_CIPHER_SUITE_WEP104[] = { 0x00, 0x0f, 0xac, 5 };

/* EAPOL-Key Key Data Encapsulation
 * GroupKey and STAKey require encryption, otherwise, encryption is optional.
 */
static const u8 RSN_KEY_DATA_GROUPKEY[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_KEY_DATA_STAKEY[] = { 0x00, 0x0f, 0xac, 2 };
static const u8 RSN_KEY_DATA_MAC_ADDR[] = { 0x00, 0x0f, 0xac, 3 };
static const u8 RSN_KEY_DATA_PMKID[] = { 0x00, 0x0f, 0xac, 4 };

#define GENERIC_INFO_ELEM 0xdd
#define RSN_INFO_ELEM 0x30

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif /* _MSC_VER */

/* WPA IE version 1
 * 00-50-f2:1 (OUI:OUI type)
 * 0x01 0x00 (version; little endian)
 * (all following fields are optional:)
 * Group Suite Selector (4 octets) (default: TKIP)
 * Pairwise Suite Count (2 octets, little endian) (default: 1)
 * Pairwise Suite List (4 * n octets) (default: TKIP)
 * Authenticated Key Management Suite Count (2 octets, little endian)
 *    (default: 1)
 * Authenticated Key Management Suite List (4 * n octets)
 *    (default: unspec 802.1X)
 * WPA Capabilities (2 octets, little endian) (default: 0)
 */

struct wpa_ie_hdr {
	u8 elem_id;
	u8 len;
	u8 oui[3];
	u8 oui_type;
	u16 version;
} STRUCT_PACKED;


/* RSN IE version 1
 * 0x01 0x00 (version; little endian)
 * (all following fields are optional:)
 * Group Suite Selector (4 octets) (default: CCMP)
 * Pairwise Suite Count (2 octets, little endian) (default: 1)
 * Pairwise Suite List (4 * n octets) (default: CCMP)
 * Authenticated Key Management Suite Count (2 octets, little endian)
 *    (default: 1)
 * Authenticated Key Management Suite List (4 * n octets)
 *    (default: unspec 802.1X)
 * RSN Capabilities (2 octets, little endian) (default: 0)
 * PMKID Count (2 octets) (default: 0)
 * PMKID List (16 * n octets)
 */

struct rsn_ie_hdr {
	u8 elem_id; /* WLAN_EID_RSN */
	u8 len;
	u16 version;
} STRUCT_PACKED;


struct rsn_stakey_kde {
	u8 id;
	u8 len;
	u8 oui[3];
	u8 oui_type;
	u8 reserved[2];
	u8 mac_addr[ETH_ALEN];
	u8 stakey[32]; /* up to 32 bytes */
} STRUCT_PACKED;

#ifdef _MSC_VER
#pragma pack(pop)
#endif /* _MSC_VER */


static inline void wpa_auth_mic_failure_report(
	struct wpa_authenticator *wpa_auth, const u8 *addr)
{
	if (wpa_auth->cb.mic_failure_report)
		wpa_auth->cb.mic_failure_report(wpa_auth->cb.ctx, addr);
}


static inline void wpa_auth_set_eapol(struct wpa_authenticator *wpa_auth,
				      const u8 *addr, wpa_eapol_variable var,
				      int value)
{
	if (wpa_auth->cb.set_eapol)
		wpa_auth->cb.set_eapol(wpa_auth->cb.ctx, addr, var, value);
}


static inline int wpa_auth_get_eapol(struct wpa_authenticator *wpa_auth,
				     const u8 *addr, wpa_eapol_variable var)
{
	if (wpa_auth->cb.get_eapol == NULL)
		return -1;
	return wpa_auth->cb.get_eapol(wpa_auth->cb.ctx, addr, var);
}


static inline const u8 * wpa_auth_get_psk(struct wpa_authenticator *wpa_auth,
					  const u8 *addr, const u8 *prev_psk)
{
	if (wpa_auth->cb.get_psk == NULL)
		return NULL;
	return wpa_auth->cb.get_psk(wpa_auth->cb.ctx, addr, prev_psk);
}


static inline int wpa_auth_get_pmk(struct wpa_authenticator *wpa_auth,
				   const u8 *addr, u8 *pmk, size_t *len)
{
	if (wpa_auth->cb.get_pmk == NULL)
		return -1;
	return wpa_auth->cb.get_pmk(wpa_auth->cb.ctx, addr, pmk, len);
}


static inline int wpa_auth_set_key(struct wpa_authenticator *wpa_auth,
				   const char *alg, const u8 *addr, int idx,
				   u8 *key, size_t key_len)
{
	if (wpa_auth->cb.set_key == NULL)
		return -1;
	return wpa_auth->cb.set_key(wpa_auth->cb.ctx, alg, addr, idx, key,
				    key_len);
}


static inline int wpa_auth_get_seqnum(struct wpa_authenticator *wpa_auth,
				      const u8 *addr, int idx, u8 *seq)
{
	if (wpa_auth->cb.get_seqnum == NULL)
		return -1;
	return wpa_auth->cb.get_seqnum(wpa_auth->cb.ctx, addr, idx, seq);
}


static inline int
wpa_auth_send_eapol(struct wpa_authenticator *wpa_auth, const u8 *addr,
		    const u8 *data, size_t data_len, int encrypt)
{
	if (wpa_auth->cb.send_eapol == NULL)
		return -1;
	return wpa_auth->cb.send_eapol(wpa_auth->cb.ctx, addr, data, data_len,
				       encrypt);
}


static inline int wpa_auth_for_each_sta(struct wpa_authenticator *wpa_auth,
					int (*cb)(struct wpa_state_machine *sm,
						  void *ctx),
					void *cb_ctx)
{
	if (wpa_auth->cb.for_each_sta == NULL)
		return 0;
	return wpa_auth->cb.for_each_sta(wpa_auth->cb.ctx, cb, cb_ctx);
}


static void wpa_auth_logger(struct wpa_authenticator *wpa_auth, const u8 *addr,
			    logger_level level, const char *txt)
{
	if (wpa_auth->cb.logger == NULL)
		return;
	wpa_auth->cb.logger(wpa_auth->cb.ctx, addr, level, txt);
}


static void wpa_auth_vlogger(struct wpa_authenticator *wpa_auth,
			     const u8 *addr, logger_level level,
			     const char *fmt, ...)
{
	char *format;
	int maxlen;
	va_list ap;

	if (wpa_auth->cb.logger == NULL)
		return;

	maxlen = strlen(fmt) + 100;
	format = malloc(maxlen);
	if (!format)
		return;

	va_start(ap, fmt);
	vsnprintf(format, maxlen, fmt, ap);
	va_end(ap);

	wpa_auth_logger(wpa_auth, addr, level, format);

	free(format);
}


static int wpa_write_wpa_ie(struct wpa_auth_config *conf, u8 *buf, size_t len)
{
	struct wpa_ie_hdr *hdr;
	int num_suites;
	u8 *pos, *count;

	hdr = (struct wpa_ie_hdr *) buf;
	hdr->elem_id = WLAN_EID_GENERIC;
	memcpy(&hdr->oui, WPA_OUI_TYPE, WPA_SELECTOR_LEN);
	hdr->version = host_to_le16(WPA_VERSION);
	pos = (u8 *) (hdr + 1);

	if (conf->wpa_group == WPA_CIPHER_CCMP) {
		memcpy(pos, WPA_CIPHER_SUITE_CCMP, WPA_SELECTOR_LEN);
	} else if (conf->wpa_group == WPA_CIPHER_TKIP) {
		memcpy(pos, WPA_CIPHER_SUITE_TKIP, WPA_SELECTOR_LEN);
	} else if (conf->wpa_group == WPA_CIPHER_WEP104) {
		memcpy(pos, WPA_CIPHER_SUITE_WEP104, WPA_SELECTOR_LEN);
	} else if (conf->wpa_group == WPA_CIPHER_WEP40) {
		memcpy(pos, WPA_CIPHER_SUITE_WEP40, WPA_SELECTOR_LEN);
	} else {
		wpa_printf(MSG_DEBUG, "Invalid group cipher (%d).",
			   conf->wpa_group);
		return -1;
	}
	pos += WPA_SELECTOR_LEN;

	num_suites = 0;
	count = pos;
	pos += 2;

	if (conf->wpa_pairwise & WPA_CIPHER_CCMP) {
		memcpy(pos, WPA_CIPHER_SUITE_CCMP, WPA_SELECTOR_LEN);
		pos += WPA_SELECTOR_LEN;
		num_suites++;
	}
	if (conf->wpa_pairwise & WPA_CIPHER_TKIP) {
		memcpy(pos, WPA_CIPHER_SUITE_TKIP, WPA_SELECTOR_LEN);
		pos += WPA_SELECTOR_LEN;
		num_suites++;
	}
	if (conf->wpa_pairwise & WPA_CIPHER_NONE) {
		memcpy(pos, WPA_CIPHER_SUITE_NONE, WPA_SELECTOR_LEN);
		pos += WPA_SELECTOR_LEN;
		num_suites++;
	}

	if (num_suites == 0) {
		wpa_printf(MSG_DEBUG, "Invalid pairwise cipher (%d).",
			   conf->wpa_pairwise);
		return -1;
	}
	*count++ = num_suites & 0xff;
	*count = (num_suites >> 8) & 0xff;

	num_suites = 0;
	count = pos;
	pos += 2;

	if (conf->wpa_key_mgmt & WPA_KEY_MGMT_IEEE8021X) {
		memcpy(pos, WPA_AUTH_KEY_MGMT_UNSPEC_802_1X, WPA_SELECTOR_LEN);
		pos += WPA_SELECTOR_LEN;
		num_suites++;
	}
	if (conf->wpa_key_mgmt & WPA_KEY_MGMT_PSK) {
		memcpy(pos, WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X,
		       WPA_SELECTOR_LEN);
		pos += WPA_SELECTOR_LEN;
		num_suites++;
	}

	if (num_suites == 0) {
		wpa_printf(MSG_DEBUG, "Invalid key management type (%d).",
			   conf->wpa_key_mgmt);
		return -1;
	}
	*count++ = num_suites & 0xff;
	*count = (num_suites >> 8) & 0xff;

	/* WPA Capabilities; use defaults, so no need to include it */

	hdr->len = (pos - buf) - 2;

	return pos - buf;
}


static int wpa_write_rsn_ie(struct wpa_auth_config *conf, u8 *buf, size_t len)
{
	struct rsn_ie_hdr *hdr;
	int num_suites;
	u8 *pos, *count;

	hdr = (struct rsn_ie_hdr *) buf;
	hdr->elem_id = WLAN_EID_RSN;
	pos = (u8 *) &hdr->version;
	*pos++ = RSN_VERSION & 0xff;
	*pos++ = RSN_VERSION >> 8;
	pos = (u8 *) (hdr + 1);

	if (conf->wpa_group == WPA_CIPHER_CCMP) {
		memcpy(pos, RSN_CIPHER_SUITE_CCMP, RSN_SELECTOR_LEN);
	} else if (conf->wpa_group == WPA_CIPHER_TKIP) {
		memcpy(pos, RSN_CIPHER_SUITE_TKIP, RSN_SELECTOR_LEN);
	} else if (conf->wpa_group == WPA_CIPHER_WEP104) {
		memcpy(pos, RSN_CIPHER_SUITE_WEP104, RSN_SELECTOR_LEN);
	} else if (conf->wpa_group == WPA_CIPHER_WEP40) {
		memcpy(pos, RSN_CIPHER_SUITE_WEP40, RSN_SELECTOR_LEN);
	} else {
		wpa_printf(MSG_DEBUG, "Invalid group cipher (%d).",
			   conf->wpa_group);
		return -1;
	}
	pos += RSN_SELECTOR_LEN;

	num_suites = 0;
	count = pos;
	pos += 2;

	if (conf->wpa_pairwise & WPA_CIPHER_CCMP) {
		memcpy(pos, RSN_CIPHER_SUITE_CCMP, RSN_SELECTOR_LEN);
		pos += RSN_SELECTOR_LEN;
		num_suites++;
	}
	if (conf->wpa_pairwise & WPA_CIPHER_TKIP) {
		memcpy(pos, RSN_CIPHER_SUITE_TKIP, RSN_SELECTOR_LEN);
		pos += RSN_SELECTOR_LEN;
		num_suites++;
	}
	if (conf->wpa_pairwise & WPA_CIPHER_NONE) {
		memcpy(pos, RSN_CIPHER_SUITE_NONE, RSN_SELECTOR_LEN);
		pos += RSN_SELECTOR_LEN;
		num_suites++;
	}

	if (num_suites == 0) {
		wpa_printf(MSG_DEBUG, "Invalid pairwise cipher (%d).",
			   conf->wpa_pairwise);
		return -1;
	}
	*count++ = num_suites & 0xff;
	*count = (num_suites >> 8) & 0xff;

	num_suites = 0;
	count = pos;
	pos += 2;

	if (conf->wpa_key_mgmt & WPA_KEY_MGMT_IEEE8021X) {
		memcpy(pos, RSN_AUTH_KEY_MGMT_UNSPEC_802_1X, RSN_SELECTOR_LEN);
		pos += RSN_SELECTOR_LEN;
		num_suites++;
	}
	if (conf->wpa_key_mgmt & WPA_KEY_MGMT_PSK) {
		memcpy(pos, RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X,
		       RSN_SELECTOR_LEN);
		pos += RSN_SELECTOR_LEN;
		num_suites++;
	}

	if (num_suites == 0) {
		wpa_printf(MSG_DEBUG, "Invalid key management type (%d).",
			   conf->wpa_key_mgmt);
		return -1;
	}
	*count++ = num_suites & 0xff;
	*count = (num_suites >> 8) & 0xff;

	/* RSN Capabilities */
	*pos++ = conf->rsn_preauth ? BIT(0) : 0;
	*pos++ = 0;

	hdr->len = (pos - buf) - 2;

	return pos - buf;
}


static int wpa_gen_wpa_ie(struct wpa_authenticator *wpa_auth)
{
	u8 *pos, buf[100];
	int res;

	pos = buf;

	if (wpa_auth->conf.wpa & HOSTAPD_WPA_VERSION_WPA2) {
		res = wpa_write_rsn_ie(&wpa_auth->conf,
				       pos, buf + sizeof(buf) - pos);
		if (res < 0)
			return res;
		pos += res;
	}
	if (wpa_auth->conf.wpa & HOSTAPD_WPA_VERSION_WPA) {
		res = wpa_write_wpa_ie(&wpa_auth->conf,
				       pos, buf + sizeof(buf) - pos);
		if (res < 0)
			return res;
		pos += res;
	}

	free(wpa_auth->wpa_ie);
	wpa_auth->wpa_ie = malloc(pos - buf);
	if (wpa_auth->wpa_ie == NULL)
		return -1;
	memcpy(wpa_auth->wpa_ie, buf, pos - buf);
	wpa_auth->wpa_ie_len = pos - buf;

	return 0;
}


static void wpa_sta_disconnect(struct wpa_authenticator *wpa_auth,
			       const u8 *addr)
{
	if (wpa_auth->cb.disconnect == NULL)
		return;
	wpa_auth->cb.disconnect(wpa_auth->cb.ctx, addr,
				WLAN_REASON_PREV_AUTH_NOT_VALID);
}


static void wpa_rekey_gmk(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_authenticator *wpa_auth = eloop_ctx;

	if (hostapd_get_rand(wpa_auth->GMK, WPA_GMK_LEN)) {
		wpa_printf(MSG_ERROR, "Failed to get random data for WPA "
			   "initialization.");
	} else {
		wpa_auth_logger(wpa_auth, NULL, LOGGER_DEBUG, "GMK rekeyd");
	}

	if (wpa_auth->conf.wpa_gmk_rekey) {
		eloop_register_timeout(wpa_auth->conf.wpa_gmk_rekey, 0,
				       wpa_rekey_gmk, wpa_auth, NULL);
	}
}


static void wpa_rekey_gtk(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_authenticator *wpa_auth = eloop_ctx;

	wpa_auth_logger(wpa_auth, NULL, LOGGER_DEBUG, "rekeying GTK");
	wpa_auth->GTKReKey = TRUE;
	do {
		wpa_auth->changed = FALSE;
		wpa_group_sm_step(wpa_auth);
	} while (wpa_auth->changed);

	if (wpa_auth->conf.wpa_group_rekey) {
		eloop_register_timeout(wpa_auth->conf.wpa_group_rekey,
				       0, wpa_rekey_gtk, wpa_auth, NULL);
	}
}


static int wpa_auth_pmksa_clear_cb(struct wpa_state_machine *sm, void *ctx)
{
	if (sm->pmksa == ctx)
		sm->pmksa = NULL;
	return 0;
}


static void wpa_auth_pmksa_free_cb(struct rsn_pmksa_cache_entry *entry,
				   void *ctx)
{
	struct wpa_authenticator *wpa_auth = ctx;
	wpa_auth_for_each_sta(wpa_auth, wpa_auth_pmksa_clear_cb, entry);
}


/**
 * wpa_init - Initialize WPA authenticator
 * @addr: Authenticator address
 * @conf: Configuration for WPA authenticator
 * Returns: Pointer to WPA authenticator data or %NULL on failure
 */
struct wpa_authenticator * wpa_init(const u8 *addr,
				    struct wpa_auth_config *conf,
				    struct wpa_auth_callbacks *cb)
{
	struct wpa_authenticator *wpa_auth;
	u8 rkey[32];
	u8 buf[ETH_ALEN + 8];

	wpa_auth = wpa_zalloc(sizeof(struct wpa_authenticator));
	if (wpa_auth == NULL)
		return NULL;
	memcpy(wpa_auth->addr, addr, ETH_ALEN);
	memcpy(&wpa_auth->conf, conf, sizeof(*conf));
	memcpy(&wpa_auth->cb, cb, sizeof(*cb));

	if (wpa_gen_wpa_ie(wpa_auth)) {
		wpa_printf(MSG_ERROR, "Could not generate WPA IE.");
		free(wpa_auth);
		return NULL;
	}

	wpa_auth->GTKAuthenticator = TRUE;
	switch (wpa_auth->conf.wpa_group) {
	case WPA_CIPHER_CCMP:
		wpa_auth->GTK_len = 16;
		break;
	case WPA_CIPHER_TKIP:
		wpa_auth->GTK_len = 32;
		break;
	case WPA_CIPHER_WEP104:
		wpa_auth->GTK_len = 13;
		break;
	case WPA_CIPHER_WEP40:
		wpa_auth->GTK_len = 5;
		break;
	}

	wpa_auth->pmksa = pmksa_cache_init(wpa_auth_pmksa_free_cb, wpa_auth);
	if (wpa_auth->pmksa == NULL) {
		wpa_printf(MSG_ERROR, "PMKSA cache initialization failed.");
		free(wpa_auth->wpa_ie);
		free(wpa_auth);
		return NULL;
	}

	/* Counter = PRF-256(Random number, "Init Counter",
	 *                   Local MAC Address || Time)
	 */
	memcpy(buf, wpa_auth->addr, ETH_ALEN);
	wpa_get_ntp_timestamp(buf + ETH_ALEN);
	if (hostapd_get_rand(rkey, sizeof(rkey)) ||
	    hostapd_get_rand(wpa_auth->GMK, WPA_GMK_LEN)) {
		wpa_printf(MSG_ERROR, "Failed to get random data for WPA "
			   "initialization.");
		pmksa_cache_deinit(wpa_auth->pmksa);
		free(wpa_auth->wpa_ie);
		free(wpa_auth);
		return NULL;
	}

	sha1_prf(rkey, sizeof(rkey), "Init Counter", buf, sizeof(buf),
		 wpa_auth->Counter, WPA_NONCE_LEN);

	if (wpa_auth->conf.wpa_gmk_rekey) {
		eloop_register_timeout(wpa_auth->conf.wpa_gmk_rekey, 0,
				       wpa_rekey_gmk, wpa_auth, NULL);
	}

	if (wpa_auth->conf.wpa_group_rekey) {
		eloop_register_timeout(wpa_auth->conf.wpa_group_rekey, 0,
				       wpa_rekey_gtk, wpa_auth, NULL);
	}

	wpa_auth->GInit = TRUE;
	wpa_group_sm_step(wpa_auth);
	wpa_auth->GInit = FALSE;
	wpa_group_sm_step(wpa_auth);

	return wpa_auth;
}


/**
 * wpa_deinit - Deinitialize WPA authenticator
 * @wpa_auth: Pointer to WPA authenticator data from wpa_init()
 */
void wpa_deinit(struct wpa_authenticator *wpa_auth)
{
	eloop_cancel_timeout(wpa_rekey_gmk, wpa_auth, NULL);
	eloop_cancel_timeout(wpa_rekey_gtk, wpa_auth, NULL);

	while (wpa_auth->stakey_negotiations)
		wpa_stakey_remove(wpa_auth, wpa_auth->stakey_negotiations);

	pmksa_cache_deinit(wpa_auth->pmksa);

	free(wpa_auth->wpa_ie);
	free(wpa_auth);
}


/**
 * wpa_reconfig - Update WPA authenticator configuration
 * @wpa_auth: Pointer to WPA authenticator data from wpa_init()
 * @conf: Configuration for WPA authenticator
 */
int wpa_reconfig(struct wpa_authenticator *wpa_auth,
		 struct wpa_auth_config *conf)
{
	if (wpa_auth == NULL)
		return 0;

	memcpy(&wpa_auth->conf, conf, sizeof(*conf));
	/*
	 * TODO:
	 * Disassociate stations if configuration changed
	 * Update WPA/RSN IE
	 */
	return 0;
}


static int wpa_selector_to_bitfield(u8 *s)
{
	if (memcmp(s, WPA_CIPHER_SUITE_NONE, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_NONE;
	if (memcmp(s, WPA_CIPHER_SUITE_WEP40, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP40;
	if (memcmp(s, WPA_CIPHER_SUITE_TKIP, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_TKIP;
	if (memcmp(s, WPA_CIPHER_SUITE_CCMP, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_CCMP;
	if (memcmp(s, WPA_CIPHER_SUITE_WEP104, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP104;
	return 0;
}


static int wpa_key_mgmt_to_bitfield(u8 *s)
{
	if (memcmp(s, WPA_AUTH_KEY_MGMT_UNSPEC_802_1X, WPA_SELECTOR_LEN) == 0)
		return WPA_KEY_MGMT_IEEE8021X;
	if (memcmp(s, WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X, WPA_SELECTOR_LEN) ==
	    0)
		return WPA_KEY_MGMT_PSK;
	return 0;
}


static int rsn_selector_to_bitfield(u8 *s)
{
	if (memcmp(s, RSN_CIPHER_SUITE_NONE, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_NONE;
	if (memcmp(s, RSN_CIPHER_SUITE_WEP40, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP40;
	if (memcmp(s, RSN_CIPHER_SUITE_TKIP, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_TKIP;
	if (memcmp(s, RSN_CIPHER_SUITE_CCMP, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_CCMP;
	if (memcmp(s, RSN_CIPHER_SUITE_WEP104, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP104;
	return 0;
}


static int rsn_key_mgmt_to_bitfield(u8 *s)
{
	if (memcmp(s, RSN_AUTH_KEY_MGMT_UNSPEC_802_1X, RSN_SELECTOR_LEN) == 0)
		return WPA_KEY_MGMT_IEEE8021X;
	if (memcmp(s, RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X, RSN_SELECTOR_LEN) ==
	    0)
		return WPA_KEY_MGMT_PSK;
	return 0;
}


struct wpa_ie_data {
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	size_t num_pmkid;
	u8 *pmkid;
};


static int wpa_parse_wpa_ie_wpa(const u8 *wpa_ie, size_t wpa_ie_len,
				struct wpa_ie_data *data)
{
	struct wpa_ie_hdr *hdr;
	u8 *pos;
	int left;
	int i, count;

	memset(data, 0, sizeof(*data));
	data->pairwise_cipher = WPA_CIPHER_TKIP;
	data->group_cipher = WPA_CIPHER_TKIP;
	data->key_mgmt = WPA_KEY_MGMT_IEEE8021X;

	if (wpa_ie_len < sizeof(struct wpa_ie_hdr))
		return -1;

	hdr = (struct wpa_ie_hdr *) wpa_ie;

	if (hdr->elem_id != WLAN_EID_GENERIC ||
	    hdr->len != wpa_ie_len - 2 ||
	    memcmp(&hdr->oui, WPA_OUI_TYPE, WPA_SELECTOR_LEN) != 0 ||
	    le_to_host16(hdr->version) != WPA_VERSION) {
		return -2;
	}

	pos = (u8 *) (hdr + 1);
	left = wpa_ie_len - sizeof(*hdr);

	if (left >= WPA_SELECTOR_LEN) {
		data->group_cipher = wpa_selector_to_bitfield(pos);
		pos += WPA_SELECTOR_LEN;
		left -= WPA_SELECTOR_LEN;
	} else if (left > 0)
		  return -3;

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * WPA_SELECTOR_LEN)
			return -4;
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= wpa_selector_to_bitfield(pos);
			pos += WPA_SELECTOR_LEN;
			left -= WPA_SELECTOR_LEN;
		}
	} else if (left == 1)
		return -5;

	if (left >= 2) {
		data->key_mgmt = 0;
		count = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * WPA_SELECTOR_LEN)
			return -6;
		for (i = 0; i < count; i++) {
			data->key_mgmt |= wpa_key_mgmt_to_bitfield(pos);
			pos += WPA_SELECTOR_LEN;
			left -= WPA_SELECTOR_LEN;
		}
	} else if (left == 1)
		return -7;

	if (left >= 2) {
		data->capabilities = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
	}

	if (left > 0) {
		return -8;
	}

	return 0;
}


static int wpa_parse_wpa_ie_rsn(const u8 *rsn_ie, size_t rsn_ie_len,
				struct wpa_ie_data *data)
{
	struct rsn_ie_hdr *hdr;
	u8 *pos;
	int left;
	int i, count;

	memset(data, 0, sizeof(*data));
	data->pairwise_cipher = WPA_CIPHER_CCMP;
	data->group_cipher = WPA_CIPHER_CCMP;
	data->key_mgmt = WPA_KEY_MGMT_IEEE8021X;

	if (rsn_ie_len < sizeof(struct rsn_ie_hdr))
		return -1;

	hdr = (struct rsn_ie_hdr *) rsn_ie;

	if (hdr->elem_id != WLAN_EID_RSN ||
	    hdr->len != rsn_ie_len - 2 ||
	    le_to_host16(hdr->version) != RSN_VERSION) {
		return -2;
	}

	pos = (u8 *) (hdr + 1);
	left = rsn_ie_len - sizeof(*hdr);

	if (left >= RSN_SELECTOR_LEN) {
		data->group_cipher = rsn_selector_to_bitfield(pos);
		pos += RSN_SELECTOR_LEN;
		left -= RSN_SELECTOR_LEN;
	} else if (left > 0)
		  return -3;

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * RSN_SELECTOR_LEN)
			return -4;
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= rsn_selector_to_bitfield(pos);
			pos += RSN_SELECTOR_LEN;
			left -= RSN_SELECTOR_LEN;
		}
	} else if (left == 1)
		return -5;

	if (left >= 2) {
		data->key_mgmt = 0;
		count = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * RSN_SELECTOR_LEN)
			return -6;
		for (i = 0; i < count; i++) {
			data->key_mgmt |= rsn_key_mgmt_to_bitfield(pos);
			pos += RSN_SELECTOR_LEN;
			left -= RSN_SELECTOR_LEN;
		}
	} else if (left == 1)
		return -7;

	if (left >= 2) {
		data->capabilities = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
	}

	if (left >= 2) {
		data->num_pmkid = pos[0] | (pos[1] << 8);
		pos += 2;
		left -= 2;
		if (left < data->num_pmkid * PMKID_LEN) {
			wpa_printf(MSG_DEBUG, "RSN: too short RSN IE for "
				   "PMKIDs (num=%lu, left=%d)",
				   (unsigned long) data->num_pmkid, left);
			return -9;
		}
		data->pmkid = pos;
		pos += data->num_pmkid * PMKID_LEN;
		left -= data->num_pmkid * PMKID_LEN;
	}

	if (left > 0) {
		return -8;
	}

	return 0;
}


int wpa_validate_wpa_ie(struct wpa_authenticator *wpa_auth,
			struct wpa_state_machine *sm,
			const u8 *wpa_ie, size_t wpa_ie_len)
{
	struct wpa_ie_data data;
	int ciphers, key_mgmt, res, i, version;
	const u8 *selector;

	if (wpa_auth == NULL || sm == NULL)
		return WPA_NOT_ENABLED;

	if (wpa_ie == NULL || wpa_ie_len < 1)
		return WPA_INVALID_IE;

	if (wpa_ie[0] == WLAN_EID_RSN)
		version = HOSTAPD_WPA_VERSION_WPA2;
	else
		version = HOSTAPD_WPA_VERSION_WPA;

	if (version == HOSTAPD_WPA_VERSION_WPA2) {
		res = wpa_parse_wpa_ie_rsn(wpa_ie, wpa_ie_len, &data);

		selector = RSN_AUTH_KEY_MGMT_UNSPEC_802_1X;
		if (data.key_mgmt & WPA_KEY_MGMT_IEEE8021X)
			selector = RSN_AUTH_KEY_MGMT_UNSPEC_802_1X;
		else if (data.key_mgmt & WPA_KEY_MGMT_PSK)
			selector = RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X;
		memcpy(wpa_auth->dot11RSNAAuthenticationSuiteSelected,
		       selector, RSN_SELECTOR_LEN);

		selector = RSN_CIPHER_SUITE_CCMP;
		if (data.pairwise_cipher & WPA_CIPHER_CCMP)
			selector = RSN_CIPHER_SUITE_CCMP;
		else if (data.pairwise_cipher & WPA_CIPHER_TKIP)
			selector = RSN_CIPHER_SUITE_TKIP;
		else if (data.pairwise_cipher & WPA_CIPHER_WEP104)
			selector = RSN_CIPHER_SUITE_WEP104;
		else if (data.pairwise_cipher & WPA_CIPHER_WEP40)
			selector = RSN_CIPHER_SUITE_WEP40;
		else if (data.pairwise_cipher & WPA_CIPHER_NONE)
			selector = RSN_CIPHER_SUITE_NONE;
		memcpy(wpa_auth->dot11RSNAPairwiseCipherSelected,
		       selector, RSN_SELECTOR_LEN);

		selector = RSN_CIPHER_SUITE_CCMP;
		if (data.group_cipher & WPA_CIPHER_CCMP)
			selector = RSN_CIPHER_SUITE_CCMP;
		else if (data.group_cipher & WPA_CIPHER_TKIP)
			selector = RSN_CIPHER_SUITE_TKIP;
		else if (data.group_cipher & WPA_CIPHER_WEP104)
			selector = RSN_CIPHER_SUITE_WEP104;
		else if (data.group_cipher & WPA_CIPHER_WEP40)
			selector = RSN_CIPHER_SUITE_WEP40;
		else if (data.group_cipher & WPA_CIPHER_NONE)
			selector = RSN_CIPHER_SUITE_NONE;
		memcpy(wpa_auth->dot11RSNAGroupCipherSelected,
		       selector, RSN_SELECTOR_LEN);
	} else {
		res = wpa_parse_wpa_ie_wpa(wpa_ie, wpa_ie_len, &data);

		selector = WPA_AUTH_KEY_MGMT_UNSPEC_802_1X;
		if (data.key_mgmt & WPA_KEY_MGMT_IEEE8021X)
			selector = WPA_AUTH_KEY_MGMT_UNSPEC_802_1X;
		else if (data.key_mgmt & WPA_KEY_MGMT_PSK)
			selector = WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X;
		memcpy(wpa_auth->dot11RSNAAuthenticationSuiteSelected,
		       selector, WPA_SELECTOR_LEN);

		selector = WPA_CIPHER_SUITE_TKIP;
		if (data.pairwise_cipher & WPA_CIPHER_CCMP)
			selector = WPA_CIPHER_SUITE_CCMP;
		else if (data.pairwise_cipher & WPA_CIPHER_TKIP)
			selector = WPA_CIPHER_SUITE_TKIP;
		else if (data.pairwise_cipher & WPA_CIPHER_WEP104)
			selector = WPA_CIPHER_SUITE_WEP104;
		else if (data.pairwise_cipher & WPA_CIPHER_WEP40)
			selector = WPA_CIPHER_SUITE_WEP40;
		else if (data.pairwise_cipher & WPA_CIPHER_NONE)
			selector = WPA_CIPHER_SUITE_NONE;
		memcpy(wpa_auth->dot11RSNAPairwiseCipherSelected,
		       selector, WPA_SELECTOR_LEN);

		selector = WPA_CIPHER_SUITE_TKIP;
		if (data.group_cipher & WPA_CIPHER_CCMP)
			selector = WPA_CIPHER_SUITE_CCMP;
		else if (data.group_cipher & WPA_CIPHER_TKIP)
			selector = WPA_CIPHER_SUITE_TKIP;
		else if (data.group_cipher & WPA_CIPHER_WEP104)
			selector = WPA_CIPHER_SUITE_WEP104;
		else if (data.group_cipher & WPA_CIPHER_WEP40)
			selector = WPA_CIPHER_SUITE_WEP40;
		else if (data.group_cipher & WPA_CIPHER_NONE)
			selector = WPA_CIPHER_SUITE_NONE;
		memcpy(wpa_auth->dot11RSNAGroupCipherSelected,
		       selector, WPA_SELECTOR_LEN);
	}
	if (res) {
		wpa_printf(MSG_DEBUG, "Failed to parse WPA/RSN IE from "
			   MACSTR " (res=%d)", MAC2STR(sm->addr), res);
		wpa_hexdump(MSG_DEBUG, "WPA/RSN IE", wpa_ie, wpa_ie_len);
		return WPA_INVALID_IE;
	}

	if (data.group_cipher != wpa_auth->conf.wpa_group) {
		wpa_printf(MSG_DEBUG, "Invalid WPA group cipher (0x%x) from "
			   MACSTR, data.group_cipher, MAC2STR(sm->addr));
		return WPA_INVALID_GROUP;
	}

	key_mgmt = data.key_mgmt & wpa_auth->conf.wpa_key_mgmt;
	if (!key_mgmt) {
		wpa_printf(MSG_DEBUG, "Invalid WPA key mgmt (0x%x) from "
			   MACSTR, data.key_mgmt, MAC2STR(sm->addr));
		return WPA_INVALID_AKMP;
	}
	if (key_mgmt & WPA_KEY_MGMT_IEEE8021X)
		sm->wpa_key_mgmt = WPA_KEY_MGMT_IEEE8021X;
	else
		sm->wpa_key_mgmt = WPA_KEY_MGMT_PSK;

	ciphers = data.pairwise_cipher & wpa_auth->conf.wpa_pairwise;
	if (!ciphers) {
		wpa_printf(MSG_DEBUG, "Invalid WPA pairwise cipher (0x%x) "
			   "from " MACSTR,
			   data.pairwise_cipher, MAC2STR(sm->addr));
		return WPA_INVALID_PAIRWISE;
	}

	if (ciphers & WPA_CIPHER_CCMP)
		sm->pairwise = WPA_CIPHER_CCMP;
	else
		sm->pairwise = WPA_CIPHER_TKIP;

	/* TODO: clear WPA/WPA2 state if STA changes from one to another */
	if (wpa_ie[0] == WLAN_EID_RSN)
		sm->wpa = WPA_VERSION_WPA2;
	else
		sm->wpa = WPA_VERSION_WPA;

	for (i = 0; i < data.num_pmkid; i++) {
		wpa_hexdump(MSG_DEBUG, "RSN IE: STA PMKID",
			    &data.pmkid[i * PMKID_LEN], PMKID_LEN);
		sm->pmksa = pmksa_cache_get(wpa_auth->pmksa, sm->addr,
					    &data.pmkid[i * PMKID_LEN]);
		if (sm->pmksa) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_DEBUG,
					"PMKID found from PMKSA cache");
			memcpy(wpa_auth->dot11RSNAPMKIDUsed,
			       sm->pmksa->pmkid, PMKID_LEN);
			break;
		}
	}

	if (sm->wpa_ie == NULL || sm->wpa_ie_len < wpa_ie_len) {
		free(sm->wpa_ie);
		sm->wpa_ie = malloc(wpa_ie_len);
		if (sm->wpa_ie == NULL)
			return WPA_ALLOC_FAIL;
	}
	memcpy(sm->wpa_ie, wpa_ie, wpa_ie_len);
	sm->wpa_ie_len = wpa_ie_len;

	return WPA_IE_OK;
}


struct wpa_eapol_ie_parse {
	const u8 *wpa_ie;
	size_t wpa_ie_len;
	const u8 *rsn_ie;
	size_t rsn_ie_len;
	const u8 *pmkid;
	const u8 *gtk;
	size_t gtk_len;
	const u8 *stakey;
	size_t stakey_len;
	const u8 *mac_addr;
	size_t mac_addr_len;
};


/**
 * wpa_parse_generic - Parse EAPOL-Key Key Data Generic IEs
 * @pos: Pointer to the IE header
 * @end: Pointer to the end of the Key Data buffer
 * @ie: Pointer to parsed IE data
 * Returns: 0 on success, 1 if end mark is found, -1 on failure
 */
static int wpa_parse_generic(const u8 *pos, const u8 *end,
			     struct wpa_eapol_ie_parse *ie)
{
	if (pos[1] == 0)
		return 1;

	if (pos[1] >= 6 &&
	    memcmp(pos + 2, WPA_OUI_TYPE, WPA_SELECTOR_LEN) == 0 &&
	    pos[2 + WPA_SELECTOR_LEN] == 1 &&
	    pos[2 + WPA_SELECTOR_LEN + 1] == 0) {
		ie->wpa_ie = pos;
		ie->wpa_ie_len = pos[1] + 2;
		return 0;
	}

	if (pos + 1 + RSN_SELECTOR_LEN < end &&
	    pos[1] >= RSN_SELECTOR_LEN + PMKID_LEN &&
	    memcmp(pos + 2, RSN_KEY_DATA_PMKID, RSN_SELECTOR_LEN) == 0) {
		ie->pmkid = pos + 2 + RSN_SELECTOR_LEN;
		return 0;
	}

	if (pos[1] > RSN_SELECTOR_LEN + 2 &&
	    memcmp(pos + 2, RSN_KEY_DATA_GROUPKEY, RSN_SELECTOR_LEN) == 0) {
		ie->gtk = pos + 2 + RSN_SELECTOR_LEN;
		ie->gtk_len = pos[1] - RSN_SELECTOR_LEN;
		return 0;
	}

	if (pos[1] > RSN_SELECTOR_LEN + 2 &&
	    memcmp(pos + 2, RSN_KEY_DATA_STAKEY, RSN_SELECTOR_LEN) == 0) {
		ie->stakey = pos + 2 + RSN_SELECTOR_LEN;
		ie->stakey_len = pos[1] - RSN_SELECTOR_LEN;
		return 0;
	}

	if (pos[1] > RSN_SELECTOR_LEN + 2 &&
	    memcmp(pos + 2, RSN_KEY_DATA_MAC_ADDR, RSN_SELECTOR_LEN) == 0) {
		ie->mac_addr = pos + 2 + RSN_SELECTOR_LEN;
		ie->mac_addr_len = pos[1] - RSN_SELECTOR_LEN;
		return 0;
	}

	return 0;
}


/**
 * wpa_parse_kde_ies - Parse EAPOL-Key Key Data IEs
 * @buf: Pointer to the Key Data buffer
 * @len: Key Data Length
 * @ie: Pointer to parsed IE data
 * Returns: 0 on success, -1 on failure
 */
static int wpa_parse_kde_ies(const u8 *buf, size_t len,
			     struct wpa_eapol_ie_parse *ie)
{
	const u8 *pos, *end;
	int ret = 0;

	memset(ie, 0, sizeof(*ie));
	for (pos = buf, end = pos + len; pos + 1 < end; pos += 2 + pos[1]) {
		if (pos[0] == 0xdd &&
		    ((pos == buf + len - 1) || pos[1] == 0)) {
			/* Ignore padding */
			break;
		}
		if (pos + 2 + pos[1] > end) {
			wpa_printf(MSG_DEBUG, "WPA: EAPOL-Key Key Data "
				   "underflow (ie=%d len=%d)", pos[0], pos[1]);
			ret = -1;
			break;
		}
		if (*pos == RSN_INFO_ELEM) {
			ie->rsn_ie = pos;
			ie->rsn_ie_len = pos[1] + 2;
		} else if (*pos == GENERIC_INFO_ELEM) {
			ret = wpa_parse_generic(pos, end, ie);
			if (ret < 0)
				break;
			if (ret > 0) {
				ret = 0;
				break;
			}
		} else {
			wpa_hexdump(MSG_DEBUG, "WPA: Unrecognized EAPOL-Key "
				    "Key Data IE", pos, 2 + pos[1]);
		}
	}

	return ret;
}


struct wpa_state_machine *
wpa_auth_sta_init(struct wpa_authenticator *wpa_auth, const u8 *addr)
{
	struct wpa_state_machine *sm;

	sm = wpa_zalloc(sizeof(struct wpa_state_machine));
	if (sm == NULL)
		return NULL;
	memcpy(sm->addr, addr, ETH_ALEN);

	sm->wpa_auth = wpa_auth;

	return sm;
}


void wpa_auth_sta_associated(struct wpa_authenticator *wpa_auth,
			     struct wpa_state_machine *sm)
{
	if (wpa_auth == NULL || !wpa_auth->conf.wpa || sm == NULL)
		return;

	if (sm->started) {
		memset(sm->key_replay_counter, 0, WPA_REPLAY_COUNTER_LEN);
		sm->ReAuthenticationRequest = TRUE;
		wpa_sm_step(sm);
		return;
	}

	wpa_auth_logger(wpa_auth, sm->addr, LOGGER_DEBUG,
			"start authentication");
	sm->started = 1;

	sm->Init = TRUE;
	wpa_sm_step(sm);
	sm->Init = FALSE;
	sm->AuthenticationRequest = TRUE;
	wpa_sm_step(sm);
}


static void wpa_free_sta_sm(struct wpa_state_machine *sm)
{
	free(sm->last_rx_eapol_key);
	free(sm->wpa_ie);
	free(sm);
}


void wpa_auth_sta_deinit(struct wpa_state_machine *sm)
{
	if (sm == NULL)
		return;

	if (sm->wpa_auth->conf.wpa_strict_rekey && sm->has_GTK) {
		wpa_auth_logger(sm->wpa_auth, sm->addr, LOGGER_DEBUG,
				"strict rekeying - force GTK rekey since STA "
				"is leaving");
		eloop_cancel_timeout(wpa_rekey_gtk, sm->wpa_auth, NULL);
		eloop_register_timeout(0, 500000, wpa_rekey_gtk, sm->wpa_auth,
				       NULL);
	}

	eloop_cancel_timeout(wpa_send_eapol_timeout, sm->wpa_auth, sm);
	eloop_cancel_timeout(wpa_sm_call_step, sm, NULL);
	if (sm->in_step_loop) {
		/* Must not free state machine while wpa_sm_step() is running.
		 * Freeing will be completed in the end of wpa_sm_step(). */
		wpa_printf(MSG_DEBUG, "WPA: Registering pending STA state "
			   "machine deinit for " MACSTR, MAC2STR(sm->addr));
		sm->pending_deinit = 1;
	} else
		wpa_free_sta_sm(sm);
}


static void wpa_request_new_ptk(struct wpa_state_machine *sm)
{
	if (sm == NULL)
		return;

	sm->PTKRequest = TRUE;
	sm->PTK_valid = 0;
}


#ifdef CONFIG_STAKEY
static struct wpa_stakey_negotiation *
wpa_stakey_get(struct wpa_authenticator *wpa_auth, const u8 *addr1,
	       const u8 *addr2)
{
	struct wpa_stakey_negotiation *neg;
	if (wpa_auth == NULL)
		return NULL;
	neg = wpa_auth->stakey_negotiations;
	while (neg) {
		if ((memcmp(neg->initiator, addr1, ETH_ALEN) == 0 &&
		     memcmp(neg->peer, addr2, ETH_ALEN) == 0) ||
		    (memcmp(neg->initiator, addr2, ETH_ALEN) == 0 &&
		     memcmp(neg->peer, addr1, ETH_ALEN) == 0))
			return neg;
		neg = neg->next;
	}
	return NULL;
}


struct wpa_stakey_search {
	const u8 *addr;
	struct wpa_state_machine *sm;
};


static int wpa_stakey_select_sta(struct wpa_state_machine *sm, void *ctx)
{
	struct wpa_stakey_search *search = ctx;
	if (memcmp(search->addr, sm->addr, ETH_ALEN) == 0) {
		search->sm = sm;
		return 1;
	}
	return 0;
}


static void wpa_stakey_step(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_authenticator *wpa_auth = eloop_ctx;
	struct wpa_stakey_negotiation *neg = timeout_ctx;
	const u8 *dst, *peer;
	struct rsn_stakey_kde kde;
	int version;
	struct wpa_stakey_search search;

	dst = neg->state == PEER ? neg->peer : neg->initiator;
	peer = neg->state == PEER ? neg->initiator : neg->peer;
	search.addr = dst;
	search.sm = NULL;
	if (wpa_auth_for_each_sta(wpa_auth, wpa_stakey_select_sta, &search) ==
	    0 || search.sm == NULL) {
		wpa_printf(MSG_DEBUG, "RSN: STAKey handshake with " MACSTR
			   " aborted - STA not associated anymore",
			   MAC2STR(dst));
		wpa_stakey_remove(wpa_auth, neg);
		return;
	}

	neg->num_retries++;
	if (neg->num_retries > dot11RSNAConfigPairwiseUpdateCount) {
		wpa_printf(MSG_DEBUG, "RSN: STAKey handshake with " MACSTR
			   " timed out", MAC2STR(dst));
		wpa_stakey_remove(wpa_auth, neg);
		return;
	}

	wpa_printf(MSG_DEBUG, "RSN: Sending STAKey 1/2 to " MACSTR " (try=%d)",
		   MAC2STR(dst), neg->num_retries);
	memset(&kde, 0, sizeof(kde));
	kde.id = GENERIC_INFO_ELEM;
	kde.len = sizeof(kde) - 2 - 32 + neg->key_len;
	memcpy(kde.oui, RSN_KEY_DATA_STAKEY, RSN_SELECTOR_LEN);
	memcpy(kde.mac_addr, peer, ETH_ALEN);
	memcpy(kde.stakey, neg->key, neg->key_len);

	if (neg->alg == WPA_CIPHER_CCMP)
		version = WPA_KEY_INFO_TYPE_HMAC_SHA1_AES;
	else
		version = WPA_KEY_INFO_TYPE_HMAC_MD5_RC4;

	__wpa_send_eapol(wpa_auth, search.sm, 1, 1, 1, 0, 0, NULL,
			 NULL /* nonce */,
			 (u8 *) &kde, sizeof(kde) - 32 + neg->key_len,
			 NULL, 0, 0, 1, version);

	eloop_register_timeout(dot11RSNAConfigPairwiseUpdateTimeOut / 1000,
			       (dot11RSNAConfigPairwiseUpdateTimeOut % 1000) *
			       1000, wpa_stakey_step, wpa_auth, neg);
}
#endif /* CONFIG_STAKEY */


static int wpa_stakey_remove(struct wpa_authenticator *wpa_auth,
			     struct wpa_stakey_negotiation *neg)
{
#ifdef CONFIG_STAKEY
	struct wpa_stakey_negotiation *pos, *prev;

	if (wpa_auth == NULL)
		return -1;
	pos = wpa_auth->stakey_negotiations;
	prev = NULL;
	while (pos) {
		if (pos == neg) {
			if (prev)
				prev->next = pos->next;
			else
				wpa_auth->stakey_negotiations = pos->next;

			eloop_cancel_timeout(wpa_stakey_step, wpa_auth, pos);
			free(pos);
			return 0;
		}
		prev = pos;
		pos = pos->next;
	}
#endif /* CONFIG_STAKEY */

	return -1;
}


#ifdef CONFIG_STAKEY
static int wpa_stakey_initiate(struct wpa_authenticator *wpa_auth,
			       const u8 *initiator, const u8 *peer, int alg)
{
	struct wpa_stakey_negotiation *neg;

	if (wpa_auth == NULL)
		return -1;

	neg = wpa_stakey_get(wpa_auth, initiator, peer);
	if (neg) {
		wpa_printf(MSG_DEBUG, "RSN: Pending STAKey handshake in "
			   "progress - ignoring new request");
		return -1;
	}

	neg = wpa_zalloc(sizeof(*neg));
	if (neg == NULL)
		return -1;
	memcpy(neg->initiator, initiator, ETH_ALEN);
	memcpy(neg->peer, peer, ETH_ALEN);
	neg->state = PEER;
	neg->alg = alg;
	if (alg == WPA_CIPHER_TKIP)
		neg->key_len = 32;
	else
		neg->key_len = 16;
	if (hostapd_get_rand(neg->key, neg->key_len)) {
		wpa_printf(MSG_DEBUG, "RSN: Failed to get random data for "
			   "STAKey");
		free(neg);
		return -1;
	}

	neg->next = wpa_auth->stakey_negotiations;
	wpa_auth->stakey_negotiations = neg;
	wpa_stakey_step(wpa_auth, neg);

	return 0;
}
#endif /* CONFIG_STAKEY */


static void wpa_stakey_receive(struct wpa_authenticator *wpa_auth,
			       const u8 *src_addr, const u8 *peer)
{
#ifdef CONFIG_STAKEY
	struct wpa_stakey_negotiation *neg;

	neg = wpa_stakey_get(wpa_auth, src_addr, peer);
	if (neg == NULL) {
		wpa_printf(MSG_DEBUG, "RSN: No matching STAKey negotiation "
			   "found - ignored received STAKey frame");
		return;
	}

	if (neg->state == PEER &&
	    memcmp(src_addr, neg->peer, ETH_ALEN) == 0) {
		neg->state = INITIATOR;
		neg->num_retries = 0;
		wpa_printf(MSG_DEBUG, "RSN: STAKey completed with peer "
			   MACSTR, MAC2STR(neg->peer));
		eloop_cancel_timeout(wpa_stakey_step, wpa_auth, neg);
		wpa_stakey_step(wpa_auth, neg);
	} else if (neg->state == INITIATOR &&
		   memcmp(src_addr, neg->initiator, ETH_ALEN) == 0) {
		wpa_printf(MSG_DEBUG, "RSN: STAKey negotiation completed "
			   MACSTR " <-> " MACSTR,
			   MAC2STR(neg->initiator), MAC2STR(neg->peer));
		wpa_stakey_remove(wpa_auth, neg);
	} else {
		wpa_printf(MSG_DEBUG, "RSN: Unexpected STAKey message - "
			   "src " MACSTR " peer " MACSTR " - dropped",
			   MAC2STR(src_addr), MAC2STR(peer));
	}
#endif /* CONFIG_STAKEY */
}


void wpa_receive(struct wpa_authenticator *wpa_auth,
		 struct wpa_state_machine *sm,
		 u8 *data, size_t data_len)
{
	struct ieee802_1x_hdr *hdr;
	struct wpa_eapol_key *key;
	u16 key_info, key_data_length;
	enum { PAIRWISE_2, PAIRWISE_4, GROUP_2, STAKEY_2, REQUEST } msg;
	char *msgtxt;
	struct wpa_eapol_ie_parse kde;
	const u8 *mac_addr = NULL;

	if (wpa_auth == NULL || !wpa_auth->conf.wpa || sm == NULL)
		return;

	if (data_len < sizeof(*hdr) + sizeof(*key))
		return;

	hdr = (struct ieee802_1x_hdr *) data;
	key = (struct wpa_eapol_key *) (hdr + 1);
	key_info = ntohs(key->key_info);
	key_data_length = ntohs(key->key_data_length);
	if (key_data_length > data_len - sizeof(*hdr) - sizeof(*key)) {
		wpa_printf(MSG_INFO, "WPA: Invalid EAPOL-Key frame - "
			   "key_data overflow (%d > %lu)",
			   key_data_length,
			   (unsigned long) (data_len - sizeof(*hdr) -
					    sizeof(*key)));
		return;
	}

	/* FIX: verify that the EAPOL-Key frame was encrypted if pairwise keys
	 * are set */

	if (key_info & WPA_KEY_INFO_REQUEST) {
		msg = REQUEST;
		msgtxt = "Request";
	} else if (!(key_info & WPA_KEY_INFO_KEY_TYPE)) {
		/* FIX: should decrypt key_data if encrypted */
		if (key_data_length > 0 &&
		    wpa_parse_kde_ies((const u8 *) (key + 1),
				      key_data_length, &kde) == 0 &&
		    kde.mac_addr) {
			msg = STAKEY_2;
			msgtxt = "2/2 STAKey";
			mac_addr = kde.mac_addr;
		} else {
			msg = GROUP_2;
			msgtxt = "2/2 Group";
		}
	} else if (key_data_length == 0) {
		msg = PAIRWISE_4;
		msgtxt = "4/4 Pairwise";
	} else {
		msg = PAIRWISE_2;
		msgtxt = "2/4 Pairwise";
	}

	if (key_info & WPA_KEY_INFO_REQUEST) {
		if (sm->req_replay_counter_used &&
		    memcmp(key->replay_counter, sm->req_replay_counter,
			   WPA_REPLAY_COUNTER_LEN) <= 0) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_WARNING,
					"received EAPOL-Key request with "
					"replayed counter");
			return;
		}
	}

	if (!(key_info & WPA_KEY_INFO_REQUEST) &&
	    (!sm->key_replay_counter_valid ||
	     memcmp(key->replay_counter, sm->key_replay_counter,
		    WPA_REPLAY_COUNTER_LEN) != 0)) {
		wpa_auth_vlogger(wpa_auth, sm->addr, LOGGER_INFO,
				 "received EAPOL-Key %s with unexpected "
				 "replay counter", msgtxt);
		wpa_hexdump(MSG_DEBUG, "expected replay counter",
			    sm->key_replay_counter, WPA_REPLAY_COUNTER_LEN);
		wpa_hexdump(MSG_DEBUG, "received replay counter",
			    key->replay_counter, WPA_REPLAY_COUNTER_LEN);
		return;
	}

	switch (msg) {
	case PAIRWISE_2:
		if (sm->wpa_ptk_state != WPA_PTK_PTKSTART &&
		    sm->wpa_ptk_state != WPA_PTK_PTKCALCNEGOTIATING) {
			wpa_auth_vlogger(wpa_auth, sm->addr, LOGGER_INFO,
					 "received EAPOL-Key msg 2/4 in "
					 "invalid state (%d) - dropped",
					 sm->wpa_ptk_state);
			return;
		}
		if (sm->wpa_ie == NULL ||
		    sm->wpa_ie_len != key_data_length ||
		    memcmp(sm->wpa_ie, key + 1, key_data_length) != 0) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"WPA IE from (Re)AssocReq did not "
					"match with msg 2/4");
			if (sm->wpa_ie) {
				wpa_hexdump(MSG_DEBUG, "WPA IE in AssocReq",
					    sm->wpa_ie, sm->wpa_ie_len);
			}
			wpa_hexdump(MSG_DEBUG, "WPA IE in msg 2/4",
				    (u8 *) (key + 1), key_data_length);
			/* MLME-DEAUTHENTICATE.request */
			wpa_sta_disconnect(wpa_auth, sm->addr);
			return;
		}
		break;
	case PAIRWISE_4:
		if (sm->wpa_ptk_state != WPA_PTK_PTKINITNEGOTIATING ||
		    !sm->PTK_valid) {
			wpa_auth_vlogger(wpa_auth, sm->addr, LOGGER_INFO,
					 "received EAPOL-Key msg 4/4 in "
					 "invalid state (%d) - dropped",
					 sm->wpa_ptk_state);
			return;
		}
		break;
	case GROUP_2:
		if (sm->wpa_ptk_group_state != WPA_PTK_GROUP_REKEYNEGOTIATING
		    || !sm->PTK_valid) {
			wpa_auth_vlogger(wpa_auth, sm->addr, LOGGER_INFO,
					 "received EAPOL-Key msg 2/2 in "
					 "invalid state (%d) - dropped",
					 sm->wpa_ptk_group_state);
			return;
		}
		break;
	case STAKEY_2:
		if (!sm->PTK_valid) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"received EAPOL-Key msg STAKey 2/2 in "
					"invalid state - dropped");
			return;
		}
		break;
	case REQUEST:
		break;
	}

	wpa_auth_vlogger(wpa_auth, sm->addr, LOGGER_DEBUG,
			 "received EAPOL-Key frame (%s)", msgtxt);

	if (key_info & WPA_KEY_INFO_ACK) {
		wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
				"received invalid EAPOL-Key: Key Ack set");
		return;
	}

	if (!(key_info & WPA_KEY_INFO_MIC)) {
		wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
				"received invalid EAPOL-Key: Key MIC not set");
		return;
	}

	sm->MICVerified = FALSE;
	if (sm->PTK_valid) {
		if (wpa_verify_key_mic(&sm->PTK, data, data_len)) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"received EAPOL-Key with invalid MIC");
			return;
		}
		sm->MICVerified = TRUE;
		eloop_cancel_timeout(wpa_send_eapol_timeout, wpa_auth, sm);
	}

	if (key_info & WPA_KEY_INFO_REQUEST) {
		if (sm->MICVerified) {
			sm->req_replay_counter_used = 1;
			memcpy(sm->req_replay_counter, key->replay_counter,
			       WPA_REPLAY_COUNTER_LEN);
		} else {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"received EAPOL-Key request with "
					"invalid MIC");
			return;
		}

		/*
		 * TODO: should decrypt key data field if encryption was used;
		 * even though MAC address KDE is not normally encrypted,
		 * supplicant is allowed to encrypt it.
		 */
		if (key_info & WPA_KEY_INFO_ERROR) {
			/* Supplicant reported a Michael MIC error */
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"received EAPOL-Key Error Request "
					"(STA detected Michael MIC failure)");
			wpa_auth_mic_failure_report(wpa_auth, sm->addr);
			sm->dot11RSNAStatsTKIPRemoteMICFailures++;
			wpa_auth->dot11RSNAStatsTKIPRemoteMICFailures++;
			/* Error report is not a request for a new key
			 * handshake, but since Authenticator may do it, let's
			 * change the keys now anyway. */
			wpa_request_new_ptk(sm);
		} else if (key_info & WPA_KEY_INFO_KEY_TYPE) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"received EAPOL-Key Request for new "
					"4-Way Handshake");
			wpa_request_new_ptk(sm);
		} else if (key_data_length > 0 &&
			   wpa_parse_kde_ies((const u8 *) (key + 1),
					     key_data_length, &kde) == 0 &&
			   kde.mac_addr) {
#ifdef CONFIG_STAKEY
			/* STAKey Request */
			if (!wpa_auth->conf.stakey) {
				wpa_printf(MSG_DEBUG, "RSN: STAKey Request, "
					   "but STAKey use disabled - ignoring"
					   " request");
			} else if (kde.mac_addr_len != ETH_ALEN) {
				wpa_printf(MSG_DEBUG, "RSN: Invalid MAC "
					   "address KDE length %d",
					   kde.mac_addr_len);
			} else {
				int alg;
				wpa_printf(MSG_DEBUG, "RSN: STAKey Request for"
					   " peer " MACSTR,
					   MAC2STR(kde.mac_addr));
				switch (key_info & WPA_KEY_INFO_TYPE_MASK) {
				case 1:
					alg = WPA_CIPHER_TKIP;
					break;
				case 2:
					alg = WPA_CIPHER_CCMP;
					break;
				default:
					wpa_printf(MSG_DEBUG, "Unexpected "
						   "STAKey key version (%d)",
						   key_info &
						   WPA_KEY_INFO_TYPE_MASK);
					alg = WPA_CIPHER_NONE;
					break;
				}
				if (alg != WPA_CIPHER_NONE) {
					wpa_stakey_initiate(wpa_auth, sm->addr,
							    kde.mac_addr, alg);
				}
			}
#endif /* CONFIG_STAKEY */
		} else {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_INFO,
					"received EAPOL-Key Request for GTK "
					"rekeying");
			/* FIX: why was this triggering PTK rekeying for the
			 * STA that requested Group Key rekeying?? */
			/* wpa_request_new_ptk(sta->wpa_sm); */
			eloop_cancel_timeout(wpa_rekey_gtk, wpa_auth, NULL);
			wpa_rekey_gtk(wpa_auth, NULL);
		}
	} else {
		/* Do not allow the same key replay counter to be reused. */
		sm->key_replay_counter_valid = FALSE;
	}

	if (msg == STAKEY_2) {
		wpa_stakey_receive(wpa_auth, sm->addr, mac_addr);
		return;
	}

	free(sm->last_rx_eapol_key);
	sm->last_rx_eapol_key = malloc(data_len);
	if (sm->last_rx_eapol_key == NULL)
		return;
	memcpy(sm->last_rx_eapol_key, data, data_len);
	sm->last_rx_eapol_key_len = data_len;

	sm->EAPOLKeyReceived = TRUE;
	sm->EAPOLKeyPairwise = !!(key_info & WPA_KEY_INFO_KEY_TYPE);
	sm->EAPOLKeyRequest = !!(key_info & WPA_KEY_INFO_REQUEST);
	memcpy(sm->SNonce, key->key_nonce, WPA_NONCE_LEN);
	wpa_sm_step(sm);
}


static void wpa_pmk_to_ptk(const u8 *pmk, const u8 *addr1, const u8 *addr2,
			   const u8 *nonce1, const u8 *nonce2,
			   u8 *ptk, size_t ptk_len)
{
	u8 data[2 * ETH_ALEN + 2 * WPA_NONCE_LEN];

	/* PTK = PRF-X(PMK, "Pairwise key expansion",
	 *             Min(AA, SA) || Max(AA, SA) ||
	 *             Min(ANonce, SNonce) || Max(ANonce, SNonce)) */

	if (memcmp(addr1, addr2, ETH_ALEN) < 0) {
		memcpy(data, addr1, ETH_ALEN);
		memcpy(data + ETH_ALEN, addr2, ETH_ALEN);
	} else {
		memcpy(data, addr2, ETH_ALEN);
		memcpy(data + ETH_ALEN, addr1, ETH_ALEN);
	}

	if (memcmp(nonce1, nonce2, WPA_NONCE_LEN) < 0) {
		memcpy(data + 2 * ETH_ALEN, nonce1, WPA_NONCE_LEN);
		memcpy(data + 2 * ETH_ALEN + WPA_NONCE_LEN, nonce2,
		       WPA_NONCE_LEN);
	} else {
		memcpy(data + 2 * ETH_ALEN, nonce2, WPA_NONCE_LEN);
		memcpy(data + 2 * ETH_ALEN + WPA_NONCE_LEN, nonce1,
		       WPA_NONCE_LEN);
	}

	sha1_prf(pmk, WPA_PMK_LEN, "Pairwise key expansion",
		 data, sizeof(data), ptk, ptk_len);

	wpa_hexdump_key(MSG_DEBUG, "PMK", pmk, WPA_PMK_LEN);
	wpa_hexdump_key(MSG_DEBUG, "PTK", ptk, ptk_len);
}


static void wpa_gmk_to_gtk(const u8 *gmk, const u8 *addr, const u8 *gnonce,
			   u8 *gtk, size_t gtk_len)
{
	u8 data[ETH_ALEN + WPA_NONCE_LEN];

	/* GTK = PRF-X(GMK, "Group key expansion", AA || GNonce) */
	memcpy(data, addr, ETH_ALEN);
	memcpy(data + ETH_ALEN, gnonce, WPA_NONCE_LEN);

	sha1_prf(gmk, WPA_GMK_LEN, "Group key expansion",
		 data, sizeof(data), gtk, gtk_len);

	wpa_hexdump_key(MSG_DEBUG, "GMK", gmk, WPA_GMK_LEN);
	wpa_hexdump_key(MSG_DEBUG, "GTK", gtk, gtk_len);
}


static void wpa_send_eapol_timeout(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_authenticator *wpa_auth = eloop_ctx;
	struct wpa_state_machine *sm = timeout_ctx;

	wpa_auth_logger(wpa_auth, sm->addr, LOGGER_DEBUG, "EAPOL-Key timeout");
	sm->TimeoutEvt = TRUE;
	wpa_sm_step(sm);
}


static int wpa_calc_eapol_key_mic(int ver, u8 *key, u8 *data, size_t len,
				  u8 *mic)
{
	u8 hash[SHA1_MAC_LEN];

	switch (ver) {
	case WPA_KEY_INFO_TYPE_HMAC_MD5_RC4:
		hmac_md5(key, 16, data, len, mic);
		break;
	case WPA_KEY_INFO_TYPE_HMAC_SHA1_AES:
		hmac_sha1(key, 16, data, len, hash);
		memcpy(mic, hash, MD5_MAC_LEN);
		break;
	default:
		return -1;
	}
	return 0;
}


static void __wpa_send_eapol(struct wpa_authenticator *wpa_auth,
			     struct wpa_state_machine *sm,
			     int secure, int mic, int ack, int install,
			     int pairwise, u8 *key_rsc, u8 *nonce,
			     u8 *ie, size_t ie_len, u8 *gtk, size_t gtk_len,
			     int keyidx, int encr, int force_version)
{
	struct ieee802_1x_hdr *hdr;
	struct wpa_eapol_key *key;
	size_t len;
	int key_info, alg;
	int key_data_len, pad_len = 0;
	u8 *buf, *pos;
	int version;

	len = sizeof(struct ieee802_1x_hdr) + sizeof(struct wpa_eapol_key);

	if (force_version)
		version = force_version;
	else if (sm->pairwise == WPA_CIPHER_CCMP)
		version = WPA_KEY_INFO_TYPE_HMAC_SHA1_AES;
	else
		version = WPA_KEY_INFO_TYPE_HMAC_MD5_RC4;

	wpa_printf(MSG_DEBUG, "WPA: Send EAPOL(secure=%d mic=%d ack=%d "
		   "install=%d pairwise=%d ie_len=%lu gtk_len=%lu keyidx=%d "
		   "encr=%d)",
		   secure, mic, ack, install, pairwise, (unsigned long) ie_len,
		   (unsigned long) gtk_len, keyidx, encr);

	key_data_len = ie_len + gtk_len;

	if (sm->wpa == WPA_VERSION_WPA2 && gtk_len)
		key_data_len += 2 + RSN_SELECTOR_LEN + 2;

	if (version == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES) {
		pad_len = key_data_len % 8;
		if (pad_len)
			pad_len = 8 - pad_len;
		key_data_len += pad_len + 8;
	}

	len += key_data_len;

	hdr = wpa_zalloc(len);
	if (hdr == NULL)
		return;
	hdr->version = wpa_auth->conf.eapol_version;
	hdr->type = IEEE802_1X_TYPE_EAPOL_KEY;
	hdr->length = htons(len  - sizeof(*hdr));
	key = (struct wpa_eapol_key *) (hdr + 1);

	key->type = sm->wpa == WPA_VERSION_WPA2 ?
		EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;
	key_info = version;
	if (secure)
		key_info |= WPA_KEY_INFO_SECURE;
	if (mic)
		key_info |= WPA_KEY_INFO_MIC;
	if (ack)
		key_info |= WPA_KEY_INFO_ACK;
	if (install)
		key_info |= WPA_KEY_INFO_INSTALL;
	if (pairwise)
		key_info |= WPA_KEY_INFO_KEY_TYPE;
	if (encr && sm->wpa == WPA_VERSION_WPA2)
		key_info |= WPA_KEY_INFO_ENCR_KEY_DATA;
	if (sm->wpa != WPA_VERSION_WPA2)
		key_info |= keyidx << WPA_KEY_INFO_KEY_INDEX_SHIFT;
	key->key_info = htons(key_info);

	alg = pairwise ? sm->pairwise : wpa_auth->conf.wpa_group;
	switch (alg) {
	case WPA_CIPHER_CCMP:
		key->key_length = htons(16);
		break;
	case WPA_CIPHER_TKIP:
		key->key_length = htons(32);
		break;
	case WPA_CIPHER_WEP40:
		key->key_length = htons(5);
		break;
	case WPA_CIPHER_WEP104:
		key->key_length = htons(13);
		break;
	}

	inc_byte_array(sm->key_replay_counter, WPA_REPLAY_COUNTER_LEN);
	memcpy(key->replay_counter, sm->key_replay_counter,
	       WPA_REPLAY_COUNTER_LEN);
	sm->key_replay_counter_valid = TRUE;

	if (nonce)
		memcpy(key->key_nonce, nonce, WPA_NONCE_LEN);

	if (key_rsc)
		memcpy(key->key_rsc, key_rsc, WPA_KEY_RSC_LEN);

	if (ie && !encr) {
		memcpy(key + 1, ie, ie_len);
		key->key_data_length = htons(ie_len);
	} else if (encr && (gtk || ie)) {
		buf = wpa_zalloc(key_data_len);
		if (buf == NULL) {
			free(hdr);
			return;
		}
		pos = buf;
		if (ie) {
			memcpy(pos, ie, ie_len);
			pos += ie_len;
		}

		if (gtk) {
			if (sm->wpa == WPA_VERSION_WPA2) {
				*pos++ = WLAN_EID_GENERIC;
				*pos++ = RSN_SELECTOR_LEN + 2 + gtk_len;
				memcpy(pos, RSN_KEY_DATA_GROUPKEY,
				       RSN_SELECTOR_LEN);
				pos += RSN_SELECTOR_LEN;
				*pos++ = keyidx & 0x03;
				*pos++ = 0;
			}

			memcpy(pos, gtk, gtk_len);
			pos += gtk_len;
		}

		if (pad_len)
			*pos++ = 0xdd;

		wpa_hexdump_key(MSG_DEBUG, "Plaintext EAPOL-Key Key Data",
				buf, key_data_len);
		if (version == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES) {
			aes_wrap(sm->PTK.encr_key, (key_data_len - 8) / 8, buf,
				 (u8 *) (key + 1));
			key->key_data_length = htons(key_data_len);
		} else {
			u8 ek[32];
			memcpy(key->key_iv,
			       wpa_auth->Counter + WPA_NONCE_LEN - 16, 16);
			inc_byte_array(wpa_auth->Counter, WPA_NONCE_LEN);
			memcpy(ek, key->key_iv, 16);
			memcpy(ek + 16, sm->PTK.encr_key, 16);
			memcpy(key + 1, buf, key_data_len);
			rc4_skip(ek, 32, 256, (u8 *) (key + 1), key_data_len);
			key->key_data_length = htons(key_data_len);
		}
		free(buf);
	}

	if (mic) {
		if (!sm->PTK_valid) {
			wpa_auth_logger(wpa_auth, sm->addr, LOGGER_DEBUG,
					"PTK not valid when sending EAPOL-Key "
					"frame");
			free(hdr);
			return;
		}
		wpa_calc_eapol_key_mic(version,
				       sm->PTK.mic_key, (u8 *) hdr, len,
				       key->key_mic);
	}

	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_inc_EapolFramesTx,
			   1);
	wpa_auth_send_eapol(wpa_auth, sm->addr, (u8 *) hdr, len,
			    sm->pairwise_set);
	free(hdr);
}


static void wpa_send_eapol(struct wpa_authenticator *wpa_auth,
			   struct wpa_state_machine *sm,
			   int secure, int mic, int ack, int install,
			   int pairwise, u8 *key_rsc, u8 *nonce,
			   u8 *ie, size_t ie_len, u8 *gtk, size_t gtk_len,
			   int keyidx)
{
	int timeout_ms;

	if (sm == NULL)
		return;

	__wpa_send_eapol(wpa_auth, sm, secure, mic, ack, install, pairwise,
			 key_rsc, nonce, ie, ie_len, gtk, gtk_len, keyidx,
			 gtk ? 1 : 0, 0);

	timeout_ms = pairwise ? dot11RSNAConfigPairwiseUpdateTimeOut :
		dot11RSNAConfigGroupUpdateTimeOut;
	eloop_register_timeout(timeout_ms / 1000, (timeout_ms % 1000) * 1000,
			       wpa_send_eapol_timeout, wpa_auth, sm);
}


static int wpa_verify_key_mic(struct wpa_ptk *PTK, u8 *data, size_t data_len)
{
	struct ieee802_1x_hdr *hdr;
	struct wpa_eapol_key *key;
	u16 key_info;
	int type, ret = 0;
	u8 mic[16];

	if (data_len < sizeof(*hdr) + sizeof(*key))
		return -1;

	hdr = (struct ieee802_1x_hdr *) data;
	key = (struct wpa_eapol_key *) (hdr + 1);
	key_info = ntohs(key->key_info);
	type = key_info & WPA_KEY_INFO_TYPE_MASK;
	memcpy(mic, key->key_mic, 16);
	memset(key->key_mic, 0, 16);
	if (wpa_calc_eapol_key_mic(key_info & WPA_KEY_INFO_TYPE_MASK,
				   PTK->mic_key, data, data_len, key->key_mic)
	    || memcmp(mic, key->key_mic, 16) != 0)
		ret = -1;
	memcpy(key->key_mic, mic, 16);
	return ret;
}


void wpa_auth_sm_event(struct wpa_state_machine *sm, wpa_event event)
{
	if (sm == NULL)
		return;

	wpa_auth_vlogger(sm->wpa_auth, sm->addr, LOGGER_DEBUG,
			 "event %d notification", event);

	switch (event) {
	case WPA_AUTH:
	case WPA_ASSOC:
		break;
	case WPA_DEAUTH:
	case WPA_DISASSOC:
		sm->DeauthenticationRequest = TRUE;
		break;
	case WPA_REAUTH:
	case WPA_REAUTH_EAPOL:
		sm->ReAuthenticationRequest = TRUE;
		break;
	}

	sm->PTK_valid = FALSE;
	memset(&sm->PTK, 0, sizeof(sm->PTK));

	if (event != WPA_REAUTH_EAPOL) {
		sm->pairwise_set = FALSE;
		wpa_auth_set_key(sm->wpa_auth, "none", sm->addr, 0,
				 (u8 *) "", 0);
	}

	wpa_sm_step(sm);
}


static const char * wpa_alg_txt(int alg)
{
	switch (alg) {
	case WPA_CIPHER_CCMP:
		return "CCMP";
	case WPA_CIPHER_TKIP:
		return "TKIP";
	case WPA_CIPHER_WEP104:
	case WPA_CIPHER_WEP40:
		return "WEP";
	default:
		return "";
	}
}


SM_STATE(WPA_PTK, INITIALIZE)
{
	SM_ENTRY_MA(WPA_PTK, INITIALIZE, wpa_ptk);
	if (sm->Init) {
		/* Init flag is not cleared here, so avoid busy
		 * loop by claiming nothing changed. */
		sm->changed = FALSE;
	}

	sm->keycount = 0;
	if (sm->GUpdateStationKeys)
		sm->wpa_auth->GKeyDoneStations--;
	sm->GUpdateStationKeys = FALSE;
	if (sm->wpa == WPA_VERSION_WPA)
		sm->PInitAKeys = FALSE;
	if (1 /* Unicast cipher supported AND (ESS OR ((IBSS or WDS) and
	       * Local AA > Remote AA)) */) {
		sm->Pair = TRUE;
	}
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_portEnabled, 0);
	wpa_auth_set_key(sm->wpa_auth, "none", sm->addr, 0, (u8 *) "", 0);
	sm->pairwise_set = FALSE;
	sm->PTK_valid = FALSE;
	memset(&sm->PTK, 0, sizeof(sm->PTK));
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_portValid, 0);
	sm->TimeoutCtr = 0;
	if (sm->wpa_key_mgmt == WPA_KEY_MGMT_PSK) {
		wpa_auth_set_eapol(sm->wpa_auth, sm->addr,
				   WPA_EAPOL_authorized, 0);
	}
}


SM_STATE(WPA_PTK, DISCONNECT)
{
	SM_ENTRY_MA(WPA_PTK, DISCONNECT, wpa_ptk);
	sm->Disconnect = FALSE;
	wpa_sta_disconnect(sm->wpa_auth, sm->addr);
}


SM_STATE(WPA_PTK, DISCONNECTED)
{
	SM_ENTRY_MA(WPA_PTK, DISCONNECTED, wpa_ptk);
	sm->wpa_auth->GNoStations--;
	sm->DeauthenticationRequest = FALSE;
}


SM_STATE(WPA_PTK, AUTHENTICATION)
{
	SM_ENTRY_MA(WPA_PTK, AUTHENTICATION, wpa_ptk);
	sm->wpa_auth->GNoStations++;
	memset(&sm->PTK, 0, sizeof(sm->PTK));
	sm->PTK_valid = FALSE;
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_portControl_Auto,
			   1);
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_portEnabled, 1);
	sm->AuthenticationRequest = FALSE;
}


SM_STATE(WPA_PTK, AUTHENTICATION2)
{
	SM_ENTRY_MA(WPA_PTK, AUTHENTICATION2, wpa_ptk);
	memcpy(sm->ANonce, sm->wpa_auth->Counter, WPA_NONCE_LEN);
	inc_byte_array(sm->wpa_auth->Counter, WPA_NONCE_LEN);
	sm->ReAuthenticationRequest = FALSE;
	/* IEEE 802.11i does not clear TimeoutCtr here, but this is more
	 * logical place than INITIALIZE since AUTHENTICATION2 can be
	 * re-entered on ReAuthenticationRequest without going through
	 * INITIALIZE. */
	sm->TimeoutCtr = 0;
}


SM_STATE(WPA_PTK, INITPMK)
{
	size_t len = WPA_PMK_LEN;

	SM_ENTRY_MA(WPA_PTK, INITPMK, wpa_ptk);
	if (sm->pmksa) {
		wpa_printf(MSG_DEBUG, "WPA: PMK from PMKSA cache");
		memcpy(sm->PMK, sm->pmksa->pmk, WPA_PMK_LEN);
	} else if (wpa_auth_get_pmk(sm->wpa_auth, sm->addr, sm->PMK, &len) ==
		   0) {
		wpa_printf(MSG_DEBUG, "WPA: PMK from EAPOL state machine "
			   "(len=%lu)", (unsigned long) len);
	} else {
		wpa_printf(MSG_DEBUG, "WPA: Could not get PMK");
	}

	sm->req_replay_counter_used = 0;
	/* IEEE 802.11i does not set keyRun to FALSE, but not doing this
	 * will break reauthentication since EAPOL state machines may not be
	 * get into AUTHENTICATING state that clears keyRun before WPA state
	 * machine enters AUTHENTICATION2 state and goes immediately to INITPMK
	 * state and takes PMK from the previously used AAA Key. This will
	 * eventually fail in 4-Way Handshake because Supplicant uses PMK
	 * derived from the new AAA Key. Setting keyRun = FALSE here seems to
	 * be good workaround for this issue. */
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_keyRun, 0);
}


SM_STATE(WPA_PTK, INITPSK)
{
	const u8 *psk;
	SM_ENTRY_MA(WPA_PTK, INITPSK, wpa_ptk);
	psk = wpa_auth_get_psk(sm->wpa_auth, sm->addr, NULL);
	if (psk)
		memcpy(sm->PMK, psk, WPA_PMK_LEN);
	sm->req_replay_counter_used = 0;
}


SM_STATE(WPA_PTK, PTKSTART)
{
	u8 *pmkid = NULL;
	size_t pmkid_len = 0;

	SM_ENTRY_MA(WPA_PTK, PTKSTART, wpa_ptk);
	sm->PTKRequest = FALSE;
	sm->TimeoutEvt = FALSE;
	wpa_auth_logger(sm->wpa_auth, sm->addr, LOGGER_DEBUG,
			"sending 1/4 msg of 4-Way Handshake");
	if (sm->pmksa &&
	    (pmkid = malloc(2 + RSN_SELECTOR_LEN + PMKID_LEN))) {
		pmkid_len = 2 + RSN_SELECTOR_LEN + PMKID_LEN;
		pmkid[0] = WLAN_EID_GENERIC;
		pmkid[1] = RSN_SELECTOR_LEN + PMKID_LEN;
		memcpy(&pmkid[2], RSN_KEY_DATA_PMKID, RSN_SELECTOR_LEN);
		memcpy(&pmkid[2 + RSN_SELECTOR_LEN], sm->pmksa->pmkid,
		       PMKID_LEN);
	}
	wpa_send_eapol(sm->wpa_auth, sm, 0, 0, 1, 0, 1, NULL, sm->ANonce,
		       pmkid, pmkid_len, NULL, 0, 0);
	free(pmkid);
	sm->TimeoutCtr++;
}


SM_STATE(WPA_PTK, PTKCALCNEGOTIATING)
{
	struct wpa_ptk PTK;
	int ok = 0;
	const u8 *pmk = NULL;

	SM_ENTRY_MA(WPA_PTK, PTKCALCNEGOTIATING, wpa_ptk);
	sm->EAPOLKeyReceived = FALSE;

	/* WPA with IEEE 802.1X: use the derived PMK from EAP
	 * WPA-PSK: iterate through possible PSKs and select the one matching
	 * the packet */
	for (;;) {
		if (sm->wpa_key_mgmt == WPA_KEY_MGMT_PSK) {
			pmk = wpa_auth_get_psk(sm->wpa_auth, sm->addr, pmk);
			if (pmk == NULL)
				break;
		} else
			pmk = sm->PMK;

		wpa_pmk_to_ptk(pmk, sm->wpa_auth->addr, sm->addr,
			       sm->ANonce, sm->SNonce,
			       (u8 *) &PTK, sizeof(PTK));

		if (wpa_verify_key_mic(&PTK, sm->last_rx_eapol_key,
				       sm->last_rx_eapol_key_len) == 0) {
			ok = 1;
			break;
		}

		if (sm->wpa_key_mgmt != WPA_KEY_MGMT_PSK)
			break;
	}

	if (!ok) {
		wpa_auth_logger(sm->wpa_auth, sm->addr, LOGGER_DEBUG,
				"invalid MIC in msg 2/4 of 4-Way Handshake");
		return;
	}

	eloop_cancel_timeout(wpa_send_eapol_timeout, sm->wpa_auth, sm);

	if (sm->wpa_key_mgmt == WPA_KEY_MGMT_PSK) {
		/* PSK may have changed from the previous choice, so update
		 * state machine data based on whatever PSK was selected here.
		 */
		memcpy(sm->PMK, pmk, WPA_PMK_LEN);
	}

	sm->MICVerified = TRUE;

	memcpy(&sm->PTK, &PTK, sizeof(PTK));
	sm->PTK_valid = TRUE;
}


SM_STATE(WPA_PTK, PTKCALCNEGOTIATING2)
{
	SM_ENTRY_MA(WPA_PTK, PTKCALCNEGOTIATING2, wpa_ptk);
	sm->TimeoutCtr = 0;
}


SM_STATE(WPA_PTK, PTKINITNEGOTIATING)
{
	u8 rsc[WPA_KEY_RSC_LEN], *_rsc, *gtk;
	size_t gtk_len;
	struct wpa_authenticator *gsm = sm->wpa_auth;
	u8 *wpa_ie;
	int wpa_ie_len, secure, keyidx;

	SM_ENTRY_MA(WPA_PTK, PTKINITNEGOTIATING, wpa_ptk);
	sm->TimeoutEvt = FALSE;
	/* Send EAPOL(1, 1, 1, Pair, P, RSC, ANonce, MIC(PTK), RSNIE, GTK[GN])
	 */
	memset(rsc, 0, WPA_KEY_RSC_LEN);
	wpa_auth_get_seqnum(sm->wpa_auth, NULL, gsm->GN, rsc);
	wpa_ie = gsm->wpa_ie;
	wpa_ie_len = gsm->wpa_ie_len;
	if (sm->wpa == WPA_VERSION_WPA &&
	    (sm->wpa_auth->conf.wpa & HOSTAPD_WPA_VERSION_WPA2) &&
	    wpa_ie_len > wpa_ie[1] + 2 && wpa_ie[0] == WLAN_EID_RSN) {
		/* WPA-only STA, remove RSN IE */
		wpa_ie = wpa_ie + wpa_ie[1] + 2;
		wpa_ie_len = wpa_ie[1] + 2;
	}
	wpa_auth_logger(sm->wpa_auth, sm->addr, LOGGER_DEBUG,
			"sending 3/4 msg of 4-Way Handshake");
	if (sm->wpa == WPA_VERSION_WPA2) {
		/* WPA2 send GTK in the 4-way handshake */
		secure = 1;
		gtk = gsm->GTK[gsm->GN - 1];
		gtk_len = gsm->GTK_len;
		keyidx = gsm->GN;
		_rsc = rsc;
	} else {
		/* WPA does not include GTK in msg 3/4 */
		secure = 0;
		gtk = NULL;
		gtk_len = 0;
		keyidx = 0;
		_rsc = NULL;
	}

	wpa_send_eapol(sm->wpa_auth, sm, secure, 1, 1, 1, 1, _rsc, sm->ANonce,
		       wpa_ie, wpa_ie_len, gtk, gtk_len, keyidx);
	sm->TimeoutCtr++;
}


SM_STATE(WPA_PTK, PTKINITDONE)
{
	SM_ENTRY_MA(WPA_PTK, PTKINITDONE, wpa_ptk);
	sm->EAPOLKeyReceived = FALSE;
	if (sm->Pair) {
		char *alg;
		int klen;
		if (sm->pairwise == WPA_CIPHER_TKIP) {
			alg = "TKIP";
			klen = 32;
		} else {
			alg = "CCMP";
			klen = 16;
		}
		if (wpa_auth_set_key(sm->wpa_auth, alg, sm->addr, 0,
				     sm->PTK.tk1, klen)) {
			wpa_sta_disconnect(sm->wpa_auth, sm->addr);
			return;
		}
		/* FIX: MLME-SetProtection.Request(TA, Tx_Rx) */
		sm->pairwise_set = TRUE;

		if (sm->wpa_key_mgmt == WPA_KEY_MGMT_PSK) {
			wpa_auth_set_eapol(sm->wpa_auth, sm->addr,
					   WPA_EAPOL_authorized, 1);
		}
	}

	if (0 /* IBSS == TRUE */) {
		sm->keycount++;
		if (sm->keycount == 2) {
			wpa_auth_set_eapol(sm->wpa_auth, sm->addr,
					   WPA_EAPOL_portValid, 1);
		}
	} else {
		wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_portValid,
				   1);
	}
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_keyAvailable, 0);
	wpa_auth_set_eapol(sm->wpa_auth, sm->addr, WPA_EAPOL_keyDone, 1);
	if (sm->wpa == WPA_VERSION_WPA)
		sm->PInitAKeys = TRUE;
	else
		sm->has_GTK = TRUE;
	wpa_auth_vlogger(sm->wpa_auth, sm->addr, LOGGER_INFO,
			 "pairwise key handshake completed (%s)",
			 sm->wpa == WPA_VERSION_WPA ? "WPA" : "RSN");
}


SM_STEP(WPA_PTK)
{
	struct wpa_authenticator *wpa_auth = sm->wpa_auth;

	if (sm->Init)
		SM_ENTER(WPA_PTK, INITIALIZE);
	else if (sm->Disconnect
		 /* || FIX: dot11RSNAConfigSALifetime timeout */)
		SM_ENTER(WPA_PTK, DISCONNECT);
	else if (sm->DeauthenticationRequest)
		SM_ENTER(WPA_PTK, DISCONNECTED);
	else if (sm->AuthenticationRequest)
		SM_ENTER(WPA_PTK, AUTHENTICATION);
	else if (sm->ReAuthenticationRequest)
		SM_ENTER(WPA_PTK, AUTHENTICATION2);
	else if (sm->PTKRequest)
		SM_ENTER(WPA_PTK, PTKSTART);
	else switch (sm->wpa_ptk_state) {
	case WPA_PTK_INITIALIZE:
		break;
	case WPA_PTK_DISCONNECT:
		SM_ENTER(WPA_PTK, DISCONNECTED);
		break;
	case WPA_PTK_DISCONNECTED:
		SM_ENTER(WPA_PTK, INITIALIZE);
		break;
	case WPA_PTK_AUTHENTICATION:
		SM_ENTER(WPA_PTK, AUTHENTICATION2);
		break;
	case WPA_PTK_AUTHENTICATION2:
		if ((sm->wpa_key_mgmt == WPA_KEY_MGMT_IEEE8021X) &&
		    wpa_auth_get_eapol(sm->wpa_auth, sm->addr,
				       WPA_EAPOL_keyRun) > 0)
			SM_ENTER(WPA_PTK, INITPMK);
		else if ((sm->wpa_key_mgmt == WPA_KEY_MGMT_PSK)
			 /* FIX: && 802.1X::keyRun */)
			SM_ENTER(WPA_PTK, INITPSK);
		break;
	case WPA_PTK_INITPMK:
		if (wpa_auth_get_eapol(sm->wpa_auth, sm->addr,
				       WPA_EAPOL_keyAvailable) > 0)
			SM_ENTER(WPA_PTK, PTKSTART);
		else {
			wpa_auth->dot11RSNA4WayHandshakeFailures++;
			SM_ENTER(WPA_PTK, DISCONNECT);
		}
		break;
	case WPA_PTK_INITPSK:
		if (wpa_auth_get_psk(sm->wpa_auth, sm->addr, NULL))
			SM_ENTER(WPA_PTK, PTKSTART);
		else {
			wpa_auth_logger(sm->wpa_auth, sm->addr, LOGGER_INFO,
					"no PSK configured for the STA");
			wpa_auth->dot11RSNA4WayHandshakeFailures++;
			SM_ENTER(WPA_PTK, DISCONNECT);
		}
		break;
	case WPA_PTK_PTKSTART:
		if (sm->EAPOLKeyReceived && !sm->EAPOLKeyRequest &&
		    sm->EAPOLKeyPairwise)
			SM_ENTER(WPA_PTK, PTKCALCNEGOTIATING);
		else if (sm->TimeoutCtr > dot11RSNAConfigPairwiseUpdateCount) {
			wpa_auth->dot11RSNA4WayHandshakeFailures++;
			SM_ENTER(WPA_PTK, DISCONNECT);
		} else if (sm->TimeoutEvt)
			SM_ENTER(WPA_PTK, PTKSTART);
		break;
	case WPA_PTK_PTKCALCNEGOTIATING:
		if (sm->MICVerified)
			SM_ENTER(WPA_PTK, PTKCALCNEGOTIATING2);
		else if (sm->EAPOLKeyReceived && !sm->EAPOLKeyRequest &&
			 sm->EAPOLKeyPairwise)
			SM_ENTER(WPA_PTK, PTKCALCNEGOTIATING);
		else if (sm->TimeoutEvt)
			SM_ENTER(WPA_PTK, PTKSTART);
		break;
	case WPA_PTK_PTKCALCNEGOTIATING2:
		SM_ENTER(WPA_PTK, PTKINITNEGOTIATING);
		break;
	case WPA_PTK_PTKINITNEGOTIATING:
		if (sm->EAPOLKeyReceived && !sm->EAPOLKeyRequest &&
		    sm->EAPOLKeyPairwise && sm->MICVerified)
			SM_ENTER(WPA_PTK, PTKINITDONE);
		else if (sm->TimeoutCtr > dot11RSNAConfigPairwiseUpdateCount) {
			wpa_auth->dot11RSNA4WayHandshakeFailures++;
			SM_ENTER(WPA_PTK, DISCONNECT);
		} else if (sm->TimeoutEvt)
			SM_ENTER(WPA_PTK, PTKINITNEGOTIATING);
		break;
	case WPA_PTK_PTKINITDONE:
		break;
	}
}


SM_STATE(WPA_PTK_GROUP, IDLE)
{
	SM_ENTRY_MA(WPA_PTK_GROUP, IDLE, wpa_ptk_group);
	if (sm->Init) {
		/* Init flag is not cleared here, so avoid busy
		 * loop by claiming nothing changed. */
		sm->changed = FALSE;
	}
	sm->GTimeoutCtr = 0;
}


SM_STATE(WPA_PTK_GROUP, REKEYNEGOTIATING)
{
	u8 rsc[WPA_KEY_RSC_LEN];
	struct wpa_authenticator *gsm = sm->wpa_auth;

	SM_ENTRY_MA(WPA_PTK_GROUP, REKEYNEGOTIATING, wpa_ptk_group);
	if (sm->wpa == WPA_VERSION_WPA)
		sm->PInitAKeys = FALSE;
	sm->TimeoutEvt = FALSE;
	/* Send EAPOL(1, 1, 1, !Pair, G, RSC, GNonce, MIC(PTK), GTK[GN]) */
	memset(rsc, 0, WPA_KEY_RSC_LEN);
	if (gsm->wpa_group_state == WPA_GROUP_SETKEYSDONE)
		wpa_auth_get_seqnum(sm->wpa_auth, NULL, gsm->GN, rsc);
	wpa_auth_logger(sm->wpa_auth, sm->addr, LOGGER_DEBUG,
			"sending 1/2 msg of Group Key Handshake");
	wpa_send_eapol(gsm, sm, 1, 1, 1, !sm->Pair, 0, rsc, gsm->GNonce,
		       NULL, 0, gsm->GTK[gsm->GN - 1], gsm->GTK_len, gsm->GN);
	sm->GTimeoutCtr++;
}


SM_STATE(WPA_PTK_GROUP, REKEYESTABLISHED)
{
	SM_ENTRY_MA(WPA_PTK_GROUP, REKEYESTABLISHED, wpa_ptk_group);
	sm->EAPOLKeyReceived = FALSE;
	sm->GUpdateStationKeys = FALSE;
	sm->wpa_auth->GKeyDoneStations--;
	sm->GTimeoutCtr = 0;
	/* FIX: MLME.SetProtection.Request(TA, Tx_Rx) */
	wpa_auth_vlogger(sm->wpa_auth, sm->addr, LOGGER_INFO,
			 "group key handshake completed (%s)",
			 sm->wpa == WPA_VERSION_WPA ? "WPA" : "RSN");
	sm->has_GTK = TRUE;
}


SM_STATE(WPA_PTK_GROUP, KEYERROR)
{
	SM_ENTRY_MA(WPA_PTK_GROUP, KEYERROR, wpa_ptk_group);
	sm->wpa_auth->GKeyDoneStations--;
	sm->GUpdateStationKeys = FALSE;
	sm->Disconnect = TRUE;
}


SM_STEP(WPA_PTK_GROUP)
{
	if (sm->Init)
		SM_ENTER(WPA_PTK_GROUP, IDLE);
	else switch (sm->wpa_ptk_group_state) {
	case WPA_PTK_GROUP_IDLE:
		if (sm->GUpdateStationKeys ||
		    (sm->wpa == WPA_VERSION_WPA && sm->PInitAKeys))
			SM_ENTER(WPA_PTK_GROUP, REKEYNEGOTIATING);
		break;
	case WPA_PTK_GROUP_REKEYNEGOTIATING:
		if (sm->EAPOLKeyReceived && !sm->EAPOLKeyRequest &&
		    !sm->EAPOLKeyPairwise && sm->MICVerified)
			SM_ENTER(WPA_PTK_GROUP, REKEYESTABLISHED);
		else if (sm->GTimeoutCtr > dot11RSNAConfigGroupUpdateCount)
			SM_ENTER(WPA_PTK_GROUP, KEYERROR);
		else if (sm->TimeoutEvt)
			SM_ENTER(WPA_PTK_GROUP, REKEYNEGOTIATING);
		break;
	case WPA_PTK_GROUP_KEYERROR:
		SM_ENTER(WPA_PTK_GROUP, IDLE);
		break;
	case WPA_PTK_GROUP_REKEYESTABLISHED:
		SM_ENTER(WPA_PTK_GROUP, IDLE);
		break;
	}
}


static void wpa_group_gtk_init(struct wpa_authenticator *wpa_auth)
{
	wpa_printf(MSG_DEBUG, "WPA: group state machine entering state "
		   "GTK_INIT");
	wpa_auth->changed = FALSE; /* GInit is not cleared here; avoid loop */
	wpa_auth->wpa_group_state = WPA_GROUP_GTK_INIT;

	/* GTK[0..N] = 0 */
	memset(wpa_auth->GTK, 0, sizeof(wpa_auth->GTK));
	wpa_auth->GN = 1;
	wpa_auth->GM = 2;
	/* GTK[GN] = CalcGTK() */
	/* FIX: is this the correct way of getting GNonce? */
	memcpy(wpa_auth->GNonce, wpa_auth->Counter, WPA_NONCE_LEN);
	inc_byte_array(wpa_auth->Counter, WPA_NONCE_LEN);
	wpa_gmk_to_gtk(wpa_auth->GMK, wpa_auth->addr, wpa_auth->GNonce,
		       wpa_auth->GTK[wpa_auth->GN - 1], wpa_auth->GTK_len);
}


static int wpa_group_update_sta(struct wpa_state_machine *sm, void *ctx)
{
	sm->GUpdateStationKeys = TRUE;
	wpa_sm_step(sm);
	return 0;
}


static void wpa_group_setkeys(struct wpa_authenticator *wpa_auth)
{
	int tmp;

	wpa_printf(MSG_DEBUG, "WPA: group state machine entering state "
		   "SETKEYS");
	wpa_auth->changed = TRUE;
	wpa_auth->wpa_group_state = WPA_GROUP_SETKEYS;
	wpa_auth->GTKReKey = FALSE;
	tmp = wpa_auth->GM;
	wpa_auth->GM = wpa_auth->GN;
	wpa_auth->GN = tmp;
	wpa_auth->GKeyDoneStations = wpa_auth->GNoStations;
	/* FIX: is this the correct way of getting GNonce? */
	memcpy(wpa_auth->GNonce, wpa_auth->Counter, WPA_NONCE_LEN);
	inc_byte_array(wpa_auth->Counter, WPA_NONCE_LEN);
	wpa_gmk_to_gtk(wpa_auth->GMK, wpa_auth->addr, wpa_auth->GNonce,
		       wpa_auth->GTK[wpa_auth->GN - 1], wpa_auth->GTK_len);

	wpa_auth_for_each_sta(wpa_auth, wpa_group_update_sta, NULL);
}


static void wpa_group_setkeysdone(struct wpa_authenticator *wpa_auth)
{
	wpa_printf(MSG_DEBUG, "WPA: group state machine entering state "
		   "SETKEYSDONE");
	wpa_auth->changed = TRUE;
	wpa_auth->wpa_group_state = WPA_GROUP_SETKEYSDONE;
	wpa_auth_set_key(wpa_auth, wpa_alg_txt(wpa_auth->conf.wpa_group),
			 NULL, wpa_auth->GN, wpa_auth->GTK[wpa_auth->GN - 1],
			 wpa_auth->GTK_len);
}


static void wpa_group_sm_step(struct wpa_authenticator *wpa_auth)
{
	if (wpa_auth->GInit) {
		wpa_group_gtk_init(wpa_auth);
	} else if (wpa_auth->wpa_group_state == WPA_GROUP_GTK_INIT &&
		   wpa_auth->GTKAuthenticator) {
		wpa_group_setkeysdone(wpa_auth);
	} else if (wpa_auth->wpa_group_state == WPA_GROUP_SETKEYSDONE &&
		   wpa_auth->GTKReKey) {
		wpa_group_setkeys(wpa_auth);
	} else if (wpa_auth->wpa_group_state == WPA_GROUP_SETKEYS) {
		if (wpa_auth->GKeyDoneStations == 0)
			wpa_group_setkeysdone(wpa_auth);
		else if (wpa_auth->GTKReKey)
			wpa_group_setkeys(wpa_auth);
	}
}


static void wpa_sm_step(struct wpa_state_machine *sm)
{
	if (sm == NULL)
		return;

	if (sm->in_step_loop) {
		/* This should not happen, but if it does, make sure we do not
		 * end up freeing the state machine too early by exiting the
		 * recursive call. */
		wpa_printf(MSG_ERROR, "WPA: wpa_sm_step() called recursively");
		return;
	}

	sm->in_step_loop = 1;
	do {
		if (sm->pending_deinit)
			break;

		sm->changed = FALSE;
		sm->wpa_auth->changed = FALSE;

		SM_STEP_RUN(WPA_PTK);
		if (sm->pending_deinit)
			break;
		SM_STEP_RUN(WPA_PTK_GROUP);
		if (sm->pending_deinit)
			break;
		wpa_group_sm_step(sm->wpa_auth);
	} while (sm->changed || sm->wpa_auth->changed);
	sm->in_step_loop = 0;

	if (sm->pending_deinit) {
		wpa_printf(MSG_DEBUG, "WPA: Completing pending STA state "
			   "machine deinit for " MACSTR, MAC2STR(sm->addr));
		wpa_free_sta_sm(sm);
	}
}


static void wpa_sm_call_step(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_state_machine *sm = eloop_ctx;
	wpa_sm_step(sm);
}


void wpa_auth_sm_notify(struct wpa_state_machine *sm)
{
	if (sm == NULL)
		return;
	eloop_register_timeout(0, 0, wpa_sm_call_step, sm, NULL);
}


void wpa_gtk_rekey(struct wpa_authenticator *wpa_auth)
{
	int tmp, i;

	if (wpa_auth == NULL)
		return;

	for (i = 0; i < 2; i++) {
		tmp = wpa_auth->GM;
		wpa_auth->GM = wpa_auth->GN;
		wpa_auth->GN = tmp;
		memcpy(wpa_auth->GNonce, wpa_auth->Counter, WPA_NONCE_LEN);
		inc_byte_array(wpa_auth->Counter, WPA_NONCE_LEN);
		wpa_gmk_to_gtk(wpa_auth->GMK, wpa_auth->addr, wpa_auth->GNonce,
			       wpa_auth->GTK[wpa_auth->GN - 1],
			       wpa_auth->GTK_len);
	}
}


static const char * wpa_bool_txt(int bool)
{
	return bool ? "TRUE" : "FALSE";
}


static int wpa_cipher_bits(int cipher)
{
	switch (cipher) {
	case WPA_CIPHER_CCMP:
		return 128;
	case WPA_CIPHER_TKIP:
		return 256;
	case WPA_CIPHER_WEP104:
		return 104;
	case WPA_CIPHER_WEP40:
		return 40;
	default:
		return 0;
	}
}


#define RSN_SUITE "%02x-%02x-%02x-%d"
#define RSN_SUITE_ARG(s) (s)[0], (s)[1], (s)[2], (s)[3]

int wpa_get_mib(struct wpa_authenticator *wpa_auth, char *buf, size_t buflen)
{
	int len = 0;
	char pmkid_txt[PMKID_LEN * 2 + 1];

	if (wpa_auth == NULL)
		return len;

	len += snprintf(buf + len, buflen - len,
			"dot11RSNAOptionImplemented=TRUE\n"
#ifdef CONFIG_RSN_PREAUTH
			"dot11RSNAPreauthenticationImplemented=TRUE\n"
#else /* CONFIG_RSN_PREAUTH */
			"dot11RSNAPreauthenticationImplemented=FALSE\n"
#endif /* CONFIG_RSN_PREAUTH */
			"dot11RSNAEnabled=%s\n"
			"dot11RSNAPreauthenticationEnabled=%s\n",
			wpa_bool_txt(wpa_auth->conf.wpa &
				     HOSTAPD_WPA_VERSION_WPA2),
			wpa_bool_txt(wpa_auth->conf.rsn_preauth));

	wpa_snprintf_hex(pmkid_txt, sizeof(pmkid_txt),
			 wpa_auth->dot11RSNAPMKIDUsed, PMKID_LEN);

	len += snprintf(buf + len, buflen - len,
			"dot11RSNAConfigVersion=%u\n"
			"dot11RSNAConfigPairwiseKeysSupported=9999\n"
			/* FIX: dot11RSNAConfigGroupCipher */
			/* FIX: dot11RSNAConfigGroupRekeyMethod */
			/* FIX: dot11RSNAConfigGroupRekeyTime */
			/* FIX: dot11RSNAConfigGroupRekeyPackets */
			"dot11RSNAConfigGroupRekeyStrict=%u\n"
			"dot11RSNAConfigGroupUpdateCount=%u\n"
			"dot11RSNAConfigPairwiseUpdateCount=%u\n"
			"dot11RSNAConfigGroupCipherSize=%u\n"
			"dot11RSNAConfigPMKLifetime=%u\n"
			"dot11RSNAConfigPMKReauthThreshold=%u\n"
			"dot11RSNAConfigNumberOfPTKSAReplayCounters=0\n"
			"dot11RSNAConfigSATimeout=%u\n"
			"dot11RSNAAuthenticationSuiteSelected=" RSN_SUITE "\n"
			"dot11RSNAPairwiseCipherSelected=" RSN_SUITE "\n"
			"dot11RSNAGroupCipherSelected=" RSN_SUITE "\n"
			"dot11RSNAPMKIDUsed=%s\n"
			"dot11RSNAAuthenticationSuiteRequested=" RSN_SUITE "\n"
			"dot11RSNAPairwiseCipherRequested=" RSN_SUITE "\n"
			"dot11RSNAGroupCipherRequested=" RSN_SUITE "\n"
			"dot11RSNATKIPCounterMeasuresInvoked=%u\n"
			"dot11RSNA4WayHandshakeFailures=%u\n"
			"dot11RSNAConfigNumberOfGTKSAReplayCounters=0\n",
			RSN_VERSION,
			!!wpa_auth->conf.wpa_strict_rekey,
			dot11RSNAConfigGroupUpdateCount,
			dot11RSNAConfigPairwiseUpdateCount,
			wpa_cipher_bits(wpa_auth->conf.wpa_group),
			dot11RSNAConfigPMKLifetime,
			dot11RSNAConfigPMKReauthThreshold,
			dot11RSNAConfigSATimeout,
			RSN_SUITE_ARG(wpa_auth->
				      dot11RSNAAuthenticationSuiteSelected),
			RSN_SUITE_ARG(wpa_auth->
				      dot11RSNAPairwiseCipherSelected),
			RSN_SUITE_ARG(wpa_auth->dot11RSNAGroupCipherSelected),
			pmkid_txt,
			RSN_SUITE_ARG(wpa_auth->
				      dot11RSNAAuthenticationSuiteRequested),
			RSN_SUITE_ARG(wpa_auth->
				      dot11RSNAPairwiseCipherRequested),
			RSN_SUITE_ARG(wpa_auth->dot11RSNAGroupCipherRequested),
			wpa_auth->dot11RSNATKIPCounterMeasuresInvoked,
			wpa_auth->dot11RSNA4WayHandshakeFailures);

	/* TODO: dot11RSNAConfigPairwiseCiphersTable */
	/* TODO: dot11RSNAConfigAuthenticationSuitesTable */

	/* Private MIB */
	len += snprintf(buf + len, buflen - len, "hostapdWPAGroupState=%d\n",
			wpa_auth->wpa_group_state);

	return len;
}


int wpa_get_mib_sta(struct wpa_state_machine *sm, char *buf, size_t buflen)
{
	int len = 0;
	u8 not_used[4] = { 0, 0, 0, 0 };
	const u8 *pairwise = not_used;

	if (sm == NULL)
		return 0;

	/* TODO: FF-FF-FF-FF-FF-FF entry for broadcast/multicast stats */

	/* dot11RSNAStatsEntry */

	if (sm->wpa == WPA_VERSION_WPA) {
		if (sm->pairwise == WPA_CIPHER_CCMP)
			pairwise = WPA_CIPHER_SUITE_CCMP;
		else if (sm->pairwise == WPA_CIPHER_TKIP)
			pairwise = WPA_CIPHER_SUITE_TKIP;
		else if (sm->pairwise == WPA_CIPHER_WEP104)
			pairwise = WPA_CIPHER_SUITE_WEP104;
		else if (sm->pairwise == WPA_CIPHER_WEP40)
			pairwise = WPA_CIPHER_SUITE_WEP40;
		else if (sm->pairwise == WPA_CIPHER_NONE)
			pairwise = WPA_CIPHER_SUITE_NONE;
	} else if (sm->wpa == WPA_VERSION_WPA2) {
		if (sm->pairwise == WPA_CIPHER_CCMP)
			pairwise = RSN_CIPHER_SUITE_CCMP;
		else if (sm->pairwise == WPA_CIPHER_TKIP)
			pairwise = RSN_CIPHER_SUITE_TKIP;
		else if (sm->pairwise == WPA_CIPHER_WEP104)
			pairwise = RSN_CIPHER_SUITE_WEP104;
		else if (sm->pairwise == WPA_CIPHER_WEP40)
			pairwise = RSN_CIPHER_SUITE_WEP40;
		else if (sm->pairwise == WPA_CIPHER_NONE)
			pairwise = RSN_CIPHER_SUITE_NONE;
	} else
		return 0;

	len += snprintf(buf + len, buflen - len,
			/* TODO: dot11RSNAStatsIndex */
			"dot11RSNAStatsSTAAddress=" MACSTR "\n"
			"dot11RSNAStatsVersion=1\n"
			"dot11RSNAStatsSelectedPairwiseCipher=" RSN_SUITE "\n"
			/* TODO: dot11RSNAStatsTKIPICVErrors */
			"dot11RSNAStatsTKIPLocalMICFailures=%u\n"
			"dot11RSNAStatsTKIPRemoveMICFailures=%u\n"
			/* TODO: dot11RSNAStatsCCMPReplays */
			/* TODO: dot11RSNAStatsCCMPDecryptErrors */
			/* TODO: dot11RSNAStatsTKIPReplays */,
			MAC2STR(sm->addr),
			RSN_SUITE_ARG(pairwise),
			sm->dot11RSNAStatsTKIPLocalMICFailures,
			sm->dot11RSNAStatsTKIPRemoteMICFailures);

	/* Private MIB */
	len += snprintf(buf + len, buflen - len,
			"hostapdWPAPTKState=%d\n"
			"hostapdWPAPTKGroupState=%d\n",
			sm->wpa_ptk_state,
			sm->wpa_ptk_group_state);

	return len;
}


void wpa_auth_countermeasures_start(struct wpa_authenticator *wpa_auth)
{
	if (wpa_auth)
		wpa_auth->dot11RSNATKIPCounterMeasuresInvoked++;
}


int wpa_auth_pairwise_set(struct wpa_state_machine *sm)
{
	return sm && sm->pairwise_set;
}


int wpa_auth_sta_key_mgmt(struct wpa_state_machine *sm)
{
	if (sm == NULL)
		return -1;
	return sm->wpa_key_mgmt;
}


int wpa_auth_sta_wpa_version(struct wpa_state_machine *sm)
{
	if (sm == NULL)
		return 0;
	return sm->wpa;
}


int wpa_auth_sta_clear_pmksa(struct wpa_state_machine *sm,
			     struct rsn_pmksa_cache_entry *entry)
{
	if (sm == NULL || sm->pmksa != entry)
		return -1;
	sm->pmksa = NULL;
	return 0;
}


struct rsn_pmksa_cache_entry *
wpa_auth_sta_get_pmksa(struct wpa_state_machine *sm)
{
	return sm ? sm->pmksa : NULL;
}


void wpa_auth_sta_local_mic_failure_report(struct wpa_state_machine *sm)
{
	if (sm)
		sm->dot11RSNAStatsTKIPLocalMICFailures++;
}


const u8 * wpa_auth_get_wpa_ie(struct wpa_authenticator *wpa_auth, size_t *len)
{
	if (wpa_auth == NULL)
		return NULL;
	*len = wpa_auth->wpa_ie_len;
	return wpa_auth->wpa_ie;
}


int wpa_auth_pmksa_add(struct wpa_state_machine *sm, const u8 *pmk,
		       int session_timeout, struct eapol_state_machine *eapol)
{
	if (sm == NULL || sm->wpa != WPA_VERSION_WPA2)
		return -1;

	if (pmksa_cache_add(sm->wpa_auth->pmksa, pmk, WPA_PMK_LEN,
			    sm->wpa_auth->addr, sm->addr, session_timeout,
			    eapol))
		return 0;

	return -1;
}

#endif /* CONFIG_NATIVE_WINDOWS */
