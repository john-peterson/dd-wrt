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

#include "includes.h"
#ifndef CONFIG_NATIVE_WINDOWS
#include <grp.h>
#endif /* CONFIG_NATIVE_WINDOWS */

#include "hostapd.h"
#include "driver.h"
#include "sha1.h"
#include "eap.h"
#include "radius_client.h"
#include "wpa_common.h"


static void hostapd_config_defaults_bss(struct hostapd_bss_config *bss)
{
	bss->radius = (struct hostapd_radius_servers *) (bss + 1);

	bss->logger_syslog_level = HOSTAPD_LEVEL_INFO;
	bss->logger_stdout_level = HOSTAPD_LEVEL_INFO;
	bss->logger_syslog = (unsigned int) -1;
	bss->logger_stdout = (unsigned int) -1;

	bss->auth_algs = HOSTAPD_AUTH_OPEN | HOSTAPD_AUTH_SHARED_KEY;

	bss->wep_rekeying_period = 300;
	bss->eap_reauth_period = 3600;

	bss->wpa_group_rekey = 600;
	bss->wpa_gmk_rekey = 86400;
	bss->wpa_key_mgmt = WPA_KEY_MGMT_PSK;
	bss->wpa_pairwise = WPA_CIPHER_TKIP;
	bss->wpa_group = WPA_CIPHER_TKIP;

	bss->radius_server_auth_port = 1812;
	bss->ap_max_inactivity = AP_MAX_INACTIVITY;
	bss->eapol_version = EAPOL_VERSION;
}


static struct hostapd_config * hostapd_config_defaults(void)
{
	struct hostapd_config *conf;
	struct hostapd_bss_config *bss;

	conf = wpa_zalloc(sizeof(*conf));
	bss = wpa_zalloc(sizeof(*bss) + sizeof(struct hostapd_radius_servers));
	if (conf == NULL || bss == NULL) {
		printf("Failed to allocate memory for configuration data.\n");
		free(conf);
		free(bss);
		return NULL;
	}

	/* set default driver based on configuration */
	conf->driver = driver_lookup("default");
	if (conf->driver == NULL) {
		printf("No default driver registered!\n");
		free(conf);
		free(bss);
		return NULL;
	}

	hostapd_config_defaults_bss(bss);

	conf->num_bss = 1;
	conf->bss = bss;

	conf->beacon_int = 100;
	conf->send_probe_response = 1;

	return conf;
}


static int hostapd_parse_ip_addr(const char *txt, struct hostapd_ip_addr *addr)
{
	if (inet_aton(txt, &addr->u.v4)) {
		addr->af = AF_INET;
		return 0;
	}

#ifdef CONFIG_IPV6
	if (inet_pton(AF_INET6, txt, &addr->u.v6) > 0) {
		addr->af = AF_INET6;
		return 0;
	}
#endif /* CONFIG_IPV6 */

	return -1;
}


static int mac_comp(const void *a, const void *b)
{
	return memcmp(a, b, sizeof(macaddr));
}


static int hostapd_config_read_maclist(const char *fname, macaddr **acl,
				       int *num)
{
	FILE *f;
	char buf[128], *pos;
	int line = 0;
	u8 addr[ETH_ALEN];
	macaddr *newacl;

	if (!fname)
		return 0;

	f = fopen(fname, "r");
	if (!f) {
		printf("MAC list file '%s' not found.\n", fname);
		return -1;
	}

	while (fgets(buf, sizeof(buf), f)) {
		line++;

		if (buf[0] == '#')
			continue;
		pos = buf;
		while (*pos != '\0') {
			if (*pos == '\n') {
				*pos = '\0';
				break;
			}
			pos++;
		}
		if (buf[0] == '\0')
			continue;

		if (hwaddr_aton(buf, addr)) {
			printf("Invalid MAC address '%s' at line %d in '%s'\n",
			       buf, line, fname);
			fclose(f);
			return -1;
		}

		newacl = (macaddr *) realloc(*acl, (*num + 1) * ETH_ALEN);
		if (newacl == NULL) {
			printf("MAC list reallocation failed\n");
			fclose(f);
			return -1;
		}

		*acl = newacl;
		memcpy((*acl)[*num], addr, ETH_ALEN);
		(*num)++;
	}

	fclose(f);

	qsort(*acl, *num, sizeof(macaddr), mac_comp);

	return 0;
}


static int hostapd_config_read_wpa_psk(const char *fname,
				       struct hostapd_ssid *ssid)
{
	FILE *f;
	char buf[128], *pos;
	int line = 0, ret = 0, len, ok;
	u8 addr[ETH_ALEN];
	struct hostapd_wpa_psk *psk;

	if (!fname)
		return 0;

	f = fopen(fname, "r");
	if (!f) {
		printf("WPA PSK file '%s' not found.\n", fname);
		return -1;
	}

	while (fgets(buf, sizeof(buf), f)) {
		line++;

		if (buf[0] == '#')
			continue;
		pos = buf;
		while (*pos != '\0') {
			if (*pos == '\n') {
				*pos = '\0';
				break;
			}
			pos++;
		}
		if (buf[0] == '\0')
			continue;

		if (hwaddr_aton(buf, addr)) {
			printf("Invalid MAC address '%s' on line %d in '%s'\n",
			       buf, line, fname);
			ret = -1;
			break;
		}

		psk = wpa_zalloc(sizeof(*psk));
		if (psk == NULL) {
			printf("WPA PSK allocation failed\n");
			ret = -1;
			break;
		}
		if (memcmp(addr, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0)
			psk->group = 1;
		else
			memcpy(psk->addr, addr, ETH_ALEN);

		pos = buf + 17;
		if (pos == '\0') {
			printf("No PSK on line %d in '%s'\n", line, fname);
			free(psk);
			ret = -1;
			break;
		}
		pos++;

		ok = 0;
		len = strlen(pos);
		if (len == 64 && hexstr2bin(pos, psk->psk, PMK_LEN) == 0)
			ok = 1;
		else if (len >= 8 && len < 64) {
			pbkdf2_sha1(pos, ssid->ssid, ssid->ssid_len,
				    4096, psk->psk, PMK_LEN);
			ok = 1;
		}
		if (!ok) {
			printf("Invalid PSK '%s' on line %d in '%s'\n",
			       pos, line, fname);
			free(psk);
			ret = -1;
			break;
		}

		psk->next = ssid->wpa_psk;
		ssid->wpa_psk = psk;
	}

	fclose(f);

	return ret;
}


int hostapd_setup_wpa_psk(struct hostapd_bss_config *conf)
{
	struct hostapd_ssid *ssid = &conf->ssid;

	if (ssid->wpa_passphrase != NULL) {
		if (ssid->wpa_psk != NULL) {
			printf("Warning: both WPA PSK and passphrase set. "
			       "Using passphrase.\n");
			free(ssid->wpa_psk);
		}
		ssid->wpa_psk = wpa_zalloc(sizeof(struct hostapd_wpa_psk));
		if (ssid->wpa_psk == NULL) {
			printf("Unable to alloc space for PSK\n");
			return -1;
		}
		wpa_hexdump_ascii(MSG_DEBUG, "SSID",
				  (u8 *) ssid->ssid, ssid->ssid_len);
		wpa_hexdump_ascii(MSG_DEBUG, "PSK (ASCII passphrase)",
				  (u8 *) ssid->wpa_passphrase,
				  strlen(ssid->wpa_passphrase));
		pbkdf2_sha1(ssid->wpa_passphrase,
			    ssid->ssid, ssid->ssid_len,
			    4096, ssid->wpa_psk->psk, PMK_LEN);
		wpa_hexdump(MSG_DEBUG, "PSK (from passphrase)",
			    ssid->wpa_psk->psk, PMK_LEN);
		ssid->wpa_psk->group = 1;

		memset(ssid->wpa_passphrase, 0,
		       strlen(ssid->wpa_passphrase));
		free(ssid->wpa_passphrase);
		ssid->wpa_passphrase = 0;
	}

	if (ssid->wpa_psk_file) {
		if (hostapd_config_read_wpa_psk(ssid->wpa_psk_file,
						&conf->ssid))
			return -1;
		free(ssid->wpa_psk_file);
		ssid->wpa_psk_file = NULL;
	}

	return 0;
}


#ifdef EAP_SERVER
static int hostapd_config_read_eap_user(const char *fname,
					struct hostapd_bss_config *conf)
{
	FILE *f;
	char buf[512], *pos, *start, *pos2;
	int line = 0, ret = 0, num_methods;
	struct hostapd_eap_user *user, *tail = NULL;

	if (!fname)
		return 0;

	f = fopen(fname, "r");
	if (!f) {
		printf("EAP user file '%s' not found.\n", fname);
		return -1;
	}

	/* Lines: "user" METHOD,METHOD2 "password" (password optional) */
	while (fgets(buf, sizeof(buf), f)) {
		line++;

		if (buf[0] == '#')
			continue;
		pos = buf;
		while (*pos != '\0') {
			if (*pos == '\n') {
				*pos = '\0';
				break;
			}
			pos++;
		}
		if (buf[0] == '\0')
			continue;

		user = NULL;

		if (buf[0] != '"' && buf[0] != '*') {
			printf("Invalid EAP identity (no \" in start) on "
			       "line %d in '%s'\n", line, fname);
			goto failed;
		}

		user = wpa_zalloc(sizeof(*user));
		if (user == NULL) {
			printf("EAP user allocation failed\n");
			goto failed;
		}
		user->force_version = -1;

		if (buf[0] == '*') {
			pos = buf;
		} else {
			pos = buf + 1;
			start = pos;
			while (*pos != '"' && *pos != '\0')
				pos++;
			if (*pos == '\0') {
				printf("Invalid EAP identity (no \" in end) on"
				       " line %d in '%s'\n", line, fname);
				goto failed;
			}

			user->identity = malloc(pos - start);
			if (user->identity == NULL) {
				printf("Failed to allocate memory for EAP "
				       "identity\n");
				goto failed;
			}
			memcpy(user->identity, start, pos - start);
			user->identity_len = pos - start;

			if (pos[0] == '"' && pos[1] == '*') {
				user->wildcard_prefix = 1;
				pos++;
			}
		}
		pos++;
		while (*pos == ' ' || *pos == '\t')
			pos++;

		if (*pos == '\0') {
			printf("No EAP method on line %d in '%s'\n",
			       line, fname);
			goto failed;
		}

		start = pos;
		while (*pos != ' ' && *pos != '\t' && *pos != '\0')
			pos++;
		if (*pos == '\0') {
			pos = NULL;
		} else {
			*pos = '\0';
			pos++;
		}
		num_methods = 0;
		while (*start) {
			char *pos2 = strchr(start, ',');
			if (pos2) {
				*pos2++ = '\0';
			}
			user->methods[num_methods].method =
				eap_get_type(start, &user->methods[num_methods]
					     .vendor);
			if (user->methods[num_methods].vendor ==
			    EAP_VENDOR_IETF &&
			    user->methods[num_methods].method == EAP_TYPE_NONE)
			{
				printf("Unsupported EAP type '%s' on line %d "
				       "in '%s'\n", start, line, fname);
				goto failed;
			}

			num_methods++;
			if (num_methods >= EAP_USER_MAX_METHODS)
				break;
			if (pos2 == NULL)
				break;
			start = pos2;
		}
		if (num_methods == 0) {
			printf("No EAP types configured on line %d in '%s'\n",
			       line, fname);
			goto failed;
		}

		if (pos == NULL)
			goto done;

		while (*pos == ' ' || *pos == '\t')
			pos++;
		if (*pos == '\0')
			goto done;

		if (strncmp(pos, "[ver=0]", 7) == 0) {
			user->force_version = 0;
			goto done;
		}

		if (strncmp(pos, "[ver=1]", 7) == 0) {
			user->force_version = 1;
			goto done;
		}

		if (strncmp(pos, "[2]", 3) == 0) {
			user->phase2 = 1;
			goto done;
		}

		if (*pos == '"') {
			pos++;
			start = pos;
			while (*pos != '"' && *pos != '\0')
				pos++;
			if (*pos == '\0') {
				printf("Invalid EAP password (no \" in end) "
				       "on line %d in '%s'\n", line, fname);
				goto failed;
			}

			user->password = malloc(pos - start);
			if (user->password == NULL) {
				printf("Failed to allocate memory for EAP "
				       "password\n");
				goto failed;
			}
			memcpy(user->password, start, pos - start);
			user->password_len = pos - start;

			pos++;
		} else if (strncmp(pos, "hash:", 5) == 0) {
			pos += 5;
			pos2 = pos;
			while (*pos2 != '\0' && *pos2 != ' ' &&
			       *pos2 != '\t' && *pos2 != '#')
				pos2++;
			if (pos2 - pos != 32) {
				printf("Invalid password hash on line %d in "
				       "'%s'\n", line, fname);
				goto failed;
			}
			user->password = malloc(16);
			if (user->password == NULL) {
				printf("Failed to allocate memory for EAP "
				       "password hash\n");
				goto failed;
			}
			if (hexstr2bin(pos, user->password, 16) < 0) {
				printf("Invalid hash password on line %d in "
				       "'%s'\n", line, fname);
				goto failed;
			}
			user->password_len = 16;
			user->password_hash = 1;
			pos = pos2;
		} else {
			pos2 = pos;
			while (*pos2 != '\0' && *pos2 != ' ' &&
			       *pos2 != '\t' && *pos2 != '#')
				pos2++;
			if ((pos2 - pos) & 1) {
				printf("Invalid hex password on line %d in "
				       "'%s'\n", line, fname);
				goto failed;
			}
			user->password = malloc((pos2 - pos) / 2);
			if (user->password == NULL) {
				printf("Failed to allocate memory for EAP "
				       "password\n");
				goto failed;
			}
			if (hexstr2bin(pos, user->password,
				       (pos2 - pos) / 2) < 0) {
				printf("Invalid hex password on line %d in "
				       "'%s'\n", line, fname);
				goto failed;
			}
			user->password_len = (pos2 - pos) / 2;
			pos = pos2;
		}

		while (*pos == ' ' || *pos == '\t')
			pos++;
		if (strncmp(pos, "[2]", 3) == 0) {
			user->phase2 = 1;
		}

	done:
		if (tail == NULL) {
			tail = conf->eap_user = user;
		} else {
			tail->next = user;
			tail = user;
		}
		continue;

	failed:
		if (user) {
			free(user->password);
			free(user->identity);
			free(user);
		}
		ret = -1;
		break;
	}

	fclose(f);

	return ret;
}
#endif /* EAP_SERVER */


static int
hostapd_config_read_radius_addr(struct hostapd_radius_server **server,
				int *num_server, const char *val, int def_port,
				struct hostapd_radius_server **curr_serv)
{
	struct hostapd_radius_server *nserv;
	int ret;
	static int server_index = 1;

	nserv = realloc(*server, (*num_server + 1) * sizeof(*nserv));
	if (nserv == NULL)
		return -1;

	*server = nserv;
	nserv = &nserv[*num_server];
	(*num_server)++;
	(*curr_serv) = nserv;

	memset(nserv, 0, sizeof(*nserv));
	nserv->port = def_port;
	ret = hostapd_parse_ip_addr(val, &nserv->addr);
	nserv->index = server_index++;

	return ret;
}


static int hostapd_config_parse_key_mgmt(int line, const char *value)
{
	int val = 0, last;
	char *start, *end, *buf;

	buf = strdup(value);
	if (buf == NULL)
		return -1;
	start = buf;

	while (start != '\0') {
		while (*start == ' ' || *start == '\t')
			start++;
		if (*start == '\0')
			break;
		end = start;
		while (*end != ' ' && *end != '\t' && *end != '\0')
			end++;
		last = *end == '\0';
		*end = '\0';
		if (strcmp(start, "WPA-PSK") == 0)
			val |= WPA_KEY_MGMT_PSK;
		else if (strcmp(start, "WPA-EAP") == 0)
			val |= WPA_KEY_MGMT_IEEE8021X;
		else {
			printf("Line %d: invalid key_mgmt '%s'\n",
			       line, start);
			free(buf);
			return -1;
		}

		if (last)
			break;
		start = end + 1;
	}

	free(buf);
	if (val == 0) {
		printf("Line %d: no key_mgmt values configured.\n", line);
		return -1;
	}

	return val;
}


static int hostapd_config_parse_cipher(int line, const char *value)
{
	int val = 0, last;
	char *start, *end, *buf;

	buf = strdup(value);
	if (buf == NULL)
		return -1;
	start = buf;

	while (start != '\0') {
		while (*start == ' ' || *start == '\t')
			start++;
		if (*start == '\0')
			break;
		end = start;
		while (*end != ' ' && *end != '\t' && *end != '\0')
			end++;
		last = *end == '\0';
		*end = '\0';
		if (strcmp(start, "CCMP") == 0)
			val |= WPA_CIPHER_CCMP;
		else if (strcmp(start, "TKIP") == 0)
			val |= WPA_CIPHER_TKIP;
		else if (strcmp(start, "WEP104") == 0)
			val |= WPA_CIPHER_WEP104;
		else if (strcmp(start, "WEP40") == 0)
			val |= WPA_CIPHER_WEP40;
		else if (strcmp(start, "NONE") == 0)
			val |= WPA_CIPHER_NONE;
		else {
			printf("Line %d: invalid cipher '%s'.", line, start);
			free(buf);
			return -1;
		}

		if (last)
			break;
		start = end + 1;
	}
	free(buf);

	if (val == 0) {
		printf("Line %d: no cipher values configured.", line);
		return -1;
	}
	return val;
}


static int hostapd_config_check(struct hostapd_bss_config *conf)
{
	if (conf->ieee802_1x && !conf->eap_server &&
	    !conf->radius->auth_servers) {
		printf("Invalid IEEE 802.1X configuration (no EAP "
		       "authenticator configured).\n");
		return -1;
	}

	if (conf->wpa && (conf->wpa_key_mgmt & WPA_KEY_MGMT_PSK) &&
	    conf->ssid.wpa_psk == NULL && conf->ssid.wpa_passphrase == NULL &&
	    conf->ssid.wpa_psk_file == NULL) {
		printf("WPA-PSK enabled, but PSK or passphrase is not "
		       "configured.\n");
		return -1;
	}

	return 0;
}


static int hostapd_config_read_wep(struct hostapd_wep_keys *wep, int keyidx,
				   char *val)
{
	size_t len = strlen(val);

	if (keyidx < 0 || keyidx > 3 || wep->key[keyidx] != NULL)
		return -1;

	if (val[0] == '"') {
		if (len < 2 || val[len - 1] != '"')
			return -1;
		len -= 2;
		wep->key[keyidx] = malloc(len);
		if (wep->key[keyidx] == NULL)
			return -1;
		memcpy(wep->key[keyidx], val + 1, len);
		wep->len[keyidx] = len;
	} else {
		if (len & 1)
			return -1;
		len /= 2;
		wep->key[keyidx] = malloc(len);
		if (wep->key[keyidx] == NULL)
			return -1;
		wep->len[keyidx] = len;
		if (hexstr2bin(val, wep->key[keyidx], len) < 0)
			return -1;
	}

	wep->keys_set++;

	return 0;
}


static int hostapd_parse_rates(int **rate_list, char *val)
{
	int *list;
	int count;
	char *pos, *end;

	free(*rate_list);
	*rate_list = NULL;

	pos = val;
	count = 0;
	while (*pos != '\0') {
		if (*pos == ' ')
			count++;
		pos++;
	}

	list = malloc(sizeof(int) * (count + 2));
	if (list == NULL)
		return -1;
	pos = val;
	count = 0;
	while (*pos != '\0') {
		end = strchr(pos, ' ');
		if (end)
			*end = '\0';

		list[count++] = atoi(pos);
		if (!end)
			break;
		pos = end + 1;
	}
	list[count] = -1;

	*rate_list = list;
	return 0;
}


struct hostapd_config * hostapd_config_read(const char *fname)
{
	struct hostapd_config *conf;
	struct hostapd_bss_config *bss;
	FILE *f;
	char buf[256], *pos;
	int line = 0;
	int errors = 0;
	size_t i;

	f = fopen(fname, "r");
	if (f == NULL) {
		printf("Could not open configuration file '%s' for reading.\n",
		       fname);
		return NULL;
	}

	conf = hostapd_config_defaults();
	if (conf == NULL) {
		fclose(f);
		return NULL;
	}
	bss = conf->last_bss = conf->bss;

	while (fgets(buf, sizeof(buf), f)) {
		bss = conf->last_bss;
		line++;

		if (buf[0] == '#')
			continue;
		pos = buf;
		while (*pos != '\0') {
			if (*pos == '\n') {
				*pos = '\0';
				break;
			}
			pos++;
		}
		if (buf[0] == '\0')
			continue;

		pos = strchr(buf, '=');
		if (pos == NULL) {
			printf("Line %d: invalid line '%s'\n", line, buf);
			errors++;
			continue;
		}
		*pos = '\0';
		pos++;

		if (strcmp(buf, "interface") == 0) {
			snprintf(conf->bss[0].iface,
				 sizeof(conf->bss[0].iface), "%s", pos);
		} else if (strcmp(buf, "bridge") == 0) {
			snprintf(bss->bridge, sizeof(bss->bridge), "%s", pos);
		} else if (strcmp(buf, "driver") == 0) {
			conf->driver = driver_lookup(pos);
			if (conf->driver == NULL) {
				printf("Line %d: invalid/unknown driver "
				       "'%s'\n", line, pos);
				errors++;
			}
		} else if (strcmp(buf, "debug") == 0) {
			bss->debug = atoi(pos);
		} else if (strcmp(buf, "logger_syslog_level") == 0) {
			bss->logger_syslog_level = atoi(pos);
		} else if (strcmp(buf, "logger_stdout_level") == 0) {
			bss->logger_stdout_level = atoi(pos);
		} else if (strcmp(buf, "logger_syslog") == 0) {
			bss->logger_syslog = atoi(pos);
		} else if (strcmp(buf, "logger_stdout") == 0) {
			bss->logger_stdout = atoi(pos);
		} else if (strcmp(buf, "dump_file") == 0) {
			bss->dump_log_name = strdup(pos);
		} else if (strcmp(buf, "ssid") == 0) {
			bss->ssid.ssid_len = strlen(pos);
			if (bss->ssid.ssid_len >= HOSTAPD_MAX_SSID_LEN ||
			    bss->ssid.ssid_len < 1) {
				printf("Line %d: invalid SSID '%s'\n", line,
				       pos);
				errors++;
			}
			memcpy(bss->ssid.ssid, pos, bss->ssid.ssid_len);
			bss->ssid.ssid[bss->ssid.ssid_len] = '\0';
			bss->ssid.ssid_set = 1;
		} else if (strcmp(buf, "macaddr_acl") == 0) {
			bss->macaddr_acl = atoi(pos);
			if (bss->macaddr_acl != ACCEPT_UNLESS_DENIED &&
			    bss->macaddr_acl != DENY_UNLESS_ACCEPTED &&
			    bss->macaddr_acl != USE_EXTERNAL_RADIUS_AUTH) {
				printf("Line %d: unknown macaddr_acl %d\n",
				       line, bss->macaddr_acl);
			}
		} else if (strcmp(buf, "accept_mac_file") == 0) {
			if (hostapd_config_read_maclist(pos, &bss->accept_mac,
							&bss->num_accept_mac))
			{
				printf("Line %d: Failed to read "
				       "accept_mac_file '%s'\n",
				       line, pos);
				errors++;
			}
		} else if (strcmp(buf, "deny_mac_file") == 0) {
			if (hostapd_config_read_maclist(pos, &bss->deny_mac,
							&bss->num_deny_mac))
			{
				printf("Line %d: Failed to read "
				       "deny_mac_file '%s'\n",
				       line, pos);
				errors++;
			}
		} else if (strcmp(buf, "ap_max_inactivity") == 0) {
			bss->ap_max_inactivity = atoi(pos);
		} else if (strcmp(buf, "assoc_ap_addr") == 0) {
			if (hwaddr_aton(pos, bss->assoc_ap_addr)) {
				printf("Line %d: invalid MAC address '%s'\n",
				       line, pos);
				errors++;
			}
			bss->assoc_ap = 1;
		} else if (strcmp(buf, "ieee8021x") == 0) {
			bss->ieee802_1x = atoi(pos);
		} else if (strcmp(buf, "eapol_version") == 0) {
			bss->eapol_version = atoi(pos);
			if (bss->eapol_version < 1 ||
			    bss->eapol_version > 2) {
				printf("Line %d: invalid EAPOL "
				       "version (%d): '%s'.\n",
				       line, bss->eapol_version, pos);
				errors++;
			} else
				wpa_printf(MSG_DEBUG, "eapol_version=%d",
					   bss->eapol_version);
#ifdef EAP_SERVER
		} else if (strcmp(buf, "eap_authenticator") == 0) {
			bss->eap_server = atoi(pos);
			printf("Line %d: obsolete eap_authenticator used; "
			       "this has been renamed to eap_server\n", line);
		} else if (strcmp(buf, "eap_server") == 0) {
			bss->eap_server = atoi(pos);
		} else if (strcmp(buf, "eap_user_file") == 0) {
			if (hostapd_config_read_eap_user(pos, bss))
				errors++;
		} else if (strcmp(buf, "ca_cert") == 0) {
			free(bss->ca_cert);
			bss->ca_cert = strdup(pos);
		} else if (strcmp(buf, "server_cert") == 0) {
			free(bss->server_cert);
			bss->server_cert = strdup(pos);
		} else if (strcmp(buf, "private_key") == 0) {
			free(bss->private_key);
			bss->private_key = strdup(pos);
		} else if (strcmp(buf, "private_key_passwd") == 0) {
			free(bss->private_key_passwd);
			bss->private_key_passwd = strdup(pos);
		} else if (strcmp(buf, "check_crl") == 0) {
			bss->check_crl = atoi(pos);
#ifdef EAP_SIM
		} else if (strcmp(buf, "eap_sim_db") == 0) {
			free(bss->eap_sim_db);
			bss->eap_sim_db = strdup(pos);
#endif /* EAP_SIM */
#endif /* EAP_SERVER */
		} else if (strcmp(buf, "eap_message") == 0) {
			char *term;
			bss->eap_req_id_text = strdup(pos);
			if (bss->eap_req_id_text == NULL) {
				printf("Line %d: Failed to allocate memory "
				       "for eap_req_id_text\n", line);
				errors++;
				continue;
			}
			bss->eap_req_id_text_len =
				strlen(bss->eap_req_id_text);
			term = strstr(bss->eap_req_id_text, "\\0");
			if (term) {
				*term++ = '\0';
				memmove(term, term + 1,
					bss->eap_req_id_text_len -
					(term - bss->eap_req_id_text) - 1);
				bss->eap_req_id_text_len--;
			}
		} else if (strcmp(buf, "wep_key_len_broadcast") == 0) {
			bss->default_wep_key_len = atoi(pos);
			if (bss->default_wep_key_len > 13) {
				printf("Line %d: invalid WEP key len %lu "
				       "(= %lu bits)\n", line,
				       (unsigned long)
				       bss->default_wep_key_len,
				       (unsigned long)
				       bss->default_wep_key_len * 8);
				errors++;
			}
		} else if (strcmp(buf, "wep_key_len_unicast") == 0) {
			bss->individual_wep_key_len = atoi(pos);
			if (bss->individual_wep_key_len < 0 ||
			    bss->individual_wep_key_len > 13) {
				printf("Line %d: invalid WEP key len %d "
				       "(= %d bits)\n", line,
				       bss->individual_wep_key_len,
				       bss->individual_wep_key_len * 8);
				errors++;
			}
		} else if (strcmp(buf, "wep_rekey_period") == 0) {
			bss->wep_rekeying_period = atoi(pos);
			if (bss->wep_rekeying_period < 0) {
				printf("Line %d: invalid period %d\n",
				       line, bss->wep_rekeying_period);
				errors++;
			}
		} else if (strcmp(buf, "eap_reauth_period") == 0) {
			bss->eap_reauth_period = atoi(pos);
			if (bss->eap_reauth_period < 0) {
				printf("Line %d: invalid period %d\n",
				       line, bss->eap_reauth_period);
				errors++;
			}
		} else if (strcmp(buf, "eapol_key_index_workaround") == 0) {
			bss->eapol_key_index_workaround = atoi(pos);
#ifdef CONFIG_IAPP
		} else if (strcmp(buf, "iapp_interface") == 0) {
			bss->ieee802_11f = 1;
			snprintf(bss->iapp_iface, sizeof(bss->iapp_iface),
				 "%s", pos);
#endif /* CONFIG_IAPP */
		} else if (strcmp(buf, "own_ip_addr") == 0) {
			if (hostapd_parse_ip_addr(pos, &bss->own_ip_addr)) {
				printf("Line %d: invalid IP address '%s'\n",
				       line, pos);
				errors++;
			}
		} else if (strcmp(buf, "nas_identifier") == 0) {
			bss->nas_identifier = strdup(pos);
		} else if (strcmp(buf, "auth_server_addr") == 0) {
			if (hostapd_config_read_radius_addr(
				    &bss->radius->auth_servers,
				    &bss->radius->num_auth_servers, pos, 1812,
				    &bss->radius->auth_server)) {
				printf("Line %d: invalid IP address '%s'\n",
				       line, pos);
				errors++;
			}
		} else if (bss->radius->auth_server &&
			   strcmp(buf, "auth_server_port") == 0) {
			bss->radius->auth_server->port = atoi(pos);
		} else if (bss->radius->auth_server &&
			   strcmp(buf, "auth_server_shared_secret") == 0) {
			int len = strlen(pos);
			if (len == 0) {
				/* RFC 2865, Ch. 3 */
				printf("Line %d: empty shared secret is not "
				       "allowed.\n", line);
				errors++;
			}
			bss->radius->auth_server->shared_secret =
				(u8 *) strdup(pos);
			bss->radius->auth_server->shared_secret_len = len;
		} else if (strcmp(buf, "acct_server_addr") == 0) {
			if (hostapd_config_read_radius_addr(
				    &bss->radius->acct_servers,
				    &bss->radius->num_acct_servers, pos, 1813,
				    &bss->radius->acct_server)) {
				printf("Line %d: invalid IP address '%s'\n",
				       line, pos);
				errors++;
			}
		} else if (bss->radius->acct_server &&
			   strcmp(buf, "acct_server_port") == 0) {
			bss->radius->acct_server->port = atoi(pos);
		} else if (bss->radius->acct_server &&
			   strcmp(buf, "acct_server_shared_secret") == 0) {
			int len = strlen(pos);
			if (len == 0) {
				/* RFC 2865, Ch. 3 */
				printf("Line %d: empty shared secret is not "
				       "allowed.\n", line);
				errors++;
			}
			bss->radius->acct_server->shared_secret =
				(u8 *) strdup(pos);
			bss->radius->acct_server->shared_secret_len = len;
		} else if (strcmp(buf, "radius_retry_primary_interval") == 0) {
			bss->radius->retry_primary_interval = atoi(pos);
		} else if (strcmp(buf, "radius_acct_interim_interval") == 0) {
			bss->radius->acct_interim_interval = atoi(pos);
		} else if (strcmp(buf, "auth_algs") == 0) {
			bss->auth_algs = atoi(pos);
			if (bss->auth_algs == 0) {
				printf("Line %d: no authentication algorithms "
				       "allowed\n",
				       line);
				errors++;
			}
		} else if (strcmp(buf, "wpa") == 0) {
			bss->wpa = atoi(pos);
		} else if (strcmp(buf, "wpa_group_rekey") == 0) {
			bss->wpa_group_rekey = atoi(pos);
		} else if (strcmp(buf, "wpa_strict_rekey") == 0) {
			bss->wpa_strict_rekey = atoi(pos);
		} else if (strcmp(buf, "wpa_gmk_rekey") == 0) {
			bss->wpa_gmk_rekey = atoi(pos);
		} else if (strcmp(buf, "wpa_passphrase") == 0) {
			int len = strlen(pos);
			if (len < 8 || len > 63) {
				printf("Line %d: invalid WPA passphrase length"
				       " %d (expected 8..63)\n", line, len);
				errors++;
			} else {
				free(bss->ssid.wpa_passphrase);
				bss->ssid.wpa_passphrase = strdup(pos);
			}
		} else if (strcmp(buf, "wpa_psk") == 0) {
			free(bss->ssid.wpa_psk);
			bss->ssid.wpa_psk =
				wpa_zalloc(sizeof(struct hostapd_wpa_psk));
			if (bss->ssid.wpa_psk == NULL)
				errors++;
			else if (hexstr2bin(pos, bss->ssid.wpa_psk->psk,
					    PMK_LEN) ||
				 pos[PMK_LEN * 2] != '\0') {
				printf("Line %d: Invalid PSK '%s'.\n", line,
				       pos);
				errors++;
			} else {
				bss->ssid.wpa_psk->group = 1;
			}
		} else if (strcmp(buf, "wpa_psk_file") == 0) {
			free(bss->ssid.wpa_psk_file);
			bss->ssid.wpa_psk_file = strdup(pos);
			if (!bss->ssid.wpa_psk_file) {
				printf("Line %d: allocation failed\n", line);
				errors++;
			}
		} else if (strcmp(buf, "wpa_key_mgmt") == 0) {
			bss->wpa_key_mgmt =
				hostapd_config_parse_key_mgmt(line, pos);
			if (bss->wpa_key_mgmt == -1)
				errors++;
		} else if (strcmp(buf, "wpa_pairwise") == 0) {
			bss->wpa_pairwise =
				hostapd_config_parse_cipher(line, pos);
			if (bss->wpa_pairwise == -1 ||
			    bss->wpa_pairwise == 0)
				errors++;
			else if (bss->wpa_pairwise &
				 (WPA_CIPHER_NONE | WPA_CIPHER_WEP40 |
				  WPA_CIPHER_WEP104)) {
				printf("Line %d: unsupported pairwise "
				       "cipher suite '%s'\n",
				       bss->wpa_pairwise, pos);
				errors++;
			} else {
				if (bss->wpa_pairwise & WPA_CIPHER_TKIP)
					bss->wpa_group = WPA_CIPHER_TKIP;
				else
					bss->wpa_group = WPA_CIPHER_CCMP;
			}
#ifdef CONFIG_RSN_PREAUTH
		} else if (strcmp(buf, "rsn_preauth") == 0) {
			bss->rsn_preauth = atoi(pos);
		} else if (strcmp(buf, "rsn_preauth_interfaces") == 0) {
			bss->rsn_preauth_interfaces = strdup(pos);
#endif /* CONFIG_RSN_PREAUTH */
#ifdef CONFIG_STAKEY
		} else if (strcmp(buf, "stakey") == 0) {
			bss->stakey = atoi(pos);
#endif /* CONFIG_STAKEY */
#ifdef CONFIG_PEERKEY
		} else if (strcmp(buf, "peerkey") == 0) {
			bss->peerkey = atoi(pos);
#endif /* CONFIG_PEERKEY */
		} else if (strcmp(buf, "ctrl_interface") == 0) {
			free(bss->ctrl_interface);
			bss->ctrl_interface = strdup(pos);
		} else if (strcmp(buf, "ctrl_interface_group") == 0) {
#ifndef CONFIG_NATIVE_WINDOWS
			struct group *grp;
			char *endp;
			const char *group = pos;

			grp = getgrnam(group);
			if (grp) {
				bss->ctrl_interface_gid = grp->gr_gid;
				bss->ctrl_interface_gid_set = 1;
				wpa_printf(MSG_DEBUG, "ctrl_interface_group=%d"
					   " (from group name '%s')",
					   bss->ctrl_interface_gid, group);
				continue;
			}

			/* Group name not found - try to parse this as gid */
			bss->ctrl_interface_gid = strtol(group, &endp, 10);
			if (*group == '\0' || *endp != '\0') {
				wpa_printf(MSG_DEBUG, "Line %d: Invalid group "
					   "'%s'", line, group);
				errors++;
				continue;
			}
			bss->ctrl_interface_gid_set = 1;
			wpa_printf(MSG_DEBUG, "ctrl_interface_group=%d",
				   bss->ctrl_interface_gid);
#endif /* CONFIG_NATIVE_WINDOWS */
#ifdef RADIUS_SERVER
		} else if (strcmp(buf, "radius_server_clients") == 0) {
			free(bss->radius_server_clients);
			bss->radius_server_clients = strdup(pos);
		} else if (strcmp(buf, "radius_server_auth_port") == 0) {
			bss->radius_server_auth_port = atoi(pos);
		} else if (strcmp(buf, "radius_server_ipv6") == 0) {
			bss->radius_server_ipv6 = atoi(pos);
#endif /* RADIUS_SERVER */
		} else if (strcmp(buf, "test_socket") == 0) {
			free(bss->test_socket);
			bss->test_socket = strdup(pos);
		} else if (strcmp(buf, "use_pae_group_addr") == 0) {
			bss->use_pae_group_addr = atoi(pos);
		} else if (strcmp(buf, "hw_mode") == 0) {
			if (strcmp(pos, "a") == 0)
				conf->hw_mode = HOSTAPD_MODE_IEEE80211A;
			else if (strcmp(pos, "b") == 0)
				conf->hw_mode = HOSTAPD_MODE_IEEE80211B;
			else if (strcmp(pos, "g") == 0)
				conf->hw_mode = HOSTAPD_MODE_IEEE80211G;
			else {
				printf("Line %d: unknown hw_mode '%s'\n",
				       line, pos);
				errors++;
			}
		} else if (strcmp(buf, "channel") == 0) {
			conf->channel = atoi(pos);
		} else if (strcmp(buf, "beacon_int") == 0) {
			int val = atoi(pos);
			/* MIB defines range as 1..65535, but very small values
			 * cause problems with the current implementation.
			 * Since it is unlikely that this small numbers are
			 * useful in real life scenarios, do not allow beacon
			 * period to be set below 15 TU. */
			if (val < 15 || val > 65535) {
				printf("Line %d: invalid beacon_int %d "
				       "(expected 15..65535)\n",
				       line, val);
				errors++;
			} else
				conf->beacon_int = val;
		} else if (strcmp(buf, "send_probe_response") == 0) {
			int val = atoi(pos);
			if (val != 0 && val != 1) {
				printf("Line %d: invalid send_probe_response "
				       "%d (expected 0 or 1)\n", line, val);
			} else
				conf->send_probe_response = val;
		} else if (strcmp(buf, "supported_rates") == 0) {
			if (hostapd_parse_rates(&conf->supported_rates, pos)) {
				printf("Line %d: invalid rate list\n", line);
				errors++;
			}
		} else if (strcmp(buf, "basic_rates") == 0) {
			if (hostapd_parse_rates(&conf->basic_rates, pos)) {
				printf("Line %d: invalid rate list\n", line);
				errors++;
			}
		} else if (strcmp(buf, "ignore_broadcast_ssid") == 0) {
			bss->ignore_broadcast_ssid = atoi(pos);
		} else if (strcmp(buf, "wep_default_key") == 0) {
			bss->ssid.wep.idx = atoi(pos);
			if (bss->ssid.wep.idx > 3) {
				printf("Invalid wep_default_key index %d\n",
				       bss->ssid.wep.idx);
				errors++;
			}
		} else if (strcmp(buf, "wep_key0") == 0 ||
			   strcmp(buf, "wep_key1") == 0 ||
			   strcmp(buf, "wep_key2") == 0 ||
			   strcmp(buf, "wep_key3") == 0) {
			if (hostapd_config_read_wep(&bss->ssid.wep,
						    buf[7] - '0', pos)) {
				printf("Line %d: invalid WEP key '%s'\n",
				       line, buf);
				errors++;
			}
		} else if (strcmp(buf, "dynamic_vlan") == 0) {
			bss->ssid.dynamic_vlan = atoi(pos);
		} else {
			printf("Line %d: unknown configuration item '%s'\n",
			       line, buf);
			errors++;
		}
	}

	fclose(f);

	for (i = 0; i < conf->num_bss; i++) {
		bss = &conf->bss[i];

		bss->radius->auth_server = bss->radius->auth_servers;
		bss->radius->acct_server = bss->radius->acct_servers;

		if (bss->wpa && bss->ieee802_1x) {
			bss->ssid.security_policy = SECURITY_WPA;
		} else if (bss->wpa) {
			bss->ssid.security_policy = SECURITY_WPA_PSK;
		} else if (bss->ieee802_1x) {
			bss->ssid.security_policy = SECURITY_IEEE_802_1X;
			bss->ssid.wep.default_len = bss->default_wep_key_len;
		} else if (bss->ssid.wep.keys_set)
			bss->ssid.security_policy = SECURITY_STATIC_WEP;
		else
			bss->ssid.security_policy = SECURITY_PLAINTEXT;

		if (hostapd_config_check(bss))
			errors++;
	}

	if (errors) {
		printf("%d errors found in configuration file '%s'\n",
		       errors, fname);
		hostapd_config_free(conf);
		conf = NULL;
	}

	return conf;
}


static void hostapd_config_free_radius(struct hostapd_radius_server *servers,
				       int num_servers)
{
	int i;

	for (i = 0; i < num_servers; i++) {
		free(servers[i].shared_secret);
	}
	free(servers);
}


static void hostapd_config_free_eap_user(struct hostapd_eap_user *user)
{
	free(user->identity);
	free(user->password);
	free(user);
}


static void hostapd_config_free_bss(struct hostapd_bss_config *conf)
{
	struct hostapd_wpa_psk *psk, *prev;
	struct hostapd_eap_user *user, *prev_user;

	if (conf == NULL)
		return;

	psk = conf->ssid.wpa_psk;
	while (psk) {
		prev = psk;
		psk = psk->next;
		free(prev);
	}

	free(conf->ssid.wpa_passphrase);
	free(conf->ssid.wpa_psk_file);

	user = conf->eap_user;
	while (user) {
		prev_user = user;
		user = user->next;
		hostapd_config_free_eap_user(prev_user);
	}

	free(conf->dump_log_name);
	free(conf->eap_req_id_text);
	free(conf->accept_mac);
	free(conf->deny_mac);
	free(conf->nas_identifier);
	hostapd_config_free_radius(conf->radius->auth_servers,
				   conf->radius->num_auth_servers);
	hostapd_config_free_radius(conf->radius->acct_servers,
				   conf->radius->num_acct_servers);
	free(conf->rsn_preauth_interfaces);
	free(conf->ctrl_interface);
	free(conf->ca_cert);
	free(conf->server_cert);
	free(conf->private_key);
	free(conf->private_key_passwd);
	free(conf->eap_sim_db);
	free(conf->radius_server_clients);
	free(conf->test_socket);
}


void hostapd_config_free(struct hostapd_config *conf)
{
	size_t i;

	if (conf == NULL)
		return;

	for (i = 0; i < conf->num_bss; i++)
		hostapd_config_free_bss(&conf->bss[i]);
	free(conf->bss);

	free(conf);
}


/* Perform a binary search for given MAC address from a pre-sorted list.
 * Returns 1 if address is in the list or 0 if not. */
int hostapd_maclist_found(macaddr *list, int num_entries, const u8 *addr)
{
	int start, end, middle, res;

	start = 0;
	end = num_entries - 1;

	while (start <= end) {
		middle = (start + end) / 2;
		res = memcmp(list[middle], addr, ETH_ALEN);
		if (res == 0)
			return 1;
		if (res < 0)
			start = middle + 1;
		else
			end = middle - 1;
	}

	return 0;
}


int hostapd_rate_found(int *list, int rate)
{
	int i;

	if (list == NULL)
		return 0;

	for (i = 0; list[i] >= 0; i++)
		if (list[i] == rate)
			return 1;

	return 0;
}


const u8 * hostapd_get_psk(const struct hostapd_bss_config *conf,
			   const u8 *addr, const u8 *prev_psk)
{
	struct hostapd_wpa_psk *psk;
	int next_ok = prev_psk == NULL;

	for (psk = conf->ssid.wpa_psk; psk != NULL; psk = psk->next) {
		if (next_ok &&
		    (psk->group || memcmp(psk->addr, addr, ETH_ALEN) == 0))
			return psk->psk;

		if (psk->psk == prev_psk)
			next_ok = 1;
	}

	return NULL;
}


const struct hostapd_eap_user *
hostapd_get_eap_user(const struct hostapd_bss_config *conf, const u8 *identity,
		     size_t identity_len, int phase2)
{
	struct hostapd_eap_user *user = conf->eap_user;

	while (user) {
		if (!phase2 && user->identity == NULL) {
			/* Wildcard match */
			break;
		}

		if (!phase2 && user->wildcard_prefix &&
		    identity_len >= user->identity_len &&
		    memcmp(user->identity, identity, user->identity_len) == 0)
		{
			/* Wildcard prefix match */
			break;
		}

		if (user->phase2 == !!phase2 &&
		    user->identity_len == identity_len &&
		    memcmp(user->identity, identity, identity_len) == 0)
			break;
		user = user->next;
	}

	return user;
}
