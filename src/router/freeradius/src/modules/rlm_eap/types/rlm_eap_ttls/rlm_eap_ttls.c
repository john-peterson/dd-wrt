/*
 * rlm_eap_ttls.c  contains the interfaces that are called from eap
 *
 * Version:     $Id: rlm_eap_ttls.c,v 1.5.4.2 2007/02/16 09:31:34 aland Exp $
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Copyright 2003 Alan DeKok <aland@freeradius.org>
 */

#include "autoconf.h"
#include "eap_ttls.h"


typedef struct rlm_eap_ttls_t {
	/*
	 *	Default tunneled EAP type
	 */
	char	*default_eap_type_name;
	int	default_eap_type;

	/*
	 *	Use the reply attributes from the tunneled session in
	 *	the non-tunneled reply to the client.
	 */
	int	use_tunneled_reply;

	/*
	 *	Use SOME of the request attributes from outside of the
	 *	tunneled session in the tunneled request
	 */
	int	copy_request_to_tunnel;
} rlm_eap_ttls_t;


static CONF_PARSER module_config[] = {
	{ "default_eap_type", PW_TYPE_STRING_PTR,
	  offsetof(rlm_eap_ttls_t, default_eap_type_name), NULL, "md5" },

	{ "copy_request_to_tunnel", PW_TYPE_BOOLEAN,
	  offsetof(rlm_eap_ttls_t, copy_request_to_tunnel), NULL, "no" },

	{ "use_tunneled_reply", PW_TYPE_BOOLEAN,
	  offsetof(rlm_eap_ttls_t, use_tunneled_reply), NULL, "no" },

 	{ NULL, -1, 0, NULL, NULL }           /* end the list */
};

/*
 *	Detach the module.
 */
static int eapttls_detach(void *arg)
{
	rlm_eap_ttls_t *inst = (rlm_eap_ttls_t *) arg;

	if (inst->default_eap_type_name) free(inst->default_eap_type_name);

	free(inst);

	return 0;
}

/*
 *	Attach the module.
 */
static int eapttls_attach(CONF_SECTION *cs, void **instance)
{
	rlm_eap_ttls_t *inst;

	inst = malloc(sizeof(*inst));
	if (!inst) {
		radlog(L_ERR, "rlm_eap_ttls: out of memory");
		return -1;
	}
	memset(inst, 0, sizeof(*inst));

	/*
	 *	Parse the configuration attributes.
	 */
	if (cf_section_parse(cs, inst, module_config) < 0) {
		eapttls_detach(inst);
		return -1;
	}

	/*
	 *	Convert the name to an integer, to make it easier to
	 *	handle.
	 */
	inst->default_eap_type = eaptype_name2type(inst->default_eap_type_name);
	if (inst->default_eap_type < 0) {
		radlog(L_ERR, "rlm_eap_ttls: Unknown EAP type %s",
		       inst->default_eap_type_name);
		eapttls_detach(inst);
		return -1;
	}

	/*
	 *	Can't tunnel TLS inside of TLS, we don't like it.
	 *
	 *	More realistically, we haven't tested it, so we don't
	 *	claim it works.
	 */
	if ((inst->default_eap_type == PW_EAP_TLS) ||
	    (inst->default_eap_type == PW_EAP_TTLS) ||
	    (inst->default_eap_type == PW_EAP_PEAP)) {
		radlog(L_ERR, "rlm_eap_ttls: Cannot tunnel EAP-Type/%s inside of TTLS",
		       inst->default_eap_type_name);
		eapttls_detach(inst);
		return -1;
	}

	*instance = inst;
	return 0;
}


/*
 *	Free the TTLS per-session data
 */
static void ttls_free(void *p)
{
	ttls_tunnel_t *t = (ttls_tunnel_t *) p;

	if (!t) return;

	if (t->username) {
		DEBUG2("  TTLS: Freeing handler for user %s",
		       t->username->strvalue);
	}

	pairfree(&t->username);
	pairfree(&t->state);
	pairfree(&t->reply);
	free(t);
}


/*
 *	Allocate the TTLS per-session data
 */
static ttls_tunnel_t *ttls_alloc(rlm_eap_ttls_t *inst)
{
	ttls_tunnel_t *t;

	t = rad_malloc(sizeof(*t));
	memset(t, 0, sizeof(*t));

	t->default_eap_type = inst->default_eap_type;
	t->copy_request_to_tunnel = inst->copy_request_to_tunnel;
	t->use_tunneled_reply = inst->use_tunneled_reply;
	return t;
}


/*
 *	Do authentication, by letting EAP-TLS do most of the work.
 */
static int eapttls_authenticate(void *arg, EAP_HANDLER *handler)
{
	int rcode;
	eaptls_status_t	status;
	rlm_eap_ttls_t *inst = (rlm_eap_ttls_t *) arg;
	tls_session_t *tls_session = (tls_session_t *) handler->opaque;
	ttls_tunnel_t *t = (ttls_tunnel_t *) tls_session->opaque;

	DEBUG2("  rlm_eap_ttls: Authenticate");

	/*
	 *	Process TLS layer until done.
	 */
	status = eaptls_process(handler);
	DEBUG2("  eaptls_process returned %d\n", status);
	switch (status) {
		/*
		 *	EAP-TLS handshake was successful, tell the
		 *	client to keep talking.
		 *
		 *	If this was EAP-TLS, we would just return
		 *	an EAP-TLS-Success packet here.
		 */
	case EAPTLS_SUCCESS:
		if (t->authenticated) {
			if (t->reply) {
				pairadd(&handler->request->reply->vps, t->reply);
				t->reply = NULL;
			}
			eaptls_success(handler->eap_ds, 0);
			eaptls_gen_mppe_keys(&handler->request->reply->vps,
					     tls_session->ssl,
					     "ttls keying material");
		} else {
			eaptls_request(handler->eap_ds, tls_session);
		}
		return 1;

		/*
		 *	The TLS code is still working on the TLS
		 *	exchange, and it's a valid TLS request.
		 *	do nothing.
		 */
	case EAPTLS_HANDLED:
		return 1;

		/*
		 *	Handshake is done, proceed with decoding tunneled
		 *	data.
		 */
	case EAPTLS_OK:
		break;

		/*
		 *	Anything else: fail.
		 */
	default:
		return 0;
	}

	/*
	 *	Session is established, proceed with decoding
	 *	tunneled data.
	 */
	DEBUG2("  rlm_eap_ttls: Session established.  Proceeding to decode tunneled attributes.");

	/*
	 *	We may need TTLS data associated with the session, so
	 *	allocate it here, if it wasn't already alloacted.
	 */
	if (!tls_session->opaque) {
		tls_session->opaque = ttls_alloc(inst);
		tls_session->free_opaque = ttls_free;
	}

	/*
	 *	Process the TTLS portion of the request.
	 */
	rcode = eapttls_process(handler, tls_session);
	switch (rcode) {
	case PW_AUTHENTICATION_REJECT:
		eaptls_fail(handler->eap_ds, 0);
		return 0;

		/*
		 *	Access-Challenge, continue tunneled conversation.
		 */
	case PW_ACCESS_CHALLENGE:
		eaptls_request(handler->eap_ds, tls_session);
		return 1;

		/*
		 *	Success: Return MPPE keys.
		 */
	case PW_AUTHENTICATION_ACK:
		eaptls_success(handler->eap_ds, 0);
		eaptls_gen_mppe_keys(&handler->request->reply->vps,
				     tls_session->ssl,
				     "ttls keying material");
		return 1;

		/*
		 *	No response packet, MUST be proxying it.
		 *	The main EAP module will take care of discovering
		 *	that the request now has a "proxy" packet, and
		 *	will proxy it, rather than returning an EAP packet.
		 */
	case PW_STATUS_CLIENT:
		rad_assert(handler->request->proxy != NULL);
		return 1;
		break;

	default:
		break;
	}

	/*
	 *	Something we don't understand: Reject it.
	 */
	eaptls_fail(handler->eap_ds, 0);
	return 0;
}

/*
 *	The module name should be the only globally exported symbol.
 *	That is, everything else should be 'static'.
 */
EAP_TYPE rlm_eap_ttls = {
	"eap_ttls",
	eapttls_attach,			/* attach */
	/*
	 *	Note! There is NO eapttls_initate() function, as the
	 *	main EAP module takes care of calling
	 *	eaptls_initiate().
	 *
	 *	This is because TTLS is a protocol on top of TLS, so
	 *	before we need to do TTLS, we've got to initiate a TLS
	 *	session.
	 */
	NULL,				/* Start the initial request */
	NULL,				/* authorization */
	eapttls_authenticate,		/* authentication */
	eapttls_detach			/* detach */
};
