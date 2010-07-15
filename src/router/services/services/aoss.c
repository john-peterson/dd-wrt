/*
 * aoss.c
 *
 * Copyright (C) 2010 Sebastian Gottschall <gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
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
#ifdef HAVE_AOSS
#include <stdlib.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include <syslog.h>
#include <signal.h>

void stop_aoss(void);

void start_aoss(void)
{
	int ret;

	if (nvram_match("aoss_enable", "0")) {
		stop_aoss();
		return;
	}
	if (pidof("aoss") > 0)
	    return;
	system("killall ledtool");
	nvram_set("aoss_success","0");
	led_control(LED_SES, LED_OFF);
	system("ledtool 180 2");
	char *vifbak = nvram_safe_get("ath0_vifs");
	char copy[256];
	strcpy(copy, vifbak);
	sysprintf("startservice deconfigurewifi");
	nvram_unset("ath0_vifs");
	sysprintf("startservice configurewifi");
	nvram_set("ath0_vifs", copy);
	nvram_commit();

	sysprintf("80211n_wlanconfig aoss create wlandev wifi0 wlanmode ap");
	sysprintf("iwconfig aoss essid ESSID-AOSS");
	sysprintf("iwpriv aoss authmode 4");
	sysprintf("iwconfig aoss key [1] 4D454C434F");
	sysprintf("iwconfig aoss key [1]");
	sysprintf("ifconfig aoss 0.0.0.0 up");

	ret = eval("aoss", "-i", "aoss", "-m", "ap");
	dd_syslog(LOG_INFO, "aoss : aoss daemon successfully started\n");
	cprintf("done\n");
	return;
}

void stop_aoss(void)
{
	stop_process("aoss", "buffalo aoss daemon");
	return;
}

#endif
