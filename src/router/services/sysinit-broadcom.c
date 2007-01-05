/*
 * sysinit-broadcom.c
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

#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include <bcmdevs.h>

static void
check_brcm_cpu_type (void)
{
  FILE *fcpu;
  char cpu_type[20];
  char type2[30];

  fcpu = fopen ("/proc/cpuinfo", "r");

  if (fcpu == NULL)
    fprintf (stderr, "Open /proc/cpuinfo fail...0\n");
  else
    {
      char buf[500];
      fgets (buf, 500, fcpu);
      fscanf (fcpu, "%s %s %s %s %s", buf, buf, buf, cpu_type, type2);
      if (!strcmp (type2, "BCM4704"))
	{
	  nvram_set ("cpu_type", cpu_type);
	  fclose (fcpu);
	  return;
	}
      if (!strcmp (type2, "BCM4712"))
	{
	  nvram_set ("cpu_type", cpu_type);
	  fclose (fcpu);
	  return;
	}
      if (!strcmp (type2, "BCM4702"))
	{
	  nvram_set ("cpu_type", cpu_type);
	  fclose (fcpu);
	  return;
	}
      if (!strcmp (type2, "BCM3302"))
	{
	  nvram_set ("cpu_type", cpu_type);
	  fclose (fcpu);
	  return;
	}
      fgets (buf, 500, fcpu);
      fscanf (fcpu, "%s %s %s %s %s", buf, buf, buf, cpu_type, type2);
      //fprintf(stderr, "cpu_type : %s\n", cpu_type); 
      fclose (fcpu);
      if (!strcmp (cpu_type, "BCM4710") || !strcmp (cpu_type, "BCM4702"))
	{
	  fprintf (stderr, "We got BCM4702 board...\n");
	  nvram_set ("cpu_type", cpu_type);
	}
      else if (!strcmp (cpu_type, "BCM3302") || !strcmp (cpu_type, "BCM4712"))
	{
	  fprintf (stderr, "We got BCM4712 board...\n");
	  nvram_set ("cpu_type", cpu_type);
	}
      else
	{
	  fprintf (stderr, "We got unknown board...\n");
	  nvram_set ("cpu_type", cpu_type);
	}
    }



}

static void
enableAfterBurner (void)
{

  int boardflags;
  int brand = getRouterBrand ();
  switch (brand)
    {
    case ROUTER_LINKSYS_WRT55AG:
    case ROUTER_WRT300N:
    case ROUTER_WRT350N:
    case ROUTER_MOTOROLA_V1:
    case ROUTER_BUFFALO_WZRRSG54:
      return;
      break;
    default:
      if (nvram_get ("boardflags") == NULL)
	return;
      boardflags = strtoul (nvram_safe_get ("boardflags"), NULL, 0);
      fprintf (stderr, "boardflags are 0x0%X\n", boardflags);
      if (!(boardflags & BFL_AFTERBURNER))
	{
	  boardflags |= BFL_AFTERBURNER;
	  char ab[100];
	  fprintf (stderr, "enable Afterburner....\n");
	  sprintf (ab, "0x0%X", boardflags);
	  nvram_set ("boardflags", ab);
	  nvram_set ("need_commit", "1");
	}
    }
}

char wanifname[8], wlifname[8];
#define BCM4712_CPUTYPE "0x4712"

static void
setup_4712 (void)
{
  uint boardflags = strtoul (nvram_safe_get ("boardflags"), NULL, 0);
  if (nvram_match ("cpu_type", BCM4712_CPUTYPE) ||
      nvram_match ("cpu_type", "BCM3302") ||
      nvram_match ("cpu_type", "BCM4712"))
    {
      if (boardflags & BFL_ENETVLAN)
	{
	  cprintf ("setup_4712(): Enable VLAN\n");
	  //nvram_set("setup_4712","1");
	  strcpy (wanifname, "vlan1");
	  strcpy (wlifname, "eth1");
	  if (!strcmp (nvram_safe_get ("wan_ifname"), "eth1"))
	    {
	      //nvram_set("setup_4712","1-1");
	      nvram_set ("wan_ifname", "vlan1");
	      nvram_set ("wan_ifnames", "vlan1");
	    }
	  if (!strstr (nvram_safe_get ("lan_ifnames"), "vlan0"))
	    {
	      //nvram_set("setup_4712","1-2");
	      nvram_set ("lan_ifnames", "vlan0 eth1");
	      nvram_set ("vlan0hwname", "et0");
	      nvram_set ("vlan1hwname", "et0");
	      nvram_set ("pppoe_ifname", "vlan1");
	      nvram_set ("wl0_ifname", "eth1");
//              nvram_set ("need_commit","1");
	    }
	}			// VLAN enabled
      else
	{
	  //nvram_set("setup_4712","2");
	  cprintf ("setup_4712(): Disable VLAN, it must be in bridge mode\n");
	  nvram_set ("lan_ifnames", "eth0 eth1");
	  strcpy (wlifname, "eth1");
	  nvram_set ("wl0_ifname", "eth1");
	}
    }
  else
    {				// 4702, 4704
      cprintf
	("setup_4712(): It's a 4702 or 4704 hardware, VLAN can't be used in these 2 boards\n");
      strcpy (wanifname, "eth1");
      strcpy (wlifname, "eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("pppoe_ifname", "eth1");
      if (!strcmp (nvram_safe_get ("wan_ifname"), ""))
	nvram_set ("lan_ifnames", "eth0 eth1 eth2 wlanb0 wlana0");
      else
	nvram_set ("lan_ifnames", "eth0 eth2");
    }
//nvram_set ("need_commit","1");



}


int
start_sysinit (void)
{
  char buf[PATH_MAX];
  struct utsname name;
  struct stat tmp_stat;
  time_t tm = 0;
  cprintf ("sysinit() proc\n");
  /* /proc */
  mount ("proc", "/proc", "proc", MS_MGC_VAL, NULL);
  cprintf ("sysinit() tmp\n");

  /* /tmp */
  mount ("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);
  eval ("mkdir", "/tmp/www");

  cprintf ("sysinit() var\n");

  /* /var */
  mkdir ("/tmp/var", 0777);
  mkdir ("/var/lock", 0777);
  mkdir ("/var/log", 0777);
  mkdir ("/var/run", 0777);
  mkdir ("/var/tmp", 0777);
  cprintf ("sysinit() setup console\n");

  eval ("/sbin/watchdog");	// system watchdog

  /* Setup console */

  cprintf ("sysinit() klogctl\n");
  klogctl (8, NULL, atoi (nvram_safe_get ("console_loglevel")));
  cprintf ("sysinit() get router\n");

  int brand = getRouterBrand ();
  char *rname = getRouter();
  fprintf(stderr,"Booting Device:%s\n",rname);   
  switch (brand)
    {
    case ROUTER_BUFFALO_WZRRSG54:
      check_brcm_cpu_type ();
      setup_4712 ();
      eval ("gpio", "disable", "4");	//maybe fix for reset button not working
#ifndef HAVE_MSSID
      nvram_set ("wl0gpio0", "130");	//Fix for wireless led polarity (v23 only)
#endif
      break;

    case ROUTER_MOTOROLA:
      nvram_set ("cpu_type", "BCM4712");
      nvram_set ("wl0gpio0", "2");	//Fix for wireless led, Eko.10.may.06
      setup_4712 ();
      break;

    case ROUTER_SIEMENS:
    case ROUTER_BELKIN_F5D7230:
      setup_4712 ();
      break;

    case ROUTER_RT210W:
      setup_4712 ();
      nvram_set ("wan_ifname", "eth1");	// fix for Belkin f5d7230 v1000 WAN problem.
      nvram_set ("wan_ifnames", "eth1");
      if (nvram_get ("et0macaddr") == NULL || nvram_get ("et0macaddr") == "")
	{
	  nvram_set ("et0macaddr", "00:16:E3:00:00:10");	//fix for missing cfe default = dead LAN ports.
	}
      break;

    case ROUTER_BRCM4702_GENERIC:
      setup_4712 ();
      nvram_set ("wan_ifname", "eth1");	// fix for Belkin f5d7230 v1000 WAN problem.
      nvram_set ("wan_ifnames", "eth1");

      if (nvram_get ("et0macaddr") == NULL || nvram_get ("et0macaddr") == "")
	{
	  nvram_set ("et0macaddr", "00:0C:6E:00:00:10");	//fix for missing cfe default = dead LAN ports.
	}
      break;
      
    case DELL_TRUEMOBILE_2300:
    	setup_4712();
    	nvram_set ("wan_ifname", "eth1");	// fix for WAN problem.
        nvram_set ("wan_ifnames", "eth1");   
      break;
                                               
    case ROUTER_WLI2_TX1_G54:
    case ROUTER_BUFFALO_WLAG54C:
    case ROUTER_WAP54G_V1:
      nvram_set ("lan_ifnames", "eth1 eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("wan_ifname", "eth0");	//WAN to nonexist. iface.
      nvram_set ("wan_ifnames", "eth0");
      nvram_set ("port_swap", "1");
      break;
      
    case ROUTER_MOTOROLA_WE800G:
      nvram_set ("lan_ifnames", "eth1 eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("wan_ifname", "eth0");	//WAN to nonexist. iface.
      nvram_set ("wan_ifnames", "eth0");
      nvram_set ("port_swap", "1");
      eval ("gpio", "disable", "7");
      break;

    case ROUTER_WZRG300N:
    case ROUTER_WRT300N:
    case ROUTER_WRTSL54GS:
      nvram_set ("wan_ifname", "eth1");
      nvram_set ("wan_ifnames", "eth1");
      nvram_set ("pppoe_wan_ifname", "eth1");
      break;
      
    case ROUTER_WRT350N:
      nvram_set ("wan_ifname", "vlan2");
      nvram_set ("wan_ifnames", "vlan2");
      nvram_set ("pppoe_wan_ifname", "vlan2");
    break;
    
    case ROUTER_ASUS_WL500G_PRE:
      nvram_set ("sdram_init", "0x0009");
//      nvram_set ("sdram_ncdl", "0x208");
      nvram_set ("lan_ifnames", "vlan0 eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("wan_ifname", "vlan1");	// fix for Asus WL500gPremium WAN problem.
      nvram_set ("wan_ifnames", "vlan1");
      nvram_set ("pppoe_wan_ifname", "vlan1");
      nvram_set ("vlan1ports", "0 5");
      eval ("gpio", "disable", "0");	//reset the reset button to 0
      break;


    case ROUTER_BUFFALO_WBR54G:
    case ROUTER_BUFFALO_WBR2G54S:
#ifdef HAVE_MSSID
      nvram_set ("wl0gpio0", "8");		//Fix for wireless led polarity (v24 only)
#else
	  nvram_set ("wl0gpio0", "130");	//Fix for wireless led polarity (v23 only)
#endif
      break;

    case ROUTER_BUFFALO_WLA2G54C:
      nvram_set ("lan_ifnames", "eth0 eth1");	// fix for WLA2G54C interfaces
      nvram_set ("wl0_ifname", "eth1");
      nvram_set ("wan_ifname", "eth2");	// map WAN port to nonexistant interface
      nvram_set ("wan_ifnames", "eth2");
      break;
      
#ifdef HAVE_MSSID
    case ROUTER_WRT54G:
      nvram_set ("wl0gpio0", "136");		//Fix for wireless led olways on (v24 only)
      break;
#endif
    }

  if (nvram_match ("boardnum", "1024") && nvram_match ("boardtype", "0x0446"))
    {
      nvram_set ("lan_ifnames", "eth0 eth1");	// fix for WAP54Gv2 interfaces
      nvram_set ("wl0_ifname", "eth1");
      nvram_set ("wan_ifname", "eth2");	// map WAN port to nonexistant interface
      nvram_set ("wan_ifnames", "eth2");
    }
    
    led_control (LED_POWER, LED_ON);
    led_control (LED_DIAG, LED_OFF);
    led_control (LED_DIAG2, LED_OFF);
    led_control (LED_AOSS, LED_OFF);
    led_control (LED_BRIDGE, LED_OFF);
    led_control (LED_WLAN, LED_OFF);
    

  /* ifnames */    
    strcpy (wanifname, nvram_safe_get ("wan_ifname"));
    strcpy (wlifname, nvram_safe_get ("wl0_ifname"));
    
    
  /* Modules */
  uname (&name);

  enableAfterBurner ();
#ifdef HAVE_MSSID
  nvram_set("pa0maxpwr", "251"); //force pa0maxpwr to be 251
#endif

  snprintf (buf, sizeof (buf), "/lib/modules/%s", name.release);
  if (stat ("/proc/modules", &tmp_stat) == 0 && stat (buf, &tmp_stat) == 0)
    {
      char module[80], *modules, *next;
      //modules="wl switch-core";

      if (check_vlan_support () && check_hw_type () != BCM5325E_CHIP)
	{
	  switch (brand)
	    {
	    case ROUTER_WRT350N:
	    modules = "diag bcm57xxlsys wl";
	    break;    
	    case ROUTER_LINKSYS_WRT55AG:
	    case ROUTER_MOTOROLA_V1:
	    case ROUTER_MOTOROLA:
	    case ROUTER_BUFFALO_WBR2G54S:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"diag wl switch-core switch-adm";

	      break;
	    case ROUTER_WRT54G1X:
	    case ROUTER_WRT54G:
	    case ROUTER_SIEMENS:
	    case ROUTER_WLI2_TX1_G54:
	    case ROUTER_BELKIN_F5D7230:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"diag wl";
	      eval ("insmod", "switch-core");
	      if (eval ("insmod", "switch-robo"))
		eval ("insmod", "switch-adm");

	      break;

	    default:

	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"diag wl switch-core switch-robo";
	      break;
	    }
	}
      else
	{
	  switch (brand)
	    {
	    case ROUTER_WRT350N:
	    modules = "diag bcm57xxlsys wl";
	    break;    
	    case ROUTER_LINKSYS_WRT55AG:
	    case ROUTER_MOTOROLA_V1:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"diag wl switch-core switch-adm";

	      break;
	    case ROUTER_ASUS:
	    case ROUTER_BELKIN_F5D7230:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"diag wl";
	      eval ("insmod", "switch-core");
	      if (eval ("insmod", "switch-robo"))
		eval ("insmod", "switch-adm");
	      break;
	    case ROUTER_BUFFALO_WZRRSG54:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"diag wl";
	      break;
	    default:
	      if (check_vlan_support ())
		modules =
		  nvram_invmatch ("ct_modules",
				  "") ? nvram_safe_get ("ct_modules") :
		  "diag wl switch-core switch-robo";
	      else
		modules =
		  nvram_invmatch ("ct_modules",
				  "") ? nvram_safe_get ("ct_modules") :
		  "diag wl";
	      break;
	    }
	}
      cprintf ("insmod %s\n", modules);

      foreach (module, modules, next)
      {
#ifdef HAVE_MACBIND
	if (nvram_match ("et0macaddr", MACBRAND))
	  eval ("insmod", module);
#else

	cprintf ("loading %s\n", module);
	eval ("insmod", module);
	cprintf ("done\n");
#endif
      }
      
#ifdef HAVE_USB
//load usb driver. we will add samba server, ftp server and ctorrent support in future
      modules = "usbcore usb-ohci usb-uhci ehci-hcd scsi_mod usb-storage ide-core ide-detect ide-disk ide-scsi cdrom ide-cd printer sd_mod sr_mod"
      foreach (module, modules, next)
      {
      cprintf ("loading %s\n", module);
      eval ("insmod", module);
      }
#endif
      
    }
  /* Set a sane date */
  stime (&tm);
  if (brand == ROUTER_SIEMENS)
    {
      eval ("insmod", "led.o");	// Jerry Lee
      powerled_ctrl (0);
      led_ctrl (0);		// turn LED2 off
    }

  return 0;
  cprintf ("done\n");
}
