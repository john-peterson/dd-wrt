/*
 * EAP peer method: EAP-TTLS (draft-ietf-pppext-eap-ttls-03.txt)
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
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

#include "common.h"
#include "eap_peer/eap_i.h"
#include "eap_peer/eap_tls_common.h"
#include "config_ssid.h"
#include "ms_funcs.h"
#include "sha1.h"
#include "crypto.h"
#include "tls.h"
#include "eap_common/eap_ttls.h"


/* Maximum supported TTLS version
 * 0 = draft-ietf-pppext-eap-ttls-03.txt / draft-funk-eap-ttls-v0-00.txt
 * 1 = draft-funk-eap-ttls-v1-00.txt
 */
#ifndef EAP_TTLS_VERSION
#define EAP_TTLS_VERSION 0 /* TTLSv1 implementation is not yet complete */
#endif /* EAP_TTLS_VERSION */


#define MSCHAPV2_KEY_LEN 16


static void eap_ttls_deinit(struct eap_sm *sm, void *priv);


struct eap_ttls_data {
	struct eap_ssl_data ssl;
	int ssl_initialized;

	int ttls_version, force_ttls_version;

	const struct eap_method *phase2_method;
	void *phase2_priv;
	int phase2_success;
	int phase2_start;

	enum {
		EAP_TTLS_PHASE2_EAP,
		EAP_TTLS_PHASE2_MSCHAPV2,
		EAP_TTLS_PHASE2_MSCHAP,
		EAP_TTLS_PHASE2_PAP,
		EAP_TTLS_PHASE2_CHAP
	} phase2_type;
	struct eap_method_type phase2_eap_type;
	struct eap_method_type *phase2_eap_types;
	size_t num_phase2_eap_types;

	u8 auth_response[20];
	int auth_response_valid;
	u8 ident;
	int resuming; /* starting a resumed session */
	int reauth; /* reauthentication */
	u8 *key_data;

	u8 *pending_phase2_req;
	size_t pending_phase2_req_len;

#ifdef EAP_TNC
	int ready_for_tnc;
	int tnc_started;
#endif /* EAP_TNC */
};


static void * eap_ttls_init(struct eap_sm *sm)
{
	struct eap_ttls_data *data;
	struct wpa_ssid *config = eap_get_config(sm);
	char *selected;

	data = os_zalloc(sizeof(*data));
	if (data == NULL)
		return NULL;
	data->ttls_version = EAP_TTLS_VERSION;
	data->force_ttls_version = -1;
	selected = "EAP";
	data->phase2_type = EAP_TTLS_PHASE2_EAP;

#if EAP_TTLS_VERSION > 0
	if (config && config->phase1) {
		const char *pos = os_strstr(config->phase1, "ttlsver=");
		if (pos) {
			data->force_ttls_version = atoi(pos + 8);
			data->ttls_version = data->force_ttls_version;
			wpa_printf(MSG_DEBUG, "EAP-TTLS: Forced TTLS version "
				   "%d", data->force_ttls_version);
		}
	}
#endif /* EAP_TTLS_VERSION */

	if (config && config->phase2) {
		if (os_strstr(config->phase2, "autheap=")) {
			selected = "EAP";
			data->phase2_type = EAP_TTLS_PHASE2_EAP;
		} else if (os_strstr(config->phase2, "auth=MSCHAPV2")) {
			selected = "MSCHAPV2";
			data->phase2_type = EAP_TTLS_PHASE2_MSCHAPV2;
		} else if (os_strstr(config->phase2, "auth=MSCHAP")) {
			selected = "MSCHAP";
			data->phase2_type = EAP_TTLS_PHASE2_MSCHAP;
		} else if (os_strstr(config->phase2, "auth=PAP")) {
			selected = "PAP";
			data->phase2_type = EAP_TTLS_PHASE2_PAP;
		} else if (os_strstr(config->phase2, "auth=CHAP")) {
			selected = "CHAP";
			data->phase2_type = EAP_TTLS_PHASE2_CHAP;
		}
	}
	wpa_printf(MSG_DEBUG, "EAP-TTLS: Phase2 type: %s", selected);

	if (data->phase2_type == EAP_TTLS_PHASE2_EAP) {
		if (eap_peer_select_phase2_methods(config, "autheap=",
						   &data->phase2_eap_types,
						   &data->num_phase2_eap_types)
		    < 0) {
			eap_ttls_deinit(sm, data);
			return NULL;
		}

		data->phase2_eap_type.vendor = EAP_VENDOR_IETF;
		data->phase2_eap_type.method = EAP_TYPE_NONE;
	}

#if EAP_TTLS_VERSION > 0
	if (!(tls_capabilities(sm->ssl_ctx) & TLS_CAPABILITY_IA) &&
	    data->ttls_version > 0) {
		if (data->force_ttls_version > 0) {
			wpa_printf(MSG_INFO, "EAP-TTLS: Forced TTLSv%d and "
				   "TLS library does not support TLS/IA.",
				   data->force_ttls_version);
			eap_ttls_deinit(sm, data);
			return NULL;
		}
		data->ttls_version = 0;
	}
#endif /* EAP_TTLS_VERSION */

	return data;
}


static void eap_ttls_deinit(struct eap_sm *sm, void *priv)
{
	struct eap_ttls_data *data = priv;
	if (data == NULL)
		return;
	if (data->phase2_priv && data->phase2_method)
		data->phase2_method->deinit(sm, data->phase2_priv);
	os_free(data->phase2_eap_types);
	if (data->ssl_initialized)
		eap_peer_tls_ssl_deinit(sm, &data->ssl);
	os_free(data->key_data);
	os_free(data->pending_phase2_req);
	os_free(data);
}


static u8 * eap_ttls_avp_hdr(u8 *avphdr, u32 avp_code, u32 vendor_id,
			     int mandatory, size_t len)
{
	struct ttls_avp_vendor *avp;
	u8 flags;
	size_t hdrlen;

	avp = (struct ttls_avp_vendor *) avphdr;
	flags = mandatory ? AVP_FLAGS_MANDATORY : 0;
	if (vendor_id) {
		flags |= AVP_FLAGS_VENDOR;
		hdrlen = sizeof(*avp);
		avp->vendor_id = host_to_be32(vendor_id);
	} else {
		hdrlen = sizeof(struct ttls_avp);
	}

	avp->avp_code = host_to_be32(avp_code);
	avp->avp_length = host_to_be32((flags << 24) | (hdrlen + len));

	return avphdr + hdrlen;
}


static u8 * eap_ttls_avp_add(u8 *start, u8 *avphdr, u32 avp_code,
			     u32 vendor_id, int mandatory,
			     const u8 *data, size_t len)
{
	u8 *pos;
	pos = eap_ttls_avp_hdr(avphdr, avp_code, vendor_id, mandatory, len);
	os_memcpy(pos, data, len);
	pos += len;
	AVP_PAD(start, pos);
	return pos;
}


static int eap_ttls_avp_encapsulate(u8 **resp, size_t *resp_len, u32 avp_code,
				    int mandatory)
{
	u8 *avp, *pos;

	avp = os_malloc(sizeof(struct ttls_avp) + *resp_len + 4);
	if (avp == NULL) {
		os_free(*resp);
		*resp = NULL;
		*resp_len = 0;
		return -1;
	}

	pos = eap_ttls_avp_hdr(avp, avp_code, 0, mandatory, *resp_len);
	os_memcpy(pos, *resp, *resp_len);
	pos += *resp_len;
	AVP_PAD(avp, pos);
	os_free(*resp);
	*resp = avp;
	*resp_len = pos - avp;
	return 0;
}


#if EAP_TTLS_VERSION > 0
static int eap_ttls_ia_permute_inner_secret(struct eap_sm *sm,
					    struct eap_ttls_data *data,
					    const u8 *key, size_t key_len)
{
	u8 *buf;
	size_t buf_len;
	int ret;

	if (key) {
		buf_len = 2 + key_len;
		buf = os_malloc(buf_len);
		if (buf == NULL)
			return -1;
		WPA_PUT_BE16(buf, key_len);
		os_memcpy(buf + 2, key, key_len);
	} else {
		buf = NULL;
		buf_len = 0;
	}

	wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: Session keys for TLS/IA inner "
			"secret permutation", buf, buf_len);
	ret = tls_connection_ia_permute_inner_secret(sm->ssl_ctx,
						     data->ssl.conn,
						     buf, buf_len);
	os_free(buf);

	return ret;
}
#endif /* EAP_TTLS_VERSION */


static int eap_ttls_v0_derive_key(struct eap_sm *sm,
				  struct eap_ttls_data *data)
{
	os_free(data->key_data);
	data->key_data = eap_peer_tls_derive_key(sm, &data->ssl,
						 "ttls keying material",
						 EAP_TLS_KEY_LEN);
	if (!data->key_data) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Failed to derive key");
		return -1;
	}

	wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: Derived key",
			data->key_data, EAP_TLS_KEY_LEN);

	return 0;
}


#if EAP_TTLS_VERSION > 0
static int eap_ttls_v1_derive_key(struct eap_sm *sm,
				  struct eap_ttls_data *data)
{
	struct tls_keys keys;
	u8 *rnd;

	os_free(data->key_data);
	data->key_data = NULL;

	os_memset(&keys, 0, sizeof(keys));
	if (tls_connection_get_keys(sm->ssl_ctx, data->ssl.conn, &keys) ||
	    keys.client_random == NULL || keys.server_random == NULL ||
	    keys.inner_secret == NULL) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Could not get inner secret, "
			   "client random, or server random to derive keying "
			   "material");
		return -1;
	}

	rnd = os_malloc(keys.client_random_len + keys.server_random_len);
	data->key_data = os_malloc(EAP_TLS_KEY_LEN);
	if (rnd == NULL || data->key_data == NULL) {
		wpa_printf(MSG_INFO, "EAP-TTLS: No memory for key derivation");
		os_free(rnd);
		os_free(data->key_data);
		data->key_data = NULL;
		return -1;
	}
	os_memcpy(rnd, keys.client_random, keys.client_random_len);
	os_memcpy(rnd + keys.client_random_len, keys.server_random,
		  keys.server_random_len);

	if (tls_prf(keys.inner_secret, keys.inner_secret_len,
		    "ttls v1 keying material", rnd, keys.client_random_len +
		    keys.server_random_len, data->key_data, EAP_TLS_KEY_LEN)) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Failed to derive key");
		os_free(rnd);
		os_free(data->key_data);
		data->key_data = NULL;
		return -1;
	}

	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: client/server random",
		    rnd, keys.client_random_len + keys.server_random_len);
	wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: TLS/IA inner secret",
			keys.inner_secret, keys.inner_secret_len);

	os_free(rnd);

	wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: Derived key",
			data->key_data, EAP_TLS_KEY_LEN);

	return 0;
}
#endif /* EAP_TTLS_VERSION */


static u8 * eap_ttls_implicit_challenge(struct eap_sm *sm,
					struct eap_ttls_data *data, size_t len)
{
#if EAP_TTLS_VERSION > 0
	struct tls_keys keys;
	u8 *challenge, *rnd;
#endif /* EAP_TTLS_VERSION */

	if (data->ttls_version == 0) {
		return eap_peer_tls_derive_key(sm, &data->ssl,
					       "ttls challenge", len);
	}

#if EAP_TTLS_VERSION > 0

	os_memset(&keys, 0, sizeof(keys));
	if (tls_connection_get_keys(sm->ssl_ctx, data->ssl.conn, &keys) ||
	    keys.client_random == NULL || keys.server_random == NULL ||
	    keys.inner_secret == NULL) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Could not get inner secret, "
			   "client random, or server random to derive "
			   "implicit challenge");
		return NULL;
	}

	rnd = os_malloc(keys.client_random_len + keys.server_random_len);
	challenge = os_malloc(len);
	if (rnd == NULL || challenge == NULL) {
		wpa_printf(MSG_INFO, "EAP-TTLS: No memory for implicit "
			   "challenge derivation");
		os_free(rnd);
		os_free(challenge);
		return NULL;
	}
	os_memcpy(rnd, keys.server_random, keys.server_random_len);
	os_memcpy(rnd + keys.server_random_len, keys.client_random,
		  keys.client_random_len);

	if (tls_prf(keys.inner_secret, keys.inner_secret_len,
		    "inner application challenge", rnd,
		    keys.client_random_len + keys.server_random_len,
		    challenge, len)) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Failed to derive implicit "
			   "challenge");
		os_free(rnd);
		os_free(challenge);
		return NULL;
	}

	os_free(rnd);

	wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: Derived implicit challenge",
			challenge, len);

	return challenge;

#else /* EAP_TTLS_VERSION */

	return NULL;

#endif /* EAP_TTLS_VERSION */
}


static void eap_ttlsv1_phase2_eap_finish(struct eap_sm *sm,
					 struct eap_ttls_data *data,
					 struct eap_method_ret *ret)
{
#if EAP_TTLS_VERSION > 0
	if (data->ttls_version > 0) {
		const struct eap_method *m = data->phase2_method;
		void *priv = data->phase2_priv;

		/* TTLSv1 requires TLS/IA FinalPhaseFinished */
		if (ret->decision == DECISION_UNCOND_SUCC)
			ret->decision = DECISION_COND_SUCC;
		ret->methodState = METHOD_CONT;

		if (ret->decision == DECISION_COND_SUCC &&
		    m->isKeyAvailable && m->getKey &&
		    m->isKeyAvailable(sm, priv)) {
			u8 *key;
			size_t key_len;
			key = m->getKey(sm, priv, &key_len);
			if (key) {
				eap_ttls_ia_permute_inner_secret(
					sm, data, key, key_len);
				os_free(key);
			}
		}
	}
#endif /* EAP_TTLS_VERSION */
}


static void eap_ttls_phase2_select_eap_method(struct eap_ttls_data *data,
					      u8 method)
{
	size_t i;
	for (i = 0; i < data->num_phase2_eap_types; i++) {
		if (data->phase2_eap_types[i].vendor != EAP_VENDOR_IETF ||
		    data->phase2_eap_types[i].method != method)
			continue;

		data->phase2_eap_type.vendor =
			data->phase2_eap_types[i].vendor;
		data->phase2_eap_type.method =
			data->phase2_eap_types[i].method;
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Selected "
			   "Phase 2 EAP vendor %d method %d",
			   data->phase2_eap_type.vendor,
			   data->phase2_eap_type.method);
		break;
	}
}


static int eap_ttls_phase2_request_eap_method(struct eap_sm *sm,
					      struct eap_ttls_data *data,
					      struct eap_method_ret *ret,
					      struct eap_hdr *hdr, size_t len,
					      u8 method,
					      u8 **resp, size_t *resp_len)
{
	struct eap_method_ret iret;

#ifdef EAP_TNC
	if (data->tnc_started && data->phase2_method &&
	    data->phase2_priv && method == EAP_TYPE_TNC &&
	    data->phase2_eap_type.method == EAP_TYPE_TNC)
		goto process;

	if (data->ready_for_tnc && !data->tnc_started &&
	    method == EAP_TYPE_TNC) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Start TNC after completed "
			   "EAP method");
		data->tnc_started = 1;
	}

	if (data->tnc_started) {
		if (data->phase2_eap_type.vendor != EAP_VENDOR_IETF ||
		    data->phase2_eap_type.method == EAP_TYPE_TNC) {
			wpa_printf(MSG_DEBUG, "EAP-TTLS: Unexpected EAP "
				   "type %d for TNC", method);
			return -1;
		}

		data->phase2_eap_type.vendor = EAP_VENDOR_IETF;
		data->phase2_eap_type.method = method;
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Selected "
			   "Phase 2 EAP vendor %d method %d (TNC)",
			   data->phase2_eap_type.vendor,
			   data->phase2_eap_type.method);

		if (data->phase2_type == EAP_TTLS_PHASE2_EAP &&
		    data->phase2_priv && data->phase2_method) {
			data->phase2_method->deinit(sm, data->phase2_priv);
			data->phase2_method = NULL;
			data->phase2_priv = NULL;
		}

		goto init;
	}
#endif /* EAP_TNC */

	if (data->phase2_eap_type.vendor == EAP_VENDOR_IETF &&
	    data->phase2_eap_type.method == EAP_TYPE_NONE)
		eap_ttls_phase2_select_eap_method(data, method);

	if (method != data->phase2_eap_type.method || method == EAP_TYPE_NONE)
	{
		if (eap_peer_tls_phase2_nak(data->phase2_eap_types,
					    data->num_phase2_eap_types,
					    hdr, resp, resp_len))
			return -1;
		return 0;
	}

#ifdef EAP_TNC
init:
#endif /* EAP_TNC */

	if (data->phase2_priv == NULL) {
		data->phase2_method = eap_peer_get_eap_method(
			EAP_VENDOR_IETF, method);
		if (data->phase2_method) {
			sm->init_phase2 = 1;
			sm->mschapv2_full_key = 1;
			data->phase2_priv = data->phase2_method->init(sm);
			sm->init_phase2 = 0;
			sm->mschapv2_full_key = 0;
		}
	}
	if (data->phase2_priv == NULL || data->phase2_method == NULL) {
		wpa_printf(MSG_INFO, "EAP-TTLS: failed to initialize "
			   "Phase 2 EAP method %d", method);
		return -1;
	}

#ifdef EAP_TNC
process:
#endif /* EAP_TNC */
	os_memset(&iret, 0, sizeof(iret));
	*resp = data->phase2_method->process(sm, data->phase2_priv,
					     &iret, (u8 *) hdr, len,
					     resp_len);
	if ((iret.methodState == METHOD_DONE ||
	     iret.methodState == METHOD_MAY_CONT) &&
	    (iret.decision == DECISION_UNCOND_SUCC ||
	     iret.decision == DECISION_COND_SUCC ||
	     iret.decision == DECISION_FAIL)) {
		ret->methodState = iret.methodState;
		ret->decision = iret.decision;
	}
	eap_ttlsv1_phase2_eap_finish(sm, data, ret);

	return 0;
}


static int eap_ttls_phase2_request_eap(struct eap_sm *sm,
				       struct eap_ttls_data *data,
				       struct eap_method_ret *ret,
				       struct eap_hdr *hdr,
				       u8 **resp, size_t *resp_len)
{
	size_t len = be_to_host16(hdr->length);
	u8 *pos;
	struct wpa_ssid *config = eap_get_config(sm);

	if (len <= sizeof(struct eap_hdr)) {
		wpa_printf(MSG_INFO, "EAP-TTLS: too short "
			   "Phase 2 request (len=%lu)", (unsigned long) len);
		return -1;
	}
	pos = (u8 *) (hdr + 1);
	wpa_printf(MSG_DEBUG, "EAP-TTLS: Phase 2 EAP Request: type=%d", *pos);
	switch (*pos) {
	case EAP_TYPE_IDENTITY:
		*resp = eap_sm_buildIdentity(sm, hdr->identifier, resp_len, 1);
		break;
	default:
		if (eap_ttls_phase2_request_eap_method(sm, data, ret, hdr, len,
						       *pos, resp, resp_len) <
		    0)
			return -1;
		break;
	}

	if (*resp == NULL &&
	    (config->pending_req_identity || config->pending_req_password ||
	     config->pending_req_otp)) {
		return 0;
	}

	if (*resp == NULL)
		return -1;

	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: AVP encapsulate EAP Response",
		    *resp, *resp_len);
	return eap_ttls_avp_encapsulate(resp, resp_len,
					RADIUS_ATTR_EAP_MESSAGE, 1);
}


static void eap_ttlsv1_permute_inner(struct eap_sm *sm,
				     struct eap_ttls_data *data,
				     const u8 *nt_response)
{
#if EAP_TTLS_VERSION > 0
	u8 pw_hash[16], pw_hash_hash[16], master_key[16];
	u8 session_key[2 * MSCHAPV2_KEY_LEN];
	const u8 *password;
	size_t password_len;
	int pwhash;

	if (data->ttls_version == 0)
		return;

	password = eap_get_config_password2(sm, &password_len, &pwhash);
	if (password == NULL)
		return;

	if (pwhash)
		hash_nt_password_hash(password, pw_hash_hash);
	else {
		nt_password_hash(password, password_len, pw_hash);
		hash_nt_password_hash(pw_hash, pw_hash_hash);
	}
	get_master_key(pw_hash_hash, nt_response, master_key);
	get_asymetric_start_key(master_key, session_key,
				MSCHAPV2_KEY_LEN, 0, 0);
	get_asymetric_start_key(master_key, session_key + MSCHAPV2_KEY_LEN,
				MSCHAPV2_KEY_LEN, 1, 0);
	eap_ttls_ia_permute_inner_secret(sm, data, session_key,
					 sizeof(session_key));
#endif /* EAP_TTLS_VERSION */
}


static int eap_ttls_phase2_request_mschapv2(struct eap_sm *sm,
					    struct eap_ttls_data *data,
					    struct eap_method_ret *ret,
					    u8 **resp, size_t *resp_len)
{
	u8 *buf, *pos, *challenge, *peer_challenge;
	const u8 *identity, *username, *password;
	size_t identity_len, password_len, username_len, i;
	int pwhash;

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Phase 2 MSCHAPV2 Request");

	/* MSCHAPv2 does not include optional domain name in the
	 * challenge-response calculation, so remove domain prefix
	 * (if present). */
	identity = eap_get_config_identity(sm, &identity_len);
	password = eap_get_config_password2(sm, &password_len, &pwhash);
	if (identity == NULL || password == NULL)
		return -1;
	username = identity;
	username_len = identity_len;
	for (i = 0; i < username_len; i++) {
		if (username[i] == '\\') {
			username_len -= i + 1;
			username += i + 1;
			break;
		}
	}

	pos = buf = os_malloc(identity_len + 1000);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR,
			   "EAP-TTLS/MSCHAPV2: Failed to allocate memory");
		return -1;
	}

	/* User-Name */
	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_USER_NAME, 0, 1,
			       identity, identity_len);

	/* MS-CHAP-Challenge */
	challenge = eap_ttls_implicit_challenge(
		sm, data, EAP_TTLS_MSCHAPV2_CHALLENGE_LEN * 2 + 1);
	if (challenge == NULL) {
		os_free(buf);
		wpa_printf(MSG_ERROR, "EAP-TTLS/MSCHAPV2: Failed to derive "
			   "implicit challenge");
		return -1;
	}
	peer_challenge = challenge + 1 + EAP_TTLS_MSCHAPV2_CHALLENGE_LEN;

	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_MS_CHAP_CHALLENGE,
			       RADIUS_VENDOR_ID_MICROSOFT, 1,
			       challenge, EAP_TTLS_MSCHAPV2_CHALLENGE_LEN);

	/* MS-CHAP2-Response */
	pos = eap_ttls_avp_hdr(pos, RADIUS_ATTR_MS_CHAP2_RESPONSE,
			       RADIUS_VENDOR_ID_MICROSOFT, 1,
			       EAP_TTLS_MSCHAPV2_RESPONSE_LEN);
	data->ident = challenge[EAP_TTLS_MSCHAPV2_CHALLENGE_LEN];
	*pos++ = data->ident;
	*pos++ = 0; /* Flags */
	os_memcpy(pos, peer_challenge, EAP_TTLS_MSCHAPV2_CHALLENGE_LEN);
	pos += EAP_TTLS_MSCHAPV2_CHALLENGE_LEN;
	os_memset(pos, 0, 8); /* Reserved, must be zero */
	pos += 8;
	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: MSCHAPV2: implicit auth_challenge",
		    challenge, EAP_TTLS_MSCHAPV2_CHALLENGE_LEN);
	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: MSCHAPV2: peer_challenge",
		    peer_challenge, EAP_TTLS_MSCHAPV2_CHALLENGE_LEN);
	wpa_hexdump_ascii(MSG_DEBUG, "EAP-TTLS: MSCHAPV2 username",
			  username, username_len);
	if (pwhash) {
		wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: MSCHAPV2 password hash",
				password, 16);
		generate_nt_response_pwhash(challenge, peer_challenge,
					    username, username_len,
					    password, pos);
		wpa_hexdump(MSG_DEBUG, "EAP-TTLS: MSCHAPV2 response", pos, 24);
		generate_authenticator_response_pwhash(
			password, peer_challenge, challenge,
			username, username_len, pos, data->auth_response);
	} else {
		wpa_hexdump_ascii_key(MSG_DEBUG, "EAP-TTLS: MSCHAPV2 password",
				      password, password_len);
		generate_nt_response(challenge, peer_challenge,
				     username, username_len,
				     password, password_len, pos);
		wpa_hexdump(MSG_DEBUG, "EAP-TTLS: MSCHAPV2 response", pos, 24);
		generate_authenticator_response(password, password_len,
						peer_challenge, challenge,
						username, username_len,
						pos, data->auth_response);
	}
	data->auth_response_valid = 1;

	eap_ttlsv1_permute_inner(sm, data, pos);

	pos += 24;
	os_free(challenge);
	AVP_PAD(buf, pos);

	*resp = buf;
	*resp_len = pos - buf;

	if (sm->workaround && data->ttls_version == 0) {
		/* At least FreeRADIUS seems to be terminating
		 * EAP-TTLS/MSHCAPV2 without the expected MS-CHAP-v2 Success
		 * packet. */
		wpa_printf(MSG_DEBUG, "EAP-TTLS/MSCHAPV2: EAP workaround - "
			   "allow success without tunneled response");
		ret->methodState = METHOD_MAY_CONT;
		ret->decision = DECISION_COND_SUCC;
	}

	return 0;
}


static int eap_ttls_phase2_request_mschap(struct eap_sm *sm,
					  struct eap_ttls_data *data,
					  struct eap_method_ret *ret,
					  u8 **resp, size_t *resp_len)
{
	u8 *buf, *pos, *challenge;
	const u8 *identity, *password;
	size_t identity_len, password_len;
	int pwhash;

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Phase 2 MSCHAP Request");

	identity = eap_get_config_identity(sm, &identity_len);
	password = eap_get_config_password2(sm, &password_len, &pwhash);
	if (identity == NULL || password == NULL)
		return -1;

	pos = buf = os_malloc(identity_len + 1000);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR,
			   "EAP-TTLS/MSCHAP: Failed to allocate memory");
		return -1;
	}

	/* User-Name */
	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_USER_NAME, 0, 1,
			       identity, identity_len);

	/* MS-CHAP-Challenge */
	challenge = eap_ttls_implicit_challenge(sm, data, EAP_TLS_KEY_LEN);
	if (challenge == NULL) {
		os_free(buf);
		wpa_printf(MSG_ERROR, "EAP-TTLS/MSCHAP: Failed to derive "
			   "implicit challenge");
		return -1;
	}

	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_MS_CHAP_CHALLENGE,
			       RADIUS_VENDOR_ID_MICROSOFT, 1,
			       challenge, EAP_TTLS_MSCHAP_CHALLENGE_LEN);

	/* MS-CHAP-Response */
	pos = eap_ttls_avp_hdr(pos, RADIUS_ATTR_MS_CHAP_RESPONSE,
			       RADIUS_VENDOR_ID_MICROSOFT, 1,
			       EAP_TTLS_MSCHAP_RESPONSE_LEN);
	data->ident = challenge[EAP_TTLS_MSCHAP_CHALLENGE_LEN];
	*pos++ = data->ident;
	*pos++ = 1; /* Flags: Use NT style passwords */
	os_memset(pos, 0, 24); /* LM-Response */
	pos += 24;
	if (pwhash) {
		challenge_response(challenge, password, pos); /* NT-Response */
		wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: MSCHAP password hash",
				password, 16);
	} else {
		nt_challenge_response(challenge, password, password_len,
				      pos); /* NT-Response */
		wpa_hexdump_ascii_key(MSG_DEBUG, "EAP-TTLS: MSCHAP password",
				      password, password_len);
	}
	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: MSCHAP implicit challenge",
		    challenge, EAP_TTLS_MSCHAP_CHALLENGE_LEN);
	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: MSCHAP response", pos, 24);
	pos += 24;
	os_free(challenge);
	AVP_PAD(buf, pos);

	*resp = buf;
	*resp_len = pos - buf;

	if (data->ttls_version > 0) {
		/* EAP-TTLSv1 uses TLS/IA FinalPhaseFinished to report success,
		 * so do not allow connection to be terminated yet. */
		ret->methodState = METHOD_CONT;
		ret->decision = DECISION_COND_SUCC;
	} else {
		/* EAP-TTLS/MSCHAP does not provide tunneled success
		 * notification, so assume that Phase2 succeeds. */
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_COND_SUCC;
	}

	return 0;
}


static int eap_ttls_phase2_request_pap(struct eap_sm *sm,
				       struct eap_ttls_data *data,
				       struct eap_method_ret *ret,
				       u8 **resp, size_t *resp_len)
{
	u8 *buf, *pos;
	size_t pad;
	const u8 *identity, *password;
	size_t identity_len, password_len;

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Phase 2 PAP Request");

	identity = eap_get_config_identity(sm, &identity_len);
	password = eap_get_config_password(sm, &password_len);
	if (identity == NULL || password == NULL)
		return -1;

	pos = buf = os_malloc(identity_len + password_len + 100);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR,
			   "EAP-TTLS/PAP: Failed to allocate memory");
		return -1;
	}

	/* User-Name */
	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_USER_NAME, 0, 1,
			       identity, identity_len);

	/* User-Password; in RADIUS, this is encrypted, but EAP-TTLS encrypts
	 * the data, so no separate encryption is used in the AVP itself.
	 * However, the password is padded to obfuscate its length. */
	pad = (16 - (password_len & 15)) & 15;
	pos = eap_ttls_avp_hdr(pos, RADIUS_ATTR_USER_PASSWORD, 0, 1,
			       password_len + pad);
	os_memcpy(pos, password, password_len);
	pos += password_len;
	os_memset(pos, 0, pad);
	pos += pad;
	AVP_PAD(buf, pos);

	*resp = buf;
	*resp_len = pos - buf;

	if (data->ttls_version > 0) {
		/* EAP-TTLSv1 uses TLS/IA FinalPhaseFinished to report success,
		 * so do not allow connection to be terminated yet. */
		ret->methodState = METHOD_CONT;
		ret->decision = DECISION_COND_SUCC;
	} else {
		/* EAP-TTLS/PAP does not provide tunneled success notification,
		 * so assume that Phase2 succeeds. */
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_COND_SUCC;
	}

	return 0;
}


static int eap_ttls_phase2_request_chap(struct eap_sm *sm,
					struct eap_ttls_data *data,
					struct eap_method_ret *ret,
					u8 **resp, size_t *resp_len)
{
	u8 *buf, *pos, *challenge;
	const u8 *addr[3];
	size_t len[3];
	const u8 *identity, *password;
	size_t identity_len, password_len;

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Phase 2 CHAP Request");

	identity = eap_get_config_identity(sm, &identity_len);
	password = eap_get_config_password(sm, &password_len);
	if (identity == NULL || password == NULL)
		return -1;

	pos = buf = os_malloc(identity_len + 1000);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR,
			   "EAP-TTLS/CHAP: Failed to allocate memory");
		return -1;
	}

	/* User-Name */
	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_USER_NAME, 0, 1,
			       identity, identity_len);

	/* CHAP-Challenge */
	challenge = eap_ttls_implicit_challenge(sm, data, EAP_TLS_KEY_LEN);
	if (challenge == NULL) {
		os_free(buf);
		wpa_printf(MSG_ERROR, "EAP-TTLS/CHAP: Failed to derive "
			   "implicit challenge");
		return -1;
	}

	pos = eap_ttls_avp_add(buf, pos, RADIUS_ATTR_CHAP_CHALLENGE, 0, 1,
			       challenge, EAP_TTLS_CHAP_CHALLENGE_LEN);

	/* CHAP-Password */
	pos = eap_ttls_avp_hdr(pos, RADIUS_ATTR_CHAP_PASSWORD, 0, 1,
			       1 + EAP_TTLS_CHAP_PASSWORD_LEN);
	data->ident = challenge[EAP_TTLS_CHAP_CHALLENGE_LEN];
	*pos++ = data->ident;

	/* MD5(Ident + Password + Challenge) */
	addr[0] = &data->ident;
	len[0] = 1;
	addr[1] = password;
	len[1] = password_len;
	addr[2] = challenge;
	len[2] = EAP_TTLS_CHAP_CHALLENGE_LEN;
	md5_vector(3, addr, len, pos);

	wpa_hexdump_ascii(MSG_DEBUG, "EAP-TTLS: CHAP username",
			  identity, identity_len);
	wpa_hexdump_ascii_key(MSG_DEBUG, "EAP-TTLS: CHAP password",
			      password, password_len);
	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: CHAP implicit challenge",
		    challenge, EAP_TTLS_CHAP_CHALLENGE_LEN);
	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: CHAP password",
		    pos, EAP_TTLS_CHAP_PASSWORD_LEN);
	pos += EAP_TTLS_CHAP_PASSWORD_LEN;
	os_free(challenge);
	AVP_PAD(buf, pos);

	*resp = buf;
	*resp_len = pos - buf;

	if (data->ttls_version > 0) {
		/* EAP-TTLSv1 uses TLS/IA FinalPhaseFinished to report success,
		 * so do not allow connection to be terminated yet. */
		ret->methodState = METHOD_CONT;
		ret->decision = DECISION_COND_SUCC;
	} else {
		/* EAP-TTLS/CHAP does not provide tunneled success
		 * notification, so assume that Phase2 succeeds. */
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_COND_SUCC;
	}

	return 0;
}


static int eap_ttls_phase2_request(struct eap_sm *sm,
				   struct eap_ttls_data *data,
				   struct eap_method_ret *ret,
				   struct eap_hdr *hdr,
				   u8 **resp, size_t *resp_len)
{
	int res = 0;
	size_t len;

	if (data->phase2_type == EAP_TTLS_PHASE2_MSCHAPV2 ||
	    data->phase2_type == EAP_TTLS_PHASE2_MSCHAP ||
	    data->phase2_type == EAP_TTLS_PHASE2_PAP ||
	    data->phase2_type == EAP_TTLS_PHASE2_CHAP) {
		if (eap_get_config_identity(sm, &len) == NULL) {
			wpa_printf(MSG_INFO,
				   "EAP-TTLS: Identity not configured");
			eap_sm_request_identity(sm);
			if (eap_get_config_password(sm, &len) == NULL)
				eap_sm_request_password(sm);
			return 0;
		}

		if (eap_get_config_password(sm, &len) == NULL) {
			wpa_printf(MSG_INFO,
				   "EAP-TTLS: Password not configured");
			eap_sm_request_password(sm);
			return 0;
		}
	}

#ifdef EAP_TNC
	if (data->tnc_started) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Processing TNC");
		res = eap_ttls_phase2_request_eap(sm, data, ret, hdr,
						  resp, resp_len);
		goto done;
	}
#endif /* EAP_TNC */

	switch (data->phase2_type) {
	case EAP_TTLS_PHASE2_EAP:
		res = eap_ttls_phase2_request_eap(sm, data, ret, hdr,
						  resp, resp_len);
		break;
	case EAP_TTLS_PHASE2_MSCHAPV2:
		res = eap_ttls_phase2_request_mschapv2(sm, data, ret,
						       resp, resp_len);
		break;
	case EAP_TTLS_PHASE2_MSCHAP:
		res = eap_ttls_phase2_request_mschap(sm, data, ret,
						     resp, resp_len);
		break;
	case EAP_TTLS_PHASE2_PAP:
		res = eap_ttls_phase2_request_pap(sm, data, ret,
						  resp, resp_len);
		break;
	case EAP_TTLS_PHASE2_CHAP:
		res = eap_ttls_phase2_request_chap(sm, data, ret,
						   resp, resp_len);
		break;
	default:
		wpa_printf(MSG_ERROR, "EAP-TTLS: Phase 2 - Unknown");
		res = -1;
		break;
	}

#ifdef EAP_TNC
done:
#endif /* EAP_TNC */

	if (res < 0) {
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
	}

	return res;
}


#if EAP_TTLS_VERSION > 0
static u8 * eap_ttls_build_phase_finished(struct eap_sm *sm,
					  struct eap_ttls_data *data,
					  int id, int final,
					  size_t *reqDataLen)
{
	int len;
	struct eap_hdr *req;
	u8 *pos;
	const int max_len = 300;

	len = sizeof(struct eap_hdr) + 2 + max_len;
	req = os_malloc(len);
	if (req == NULL)
		return NULL;

	req->code = EAP_CODE_RESPONSE;
	req->identifier = id;

	pos = (u8 *) (req + 1);
	*pos++ = EAP_TYPE_TTLS;
	*pos++ = data->ttls_version;

	len = tls_connection_ia_send_phase_finished(sm->ssl_ctx,
						    data->ssl.conn,
						    final, pos, max_len);
	if (len < 0) {
		os_free(req);
		return NULL;
	}

	*reqDataLen = sizeof(struct eap_hdr) + 2 + len;
	req->length = host_to_be16(*reqDataLen);

	return (u8 *) req;
}
#endif /* EAP_TTLS_VERSION */


struct ttls_parse_avp {
	u8 *mschapv2;
	u8 *eapdata;
	size_t eap_len;
	int mschapv2_error;
};


static int eap_ttls_parse_attr_eap(const u8 *dpos, size_t dlen,
				   struct ttls_parse_avp *parse)
{
	wpa_printf(MSG_DEBUG, "EAP-TTLS: AVP - EAP Message");
	if (parse->eapdata == NULL) {
		parse->eapdata = os_malloc(dlen);
		if (parse->eapdata == NULL) {
			wpa_printf(MSG_WARNING, "EAP-TTLS: Failed to allocate "
				   "memory for Phase 2 EAP data");
			return -1;
		}
		os_memcpy(parse->eapdata, dpos, dlen);
		parse->eap_len = dlen;
	} else {
		u8 *neweap = os_realloc(parse->eapdata, parse->eap_len + dlen);
		if (neweap == NULL) {
			wpa_printf(MSG_WARNING, "EAP-TTLS: Failed to allocate "
				   "memory for Phase 2 EAP data");
			return -1;
		}
		os_memcpy(neweap + parse->eap_len, dpos, dlen);
		parse->eapdata = neweap;
		parse->eap_len += dlen;
	}

	return 0;
}


static int eap_ttls_parse_avp(u8 *pos, size_t left,
			      struct ttls_parse_avp *parse)
{
	struct ttls_avp *avp;
	u32 avp_code, avp_length, vendor_id = 0;
	u8 avp_flags, *dpos;
	size_t dlen;

	avp = (struct ttls_avp *) pos;
	avp_code = be_to_host32(avp->avp_code);
	avp_length = be_to_host32(avp->avp_length);
	avp_flags = (avp_length >> 24) & 0xff;
	avp_length &= 0xffffff;
	wpa_printf(MSG_DEBUG, "EAP-TTLS: AVP: code=%d flags=0x%02x "
		   "length=%d", (int) avp_code, avp_flags,
		   (int) avp_length);

	if (avp_length > left) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: AVP overflow "
			   "(len=%d, left=%lu) - dropped",
			   (int) avp_length, (unsigned long) left);
		return -1;
	}

	if (avp_length < sizeof(*avp)) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Invalid AVP length %d",
			   avp_length);
		return -1;
	}

	dpos = (u8 *) (avp + 1);
	dlen = avp_length - sizeof(*avp);
	if (avp_flags & AVP_FLAGS_VENDOR) {
		if (dlen < 4) {
			wpa_printf(MSG_WARNING, "EAP-TTLS: Vendor AVP "
				   "underflow");
			return -1;
		}
		vendor_id = WPA_GET_BE32(dpos);
		wpa_printf(MSG_DEBUG, "EAP-TTLS: AVP vendor_id %d",
			   (int) vendor_id);
		dpos += 4;
		dlen -= 4;
	}

	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: AVP data", dpos, dlen);

	if (vendor_id == 0 && avp_code == RADIUS_ATTR_EAP_MESSAGE) {
		if (eap_ttls_parse_attr_eap(dpos, dlen, parse) < 0)
			return -1;
	} else if (vendor_id == 0 && avp_code == RADIUS_ATTR_REPLY_MESSAGE) {
		/* This is an optional message that can be displayed to
		 * the user. */
		wpa_hexdump_ascii(MSG_DEBUG, "EAP-TTLS: AVP - Reply-Message",
				  dpos, dlen);
	} else if (vendor_id == RADIUS_VENDOR_ID_MICROSOFT &&
		   avp_code == RADIUS_ATTR_MS_CHAP2_SUCCESS) {
		wpa_hexdump_ascii(MSG_DEBUG, "EAP-TTLS: MS-CHAP2-Success",
				  dpos, dlen);
		if (dlen != 43) {
			wpa_printf(MSG_WARNING, "EAP-TTLS: Unexpected "
				   "MS-CHAP2-Success length "
				   "(len=%lu, expected 43)",
				   (unsigned long) dlen);
			return -1;
		}
		parse->mschapv2 = dpos;
	} else if (vendor_id == RADIUS_VENDOR_ID_MICROSOFT &&
		   avp_code == RADIUS_ATTR_MS_CHAP_ERROR) {
		wpa_hexdump_ascii(MSG_DEBUG, "EAP-TTLS: MS-CHAP-Error",
				  dpos, dlen);
		parse->mschapv2_error = 1;
	} else if (avp_flags & AVP_FLAGS_MANDATORY) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Unsupported mandatory AVP "
			   "code %d vendor_id %d - dropped",
			   (int) avp_code, (int) vendor_id);
		return -1;
	} else {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Ignoring unsupported AVP "
			   "code %d vendor_id %d",
			   (int) avp_code, (int) vendor_id);
	}

	return avp_length;
}


static int eap_ttls_parse_avps(u8 *in_decrypted, size_t len_decrypted,
			       struct ttls_parse_avp *parse)
{
	u8 *pos;
	size_t left, pad;
	int avp_length;

	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: Decrypted Phase 2 AVPs",
		    in_decrypted, len_decrypted);
	if (len_decrypted < sizeof(struct ttls_avp)) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Too short Phase 2 AVP frame"
			   " len=%lu expected %lu or more - dropped",
			   (unsigned long) len_decrypted,
			   (unsigned long) sizeof(struct ttls_avp));
		return -1;
	}

	/* Parse AVPs */
	pos = in_decrypted;
	left = len_decrypted;

	os_memset(parse, 0, sizeof(*parse));

	while (left > 0) {
		avp_length = eap_ttls_parse_avp(pos, left, parse);
		if (avp_length < 0)
			return -1;

		pad = (4 - (avp_length & 3)) & 3;
		pos += avp_length + pad;
		if (left < avp_length + pad)
			left = 0;
		else
			left -= avp_length + pad;
	}

	return 0;
}


static u8 * eap_ttls_fake_identity_request(void)
{
	struct eap_hdr *hdr;
	u8 *buf;

	wpa_printf(MSG_DEBUG, "EAP-TTLS: empty data in beginning of "
		   "Phase 2 - use fake EAP-Request Identity");
	buf = os_malloc(sizeof(*hdr) + 1);
	if (buf == NULL) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: failed to allocate "
			   "memory for fake EAP-Identity Request");
		return NULL;
	}

	hdr = (struct eap_hdr *) buf;
	hdr->code = EAP_CODE_REQUEST;
	hdr->identifier = 0;
	hdr->length = host_to_be16(sizeof(*hdr) + 1);
	buf[sizeof(*hdr)] = EAP_TYPE_IDENTITY;

	return buf;
}


static int eap_ttls_encrypt_response(struct eap_sm *sm,
				     struct eap_ttls_data *data,
				     u8 *resp, size_t resp_len,
				     u8 identifier,
				     u8 **out_data, size_t *out_len)
{
	if (resp == NULL)
		return 0;

	wpa_hexdump_key(MSG_DEBUG, "EAP-TTLS: Encrypting Phase 2 data",
			resp, resp_len);
	if (eap_peer_tls_encrypt(sm, &data->ssl, EAP_TYPE_TTLS,
				 data->ttls_version, identifier,
				 resp, resp_len, out_data, out_len)) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Failed to encrypt a Phase 2 "
			   "frame");
		return -1;
	}
	os_free(resp);

	return 0;
}


static int eap_ttls_process_phase2_eap(struct eap_sm *sm,
				       struct eap_ttls_data *data,
				       struct eap_method_ret *ret,
				       struct ttls_parse_avp *parse,
				       u8 **resp, size_t *resp_len)
{
	struct eap_hdr *hdr;
	size_t len;

	if (parse->eapdata == NULL) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: No EAP Message in the "
			   "packet - dropped");
		return -1;
	}

	wpa_hexdump(MSG_DEBUG, "EAP-TTLS: Phase 2 EAP",
		    parse->eapdata, parse->eap_len);
	hdr = (struct eap_hdr *) parse->eapdata;

	if (parse->eap_len < sizeof(*hdr)) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Too short Phase 2 EAP "
			   "frame (len=%lu, expected %lu or more) - dropped",
			   (unsigned long) parse->eap_len,
			   (unsigned long) sizeof(*hdr));
		return -1;
	}
	len = be_to_host16(hdr->length);
	if (len > parse->eap_len) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Length mismatch in Phase 2 "
			   "EAP frame (EAP hdr len=%lu, EAP data len in "
			   "AVP=%lu)",
			   (unsigned long) len,
			   (unsigned long) parse->eap_len);
		return -1;
	}
	wpa_printf(MSG_DEBUG, "EAP-TTLS: received Phase 2: code=%d "
		   "identifier=%d length=%lu",
		   hdr->code, hdr->identifier, (unsigned long) len);
	switch (hdr->code) {
	case EAP_CODE_REQUEST:
		if (eap_ttls_phase2_request(sm, data, ret, hdr, resp,
					    resp_len)) {
			wpa_printf(MSG_INFO, "EAP-TTLS: Phase2 Request "
				   "processing failed");
			return -1;
		}
		break;
	default:
		wpa_printf(MSG_INFO, "EAP-TTLS: Unexpected code=%d in "
			   "Phase 2 EAP header", hdr->code);
		return -1;
	}

	return 0;
}


static int eap_ttls_process_phase2_mschapv2(struct eap_sm *sm,
					    struct eap_ttls_data *data,
					    struct eap_method_ret *ret,
					    struct ttls_parse_avp *parse)
{
	u8 recv_response[20];

	if (parse->mschapv2_error) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS/MSCHAPV2: Received "
			   "MS-CHAP-Error - failed");
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
		/* Reply with empty data to ACK error */
		return 1;
	}

	if (parse->mschapv2 == NULL) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: no MS-CHAP2-Success AVP "
			   "received for Phase2 MSCHAPV2");
		return -1;
	}
	if (parse->mschapv2[0] != data->ident) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Ident mismatch for Phase 2 "
			   "MSCHAPV2 (received Ident 0x%02x, expected 0x%02x)",
			   parse->mschapv2[0], data->ident);
		return -1;
	}
	if (!data->auth_response_valid ||
	    parse->mschapv2[1] != 'S' || parse->mschapv2[2] != '=' ||
	    hexstr2bin((char *) (parse->mschapv2 + 3), recv_response, 20) ||
	    os_memcmp(data->auth_response, recv_response, 20) != 0) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Invalid authenticator "
			   "response in Phase 2 MSCHAPV2 success request");
		return -1;
	}

	wpa_printf(MSG_INFO, "EAP-TTLS: Phase 2 MSCHAPV2 "
		   "authentication succeeded");
	if (data->ttls_version > 0) {
		/*
		 * EAP-TTLSv1 uses TLS/IA FinalPhaseFinished to report
		 * success, so do not allow connection to be terminated
		 * yet.
		 */
		ret->methodState = METHOD_CONT;
		ret->decision = DECISION_COND_SUCC;
	} else {
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_UNCOND_SUCC;
		data->phase2_success = 1;
	}

	/*
	 * Reply with empty data; authentication server will reply
	 * with EAP-Success after this.
	 */
	return 1;
}


#ifdef EAP_TNC
static int eap_ttls_process_tnc_start(struct eap_sm *sm,
				      struct eap_ttls_data *data,
				      struct eap_method_ret *ret,
				      struct ttls_parse_avp *parse,
				      u8 **resp, size_t *resp_len)
{
	/* TNC uses inner EAP method after non-EAP TTLS phase 2. */
	if (parse->eapdata == NULL) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Phase 2 received "
			   "unexpected tunneled data (no EAP)");
		return -1;
	}

	if (!data->ready_for_tnc) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Phase 2 received "
			   "EAP after non-EAP, but not ready for TNC");
		return -1;
	}

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Start TNC after completed "
		   "non-EAP method");
	data->tnc_started = 1;

	if (eap_ttls_process_phase2_eap(sm, data, ret, parse, resp,
					resp_len) < 0)
		return -1;

	return 0;
}
#endif /* EAP_TNC */


static int eap_ttls_process_decrypted(struct eap_sm *sm,
				      struct eap_ttls_data *data,
				      struct eap_method_ret *ret,
				      u8 identifier,
				      struct ttls_parse_avp *parse,
				      u8 *in_decrypted, size_t len_decrypted,
				      u8 **out_data, size_t *out_len)
{
	u8 *resp = NULL;
	size_t resp_len;
	struct wpa_ssid *config = eap_get_config(sm);
	int res;

	switch (data->phase2_type) {
	case EAP_TTLS_PHASE2_EAP:
		if (eap_ttls_process_phase2_eap(sm, data, ret, parse, &resp,
						&resp_len) < 0)
			return -1;
		break;
	case EAP_TTLS_PHASE2_MSCHAPV2:
		res = eap_ttls_process_phase2_mschapv2(sm, data, ret, parse);
#ifdef EAP_TNC
		if (res == 1 && parse->eapdata &&
		    ret->methodState == METHOD_DONE &&
		    ret->decision == DECISION_UNCOND_SUCC) {
			/*
			 * TNC may be required as the next
			 * authentication method within the tunnel.
			 */
			ret->methodState = METHOD_MAY_CONT;
			data->ready_for_tnc = 1;
			if (eap_ttls_process_tnc_start(sm, data, ret, parse,
						       &resp, &resp_len) == 0)
				break;
		}
#endif /* EAP_TNC */
		return res;
	case EAP_TTLS_PHASE2_MSCHAP:
	case EAP_TTLS_PHASE2_PAP:
	case EAP_TTLS_PHASE2_CHAP:
#ifdef EAP_TNC
		if (eap_ttls_process_tnc_start(sm, data, ret, parse,
					       &resp, &resp_len) < 0)
			return -1;
		break;
#else /* EAP_TNC */
		/* EAP-TTLS/{MSCHAP,PAP,CHAP} should not send any TLS tunneled
		 * requests to the supplicant */
		wpa_printf(MSG_INFO, "EAP-TTLS: Phase 2 received unexpected "
			   "tunneled data");
		return -1;
#endif /* EAP_TNC */
	}

	if (resp) {
		if (eap_ttls_encrypt_response(sm, data, resp, resp_len,
					      identifier,
					      out_data, out_len) < 0)
			return -1;
	} else if (config->pending_req_identity ||
		   config->pending_req_password ||
		   config->pending_req_otp ||
		   config->pending_req_new_password) {
		os_free(data->pending_phase2_req);
		data->pending_phase2_req = os_malloc(len_decrypted);
		if (data->pending_phase2_req) {
			os_memcpy(data->pending_phase2_req, in_decrypted,
				  len_decrypted);
			data->pending_phase2_req_len = len_decrypted;
		}
	}

	return 0;
}


#if EAP_TTLS_VERSION > 0
static void eap_ttls_final_phase_finished(struct eap_sm *sm,
					  struct eap_ttls_data *data,
					  struct eap_method_ret *ret,
					  u8 identifier,
					  u8 **out_data, size_t *out_len)
{
	wpa_printf(MSG_DEBUG, "EAP-TTLS: FinalPhaseFinished received");
	wpa_printf(MSG_INFO, "EAP-TTLS: TLS/IA authentication succeeded");
	ret->methodState = METHOD_DONE;
	ret->decision = DECISION_UNCOND_SUCC;
	data->phase2_success = 1;
	*out_data = eap_ttls_build_phase_finished(sm, data, identifier, 1,
						  out_len);
	eap_ttls_v1_derive_key(sm, data);
}
#endif /* EAP_TTLS_VERSION */


static int eap_ttls_implicit_identity_request(struct eap_sm *sm,
					      struct eap_ttls_data *data,
					      struct eap_method_ret *ret,
					      u8 identifier,
					      u8 **out_data, size_t *out_len)
{
	int retval = 0;
	struct eap_hdr *hdr;
	u8 *resp;
	size_t resp_len;

	hdr = (struct eap_hdr *) eap_ttls_fake_identity_request();
	if (hdr == NULL) {
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
		return -1;
	}

	resp = NULL;
	if (eap_ttls_phase2_request(sm, data, ret, hdr, &resp, &resp_len)) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Phase2 Request "
			   "processing failed");
		retval = -1;
	} else {
		retval = eap_ttls_encrypt_response(sm, data, resp, resp_len,
						   identifier, out_data,
						   out_len);
	}

	os_free(hdr);

	if (retval < 0) {
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
	}

	return retval;
}


static int eap_ttls_phase2_start(struct eap_sm *sm, struct eap_ttls_data *data,
				 struct eap_method_ret *ret, u8 identifier,
				 u8 **out_data, size_t *out_len)
{
	data->phase2_start = 0;

	/*
	 * EAP-TTLS does not use Phase2 on fast re-auth; this must be done only
	 * if TLS part was indeed resuming a previous session. Most
	 * Authentication Servers terminate EAP-TTLS before reaching this
	 * point, but some do not. Make wpa_supplicant stop phase 2 here, if
	 * needed.
	 */
	if (data->reauth &&
	    tls_connection_resumed(sm->ssl_ctx, data->ssl.conn)) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Session resumption - "
			   "skip phase 2");
		*out_data = eap_peer_tls_build_ack(&data->ssl, out_len,
						   identifier, EAP_TYPE_TTLS,
						   data->ttls_version);
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_UNCOND_SUCC;
		data->phase2_success = 1;
		return 0;
	}

	return eap_ttls_implicit_identity_request(sm, data, ret, identifier,
						  out_data, out_len);
}


static int eap_ttls_decrypt(struct eap_sm *sm, struct eap_ttls_data *data,
			    struct eap_method_ret *ret,
			    u8 identifier,
			    const u8 *in_data, size_t in_len,
			    u8 **out_data, size_t *out_len)
{
	u8 *in_decrypted = NULL;
	int retval = 0;
	size_t len_decrypted = 0;
	struct ttls_parse_avp parse;

	os_memset(&parse, 0, sizeof(parse));

	wpa_printf(MSG_DEBUG, "EAP-TTLS: received %lu bytes encrypted data for"
		   " Phase 2", (unsigned long) in_len);

	if (data->pending_phase2_req) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: Pending Phase 2 request - "
			   "skip decryption and use old data");
		/* Clear TLS reassembly state. */
		eap_peer_tls_reset_input(&data->ssl);

		in_decrypted = data->pending_phase2_req;
		data->pending_phase2_req = NULL;
		len_decrypted = data->pending_phase2_req_len;
		if (data->pending_phase2_req_len == 0) {
			os_free(in_decrypted);
			return eap_ttls_implicit_identity_request(
				sm, data, ret, identifier, out_data,
				out_len);
		}
		goto continue_req;
	}

	if (in_len == 0 && data->phase2_start) {
		return eap_ttls_phase2_start(sm, data, ret, identifier,
					     out_data, out_len);
	}

	if (in_len == 0) {
		/* Received TLS ACK - requesting more fragments */
		return eap_peer_tls_encrypt(sm, &data->ssl, EAP_TYPE_TTLS,
					    data->ttls_version,
					    identifier, NULL, 0,
					    out_data, out_len);
	}

	retval = eap_peer_tls_decrypt(sm, &data->ssl, in_data, in_len,
				      &in_decrypted, &len_decrypted);
	if (retval)
		goto done;

#if EAP_TTLS_VERSION > 0
	if (data->ttls_version > 0 && len_decrypted == 0 &&
	    tls_connection_ia_final_phase_finished(sm->ssl_ctx,
						   data->ssl.conn)) {
		eap_ttls_final_phase_finished(sm, data, ret, identifier,
					      out_data, out_len);
		goto done;
	}
#endif /* EAP_TTLS_VERSION */

continue_req:
	data->phase2_start = 0;

	if (eap_ttls_parse_avps(in_decrypted, len_decrypted, &parse) < 0) {
		retval = -1;
		goto done;
	}

	retval = eap_ttls_process_decrypted(sm, data, ret, identifier,
					    &parse,
					    in_decrypted, len_decrypted,
					    out_data, out_len);

done:
	os_free(in_decrypted);
	os_free(parse.eapdata);

	if (retval < 0) {
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
	}

	return retval;
}


static int eap_ttls_process_start(struct eap_sm *sm,
				  struct eap_ttls_data *data, u8 flags,
				  struct eap_method_ret *ret)
{
	struct wpa_ssid *config = eap_get_config(sm);

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Start (server ver=%d, own ver=%d)",
		   flags & EAP_PEAP_VERSION_MASK, data->ttls_version);
#if EAP_TTLS_VERSION > 0
	if ((flags & EAP_PEAP_VERSION_MASK) < data->ttls_version)
		data->ttls_version = flags & EAP_PEAP_VERSION_MASK;
	if (data->force_ttls_version >= 0 &&
	    data->force_ttls_version != data->ttls_version) {
		wpa_printf(MSG_WARNING, "EAP-TTLS: Failed to select "
			   "forced TTLS version %d",
			   data->force_ttls_version);
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
		ret->allowNotifications = FALSE;
		return -1;
	}
	wpa_printf(MSG_DEBUG, "EAP-TTLS: Using TTLS version %d",
		   data->ttls_version);

	if (data->ttls_version > 0)
		data->ssl.tls_ia = 1;
#endif /* EAP_TTLS_VERSION */
	if (!data->ssl_initialized &&
	    eap_peer_tls_ssl_init(sm, &data->ssl, config)) {
		wpa_printf(MSG_INFO, "EAP-TTLS: Failed to initialize SSL.");
		return -1;
	}
	data->ssl_initialized = 1;

	wpa_printf(MSG_DEBUG, "EAP-TTLS: Start");

	return 0;
}


static int eap_ttls_process_handshake(struct eap_sm *sm,
				      struct eap_ttls_data *data,
				      struct eap_method_ret *ret,
				      u8 identifier,
				      const u8 *in_data, size_t in_len,
				      u8 **out_data, size_t *out_len)
{
	int res;

	res = eap_peer_tls_process_helper(sm, &data->ssl, EAP_TYPE_TTLS,
					  data->ttls_version, identifier,
					  in_data, in_len, out_data, out_len);

	if (tls_connection_established(sm->ssl_ctx, data->ssl.conn)) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: TLS done, proceed to "
			   "Phase 2");
		if (data->resuming) {
			wpa_printf(MSG_DEBUG, "EAP-TTLS: fast reauth - may "
				   "skip Phase 2");
			ret->decision = DECISION_COND_SUCC;
			ret->methodState = METHOD_MAY_CONT;
		}
		data->phase2_start = 1;
		if (data->ttls_version == 0)
			eap_ttls_v0_derive_key(sm, data);

		if (*out_len == 0) {
			if (eap_ttls_decrypt(sm, data, ret, identifier,
					     NULL, 0, out_data, out_len)) {
				wpa_printf(MSG_WARNING, "EAP-TTLS: "
					   "failed to process early "
					   "start for Phase 2");
			}
			res = 0;
		}
		data->resuming = 0;
	}

	if (res == 2) {
		/*
		 * Application data included in the handshake message.
		 */
		os_free(data->pending_phase2_req);
		data->pending_phase2_req = *out_data;
		data->pending_phase2_req_len = *out_len;
		*out_data = NULL;
		*out_len = 0;
		res = eap_ttls_decrypt(sm, data, ret, identifier,
				       in_data, in_len, out_data, out_len);
	}

	return res;
}


static void eap_ttls_check_auth_status(struct eap_sm *sm, 
				       struct eap_ttls_data *data,
				       struct eap_method_ret *ret)
{
	if (data->ttls_version == 0 && ret->methodState == METHOD_DONE) {
		ret->allowNotifications = FALSE;
		if (ret->decision == DECISION_UNCOND_SUCC ||
		    ret->decision == DECISION_COND_SUCC) {
			wpa_printf(MSG_DEBUG, "EAP-TTLS: Authentication "
				   "completed successfully");
			data->phase2_success = 1;
#ifdef EAP_TNC
			if (!data->ready_for_tnc && !data->tnc_started) {
				/*
				 * TNC may be required as the next
				 * authentication method within the tunnel.
				 */
				ret->methodState = METHOD_MAY_CONT;
				data->ready_for_tnc = 1;
			}
#endif /* EAP_TNC */
		}
	} else if (data->ttls_version == 0 && sm->workaround &&
		   ret->methodState == METHOD_MAY_CONT &&
		   (ret->decision == DECISION_UNCOND_SUCC ||
		    ret->decision == DECISION_COND_SUCC)) {
			wpa_printf(MSG_DEBUG, "EAP-TTLS: Authentication "
				   "completed successfully (EAP workaround)");
			data->phase2_success = 1;
	}
}


static u8 * eap_ttls_process(struct eap_sm *sm, void *priv,
			     struct eap_method_ret *ret,
			     const u8 *reqData, size_t reqDataLen,
			     size_t *respDataLen)
{
	const struct eap_hdr *req;
	size_t left;
	int res;
	u8 flags, *resp;
	const u8 *pos;
	struct eap_ttls_data *data = priv;

	pos = eap_peer_tls_process_init(sm, &data->ssl, EAP_TYPE_TTLS, ret,
					reqData, reqDataLen, &left, &flags);
	if (pos == NULL)
		return NULL;
	req = (const struct eap_hdr *) reqData;

	if (flags & EAP_TLS_FLAGS_START) {
		if (eap_ttls_process_start(sm, data, flags, ret) < 0)
			return NULL;

		/* draft-ietf-pppext-eap-ttls-03.txt, Ch. 8.1:
		 * EAP-TTLS Start packet may, in a future specification, be
		 * allowed to contain data. Client based on this draft version
		 * must ignore such data but must not reject the Start packet.
		 */
		left = 0;
	} else if (!data->ssl_initialized) {
		wpa_printf(MSG_DEBUG, "EAP-TTLS: First message did not "
			   "include Start flag");
		ret->methodState = METHOD_DONE;
		ret->decision = DECISION_FAIL;
		ret->allowNotifications = FALSE;
		return NULL;
	}

	resp = NULL;
	if (tls_connection_established(sm->ssl_ctx, data->ssl.conn) &&
	    !data->resuming) {
		res = eap_ttls_decrypt(sm, data, ret, req->identifier,
				       pos, left, &resp, respDataLen);
	} else {
		res = eap_ttls_process_handshake(sm, data, ret,
						 req->identifier, pos, left,
						 &resp, respDataLen);
	}

	eap_ttls_check_auth_status(sm, data, ret);

	/* FIX: what about res == -1? Could just move all error processing into
	 * the other functions and get rid of this res==1 case here. */
	if (res == 1) {
		os_free(resp);
		return eap_peer_tls_build_ack(&data->ssl, respDataLen,
					      req->identifier, EAP_TYPE_TTLS,
					      data->ttls_version);
	}
	return resp;
}


static Boolean eap_ttls_has_reauth_data(struct eap_sm *sm, void *priv)
{
	struct eap_ttls_data *data = priv;
	return tls_connection_established(sm->ssl_ctx, data->ssl.conn) &&
		data->phase2_success;
}


static void eap_ttls_deinit_for_reauth(struct eap_sm *sm, void *priv)
{
	struct eap_ttls_data *data = priv;
	os_free(data->pending_phase2_req);
	data->pending_phase2_req = NULL;
#ifdef EAP_TNC
	data->ready_for_tnc = 0;
	data->tnc_started = 0;
#endif /* EAP_TNC */
}


static void * eap_ttls_init_for_reauth(struct eap_sm *sm, void *priv)
{
	struct eap_ttls_data *data = priv;
	os_free(data->key_data);
	data->key_data = NULL;
	if (eap_peer_tls_reauth_init(sm, &data->ssl)) {
		os_free(data);
		return NULL;
	}
	if (data->phase2_priv && data->phase2_method &&
	    data->phase2_method->init_for_reauth)
		data->phase2_method->init_for_reauth(sm, data->phase2_priv);
	data->phase2_start = 0;
	data->phase2_success = 0;
	data->resuming = 1;
	data->reauth = 1;
	return priv;
}


static int eap_ttls_get_status(struct eap_sm *sm, void *priv, char *buf,
			       size_t buflen, int verbose)
{
	struct eap_ttls_data *data = priv;
	int len, ret;

	len = eap_peer_tls_status(sm, &data->ssl, buf, buflen, verbose);
	ret = os_snprintf(buf + len, buflen - len,
			  "EAP-TTLSv%d Phase2 method=",
			  data->ttls_version);
	if (ret < 0 || (size_t) ret >= buflen - len)
		return len;
	len += ret;
	switch (data->phase2_type) {
	case EAP_TTLS_PHASE2_EAP:
		ret = os_snprintf(buf + len, buflen - len, "EAP-%s\n",
				  data->phase2_method ?
				  data->phase2_method->name : "?");
		break;
	case EAP_TTLS_PHASE2_MSCHAPV2:
		ret = os_snprintf(buf + len, buflen - len, "MSCHAPV2\n");
		break;
	case EAP_TTLS_PHASE2_MSCHAP:
		ret = os_snprintf(buf + len, buflen - len, "MSCHAP\n");
		break;
	case EAP_TTLS_PHASE2_PAP:
		ret = os_snprintf(buf + len, buflen - len, "PAP\n");
		break;
	case EAP_TTLS_PHASE2_CHAP:
		ret = os_snprintf(buf + len, buflen - len, "CHAP\n");
		break;
	default:
		ret = 0;
		break;
	}
	if (ret < 0 || (size_t) ret >= buflen - len)
		return len;
	len += ret;

	return len;
}


static Boolean eap_ttls_isKeyAvailable(struct eap_sm *sm, void *priv)
{
	struct eap_ttls_data *data = priv;
	return data->key_data != NULL && data->phase2_success;
}


static u8 * eap_ttls_getKey(struct eap_sm *sm, void *priv, size_t *len)
{
	struct eap_ttls_data *data = priv;
	u8 *key;

	if (data->key_data == NULL || !data->phase2_success)
		return NULL;

	key = os_malloc(EAP_TLS_KEY_LEN);
	if (key == NULL)
		return NULL;

	*len = EAP_TLS_KEY_LEN;
	os_memcpy(key, data->key_data, EAP_TLS_KEY_LEN);

	return key;
}


int eap_peer_ttls_register(void)
{
	struct eap_method *eap;
	int ret;

	eap = eap_peer_method_alloc(EAP_PEER_METHOD_INTERFACE_VERSION,
				    EAP_VENDOR_IETF, EAP_TYPE_TTLS, "TTLS");
	if (eap == NULL)
		return -1;

	eap->init = eap_ttls_init;
	eap->deinit = eap_ttls_deinit;
	eap->process = eap_ttls_process;
	eap->isKeyAvailable = eap_ttls_isKeyAvailable;
	eap->getKey = eap_ttls_getKey;
	eap->get_status = eap_ttls_get_status;
	eap->has_reauth_data = eap_ttls_has_reauth_data;
	eap->deinit_for_reauth = eap_ttls_deinit_for_reauth;
	eap->init_for_reauth = eap_ttls_init_for_reauth;

	ret = eap_peer_method_register(eap);
	if (ret)
		eap_peer_method_free(eap);
	return ret;
}
