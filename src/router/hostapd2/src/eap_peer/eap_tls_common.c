/*
 * EAP peer: EAP-TLS/PEAP/TTLS/FAST common functions
 * Copyright (c) 2004-2006, Jouni Malinen <j@w1.fi>
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
#include "eap_i.h"
#include "eap_tls_common.h"
#include "config_ssid.h"
#include "sha1.h"
#include "tls.h"
#include "config.h"


static int eap_tls_check_blob(struct eap_sm *sm, const char **name,
			      const u8 **data, size_t *data_len)
{
	const struct wpa_config_blob *blob;

	if (*name == NULL || os_strncmp(*name, "blob://", 7) != 0)
		return 0;

	blob = eap_get_config_blob(sm, *name + 7);
	if (blob == NULL) {
		wpa_printf(MSG_ERROR, "%s: Named configuration blob '%s' not "
			   "found", __func__, *name + 7);
		return -1;
	}

	*name = NULL;
	*data = blob->data;
	*data_len = blob->len;

	return 0;
}


static void eap_tls_params_from_conf1(struct tls_connection_params *params,
				      struct wpa_ssid *config)
{
	params->ca_cert = (char *) config->ca_cert;
	params->ca_path = (char *) config->ca_path;
	params->client_cert = (char *) config->client_cert;
	params->private_key = (char *) config->private_key;
	params->private_key_passwd = (char *) config->private_key_passwd;
	params->dh_file = (char *) config->dh_file;
	params->subject_match = (char *) config->subject_match;
	params->altsubject_match = (char *) config->altsubject_match;
	params->engine_id = config->engine_id;
	params->pin = config->pin;
	params->key_id = config->key_id;
}


static void eap_tls_params_from_conf2(struct tls_connection_params *params,
				      struct wpa_ssid *config)
{
	params->ca_cert = (char *) config->ca_cert2;
	params->ca_path = (char *) config->ca_path2;
	params->client_cert = (char *) config->client_cert2;
	params->private_key = (char *) config->private_key2;
	params->private_key_passwd = (char *) config->private_key2_passwd;
	params->dh_file = (char *) config->dh_file2;
	params->subject_match = (char *) config->subject_match2;
	params->altsubject_match = (char *) config->altsubject_match2;
}


static int eap_tls_params_from_conf(struct eap_sm *sm,
				    struct eap_ssl_data *data,
				    struct tls_connection_params *params,
				    struct wpa_ssid *config, int phase2)
{
	os_memset(params, 0, sizeof(*params));
	params->engine = config->engine;
	if (phase2)
		eap_tls_params_from_conf2(params, config);
	else
		eap_tls_params_from_conf1(params, config);
	params->tls_ia = data->tls_ia;

	/*
	 * Use blob data, if available. Otherwise, leave reference to external
	 * file as-is.
	 */
	if (eap_tls_check_blob(sm, &params->ca_cert, &params->ca_cert_blob,
			       &params->ca_cert_blob_len) ||
	    eap_tls_check_blob(sm, &params->client_cert,
			       &params->client_cert_blob,
			       &params->client_cert_blob_len) ||
	    eap_tls_check_blob(sm, &params->private_key,
			       &params->private_key_blob,
			       &params->private_key_blob_len) ||
	    eap_tls_check_blob(sm, &params->dh_file, &params->dh_blob,
			       &params->dh_blob_len)) {
		wpa_printf(MSG_INFO, "SSL: Failed to get configuration blobs");
		return -1;
	}

	return 0;
}


static int eap_tls_init_connection(struct eap_sm *sm,
				   struct eap_ssl_data *data,
				   struct wpa_ssid *config,
				   struct tls_connection_params *params)
{
	int res;

	data->conn = tls_connection_init(sm->ssl_ctx);
	if (data->conn == NULL) {
		wpa_printf(MSG_INFO, "SSL: Failed to initialize new TLS "
			   "connection");
		return -1;
	}

	res = tls_connection_set_params(sm->ssl_ctx, data->conn, params);
	if (res == TLS_SET_PARAMS_ENGINE_PRV_INIT_FAILED) {
		/*
		 * At this point with the pkcs11 engine the PIN might be wrong.
		 * We reset the PIN in the configuration to be sure to not use
		 * it again and the calling function must request a new one.
		 */
		os_free(config->pin);
		config->pin = NULL;
	} else if (res == TLS_SET_PARAMS_ENGINE_PRV_VERIFY_FAILED) {
		wpa_printf(MSG_INFO, "TLS: Failed to load private key");
		/*
		 * We do not know exactly but maybe the PIN was wrong,
		 * so ask for a new one.
		 */
		os_free(config->pin);
		config->pin = NULL;
		eap_sm_request_pin(sm);
		sm->ignore = TRUE;
		return -1;
	} else if (res) {
		wpa_printf(MSG_INFO, "TLS: Failed to set TLS connection "
			   "parameters");
		return -1;
	}

	return 0;
}


/**
 * eap_peer_tls_ssl_init - Initialize shared TLS functionality
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @config: Pointer to the network configuration
 * Returns: 0 on success, -1 on failure
 *
 * This function is used to initialize shared TLS functionality for EAP-TLS,
 * EAP-PEAP, EAP-TTLS, and EAP-FAST.
 */
int eap_peer_tls_ssl_init(struct eap_sm *sm, struct eap_ssl_data *data,
			  struct wpa_ssid *config)
{
	struct tls_connection_params params;

	if (config == NULL)
		return -1;

	data->eap = sm;
	data->phase2 = sm->init_phase2;
	if (eap_tls_params_from_conf(sm, data, &params, config, data->phase2) <
	    0)
		return -1;

	if (eap_tls_init_connection(sm, data, config, &params) < 0)
		return -1;

	data->tls_out_limit = config->fragment_size;
	if (data->phase2) {
		/* Limit the fragment size in the inner TLS authentication
		 * since the outer authentication with EAP-PEAP does not yet
		 * support fragmentation */
		if (data->tls_out_limit > 100)
			data->tls_out_limit -= 100;
	}

	if (config->phase1 &&
	    os_strstr(config->phase1, "include_tls_length=1")) {
		wpa_printf(MSG_DEBUG, "TLS: Include TLS Message Length in "
			   "unfragmented packets");
		data->include_tls_length = 1;
	}

	return 0;
}


/**
 * eap_peer_tls_ssl_deinit - Deinitialize shared TLS functionality
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 *
 * This function deinitializes shared TLS functionality that was initialized
 * with eap_peer_tls_ssl_init().
 */
void eap_peer_tls_ssl_deinit(struct eap_sm *sm, struct eap_ssl_data *data)
{
	tls_connection_deinit(sm->ssl_ctx, data->conn);
	eap_peer_tls_reset_input(data);
	eap_peer_tls_reset_output(data);
}


/**
 * eap_peer_tls_derive_key - Derive a key based on TLS session data
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @label: Label string for deriving the keys, e.g., "client EAP encryption"
 * @len: Length of the key material to generate (usually 64 for MSK)
 * Returns: Pointer to allocated key on success or %NULL on failure
 *
 * This function uses TLS-PRF to generate pseudo-random data based on the TLS
 * session data (client/server random and master key). Each key type may use a
 * different label to bind the key usage into the generated material.
 *
 * The caller is responsible for freeing the returned buffer.
 */
u8 * eap_peer_tls_derive_key(struct eap_sm *sm, struct eap_ssl_data *data,
			     const char *label, size_t len)
{
	struct tls_keys keys;
	u8 *rnd = NULL, *out;

	out = os_malloc(len);
	if (out == NULL)
		return NULL;

	/* First, try to use TLS library function for PRF, if available. */
	if (tls_connection_prf(sm->ssl_ctx, data->conn, label, 0, out, len) ==
	    0)
		return out;

	/*
	 * TLS library did not support key generation, so get the needed TLS
	 * session parameters and use an internal implementation of TLS PRF to
	 * derive the key.
	 */
	if (tls_connection_get_keys(sm->ssl_ctx, data->conn, &keys))
		goto fail;

	if (keys.client_random == NULL || keys.server_random == NULL ||
	    keys.master_key == NULL)
		goto fail;

	rnd = os_malloc(keys.client_random_len + keys.server_random_len);
	if (rnd == NULL)
		goto fail;
	os_memcpy(rnd, keys.client_random, keys.client_random_len);
	os_memcpy(rnd + keys.client_random_len, keys.server_random,
		  keys.server_random_len);

	if (tls_prf(keys.master_key, keys.master_key_len,
		    label, rnd, keys.client_random_len +
		    keys.server_random_len, out, len))
		goto fail;

	os_free(rnd);
	return out;

fail:
	os_free(out);
	os_free(rnd);
	return NULL;
}


/**
 * eap_peer_tls_reassemble_fragment - Reassemble a received fragment
 * @data: Data for TLS processing
 * @in_data: Next incoming TLS segment
 * @in_len: Length of in_data
 * Returns: 0 on success, 1 if more data is needed for the full message, or
 * -1 on error
 */
static int eap_peer_tls_reassemble_fragment(struct eap_ssl_data *data,
					    const u8 *in_data, size_t in_len)
{
	u8 *buf;

	if (data->tls_in_len + in_len == 0) {
		/* No message data received?! */
		wpa_printf(MSG_WARNING, "SSL: Invalid reassembly state: "
			   "tls_in_left=%lu tls_in_len=%lu in_len=%lu",
			   (unsigned long) data->tls_in_left,
			   (unsigned long) data->tls_in_len,
			   (unsigned long) in_len);
		eap_peer_tls_reset_input(data);
		return -1;
	}

	if (data->tls_in_len + in_len > 65536) {
		/*
		 * Limit length to avoid rogue servers from causing large
		 * memory allocations.
		 */
		wpa_printf(MSG_INFO, "SSL: Too long TLS fragment (size over "
			   "64 kB)");
		eap_peer_tls_reset_input(data);
		return -1;
	}

	if (in_len > data->tls_in_left) {
		/* Sender is doing something odd - reject message */
		wpa_printf(MSG_INFO, "SSL: more data than TLS message length "
			   "indicated");
		eap_peer_tls_reset_input(data);
		return -1;
	}

	buf = os_realloc(data->tls_in, data->tls_in_len + in_len);
	if (buf == NULL) {
		wpa_printf(MSG_INFO, "SSL: Could not allocate memory for TLS "
			   "data");
		eap_peer_tls_reset_input(data);
		return -1;
	}
	os_memcpy(buf + data->tls_in_len, in_data, in_len);
	data->tls_in = buf;
	data->tls_in_len += in_len;
	data->tls_in_left -= in_len;

	if (data->tls_in_left > 0) {
		wpa_printf(MSG_DEBUG, "SSL: Need %lu bytes more input "
			   "data", (unsigned long) data->tls_in_left);
		return 1;
	}

	return 0;
}


/**
 * eap_peer_tls_data_reassemble - Reassemble TLS data
 * @data: Data for TLS processing
 * @in_data: Next incoming TLS segment
 * @in_len: Length of in_data
 * @out_len: Variable for returning length of the reassembled message
 * @need_more_input: Variable for returning whether more input data is needed
 * to reassemble this TLS packet
 * Returns: Pointer to output data, %NULL on error or when more data is needed
 * for the full message (in which case, *need_more_input is also set to 1).
 *
 * This function reassembles TLS fragments. Caller must not free the returned
 * data buffer since an internal pointer to it is maintained.
 */
const u8 * eap_peer_tls_data_reassemble(
	struct eap_ssl_data *data, const u8 *in_data, size_t in_len,
	size_t *out_len, int *need_more_input)
{
	*need_more_input = 0;

	if (data->tls_in_left > in_len || data->tls_in) {
		/* Message has fragments */
		int res = eap_peer_tls_reassemble_fragment(data, in_data,
							   in_len);
		if (res) {
			if (res == 1)
				*need_more_input = 1;
			return NULL;
		}

		/* Message is now fully reassembled. */
	} else {
		/* No fragments in this message, so just make a copy of it. */
		data->tls_in_left = 0;
		data->tls_in = os_malloc(in_len ? in_len : 1);
		if (data->tls_in == NULL)
			return NULL;
		os_memcpy(data->tls_in, in_data, in_len);
		data->tls_in_len = in_len;
	}

	*out_len = data->tls_in_len;
	return data->tls_in;
}


/**
 * eap_tls_process_input - Process incoming TLS message
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @in_data: Message received from the server
 * @in_len: Length of in_data
 * @out_data: Buffer for returning a pointer to application data (if available)
 * @out_len: Buffer for returning the length of the application data
 * Returns: 0 on success, 1 if more input data is needed, 2 if application data
 * is available, -1 on failure
 */
static int eap_tls_process_input(struct eap_sm *sm, struct eap_ssl_data *data,
				 const u8 *in_data, size_t in_len,
				 u8 **out_data, size_t *out_len)
{
	const u8 *msg;
	size_t msg_len;
	int need_more_input;
	u8 *appl_data;
	size_t appl_data_len;

	msg = eap_peer_tls_data_reassemble(data, in_data, in_len,
					   &msg_len, &need_more_input);
	if (msg == NULL)
		return need_more_input ? 1 : -1;

	/* Full TLS message reassembled - continue handshake processing */
	if (data->tls_out) {
		/* This should not happen.. */
		wpa_printf(MSG_INFO, "SSL: eap_tls_process_input - pending "
			   "tls_out data even though tls_out_len = 0");
		os_free(data->tls_out);
		WPA_ASSERT(data->tls_out == NULL);
	}
	appl_data = NULL;
	data->tls_out = tls_connection_handshake(sm->ssl_ctx, data->conn,
						 msg, msg_len,
						 &data->tls_out_len,
						 &appl_data, &appl_data_len);

	eap_peer_tls_reset_input(data);

	if (appl_data &&
	    tls_connection_established(sm->ssl_ctx, data->conn) &&
	    !tls_connection_get_failed(sm->ssl_ctx, data->conn)) {
		wpa_hexdump_key(MSG_MSGDUMP, "SSL: Application data",
				appl_data, appl_data_len);
		*out_data = appl_data;
		*out_len = appl_data_len;
		return 2;
	}

	os_free(appl_data);

	return 0;
}


/**
 * eap_tls_process_output - Process outgoing TLS message
 * @data: Data for TLS processing
 * @eap_type: EAP type (EAP_TYPE_TLS, EAP_TYPE_PEAP, ...)
 * @peap_version: Version number for EAP-PEAP/TTLS
 * @id: EAP identifier for the response
 * @ret: Return value to use on success
 * @out_data: Buffer for returning the allocated output buffer
 * @out_len: Buffer for returning the length of the output buffer
 * Returns: ret (0 or 1) on success, -1 on failure
 */
static int eap_tls_process_output(struct eap_ssl_data *data, EapType eap_type,
				  int peap_version, u8 id, int ret,
				  u8 **out_data, size_t *out_len)
{
	size_t len;
	u8 *pos, *flags;
	int more_fragments, length_included;
	
	len = data->tls_out_len - data->tls_out_pos;
	wpa_printf(MSG_DEBUG, "SSL: %lu bytes left to be sent out (of total "
		   "%lu bytes)",
		   (unsigned long) len, (unsigned long) data->tls_out_len);

	/*
	 * Limit outgoing message to the configured maximum size. Fragment
	 * message if needed.
	 */
	if (len > data->tls_out_limit) {
		more_fragments = 1;
		len = data->tls_out_limit;
		wpa_printf(MSG_DEBUG, "SSL: sending %lu bytes, more fragments "
			   "will follow", (unsigned long) len);
	} else
		more_fragments = 0;

	length_included = data->tls_out_pos == 0 &&
		(data->tls_out_len > data->tls_out_limit ||
		 data->include_tls_length);

	*out_data = (u8 *)
		eap_msg_alloc(EAP_VENDOR_IETF, eap_type, out_len,
			      1 + length_included * 4 + len, EAP_CODE_RESPONSE,
			      id, &pos);
	if (*out_data == NULL)
		return -1;

	flags = pos++;
	*flags = peap_version;
	if (more_fragments)
		*flags |= EAP_TLS_FLAGS_MORE_FRAGMENTS;
	if (length_included) {
		*flags |= EAP_TLS_FLAGS_LENGTH_INCLUDED;
		WPA_PUT_BE32(pos, data->tls_out_len);
		pos += 4;
	}

	os_memcpy(pos, &data->tls_out[data->tls_out_pos], len);
	data->tls_out_pos += len;

	if (!more_fragments)
		eap_peer_tls_reset_output(data);

	return ret;
}


/**
 * eap_peer_tls_process_helper - Process TLS handshake message
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @eap_type: EAP type (EAP_TYPE_TLS, EAP_TYPE_PEAP, ...)
 * @peap_version: Version number for EAP-PEAP/TTLS
 * @id: EAP identifier for the response
 * @in_data: Message received from the server
 * @in_len: Length of in_data
 * @out_data: Buffer for returning a pointer to the response message
 * @out_len: Buffer for returning the length of the response message
 * Returns: 0 on success, 1 if more input data is needed, 2 if application data
 * is available, or -1 on failure
 *
 * This function can be used to process TLS handshake messages. It reassembles
 * the received fragments and uses a TLS library to process the messages. The
 * response data from the TLS library is fragmented to suitable output messages
 * that the caller can send out.
 *
 * out_data is used to return the response message if the return value of this
 * function is 0, 2, or -1. In case of failure, the message is likely a TLS
 * alarm message. The caller is responsible for freeing the allocated buffer if
 * *out_data is not %NULL.
 *
 * This function is called for each received TLS message during the TLS
 * handshake after eap_peer_tls_process_init() call and possible processing of
 * TLS Flags field. Once the handshake has been completed, i.e., when
 * tls_connection_established() returns 1, EAP method specific decrypting of
 * the tunneled data is used.
 */
int eap_peer_tls_process_helper(struct eap_sm *sm, struct eap_ssl_data *data,
				EapType eap_type, int peap_version,
				u8 id, const u8 *in_data, size_t in_len,
				u8 **out_data, size_t *out_len)
{
	int ret = 0;

	*out_len = 0;
	*out_data = NULL;

	if (data->tls_out_len > 0 && in_len > 0) {
		wpa_printf(MSG_DEBUG, "SSL: Received non-ACK when output "
			   "fragments are waiting to be sent out");
		return -1;
	}

	if (data->tls_out_len == 0) {
		/*
		 * No more data to send out - expect to receive more data from
		 * the AS.
		 */
		int res = eap_tls_process_input(sm, data, in_data, in_len,
						out_data, out_len);
		if (res) {
			/*
			 * Input processing failed (res = -1) or more data is
			 * needed (res = 1).
			 */
			return res;
		}

		/*
		 * The incoming message has been reassembled and processed. The
		 * response was allocated into data->tls_out buffer.
		 */
	}

	if (data->tls_out == NULL) {
		/*
		 * No outgoing fragments remaining from the previous message
		 * and no new message generated. This indicates an error in TLS
		 * processing.
		 */
		eap_peer_tls_reset_output(data);
		return -1;
	}

	if (tls_connection_get_failed(sm->ssl_ctx, data->conn)) {
		/* TLS processing has failed - return error */
		wpa_printf(MSG_DEBUG, "SSL: Failed - tls_out available to "
			   "report error");
		ret = -1;
		/* TODO: clean pin if engine used? */
	}

	if (data->tls_out_len == 0) {
		/*
		 * TLS negotiation should now be complete since all other cases
		 * needing more data should have been caught above based on
		 * the TLS Message Length field.
		 */
		wpa_printf(MSG_DEBUG, "SSL: No data to be sent out");
		os_free(data->tls_out);
		data->tls_out = NULL;
		return 1;
	}

	/* Send the pending message (in fragments, if needed). */
	return eap_tls_process_output(data, eap_type, peap_version, id, ret,
				      out_data, out_len);
}


/**
 * eap_peer_tls_build_ack - Build a TLS ACK frame
 * @data: Data for TLS processing
 * @respDataLen: Buffer for returning the length of the response message
 * @id: EAP identifier for the response
 * @eap_type: EAP type (EAP_TYPE_TLS, EAP_TYPE_PEAP, ...)
 * @peap_version: Version number for EAP-PEAP/TTLS
 * Returns: Pointer to the allocated ACK frame or %NULL on failure
 */
u8 * eap_peer_tls_build_ack(struct eap_ssl_data *data, size_t *respDataLen,
			    u8 id, EapType eap_type, int peap_version)
{
	struct eap_hdr *resp;
	u8 *pos;

	resp = eap_msg_alloc(EAP_VENDOR_IETF, eap_type, respDataLen,
			     1, EAP_CODE_RESPONSE, id, &pos);
	if (resp == NULL)
		return NULL;
	wpa_printf(MSG_DEBUG, "SSL: Building ACK (type=%d id=%d ver=%d)",
		   (int) eap_type, id, peap_version);
	*pos = peap_version; /* Flags */
	return (u8 *) resp;
}


/**
 * eap_peer_tls_reauth_init - Re-initialize shared TLS for session resumption
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * Returns: 0 on success, -1 on failure
 */
int eap_peer_tls_reauth_init(struct eap_sm *sm, struct eap_ssl_data *data)
{
	eap_peer_tls_reset_input(data);
	eap_peer_tls_reset_output(data);
	return tls_connection_shutdown(sm->ssl_ctx, data->conn);
}


/**
 * eap_peer_tls_status - Get TLS status
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @buf: Buffer for status information
 * @buflen: Maximum buffer length
 * @verbose: Whether to include verbose status information
 * Returns: Number of bytes written to buf.
 */
int eap_peer_tls_status(struct eap_sm *sm, struct eap_ssl_data *data,
			char *buf, size_t buflen, int verbose)
{
	char name[128];
	int len = 0, ret;

	if (tls_get_cipher(sm->ssl_ctx, data->conn, name, sizeof(name)) == 0) {
		ret = os_snprintf(buf + len, buflen - len,
				  "EAP TLS cipher=%s\n", name);
		if (ret < 0 || (size_t) ret >= buflen - len)
			return len;
		len += ret;
	}

	return len;
}


/**
 * eap_peer_tls_process_init - Initial validation/processing of EAP requests
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @eap_type: EAP type (EAP_TYPE_TLS, EAP_TYPE_PEAP, ...)
 * @ret: Return values from EAP request validation and processing
 * @reqData: EAP request to be processed (eapReqData)
 * @reqDataLen: Length of the EAP request
 * @len: Buffer for returning length of the remaining payload
 * @flags: Buffer for returning TLS flags
 * Returns: Buffer to payload after TLS flags and length or %NULL on failure
 *
 * This function validates the EAP header and processes the optional TLS
 * Message Length field. If this is the first fragment of a TLS message, the
 * TLS reassembly code is initialized to receive the indicated number of bytes.
 *
 * EAP-TLS, EAP-PEAP, EAP-TTLS, and EAP-FAST methods are expected to use this
 * function as the first step in processing received messages. They will need
 * to process the flags (apart from Message Length Included) that are returned
 * through the flags pointer and the message payload that will be returned (and
 * the length is returned through the len pointer). Return values (ret) are set
 * for continuation of EAP method processing. The caller is responsible for
 * setting these to indicate completion (either success or failure) based on
 * the authentication result.
 */
const u8 * eap_peer_tls_process_init(struct eap_sm *sm,
				     struct eap_ssl_data *data,
				     EapType eap_type,
				     struct eap_method_ret *ret,
				     const u8 *reqData, size_t reqDataLen,
				     size_t *len, u8 *flags)
{
	const u8 *pos;
	size_t left;
	unsigned int tls_msg_len;

	if (tls_get_errors(sm->ssl_ctx)) {
		wpa_printf(MSG_INFO, "SSL: TLS errors detected");
		ret->ignore = TRUE;
		return NULL;
	}

	pos = eap_hdr_validate(EAP_VENDOR_IETF, eap_type, reqData, reqDataLen,
			       &left);
	if (pos == NULL) {
		ret->ignore = TRUE;
		return NULL;
	}
	*flags = *pos++;
	left--;
	wpa_printf(MSG_DEBUG, "SSL: Received packet(len=%lu) - "
		   "Flags 0x%02x", (unsigned long) reqDataLen, *flags);
	if (*flags & EAP_TLS_FLAGS_LENGTH_INCLUDED) {
		if (left < 4) {
			wpa_printf(MSG_INFO, "SSL: Short frame with TLS "
				   "length");
			ret->ignore = TRUE;
			return NULL;
		}
		tls_msg_len = WPA_GET_BE32(pos);
		wpa_printf(MSG_DEBUG, "SSL: TLS Message Length: %d",
			   tls_msg_len);
		if (data->tls_in_left == 0) {
			data->tls_in_total = tls_msg_len;
			data->tls_in_left = tls_msg_len;
			os_free(data->tls_in);
			data->tls_in = NULL;
			data->tls_in_len = 0;
		}
		pos += 4;
		left -= 4;
	}

	ret->ignore = FALSE;
	ret->methodState = METHOD_MAY_CONT;
	ret->decision = DECISION_FAIL;
	ret->allowNotifications = TRUE;

	*len = left;
	return pos;
}


/**
 * eap_peer_tls_reset_input - Reset input buffers
 * @data: Data for TLS processing
 *
 * This function frees any allocated memory for input buffers and resets input
 * state.
 */
void eap_peer_tls_reset_input(struct eap_ssl_data *data)
{
	data->tls_in_left = data->tls_in_total = data->tls_in_len = 0;
	os_free(data->tls_in);
	data->tls_in = NULL;
}


/**
 * eap_peer_tls_reset_output - Reset output buffers
 * @data: Data for TLS processing
 *
 * This function frees any allocated memory for output buffers and resets
 * output state.
 */
void eap_peer_tls_reset_output(struct eap_ssl_data *data)
{
	data->tls_out_len = 0;
	data->tls_out_pos = 0;
	os_free(data->tls_out);
	data->tls_out = NULL;
}


/**
 * eap_peer_tls_decrypt - Decrypt received phase 2 TLS message
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @in_data: Message received from the server
 * @in_len: Length of in_data
 * @in_decrypted: Buffer for returning a pointer to the decrypted message
 * @len_decrypted: Buffer for returning the length of the decrypted message
 * Returns: 0 on success, 1 if more input data is needed, or -1 on failure
 */
int eap_peer_tls_decrypt(struct eap_sm *sm, struct eap_ssl_data *data,
			 const u8 *in_data, size_t in_len,
			 u8 **in_decrypted, size_t *len_decrypted)
{
	int res;
	const u8 *msg;
	size_t msg_len, buf_len;
	int need_more_input;

	msg = eap_peer_tls_data_reassemble(data, in_data, in_len, &msg_len,
					   &need_more_input);
	if (msg == NULL)
		return need_more_input ? 1 : -1;

	buf_len = in_len;
	if (data->tls_in_total > buf_len)
		buf_len = data->tls_in_total;
	*in_decrypted = os_malloc(buf_len ? buf_len : 1);
	if (*in_decrypted == NULL) {
		eap_peer_tls_reset_input(data);
		wpa_printf(MSG_WARNING, "SSL: Failed to allocate memory for "
			   "decryption");
		return -1;
	}

	res = tls_connection_decrypt(sm->ssl_ctx, data->conn, msg, msg_len,
				     *in_decrypted, buf_len);
	eap_peer_tls_reset_input(data);
	if (res < 0) {
		wpa_printf(MSG_INFO, "SSL: Failed to decrypt Phase 2 data");
		return -1;
	}
	*len_decrypted = res;
	return 0;
}


/**
 * eap_peer_tls_encrypt - Encrypt phase 2 TLS message
 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
 * @data: Data for TLS processing
 * @eap_type: EAP type (EAP_TYPE_TLS, EAP_TYPE_PEAP, ...)
 * @peap_version: Version number for EAP-PEAP/TTLS
 * @id: EAP identifier for the response
 * @in_data: Plaintext phase 2 data to encrypt or %NULL to continue fragments
 * @in_len: Length of in_data
 * @out_data: Buffer for returning a pointer to the encrypted response message
 * @out_len: Buffer for returning the length of the encrypted response message
 * Returns: 0 on success, -1 on failure
 */
int eap_peer_tls_encrypt(struct eap_sm *sm, struct eap_ssl_data *data,
			 EapType eap_type, int peap_version, u8 id,
			 const u8 *in_data, size_t in_len,
			 u8 **out_data, size_t *out_len)
{
	int res;
	size_t len;

	if (in_data) {
		eap_peer_tls_reset_output(data);
		len = in_len + 100;
		data->tls_out = os_malloc(len);
		if (data->tls_out == NULL)
			return -1;

		res = tls_connection_encrypt(sm->ssl_ctx, data->conn, in_data,
					     in_len, data->tls_out, len);
		if (res < 0) {
			wpa_printf(MSG_INFO, "SSL: Failed to encrypt Phase 2 "
				   "data (in_len=%lu)",
				   (unsigned long) in_len);
			eap_peer_tls_reset_output(data);
			return -1;
		}

		data->tls_out_len = res;
	}

	return eap_tls_process_output(data, eap_type, peap_version, id, 0,
				      out_data, out_len);
}


/**
 * eap_peer_select_phase2_methods - Select phase 2 EAP method
 * @config: Pointer to the network configuration
 * @prefix: 'phase2' configuration prefix, e.g., "auth="
 * @types: Buffer for returning allocated list of allowed EAP methods
 * @num_types: Buffer for returning number of allocated EAP methods
 * Returns: 0 on success, -1 on failure
 *
 * This function is used to parse EAP method list and select allowed methods
 * for Phase2 authentication.
 */
int eap_peer_select_phase2_methods(struct wpa_ssid *config, const char *prefix,
				   struct eap_method_type **types,
				   size_t *num_types)
{
	char *start, *pos, *buf;
	struct eap_method_type *methods = NULL, *_methods;
	u8 method;
	size_t num_methods = 0, prefix_len;

	if (config == NULL || config->phase2 == NULL)
		goto get_defaults;

	start = buf = os_strdup(config->phase2);
	if (buf == NULL)
		return -1;

	prefix_len = os_strlen(prefix);

	while (start && *start != '\0') {
		int vendor;
		pos = os_strstr(start, prefix);
		if (pos == NULL)
			break;
		if (start != pos && *(pos - 1) != ' ') {
			start = pos + prefix_len;
			continue;
		}

		start = pos + prefix_len;
		pos = os_strchr(start, ' ');
		if (pos)
			*pos++ = '\0';
		method = eap_get_phase2_type(start, &vendor);
		if (vendor == EAP_VENDOR_IETF && method == EAP_TYPE_NONE) {
			wpa_printf(MSG_ERROR, "TLS: Unsupported Phase2 EAP "
				   "method '%s'", start);
		} else {
			num_methods++;
			_methods = os_realloc(methods,
					      num_methods * sizeof(*methods));
			if (_methods == NULL) {
				os_free(methods);
				os_free(buf);
				return -1;
			}
			methods = _methods;
			methods[num_methods - 1].vendor = vendor;
			methods[num_methods - 1].method = method;
		}

		start = pos;
	}

	os_free(buf);

get_defaults:
	if (methods == NULL)
		methods = eap_get_phase2_types(config, &num_methods);

	if (methods == NULL) {
		wpa_printf(MSG_ERROR, "TLS: No Phase2 EAP methods available");
		return -1;
	}
	wpa_hexdump(MSG_DEBUG, "TLS: Phase2 EAP types",
		    (u8 *) methods,
		    num_methods * sizeof(struct eap_method_type));

	*types = methods;
	*num_types = num_methods;

	return 0;
}


/**
 * eap_peer_tls_phase2_nak - Generate EAP-Nak for Phase 2
 * @types: Buffer for returning allocated list of allowed EAP methods
 * @num_types: Buffer for returning number of allocated EAP methods
 * @hdr: EAP-Request header (and the following EAP type octet)
 * @resp: Buffer for returning the EAP-Nak message
 * @resp_len: Buffer for returning the length of the EAP-Nak message
 * Returns: 0 on success, -1 on failure
 */
int eap_peer_tls_phase2_nak(struct eap_method_type *types, size_t num_types,
			    struct eap_hdr *hdr, u8 **resp, size_t *resp_len)
{
	struct eap_hdr *resp_hdr;
	u8 *pos = (u8 *) (hdr + 1);
	size_t i;

	/* TODO: add support for expanded Nak */
	wpa_printf(MSG_DEBUG, "TLS: Phase 2 Request: Nak type=%d", *pos);
	wpa_hexdump(MSG_DEBUG, "TLS: Allowed Phase2 EAP types",
		    (u8 *) types, num_types * sizeof(struct eap_method_type));
	*resp_len = sizeof(struct eap_hdr) + 1;
	*resp = os_malloc(*resp_len + num_types);
	if (*resp == NULL)
		return -1;

	resp_hdr = (struct eap_hdr *) (*resp);
	resp_hdr->code = EAP_CODE_RESPONSE;
	resp_hdr->identifier = hdr->identifier;
	pos = (u8 *) (resp_hdr + 1);
	*pos++ = EAP_TYPE_NAK;
	for (i = 0; i < num_types; i++) {
		if (types[i].vendor == EAP_VENDOR_IETF &&
		    types[i].method < 256) {
			(*resp_len)++;
			*pos++ = types[i].method;
		}
	}
	resp_hdr->length = host_to_be16(*resp_len);

	return 0;
}
