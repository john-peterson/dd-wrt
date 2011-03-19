/*
 * Received Data frame processing for TDLS packets
 * Copyright (c) 2010, Jouni Malinen <j@w1.fi>
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

#include "utils/includes.h"

#include "utils/common.h"
#include "crypto/sha256.h"
#include "crypto/crypto.h"
#include "crypto/aes_wrap.h"
#include "common/ieee802_11_defs.h"
#include "common/ieee802_11_common.h"
#include "wlantest.h"


static struct wlantest_tdls * get_tdls(struct wlantest *wt, const u8 *linkid)
{
	struct wlantest_bss *bss;
	struct wlantest_sta *init, *resp;
	struct wlantest_tdls *tdls;

	bss = bss_find(wt, linkid);
	if (bss == NULL)
		return NULL;

	init = sta_find(bss, linkid + ETH_ALEN);
	if (init == NULL)
		return NULL;

	resp = sta_find(bss, linkid + 2 * ETH_ALEN);
	if (resp == NULL)
		return NULL;

	dl_list_for_each(tdls, &bss->tdls, struct wlantest_tdls, list) {
		if (tdls->init == init && tdls->resp == resp)
			return tdls;
	}

	tdls = os_zalloc(sizeof(*tdls));
	if (tdls == NULL)
		return NULL;
	tdls->init = init;
	tdls->resp = resp;
	dl_list_add(&bss->tdls, &tdls->list);
	return tdls;
}


static int tdls_derive_tpk(struct wlantest_tdls *tdls, const u8 *bssid,
			   const u8 *ftie, u8 ftie_len)
{
	const struct rsn_ftie *f;
	u8 key_input[SHA256_MAC_LEN];
	const u8 *nonce[2];
	size_t len[2];
	u8 data[3 * ETH_ALEN];

	if (ftie == NULL || ftie_len < sizeof(struct rsn_ftie))
		return 0;

	f = (const struct rsn_ftie *) ftie;
	wpa_hexdump(MSG_DEBUG, "TDLS ANonce", f->anonce, WPA_NONCE_LEN);
	wpa_hexdump(MSG_DEBUG, "TDLS SNonce", f->snonce, WPA_NONCE_LEN);

	/*
	 * IEEE Std 802.11z-2010 8.5.9.1:
	 * TPK-Key-Input = SHA-256(min(SNonce, ANonce) || max(SNonce, ANonce))
	 */
	len[0] = WPA_NONCE_LEN;
	len[1] = WPA_NONCE_LEN;
	if (os_memcmp(f->anonce, f->snonce, WPA_NONCE_LEN) < 0) {
		nonce[0] = f->anonce;
		nonce[1] = f->snonce;
	} else {
		nonce[0] = f->snonce;
		nonce[1] = f->anonce;
	}
	sha256_vector(2, nonce, len, key_input);
	wpa_hexdump_key(MSG_DEBUG, "TDLS: TPK-Key-Input",
			key_input, SHA256_MAC_LEN);

	/*
	 * TPK-Key-Data = KDF-N_KEY(TPK-Key-Input, "TDLS PMK",
	 *	min(MAC_I, MAC_R) || max(MAC_I, MAC_R) || BSSID || N_KEY)
	 * TODO: is N_KEY really included in KDF Context and if so, in which
	 * presentation format (little endian 16-bit?) is it used? It gets
	 * added by the KDF anyway..
	 */

	if (os_memcmp(tdls->init->addr, tdls->resp->addr, ETH_ALEN) < 0) {
		os_memcpy(data, tdls->init->addr, ETH_ALEN);
		os_memcpy(data + ETH_ALEN, tdls->resp->addr, ETH_ALEN);
	} else {
		os_memcpy(data, tdls->resp->addr, ETH_ALEN);
		os_memcpy(data + ETH_ALEN, tdls->init->addr, ETH_ALEN);
	}
	os_memcpy(data + 2 * ETH_ALEN, bssid, ETH_ALEN);
	wpa_hexdump(MSG_DEBUG, "TDLS: KDF Context", data, sizeof(data));

	sha256_prf(key_input, SHA256_MAC_LEN, "TDLS PMK", data, sizeof(data),
		   (u8 *) &tdls->tpk, sizeof(tdls->tpk));
	wpa_hexdump_key(MSG_DEBUG, "TDLS: TPK-KCK",
			tdls->tpk.kck, sizeof(tdls->tpk.kck));
	wpa_hexdump_key(MSG_DEBUG, "TDLS: TPK-TK",
			tdls->tpk.tk, sizeof(tdls->tpk.tk));

	return 1;
}


static int tdls_verify_mic(struct wlantest_tdls *tdls, u8 trans_seq,
			   struct ieee802_11_elems *elems)
{
	u8 *buf, *pos;
	int len;
	u8 mic[16];
	int ret;
	const struct rsn_ftie *rx_ftie;
	struct rsn_ftie *tmp_ftie;

	if (elems->link_id == NULL || elems->rsn_ie == NULL ||
	    elems->timeout_int == NULL || elems->ftie == NULL)
		return -1;

	len = 2 * ETH_ALEN + 1 + 2 + 18 + 2 + elems->rsn_ie_len +
		2 + elems->timeout_int_len + 2 + elems->ftie_len;

	buf = os_zalloc(len);
	if (buf == NULL)
		return -1;

	pos = buf;
	/* 1) TDLS initiator STA MAC address */
	os_memcpy(pos, elems->link_id + ETH_ALEN, ETH_ALEN);
	pos += ETH_ALEN;
	/* 2) TDLS responder STA MAC address */
	os_memcpy(pos, elems->link_id + 2 * ETH_ALEN, ETH_ALEN);
	pos += ETH_ALEN;
	/* 3) Transaction Sequence number */
	*pos++ = trans_seq;
	/* 4) Link Identifier IE */
	os_memcpy(pos, elems->link_id - 2, 2 + 18);
	pos += 2 + 18;
	/* 5) RSN IE */
	os_memcpy(pos, elems->rsn_ie - 2, 2 + elems->rsn_ie_len);
	pos += 2 + elems->rsn_ie_len;
	/* 6) Timeout Interval IE */
	os_memcpy(pos, elems->timeout_int - 2, 2 + elems->timeout_int_len);
	pos += 2 + elems->timeout_int_len;
	/* 7) FTIE, with the MIC field of the FTIE set to 0 */
	os_memcpy(pos, elems->ftie - 2, 2 + elems->ftie_len);
	pos += 2;
	tmp_ftie = (struct rsn_ftie *) pos;
	os_memset(tmp_ftie->mic, 0, 16);
	pos += elems->ftie_len;

	wpa_hexdump(MSG_DEBUG, "TDLS: Data for FTIE MIC", buf, pos - buf);
	wpa_hexdump_key(MSG_DEBUG, "TDLS: KCK", tdls->tpk.kck, 16);
	ret = omac1_aes_128(tdls->tpk.kck, buf, pos - buf, mic);
	os_free(buf);
	if (ret)
		return -1;
	wpa_hexdump(MSG_DEBUG, "TDLS: FTIE MIC", mic, 16);
	rx_ftie = (const struct rsn_ftie *) elems->ftie;

	if (os_memcmp(mic, rx_ftie->mic, 16) == 0) {
		wpa_printf(MSG_DEBUG, "TDLS: Valid MIC");
		return 0;
	}
	wpa_printf(MSG_DEBUG, "TDLS: Invalid MIC");
	return -1;
}


static void rx_data_tdls_setup_request(struct wlantest *wt, const u8 *bssid,
				       const u8 *sta_addr, const u8 *dst,
				       const u8 *src,
				       const u8 *data, size_t len)
{
	struct ieee802_11_elems elems;

	if (len < 3)
		return;
	wpa_printf(MSG_DEBUG, "TDLS Setup Request " MACSTR " -> "
		   MACSTR, MAC2STR(src), MAC2STR(dst));

	if (ieee802_11_parse_elems(data + 3, len - 3, &elems, 1) ==
	    ParseFailed || elems.link_id == NULL)
		return;
	wpa_printf(MSG_DEBUG, "TDLS Link Identifier: BSSID " MACSTR
		   " initiator STA " MACSTR " responder STA " MACSTR,
		   MAC2STR(elems.link_id), MAC2STR(elems.link_id + ETH_ALEN),
		   MAC2STR(elems.link_id + 2 * ETH_ALEN));
}


static void rx_data_tdls_setup_response(struct wlantest *wt, const u8 *bssid,
					const u8 *sta_addr, const u8 *dst,
					const u8 *src,
					const u8 *data, size_t len)
{
	u16 status;
	struct ieee802_11_elems elems;
	struct wlantest_tdls *tdls;

	if (len < 5)
		return;
	status = WPA_GET_LE16(data);
	wpa_printf(MSG_DEBUG, "TDLS Setup Response " MACSTR " -> "
		   MACSTR " (status %d)",
		   MAC2STR(src), MAC2STR(dst), status);
	if (status != WLAN_STATUS_SUCCESS)
		return;

	if (ieee802_11_parse_elems(data + 5, len - 5, &elems, 1) ==
	    ParseFailed || elems.link_id == NULL)
		return;
	wpa_printf(MSG_DEBUG, "TDLS Link Identifier: BSSID " MACSTR
		   " initiator STA " MACSTR " responder STA " MACSTR,
		   MAC2STR(elems.link_id), MAC2STR(elems.link_id + ETH_ALEN),
		   MAC2STR(elems.link_id + 2 * ETH_ALEN));

	tdls = get_tdls(wt, elems.link_id);
	if (!tdls)
		return;
	if (tdls_derive_tpk(tdls, bssid, elems.ftie, elems.ftie_len) < 1)
		return;
	if (tdls_verify_mic(tdls, 2, &elems) == 0) {
		tdls->dialog_token = data[2];
		wpa_printf(MSG_DEBUG, "TDLS: Dialog Token for the link: %u",
			   tdls->dialog_token);
	}
}


static void rx_data_tdls_setup_confirm(struct wlantest *wt, const u8 *bssid,
				       const u8 *sta_addr, const u8 *dst,
				       const u8 *src,
				       const u8 *data, size_t len)
{
	u16 status;
	struct ieee802_11_elems elems;
	struct wlantest_tdls *tdls;

	if (len < 3)
		return;
	status = WPA_GET_LE16(data);
	wpa_printf(MSG_DEBUG, "TDLS Setup Confirm " MACSTR " -> "
		   MACSTR " (status %d)",
		   MAC2STR(src), MAC2STR(dst), status);
	if (status != WLAN_STATUS_SUCCESS)
		return;

	if (ieee802_11_parse_elems(data + 3, len - 3, &elems, 1) ==
	    ParseFailed || elems.link_id == NULL)
		return;
	wpa_printf(MSG_DEBUG, "TDLS Link Identifier: BSSID " MACSTR
		   " initiator STA " MACSTR " responder STA " MACSTR,
		   MAC2STR(elems.link_id), MAC2STR(elems.link_id + ETH_ALEN),
		   MAC2STR(elems.link_id + 2 * ETH_ALEN));

	tdls = get_tdls(wt, elems.link_id);
	if (tdls == NULL)
		return;

	tdls->link_up = 1;
	if (tdls_derive_tpk(tdls, bssid, elems.ftie, elems.ftie_len) < 1)
		return;
	if (tdls_verify_mic(tdls, 3, &elems) == 0) {
		tdls->dialog_token = data[2];
		wpa_printf(MSG_DEBUG, "TDLS: Dialog Token for the link: %u",
			   tdls->dialog_token);
	}
}


static int tdls_verify_mic_teardown(struct wlantest_tdls *tdls, u8 trans_seq,
				    const u8 *reason_code,
				    struct ieee802_11_elems *elems)
{
	u8 *buf, *pos;
	int len;
	u8 mic[16];
	int ret;
	const struct rsn_ftie *rx_ftie;
	struct rsn_ftie *tmp_ftie;

	if (elems->link_id == NULL || elems->ftie == NULL)
		return -1;

	len = 2 + 18 + 2 + 1 + 1 + 2 + elems->ftie_len;

	buf = os_zalloc(len);
	if (buf == NULL)
		return -1;

	pos = buf;
	/* 1) Link Identifier IE */
	os_memcpy(pos, elems->link_id - 2, 2 + 18);
	pos += 2 + 18;
	/* 2) Reason Code */
	os_memcpy(pos, reason_code, 2);
	pos += 2;
	/* 3) Dialog token */
	*pos++ = tdls->dialog_token;
	/* 4) Transaction Sequence number */
	*pos++ = trans_seq;
	/* 5) FTIE, with the MIC field of the FTIE set to 0 */
	os_memcpy(pos, elems->ftie - 2, 2 + elems->ftie_len);
	pos += 2;
	tmp_ftie = (struct rsn_ftie *) pos;
	os_memset(tmp_ftie->mic, 0, 16);
	pos += elems->ftie_len;

	wpa_hexdump(MSG_DEBUG, "TDLS: Data for FTIE MIC", buf, pos - buf);
	wpa_hexdump_key(MSG_DEBUG, "TDLS: KCK", tdls->tpk.kck, 16);
	ret = omac1_aes_128(tdls->tpk.kck, buf, pos - buf, mic);
	os_free(buf);
	if (ret)
		return -1;
	wpa_hexdump(MSG_DEBUG, "TDLS: FTIE MIC", mic, 16);
	rx_ftie = (const struct rsn_ftie *) elems->ftie;

	if (os_memcmp(mic, rx_ftie->mic, 16) == 0) {
		wpa_printf(MSG_DEBUG, "TDLS: Valid MIC");
		return 0;
	}
	wpa_printf(MSG_DEBUG, "TDLS: Invalid MIC");
	return -1;
}


static void rx_data_tdls_teardown(struct wlantest *wt, const u8 *bssid,
				  const u8 *sta_addr, const u8 *dst,
				  const u8 *src,
				  const u8 *data, size_t len)
{
	u16 reason;
	struct ieee802_11_elems elems;
	struct wlantest_tdls *tdls;

	if (len < 2)
		return;
	reason = WPA_GET_LE16(data);
	wpa_printf(MSG_DEBUG, "TDLS Teardown " MACSTR " -> "
		   MACSTR " (reason %d)",
		   MAC2STR(src), MAC2STR(dst), reason);

	if (ieee802_11_parse_elems(data + 2, len - 2, &elems, 1) ==
	    ParseFailed || elems.link_id == NULL)
		return;
	wpa_printf(MSG_DEBUG, "TDLS Link Identifier: BSSID " MACSTR
		   " initiator STA " MACSTR " responder STA " MACSTR,
		   MAC2STR(elems.link_id), MAC2STR(elems.link_id + ETH_ALEN),
		   MAC2STR(elems.link_id + 2 * ETH_ALEN));

	tdls = get_tdls(wt, elems.link_id);
	if (tdls) {
		tdls->link_up = 0;
		tdls_verify_mic_teardown(tdls, 4, data, &elems);
	}
}


static void rx_data_tdls(struct wlantest *wt, const u8 *bssid,
			 const u8 *sta_addr, const u8 *dst, const u8 *src,
			 const u8 *data, size_t len)
{
	/* data contains the payload of a TDLS Action frame */
	if (len < 2 || data[0] != WLAN_ACTION_TDLS) {
		wpa_hexdump(MSG_DEBUG, "Unrecognized encapsulated TDLS frame",
			    data, len);
		return;
	}

	switch (data[1]) {
	case WLAN_TDLS_SETUP_REQUEST:
		rx_data_tdls_setup_request(wt, bssid, sta_addr, dst, src,
					   data + 2, len - 2);
		break;
	case WLAN_TDLS_SETUP_RESPONSE:
		rx_data_tdls_setup_response(wt, bssid, sta_addr, dst, src,
					    data + 2, len - 2);
		break;
	case WLAN_TDLS_SETUP_CONFIRM:
		rx_data_tdls_setup_confirm(wt, bssid, sta_addr, dst, src,
					   data + 2, len - 2);
		break;
	case WLAN_TDLS_TEARDOWN:
		rx_data_tdls_teardown(wt, bssid, sta_addr, dst, src, data + 2,
				      len - 2);
		break;
	case WLAN_TDLS_DISCOVERY_REQUEST:
		wpa_printf(MSG_DEBUG, "TDLS Discovery Request " MACSTR " -> "
			   MACSTR, MAC2STR(src), MAC2STR(dst));
		break;
	}
}


void rx_data_80211_encap(struct wlantest *wt, const u8 *bssid,
			 const u8 *sta_addr, const u8 *dst, const u8 *src,
			 const u8 *data, size_t len)
{
	wpa_hexdump(MSG_EXCESSIVE, "802.11 data encap frame", data, len);
	if (len < 1)
		return;
	if (data[0] == 0x02)
		rx_data_tdls(wt, bssid, sta_addr, dst, src, data + 1, len - 1);
}
