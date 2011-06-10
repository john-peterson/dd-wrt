/*
 * sysinit-danube.c
 *
 * Copyright (C) 2006 Sebastian Gottschall <gottschall@dd-wrt.com>
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
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>

#define SIOCGMIIREG	0x8948	/* Read MII PHY register.  */
#define SIOCSMIIREG	0x8949	/* Write MII PHY register.  */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/mii.h>
#include "devices/wireless.c"

/*
	option unit		0
	option encaps	llc
	option vpi		1
	option vci		32
	option payload	bridged # some ISPs need this set to 'routed'

	local cfg="$1"

	local atmdev
	config_get atmdev "$cfg" atmdev 0

	local unit
	config_get unit "$cfg" unit 0

	local vpi
	config_get vpi "$cfg" vpi 8

	local vci
	config_get vci "$cfg" vci 35

	local encaps
	config_get encaps "$cfg" encaps

	case "$encaps" in
		1|vc) encaps=1;;
		*) encaps=0;;
	esac

	local payload
	config_get payload "$cfg" payload

	case "$payload" in
		0|routed) payload=0;;
		*) payload=1;;
	esac

	local qos
	config_get qos "$cfg" qos

	local circuit="$atmdev.$vpi.$vci"
	local pid="/var/run/br2684ctl-$circuit.pid"

	start-stop-daemon -S -b -x /usr/sbin/br2684ctl -m -p "$pid" -- \
		-c "$unit" -e "$encaps" -p "$payload" \
		-a "$circuit" ${qos:+-q "$qos"}

*/

void start_sysinit(void)
{
	char buf[PATH_MAX];
	struct stat tmp_stat;
	time_t tm = 0;

	cprintf("sysinit() setup console\n");
	if (!nvram_match("disable_watchdog", "1"))
		eval("watchdog");
	/*
	 * Setup console 
	 */

	cprintf("sysinit() klogctl\n");
	klogctl(8, NULL, atoi(nvram_safe_get("console_loglevel")));
	cprintf("sysinit() get router\n");

	int brand = getRouterBrand();

	/*
	 * Modules 
	 */

	/*
	 * network drivers 
	 */
	// insmod("ag7100_mod");
	// sleep(1);
	//load dsl drivers
	insmod("lantiq_mei");
	insmod("lantiq_atm");
	insmod("drv_dsl_cpe_api");
#ifdef HAVE_ANNEXB
	sysprintf
	    ("/usr/sbin/dsl_cpe_control -i -f /usr/lib/firmware/annex_b.bin &");
#elif HAVE_ANNEXA
	sysprintf
	    ("/usr/sbin/dsl_cpe_control -i -f /usr/lib/firmware/annex_a.bin &");
#else
	if (nvram_match("annex", "a"))
		sysprintf
		    ("/usr/sbin/dsl_cpe_control -i -f /usr/lib/firmware/annex_a.bin &");
	else
		sysprintf
		    ("/usr/sbin/dsl_cpe_control -i -f /usr/lib/firmware/annex_b.bin &");
#endif
	nvram_default_get("vpi", "1");
	nvram_default_get("vci", "32");
	eval("ifconfig", "eth0", "up");
	detect_wireless_devices();
	struct ifreq ifr;
	int s;

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW))) {
		char eabuf[32];

		strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
		ioctl(s, SIOCGIFHWADDR, &ifr);
		nvram_set("et0macaddr",
			  ether_etoa((unsigned char *)ifr.ifr_hwaddr.sa_data,
				     eabuf));
		nvram_set("et0macaddr_safe",
			  ether_etoa((unsigned char *)ifr.ifr_hwaddr.sa_data,
				     eabuf));
		close(s);
	}
#ifdef HAVE_WMBR_G300NH
	FILE *fp = fopen("/dev/mtdblock/6", "rb");
	if (fp) {
		char mactmp[6];
		int copy[6];
		int i;
		char mac1[32];
		fseek(fp, 0x1fd0024 + 12, SEEK_SET);
		fread(mactmp, 6, 1, fp);
		for (i = 0; i < 6; i++)
			copy[i] = mactmp[i];
		for (i = 0; i < 6; i++)
			copy[i] &= 0xff;
		sprintf(mac1, "%02X:%02X:%02X:%02X:%02X:%02X", copy[0],
			copy[1], copy[2], copy[3], copy[4], copy[5]);
		eval("ifconfig", "wifi0", "hw", "ether", mac1);
	}

	system2("echo 15 >/proc/sys/dev/wifi0/ledpin");
	system2("echo 1 >/proc/sys/dev/wifi0/softled");
	led_control(LED_POWER, LED_ON);
	led_control(LED_SES, LED_OFF);
	led_control(LED_SES2, LED_OFF);
	led_control(LED_DIAG, LED_OFF);
	led_control(LED_BRIDGE, LED_OFF);
	led_control(LED_WLAN0, LED_OFF);
	led_control(LED_WLAN1, LED_OFF);
	led_control(LED_CONNECTED, LED_OFF);
	system2("gpio disable 1");
	system2("gpio disable 18");
#endif

	/*
	 * Set a sane date 
	 */
	stime(&tm);
	nvram_set("wl0_ifname", "ath0");

	return;
	cprintf("done\n");
}

int check_cfe_nv(void)
{
	nvram_set("portprio_support", "0");
	return 0;
}

int check_pmon_nv(void)
{
	return 0;
}

void start_overclocking(void)
{
}

void enable_dtag_vlan(int enable)
{

}
