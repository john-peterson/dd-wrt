
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
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
#include <typedefs.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <wlutils.h>
#include <cy_conf.h>

#include <revision.h>



/* States */
enum
{
  RESTART,
  STOP,
  START,
  TIMER,
  USER,
  IDLE,
#ifdef HAVE_X86
  REBOOT,
#endif
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
	  lcdmessage ("Signal RESTART");
	  printf ("signalling RESTART\n");
	  signalled = RESTART;
	}
      else if (sig == SIGUSR2)
	{
	  lcdmessage ("Signal START");
	  printf ("signalling START\n");
	  signalled = START;
	}
      else if (sig == SIGINT)
	{
	  lcdmessage ("Signal STOP");
	  printf ("signalling STOP\n");
	  signalled = STOP;
	}
      else if (sig == SIGALRM)
	{
	  lcdmessage ("Signal TIMER");
	  printf ("signalling TIMER\n");
	  signalled = TIMER;
	}
#ifdef HAVE_X86
      else if (sig == SIGTERM)
	{
	  lcdmessage ("Signal Reboot");
	  printf ("signalling REBOOT\n");
	  signalled = REBOOT;
	}
#endif
      else if (sig == SIGUSR1)
	{			// Receive from WEB
	  lcdmessage ("Signal USER");
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
static int noconsole = 0;

/* Main loop */
int
main_loop (int argc, char **argv)
{
  sigset_t sigset;
  pid_t shell_pid = 0;
  uint boardflags;
  //setenv("PATH", "/sbin:/bin:/usr/sbin:/usr/bin:/jffs/sbin:/jffs/bin:/jffs/usr/sbin:/jffs/usr/bin", 1);
  //system("/etc/nvram/nvram");
  /* Basic initialization */
  cprintf ("console init\n");
  if (console_init ())
    noconsole = 1;
  cprintf ("init lcd\n");
  initlcd ();
  cprintf ("first message\n");
  lcdmessage ("System Start");
  cprintf ("start service\n");
  start_service ("sysinit");
  cprintf ("setup signals\n");
  /* Setup signal handlers */
  signal_init ();
  signal (SIGHUP, rc_signal);
  signal (SIGUSR1, rc_signal);	// Start single service from WEB, by honor
  signal (SIGUSR2, rc_signal);
  signal (SIGINT, rc_signal);
  signal (SIGALRM, rc_signal);
#ifdef HAVE_X86
  signal (SIGTERM, rc_signal);
#endif
  sigemptyset (&sigset);

  /* Give user a chance to run a shell before bringing up the rest of the system */

  if (!noconsole)
    ddrun_shell (1, 0);
  cprintf ("setup nvram\n");

  start_service ("nvram");

  /* Restore defaults if necessary */
  int brand = getRouterBrand ();

#ifdef HAVE_SKYTEL
  nvram_set ("vlan0ports", "0 1 2 3 4 5*");
  nvram_set ("vlan1ports", "");
#else

  if (brand == ROUTER_WRT600N)
    {
      nvram_set ("vlan2hwname", "et0");
    }


#endif
  start_service ("restore_defaults");


  /* Add vlan */
  boardflags = strtoul (nvram_safe_get ("boardflags"), NULL, 0);
  nvram_set ("wanup", "0");


#ifndef HAVE_RB500
  switch (brand)
    {
    case ROUTER_WRT600N:
    case ROUTER_ASUS_WL500GD:
    case ROUTER_ASUS_WL550GE:
    case ROUTER_MOTOROLA:
    case ROUTER_RT480W:
    case ROUTER_WRT350N:
    case ROUTER_BUFFALO_WZRG144NH:
    case ROUTER_DELL_TRUEMOBILE_2300_V2:
      start_service ("config_vlan");
      break;
    default:
      if (check_vlan_support ())
	{
	  start_service ("config_vlan");
	}
      break;

    }
#endif

  set_ip_forward ('1');
  system ("/etc/preinit");	//sets default values for ip_conntrack
#ifdef HAVE_JFFS2
  start_service ("jffs2");
#endif
#ifdef HAVE_MMC
  start_service ("mmc");
#endif

  start_service ("mkfiles");
  char *hostname;

  /* set hostname to wan_hostname or router_name */
  if (strlen (nvram_safe_get ("wan_hostname")) > 0)
    hostname = nvram_safe_get ("wan_hostname");
  else if (strlen (nvram_safe_get ("router_name")) > 0)
    hostname = nvram_safe_get ("router_name");
  else
    hostname = "dd-wrt";

  sethostname (hostname, strlen (hostname));
  stop_service ("httpd");

//create loginprompt
  FILE *fp = fopen ("/tmp/loginprompt", "wb");

#ifndef HAVE_MAKSAT
#ifndef HAVE_MSSID
#ifdef DIST
  if (strlen (DIST) > 0)
    fprintf (fp,
	     "DD-WRT v23 SP3 %s (c) 2008 NewMedia-NET GmbH\nRelease: "
	     BUILD_DATE " (SVN revision: %s)\n", DIST, SVN_REVISION);
  else
    fprintf (fp,
	     "DD-WRT v23 SP3 custom (c) 2008 NewMedia-NET GmbH\nRelease: "
	     BUILD_DATE " (SVN revision: %s)\n", SVN_REVISION);
#else
  fprintf (fp,
	   "DD-WRT v23 SP3 custom (c) 2008 NewMedia-NET GmbH\nRelease: "
	   BUILD_DATE " (SVN revision: %s)\n", SVN_REVISION);
#endif
#else
#ifdef DIST
  if (strlen (DIST) > 0)
    fprintf (fp,
	     "DD-WRT v24 %s (c) 2008 NewMedia-NET GmbH\nRelease: " BUILD_DATE
	     " (SVN revision: %s)\n", DIST, SVN_REVISION);
  else
    fprintf (fp,
	     "DD-WRT v24 custom (c) 2008 NewMedia-NET GmbH\nRelease: "
	     BUILD_DATE " (SVN revision: %s)\n", SVN_REVISION);
#else
  fprintf (fp,
	   "DD-WRT v24 custom (c) 2008 NewMedia-NET GmbH\nRelease: "
	   BUILD_DATE " (SVN revision: %s)\n", SVN_REVISION);
#endif
#endif
#endif

  fclose (fp);
  /* Loop forever */
  for (;;)
    {
      switch (state)
	{
	case USER:		// Restart single service from WEB of tftpd, by honor
	  lcdmessage ("RESTART SERVICES");
	  cprintf ("USER1\n");
	  start_single_service ();
#ifdef HAVE_CHILLI
	  start_service ("chilli");
#endif
#ifdef HAVE_WIFIDOG
	  start_service ("wifidog");
#endif

	  state = IDLE;
	  break;

	case RESTART:
	  lcdmessage ("RESTART SYSTEM");
#ifdef HAVE_OVERCLOCKING
	  start_service ("overclocking");
#endif
	  cprintf ("RESET NVRAM VARS\n");
	  nvram_set ("wl0_lazy_wds", nvram_safe_get ("wl_lazy_wds"));
#ifndef HAVE_MSSID
	  nvram_set ("wl0_akm", nvram_safe_get ("wl_akm"));
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
#endif

	  cprintf ("RESTART\n");

#ifndef HAVE_MADWIFI
	  eval ("wlconf", nvram_safe_get ("wl0_ifname"), "down");
#ifdef HAVE_MSSID
	  char *next;
	  char var[80];
	  char *vifs = nvram_safe_get ("wl0_vifs");
	  if (vifs != NULL)
	    foreach (var, vifs, next)
	    {
	      eval ("ifconfig", var, "down");
	    }
#endif
#endif

	  /* Fall through */
	case STOP:
	  lcdmessage ("STOPPING SERVICES");
	  cprintf ("STOP\n");
	  killall ("udhcpc", SIGKILL);
	  setenv ("PATH",
		  "/sbin:/bin:/usr/sbin:/usr/bin:/jffs/sbin:/jffs/bin:/jffs/usr/sbin:/jffs/usr/bin",
		  1);
	  setenv ("LD_LIBRARY_PATH",
		  "/lib:/usr/lib:/jffs/lib:/jffs/usr/lib:/mmc/lib:/mmc/usr/lib:",
		  1);
	  cprintf ("STOP SERVICES\n");

	  stop_services ();
	  stop_service ("radio_timer");
#ifndef HAVE_MADWIFI
	  stop_service ("nas");
#endif
	  cprintf ("STOP WAN\n");
	  stop_service ("ttraff");
	  stop_service ("wan");
	  cprintf ("STOP LAN\n");
#ifdef HAVE_MADWIFI
	  stop_service ("stabridge");
#endif
#ifdef HAVE_RSTP
	  eval ("killall", "rstpd");
	  unlink ("/tmp/.rstp_server");
#endif
#ifdef HAVE_VLANTAGGING
	  stop_service ("bridging");
#endif
#ifdef HAVE_BONDING
	  stop_service ("bonding");
#endif

	  stop_service ("lan");
#ifdef HAVE_VLANTAGGING
	  stop_service ("bridgesif");
	  stop_service ("vlantagging");
#endif

#ifndef HAVE_RB500
	  stop_service ("resetbutton");
#endif
	  start_service ("create_rc_shutdown");
	  system ("/tmp/.rc_shutdown");
	  if (state == STOP)
	    {
	      state = IDLE;
	      break;
	    }
	  /* Fall through */
	case START:
	  lcdmessage ("START SERVICES");
	  nvram_set ("wl0_lazy_wds", nvram_safe_get ("wl_lazy_wds"));
#ifndef HAVE_MSSID
	  nvram_set ("wl0_akm", nvram_safe_get ("wl_akm"));
#endif
	  cprintf ("START\n");
	  setenv ("PATH",
		  "/sbin:/bin:/usr/sbin:/usr/bin:/jffs/sbin:/jffs/bin:/jffs/usr/sbin:/jffs/usr/bin",
		  1);
	  setenv ("LD_LIBRARY_PATH",
		  "/lib:/usr/lib:/jffs/lib:/jffs/usr/lib:/mmc/lib:/mmc/usr/lib:",
		  1);
#ifdef HAVE_IPV6
	  start_service ("ipv6");
#endif
#ifndef HAVE_RB500
	  start_service ("resetbutton");
#endif
	  start_service ("setup_vlans");
#ifndef HAVE_MADWIFI
	  if (nvram_match ("wl0_mode", "apstawet"))	//temporary fix for repeater-bridge mode init problem
	    {
	      nvram_set ("wl0_mode", "wet");
	      start_service ("wlconf");
	      eval ("wlconf", nvram_safe_get ("wl0_ifname"), "down");
	      nvram_set ("wl0_mode", "apstawet");
	    }
	  start_service ("wlconf");
#endif

#ifdef HAVE_VLANTAGGING
	  start_service ("bridging");
#endif
	  start_service ("lan");
#ifdef HAVE_BONDING
	  start_service ("bonding");
#endif
#ifdef HAVE_REGISTER
	  start_service ("mkfiles");
#endif
#ifdef HAVE_MADWIFI
	  start_service ("stabridge");
#endif


	  cprintf ("start services\n");
	  start_services ();

	  cprintf ("start wan boot\n");
	  start_service ("wan_boot");
	  start_service ("ttraff");

	  cprintf ("diag STOP LED\n");
	  diag_led (DIAG, STOP_LED);
	  cprintf ("set led release wan control\n");
	  SET_LED (RELEASE_WAN_CONTROL);

	  if (nvram_match ("wl0_mode", "sta")
	      || nvram_match ("wl0_mode", "wet")
	      || nvram_match ("wl0_mode", "apsta")
	      || nvram_match ("wl0_mode", "apstawet"))
	    {
	      //fix for client mode
	      cprintf ("ifconfig wl up\n");
	      eval ("/sbin/ifconfig", get_wdev (), "up");
	    }

#ifndef HAVE_MADWIFI
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1")
	      && nvram_match ("radiooff_boot_off", "1"))
	    {
	      eval ("wl", "-i", get_wl_instance_name (0), "radio", "off");
	    }
	  else
#endif
	    start_service ("nas");
#ifdef HAVE_MSSID
	  start_service ("guest_nas");
#endif
#endif

	  start_service ("radio_timer");
#ifdef HAVE_VLANTAGGING
	  start_service ("vlantagging");
	  start_service ("bridgesif");
#endif


	  cprintf ("create rc file\n");
#ifdef HAVE_REGISTER
	  if (isregistered ())
#endif
	    {
	      start_service ("create_rc_startup");
	      chmod ("/tmp/.rc_startup", 0700);
	      system ("/tmp/.rc_startup");
	      system ("/etc/init.d/rcS");	// start openwrt startup script (siPath impl)
	      cprintf ("start modules\n");
	      start_service ("modules");
#ifdef HAVE_MILKFISH
	      start_service ("milkfish_boot");
#endif
	      if (nvram_invmatch ("rc_custom", ""))	//create custom script
		{
		  nvram2file ("rc_custom", "/tmp/custom.sh");
		  chmod ("/tmp/custom.sh", 0700);
		}
	    }

#ifdef HAVE_CHILLI
	  start_service ("chilli");
#endif
#ifdef HAVE_WIFIDOG
	  start_service ("wifidog");
#endif
	  cprintf ("start syslog\n");
#ifndef HAVE_WRK54G
	  startstop ("syslog");
#endif
#ifdef HAVE_RSTP
//just experimental for playing
	  eval ("brctl", "stp", "br0", "off");
	  eval ("rstpd");
	  eval ("rstpctl", "rstp", "br0", "on");
#endif

	  system ("/etc/postinit");

	  led_control (LED_DIAG, LED_OFF);
	  lcdmessage ("System Ready");
	  /* Fall through */
	case TIMER:
	  cprintf ("TIMER\n");
	  do_timer ();
	  /* Fall through */
	case IDLE:
	  cprintf ("IDLE\n");
	  state = IDLE;
	  /* Wait for user input or state change */
	  while (signalled == -1)
	    {
	      if (!noconsole && (!shell_pid || kill (shell_pid, 0) != 0))
		shell_pid = ddrun_shell (0, 1);
	      else
		sigsuspend (&sigset);

	    }
	  state = signalled;
	  signalled = -1;
	  break;
#ifdef HAVE_X86
	case REBOOT:
	  lcdmessage ("System Reboots!");
	  system ("reboot");
	  break;
#endif
	default:
	  cprintf ("UNKNOWN\n");
	  return 0;
	}

    }

}

int
get_wanface (int argc, char **argv)
{
  fprintf (stdout, "%s", get_wan_face ());
  return 0;
}

struct MAIN
{
  char *callname;
  char *execname;
  int (*exec) (int argc, char **argv);
};

static struct MAIN maincalls[] = {
  {"init", NULL, &main_loop},
  {"ip-up", "ipup", NULL},
  {"ip-down", "ipdown", NULL},
  {"ipdown", "disconnected_pppoe", NULL},
  {"udhcpc", "udhcpc", NULL},
  {"poptop", NULL, &pptpd_main},
  {"redial", NULL, &redial_main},
#ifndef HAVE_RB500
  {"resetbutton", NULL, &resetbutton_main},
#endif
  {"wland", "wland", NULL},
  {"hb_connect", "hb_connect", NULL},
  {"hb_disconnect", "hb_disconnect", NULL},
  {"gpio", "gpio", NULL},
  {"beep", "beep", NULL},
  {"listen", NULL, &listen_main},
  {"check_ps", NULL, &check_ps_main},
  {"ddns_success", "ddns_success", NULL},
  {"process_monitor", NULL, &process_monitor_main},
  {"radio_timer", NULL, &radio_timer_main},
  {"ttraf", NULL, &ttraff_main},
#ifdef HAVE_WIVIZ
  {"run_wiviz", NULL, &run_wiviz_main},
  {"autokill_wiviz", NULL, &autokill_wiviz_main},
#endif
  {"site_survey", "site_survey", NULL},
#ifdef HAVE_WOL
  {"wol", NULL, &wol_main},
#endif
  {"event", NULL, &event_main},
  {"switch", "switch", NULL},
#ifdef HAVE_MICRO
  {"brctl", "brctl", NULL},
#endif
  {"getbridgeprio", "getbridgeprio", NULL},
  {"setuserpasswd", "setuserpasswd", NULL},
  {"getbridge", "getbridge", NULL},
  {"watchdog", NULL, &watchdog_main},
  {"nvram", NULL, &nvram_main},
#ifdef HAVE_ROAMING
  {"roaming_daemon", NULL, &roaming_daemon_main},
  {"supplicant", "supplicant", NULL},
#endif
  {"get_wanface", NULL, &get_wanface},
#ifndef HAVE_XSCALE
  {"ledtool", NULL, &ledtool_main},
#endif
#ifdef HAVE_REGISTER
  {"regshell", NULL, &reg_main}
#endif
};

int
main (int argc, char **argv)
{
  char *base = strrchr (argv[0], '/');

  base = base ? base + 1 : argv[0];
  int i;
  for (i = 0; i < sizeof (maincalls) / sizeof (struct MAIN); i++)
    {
      if (strstr (base, maincalls[i].callname))
	{
	  if (maincalls[i].execname)
	    return start_main (maincalls[i].execname, argc, argv);
	  if (maincalls[i].exec)
	    return maincalls[i].exec (argc, argv);
	}
    }



  if (strstr (base, "startservice"))
    {
      if (argc < 2)
	{
	  puts ("try to be professional\n");
	  return 0;
	}
      return start_service (argv[1]);
    }
  if (strstr (base, "stopservice"))
    {
      if (argc < 2)
	{
	  puts ("try to be professional\n");
	  return 0;
	}
      return stop_service (argv[1]);
    }

#ifndef HAVE_RB500
#ifndef HAVE_X86
  /* erase [device] */
  if (strstr (base, "erase"))
    {
      int brand = getRouterBrand ();
      if (brand == ROUTER_MOTOROLA || brand == ROUTER_MOTOROLA_V1 || brand == ROUTER_MOTOROLA_WE800G || brand == ROUTER_RT210W || brand == ROUTER_BUFFALO_WZRRSG54)	//these routers have problem erasing nvram, so we only software restore defaults
	{
	  if (argv[1] && strcmp (argv[1], "nvram"))
	    {
	      fprintf (stderr,
		       "Sorry, erasing nvram will get this router unuseable\n");
	      return 0;
	    }
	}
      else
	{
	  if (argv[1])
	    return mtd_erase (argv[1]);
	  else
	    {
	      fprintf (stderr, "usage: erase [device]\n");
	      return EINVAL;
	    }
	}
      return 0;
    }

  /* write [path] [device] */
  if (strstr (base, "write"))
    {
      if (argc >= 3)
	return mtd_write (argv[1], argv[2]);
      else
	{
	  fprintf (stderr, "usage: write [path] [device]\n");
	  return EINVAL;
	}
    }
#else
  if (strstr (base, "erase"))
    {
      if (argv[1] && strcmp (argv[1], "nvram"))
	{
	  fprintf (stderr, "Erasing configuration data...\n");
	  eval ("mount", "/usr/local", "-o", "remount,rw");
	  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
	  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
	  eval ("rm", "-f", "/etc/nvram/*");	// delete nvram database
	  eval ("mount", "/usr/local", "-o", "remount,ro");
	}
      return 0;
    }
#endif
#endif
  /* hotplug [event] */
  if (strstr (base, "hotplug"))
    {
      if (argc >= 2)
	{
	  fprintf (stderr, "hotplug %s\n", argv[1]);
	  if (!strcmp (argv[1], "net"))
	    return start_service ("hotplug_net");
#ifdef HAVE_USBHOTPLUG
	  if (!strcmp (argv[1], "usb"))
	    return start_service ("hotplug_usb");
#endif
#ifdef HAVE_XSCALE
	  if (!strcmp (argv[1], "firmware"))
	    return eval ("/etc/upload", argv[1]);
#endif
	}
      else
	{
	  fprintf (stderr, "usage: hotplug [event]\n");
	  return EINVAL;
	}
      return 0;
    }
  //////////////////////////////////////////////////////
  //
  if (strstr (base, "filtersync"))
    return start_service ("filtersync");
  /* filter [add|del] number */
  if (strstr (base, "filter"))
    {
      if (argv[1] && argv[2])
	{
	  int num = 0;
	  if ((num = atoi (argv[2])) > 0)
	    {
	      if (strcmp (argv[1], "add") == 0)
		return start_servicei ("filter_add", num);
	      else if (strcmp (argv[1], "del") == 0)
		return start_servicei ("filter_del", num);
	    }
	}
      else
	{
	  fprintf (stderr, "usage: filter [add|del] number\n");
	  return EINVAL;
	}
      return 0;
    }

  if (strstr (base, "restart_dns"))
    {
      stop_service ("dnsmasq");
      stop_service ("udhcpd");
      start_service ("udhcpd");
      start_service ("dnsmasq");
      return 0;
    }
  if (strstr (base, "setpasswd"))
    {
      start_service ("mkfiles");
      return 0;
    }
  /* rc [stop|start|restart ] */
  else if (strstr (base, "rc"))
    {
      if (argv[1])
	{
	  if (strncmp (argv[1], "start", 5) == 0)
	    return kill (1, SIGUSR2);
	  else if (strncmp (argv[1], "stop", 4) == 0)
	    return kill (1, SIGINT);
	  else if (strncmp (argv[1], "restart", 7) == 0)
	    return kill (1, SIGHUP);
	}
      else
	{
	  fprintf (stderr, "usage: rc [start|stop|restart]\n");
	  return EINVAL;
	}
    }
  return 1;
}
