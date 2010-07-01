/*
 * madwifi.c
 *
 * Copyright (C) 2005 - 2007 Sebastian Gottschall <gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id:
 */

#ifdef HAVE_MADWIFI
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <getopt.h>
#include <err.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <utils.h>
#include <unistd.h>

#include "wireless.h"
#include "net80211/ieee80211.h"
#include "net80211/ieee80211_crypto.h"
#include "net80211/ieee80211_ioctl.h"
#include <iwlib.h>
#include <services.h>

static int setsysctrl(const char *dev, const char *control, u_long value)
{
	sysprintf("echo %li > /proc/sys/dev/%s/%s", value, dev, control);

	return 0;
}

static void setdistance(char *device, int distance, int chanbw)
{
	if (distance >= 0)
		setsysctrl(device, "distance", distance);
}

// returns the number of installed atheros devices/cards

static void deconfigure_single(int count)
{
	char *next;
	char dev[16];
	char var[80];
	char wifivifs[16];
	sprintf(wifivifs, "ath%d_vifs", count);
	sprintf(dev, "ath%d", count);
	char vifs[128];
#ifdef HAVE_MADWIFI_MIMO
	if (is_ar5008(dev)) {
		deconfigure_single_11n(count);
		return;
	}
#endif

	sprintf(vifs, "%s.1 %s.2 %s.3 %s.4 %s.5 %s.6 %s.7 %s.8 %s.9", dev, dev,
		dev, dev, dev, dev, dev, dev, dev);
	int s;

	for (s = 1; s <= 10; s++) {
		sprintf(dev, "ath%d.wds%d", count, s - 1);
		if (ifexists(dev)) {
			br_del_interface("br0", dev);
			sysprintf("ifconfig %s down", dev);
		}
	}
	sprintf(dev, "ath%d", count);
	if (ifexists(dev)) {
		br_del_interface("br0", dev);
		sysprintf("ifconfig %s down", dev);
	}
	foreach(var, vifs, next) {
		if (ifexists(var)) {
			sysprintf("ifconfig %s down", dev);
		}
	}
	sprintf(dev, "ath%d", count);

	if (ifexists(dev))
		sysprintf("wlanconfig %s destroy", dev);

	foreach(var, vifs, next) {
		if (ifexists(var)) {
			sysprintf("wlanconfig %s destroy", var);
		}
	}

}

void deconfigure_wifi(void)
{

	stop_process("wrt-radauth", "Radius daemon");
	stop_process("hostapd", "hostapd daemon");
	stop_process("wpa_supplicant", "wpa_supplicant daemon");

	int c = getdevicecount();
	int i;

	for (i = 0; i < c; i++)
		deconfigure_single(i);
}

static int need_commit = 0;

static int getMaxPower(char *ifname)
{
	char buf[128];

	sprintf(buf, "iwlist %s txpower|grep \"Maximum Power:\" > /tmp/.power",
		ifname);
	system2(buf);
	FILE *in = fopen("/tmp/.power", "rb");

	if (in == NULL)
		return 1000;
	char buf2[16];
	int max;

	fscanf(in, "%s %s %d", buf, buf2, &max);
	fclose(in);
	return max;
}

void setupKey(char *prefix)
{
	char akm[16];
	char mode[16];

	sprintf(akm, "%s_akm", prefix);
	sprintf(mode, "%s_mode", prefix);
	if (nvram_match(akm, "wep")
	    && (nvram_match(mode, "ap") || nvram_match(mode, "wdsap")
		|| nvram_match(mode, "adhoc"))) {
		char key[16];
		int cnt = 1;
		int i;
		char bul[8];
		char *authmode = nvram_nget("%s_authmode", prefix);
		if (!strcmp(authmode, "shared"))
			sysprintf("iwpriv %s authmode 2", prefix);
		else if (!strcmp(authmode, "auto"))
			sysprintf("iwpriv %s authmode 4", prefix);
		else
			sysprintf("iwpriv %s authmode 1", prefix);
		for (i = 1; i < 5; i++) {
			char *athkey = nvram_nget("%s_key%d", prefix, i);

			if (athkey != NULL && strlen(athkey) > 0) {
				sysprintf("iwconfig %s key [%d] %s", prefix, cnt++, athkey);	// setup wep
			}
		}
		sysprintf("iwconfig %s key [%s]", prefix,
			  nvram_nget("%s_key", prefix));
	}

}

/*
 * MADWIFI Encryption Setup 
 */
void setupSupplicant(char *prefix, char *ssidoverride)
{
#ifdef HAVE_REGISTER
	if (!isregistered())
		return;
#endif
	char akm[16];
	char bridged[32];
	char wmode[16];
	char *background = "-B";
	char *debug;
	int i;
	debug = nvram_nget("%s_wpa_debug", prefix);
	if (debug != NULL) {
		if (!strcmp(debug, "1"))
			background = "-Bd";
		else if (!strcmp(debug, "2"))
			background = "-Bdd";
		else if (!strcmp(debug, "3"))
			background = "-Bddd";
	}

	sprintf(akm, "%s_akm", prefix);
	sprintf(wmode, "%s_mode", prefix);
	sprintf(bridged, "%s_bridged", prefix);
	if (nvram_match(akm, "psk") ||
	    nvram_match(akm, "psk2") || nvram_match(akm, "psk psk2")) {
		char fstr[32];
		char psk[16];

		sprintf(fstr, "/tmp/%s_wpa_supplicant.conf", prefix);
		FILE *fp = fopen(fstr, "wb");

		fprintf(fp, "ap_scan=1\n");
		fprintf(fp, "fast_reauth=1\n");
		fprintf(fp, "eapol_version=1\n");
		// fprintf (fp, "ctrl_interface_group=0\n");
		// fprintf (fp, "ctrl_interface=/var/run/wpa_supplicant\n");

		fprintf(fp, "network={\n");
		if (!ssidoverride)
			ssidoverride = nvram_nget("%s_ssid", prefix);
		fprintf(fp, "\tssid=\"%s\"\n", ssidoverride);
		// fprintf (fp, "\tmode=0\n");
		fprintf(fp, "\tscan_ssid=1\n");
		fprintf(fp, "\tkey_mgmt=WPA-PSK\n");

		sprintf(psk, "%s_crypto", prefix);
		if (nvram_match(psk, "aes")) {
			fprintf(fp, "\tpairwise=CCMP\n");
			fprintf(fp, "\tgroup=CCMP\n");
		}
		if (nvram_match(psk, "tkip")) {
			fprintf(fp, "\tpairwise=TKIP\n");
			fprintf(fp, "\tgroup=TKIP\n");
		}
		if (nvram_match(psk, "tkip+aes")) {
			fprintf(fp, "\tpairwise=CCMP TKIP\n");
			fprintf(fp, "\tgroup=CCMP TKIP\n");
		}
		if (nvram_match(akm, "psk"))
			fprintf(fp, "\tproto=WPA\n");
		if (nvram_match(akm, "psk2"))
			fprintf(fp, "\tproto=RSN\n");
		if (nvram_match(akm, "psk psk2"))
			fprintf(fp, "\tproto=WPA RSN\n");

		fprintf(fp, "\tpsk=\"%s\"\n", nvram_nget("%s_wpa_psk", prefix));
		fprintf(fp, "}\n");
		char extra[32];
		sprintf(extra, "%s_supplicantext", prefix);
		if (nvram_invmatch(extra, ""))
			fwritenvram(extra, fp);

		fclose(fp);
		sprintf(psk, "-i%s", prefix);
		if ((nvram_match(wmode, "wdssta") || nvram_match(wmode, "wet"))
		    && nvram_match(bridged, "1"))
			eval("wpa_supplicant", "-b", getBridge(prefix),
			     background, "-Dmadwifi", psk, "-c", fstr);
		else
			eval("wpa_supplicant", background, "-Dmadwifi", psk,
			     "-c", fstr);
	} else if (nvram_match(akm, "8021X")) {
		char fstr[32];
		char psk[64];
		char ath[64];

		sprintf(fstr, "/tmp/%s_wpa_supplicant.conf", prefix);
		FILE *fp = fopen(fstr, "wb");

		fprintf(fp, "ap_scan=1\n");
		fprintf(fp, "fast_reauth=1\n");
		fprintf(fp, "eapol_version=1\n");
		// fprintf (fp, "ctrl_interface_group=0\n");
		// fprintf (fp, "ctrl_interface=/var/run/wpa_supplicant\n");
		fprintf(fp, "network={\n");
		if (!ssidoverride)
			ssidoverride = nvram_nget("%s_ssid", prefix);
		fprintf(fp, "\tssid=\"%s\"\n", ssidoverride);
		fprintf(fp, "\tscan_ssid=1\n");
		if (nvram_prefix_match("8021xtype", prefix, "tls")) {
			fprintf(fp, "\tkey_mgmt=IEEE8021X\n");
			fprintf(fp, "\teap=TLS\n");
			fprintf(fp, "\tidentity=\"%s\"\n",
				nvram_prefix_get("tls8021xuser", prefix));
			sprintf(psk, "/tmp/%s", prefix);
			mkdir(psk);
			sprintf(psk, "/tmp/%s/ca.pem", prefix);
			sprintf(ath, "%s_tls8021xca", prefix);
			write_nvram(psk, ath);
			sprintf(psk, "/tmp/%s/user.pem", prefix);
			sprintf(ath, "%s_tls8021xpem", prefix);
			write_nvram(psk, ath);

			sprintf(psk, "/tmp/%s/user.prv", prefix);
			sprintf(ath, "%s_tls8021xprv", prefix);
			write_nvram(psk, ath);
			fprintf(fp, "\tca_cert=\"/tmp/%s/ca.pem\"\n", prefix);
			fprintf(fp, "\tclient_cert=\"/tmp/%s/user.pem\"\n",
				prefix);
			fprintf(fp, "\tprivate_key=\"/tmp/%s/user.prv\"\n",
				prefix);
			fprintf(fp, "\tprivate_key_passwd=\"%s\"\n",
				nvram_prefix_get("tls8021xpasswd", prefix));
			fprintf(fp, "\teapol_flags=3\n");
			if (strlen(nvram_nget("%s_tls8021xphase2", prefix)) > 0) {
				fprintf(fp, "\tphase2=\"%s\"\n",
					nvram_nget("%s_tls8021xphase2",
						   prefix));
			}
			if (strlen(nvram_nget("%s_tls8021xanon", prefix)) > 0) {
				fprintf(fp, "\tanonymous_identity=\"%s\"\n",
					nvram_nget("%s_tls8021xanon", prefix));
			}
			if (strlen(nvram_nget("%s_tls8021xaddopt", prefix)) > 0) {
				sprintf(ath, "%s_tls8021xaddopt", prefix);
				fprintf(fp, "\t");	// tab
				fwritenvram(ath, fp);
				fprintf(fp, "\n");	// extra new line at the end
			}
		}
		if (nvram_prefix_match("8021xtype", prefix, "peap")) {
			fprintf(fp, "\tkey_mgmt=WPA-EAP\n");
			fprintf(fp, "\teap=PEAP\n");
			fprintf(fp, "\tpairwise=CCMP TKIP\n");
			fprintf(fp, "\tgroup=CCMP TKIP\n");
			fprintf(fp, "\tphase1=\"peapver=0\"\n");
			fprintf(fp, "\tidentity=\"%s\"\n",
				nvram_prefix_get("peap8021xuser", prefix));
			fprintf(fp, "\tpassword=\"%s\"\n",
				nvram_prefix_get("peap8021xpasswd", prefix));
			sprintf(psk, "/tmp/%s", prefix);
			mkdir(psk);
			sprintf(psk, "/tmp/%s/ca.pem", prefix);
			sprintf(ath, "%s_peap8021xca", prefix);
			if (!nvram_match(ath, "")) {
				write_nvram(psk, ath);
				fprintf(fp, "\tca_cert=\"/tmp/%s/ca.pem\"\n",
					prefix);
			}
			if (strlen(nvram_nget("%s_peap8021xphase2", prefix)) >
			    0) {
				fprintf(fp, "\tphase2=\"%s\"\n",
					nvram_nget("%s_peap8021xphase2",
						   prefix));
			}
			if (strlen(nvram_nget("%s_peap8021xanon", prefix)) > 0) {
				fprintf(fp, "\tanonymous_identity=\"%s\"\n",
					nvram_nget("%s_peap8021xanon", prefix));
			}
			if (strlen(nvram_nget("%s_peap8021xaddopt", prefix)) >
			    0) {
				sprintf(ath, "%s_peap8021xaddopt", prefix);
				fprintf(fp, "\t");	// tab
				fwritenvram(ath, fp);
				fprintf(fp, "\n");	// extra new line at the end
			}
		}
		if (nvram_prefix_match("8021xtype", prefix, "ttls")) {
			fprintf(fp, "\tkey_mgmt=WPA-EAP\n");
			fprintf(fp, "\teap=TTLS\n");
			fprintf(fp, "\tpairwise=CCMP TKIP\n");
			fprintf(fp, "\tgroup=CCMP TKIP\n");
			fprintf(fp, "\tidentity=\"%s\"\n",
				nvram_prefix_get("ttls8021xuser", prefix));
			fprintf(fp, "\tpassword=\"%s\"\n",
				nvram_prefix_get("ttls8021xpasswd", prefix));
			if (strlen(nvram_nget("%s_ttls8021xca", prefix)) > 0) {
				sprintf(psk, "/tmp/%s", prefix);
				mkdir(psk);
				sprintf(psk, "/tmp/%s/ca.pem", prefix);
				sprintf(ath, "%s_ttls8021xca", prefix);
				write_nvram(psk, ath);
				fprintf(fp, "\tca_cert=\"/tmp/%s/ca.pem\"\n",
					prefix);
			}
			if (strlen(nvram_nget("%s_ttls8021xphase2", prefix)) >
			    0) {
				fprintf(fp, "\tphase2=\"%s\"\n",
					nvram_nget("%s_ttls8021xphase2",
						   prefix));
			}
			if (strlen(nvram_nget("%s_ttls8021xanon", prefix)) > 0) {
				fprintf(fp, "\tanonymous_identity=\"%s\"\n",
					nvram_nget("%s_ttls8021xanon", prefix));
			}
			if (strlen(nvram_nget("%s_ttls8021xaddopt", prefix)) >
			    0) {
				sprintf(ath, "%s_ttls8021xaddopt", prefix);
				fprintf(fp, "\t");	// tab
				fwritenvram(ath, fp);
				fprintf(fp, "\n");	// extra new line at the end
			}
		}
		if (nvram_prefix_match("8021xtype", prefix, "leap")) {
			fprintf(fp, "\tkey_mgmt=WPA-EAP\n");
			fprintf(fp, "\teap=LEAP\n");
			fprintf(fp, "\tauth_alg=LEAP\n");
			fprintf(fp, "\tproto=WPA RSN\n");
			fprintf(fp, "\tpairwise=CCMP TKIP\n");
			fprintf(fp, "\tgroup=CCMP TKIP\n");
			fprintf(fp, "\tidentity=\"%s\"\n",
				nvram_prefix_get("leap8021xuser", prefix));
			fprintf(fp, "\tpassword=\"%s\"\n",
				nvram_prefix_get("leap8021xpasswd", prefix));
			// sprintf (psk, "/tmp/%s", prefix);
			// mkdir (psk);
			// sprintf (psk, "/tmp/%s/ca.pem", prefix);
			// sprintf (ath, "%s_peap8021xca", prefix);
			// write_nvram (psk, ath);
			// fprintf (fp, "\tca_cert=\"/tmp/%s/ca.pem\"\n", prefix);
			if (strlen(nvram_nget("%s_leap8021xphase2", prefix)) >
			    0) {
				fprintf(fp, "\tphase2=\"%s\"\n",
					nvram_nget("%s_leap8021xphase2",
						   prefix));
			}
			if (strlen(nvram_nget("%s_leap8021xanon", prefix)) > 0) {
				fprintf(fp, "\tanonymous_identity=\"%s\"\n",
					nvram_nget("%s_leap8021xanon", prefix));
			}
			if (strlen(nvram_nget("%s_leap8021xaddopt", prefix)) >
			    0) {
				sprintf(ath, "%s_leap8021xaddopt", prefix);
				fprintf(fp, "\t");	// tab
				fwritenvram(ath, fp);
				fprintf(fp, "\n");	// extra new line at the end
			}
		}
		fprintf(fp, "}\n");
		char extra[32];
		sprintf(extra, "%s_supplicantext", prefix);
		if (nvram_invmatch(extra, ""))
			fwritenvram(extra, fp);
		fclose(fp);
		sprintf(psk, "-i%s", prefix);
		sysprintf("iwpriv %s hostroaming 2", prefix);
		if (nvram_match(bridged, "1")
		    && (nvram_match(wmode, "wdssta")
			|| nvram_match(wmode, "wet")))
			eval("wpa_supplicant", "-b",
			     nvram_safe_get("lan_ifname"), background,
			     "-Dmadwifi", psk, "-c", fstr);
		else
			eval("wpa_supplicant", background, "-Dmadwifi", psk,
			     "-c", fstr);
	} else if (nvram_match(akm, "disabled") || nvram_match(akm, "wep")) {
		char fstr[32];
		char psk[16];

		sprintf(fstr, "/tmp/%s_wpa_supplicant.conf", prefix);
		FILE *fp = fopen(fstr, "wb");

		fprintf(fp, "ap_scan=1\n");
		// fprintf (fp, "ctrl_interface_group=0\n");
		// fprintf (fp, "ctrl_interface=/var/run/wpa_supplicant\n");

		fprintf(fp, "network={\n");
		if (!ssidoverride)
			ssidoverride = nvram_nget("%s_ssid", prefix);
		fprintf(fp, "\tssid=\"%s\"\n", ssidoverride);
		// fprintf (fp, "\tmode=0\n");
		fprintf(fp, "\tscan_ssid=1\n");
		fprintf(fp, "\tkey_mgmt=NONE\n");
		if (nvram_match(akm, "wep")) {
			int cnt = 0;
			char *authmode = nvram_nget("%s_authmode", prefix);

			if (!strcmp(authmode, "shared"))
				fprintf(fp, "auth_alg=shared\n");
			else
				fprintf(fp, "auth_alg=open\n");

			for (i = 1; i < 5; i++) {
				char *athkey =
				    nvram_nget("%s_key%d", prefix, i);

				if (athkey != NULL && strlen(athkey) > 0) {
					fprintf(fp, "wep_key%d=%s\n", cnt++, athkey);	// setup wep
				}
			}

			fprintf(fp, "wep_tx_keyidx=%s\n",
				nvram_nget("%s_key", prefix));

		}
		fprintf(fp, "}\n");
		char extra[32];
		sprintf(extra, "%s_supplicantext", prefix);
		if (nvram_invmatch(extra, ""))
			fwritenvram(extra, fp);

		fclose(fp);
		sprintf(psk, "-i%s", prefix);
		if ((nvram_match(wmode, "wdssta") || nvram_match(wmode, "wet"))
		    && nvram_match(bridged, "1"))
			eval("wpa_supplicant", "-b", getBridge(prefix),
			     background, "-Dmadwifi", psk, "-c", fstr);
		else
			eval("wpa_supplicant", background, "-Dmadwifi", psk,
			     "-c", fstr);

	}

}

void supplicant_main(int argc, char *argv[])
{
	setupSupplicant(argv[1], argv[2]);
}

static void do_hostapd(char *fstr, char *prefix)
{
	char fname[32];
	char *debug;
	FILE *fp;
	int pid;

	sprintf(fname, "/var/run/%s_hostapd.pid", prefix);

	fp = fopen(fname, "rb");
	if (fp) {
		fread(&pid, 4, 1, fp);
		fclose(fp);
		if (pid > 0)
			kill(pid, SIGTERM);
	}

	char *argv[] = { "hostapd", "-B", NULL, NULL, NULL };
	int argc = 2;
	debug = nvram_nget("%s_wpa_debug", prefix);
	if (debug != NULL) {
		if (!strcmp(debug, "1"))
			argv[argc++] = "-d";
		else if (!strcmp(debug, "2"))
			argv[argc++] = "-dd";
		else if (!strcmp(debug, "3"))
			argv[argc++] = "-ddd";
	}
	argv[argc++] = fstr;
	_evalpid(argv, NULL, 0, &pid);
//      eval( "hostapd", "-B", fstr );
	fp = fopen(fname, "wb");
	if (fp) {
		fwrite(&pid, 4, 1, fp);
		fclose(fp);
	}

}

void setupHostAP(char *prefix, char *driver, int iswan)
{
#ifdef HAVE_REGISTER
	if (!isregistered())
		return;
#endif
	char psk[32];
	char akm[16];

	sprintf(akm, "%s_akm", prefix);
	if (nvram_match(akm, "8021X"))
		return;
	if (nvram_match(akm, "wpa") || nvram_match(akm, "wpa2")
	    || nvram_match(akm, "wpa wpa2") || nvram_match(akm, "radius")) {
		if (iswan == 0)
			return;
	}
	if (nvram_match(akm, "psk") ||
	    nvram_match(akm, "psk2") ||
	    nvram_match(akm, "psk psk2") || nvram_match(akm, "wep")) {
		if (iswan == 1)
			return;
	}
	// wep key support
	if (nvram_match(akm, "wep")) {
		/* ignore */
	} else if (nvram_match(akm, "psk") ||
		   nvram_match(akm, "psk2") ||
		   nvram_match(akm, "psk psk2") ||
		   nvram_match(akm, "wpa") ||
		   nvram_match(akm, "wpa2") || nvram_match(akm, "wpa wpa2")) {
		char fstr[32];

		sprintf(fstr, "/tmp/%s_hostap.conf", prefix);
		FILE *fp = fopen(fstr, "wb");

		fprintf(fp, "interface=%s\n", prefix);
		// sprintf(buf, "rsn_preauth_interfaces=%s\n", "br0");
		if (nvram_nmatch("1", "%s_bridged", prefix))
			fprintf(fp, "bridge=%s\n", getBridge(prefix));

		fprintf(fp, "driver=%s\n", driver);
		fprintf(fp, "logger_syslog=-1\n");
		fprintf(fp, "logger_syslog_level=2\n");
		fprintf(fp, "logger_stdout=-1\n");
		fprintf(fp, "logger_stdout_level=2\n");
		fprintf(fp, "debug=0\n");
		fprintf(fp, "dump_file=/tmp/hostapd.dump\n");
		// fprintf (fp, "eap_server=0\n");
		// fprintf (fp, "own_ip_addr=127.0.0.1\n");
		fprintf(fp, "eapol_version=1\n");
		fprintf(fp, "eapol_key_index_workaround=0\n");
		if (nvram_match(akm, "psk") || nvram_match(akm, "wpa"))
			fprintf(fp, "wpa=1\n");
		if (nvram_match(akm, "psk2") || nvram_match(akm, "wpa2"))
			fprintf(fp, "wpa=2\n");
		if (nvram_match(akm, "psk psk2")
		    || nvram_match(akm, "wpa wpa2"))
			fprintf(fp, "wpa=3\n");

		if (nvram_match(akm, "psk") ||
		    nvram_match(akm, "psk2") || nvram_match(akm, "psk psk2")) {
			fprintf(fp, "wpa_passphrase=%s\n",
				nvram_nget("%s_wpa_psk", prefix));
			fprintf(fp, "wpa_key_mgmt=WPA-PSK\n");
		} else {
			// if (nvram_invmatch (akm, "radius"))
			fprintf(fp, "wpa_key_mgmt=WPA-EAP\n");
			// else
			// fprintf (fp, "macaddr_acl=2\n");
			fprintf(fp, "ieee8021x=1\n");
			// fprintf (fp, "accept_mac_file=/tmp/hostapd.accept\n");
			// fprintf (fp, "deny_mac_file=/tmp/hostapd.deny\n");
			fprintf(fp, "own_ip_addr=%s\n",
				nvram_safe_get("lan_ipaddr"));
			fprintf(fp, "eap_server=0\n");
			fprintf(fp, "auth_algs=1\n");
			fprintf(fp, "radius_retry_primary_interval=60\n");

			fprintf(fp, "auth_server_addr=%s\n",
				nvram_nget("%s_radius_ipaddr", prefix));
			fprintf(fp, "auth_server_port=%s\n",
				nvram_nget("%s_radius_port", prefix));
			fprintf(fp, "auth_server_shared_secret=%s\n",
				nvram_nget("%s_radius_key", prefix));
			char check[64];
			sprintf(check, "%s_radius2_ipaddr", prefix);
			nvram_default_get(check, "0.0.0.0");

			if (!nvram_nmatch("", "%s_radius2_ipaddr", prefix)
			    && !nvram_nmatch("0.0.0.0", "%s_radius2_ipaddr",
					     prefix)
			    && !nvram_nmatch("", "%s_radius2_port", prefix)) {
				fprintf(fp, "auth_server_addr=%s\n",
					nvram_nget("%s_radius2_ipaddr",
						   prefix));
				fprintf(fp, "auth_server_port=%s\n",
					nvram_nget("%s_radius2_port", prefix));
				fprintf(fp, "auth_server_shared_secret=%s\n",
					nvram_nget("%s_radius2_key", prefix));
			}
			if (nvram_nmatch("1", "%s_acct", prefix)) {
				fprintf(fp, "acct_server_addr=%s\n",
					nvram_nget("%s_acct_ipaddr", prefix));
				fprintf(fp, "acct_server_port=%s\n",
					nvram_nget("%s_acct_port", prefix));
				fprintf(fp, "acct_server_shared_secret=%s\n",
					nvram_nget("%s_acct_key", prefix));
			}
		}
		if (nvram_invmatch(akm, "radius")) {
			sprintf(psk, "%s_crypto", prefix);
			if (nvram_match(psk, "aes"))
				fprintf(fp, "wpa_pairwise=CCMP\n");
			if (nvram_match(psk, "tkip"))
				fprintf(fp, "wpa_pairwise=TKIP\n");
			if (nvram_match(psk, "tkip+aes"))
				fprintf(fp, "wpa_pairwise=TKIP CCMP\n");
			fprintf(fp, "wpa_group_rekey=%s\n",
				nvram_nget("%s_wpa_gtk_rekey", prefix));
		}
		// fprintf (fp, "jumpstart_p1=1\n");
		fclose(fp);
		do_hostapd(fstr, prefix);

	} else if (nvram_match(akm, "radius")) {
		// wrt-radauth $IFNAME $server $port $share $override $mackey $maxun
		// &
		char *ifname = prefix;
		char *server = nvram_nget("%s_radius_ipaddr", prefix);
		char *port = nvram_nget("%s_radius_port", prefix);
		char *share = nvram_nget("%s_radius_key", prefix);
		char exec[64];
		char type[32];

		sprintf(type, "%s_radmactype", prefix);
		char *pragma = "";

		if (nvram_default_match(type, "0", "0"))
			pragma = "-n1 ";
		if (nvram_match(type, "1"))
			pragma = "-n2 ";
		if (nvram_match(type, "2"))
			pragma = "-n3 ";
		if (nvram_match(type, "3"))
			pragma = "";
		sleep(1);	// some delay is usefull
		sysprintf("wrt-radauth %s %s %s %s %s 1 1 0 &", pragma, prefix,
			  server, port, share);
	} else {
		sysprintf("iwconfig %s key off", prefix);
	}

}

void start_hostapdwan(void)
{
	char ath[32];
	char *next;
	char var[80];
	int c = getdevicecount();
	int i;

	for (i = 0; i < c; i++) {
		sprintf(ath, "ath%d", i);
		if (nvram_nmatch("ap", "%s_mode", ath)
		    || nvram_nmatch("wdsap", "%s_mode", ath)) {
#ifdef HAVE_MADWIFI_MIMO
			if (is_ar5008(ath))
				setupHostAP(ath, "wext", 1);
			else
#endif
				setupHostAP(ath, "madwifi", 1);
		}
		char *vifs = nvram_nget("ath%d_vifs", i);

		if (vifs != NULL)
			foreach(var, vifs, next) {
#ifdef HAVE_MADWIFI_MIMO
			if (is_ar5008(ath))
				setupHostAP(var, "wext", 1);
			else
#endif
				setupHostAP(var, "madwifi", 1);
			}
	}

}

#define SIOCSSCANLIST  		(SIOCDEVPRIVATE+6)
static void set_scanlist(char *dev, char *wif)
{
	char var[32];
	char *next;
	struct iwreq iwr;
	char scanlist[32];
	char list[64];

	sprintf(scanlist, "%s_scanlist", dev);
	char *sl = nvram_default_get(scanlist, "default");
	int c = 0;

	sysprintf("iwpriv %s setscanlist -ALL", dev);
	if (strlen(sl) > 0 && strcmp(sl, "default")) {
		foreach(var, sl, next) {
			sprintf(list, "+%s", var);
			sysprintf("iwpriv %s setscanlist %s", dev, list);
		}
	} else {
		sysprintf("iwpriv %s setscanlist +ALL", dev);
	}
}

static void set_rate(char *dev, char *priv)
{
	char rate[32];
	char maxrate[32];
	char net[32];
	char bw[32];
	char xr[32];

	sprintf(bw, "%s_channelbw", dev);
	sprintf(net, "%s_net_mode", dev);
	sprintf(rate, "%s_minrate", dev);
	sprintf(maxrate, "%s_maxrate", dev);
	sprintf(xr, "%s_xr", dev);
	char *r = nvram_default_get(rate, "0");
	char *mr = nvram_default_get(maxrate, "0");

#ifdef HAVE_WHRAG108
	char *netmode;

	if (!strcmp(dev, "ath0"))
		netmode = nvram_default_get(net, "a-only");
	else
		netmode = nvram_default_get(net, "mixed");
#else
	char *netmode = nvram_default_get(net, "mixed");
#endif

	if (nvram_match(bw, "20") && nvram_match(xr, "0"))
		if (atof(r) == 27.0f || atof(r) == 1.5f || atof(r) == 2.0f
		    || atof(r) == 3.0f || atof(r) == 4.5f || atof(r) == 9.0f
		    || atof(r) == 13.5f) {
			nvram_set(rate, "0");
			r = "0";
		}
	if (nvram_match(bw, "40"))
		if (atof(r) == 27.0f || atof(r) == 1.5f || atof(r) == 2.0f
		    || atof(r) == 3.0f || atof(r) == 4.5f || atof(r) == 9.0f
		    || atof(r) == 13.5f) {
			nvram_set(rate, "0");
			r = "0";
		}
	if (nvram_match(bw, "10"))
		if (atof(r) > 27.0f || atof(r) == 1.5f || atof(r) == 2.0f
		    || atof(r) == 13.5f) {
			nvram_set(rate, "0");
			r = "0";
		}
	if (nvram_match(bw, "5"))
		if (atof(r) > 13.5) {
			nvram_set(rate, "0");
			r = "0";
		}
	if (nvram_match(bw, "2"))
		if (atof(r) > 6.75) {
			nvram_set(rate, "0");
			r = "0";
		}
	if (!strcmp(netmode, "b-only"))
		sysprintf("iwconfig %s rate 11M auto", priv);
	else {
		sysprintf("iwconfig %s rate 54M auto", priv);
	}
	if (atol(mr) > 0)
		sysprintf("iwpriv %s maxrate %s", priv, mr);
	if (atoi(r) > 0)
		sysprintf("iwpriv %s minrate %s", priv, r);
}

static void set_netmode(char *wif, char *dev, char *use)
{
	char net[16];
	char mode[16];
	char xr[16];
	char comp[32];
	char ff[16];
	char bw[16];

	sprintf(mode, "%s_mode", dev);
	sprintf(net, "%s_net_mode", dev);
	sprintf(bw, "%s_channelbw", dev);
	sprintf(xr, "%s_xr", dev);
//    sprintf( comp, "%s_compression", dev );
	sprintf(ff, "%s_ff", dev);
#ifdef HAVE_WHRAG108
	char *netmode;

	if (!strcmp(dev, "ath0"))
		netmode = nvram_default_get(net, "a-only");
	else
		netmode = nvram_default_get(net, "mixed");
#else
	char *netmode = nvram_default_get(net, "mixed");
#endif
	// fprintf (stderr, "set netmode of %s to %s\n", net, netmode);
	cprintf("configure net mode %s\n", netmode);

	{
#ifdef HAVE_WHRAG108
		if (!strncmp(use, "ath0", 4)) {
			sysprintf("iwpriv %s mode 1", use);
		} else
#endif
#ifdef HAVE_TW6600
		if (!strncmp(use, "ath0", 4)) {
			sysprintf("iwpriv %s mode 1", use);
		} else
#endif
		{
			sysprintf("iwpriv %s xr 0", use);
			if (!strcmp(netmode, "mixed"))
				sysprintf("iwpriv %s mode 0", use);
			if (!strcmp(netmode, "b-only"))
				sysprintf("iwpriv %s mode 2", use);
			if (!strcmp(netmode, "g-only")) {
				sysprintf("iwpriv %s mode 3", use);
				sysprintf("iwpriv %s pureg 1", use);
			}
			if (!strcmp(netmode, "ng-only")) {
				sysprintf("iwpriv %s mode 7", use);
			}
			if (!strcmp(netmode, "na-only")) {
				sysprintf("iwpriv %s mode 6", use);
			}
			if (!strcmp(netmode, "bg-mixed")) {
				sysprintf("iwpriv %s mode 3", use);
			}

			if (!strcmp(netmode, "a-only"))
				sysprintf("iwpriv %s mode 1", use);
		}
	}
	if (nvram_default_match(bw, "40", "20")) {
		{
			if (!strcmp(netmode, "g-only")) {
				sysprintf("iwpriv %s mode 6", use);
			}
			if (!strcmp(netmode, "a-only")) {
				sysprintf("iwpriv %s mode 5", use);
			}
		}
	} else {
		char *ext = nvram_get(xr);

		if (ext) {
			if (strcmp(ext, "1") == 0) {
				sysprintf("iwpriv %s xr 1", use);
			} else {
				sysprintf("iwpriv %s xr 0", use);
			}
		}
	}
//    if( nvram_default_match( comp, "1", "0" ) )
//      sysprintf("iwpriv %s compression 1",use);
//    else
//      sysprintf("iwpriv %s compression 0",use);

	if (nvram_default_match(ff, "1", "0"))
		sysprintf("iwpriv %s ff 1", use);
	else
		sysprintf("iwpriv %s ff 0", use);

}

static void setRTS(char *use)
{
	char rts[32];

	sprintf(rts, "%s_protmode", use);
	nvram_default_get(rts, "None");

	sprintf(rts, "%s_rts", use);
	nvram_default_get(rts, "0");

	sprintf(rts, "%s_rtsvalue", use);
	nvram_default_get(rts, "2346");

	if (nvram_nmatch("1", "%s_rts", use)) {
		sysprintf("iwconfig %s rts %s", use,
			  nvram_nget("%s_rtsvalue", use));
	} else {
		sysprintf("iwconfig %s rts off", use);
	}
	if (nvram_nmatch("None", "%s_protmode", use))
		sysprintf("iwpriv %s protmode 0", use);
	if (nvram_nmatch("CTS", "%s_protmode", use))
		sysprintf("iwpriv %s protmode 1", use);
	if (nvram_nmatch("RTS/CTS", "%s_protmode", use))
		sysprintf("iwpriv %s protmode 2", use);
}

/*static void set_compression( int count )
{
    char comp[32];
    char wif[32];

    sprintf( wif, "wifi%d", count );
    sprintf( comp, "ath%d_compression", count );
    if( nvram_default_match( comp, "1", "0" ) )
	setsysctrl( wif, "compression", 1 );
    else
	setsysctrl( wif, "compression", 0 );
}
*/
void setMacFilter(char *iface)
{
	char *next;
	char var[32];

	sysprintf("iwpriv %s maccmd 3", iface);

	char nvvar[32];

	sprintf(nvvar, "%s_macmode", iface);
	if (nvram_match(nvvar, "deny")) {
		sysprintf("iwpriv %s maccmd 2", iface);
		char nvlist[32];

		sprintf(nvlist, "%s_maclist", iface);

		foreach(var, nvram_safe_get(nvlist), next) {
			sysprintf("iwpriv %s addmac %s", iface, var);
		}
	} else if (nvram_match(nvvar, "allow")) {
		sysprintf("iwpriv %s maccmd 1", iface);

		char nvlist[32];

		sprintf(nvlist, "%s_maclist", iface);

		foreach(var, nvram_safe_get(nvlist), next) {
			sysprintf("iwpriv %s addmac %s", iface, var);
		}
	}

}

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

static void configure_single(int count)
{

	char *next;
	static char var[80];
	static char mode[80];
	int cnt = 0;
	static char dev[10];
	static char wif[10];
	static char mtikie[32];
	static char wl[16];
	static char channel[16];
	static char ssid[16];
	static char net[16];
	static char wifivifs[16];
	static char broadcast[16];
	static char power[32];
	static char sens[32];
	static char basedev[16];
	static char diversity[32];
	static char rxantenna[32];
	static char txantenna[32];
	static char athmac[16];
	static char maxassoc[32];
	static char wl_poll[32];
	static int vapcount = 0;
	static char inact[16];
	static char inact_tick[16];
	if (count == 0)
		vapcount = 0;

	sprintf(wif, "wifi%d", count);
	sprintf(dev, "ath%d", count);
#ifdef HAVE_MADWIFI_MIMO
	if (is_ar5008(dev)) {
		configure_single_11n(count);
		return;
	}
#endif
	sprintf(wifivifs, "ath%d_vifs", count);
	sprintf(wl, "ath%d_mode", count);
#ifdef HAVE_REGISTER
	int cpeonly = iscpe();
#else
	int cpeonly = 0;
#endif
	if (cpeonly && nvram_match(wl, "ap")) {
		nvram_set(wl, "sta");
	}
	sprintf(channel, "ath%d_channel", count);
	sprintf(power, "ath%d_txpwrdbm", count);
	sprintf(sens, "ath%d_distance", count);
	sprintf(diversity, "ath%d_diversity", count);
	sprintf(txantenna, "ath%d_txantenna", count);
	sprintf(rxantenna, "ath%d_rxantenna", count);
	sprintf(athmac, "ath%d_hwaddr", count);

	// create base device
	cprintf("configure base interface %d\n", count);
	sprintf(net, "%s_net_mode", dev);
	if (nvram_match(net, "disabled"))
		return;
//    set_compression( count );
	// create wds interface(s)
	int s;

	char *apm;
	int vif = 0;
	sprintf(wl_poll, "%s_pollingmode", dev);

	setsysctrl(wif, "pollingmode", atoi(nvram_default_get(wl_poll, "0")));

	char *vifs = nvram_safe_get(wifivifs);
	int countvaps = 1;
	foreach(var, vifs, next) {
		countvaps++;
	}
	if (countvaps < 4)
		countvaps = 4;
	if (countvaps > vapcount)
		vapcount = countvaps;

	setsysctrl(wif, "maxvaps", vapcount);
	char primary[32] = { 0 };
	// create original primary interface
	apm = nvram_default_get(wl, "ap");
	if (!strcmp(apm, "ap") || !strcmp(apm, "wdsap")) {

		if (!strcmp(apm, "wet") || !strcmp(apm, "wdssta")
		    || !strcmp(apm, "sta")) {
			if (vif)
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode sta nosbeacon",
				     dev, wif);
			else
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode sta",
				     dev, wif);

		} else if (!strcmp(apm, "ap") || !strcmp(apm, "wdsap"))
			sysprintf("wlanconfig %s create wlandev %s wlanmode ap",
				  dev, wif);
		else
			sysprintf
			    ("wlanconfig %s create wlandev %s wlanmode adhoc nosbeacon",
			     dev, wif);

		if (strlen(primary) == 0)
			strcpy(primary, dev);

	}

	if (vifs != NULL)
		foreach(var, vifs, next) {
		sprintf(mode, "%s_mode", var);
		char *vapm = nvram_default_get(mode, "ap");
		// create device
		if (strlen(mode) > 0) {
			if (!strcmp(vapm, "wet") || !strcmp(vapm, "sta")
			    || !strcmp(vapm, "wdssta"))
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode sta nosbeacon",
				     var, wif);
			else if (!strcmp(vapm, "ap") || !strcmp(vapm, "wdsap"))
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode ap",
				     var, wif);
			else
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode adhoc nosbeacon",
				     var, wif);
			vif = 1;
			if (strlen(primary) == 0)
				strcpy(primary, var);
			char vathmac[16];

			sprintf(vathmac, "%s_hwaddr", var);
			char vmacaddr[32];

			getMacAddr(var, vmacaddr);
			nvram_set(vathmac, vmacaddr);

		}
		}

	if (strcmp(apm, "ap") && strcmp(apm, "wdsap")) {
		if (!strcmp(apm, "wet") || !strcmp(apm, "wdssta")
		    || !strcmp(apm, "sta")) {
			if (vif)
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode sta nosbeacon",
				     dev, wif);
			else
				sysprintf
				    ("wlanconfig %s create wlandev %s wlanmode sta",
				     dev, wif);

		} else if (!strcmp(apm, "ap") || !strcmp(apm, "wdsap"))
			sysprintf("wlanconfig %s create wlandev %s wlanmode ap",
				  dev, wif);
		else
			sysprintf
			    ("wlanconfig %s create wlandev %s wlanmode adhoc nosbeacon",
			     dev, wif);

		if (strlen(primary) == 0)
			strcpy(primary, dev);
	}
#if 0
#endif
	cprintf("detect maxpower\n");
	apm = nvram_default_get(wl, "ap");
	char maxp[16];

	vifs = nvram_safe_get(wifivifs);
	// fprintf(stderr,"vifs %s\n",vifs);
	char *useif = NULL;
	char copyvap[64];

	if (vifs != NULL)
		foreach(var, vifs, next) {
		// fprintf(stderr,"vifs %s, %s\n",vifs, var);
		if (!useif) {
			strcpy(copyvap, var);
			useif = copyvap;
		}
		}
	// config net mode
	if (useif)
		set_netmode(wif, dev, useif);
	set_netmode(wif, dev, dev);

	char wmm[32];

	sprintf(wmm, "%s_wmm", dev);
	if (nvram_nmatch("1", "%s_pollingmode", dev))
		sysprintf("iwpriv %s wmm 1", dev);
	else
		sysprintf("iwpriv %s wmm %s", dev, nvram_default_get(wmm, "0"));
	char doth[32];

	sprintf(doth, "%s_doth", dev);
	sysprintf("iwpriv %s doth %s", dev, nvram_default_get(doth, "0"));
	int disablescan = 0;

	set_scanlist(dev, wif);
	if (useif)
		set_netmode(wif, dev, useif);
	set_netmode(wif, dev, dev);
	setRTS(dev);

	char macaddr[32];

	getMacAddr(dev, macaddr);
	nvram_set(athmac, macaddr);

	cprintf("adjust sensitivity\n");

	int distance = atoi(nvram_default_get(sens, "2000"));	// to meter
	if (nvram_nmatch("1", "%s_pollingmode", var)) {
		setdistance(wif, 100000, 20);
	} else {

		if (distance > 0) {
			setsysctrl(wif, "dynack_count", 0);
			char *chanbw = nvram_nget("%s_channelbw", dev);

			setdistance(wif, distance, atoi(chanbw));	// sets the receiver
			// sensitivity
		} else {
			setsysctrl(wif, "distance", 100000);
			setsysctrl(wif, "dynack_count", 20);
		}
	}
	char wl_intmit[32];
	char wl_noise_immunity[32];
	char wl_ofdm_weak_det[32];
	char wl_csma[32];

	sprintf(wl_intmit, "%s_intmit", dev);
	sprintf(wl_noise_immunity, "%s_noise_immunity", dev);
	sprintf(wl_ofdm_weak_det, "%s_ofdm_weak_det", dev);
	sprintf(wl_csma, "%s_csma", dev);

	setsysctrl(wif, "csma", atoi(nvram_default_get(wl_csma, "1")));
	setsysctrl(wif, "intmit", atoi(nvram_default_get(wl_intmit, "-1")));
	int level = atoi(nvram_default_get(wl_noise_immunity, "4"));
	if (level < 0)
		level = 4;
	setsysctrl(wif, "noise_immunity", level);
	setsysctrl(wif, "ofdm_weak_det",
		   atoi(nvram_default_get(wl_ofdm_weak_det, "1")));

	if (isEMP(dev))		//check this only if the current installed card is usually a emp card. this is made to prevent card destruction
	{
		if (nvram_nmatch("1", "%s_cardtype", dev)) {
			setsysctrl(wif, "powerfix", 7);	//increase outputpower by 7 dbm, we will do this in future for a and b band separate
		}

	}

	char *enable = "enable";
	char *disable = "disable";

#ifdef HAVE_NS5
	char *gpio = "1";
#endif
#ifdef HAVE_NS3
	char *gpio = "1";
#endif
#ifdef HAVE_LC5
	char *gpio = "1";
#endif
#ifdef HAVE_NS2
	char *gpio = "7";
#endif
#ifdef HAVE_LC2
	enable = "disable";	// swap it
	disable = "enable";
	char *gpio = "2";
#endif

#if defined(HAVE_NS2)  || defined(HAVE_NS5) || defined(HAVE_LC2) || defined(HAVE_LC5) || defined(HAVE_NS3)
	int tx = atoi(nvram_default_get(txantenna, "0"));

	setsysctrl(wif, "diversity", 0);
	switch (tx) {
	case 0:		// vertical
		setsysctrl(wif, "rxantenna", 2);
		setsysctrl(wif, "txantenna", 2);
		eval("gpio", enable, gpio);
		break;
	case 1:		// horizontal
		setsysctrl(wif, "rxantenna", 1);
		setsysctrl(wif, "txantenna", 1);
		eval("gpio", enable, gpio);
		break;
	case 2:		// external
		setsysctrl(wif, "rxantenna", 1);
		setsysctrl(wif, "txantenna", 1);
		eval("gpio", disable, gpio);
		break;
	case 3:		// adaptive
		setsysctrl(wif, "diversity", 1);
		setsysctrl(wif, "rxantenna", 0);
		setsysctrl(wif, "txantenna", 0);
		eval("gpio", enable, gpio);
		break;
	}
#else

#if defined(HAVE_PICO2) || defined(HAVE_PICO2HP) || defined(HAVE_PICO5)
	int rx = 1;
	int tx = 1;
	int diva = 0;		//atoi( nvram_default_get( diversity, "0" ) );
//#elif defined(HAVE_EOC5610)
//      int rx = atoi(nvram_default_get(txantenna, "1"));
//      int tx = atoi(nvram_default_get(txantenna, "1"));
//      int diva = 0;           //atoi( nvram_default_get( diversity, "0" ) );
//      int rx = 1;
//      int tx = 0;             // fix to internal path, since both antennas use the same connector. so only the switch matters
//      int diva = 1;           //1;// atoi( nvram_default_get( diversity, "0" ) );
//#elif defined(HAVE_EOC1650)
//      int rx = 2;             //atoi( nvram_default_get( txantenna, "2" ) ); // secondary antenna output is the internal antenna and should be used as default value
//      int tx = 2;             //atoi( nvram_default_get( txantenna, "2" ) );
//      int rx = 1;
//      int tx = 0;             // fix to internal path, since both antennas use the same connector. so only the switch matters
//      int diva = 1;           //1;// atoi( nvram_default_get( diversity, "0" ) );
#else
	int rx = atoi(nvram_default_get(rxantenna, "1"));
	int tx = atoi(nvram_default_get(txantenna, "1"));
	int diva = atoi(nvram_default_get(diversity, "0"));
#endif

	setsysctrl(wif, "diversity", diva);
	setsysctrl(wif, "rxantenna", rx);
	setsysctrl(wif, "txantenna", tx);
#endif
	// setup vif interfaces first
	char chanshift_s[32];

	sprintf(chanshift_s, "%s_chanshift", dev);
	char *chanshift = nvram_default_get(chanshift_s, "0");

	sprintf(maxassoc, "%s_maxassoc", dev);
	sysprintf("iwpriv %s maxassoc %s", dev,
		  nvram_default_get(maxassoc, "256"));

	switch (atoi(chanshift)) {
	case 15:
		sysprintf("iwpriv %s channelshift -3", dev);
		break;
	case 10:
		sysprintf("iwpriv %s channelshift -2", dev);
		break;
	case 5:
		sysprintf("iwpriv %s channelshift -1", dev);
		break;
	case 0:
		sysprintf("iwpriv %s channelshift 0", dev);
		break;
	case -5:
		sysprintf("iwpriv %s channelshift 1", dev);
		break;
	case -10:
		sysprintf("iwpriv %s channelshift 2", dev);
		break;
	case -15:
		sysprintf("iwpriv %s channelshift 3", dev);
		break;
	default:
		sysprintf("iwpriv %s channelshift 0", dev);
		break;
	}
	if (!strcmp(apm, "wdssta") || !strcmp(apm, "wdsap"))
		sysprintf("iwpriv %s wds 1", dev);

	if (!strcmp(apm, "wdsap"))
		sysprintf("iwpriv %s wdssep 1", dev);
	else
		sysprintf("iwpriv %s wdssep 0", dev);

	vifs = nvram_safe_get(wifivifs);
	if (vifs != NULL)
		foreach(var, vifs, next) {
		sprintf(net, "%s_net_mode", var);
		if (nvram_match(net, "disabled"))
			continue;
		sprintf(ssid, "%s_ssid", var);
		sprintf(mode, "%s_mode", var);
		sprintf(maxassoc, "%s_maxassoc", var);
		sysprintf("iwpriv %s maxassoc %s", var,
			  nvram_default_get(maxassoc, "256"));
		switch (atoi(chanshift)) {
		case 15:
			sysprintf("iwpriv %s channelshift -3", var);
			break;
		case 10:
			sysprintf("iwpriv %s channelshift -2", var);
			break;
		case 5:
			sysprintf("iwpriv %s channelshift -1", var);
			break;
		case 0:
			sysprintf("iwpriv %s channelshift 0", var);
			break;
		case -5:
			sysprintf("iwpriv %s channelshift 1", var);
			break;
		case -10:
			sysprintf("iwpriv %s channelshift 2", var);
			break;
		case -15:
			sysprintf("iwpriv %s channelshift 3", var);
			break;
		default:
			sysprintf("iwpriv %s channelshift 0", var);
			break;
		}
		char *mvap = nvram_default_get(mode, "ap");
		set_scanlist(dev, wif);
		setRTS(var);
		sleep(1);
		sysprintf("iwpriv %s bgscan 0", var);
		if (strcmp(mvap, "sta") && strcmp(mvap, "wdssta")
		    && strcmp(mvap, "wet")) {
			cprintf("set channel\n");
			char *ch = nvram_default_get(channel, "0");

			if (strcmp(ch, "0") == 0) {
				sysprintf("iwconfig %s channel 0", var);
			} else {
				sysprintf("iwconfig %s freq %sM", var, ch);
			}
		}
		sleep(1);
#ifdef HAVE_MAKSAT
#ifdef HAVE_MAKSAT_BLANK
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "default_vap"));
#else
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "maksat_vap"));
#endif
#elif defined(HAVE_TRIMAX)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "trimax_vap"));
#elif defined(HAVE_WIKINGS)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "Excel Networks_vap"));
#elif defined(HAVE_NEXTMEDIA)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "nextmedia_vap"));
#elif defined(HAVE_TMK)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "KMT_vap"));
#elif defined(HAVE_BKM)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "BKM_vap"));
#elif defined(HAVE_ERC)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "ERC_vap"));
#elif defined(HAVE_CORENET)
		sysprintf("iwconfig %s essid -- \"%s\"", var,
			  nvram_default_get(ssid, "corenet.ap"));
#else
#ifdef HAVE_REGISTER
		if (!isregistered())
			sysprintf("iwconfig %s essid -- need_activation", var);
		else
#endif
			sysprintf("iwconfig %s essid -- \"%s\"", var,
				  nvram_default_get(ssid, "dd-wrt_vap"));
#endif
		cprintf("set broadcast flag vif %s\n", var);	// hide ssid
		sprintf(broadcast, "%s_closed", var);
		sysprintf("iwpriv %s hide_ssid %s", var,
			  nvram_default_get(broadcast, "0"));
		sprintf(wmm, "%s_wmm", var);
		if (nvram_nmatch("1", "%s_pollingmode", var))
			sysprintf("iwpriv %s wmm 1", var);
		else
			sysprintf("iwpriv %s wmm %s", var,
				  nvram_default_get(wmm, "0"));
		char isolate[32];

		sprintf(isolate, "%s_ap_isolate", var);
		if (nvram_default_match(isolate, "1", "0"))
			sysprintf("iwpriv %s ap_bridge 0", var);
		if (!strcmp(mvap, "wdssta") || !strcmp(mvap, "wdsap"))
			sysprintf("iwpriv %s wds 1", var);
		sprintf(mtikie, "%s_mtikie", var);
		if (nvram_default_match(mtikie, "1", "0"))
			sysprintf("iwpriv %s addmtikie 1", var);

#ifdef HAVE_BONDING
		if (!strcmp(mvap, "wdsap") && !isBond(var))
#else
		if (!strcmp(mvap, "wdsap"))
#endif
			sysprintf("iwpriv %s wdssep 1", var);
		else
			sysprintf("iwpriv %s wdssep 0", var);

		// removed hostroaming 0 due to excessive tests and driver research
		// sysprintf("iwpriv %s hostroaming 0", var);
		cnt++;
		}

	sprintf(mtikie, "%s_mtikie", dev);
	if (nvram_default_match(mtikie, "1", "0"))
		sysprintf("iwpriv %s addmtikie 1", dev);

	char isolate[32];

	sprintf(isolate, "%s_ap_isolate", dev);
	if (nvram_default_match(isolate, "1", "0"))
		sysprintf("iwpriv %s ap_bridge 0", dev);
	// removed hostroaming 0 due to excessive tests and driver research
	// sysprintf("iwpriv %s hostroaming 0", dev);

	sprintf(ssid, "ath%d_ssid", count);
	sprintf(broadcast, "ath%d_closed", count);

	memset(var, 0, 80);

	cprintf("set ssid\n");
#ifdef HAVE_MAKSAT
#ifdef HAVE_MAKSAT_BLANK
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "default"));
#else
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "maksat"));
#endif
#elif defined(HAVE_TRIMAX)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "trimax"));
#elif defined(HAVE_WIKINGS)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "Excel Networks"));
#elif defined(HAVE_NEXTMEDIA)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "nextmedia"));
#elif defined(HAVE_TMK)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "KMT"));
#elif defined(HAVE_BKM)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "BKM"));
#elif defined(HAVE_ERC)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "ERC"));
#elif defined(HAVE_CORENET)
	sysprintf("iwconfig %s essid -- \"%s\"", dev,
		  nvram_default_get(ssid, "corenet.ap"));
#else
#ifdef HAVE_REGISTER
	if (!isregistered())
		sysprintf("iwconfig %s essid -- need_activation", dev);
	else
#endif
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "dd-wrt"));
#endif
	cprintf("set broadcast flag\n");	// hide ssid
	sysprintf("iwpriv %s hide_ssid %s", dev,
		  nvram_default_get(broadcast, "0"));
	sysprintf("iwpriv %s bgscan 0", dev);
	apm = nvram_default_get(wl, "ap");

	char preamble[32];

	sprintf(preamble, "%s_preamble", dev);
	if (nvram_default_match(preamble, "1", "0")) {
		sysprintf("iwpriv %s shpreamble 1", dev);
	} else
		sysprintf("iwpriv %s shpreamble 0", dev);

	if (strcmp(apm, "sta") == 0 || strcmp(apm, "infra") == 0
	    || strcmp(apm, "wet") == 0 || strcmp(apm, "wdssta") == 0) {
		cprintf("set ssid\n");
#ifdef HAVE_MAKSAT
#ifdef HAVE_MAKSAT_BLANK
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "default"));
#else
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "maksat"));
#endif
#elif defined(HAVE_TRIMAX)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "trimax"));
#elif defined(HAVE_WIKINGS)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "Excel Networks"));
#elif defined(HAVE_NEXTMEDIA)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "nextmedia"));
#elif defined(HAVE_TMK)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "KMT"));
#elif defined(HAVE_BKM)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "BKM"));
#elif defined(HAVE_ERC)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "ERC"));
#elif defined(HAVE_CORENET)
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "corenet.ap"));
#else
		sysprintf("iwconfig %s essid -- \"%s\"", dev,
			  nvram_default_get(ssid, "dd-wrt"));
#endif
	}

	cprintf("adjust power\n");

	int newpower = atoi(nvram_default_get(power, "16"));

	sysprintf("iwconfig %s txpower %ddBm", dev, newpower);

	cprintf("done()\n");

	cprintf("setup encryption");
	// @todo ifup
	// netconfig

	/*
	 * set_rate (dev);
	 */
	set_rate(dev, dev);

	set_netmode(wif, dev, dev);

	setMacFilter(dev);
	setupKey(dev);
	if (vifs != NULL && strlen(vifs) > 0) {
		foreach(var, vifs, next) {
			setMacFilter(var);
			setupKey(var);
		}
	}

	sleep(1);
	apm = nvram_default_get(wl, "ap");
	if (strcmp(apm, "sta") && strcmp(apm, "wdssta") && strcmp(apm, "wet")) {
		cprintf("set channel\n");
		char *ch = nvram_default_get(channel, "0");

		if (strcmp(ch, "0") == 0) {
			sysprintf("iwconfig %s channel 0", dev);
		} else {
			sysprintf("iwconfig %s freq %sM", dev, ch);
		}
	}
	sleep(1);
	// set inact inact tick (order is important!)
	sprintf(inact_tick, "%s_inact_tick", dev);
	sprintf(inact, "%s_inact", dev);
#ifdef HAVE_MAKSAT
	sysprintf("iwpriv %s inact_tick %s", dev,
		  nvram_default_get(inact_tick, "1"));
	sysprintf("iwpriv %s inact %s", dev, nvram_default_get(inact, "15"));
#else
	sysprintf("iwpriv %s inact_tick %s", dev,
		  nvram_default_get(inact_tick, "15"));
	sysprintf("iwpriv %s inact %s", dev, nvram_default_get(inact, "300"));
#endif

	if (strcmp(apm, "sta")) {
		char bridged[32];

		sprintf(bridged, "%s_bridged", dev);
		if (nvram_default_match(bridged, "1", "1")) {
			sysprintf("ifconfig %s 0.0.0.0 up", dev);
			br_add_interface(getBridge(dev), dev);
			sysprintf("ifconfig %s 0.0.0.0 up", dev);
		} else {
			sysprintf("ifconfig %s mtu %s", dev, getMTU(dev));
			sysprintf("ifconfig %s %s netmask %s up", dev,
				  nvram_nget("%s_ipaddr", dev),
				  nvram_nget("%s_netmask", dev));
		}
	} else {
		char bridged[32];

		sprintf(bridged, "%s_bridged", dev);
		if (nvram_default_match(bridged, "0", "1")) {
			sysprintf("ifconfig %s mtu %s", dev, getMTU(dev));
			sysprintf("ifconfig %s %s netmask %s up", dev,
				  nvram_nget("%s_ipaddr", dev),
				  nvram_nget("%s_netmask", dev));
		}

	}

	// vif netconfig
	vifs = nvram_safe_get(wifivifs);
	if (vifs != NULL && strlen(vifs) > 0) {
		foreach(var, vifs, next) {
			sprintf(mode, "%s_mode", var);
			char *m2 = nvram_default_get(mode, "ap");

			if (strcmp(m2, "sta")) {
				char bridged[32];

				sprintf(bridged, "%s_bridged", var);
				if (nvram_default_match(bridged, "1", "1")) {
					sysprintf("ifconfig %s 0.0.0.0 up",
						  var);
					br_add_interface(getBridge(var), var);
				} else {
					char ip[32];
					char mask[32];

					sprintf(ip, "%s_ipaddr", var);
					sprintf(mask, "%s_netmask", var);
					sysprintf("ifconfig %s mtu %s", var,
						  getMTU(var));
					sysprintf
					    ("ifconfig %s %s netmask %s up",
					     var, nvram_safe_get(ip),
					     nvram_safe_get(mask));
				}
			}
		}
	}
	// setup encryption
	if (strcmp(apm, "sta") && strcmp(apm, "wdssta") && strcmp(apm, "wet"))
		setupHostAP(dev, "madwifi", 0);
	else
		setupSupplicant(dev, NULL);

	vifs = nvram_safe_get(wifivifs);
	if (vifs != NULL)
		foreach(var, vifs, next) {
		sprintf(mode, "%s_mode", var);
		char *vapm = nvram_default_get(mode, "ap");
		if (strcmp(vapm, "sta") && strcmp(vapm, "wdssta")
		    && strcmp(vapm, "wet"))
			setupHostAP(var, "madwifi", 0);
		else
			setupSupplicant(var, NULL);
		}

	for (s = 1; s <= 10; s++) {
		char wdsvarname[32] = { 0 };
		char wdsdevname[32] = { 0 };
		char wdsmacname[32] = { 0 };
		char *wdsdev;
		char *hwaddr;

		sprintf(wdsvarname, "%s_wds%d_enable", dev, s);
		sprintf(wdsdevname, "%s_wds%d_if", dev, s);
		sprintf(wdsmacname, "%s_wds%d_hwaddr", dev, s);
		wdsdev = nvram_safe_get(wdsdevname);
		if (strlen(wdsdev) == 0)
			continue;
		if (nvram_match(wdsvarname, "0"))
			continue;
		hwaddr = nvram_get(wdsmacname);
		if (hwaddr != NULL) {
			sysprintf("iwpriv %s wds_add %s", primary, hwaddr);
			set_rate(dev, primary);
		}
	}

	for (s = 1; s <= 10; s++) {
		char wdsvarname[32] = { 0 };
		char wdsdevname[32] = { 0 };
		char wdsmacname[32] = { 0 };
		char *wdsdev;
		char *hwaddr;

		sprintf(wdsvarname, "%s_wds%d_enable", dev, (11 - s));
		sprintf(wdsdevname, "%s_wds%d_if", dev, (11 - s));
		sprintf(wdsmacname, "%s_wds%d_hwaddr", dev, (11 - s));
		wdsdev = nvram_safe_get(wdsdevname);
		if (strlen(wdsdev) == 0)
			continue;
		if (nvram_match(wdsvarname, "0"))
			continue;
		hwaddr = nvram_get(wdsmacname);
		if (hwaddr != NULL) {
			sysprintf("ifconfig %s 0.0.0.0 up", wdsdev);
		}
	}
}

void start_vifs(void)
{
	char *next;
	char var[80];
	char *vifs;
	char mode[32];
	char *m;
	char wifivifs[32];
	int c = getdevicecount();
	int count = 0;

	for (count = 0; count < c; count++) {
		sprintf(wifivifs, "ath%d_vifs", count);
		vifs = nvram_safe_get(wifivifs);
		if (vifs != NULL && strlen(vifs) > 0) {
			foreach(var, vifs, next) {
				setMacFilter(var);

				sprintf(mode, "%s_mode", var);
				m = nvram_default_get(mode, "ap");

				if (strcmp(m, "sta")) {
					char bridged[32];

					sprintf(bridged, "%s_bridged", var);
					if (nvram_default_match
					    (bridged, "1", "1")) {
						eval("ifconfig", var, "0.0.0.0",
						     "up");
						br_add_interface(getBridge(var),
								 var);
					} else {
						char ip[32];
						char mask[32];

						sprintf(ip, "%s_ipaddr", var);
						sprintf(mask, "%s_netmask",
							var);
						eval("ifconfig", var, "mtu",
						     getMTU(var));
						sysprintf
						    ("ifconfig %s %s netmask %s up",
						     var, nvram_safe_get(ip),
						     nvram_safe_get(mask));
					}
				}
			}
		}
	}

}

void stop_vifs(void)
{
	char *next;
	char var[80];
	char *vifs;
	char mode[32];
	char *m;
	char wifivifs[32];
	int c = getdevicecount();
	int count = 0;

	for (count = 0; count < c; count++) {
		sprintf(wifivifs, "ath%d_vifs", count);
		vifs = nvram_safe_get(wifivifs);
		if (vifs != NULL && strlen(vifs) > 0) {
			foreach(var, vifs, next) {
				eval("ifconfig", var, "down");

			}
		}
	}

}

void start_duallink(void)
{

	if (nvram_match("duallink", "master")) {
		sysprintf("ip route flush table 100");
		sysprintf("ip route flush table 200");
		sysprintf("ip route del fwmark 1 table 200");
		sysprintf("iptables -t mangle -F PREROUTING");
		sysprintf("ip route add %s/%s dev ath0 src %s table 100",
			  nvram_safe_get("ath0_ipaddr"),
			  nvram_safe_get("ath0_netmask"),
			  nvram_safe_get("ath0_ipaddr"));
		sysprintf("ip route default via %s table 100",
			  nvram_safe_get("ath0_duallink_parent"));
		sysprintf("ip route add %s/%s dev ath0 src %s table 200",
			  nvram_safe_get("ath1_ipaddr"),
			  nvram_safe_get("ath1_netmask"),
			  nvram_safe_get("ath1_ipaddr"));
		sysprintf("ip route default via %s table 200",
			  nvram_safe_get("ath1_duallink_parent"));
		sysprintf
		    ("iptables -t mangle -A PREROUTING -i br0 -j MARK --set-mark 1");
		sysprintf("ip rule add fwmark 1 table 200");
	}
	if (nvram_match("duallink", "slave")) {
		sysprintf("ip route flush table 100");
		sysprintf("ip route flush table 200");
		sysprintf("ip route del fwmark 1 table 100");
		sysprintf("iptables -t mangle -F PREROUTING");
		sysprintf("ip route add %s/%s dev ath0 src %s table 100",
			  nvram_safe_get("ath0_ipaddr"),
			  nvram_safe_get("ath0_netmask"),
			  nvram_safe_get("ath0_ipaddr"));
		sysprintf("ip route default via %s table 100",
			  nvram_safe_get("ath0_duallink_parent"));
		sysprintf("ip route add %s/%s dev ath0 src %s table 200",
			  nvram_safe_get("ath1_ipaddr"),
			  nvram_safe_get("ath1_netmask"),
			  nvram_safe_get("ath1_ipaddr"));
		sysprintf("ip route default via %s table 200",
			  nvram_safe_get("ath1_duallink_parent"));
		sysprintf
		    ("iptables -t mangle -A PREROUTING -i br0 -j MARK --set-mark 1");
		sysprintf("ip rule add fwmark 1 table 100");
	}

}

extern void adjust_regulatory(int count);

void configure_wifi(void)	// madwifi implementation for atheros based
				// cards
{
	deconfigure_wifi();
	int c = getdevicecount();
	int i;
	int changed = 0;

	for (i = 0; i < c; i++)
		adjust_regulatory(i);

	for (i = 0; i < c; i++) {
#ifdef REGDOMAIN_OVERRIDE
		// SeG's dirty hack to make everything possible without any channel
		// restrictions. regdomain 0x60 seems to be the best way
		char regdomain[16];

		sprintf(regdomain, "ath%d_regdomain", i);

		// read current reg domain from atheros card
		// the base io 0x50010000 is hardcoded here and can be different on
		// non RB500 ports
		// @fixme: detect io by reading pci data

		cprintf("get reg domain()\n");
		int reg_domain = get_regdomain((0x50010000) + (0x10000 * i));

		if (reg_domain > -1)	// reg domain was successfully readed 
		{
			if (nvram_get(regdomain) != NULL)	// reg domain is
				// defined in nvram
			{
				int destination = atoi(nvram_safe_get(regdomain));	// read 

				// new 
				// target 
				// regdomain
				if (destination != reg_domain)	// check if changed
				{
					if (set_regdomain((0x50010000) + (0x10000 * i), destination) == 0)	// modifiy 
						// eeprom 
						// with 
						// new 
						// regdomain
						changed = 1;
				}
			}

		}
		cprintf("configure next\n");
		if (!changed)	// if regdomain not changed, configure it
#endif
		{
			configure_single(i);
		}
	}

	if (changed)		// if changed, deconfigure myself and
		// reconfigure me in the same way. 
	{
		deconfigure_wifi();
		configure_wifi();
	}
	if (need_commit) {
		nvram_commit();
		need_commit = 0;
	}
	eval("killall", "-9", "roaming_daemon");
	if (getSTA() || getWET())
		eval("roaming_daemon");

	int cnt = getifcount("wifi");
	int s;

	for (c = 0; c < cnt; c++) {
		char br1enable[32];
		char br1ipaddr[32];
		char br1netmask[32];

		sprintf(br1enable, "ath%d_br1_enable", c);
		sprintf(br1ipaddr, "ath%d_br1_ipaddr", c);
		sprintf(br1netmask, "ath%d_br1_netmask", c);
		if (nvram_get(br1enable) == NULL)
			nvram_set(br1enable, "0");
		if (nvram_get(br1ipaddr) == NULL)
			nvram_set(br1ipaddr, "0.0.0.0");
		if (nvram_get(br1netmask) == NULL)
			nvram_set(br1netmask, "255.255.255.0");
		if (nvram_match(br1enable, "1")) {
			ifconfig("br1", 0, 0, 0);

			// eval ("ifconfig", "br1", "down");
			br_del_bridge("br1");
			br_add_bridge("br1");

			if (nvram_match("lan_stp", "0"))
				br_set_stp_state("br1", 0);	// eval ("brctl", "stp",
			// "br1", "off");
			else
				br_set_stp_state("br1", 1);	// eval ("brctl", "stp",
			// "br1", "off");
			br_set_bridge_forward_delay("br1", 1);

			/*
			 * Bring up and configure br1 interface 
			 */
			if (nvram_invmatch(br1ipaddr, "0.0.0.0")) {
				ifconfig("br1", IFUP, nvram_safe_get(br1ipaddr),
					 nvram_safe_get(br1netmask));

				if (nvram_match("lan_stp", "0"))
					br_set_stp_state("br1", 0);	// eval ("brctl",
				// "stp", "br1",
				// "off");
				else
					br_set_stp_state("br1", 1);	// eval ("brctl",
				// "stp", "br1",
				// "off");

			}

		}
	}
	for (c = 0; c < cnt; c++) {

		for (s = 1; s <= MAX_WDS_DEVS; s++) {
			char wdsvarname[32] = { 0 };
			char wdsdevname[32] = { 0 };
			char *dev;

			char br1enable[32];

			sprintf(wdsvarname, "ath%d_wds%d_enable", c, s);
			sprintf(wdsdevname, "ath%d_wds%d_if", c, s);
			sprintf(br1enable, "ath%d_br1_enable", c);
			if (nvram_get(wdsvarname) == NULL)
				nvram_set(wdsvarname, "0");
			dev = nvram_safe_get(wdsdevname);
			if (strlen(dev) == 0)
				continue;
			ifconfig(dev, 0, 0, 0);

			// eval ("ifconfig", dev, "down");
			if (nvram_match(wdsvarname, "1")) {
				char *wdsip;
				char *wdsnm;
				char wdsbc[32] = { 0 };
				wdsip = nvram_nget("ath%d_wds%d_ipaddr", c, s);
				wdsnm = nvram_nget("ath%d_wds%d_netmask", c, s);

				snprintf(wdsbc, 31, "%s", wdsip);
				get_broadcast(wdsbc, wdsnm);
				eval("ifconfig", dev, wdsip, "broadcast",
				     wdsbc, "netmask", wdsnm, "up");
			} else if (nvram_match(wdsvarname, "2")
				   && nvram_match(br1enable, "1")) {
				eval("ifconfig", dev, "up");
				sleep(1);
				br_add_interface("br1", dev);
			} else if (nvram_match(wdsvarname, "3")) {
				ifconfig(dev, IFUP, 0, 0);
				sleep(1);
				br_add_interface(getBridge(dev), dev);
			}
		}
	}
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
		return;
	struct ifreq ifr;

#ifdef HAVE_RB500
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_X86
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_XSCALE
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_MAGICBOX
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_RB600
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_FONERA
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_LS2
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_SOLO51
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_LS5
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_WHRAG108
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_PB42
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_LSX
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_DANUBE
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_STORM
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_OPENRISC
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_ADM5120
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_TW6600
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
#ifdef HAVE_CA8
	strncpy(ifr.ifr_name, "ath0", IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
		char eabuf[32];

		nvram_set("wl0_hwaddr",
			  ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
	}
#endif
	close(s);
	reset_hwaddr(nvram_safe_get("lan_ifname"));
}

void start_deconfigurewifi(void)
{
	deconfigure_wifi();
}

void start_configurewifi(void)
{
	configure_wifi();
}
#endif
