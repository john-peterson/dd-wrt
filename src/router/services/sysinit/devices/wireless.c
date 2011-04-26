/*
 * wireless.c
 *
 * Copyright (C) 2009 Sebastian Gottschall <gottschall@dd-wrt.com>
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
 * 
 * detects atheros wireless adapters and loads the drivers
 */

// extern int getath9kdevicecount(void);
extern void delete_ath9k_devices(char *physical_iface);

static void detect_wireless_devices(void)
{
#ifdef HAVE_RT61
	FILE *fp = fopen("/sys/bus/pci/devices/0000:00:0e.0/device", "rb");
	if (fp) {
		char str[32];
		fscanf(fp, "%s", str);
		fclose(fp);
		if (!strcmp(str, "0x3592"))
			nvram_set("rtchip", "3062");
		else
			nvram_set("rtchip", "2860");
	} else
		nvram_set("rtchip", "2860");

#endif
#ifndef HAVE_NOWIFI
	nvram_default_get("rate_control", "minstrel");
#ifdef HAVE_MADWIFI
	fprintf(stderr, "load ATH 802.11 a/b/g Driver\n");
	insmod("ath_hal");
	if (nvram_get("rate_control") != NULL) {
		char rate[64];

		sprintf(rate, "ratectl=%s", nvram_safe_get("rate_control"));
		eval("insmod", "ath_pci", rate);
		eval("insmod", "ath_ahb", rate);
	} else {
		insmod("ath_pci");
		insmod("ath_ahb");
	}
#ifdef HAVE_ATH9K
#ifndef HAVE_MADWIFI_MIMO
	if (1)
#else
	if (nvram_match("mimo_driver", "ath9k"))
#endif
	{
		fprintf(stderr, "load ATH9K 802.11n Driver\n");
		// some are just for future use and not (yet) there
		insmod("/lib/ath9k/compat.ko");
		insmod("/lib/ath9k/compat_firmware_class.ko");
		insmod("/lib/ath9k/cfg80211.ko");
		insmod("/lib/ath9k/mac80211.ko");
		insmod("/lib/ath9k/ath.ko");
		insmod("/lib/ath9k/ath9k_hw.ko");
		insmod("/lib/ath9k/ath9k_common.ko");
#ifdef HAVE_WZRG450
		system("/sbin/insmod /lib/ath9k/ath9k.ko blink=1");
#else
		insmod("/lib/ath9k/ath9k.ko");
#endif
		delete_ath9k_devices(NULL);
	} else {
#endif
#ifdef HAVE_MADWIFI_MIMO
		fprintf(stderr, "load ATH 802.11n Driver\n");
		insmod("/lib/80211n/ath_mimo_hal.ko");
		if (nvram_get("rate_control") != NULL) {
			char rate[64];

			sprintf(rate, "ratectl=%s",
				nvram_safe_get("rate_control"));
			insmod("/lib/80211n/ath_mimo_pci.ko");
			insmod("/lib/80211n/ath_mimo_ahb.ko");
		} else {
			insmod("/lib/80211n/ath_mimo_pci.ko");
			insmod("/lib/80211n/ath_mimo_ahb.ko");
		}
#endif
#ifdef HAVE_ATH9K
	}
#endif
#endif
#endif
}
