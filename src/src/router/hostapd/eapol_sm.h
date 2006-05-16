/*
 * hostapd / IEEE 802.1X Authenticator - EAPOL state machine
 * Copyright (c) 2002-2005, Jouni Malinen <jkmaline@cc.hut.fi>
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

#ifndef EAPOL_SM_H
#define EAPOL_SM_H

#include "defs.h"

/* IEEE Std 802.1X-2004, Ch. 8.2 */

typedef enum { ForceUnauthorized = 1, ForceAuthorized = 3, Auto = 2 }
	PortTypes;
typedef enum { Unauthorized = 2, Authorized = 1 } PortState;
typedef enum { Both = 0, In = 1 } ControlledDirection;
typedef unsigned int Counter;

struct eap_sm;

struct radius_attr_data {
	u8 *data;
	size_t len;
};

struct radius_class_data {
	struct radius_attr_data *attr;
	size_t count;
};

struct eapol_state_machine {
	/* timers */
	int aWhile;
	int quietWhile;
	int reAuthWhen;

	/* global variables */
	Boolean authAbort;
	Boolean authFail;
	PortState authPortStatus;
	Boolean authStart;
	Boolean authTimeout;
	Boolean authSuccess;
	Boolean eapFail;
	Boolean eapolEap;
	Boolean eapSuccess;
	Boolean eapTimeout;
	Boolean initialize;
	Boolean keyAvailable;
	Boolean keyDone;
	Boolean keyRun;
	Boolean keyTxEnabled;
	PortTypes portControl;
	Boolean portEnabled;
	Boolean portValid;
	Boolean reAuthenticate;

	/* Port Timers state machine */
	/* 'Boolean tick' implicitly handled as registered timeout */

	/* Authenticator PAE state machine */
	enum { AUTH_PAE_INITIALIZE, AUTH_PAE_DISCONNECTED, AUTH_PAE_CONNECTING,
	       AUTH_PAE_AUTHENTICATING, AUTH_PAE_AUTHENTICATED,
	       AUTH_PAE_ABORTING, AUTH_PAE_HELD, AUTH_PAE_FORCE_AUTH,
	       AUTH_PAE_FORCE_UNAUTH, AUTH_PAE_RESTART } auth_pae_state;
	/* variables */
	Boolean eapolLogoff;
	Boolean eapolStart;
	Boolean eapRestart;
	PortTypes portMode;
	unsigned int reAuthCount;
	/* constants */
	unsigned int quietPeriod; /* default 60; 0..65535 */
#define AUTH_PAE_DEFAULT_quietPeriod 60
	unsigned int reAuthMax; /* default 2 */
#define AUTH_PAE_DEFAULT_reAuthMax 2
	/* counters */
	Counter authEntersConnecting;
	Counter authEapLogoffsWhileConnecting;
	Counter authEntersAuthenticating;
	Counter authAuthSuccessesWhileAuthenticating;
	Counter authAuthTimeoutsWhileAuthenticating;
	Counter authAuthFailWhileAuthenticating;
	Counter authAuthEapStartsWhileAuthenticating;
	Counter authAuthEapLogoffWhileAuthenticating;
	Counter authAuthReauthsWhileAuthenticated;
	Counter authAuthEapStartsWhileAuthenticated;
	Counter authAuthEapLogoffWhileAuthenticated;

	/* Backend Authentication state machine */
	enum { BE_AUTH_REQUEST, BE_AUTH_RESPONSE, BE_AUTH_SUCCESS,
	       BE_AUTH_FAIL, BE_AUTH_TIMEOUT, BE_AUTH_IDLE, BE_AUTH_INITIALIZE,
	       BE_AUTH_IGNORE
	} be_auth_state;
	/* variables */
	Boolean eapNoReq;
	Boolean eapReq;
	Boolean eapResp;
	/* constants */
	unsigned int serverTimeout; /* default 30; 1..X */
#define BE_AUTH_DEFAULT_serverTimeout 30
	/* counters */
	Counter backendResponses;
	Counter backendAccessChallenges;
	Counter backendOtherRequestsToSupplicant;
	Counter backendAuthSuccesses;
	Counter backendAuthFails;

	/* Reauthentication Timer state machine */
	enum { REAUTH_TIMER_INITIALIZE, REAUTH_TIMER_REAUTHENTICATE
	} reauth_timer_state;
	/* constants */
	unsigned int reAuthPeriod; /* default 3600 s */
	Boolean reAuthEnabled;

	/* Authenticator Key Transmit state machine */
	enum { AUTH_KEY_TX_NO_KEY_TRANSMIT, AUTH_KEY_TX_KEY_TRANSMIT
	} auth_key_tx_state;

	/* Key Receive state machine */
	enum { KEY_RX_NO_KEY_RECEIVE, KEY_RX_KEY_RECEIVE } key_rx_state;
	/* variables */
	Boolean rxKey;

	/* Controlled Directions state machine */
	enum { CTRL_DIR_FORCE_BOTH, CTRL_DIR_IN_OR_BOTH } ctrl_dir_state;
	/* variables */
	ControlledDirection adminControlledDirections;
	ControlledDirection operControlledDirections;
	Boolean operEdge;

	/* Authenticator Statistics Table */
	Counter dot1xAuthEapolFramesRx;
	Counter dot1xAuthEapolFramesTx;
	Counter dot1xAuthEapolStartFramesRx;
	Counter dot1xAuthEapolLogoffFramesRx;
	Counter dot1xAuthEapolRespIdFramesRx;
	Counter dot1xAuthEapolRespFramesRx;
	Counter dot1xAuthEapolReqIdFramesTx;
	Counter dot1xAuthEapolReqFramesTx;
	Counter dot1xAuthInvalidEapolFramesRx;
	Counter dot1xAuthEapLengthErrorFramesRx;
	Counter dot1xAuthLastEapolFrameVersion;

	/* Other variables - not defined in IEEE 802.1X */
	u8 addr[ETH_ALEN]; /* Supplicant address */
#define EAPOL_SM_PREAUTH BIT(0)
	int flags; /* EAPOL_SM_* */

	int radius_identifier;
	/* TODO: check when the last messages can be released */
	struct radius_msg *last_recv_radius;
	u8 *last_eap_supp; /* last received EAP Response from Supplicant */
	size_t last_eap_supp_len;
	u8 *last_eap_radius; /* last received EAP Response from Authentication
			      * Server */
	size_t last_eap_radius_len;
	u8 *identity;
	size_t identity_len;
	struct radius_class_data radius_class;

	/* Keys for encrypting and signing EAPOL-Key frames */
	u8 *eapol_key_sign;
	size_t eapol_key_sign_len;
	u8 *eapol_key_crypt;
	size_t eapol_key_crypt_len;

	Boolean rx_identity; /* set to TRUE on reception of
			      * EAP-Response/Identity */

	struct eap_sm *eap;

	/* currentId was removed in IEEE 802.1X-REV, but it is needed to filter
	 * out EAP-Responses to old packets (e.g., to two EAP-Request/Identity
	 * packets that are often sent in the beginning of the authentication).
	 */
	u8 currentId;

	Boolean initializing; /* in process of initializing state machines */
	Boolean changed;

	/* Somewhat nasty pointers to global hostapd and STA data to avoid
	 * passing these to every function */
	struct hostapd_data *hapd;
	struct sta_info *sta;
};


struct eapol_state_machine *eapol_sm_alloc(struct hostapd_data *hapd,
					   struct sta_info *sta);
void eapol_sm_free(struct eapol_state_machine *sm);
void eapol_sm_step(struct eapol_state_machine *sm);
void eapol_sm_initialize(struct eapol_state_machine *sm);
void eapol_sm_dump_state(FILE *f, const char *prefix,
			 struct eapol_state_machine *sm);
int eapol_sm_eap_pending_cb(struct eapol_state_machine *sm, void *ctx);

#endif /* EAPOL_SM_H */
