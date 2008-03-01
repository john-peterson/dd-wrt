/*
 * sysinit.c
 *
 * Copyright (C) 2007 Sebastian Gottschall <gottschall@dd-wrt.com>
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
#include <dirent.h>

#include <epivers.h>
#include <bcmnvram.h>
#include <mtd.h>
#include <shutils.h>
#include <rc.h>
#include <netconf.h>
#include <nvparse.h>
#include <bcmdevs.h>

#include <wlutils.h>
#include <utils.h>
#include <cyutils.h>
#include <code_pattern.h>
#include <cy_conf.h>
//#include <mkfiles.h>
#include <typedefs.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <wlutils.h>
#include <cy_conf.h>
#include <cymac.h>
//#include <ledcontrol.h>


#define WL_IOCTL(name, cmd, buf, len) (ret = wl_ioctl((name), (cmd), (buf), (len)))

#define TXPWR_MAX 251
#define TXPWR_DEFAULT 28

void start_restore_defaults (void);
static void rc_signal (int sig);
extern void start_overclocking (void);
extern int check_cfe_nv (void);
extern int check_pmon_nv (void);
static void unset_nvram (void);
int start_nvram (void);

extern struct nvram_tuple srouter_defaults[];



int
endswith (char *str, char *cmp)
{
  int cmp_len, str_len, i;
  cmp_len = strlen (cmp);
  str_len = strlen (str);
  if (cmp_len > str_len)
    return (0);
  for (i = 0; i < cmp_len; i++)
    {
      if (str[(str_len - 1) - i] != cmp[(cmp_len - 1) - i])
	return (0);
    }
  return (1);
}




#ifdef HAVE_MACBIND
#include "../../../opt/mac.h"
#endif
void
runStartup (char *folder, char *extension)
{
  struct dirent *entry;
  DIR *directory;
  unsigned char buf[128];
  directory = opendir (folder);
  if (directory == NULL)
    {
      return;
    }
//list all files in this directory 
  while ((entry = readdir (directory)) != NULL)
    {
      if (endswith (entry->d_name, extension))
	{
	  sprintf (buf, "%s/%s 2>&1 > /dev/null&\n", folder, entry->d_name);
	  //execute script     
	  system2 (buf);
	}
    }
  closedir (directory);
}

/* SeG dd-wrt addition for module startup scripts */
int
start_modules (void)
{
  runStartup ("/etc/config", ".startup");

#ifdef HAVE_RB500
  runStartup ("/usr/local/etc/config", ".startup");	//if available
#elif HAVE_X86
  runStartup ("/usr/local/etc/config", ".startup");	//if available
#else
  runStartup ("/jffs/etc/config", ".startup");	//if available
  runStartup ("/mmc/etc/config", ".startup");	//if available
#endif
  return 0;
}

int
start_wanup (void)
{
  runStartup ("/etc/config", ".wanup");
#ifdef HAVE_RB500
  runStartup ("/usr/local/etc/config", ".wanup");	//if available
#elif HAVE_X86
  runStartup ("/usr/local/etc/config", ".wanup");	//if available
#else
  runStartup ("/jffs/etc/config", ".wanup");	//if available
  runStartup ("/mmc/etc/config", ".wanup");	//if available
  runStartup ("/tmp/etc/config", ".wanup");	//if available
#endif
  return 0;
}




int
start_create_rc_startup (void)
{
  create_rc_file (RC_STARTUP);
  return 0;
}

int
start_create_rc_shutdown (void)
{
  create_rc_file (RC_SHUTDOWN);
  return 0;
}


int
create_rc_file (char *name)
{
  FILE *fp;
  char *p = nvram_safe_get (name);
  char tmp_file[100] = { 0 };

  if ((void *) 0 == name || 0 == p[0])
    return -1;

  snprintf (tmp_file, 100, "/tmp/.%s", name);
  unlink (tmp_file);

  fp = fopen (tmp_file, "w");
  if (fp)
    {
      // filter Windows <cr>ud
      while (*p)
	{
	  if (*p != 0x0d)
	    fprintf (fp, "%c", *p);
	  p++;
	}
    }
  fclose (fp);
  chmod (tmp_file, 0700);

  return 0;
}

static void
ses_cleanup (void)
{
  /* well known event to cleanly initialize state machine */
  nvram_set ("ses_event", "2");

  /* Delete lethal dynamically generated variables */
  nvram_unset ("ses_bridge_disable");
}

static void
ses_restore_defaults (void)
{
  char tmp[100], prefix[] = "wlXXXXXXXXXX_ses_";
  int i;

  /* Delete dynamically generated variables */
  for (i = 0; i < MAX_NVPARSE; i++)
    {
      sprintf (prefix, "wl%d_ses_", i);
      nvram_unset (strcat_r (prefix, "ssid", tmp));
      nvram_unset (strcat_r (prefix, "closed", tmp));
      nvram_unset (strcat_r (prefix, "wpa_psk", tmp));
      nvram_unset (strcat_r (prefix, "auth", tmp));
      nvram_unset (strcat_r (prefix, "wep", tmp));
      nvram_unset (strcat_r (prefix, "auth_mode", tmp));
      nvram_unset (strcat_r (prefix, "crypto", tmp));
      nvram_unset (strcat_r (prefix, "akm", tmp));
    }
}

void
start_restore_defaults (void)
{

#ifdef HAVE_RB500
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames",
     "eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 ath0 ath1 ath2 ath3 ath4 ath5",
     0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#elif HAVE_GEMTEK
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth1 ath0", 0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#elif HAVE_GATEWORX
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "ixp0 ath0 ath1 ath2 ath3",
     0},
    {"wan_ifname2", "ixp1", 0},
    {"wan_ifname", "ixp1", 0},
    {"wan_ifnames", "ixp1", 0},
    {0, 0, 0}
  };
#elif HAVE_X86
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
#ifdef HAVE_NOWIFI
    {"lan_ifnames", "eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10", 0},
#else
#ifdef HAVE_GW700
    {"lan_ifnames",
     "eth0 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 ath0 ath1 ath2 ath3 ath5 ath6 ath7 ath8",
     0},
#else
    {"lan_ifnames",
     "eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 ath0 ath1 ath2 ath3 ath5 ath6 ath7 ath8",
     0},
#endif
#endif
#ifdef HAVE_GW700
    {"wan_ifname", "eth1", 0},
    {"wan_ifname2", "eth1", 0},
    {"wan_ifnames", "eth1", 0},
#else
    {"wan_ifname", "eth0", 0},
    {"wan_ifname2", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
#endif
    {0, 0, 0}
  };
#elif HAVE_XSCALE
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames",
     "ixp0.1 ixp0.2 ath0 ath1",
     0},
    {"wan_ifname", "ixp1", 0},
    {"wan_ifnames", "ixp1", 0},
    {0, 0, 0}
  };
#elif HAVE_MAGICBOX
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth1 ath0",
     0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#elif HAVE_FONERA
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "vlan0 ath0", 0},
    {"wan_ifname", "", 0},
    {"wan_ifnames", "eth0 vlan1", 0},
    {0, 0, 0}
  };
#elif HAVE_LS2
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "vlan1 ath0", 0},
    {"wan_ifname", "vlan2", 0},
    {"wan_ifnames", "vlan2", 0},
    {0, 0, 0}
  };
#elif HAVE_LS5
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "ath0", 0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#elif HAVE_WHRAG108
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth0 ath0 ath1", 0},
    {"wan_ifname", "eth1", 0},
    {"wan_ifnames", "eth1", 0},
    {0, 0, 0}
  };
#elif HAVE_PB42
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth1 ath0 ath1", 0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#elif HAVE_TW6600
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "ath0 ath1", 0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#elif HAVE_CA8
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "ath0", 0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };
#else
  struct nvram_tuple generic[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth0 eth2 eth3 eth4", 0},
    {"wan_ifname", "eth1", 0},
    {"wan_ifnames", "eth1", 0},
    {0, 0, 0}
  };
  struct nvram_tuple vlan[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "vlan0 eth1 eth2 eth3", 0},
    {"wan_ifname", "vlan1", 0},
    {"wan_ifnames", "vlan1", 0},
    {0, 0, 0}
  };

  struct nvram_tuple wrt350vlan[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "vlan1 eth0", 0},
    {"wan_ifname", "vlan2", 0},
    {"wan_ifnames", "vlan2", 0},
    {0, 0, 0}
  };

  struct nvram_tuple wrt600vlan[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "vlan0 eth0 eth1", 0},
    {"wan_ifname", "vlan2", 0},
    {"wan_ifnames", "vlan2", 0},
    {0, 0, 0}
  };

  struct nvram_tuple wzr144nhvlan[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "vlan2 eth0", 0},
    {"wan_ifname", "vlan1", 0},
    {"wan_ifnames", "vlan1", 0},
    {0, 0, 0}
  };

  struct nvram_tuple generic_2[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth1 eth2", 0},
    {"wan_ifname", "eth0", 0},
    {"wan_ifnames", "eth0", 0},
    {0, 0, 0}
  };

  struct nvram_tuple generic_3[] = {
    {"lan_ifname", "br0", 0},
    {"lan_ifnames", "eth0 eth1", 0},
    {"wan_ifname", "eth2", 0},
    {"wan_ifnames", "eth2", 0},
    {0, 0, 0}
  };
#endif

  struct nvram_tuple *linux_overrides;
  struct nvram_tuple *t, *u;
  int restore_defaults = 0;
//      uint boardflags;

  /* Restore defaults if told to.

     Note: an intentional side effect is that when
     upgrading from a firmware without the
     sv_restore_defaults var, defaults will
     also be restored.
   */
  char *et0mac = nvram_safe_get ("et0macaddr");
  char *et1mac = nvram_safe_get ("et1macaddr");
//  unsigned char mac[20];
//  if (getRouterBrand () == ROUTER_BUFFALO_WZRG144NH)
//    {
//      if (nvram_get ("il0macaddr") == NULL)
//      {
//        strcpy (mac, et0mac);
//        MAC_ADD (mac);
//        nvram_set ("il0macaddr", mac);
//      }
//    }

#ifdef HAVE_RB500
  linux_overrides = generic;
  int brand = getRouterBrand ();
#elif HAVE_XSCALE
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_X86
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_MAGICBOX
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_GATEWORX
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_FONERA
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_LS2
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_LS5
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_WHRAG108
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_TW6600
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_PB42
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_CA8
  linux_overrides = generic;
  int brand = getRouterBrand ();
  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
      restore_defaults = 1;
    }
#elif HAVE_GEMTEK
  linux_overrides = generic;
  int brand = getRouterBrand ();
#else
  int brand = getRouterBrand ();

  if (nvram_invmatch ("sv_restore_defaults", "0"))	// || nvram_invmatch("os_name", "linux"))
    {
//      nvram_unset("sv_restore_defaults");
      restore_defaults = 1;
    }
  if (nvram_match ("product_name", "INSPECTION"))
    {
      nvram_unset ("product_name");
      restore_defaults = 1;
    }
  if (nvram_get ("router_name") == NULL)
    restore_defaults = 1;

  if (restore_defaults)
  {
    cprintf ("Restoring defaults...\n");
 /* these unsets are important for routers where we can't erase nvram and only software restore defaults */
    nvram_unset ("wan_to_lan");
    nvram_unset ("wl_vifs");
    nvram_unset ("wl0_vifs");
    nvram_unset ("fullswitch_set");
  }

//    }

/* Delete dynamically generated variables */
  /* Choose default lan/wan i/f list. */
  char *ds;
  switch (brand)
    {
#ifndef HAVE_BUFFALO
    case ROUTER_WRTSL54GS:
    case ROUTER_WRT150N:
    case ROUTER_WRT300N:
    case ROUTER_NETGEAR_WNR834B:
    case ROUTER_ASUS_WL500G:
    case ROUTER_ASUS_WL500W:
#endif
    case ROUTER_BUFFALO_WZRG300N:
    case ROUTER_BUFFALO_WLAH_G54:
    case ROUTER_BUFFALO_WAPM_HP_AM54G54:
    case ROUTER_BUFFALO_WZRRSG54:
      linux_overrides = generic;
      break;
#ifndef HAVE_BUFFALO
    case ROUTER_ASUS_WL500GD:
    case ROUTER_ASUS_WL550GE:
      linux_overrides = vlan;
      break;
    case ROUTER_WRT350N:
      linux_overrides = wrt350vlan;
      break;
    case ROUTER_WRT600N:
      linux_overrides = wrt600vlan;
      break;
#endif
    case ROUTER_BUFFALO_WZRG144NH:
      linux_overrides = wzr144nhvlan;
      break;
#ifndef HAVE_BUFFALO
    case ROUTER_MOTOROLA_WE800G:
    case ROUTER_WAP54G_V1:
    case ROUTER_SITECOM_WL105B:
#endif
    case ROUTER_BUFFALO_WLI2_TX1_G54:
    case ROUTER_BUFFALO_WLAG54C:
      linux_overrides = generic_2;
      break;
#ifndef HAVE_BUFFALO
    case ROUTER_WAP54G_V2:
    case ROUTER_VIEWSONIC_WAPBR_100:
    case ROUTER_USR_5430:
    case ROUTER_BELKIN_F5D7230_V2000:
    case ROUTER_NETGEAR_WG602_V3:
#endif
    case ROUTER_BUFFALO_WLA2G54C:
    case ROUTER_BUFFALO_WLI_TX4_G54HP:
      linux_overrides = generic_3;
      break;
#ifndef HAVE_BUFFALO
    case ROUTER_RT480W:
    case ROUTER_RT210W:
#endif
    case ROUTER_BRCM4702_GENERIC:
      ds = nvram_safe_get ("dhcp_start");
      if (ds != NULL && strlen (ds) > 3)
	{
	  fprintf (stderr, "cleaning nvram variables\n");
	  for (t = srouter_defaults; t->name; t++)
	    {
	      nvram_unset (t->name);
	    }
	  restore_defaults = 1;
	}

/*      ds = nvram_safe_get ("http_passwd");
      if (ds == NULL || strlen (ds) == 0)	//fix for empty default password
	{
	  nvram_set ("http_passwd", "admin");
	}
      ds = nvram_safe_get ("language");
      if (ds != NULL && strlen (ds) < 3)
	{
	  nvram_set ("language", "english");
	}*/
      // fall through  
    default:
      if (check_vlan_support ())
	linux_overrides = vlan;
      else
	linux_overrides = generic;
      break;
    }
#endif
/*  int i;
 *  for (i=0;i<4;i++)
 * 		nvram_set(linux_overrides[i].name,linux_overrides[i].value);
 */

  /* Restore defaults */
#ifdef HAVE_FON
  int reset = 0;
  char *rev = nvram_safe_get ("fon_revision");
  if (rev == NULL || strlen (rev) == 0)
    reset = 1;
  if (strlen (rev) > 0)
    {
      int n = atoi (rev);
      if (atoi (srouter_defaults[0].value) != n)
	reset = 1;
    }
  if (reset)
    {
      for (t = srouter_defaults; t->name; t++)
	{
	  for (u = linux_overrides; u && u->name; u++)
	    {
	      if (!strcmp (t->name, u->name))
		{
		  nvram_set (u->name, u->value);
		  break;
		}
	    }
	  if (!u || !u->name)
	    nvram_set (t->name, t->value);
	}
    }
#endif
#ifdef HAVE_GATEWORX
  if (restore_defaults)
    {
      eval ("erase", "nvram");
    }
#elif HAVE_XSCALE
  if (restore_defaults)
    eval ("rm", "-f", "/etc/nvram/*");	// delete nvram database
#endif
#ifdef HAVE_MAGICBOX
  if (restore_defaults)
    {
      eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
      eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
      eval ("erase", "nvram");
    }
#endif
#ifdef HAVE_FONERA
  if (restore_defaults)
    {
      eval ("erase", "nvram");
    }
#endif
#ifdef HAVE_LS2
  if (restore_defaults)
    {
      eval ("erase", "nvram");
    }
#endif
#ifdef HAVE_LS5
  if (restore_defaults)
    {
      eval ("erase", "nvram");
    }
#endif
#ifdef HAVE_WHRAG108
  if (restore_defaults)
    {
      eval ("erase", "nvram");
    }
#endif
#ifdef HAVE_TW6600
  if (restore_defaults)
    {
      eval ("erase", "nvram");
    }
#endif
  int nvcnt = 0;
//  if (!nvram_match("default_init","1"))
  {
    for (t = srouter_defaults; t->name; t++)
      {
	if (restore_defaults || !nvram_get (t->name))
	  {
	    for (u = linux_overrides; u && u->name; u++)
	      {
		if (!strcmp (t->name, u->name))
		  {
		    nvcnt++;
		    nvram_set (u->name, u->value);
		    break;
		  }
	      }
	    if (!u || !u->name)
	      {
		nvcnt++;
		nvram_set (t->name, t->value);
	      }
	  }
      }
  }
#ifndef HAVE_FON
  if (restore_defaults)
    {
	  switch (brand)
	  {
		case ROUTER_ASUS_WL520G:
		  nvram_set ("vlan0ports", "0 1 2 3 5*");
		  nvram_set ("vlan1ports", "4 5");
		  break;
		case ROUTER_LINKSYS_WTR54GS:
		  nvram_set ("vlan0ports", "0 5*");
		  nvram_set ("vlan1ports", "1 5");
		  break;
		case ROUTER_ASUS_WL550GE:
		  nvram_set ("vlan0ports", "1 2 3 4 5*");
		  nvram_set ("vlan1ports", "0 5");
		  break;
		case ROUTER_MOTOROLA:
		case ROUTER_WRT54G_V8:
		  nvram_set ("vlan0ports", "3 2 1 0 5*");
		  nvram_set ("vlan1ports", "4 5");
		  break;
		case ROUTER_LINKSYS_WRH54G:
		case ROUTER_ASUS_WL500GD:
		  nvram_set ("vlan0ports", "4 3 2 1 5*");
		  nvram_set ("vlan1ports", "0 5");
		  break;
	  }
	if (nvram_match ("bootnv_ver", "4")
	  || nvram_match ("boardnum", "WAP54GV3_8M_0614"))
	{
	  nvram_set ("vlan0ports", "3 2 1 0 5*");
	  nvram_set ("vlan1ports", "4 5");
    }
#ifdef HAVE_SPUTNIK
      nvram_set ("lan_ipaddr", "192.168.180.1");
#elif HAVE_BUFFALO
      nvram_set ("lan_ipaddr", "192.168.11.1");
#else
      nvram_set ("lan_ipaddr", "192.168.1.1");
#endif
    }
#else
  if (restore_defaults)
    {
      nvram_set ("lan_ipaddr", "192.168.10.1");
    }
#endif
#ifdef HAVE_SKYTRON
  if (restore_defaults)
    {
      nvram_set ("lan_ipaddr", "192.168.0.1");
    }
#endif
  if (brand == ROUTER_WRT600N)
    {

      if (!nvram_get ("vlan0ports") || nvram_match ("vlan0ports", ""))
	{
	  nvram_set ("vlan0ports", "1 2 3 4 8*");
	}
	  if (!nvram_get ("vlan2ports") || nvram_match ("vlan2ports", ""))
    {
      nvram_set ("vlan2ports", "0 8");
    }
    
    }
  else if (brand == ROUTER_WRT350N)
    {

      if (!nvram_get ("vlan1ports") || nvram_match ("vlan1ports", ""))
	{
	  nvram_set ("vlan1ports", "1 2 3 4 8*");
	}
	  if (!nvram_get ("vlan2ports") || nvram_match ("vlan2ports", ""))
    {
      nvram_set ("vlan2ports", "0 8");
    }
    
    }
  else if (brand == ROUTER_BUFFALO_WZRG144NH)

    {
      if (!nvram_get ("vlan1ports") || nvram_match ("vlan1ports", ""))
	{
	  nvram_set ("vlan1ports", "4 8");
	}
	  if (!nvram_get ("vlan2ports") || nvram_match ("vlan2ports", ""))
    {
      nvram_set ("vlan2ports", "0 1 2 3 8");
    }

    }
  else
    {
      if (!nvram_get ("vlan0hwname") || nvram_match ("vlan0hwname", ""))
	nvram_set ("vlan0hwname", "et0");
      if (!nvram_get ("vlan1hwname") || nvram_match ("vlan1hwname", ""))
	nvram_set ("vlan1hwname", "et0");

      switch (brand)
	{
	case ROUTER_MOTOROLA:
	case ROUTER_MOTOROLA_V1:
	case ROUTER_MOTOROLA_WE800G:
	case ROUTER_RT210W:
	  if (et0mac != NULL)
	    nvram_set ("et0macaddr", et0mac);
	  if (et1mac != NULL)
	    nvram_set ("et1macaddr", et1mac);
	  break;
	}

      if (!nvram_get ("vlan0ports") || nvram_match ("vlan0ports", ""))
	{
	  switch (brand)
	    {
	    case ROUTER_LINKSYS_WTR54GS:
	      nvram_set ("vlan0ports", "0 5*");
	      break;
	    case ROUTER_ASUS_WL500G_PRE:
	      nvram_set ("vlan0ports", "1 2 3 4 5*");
	      break;
	    case ROUTER_MOTOROLA:
	    case ROUTER_WRT54G_V8:
	      nvram_set ("vlan0ports", "3 2 1 0 5*");
	      break;
	    case ROUTER_LINKSYS_WRT55AG:
	    case ROUTER_RT480W:
	    case ROUTER_DELL_TRUEMOBILE_2300_V2:
	    case ROUTER_ASUS_WL520G:
	      nvram_set ("vlan0ports", "0 1 2 3 5*");
	      break;
	    case ROUTER_LINKSYS_WRH54G:
		case ROUTER_ASUS_WL500GD:
	      nvram_set ("vlan0ports", "4 3 2 1 5*");
	      break;
	    default:
	      if (nvram_match ("bootnv_ver", "4")
		  || nvram_match ("boardnum", "WAP54GV3_8M_0614"))
		nvram_set ("vlan0ports", "3 2 1 0 5*");
	      else
		nvram_set ("vlan0ports", "1 2 3 4 5*");
	      break;
	    }
	}

	  if (!nvram_get ("vlan1ports") || nvram_match ("vlan1ports", ""))
	    {
	      switch (brand)
		{
		case ROUTER_LINKSYS_WTR54GS:
		  nvram_set ("vlan1ports", "1 5");
		  break;
		case ROUTER_ASUS_WL500G_PRE:
		case ROUTER_LINKSYS_WRH54G:
		  nvram_set ("vlan1ports", "0 5");
		  break;
		case ROUTER_MOTOROLA:
		case ROUTER_WRT54G_V8:
		case ROUTER_LINKSYS_WRT55AG:
		case ROUTER_RT480W:
		case ROUTER_DELL_TRUEMOBILE_2300_V2:
		case ROUTER_ASUS_WL520G:
		  nvram_set ("vlan1ports", "4 5");
		  break;
		default:
		  if (nvram_match ("bootnv_ver", "4")
		      || nvram_match ("boardnum", "WAP54GV3_8M_0614"))
		    nvram_set ("vlan1ports", "4 5");
		  else
		    nvram_set ("vlan1ports", "0 5");
		  break;
		}
	    }

    }

  if (brand == ROUTER_WRT54G || brand == ROUTER_WRT54G1X
      || brand == ROUTER_LINKSYS_WRT55AG)
    {
      if (!nvram_get ("aa0"))
	nvram_set ("aa0", "3");
      if (!nvram_get ("ag0"))
	nvram_set ("ag0", "255");
      if (!nvram_get ("gpio2"))
	nvram_set ("gpio2", "adm_eecs");
      if (!nvram_get ("gpio3"))
	nvram_set ("gpio3", "adm_eesk");
      if (!nvram_get ("gpio5"))
	nvram_set ("gpio5", "adm_eedi");
      if (!nvram_get ("gpio6"))
	nvram_set ("gpio6", "adm_rc");
      if (!nvram_get ("boardrev") || nvram_match ("boardrev", ""))
	nvram_set ("boardrev", "0x10");
      if (!nvram_get ("boardflags") || nvram_match ("boardflags", ""))
	nvram_set ("boardflags", "0x0388");
      if (!nvram_get ("boardflags2"))
	nvram_set ("boardflags2", "0");
    }
  /* Always set OS defaults */
  nvram_set ("os_name", "linux");
  nvram_set ("os_version", EPI_VERSION_STR);

#ifdef HAVE_DDLAN
  nvram_unset ("cur_rssi");
  nvram_unset ("cur_noise");
  nvram_unset ("cur_bssid");
  nvram_unset ("cur_snr");
  nvram_set ("cur_state",
	     "<span style=\"background-color: rgb(255, 0, 0);\">Nicht Verbunden</span>");
#endif
#ifdef HAVE_SPUTNIK_APD
  /* Added for Sputnik Agent */
  nvram_unset ("sputnik_mjid");
  nvram_unset ("sputnik_rereg");
#endif

  if (nvram_get ("overclocking") == NULL)
    nvram_set ("overclocking", nvram_safe_get ("clkfreq"));
  cprintf ("start overclocking\n");
  start_overclocking ();
  cprintf ("done()");
  if (nvram_get ("http_username") != NULL)
    {
      if (nvram_match ("http_username", ""))
	{
#ifdef HAVE_POWERNOC
	  nvram_set ("http_username", "bJz7PcC1rCRJQ");	//admin
#else
	  nvram_set ("http_username", "bJ/GddyoJuiU2");	//root
#endif
	}
    }

  cprintf ("check CFE nv\n");
  if (check_now_boot () == PMON_BOOT)
    check_pmon_nv ();
  else
    check_cfe_nv ();
  cprintf ("restore defaults\n");

  /* Commit values */
  if (restore_defaults)
    {
      int i;
      unset_nvram ();
      nvram_commit ();
      cprintf ("done\n");
      for (i = 0; i < MAX_NVPARSE; i++)
	{
	  del_wds_wsec (0, i);
	  del_wds_wsec (1, i);
	}
    }
}






/* States */
enum
{
  RESTART,
  STOP,
  START,
  TIMER,
  USER,
  IDLE,
};
static int state = START;
static int signalled = -1;

/* Signal handling */
static void
rc_signal (int sig)
{
  if (state == IDLE)
    {
      if (sig == SIGHUP)
	{
	  printf ("signalling RESTART\n");
	  signalled = RESTART;
	}
      else if (sig == SIGUSR2)
	{
	  printf ("signalling START\n");
	  signalled = START;
	}
      else if (sig == SIGINT)
	{
	  printf ("signalling STOP\n");
	  signalled = STOP;
	}
      else if (sig == SIGALRM)
	{
	  printf ("signalling TIMER\n");
	  signalled = TIMER;
	}
      else if (sig == SIGUSR1)
	{			// Receive from WEB
	  printf ("signalling USER1\n");
	  signalled = USER;
	}

    }
}

/* Timer procedure */
int
do_timer (void)
{
  //do_ntp();
  return 0;
}


#define CONVERT_NV(old, new) \
	if(nvram_get(old)) \
		nvram_set(new, nvram_safe_get(old));

int
start_nvram (void)
{
  int i = 0;

  /* broadcom 3.11.48.7 change some nvram name */
#ifdef HAVE_MSSID
  CONVERT_NV ("d11g_channel", "wl_channel");
#else
  CONVERT_NV ("d11g_channel", "wl0_channel");
#endif
  CONVERT_NV ("d11g_rateset", "wl_rateset");
  CONVERT_NV ("d11g_rts", "wl_rts");
  CONVERT_NV ("d11g_bcn", "wl_bcn");
  CONVERT_NV ("d11g_mode", "wl_gmode");
  CONVERT_NV ("d11g_rate", "wl_rate");
  CONVERT_NV ("d11g_frag", "wl_frag");
  CONVERT_NV ("d11g_dtim", "wl_dtim");

  nvram_unset ("wl0_hwaddr");	// When disbale wireless, we must get null wireless mac */

  nvram_set ("wan_get_dns", "");
  nvram_set ("filter_id", "1");
  nvram_set ("wl_active_add_mac", "0");
  nvram_set ("ddns_change", "");
  nvram_unset ("action_service");
  nvram_set ("wan_get_domain", "");


  //if(!nvram_get("wl_macmode1")){
  //      if(nvram_match("wl_macmode","disabled"))
  //              nvram_set("wl_macmode1","disabled");
  //      else
  //              nvram_set("wl_macmode1","other");
  //}
  if (nvram_match ("wl_gmode", "5"))	// Mixed mode had been changed to 5
    nvram_set ("wl_gmode", "1");

  if (nvram_match ("wl_gmode", "4"))	// G-ONLY mode had been changed to 2, after 1.40.1 for WiFi G certication
    nvram_set ("wl_gmode", "2");

//      nvram_set("wl_country","Worldwide");    // The country always Worldwide

#ifndef AOL_SUPPORT
  nvram_set ("aol_block_traffic", "0");
  nvram_set ("aol_block_traffic1", "0");
  nvram_set ("aol_block_traffic2", "0");
#endif
  nvram_set ("ping_ip", "");
  nvram_set ("ping_times", "");
//  nvram_set ("traceroute_ip", "");

  nvram_set ("filter_port", "");	// The name have been disbaled from 1.41.3

#ifdef HAVE_UPNP
  if ((nvram_match ("restore_defaults", "1"))
      || (nvram_match ("upnpcas", "1")))
    {
      nvram_set ("upnp_clear", "1");
    }
  else
    {
      char s[32];
      char *nv;
      for (i = 0; i < MAX_NVPARSE; ++i)
	{
	  sprintf (s, "forward_port%d", i);
	  if ((nv = nvram_get (s)) != NULL)
	    {
	      if (strstr (nv, "msmsgs"))
		nvram_unset (s);
	    }
	}
    }
  nvram_set ("upnp_wan_proto", "");
#endif

  /* The tkip and aes already are changed to wl_crypto from v3.63.3.0 */
  if (nvram_match ("wl_wep", "tkip"))
    {
      nvram_set ("wl_crypto", "tkip");
    }
  else if (nvram_match ("wl_wep", "aes"))
    {
      nvram_set ("wl_crypto", "aes");
    }
  else if (nvram_match ("wl_wep", "tkip+aes"))
    {
      nvram_set ("wl_crypto", "tkip+aes");
    }

  if (nvram_match ("wl_wep", "restricted"))
    nvram_set ("wl_wep", "enabled");	// the nas need this value, the "restricted" is no longer need. (20040624 by honor)


#ifdef HAVE_SET_BOOT
  if (!nvram_match ("boot_wait_web", "0"))
    nvram_set ("boot_wait_web", "1");
#endif

#ifdef HAVE_SSHD
  if (!nvram_match ("sshd_web", "0"))
    nvram_set ("sshd_web", "1");
#endif

#ifndef HAVE_MSSID
  nvram_set ("wl0_country_code", "JP");
  nvram_set ("wl0_country", "Japan");
#endif
#ifndef HAVE_BUFFALO
  if (check_hw_type () == BCM5352E_CHIP)
    {
      nvram_set ("opo", "0");	// OFDM power reducement in quarter dbm (2 dbm in this case)
      nvram_set ("ag0", "0");	// Antenna Gain definition in dbm
    }
#endif

  if (nvram_match ("svqos_port1bw", "full"))
    nvram_set ("svqos_port1bw", "FULL");
  if (nvram_match ("svqos_port2bw", "full"))
    nvram_set ("svqos_port2bw", "FULL");
  if (nvram_match ("svqos_port3bw", "full"))
    nvram_set ("svqos_port3bw", "FULL");
  if (nvram_match ("svqos_port4bw", "full"))
    nvram_set ("svqos_port4bw", "FULL");
  //dirty fix for WBR2 units


  if (strlen (nvram_safe_get ("http_username")) == 0)
    {
      nvram_set ("http_username", zencrypt ("root"));
      nvram_set ("http_passwd", zencrypt ("admin"));
    }

  //clean old filter_servicesX to free nvram
  nvram_unset ("filter_services0");
  nvram_unset ("filter_services1");
  nvram_unset ("filter_services2");
  nvram_unset ("filter_services3");
  nvram_unset ("filter_services4");
  nvram_unset ("filter_services5");
  nvram_unset ("filter_services6");
  nvram_unset ("filter_services7");

  nvram_unset ("vdsl_state");	//important (this value should never be commited, but if this will fix the vlan7 issue)

#ifdef DIST
  nvram_set ("dist_type", DIST);
#endif

  {

#ifdef DIST
#ifndef HAVE_TW6600
#ifdef HAVE_MICRO
//if dist_type micro, check styles, and force to elegant if needed
    char *style = nvram_safe_get ("router_style");
    if (!strstr ("blue cyan elegant green orange red yellow", style))
      {
	nvram_set ("router_style", "elegant");
      }
#endif
#endif
#endif
  }

#ifdef HAVE_WIVIZ
  if (!strlen (nvram_safe_get ("hopseq"))
      || !strlen (nvram_safe_get ("hopdwell")))
    {
#ifdef HAVE_MSSID
      char *channel = nvram_safe_get ("wl0_channel");
#else
      char *channel = nvram_safe_get ("wl_channel");
#endif

      nvram_set ("hopdwell", "1000");
      nvram_set ("hopseq", channel);
    }
#endif


  return 0;
}

static void
unset_nvram (void)
{
#ifndef MPPPOE_SUPPORT
  nvram_safe_unset ("ppp_username_1");
  nvram_safe_unset ("ppp_passwd_1");
  nvram_safe_unset ("ppp_idletime_1");
  nvram_safe_unset ("ppp_demand_1");
  nvram_safe_unset ("ppp_redialperiod_1");
  nvram_safe_unset ("ppp_service_1");
  nvram_safe_unset ("mpppoe_enable");
  nvram_safe_unset ("mpppoe_dname");
#endif
#ifndef HAVE_HTTPS
  nvram_safe_unset ("remote_mgt_https");
#endif
#ifndef HSIAB_SUPPORT
  nvram_safe_unset ("hsiab_mode");
  nvram_safe_unset ("hsiab_provider");
  nvram_safe_unset ("hsiab_device_id");
  nvram_safe_unset ("hsiab_device_password");
  nvram_safe_unset ("hsiab_admin_url");
  nvram_safe_unset ("hsiab_registered");
  nvram_safe_unset ("hsiab_configured");
  nvram_safe_unset ("hsiab_register_ops");
  nvram_safe_unset ("hsiab_session_ops");
  nvram_safe_unset ("hsiab_config_ops");
  nvram_safe_unset ("hsiab_manual_reg_ops");
  nvram_safe_unset ("hsiab_proxy_host");
  nvram_safe_unset ("hsiab_proxy_port");
  nvram_safe_unset ("hsiab_conf_time");
  nvram_safe_unset ("hsiab_stats_time");
  nvram_safe_unset ("hsiab_session_time");
  nvram_safe_unset ("hsiab_sync");
  nvram_safe_unset ("hsiab_config");
#endif

#ifndef HEARTBEAT_SUPPORT
  nvram_safe_unset ("hb_server_ip");
  nvram_safe_unset ("hb_server_domain");
#endif

#ifndef PARENTAL_CONTROL_SUPPORT
  nvram_safe_unset ("artemis_enable");
  nvram_safe_unset ("artemis_SVCGLOB");
  nvram_safe_unset ("artemis_HB_DB");
  nvram_safe_unset ("artemis_provisioned");
#endif

#ifndef WL_STA_SUPPORT
//        nvram_safe_unset("wl_ap_ssid");
//        nvram_safe_unset("wl_ap_ip");
#endif


}
