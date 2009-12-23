/*
 * sysinit-x86.c
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
 * 
 * System Initialisation for Standard PC and compatible Routers
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

#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/mii.h>
#include "devices/ethernet.c"

static int getdiscindex(void)	// works only for squashfs 
{
	int i;

	for (i = 0; i < 10; i++) {
		char dev[64];

		sprintf(dev, "/dev/discs/disc%d/part2", i);
		FILE *in = fopen(dev, "rb");

		if (in == NULL)
			continue;	// no second partition or disc does not
		// exist, skipping
		char buf[4];

		fread(buf, 4, 1, in);
		if (buf[0] == 'h' && buf[1] == 's' && buf[2] == 'q'
		    && buf[3] == 't') {
			fclose(in);
			// filesystem detected
			return i;
		}
		fclose(in);
	}
	return -1;
}

void start_sysinit(void)
{
	struct utsname name;
	time_t tm = 0;

	unlink("/etc/nvram/.lock");
	cprintf("sysinit() proc\n");
	/*
	 * /proc 
	 */
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);
	mount("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);
	mount("debugfs", "/sys/kernel/debug", "debugfs", MS_MGC_VAL, NULL);
	cprintf("sysinit() tmp\n");

	/*
	 * /tmp 
	 */
	mount("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);
	mount("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);
	mount("devpts", "/proc/bus/usb", "usbfs", MS_MGC_VAL, NULL);
	eval("mknod", "/dev/ppp", "c", "108", "0");
	eval("mknod", "/dev/nvram", "c", "229", "0");
	char dev[64];
	int index = getdiscindex();

	if (index == -1) {
		fprintf(stderr,
			"no valid dd-wrt partition found, calling shell");
		eval("/bin/sh");
		exit(0);
	}
	// sprintf (dev, "/dev/discs/disc%d/part1", index);
	// mount (dev, "/boot", "ext2", MS_MGC_VAL, NULL);

	sprintf(dev, "/dev/discs/disc%d/part3", index);
	// eval("insmod","block2mtd",dev);
	// sprintf (dev, "/dev/mtdblock/0");
	// eval ("fsck", dev); //checking nvram partition and correcting errors
	// detect jffs
	/*
	 * FILE *fp=fopen(dev,"rb"); int nojffs=0; if (getc(fp)!=0x85)nojffs=1;
	 * if (getc(fp)!=0x19)nojffs=1; fclose(fp);
	 */
	// eval("mtd","erase","mtd0");

	if (mount(dev, "/usr/local", "ext2", MS_MGC_VAL, NULL)) {
		eval("/sbin/mke2fs", "-F", "-b", "1024", dev);
		mount(dev, "/usr/local", "ext2", MS_MGC_VAL, NULL);
		eval("/bin/tar", "-xvvjf", "/etc/local.tar.bz2", "-C", "/");
	}
	eval("mkdir", "-p", "/usr/local/nvram");

	FILE *in = fopen("/usr/local/nvram/nvram.db", "rb");

	if (in != NULL) {
		fclose(in);
		eval("mkdir", "/tmp/nvram");
		eval("cp", "/etc/nvram/nvram.db", "/tmp/nvram");
		eval("cp", "/etc/nvram/offsets.db", "/tmp/nvram");
		eval("/usr/sbin/convertnvram");
		nvram_commit();
		eval("rm", "-f", "/etc/nvram/nvram.db");
		eval("rm", "-f", "/etc/nvram/offsets.db");
	}
	eval("mkdir", "/tmp/www");

	unlink("/tmp/nvram/.lock");
	eval("mkdir", "/tmp/nvram");

	cprintf("sysinit() var\n");

	/*
	 * /var 
	 */
	mkdir("/tmp/var", 0777);
	mkdir("/var/lock", 0777);
	mkdir("/var/log", 0777);
	mkdir("/var/run", 0777);
	mkdir("/var/tmp", 0777);

	if (!nvram_match("disable_watchdog", "1"))
		eval("watchdog");	// system watchdog

	cprintf("sysinit() setup console\n");

	/*
	 * Setup console 
	 */

	cprintf("sysinit() klogctl\n");
	klogctl(8, NULL, atoi(nvram_safe_get("console_loglevel")));
	cprintf("sysinit() get router\n");

	/*
	 * Modules 
	 */
	uname(&name);

	/*
	 * eval("insmod","md5"); eval("insmod","aes"); eval("insmod","blowfish");
	 * eval("insmod","deflate"); eval("insmod","des");
	 * eval("insmod","michael_mic"); eval("insmod","cast5");
	 * eval("insmod","crypto_null"); 
	 */

	detect_ethernet_devices();
	eval("ifconfig", "eth0", "0.0.0.0", "up");
	eval("ifconfig", "eth1", "0.0.0.0", "up");
	eval("ifconfig", "eth2", "0.0.0.0", "up");
	eval("ifconfig", "eth3", "0.0.0.0", "up");

	struct ifreq ifr;
	int s;

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW))) {
		char eabuf[32];

		strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
		ioctl(s, SIOCGIFHWADDR, &ifr);
		nvram_set("et0macaddr_safe",
			  ether_etoa((unsigned char *)ifr.ifr_hwaddr.sa_data,
				     eabuf));
		close(s);
	}
#ifndef HAVE_NOWIFI
	insmod("ath_hal");
	if (nvram_get("rate_control") != NULL) {
		char rate[64];

		sprintf(rate, "ratectl=%s", nvram_safe_get("rate_control"));
		eval("insmod", "ath_pci", rate);
	} else {
		insmod("ath_pci");
	}
#ifdef HAVE_MADWIFI_MIMO
	fprintf(stderr, "load ATH 802.11n Driver\n");
	insmod("/lib/80211n/ath_mimo_hal.ko");
	if (nvram_get("rate_control") != NULL) {
		char rate[64];

		sprintf(rate, "ratectl=%s", nvram_safe_get("rate_control"));
		insmod("/lib/80211n/ath_mimo_pci.ko");
		eval("insmod", "ath_mimo_pci");
	} else {
		insmod("/lib/80211n/ath_mimo_pci.ko");
	}
#endif

	/*
	 * eval ("ifconfig", "wifi0", "up"); eval ("ifconfig", "wifi1", "up");
	 * eval ("ifconfig", "wifi2", "up"); eval ("ifconfig", "wifi3", "up");
	 * eval ("ifconfig", "wifi4", "up"); eval ("ifconfig", "wifi5", "up");
	 */
#endif

	insmod("ipv6");
	eval("mknod", "/dev/rtc", "c", "253", "0");
#ifdef HAVE_CPUTEMP
	// insmod("nsc_gpio");
	// insmod("scx200_gpio");
	// insmod("scx200_i2c");
	// insmod("scx200_acb");
	// insmod("lm77");
#endif

	nvram_set("wl0_ifname", "ath0");
	eval("mknod", "/dev/crypto", "c", "10", "70");
	/*
	 * Set a sane date 
	 */
	stime(&tm);
	cprintf("done\n");
	return;
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
