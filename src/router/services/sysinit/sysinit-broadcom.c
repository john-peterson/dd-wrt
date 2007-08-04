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
loadWlModule (void)		//set wled params, get boardflags, set afterburner bit, load wl, unset afterburner bit
{

  int brand = getRouterBrand ();

#ifdef HAVE_MSSID		//v24

#ifndef HAVE_BUFFALO
  nvram_set ("pa0maxpwr", "251");	//force pa0maxpwr to be 251
#endif

  switch (brand)
    {
    case ROUTER_ASUS_WL500G_PRE:
    case ROUTER_WRT54G:
    case ROUTER_MOTOROLA:
    case ROUTER_BUFFALO_WLAG54C:
      nvram_set ("wl0gpio0", "136");
      break;
    case ROUTER_BUFFALO_WBR54G:
    case ROUTER_BUFFALO_WBR2G54S:
    case ROUTER_WRT150N:
    case ROUTER_WRT300N:
    case ROUTER_WRT350N:
      nvram_set ("wl0gpio0", "8");
      break;
    case ROUTER_BUFFALO_WHRG54S:
    case ROUTER_BUFFALO_WLI_TX4_G54HP:
      nvram_set ("wl0gpio2", "136");
      break;
    case ROUTER_BUFFALO_WLA2G54C:
      nvram_set ("wl0gpio0", "0");
      nvram_set ("wl0gpio5", "136");
      break;
    }
#else //v23

  switch (brand)
    {
    case ROUTER_BUFFALO_WZRRSG54:
    case ROUTER_BUFFALO_WBR54G:
    case ROUTER_BUFFALO_WBR2G54S:
      nvram_set ("wl0gpio0", "130");
      break;
    case ROUTER_MOTOROLA:
      nvram_set ("wl0gpio0", "2");
      break;
    case ROUTER_BUFFALO_WLA2G54C:
      nvram_set ("wl0gpio0", "0");
      nvram_set ("wl0gpio5", "2");
      break;
    }
#endif

  int boardflags;

  switch (brand)
    {
    case ROUTER_WRT150N:
    case ROUTER_WRT300N:
    case ROUTER_WRT350N:
    case ROUTER_BUFFALO_WZRG144NH:
    case ROUTER_BUFFALO_WZRG300N:
    case ROUTER_NETGEAR_WNR834B:
      eval ("insmod", "wl");	//load module
      break;
    default:
      boardflags = strtoul (nvram_safe_get ("boardflags"), NULL, 0);
      fprintf (stderr, "boardflags are 0x%04X\n", boardflags);
      if (boardflags == 0)	//we can try anyway
	{
	  nvram_set ("boardflags", "0x0200");
	  eval ("insmod", "wl");	//load module
	  nvram_unset ("boardflags");
	}
      else if (boardflags & BFL_AFTERBURNER)	//ab flag already set
	{
	  eval ("insmod", "wl");	//load module
	}
      else			//ab flag not set
	{
	  char bf[16];
	  sprintf (bf, "0x%04X", boardflags);
	  boardflags |= BFL_AFTERBURNER;
	  fprintf (stderr, "enable Afterburner, boardflags are 0x%04X\n",
		   boardflags);
	  char ab[16];
	  sprintf (ab, "0x%04X", boardflags);
	  nvram_set ("boardflags", ab);	//set boardflags with AfterBurner bit on
	  eval ("insmod", "wl");	//load module
	  nvram_set ("boardflags", bf);	//set back to original
	}

    }
  return;
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
  led_control(LED_DIAG,LED_ON);
  char *rname = getRouter ();
  fprintf (stderr, "Booting Device:%s\n", rname);
  switch (brand)
    {
    case ROUTER_BUFFALO_WZRRSG54:
      check_brcm_cpu_type ();
      setup_4712 ();
      break;

    case ROUTER_MOTOROLA:
      nvram_set ("cpu_type", "BCM4712");
      setup_4712 ();
      break;

    case ROUTER_RT480W:
    case ROUTER_USR_5461:
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
    case ROUTER_ASUS_WL500G:
      setup_4712 ();
      nvram_set ("wan_ifname", "eth1");	// fix for Belkin f5d7230 v1000 WAN problem.
      nvram_set ("wan_ifnames", "eth1");

      if (nvram_get ("et0macaddr") == NULL || nvram_get ("et0macaddr") == "")
	{
	  nvram_set ("et0macaddr", "00:0C:6E:00:00:10");	//fix for missing cfe default = dead LAN ports.
	}
      break;

    case ROUTER_DELL_TRUEMOBILE_2300:
      setup_4712 ();
      nvram_set ("wan_ifname", "eth1");	// fix for WAN problem.
      nvram_set ("wan_ifnames", "eth1");
      break;

	case ROUTER_BUFFALO_WBR54G:  //for WLA-G54
      nvram_set ("wan_ifname", "eth1");
      nvram_set ("wan_ifnames", "eth1");	
      if (nvram_match ("wan_to_lan", "yes") && nvram_invmatch ("wan_proto", "disabled"))  // = no lan
      	{
	    nvram_set ("lan_ifnames", "eth2");
      	nvram_set ("wan_ifname", "eth0");
      	nvram_set ("wan_ifnames", "eth0");
  		}
      break;
            
    case ROUTER_BUFFALO_WLI2_TX1_G54:
    case ROUTER_BUFFALO_WLAG54C:
    case ROUTER_WAP54G_V1:
    case ROUTER_SITECOM_WL105B:
      nvram_set ("lan_ifnames", "eth1 eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("wan_ifname", "eth0");	//WAN to nonexist. iface.
      nvram_set ("wan_ifnames", "eth0");
      nvram_set ("port_swap", "1");
      if (nvram_match ("wan_to_lan", "yes") && nvram_invmatch ("wan_proto", "disabled"))  // = no lan
      	{
	    nvram_set ("lan_ifnames", "eth2");
      	nvram_set ("wan_ifname", "eth1");
      	nvram_set ("wan_ifnames", "eth1");
  		}
      break;

    case ROUTER_MOTOROLA_WE800G:
      nvram_set ("lan_ifnames", "eth1 eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("wan_ifname", "eth0");	//WAN to nonexist. iface.
      nvram_set ("wan_ifnames", "eth0");
      nvram_set ("port_swap", "1");
      eval ("gpio", "disable", "7");
      if (nvram_match ("wan_to_lan", "yes") && nvram_invmatch ("wan_proto", "disabled"))  // = no lan
      	{
	    nvram_set ("lan_ifnames", "eth2");
      	nvram_set ("wan_ifname", "eth1");
      	nvram_set ("wan_ifnames", "eth1");
  		}
      break;

    case ROUTER_MOTOROLA_V1:
      eval ("gpio", "disable", "7");
      break;

    case ROUTER_BUFFALO_WZRG300N:
    case ROUTER_NETGEAR_WNR834B:
    case ROUTER_WRT150N:
    case ROUTER_WRT300N:
    case ROUTER_BUFFALO_WLAH_G54:
    case ROUTER_BUFFALO_WAPM_HP_AM54G54:
    case ROUTER_WRTSL54GS:
      nvram_set ("wan_ifname", "eth1");
      nvram_set ("wan_ifnames", "eth1");
      break;
    case ROUTER_WRT54G1X:
    if (check_vlan_support ())
	    {
    	    nvram_set ("lan_ifnames", "vlan0 eth2");
    	    nvram_set ("wan_ifname", "vlan1");
    	    nvram_set ("wan_ifnames", "vlan1");	    
	    }
    break;
    case ROUTER_WRT350N:
      nvram_set ("wan_ifname", "vlan2");
      nvram_set ("wan_ifnames", "vlan2");
      break;
    case ROUTER_BUFFALO_WZRG144NH:
      nvram_set ("wan_ifname", "vlan1");
      nvram_set ("wan_ifnames", "vlan1");
      break;

    case ROUTER_ASUS_WL500G_PRE:
      nvram_set ("sdram_init", "0x0009");
//      nvram_set ("sdram_ncdl", "0x208");
      nvram_set ("lan_ifnames", "vlan0 eth2");
      nvram_set ("wl0_ifname", "eth2");
      nvram_set ("wan_ifname", "vlan1");	// fix for Asus WL500gPremium WAN problem.
      nvram_set ("wan_ifnames", "vlan1");
      nvram_set ("vlan1ports", "0 5");
      break;

    case ROUTER_BUFFALO_WLA2G54C:
    case ROUTER_WAP54G_V2:
    case ROUTER_VIEWSONIC_WAPBR_100:
    case ROUTER_USR_5430:
    case ROUTER_BUFFALO_WLI_TX4_G54HP:
      nvram_set ("lan_ifnames", "eth0 eth1");
      nvram_set ("wl0_ifname", "eth1");
      nvram_set ("wan_ifname", "eth2");	// map WAN port to nonexistant interface
      nvram_set ("wan_ifnames", "eth2");
      if (nvram_match ("wan_to_lan", "yes") && nvram_invmatch ("wan_proto", "disabled"))  // = no lan
      	{
	    nvram_set ("lan_ifnames", "eth1");
      	nvram_set ("wan_ifname", "eth0");
      	nvram_set ("wan_ifnames", "eth0");
  		}
      break;

    case ROUTER_DELL_TRUEMOBILE_2300_V2:	//we must fix cfe defaults with CR added
      nvram_set ("vlan0hwname", "et0");
      nvram_set ("vlan1hwname", "et0");
      nvram_set ("et0mdcport", "0");
      nvram_set ("et0phyaddr", "30");
      nvram_set ("gpio2", "adm_eecs");
      nvram_set ("gpio3", "adm_eesk");
      nvram_set ("gpio4", "adm_eedi");
      nvram_set ("gpio5", "adm_rc");
      nvram_unset ("gpio6");
      break;
   
	case ROUTER_ASUS_WL500GD:
	  nvram_unset ("wl0gpio0");
	  break;  

    }

  /* ifnames */
  strcpy (wanifname, nvram_safe_get ("wan_ifname"));
  strcpy (wlifname, nvram_safe_get ("wl0_ifname"));

  /* set pppoe_wan_ifname */
  nvram_set ("pppoe_wan_ifname", wanifname); 

  /* Modules */
  uname (&name);


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
	    case ROUTER_BUFFALO_WZRG144NH:
	      modules = "bcm57xxlsys";
	      break;
	    case ROUTER_LINKSYS_WRT55AG:
	    case ROUTER_MOTOROLA:
	    case ROUTER_BUFFALO_WBR2G54S:
	    case ROUTER_DELL_TRUEMOBILE_2300_V2:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"switch-core switch-adm";

	      break;
	    case ROUTER_WRT54G1X:
	    case ROUTER_WRT54G:
	    case ROUTER_RT480W:
	    case ROUTER_BUFFALO_WLI2_TX1_G54:
	      modules = "diag";
	      eval ("insmod", "switch-core");
	      if (eval ("insmod", "switch-robo"))
		eval ("insmod", "switch-adm");

	      break;
	    case ROUTER_WRT54G3G:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"switch-core switch-robo pcmcia_core yenta_socket ds serial_cs usbcore usb-ohci usbserial sierra";
	      break;

	    default:

	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"switch-core switch-robo";
	      break;
	    }
	}
      else
	{
	  switch (brand)
	    {
	    case ROUTER_WRT350N:
	    case ROUTER_BUFFALO_WZRG144NH:
	      modules = "bcm57xxlsys";
	      break;
	    case ROUTER_LINKSYS_WRT55AG:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") :
		"switch-core switch-adm";

	      break;
	    case ROUTER_ASUS_WL500GD:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") : "";
	      eval ("insmod", "switch-core");
	      if (eval ("insmod", "switch-robo"))
		eval ("insmod", "switch-adm");
	      break;
	    case ROUTER_BUFFALO_WZRRSG54:
	      modules =
		nvram_invmatch ("ct_modules",
				"") ? nvram_safe_get ("ct_modules") : "";
	      break;
	    case ROUTER_WRT54G3G:
	      if (check_vlan_support ())
		modules =
		  nvram_invmatch ("ct_modules",
				  "") ? nvram_safe_get ("ct_modules") :
		  "switch-core switch-robo pcmcia_core yenta_socket ds";
	      else
		modules =
		  nvram_invmatch ("ct_modules",
				  "") ? nvram_safe_get ("ct_modules") :
		  "pcmcia_core yenta_socket ds";
	      break;

	    default:
	      if (check_vlan_support ())
		modules =
		  nvram_invmatch ("ct_modules",
				  "") ? nvram_safe_get ("ct_modules") :
		  "switch-core switch-robo";
	      else
		modules =
		  nvram_invmatch ("ct_modules",
				  "") ? nvram_safe_get ("ct_modules") :
		  "";
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

      loadWlModule ();

#ifdef HAVE_USB
//load usb driver. we will add samba server, ftp server and ctorrent support in future
      modules =
	"usbcore usb-ohci usb-uhci ehci-hcd scsi_mod usb-storage ide-core ide-detect ide-disk ide-scsi cdrom ide-cd printer sd_mod sr_mod"
	foreach (module, modules, next)
      {
	cprintf ("loading %s\n", module);
	eval ("insmod", module);
      }
#endif

    }
  /* Set a sane date */
  stime (&tm);

  led_control (LED_POWER, LED_ON);
  led_control (LED_SES, LED_OFF);
  led_control (LED_BRIDGE, LED_OFF);
  led_control (LED_WLAN, LED_OFF);

  if (brand == ROUTER_WRT54G3G)
    {
      eval ("cardmgr");
    }

  cprintf ("done\n");
  return 0;

}
static int
check_nv (char *name, char *value)
{
  int ret = 0;
  if (nvram_match ("manual_boot_nv", "1"))
    return 0;

  if (!nvram_get (name))
    {
      cprintf ("ERR: Cann't find %s !.......................\n", name);
      nvram_set (name, value);
      ret++;
    }
  else if (nvram_invmatch (name, value))
    {
      cprintf ("ERR: The %s is %s, not %s !.................\n", name,
	       nvram_safe_get (name), value);
      nvram_set (name, value);
      ret++;
    }

  return ret;
}

int
check_cfe_nv (void)
{
  int ret = 0;

  switch (getRouterBrand ())
    {

    case ROUTER_BUFFALO_WBR54G:
    case ROUTER_BUFFALO_WZRRSG54:
      ret += check_nv ("lan_hwnames", "et0 wl0");
      ret += check_nv ("lan_ifnames", "eth0 eth2");
      ret += check_nv ("wan_hwname", "et1");
      ret += check_nv ("wan_ifname", "eth1");
      ret += check_nv ("wan_ifnames", "eth1");
      ret += check_nv ("pppoe_ifname", "eth1");
      ret += check_nv ("wl0_ifname", "eth2");
      ret += check_nv ("vlans", "0");
      break;
    case ROUTER_BUFFALO_WBR2G54S:
      ret += check_nv ("aa0", "3");

      ret += check_nv ("pa0itssit", "62");
      ret += check_nv ("pa0b0", "0x1136");
      ret += check_nv ("pa0b1", "0xfb93");
      ret += check_nv ("pa0b2", "0xfea5");
#ifndef HAVE_MSSID
      ret += check_nv ("pa0maxpwr", "60");
#endif
      ret += check_nv ("wl0gpio2", "0");
      ret += check_nv ("wl0gpio3", "0");
      ret += check_nv ("cctl", "0");
      ret += check_nv ("ccode", "0");
      break;
#ifndef HAVE_BUFFALO
    case ROUTER_ASUS_WL500GD:
      ret += check_nv ("wl0_ifname", "eth1");
      return 0;
      break;
    case ROUTER_LINKSYS_WRT55AG:
    case ROUTER_MOTOROLA_V1:
    case ROUTER_WRT54G1X:
      break;

    case ROUTER_WRT54G:
      ret += check_nv ("aa0", "3");
      if (check_hw_type () == BCM5352E_CHIP)
	ret += check_nv ("ag0", "0x02");
      else
	ret += check_nv ("ag0", "255");
      if (check_hw_type () == BCM5325E_CHIP)
	{
	  /* Lower the DDR ram drive strength , the value will be stable for all boards
	     Latency 3 is more stable for all ddr 20050420 by honor */

	  ret += check_nv ("sdram_init", "0x010b");
	  ret += check_nv ("sdram_config", "0x0062");
	  if (nvram_match ("clkfreq", "200")
	      && nvram_match ("overclocking", "200"))
	    {
	      ret += check_nv ("clkfreq", "216");
	      nvram_set ("overclocking", "216");
	    }

	  if (ret)
	    {
	      nvram_set ("sdram_ncdl", "0x0");

	    }
	  ret += check_nv ("pa0itssit", "62");
	  ret += check_nv ("pa0b0", "0x15eb");
	  ret += check_nv ("pa0b1", "0xfa82");
	  ret += check_nv ("pa0b2", "0xfe66");
#ifndef HAVE_MSSID
	  ret += check_nv ("pa0maxpwr", "0x4e");
#endif
	}
      else if (check_hw_type () == BCM4705_BCM5397_EWC_CHIP)
	{
	  // nothing to do
	}
      else if (check_hw_type () == BCM4704_BCM5325F_CHIP)
	{
	  //nothing to do
	}
      else
	{
	  ret += check_nv ("pa0itssit", "62");
	  ret += check_nv ("pa0b0", "0x170c");
	  ret += check_nv ("pa0b1", "0xfa24");
	  ret += check_nv ("pa0b2", "0xfe70");
#ifndef HAVE_MSSID
	  ret += check_nv ("pa0maxpwr", "0x48");
#endif
	}

      //ret += check_nv("gpio2", "adm_eecs");
      //ret += check_nv("gpio3", "adm_eesk");
      //ret += check_nv("gpio5", "adm_eedi");
      //ret += check_nv("gpio6", "adm_rc");

      ret += check_nv ("wl0gpio2", "0");
      ret += check_nv ("wl0gpio3", "0");

      ret += check_nv ("cctl", "0");
      ret += check_nv ("ccode", "0");
      break;
#endif
    }
  if (ret)
    {
      cprintf ("Some error found, we want to reboot!.....................\n");
      nvram_commit ();
      kill (1, SIGTERM);
      exit (0);
    }


  return ret;
}

int
check_pmon_nv (void)
{
  return 0;
}

#define ISCLK(a) nvram_match("clkfreq",a);
static void
overclock (void)
{
  int rev = getcpurev ();
  char *ov = nvram_get ("overclocking");
  if (ov == NULL)
    return;
  int clk = atoi (ov);
  if (nvram_get ("clkfreq") == NULL)
    return;			//unsupported
  if (nvram_match ("clkfreq", "125"))
    return;			//unsupported
  if (rev == 0)
    return;			//unsupported

//int cclk = atoi(nvram_safe_get("clkfreq"));
//if (cclk<192)return; //unsupported
  char *pclk = nvram_safe_get ("clkfreq");
  char dup[64];
  strcpy (dup, pclk);
  int i;
  for (i = 0; i < strlen (dup); i++)
    if (dup[i] == ',')
      dup[i] = 0;
  int cclk = atoi (dup);
  if (cclk < 192 && rev == 7)
    {
      cprintf ("clkfreq is %d (%s), this is unsupported\n", cclk, dup);
      return;			//unsupported
    }
  if (cclk < 183 && rev == 8)
    {
      cprintf ("clkfreq is %d (%s), this is unsupported\n", cclk, dup);
      return;			//unsupported
    }

  if (clk == cclk)
    {
      cprintf ("clkfreq identical with new setting\n");
      return;			//clock already set
    }


  int set = 1;

  switch (clk)
    {
    case 183:
      nvram_set ("clkfreq", "183,92");
      break;
    case 187:
      nvram_set ("clkfreq", "187,94");
      break;
    case 192:
      nvram_set ("clkfreq", "192,96");
      break;
    case 198:
      nvram_set ("clkfreq", "198,98");
      break;
    case 200:
      nvram_set ("clkfreq", "200,100");
      break;
    case 216:
      nvram_set ("clkfreq", "216,108");
      break;
    case 225:
      nvram_set ("clkfreq", "225,113");
      break;
    case 228:
      nvram_set ("clkfreq", "228,114");
      break;
    case 233:
      nvram_set ("clkfreq", "233,116");
      break;
    case 237:
      nvram_set ("clkfreq", "237,119");
      break;
    case 240:
      nvram_set ("clkfreq", "240,120");
      break;
    case 250:
      nvram_set ("clkfreq", "250,125");
      break;
    case 252:
      nvram_set ("clkfreq", "252,126");
      break;
    case 264:
      nvram_set ("clkfreq", "264,132");
      break;
    case 280:
      nvram_set ("clkfreq", "280,120");
      break;
    case 300:
      nvram_set ("clkfreq", "300,120");
      break;
    default:
      set = 0;
      break;
    }

  if (set)
    {
      cprintf ("clock frequency adjusted from %d to %d, reboot needed\n",
	       cclk, clk);
      nvram_commit ();
      kill (1, SIGTERM);
      exit (0);
    }
}

void
start_overclocking (void)
{
cprintf("Overclocking...\n");
  overclock ();
cprintf("Overclocking, done\n");
}
