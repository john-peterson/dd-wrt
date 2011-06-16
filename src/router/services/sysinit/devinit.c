/*
 * devinit.c
 *
 * Copyright (C) 2010 Sebastian Gottschall <gottschall@dd-wrt.com>
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
#include <cymac.h>
#define SIOCGMIIREG	0x8948	/* Read MII PHY register.  */
#define SIOCSMIIREG	0x8949	/* Write MII PHY register.  */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>


#ifdef HAVE_X86

static char *getdisc(void)	// works only for squashfs 
{
	int i;
	unsigned char *disks[]={"/dev/sda2","/dev/sdb2","/dev/sdc2","/dev/sdd2","/dev/sde2","/dev/sdf2","/dev/sdg2","/dev/sdh2","/dev/sdi2"};
	for (i = 0; i < 9; i++) {
		char dev[64];

		strcpy(dev, disks[i]);
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
			return disks[i];
		}
		fclose(in);
	}
	return NULL;
}

#endif
void start_devinit(void)
{
	unlink("/etc/nvram/.lock");
	cprintf("sysinit() proc\n");

	/*
	 * /proc 
	 */
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);
	mount("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);
#ifdef HAVE_ATH9K
	mount("debugfs", "/sys/kernel/debug", "debugfs", MS_MGC_VAL, NULL);
#endif
	cprintf("sysinit() tmp\n");
#ifdef HAVE_X86
	char dev[64];
	char *disc = getdisc();

	if (disc == NULL) {
		fprintf(stderr,
			"no valid dd-wrt partition found, calling shell");
		eval("/bin/sh");
		exit(0);
	}
	// sprintf (dev, "/dev/discs/disc%d/part1", index);
	// mount (dev, "/boot", "ext2", MS_MGC_VAL, NULL);

	sprintf(dev, "/dev/discs/disc%d/part3", index);
	if (mount(dev, "/usr/local", "ext2", MS_MGC_VAL, NULL)) {
		eval("/sbin/mke2fs", "-F", "-b", "1024", dev);
		mount(dev, "/usr/local", "ext2", MS_MGC_VAL, NULL);
//		eval("/bin/tar", "-xvvjf", "/etc/local.tar.bz2", "-C", "/");
	}
	eval("mkdir", "-p", "/usr/local/nvram");
#endif
	/*
	 * /tmp 
	 */
	mount("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);
#ifdef HAVE_HOTPLUG2
	// shell-skript. otherwise we loose our console
	system("echo >/proc/sys/kernel/hotplug");
	system("mount -t tmpfs none /dev -o size=512K");
	system("mknod /dev/console c 5 1");
	system("mknod /dev/null c 1 3");
	system("mkdir /dev/pts");
#else
	// fix for linux kernel 2.6
	eval("mknod", "/dev/ppp", "c", "108", "0");
#endif
// fix me udevtrigger does not create that (yet) not registered?
	eval("mknod", "/dev/nvram", "c", "229", "0");
	eval("mknod", "/dev/watchdog", "c", "10", "130");

	mount("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);
	mount("devpts", "/proc/bus/usb", "usbfs", MS_MGC_VAL, NULL);
	eval("mkdir", "/tmp/www");

	unlink("/tmp/nvram/.lock");
	eval("mkdir", "/tmp/nvram");

	/*
	 * /var 
	 */
	mkdir("/tmp/var", 0777);
	mkdir("/var/lock", 0777);
	mkdir("/var/lock/subsys", 0777);
	mkdir("/var/log", 0777);
	mkdir("/var/run", 0777);
	mkdir("/var/tmp", 0777);
fprintf(stderr,"starting hotplug\n");
#ifdef HAVE_HOTPLUG2
	system("/etc/hotplug2.startup");
#endif
fprintf(stderr,"done\n");
}
