/*
 *  Unix SMB/CIFS implementation.
 *  Virtual Windows Registry Layer
 *  Copyright (C) Gerald Carter     2002-2005
 *  Copyright (C) Michael Adam      2008
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Product options registry backend.
 *
 * This replaces the former dynamic product options overlay.
 */

#include "includes.h"

#undef DBGC_CLASS
#define DBGC_CLASS DBGC_REGISTRY

extern REGISTRY_OPS regdb_ops;

static int prod_options_fetch_values(const char *key, REGVAL_CTR *regvals)
{
	const char *value_ascii = "";
	fstring value;
	int value_length;

	switch (lp_server_role()) {
		case ROLE_DOMAIN_PDC:
		case ROLE_DOMAIN_BDC:
			value_ascii = "LanmanNT";
			break;
		case ROLE_STANDALONE:
			value_ascii = "ServerNT";
			break;
		case ROLE_DOMAIN_MEMBER:
			value_ascii = "WinNT";
			break;
	}

	value_length = push_ucs2(value, value, value_ascii, sizeof(value),
				 STR_TERMINATE|STR_NOALIGN );
	regval_ctr_addvalue(regvals, "ProductType", REG_SZ, value,
			    value_length);

	return regval_ctr_numvals( regvals );
}

static int prod_options_fetch_subkeys(const char *key,
				      struct regsubkey_ctr *subkey_ctr)
{
	return regdb_ops.fetch_subkeys(key, subkey_ctr);
}

REGISTRY_OPS prod_options_reg_ops = {
	.fetch_values = prod_options_fetch_values,
	.fetch_subkeys = prod_options_fetch_subkeys,
};
