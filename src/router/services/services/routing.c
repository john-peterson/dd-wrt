#include <stdlib.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <wlutils.h>

#ifdef HAVE_ZEBRA
int
zebra_init (void)
{
  if (nvram_match ("wk_mode", "gateway"))
    {
      printf ("zebra disabled.\n");
      return 0;
    }
  else if (nvram_match ("wk_mode", "ospf"))
    {
      return zebra_ospf_init ();
      syslog (LOG_INFO, "zebra : zebra (ospf) successfully initiated\n");
    }
  else if (nvram_match ("wk_mode", "router"))
    {
      return zebra_ripd_init ();
      syslog (LOG_INFO, "zebra : zebra (router) successfully initiated\n");
    }
  else
    return 0;
}


int
zebra_ospf_init (void)
{

  char *lt = nvram_safe_get ("dr_lan_tx");
  char *lr = nvram_safe_get ("dr_lan_rx");
  char *wt = nvram_safe_get ("dr_wan_tx");
  char *wr = nvram_safe_get ("dr_wan_rx");
  char *lf = nvram_safe_get ("lan_ifname");
  char *wf = get_wan_face ();

  FILE *fp;
  int ret1, ret2, s = 0;

//      printf("Start zebra\n");
  if (!strcmp (lt, "0") && !strcmp (lr, "0") &&
      !strcmp (wt, "0") && !strcmp (wr, "0"))
    {
      printf ("zebra disabled.\n");
      return 0;
    }

  /* Write configuration file based on current information */
  if (!(fp = fopen ("/tmp/zebra.conf", "w")))
    {
      perror ("/tmp/zebra.conf");
      return errno;
    }

  if (strlen (nvram_safe_get ("zebra_conf")) < 1
      || nvram_match ("zebra_copt", "1"))
    {
      if (nvram_match ("zebra_log", "1"))
	{
	  fprintf (fp, "log file /var/log/zebra.log\n");
	}
    }

  if (strlen (nvram_safe_get ("zebra_conf")) > 0)
    {
      fprintf (fp, "%s", nvram_safe_get ("zebra_conf"));
    }

  fclose (fp);

  if (!(fp = fopen ("/tmp/ospfd.conf", "w")))
    {
      perror ("/tmp/ospfd.conf");
      return errno;
    }

  if (strlen (nvram_safe_get ("ospfd_conf")) < 1
      || nvram_match ("ospfd_copt", "1"))
    {
      fprintf (fp, "!\n");
      fprintf (fp, "password %s\n", nvram_safe_get ("http_passwd"));
      fprintf (fp, "enable password %s\n", nvram_safe_get ("http_passwd"));
      fprintf (fp, "!\n!\n!\n");

      fprintf (fp, "interface %s\n!\n", lf);
      fprintf (fp, "interface %s\n", wf);
      fprintf (fp, "passive interface lo\n");

      if (nvram_match ("wl_br1_enable", "1"))
	{
	  fprintf (fp, "!\n! 'Subnet' WDS bridge\n");
	  fprintf (fp, "interface br1\n");
	}

      if (nvram_match ("wl0_mode", "ap"))
	for (s = 1; s <= MAX_WDS_DEVS; s++)
	  {
	    char wdsvarname[32] = { 0 };
	    char wdsdevname[32] = { 0 };
	    char wdsdevlabel[32] = { 0 };
	    char wdsdevospf[32] = { 0 };
	    char *dev;

	    sprintf (wdsvarname, "wl_wds%d_enable", s);
	    sprintf (wdsdevname, "wl_wds%d_if", s);
	    sprintf (wdsdevlabel, "wl_wds%d_desc", s);
	    sprintf (wdsdevospf, "wl_wds%d_ospf", s);
	    dev = nvram_safe_get (wdsdevname);

	    if (nvram_match (wdsvarname, "1"))
	      {
		fprintf (fp, "!\n! WDS: %s\n", nvram_safe_get (wdsdevlabel));
		fprintf (fp, "interface %s\n", dev);

		if (atoi (nvram_safe_get (wdsdevospf)) > 0)
		  fprintf (fp, " ip ospf cost %s\n",
			   nvram_safe_get (wdsdevospf));
	      }
	  }

      fprintf (fp, "!\n");
      fprintf (fp, "router ospf\n");
      fprintf (fp, " ospf router-id %s\n", nvram_safe_get ("lan_ipaddr"));
      fprintf (fp, " redistribute kernel\n");
      fprintf (fp, " redistribute connected\n");
      fprintf (fp, " redistribute static\n");
      fprintf (fp, " network 0.0.0.0/0 area 0\n");	// handle all routing
      fprintf (fp, " default-information originate\n");

      for (s = 1; s <= MAX_WDS_DEVS; s++)
	{
	  char wdsdevospf[32] = { 0 };
	  char wdsdevname[32] = { 0 };

	  sprintf (wdsdevname, "wl_wds%d_if", s);
	  sprintf (wdsdevospf, "wl_wds%d_ospf", s);

	  if (atoi (nvram_safe_get (wdsdevospf)) < 0)
	    fprintf (fp, " passive-interface %s\n",
		     nvram_safe_get (wdsdevname));
	}

      if (nvram_match ("zebra_log", "1"))
	{
	  fprintf (fp, "!\n");
	  fprintf (fp, "log file /var/log/ospf.log\n");
	}

      fprintf (fp, "!\nline vty\n!\n");
    }

  if (strlen (nvram_safe_get ("ospfd_conf")) > 0)
    {
      fprintf (fp, "%s", nvram_safe_get ("ospfd_conf"));
    }

  fflush (fp);
  fclose (fp);

  if (nvram_match ("dyn_default", "1"))
    while (!eval ("/usr/sbin/ip", "route", "del", "default"))
      ;

  ret1 = eval ("/usr/sbin/zebra", "-d", "-f", "/tmp/zebra.conf");
  ret2 = eval ("/usr/sbin/ospfd", "-d", "-f", "/tmp/ospfd.conf");

  return ret1 + ret2;
}

int
zebra_ripd_init (void)
{

  char *lt = nvram_safe_get ("dr_lan_tx");
  char *lr = nvram_safe_get ("dr_lan_rx");
  char *wt = nvram_safe_get ("dr_wan_tx");
  char *wr = nvram_safe_get ("dr_wan_rx");
  char *lf = nvram_safe_get ("lan_ifname");
  char *wf = get_wan_face ();

  FILE *fp;
  int ret1, ret2;

//      printf("Start zebra\n");
  if (!strcmp (lt, "0") && !strcmp (lr, "0") &&
      !strcmp (wt, "0") && !strcmp (wr, "0"))
    {
      printf ("zebra disabled.\n");
      return 0;
    }

  /* Write configuration file based on current information */
  if (!(fp = fopen ("/tmp/zebra.conf", "w")))
    {
      perror ("/tmp/zebra.conf");
      return errno;
    }
  fclose (fp);

  if (!(fp = fopen ("/tmp/ripd.conf", "w")))
    {
      perror ("/tmp/ripd.conf");
      return errno;
    }
  fprintf (fp, "router rip\n");
  fprintf (fp, "  network %s\n", lf);
  fprintf (fp, "  network %s\n", wf);
  fprintf (fp, "redistribute connected\n");
  //fprintf(fp, "redistribute kernel\n");
  //fprintf(fp, "redistribute static\n");

  fprintf (fp, "interface %s\n", lf);
  if (strcmp (lt, "0") != 0)
    fprintf (fp, "  ip rip send version %s\n", lt);
  if (strcmp (lr, "0") != 0)
    fprintf (fp, "  ip rip receive version %s\n", lr);

  fprintf (fp, "interface %s\n", wf);
  if (strcmp (wt, "0") != 0)
    fprintf (fp, "  ip rip send version %s\n", wt);
  if (strcmp (wr, "0") != 0)
    fprintf (fp, "  ip rip receive version %s\n", wr);

  fprintf (fp, "router rip\n");
  if (strcmp (lt, "0") == 0)
    fprintf (fp, "  distribute-list private out %s\n", lf);
  if (strcmp (lr, "0") == 0)
    fprintf (fp, "  distribute-list private in  %s\n", lf);
  if (strcmp (wt, "0") == 0)
    fprintf (fp, "  distribute-list private out %s\n", wf);
  if (strcmp (wr, "0") == 0)
    fprintf (fp, "  distribute-list private in  %s\n", wf);
  fprintf (fp, "access-list private deny any\n");

  //fprintf(fp, "debug rip events\n");
  //fprintf(fp, "log file /tmp/ripd.log\n");
  fflush (fp);
  fclose (fp);

  ret1 = eval ("zebra", "-d", "-f", "/tmp/zebra.conf");
  ret2 = eval ("ripd", "-d", "-f", "/tmp/ripd.conf");

  return ret1 + ret2;
}
#endif

#ifdef HAVE_BIRD
int
bird_init (void)
{
  FILE *fp;
  int ret1;
  /*
   * compatibitly for old nvram style (site needs to be enhanced)
   */
  if (nvram_match ("wk_mode", "gateway"))
    return 0;
  nvram_set ("routing_ospf", "off");
  nvram_set ("routing_bgp", "off");
  nvram_set ("routing_rip2", "off");

  if (nvram_match ("wk_mode", "ospf"))
    nvram_set ("routing_ospf", "on");
  if (nvram_match ("wk_mode", "router"))
    nvram_set ("routing_rip2", "on");
  if (nvram_match ("wk_mode", "bgp"))
    nvram_set ("routing_bgp", "on");

  if (nvram_match ("dr_setting", "1"))
    {
      nvram_set ("routing_wan", "on");
      nvram_set ("routing_lan", "off");
    }
  if (nvram_match ("dr_setting", "2"))
    {
      nvram_set ("routing_wan", "off");
      nvram_set ("routing_lan", "on");
    }
  if (nvram_match ("dr_setting", "3"))
    {
      nvram_set ("routing_wan", "on");
      nvram_set ("routing_lan", "on");
    }
  if (nvram_match ("dr_setting", "0"))
    {
      nvram_set ("routing_wan", "off");
      nvram_set ("routing_lan", "off");
    }

  // DD-WRT bird support 
  if (nvram_match ("routing_rip2", "on") ||
      nvram_match ("routing_ospf", "on") || nvram_match ("routing_bgp", "on"))
    {
      mkdir ("/tmp/bird", 0744);
      if (!(fp = fopen ("/tmp/bird/bird.conf", "w")))
	{
	  perror ("/tmp/bird/bird.conf");
	  return errno;
	}
      fprintf (fp, "router id %s;\n", nvram_safe_get ("lan_ipaddr"));
      fprintf (fp,
	       "protocol kernel { learn; persist; scan time 10; import all; export all; }\n");
      fprintf (fp, "protocol device { scan time 10; } \n");
      fprintf (fp, "protocol direct { interface \"*\";}\n");

      if (nvram_match ("routing_rip2", "on"))
	{

	  fprintf (fp, "protocol rip WRT54G_rip {\n");
	  if (nvram_match ("routing_lan", "on"))
	    fprintf (fp, "	interface \"%s\" { };\n",
		     nvram_safe_get ("lan_ifname"));
	  if (nvram_match ("routing_wan", "on"))
	    {
	      if (nvram_match ("wl_mode", "sta")
		  || nvram_match ("wl_mode", "apsta"))
		fprintf (fp, "	interface \"%s\" { };\n", get_wdev ());
	      else
		fprintf (fp, "	interface \"%s\" { };\n",
			 nvram_safe_get ("wan_ifname"));

	    }
	  fprintf (fp, "	honor always;\n");
	  fprintf (fp,
		   "	import filter { print \"importing\"; accept; };\n");
	  fprintf (fp,
		   "	export filter { print \"exporting\"; accept; };\n");
	  fprintf (fp, "}\n");

	}
      if (nvram_match ("routing_ospf", "on"))
	{
	  fprintf (fp, "protocol ospf WRT54G_ospf {\n");
	  fprintf (fp, "area 0 {\n");
	  if (nvram_match ("routing_wan", "on"))
	    fprintf (fp,
		     "interface \"%s\" { cost 1; authentication simple; password \"%s\"; };\n",
		     nvram_safe_get ("wan_ifname"),
		     nvram_safe_get ("http_passwd"));
	  if (nvram_match ("routing_lan", "on"))
	    fprintf (fp,
		     "interface \"%s\" { cost 1; authentication simple; password \"%s\"; };\n",
		     nvram_safe_get ("lan_ifname"),
		     nvram_safe_get ("http_passwd"));
	  fprintf (fp, "};\n}\n");
	}			// if wk_mode = ospf

      if (nvram_match ("routing_bgp", "on"))
	{
	  fprintf (fp, "protocol bgp {\n");
	  fprintf (fp, "local as %s;\n", nvram_safe_get ("routing_bgp_as"));
	  fprintf (fp, "neighbor %s as %s;\n",
		   nvram_safe_get ("routing_bgp_neighbor_ip"),
		   nvram_safe_get ("routing_bgp_neighbor_as"));
	  fprintf (fp, "export all;\n");
	  fprintf (fp, "import all;\n");
	  fprintf (fp, "}\n");
	}
      fflush (fp);
      fclose (fp);

      ret1 = eval ("bird", "-c", "/tmp/bird/bird.conf");
      syslog (LOG_INFO, "bird : bird daemon successfully started\n");
    }
  return 0;

}
#endif /* HAVE_BIRD */
#ifdef HAVE_BIRD
/* Written by Sparq in 2002/07/16 */
int
start_zebra (void)
{

  // Sveasoft 2003-12-15 only start if enabled
  if (!nvram_invmatch ("zebra_enable", "0"))
    return 0;
//      else if ( nvram_invmatch("wk_mode", "ospf") &&
//                !strcmp(lt, "0") && !strcmp(lr, "0") &&
//                !strcmp(wt, "0") && !strcmp(wr, "0") ){
//              printf("routing disabled.\n");
//              return 0;
//      }

#ifdef HAVE_BIRD

  if (bird_init () != 0)
    return -1;

#elif defined(HAVE_ZEBRA)

  if (zebra_init () != 0)
    return -1;

#endif /* HAVE_BIRD */
  return 0;
}

/* Written by Sparq in 2002/07/16 */
int
stop_zebra (void)
{
  int ret1;
  if (pidof ("zebra") > 0 || pidof ("ripd") > 0 || pidof ("ospfd") > 0)
    syslog (LOG_INFO,
	    "zebra : zebra (ripd and ospfd) daemon successfully stopped\n");
#ifdef HAVE_ZEBRA
  int ret2, ret3;
  ret1 = killall ("zebra", SIGTERM);
  ret2 = killall ("ripd", SIGTERM);
  ret3 = killall ("ospfd", SIGTERM);

  while (!
	 (killall ("zebra", SIGTERM) && killall ("ripd", SIGTERM)
	  && killall ("ospfd", SIGTERM)))
    sleep (1);

  cprintf ("done\n");
  return ret1 | ret2 | ret3;

#elif defined(HAVE_BIRD)
  if (pidof ("bird") > 0)
    syslog (LOG_INFO, "bird : bird daemon successfully stopped\n");
  ret1 = killall ("bird", SIGTERM);

  cprintf ("done\n");
  return ret1;
#else
  return -1;
#endif
}

#endif
