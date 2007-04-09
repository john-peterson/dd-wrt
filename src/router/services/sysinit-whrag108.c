/*
 * sysinit-magicbox.c
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

char wanifname[8], wlifname[8];
#define SIOCGMIIREG	0x8948	/* Read MII PHY register.       */
#define SIOCSMIIREG	0x8949	/* Write MII PHY register.      */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/mii.h>

//highly experimental

void
setRegister (int socket, short reg, short value)
{
//  struct mii_ioctl_data data;
  struct ifreq ifr;
  unsigned short *data = (unsigned short *) (&ifr.ifr_data);
  data[0] = 0;
  data[1] = reg;
  data[2] = value;
  (void) strncpy (ifr.ifr_name, "eth0", sizeof ("eth0"));
//  data.reg_num = reg;
//  data.val_in = value;
  ioctl (socket, SIOCSMIIREG, &ifr);
}

void
switch_main (int argc, char *argv[])
{
  int reg = atoi (argv[1]);
  int val = atoi (argv[2]);
  int s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
    {
      return;
    }
  setRegister (s, reg, val);
  close (s);
}

void
setupSwitch (void)
{
  int s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
    {
      return;
    }
//  setRegister(s,0x02,0xa0);

//Enable 8021Q (80) and IGMP snooping (40)
  //setRegister(s,0x05,0xa0);
//vlan1: valid,5,2,1 port fid=1 vid=1 
  // setRegister(s,0x76,0x21);
  // setRegister(s,0x77,0x10);
  // setRegister(s,0x78,0x01);
//write (04) and trigger address 0
  // setRegister(s,0x6E,0x04);
  // setRegister(s,0x6F,0x00);
//vlan2: valid,5,4,3 port fid=2 vid=2 
  // setRegister(s,0x76,0x3E);
  // setRegister(s,0x77,0x20);
  // setRegister(s,0x78,0x02);

//write (04) and trigger address 0
//  setRegister(s,0x6E,0x04);
//  setRegister(s,0x6F,0x01);

//config port 1,2 to VLAN id 1
  setRegister (s, 0x14, 0x01);
//config port 1,2 to filter vid 1
  setRegister (s, 0x12, 0x46);

//config port 3,4 to VLAN id 2
  setRegister (s, 0x24, 0x02);
  setRegister (s, 0x34, 0x02);
  setRegister (s, 0x44, 0x02);
  setRegister (s, 0x54, 0x02);
//config port 3,4 to filter vid 2
  setRegister (s, 0x22, 0x46);
  setRegister (s, 0x32, 0x46);
  setRegister (s, 0x42, 0x46);
  setRegister (s, 0x52, 0x46);

//for IGMP, disenable special tagging
//  setRegister(s,0x0b,0x01);
//enable vlan tag insertion por 5
//  setRegister(s,0x50,0x04);
//  setRegister(s,0x52,0x06);
//remove it from all others
  setRegister (s, 0x10, 0x02);
  setRegister (s, 0x20, 0x02);
  setRegister (s, 0x30, 0x02);
  setRegister (s, 0x40, 0x02);
  setRegister (s, 0x50, 0x02);
//switch enable
  setRegister (s, 0x01, 0x01);
  close (s);

}

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

  unlink ("/tmp/nvram/.lock");
  eval ("mkdir", "/tmp/nvram");
  eval ("/bin/tar", "-xzf", "/dev/mtdblock/3", "-C", "/");
  FILE *in=fopen("/tmp/nvram/nvram.db","rb");
  if (in!=NULL)
    {
    fclose(in);
    eval("/usr/sbin/convertnvram");
    }
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

  int brand = getRouterBrand ();


  /* Modules */
  uname (&name);

/* network drivers */
  eval ("insmod", "ar2313eth0");
  eval ("insmod", "ar2313eth1");
//  setupSwitch();
//  eval ("ifconfig", "eth0", "0.0.0.0", "up");
//  eval ("vconfig", "add", "eth0", "1");
//  eval ("vconfig", "add", "eth0", "2");

  eval ("insmod", "ath_ahb", "autocreate=none");

  eval ("ifconfig", "wifi0", "up");
  eval ("ifconfig", "wifi1", "up");

  eval ("insmod", "ipv6");

  /* Set a sane date */
  stime (&tm);
  nvram_set ("wl0_ifname", "ath0");

  return 0;
  cprintf ("done\n");
}
