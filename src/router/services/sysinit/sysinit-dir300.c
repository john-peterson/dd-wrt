/*
 * sysinit-dir300.c
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
#include <sys/socket.h>
#include <sys/sysmacros.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <linux/if_ether.h>
#include <linux/mii.h>
#include <linux/sockios.h>
#include <net/if.h>


#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>

extern void vlan_init (int num);


int
start_sysinit (void)
{
  char buf[PATH_MAX];
  struct utsname name;
  struct stat tmp_stat;
  time_t tm = 0;
  unlink ("/etc/nvram/.lock");
  cprintf ("sysinit() proc\n");
  /* /proc */
  mount ("proc", "/proc", "proc", MS_MGC_VAL, NULL);
  mount ("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);
  cprintf ("sysinit() tmp\n");

  /* /tmp */
  mount ("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);
  // fix for linux kernel 2.6
  mount ("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);
  eval ("mkdir", "/tmp/www");
  eval ("mknod", "/dev/nvram", "c", "229", "0");
  eval ("mknod", "/dev/ppp", "c", "108", "0");

  unlink ("/tmp/nvram/.lock");
  eval ("mkdir", "/tmp/nvram");
  cprintf ("sysinit() var\n");

  /* /var */
  mkdir ("/tmp/var", 0777);
  mkdir ("/var/lock", 0777);
  mkdir ("/var/log", 0777);
  mkdir ("/var/run", 0777);
  mkdir ("/var/tmp", 0777);
  cprintf ("sysinit() setup console\n");
  eval ("/sbin/watchdog");
  /* Setup console */

  cprintf ("sysinit() klogctl\n");
  klogctl (8, NULL, atoi (nvram_safe_get ("console_loglevel")));
  cprintf ("sysinit() get router\n");

  FILE *fp = fopen ("/dev/mtdblock/6", "rb");
  if (fp)
    {
      fseek (fp, 0x1000, SEEK_SET);
      unsigned int test;
      fread (&test, 4, 1, fp);
      if (test != 0xffffffff)
	{
	  fprintf (stderr,
		   "radio config fixup is required to clean bad stuff out of memory, otherwise the radio config cannot be detected\n");
	  fseek (fp, 0, SEEK_SET);
	  char *block = (char *) malloc (65536);
	  fread (block, 65536, 1, fp);
	  fclose (fp);
	  int i;
	  for (i = 0x1000; i < 65536; i++)
	    block[i] = 0xff;
	  fp = fopen ("/tmp/radio", "wb");
	  fwrite (block, 65536, 1, fp);
	  eval ("mtd", "-f", "write", "/tmp/radio", "board_config");	//writes back new config and reboots
	  eval ("event", "5", "1", "15");
	}
      fclose (fp);
    }


  /* Modules */
  uname (&name);
/* network drivers */
  eval ("insmod", "ar2313");
  eval ("insmod", "ath_hal");
  eval ("insmod", "ath_ahb");
//  eval ("ifconfig", "wifi0", "up");
  eval ("ifconfig", "eth0", "up");	// wan
  system ("echo 2 >/proc/sys/dev/wifi0/ledpin");
  system ("echo 1 >/proc/sys/dev/wifi0/softled");
  if (getRouterBrand () == ROUTER_BOARD_FONERA2200)
    {
      eval ("ifconfig", "eth0", "up");	// required for vlan config
      eval ("/sbin/vconfig", "set_name_type", "VLAN_PLUS_VID_NO_PAD");
      eval ("/sbin/vconfig", "add", "eth0", "0");
      eval ("/sbin/vconfig", "add", "eth0", "1");
    }
  else
    {
      vlan_init (5);		// 4 lan + 1 wan
    }
//  eval ("insmod", "ipv6");

  /* Set a sane date */
  stime (&tm);
  nvram_set ("wl0_ifname", "ath0");

  return 0;
}

int
check_cfe_nv (void)
{
  nvram_set ("portprio_support", "0");
  return 0;
}

int
check_pmon_nv (void)
{
  return 0;
}

void
start_overclocking (void)
{
}
void
enable_dtag_vlan (int enable)
{

}
