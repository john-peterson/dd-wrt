/*
 * sysinit-pb42.c
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

#define sys_reboot() sysprintf("startservice run_rc_shutdown"); eval("sync"); eval("event","3","1","15")

static void install_sdcard(void)
{
	FILE *fp = fopen("/boot/.installed", "rb");
	if (fp != NULL)		// already locally installed?
	{
		fclose(fp);
		return;
	}
	sleep(10);		//give some time until sd is up
	fprintf(stderr, "check if secondary device is available\n");
	fp = fopen("/dev/discs/disc1/disc", "rb");
	if (fp == NULL)		// no cf disc installed or no sd card. doesnt matter, we exit if no secondary device is in
	{
		fclose(fp);
		return;
	}
	fclose(fp);
	fprintf(stderr, "installing firmware to internal SD Card\n");
	eval("mkdir", "/tmp/install");
	int check =
	    mount("/dev/discs/disc1/disc", "/tmp/install", "ext2", MS_MGC_VAL,
		  NULL);
	if (check != 0) {
		fprintf(stderr, "device isnt formated, use EXT2\n");
		fp = fopen("/dev/discs/disc1/disc", "rb");
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		size -= 65536 * 16;
		size /= 4096;
		char newsize[32];
		sprintf(newsize, "%d", size);
		eval("mke2fs", "-b", "4096", "-N", "65536", "-L", "dd-wrt",
		     "/dev/discs/disc1/disc", newsize);
		mount("/dev/discs/disc1/disc", "/tmp/install", "ext2",
		      MS_MGC_VAL, NULL);
	}
	fprintf(stderr, "copy files to SD Card\n");
	eval("cp", "-f", "/tmp/install/usr/local/nvram/nvram.bin",
	     "/tmp/install/usr/local/nvram/nvram.bak");
	eval("cp", "-r", "-d", "-f", "/boot", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/bin", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/etc", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/jffs", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/lib", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/mmc", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/mnt", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/opt", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/sbin", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/usr", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/www", "/tmp/install");
	eval("cp", "-r", "-d", "-f", "/var", "/tmp/install");
	eval("mv", "-f", "/tmp/install/usr/local/nvram/nvram.bak",
	     "/tmp/install/usr/local/nvram/nvram.bin");
	eval("mkdir", "/tmp/install/dev");
	eval("mkdir", "/tmp/install/sys");
	eval("mkdir", "/tmp/install/proc");
	eval("mkdir", "/tmp/install/tmp");
	sysprintf("echo \"blank\" > /tmp/install/boot/.installed");
	sysprintf("echo \"mem=59M root=/dev/sda\" > /tmp/install/boot/kparam");
	eval("umount", "/tmp/install");
	eval("sync");
	fprintf(stderr, "signal installation complete\n");
	eval("gpio", "enable", "4");
	sleep(1);
	eval("gpio", "disable", "4");
}

void start_sysinit(void)
{
	char buf[PATH_MAX];
	struct utsname name;
	struct stat tmp_stat;
	time_t tm = 0;

	unlink("/etc/nvram/.lock");
	cprintf("sysinit() proc\n");
	/*
	 * /proc 
	 */
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);
	mount("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);
	cprintf("sysinit() tmp\n");

	/*
	 * /tmp 
	 */
	mount("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);
	// fix for linux kernel 2.6
	mount("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);
	mount("devpts", "/proc/bus/usb", "usbfs", MS_MGC_VAL, NULL);
	eval("mkdir", "/tmp/www");
	eval("mknod", "/dev/gpio", "c", "127", "0");
	eval("mknod", "/dev/nvram", "c", "229", "0");
	eval("mknod", "/dev/ppp", "c", "108", "0");
	eval("mkdir", "-p", "/usr/local/nvram");

	unlink("/tmp/nvram/.lock");
	eval("mkdir", "/tmp/nvram");

	/*
	 * /var 
	 */
	mkdir("/tmp/var", 0777);
	mkdir("/var/lock", 0777);
	mkdir("/var/log", 0777);
	mkdir("/var/run", 0777);
	mkdir("/var/tmp", 0777);
	install_sdcard();
	cprintf("sysinit() setup console\n");
	eval("insmod", "ks8695_wdt", "wdt_time=30");	// load watchdog module with 30 seconds timeout
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
	uname(&name);

	/*
	 * network drivers 
	 */
	eval("ifconfig", "eth0", "up");
	eval("ifconfig", "eth1", "up");
	eval("ifconfig", "eth2", "up");
	eval("ifconfig", "eth3", "up");
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

	detect_wireless_devices();

	system2("echo 1 >/proc/sys/dev/wifi0/softled");


#ifdef HAVE_ERC
// start sercd
	system2("/usr/sbin/sercd -e -p 3696 -l "" 5 /dev/tts/2 /var/lock/tty02.lock &");
#endif



	/*
	 * Set a sane date 
	 */
	stime(&tm);
	nvram_set("wl0_ifname", "ath0");
	//disable led's 
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
