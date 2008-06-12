/*
 * wpa.c
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
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <nvparse.h>
#include "snmp.h"
#include <signal.h>
#include <utils.h>
#include <syslog.h>
#include <wlutils.h>


void
start_radius (char *prefix)
{
  char psk[64];
  //  wrt-radauth $IFNAME $server $port $share $override $mackey $maxun &
  char ifname[32];
  strcpy (ifname, prefix);
  if (!strcmp (ifname, "wl0"))
    strcpy (ifname, nvram_safe_get ("wl0_ifname"));
  if (!strcmp (ifname, "wl1"))
    strcpy (ifname, nvram_safe_get ("wl1_ifname"));

  if (nvram_nmatch ("1","%s_radauth", prefix) && nvram_nmatch ("ap","%s_mode", prefix))
    {
      char *server = nvram_nget ("%s_radius_ipaddr", prefix);
      char *port = nvram_nget ("%s_radius_port", prefix);
      char *share = nvram_nget ("%s_radius_key", prefix);
      char type[32];
      sprintf (type, "%s_radmactype", prefix);
      char *pragma = "";
      if (nvram_default_match (type, "0", "0"))
	pragma = "-n1 ";
      if (nvram_match (type, "1"))
	pragma = "-n2 ";
      if (nvram_match (type, "2"))
	pragma = "-n3 ";
      if (nvram_match (type, "3"))
	pragma = "";
      sleep (1);		//some delay is usefull
      sysprintf( "wrt-radauth %s %s %s %s %s %s %s %s &", pragma, ifname,
	       server, port, share, nvram_nget ("%s_radius_override", prefix),
	       nvram_nget ("%s_radmacpassword", prefix),
	       nvram_nget ("%s_max_unauth_users", prefix));
    }


}

int start_nas_single (char *type, char *prefix);

// #define HAVE_NASCONF  //use this to parse nas parameters from conf file. 

static void
convert_wds (int instance)
{
  char wds_mac[254];
  char buf[254];

  if (nvram_nmatch ("", "wl%d_wds", instance))	// For Router, accept all WDS link
    strcpy (wds_mac, "*");
  else				// For AP, assign remote WDS MAC
    strcpy (wds_mac, nvram_nget ("wl%d_wds", instance));

  /* For WPA-PSK mode, we want to convert wl_wds_mac to wl0_wds0 ... wl0_wds255 */
  if (nvram_match ("security_mode", "psk")
      || nvram_match ("security_mode", "psk2"))
    {
      int i = 0;
      int j;
      char mac[254];
      char *next;

      foreach (mac, wds_mac, next)
      {
	snprintf (buf, sizeof (buf), "%s,auto,%s,%s,%s,%s",
		  mac,
		  nvram_safe_get ("wl_crypto"),
		  nvram_safe_get ("security_mode"),
#ifndef HAVE_MSSID
		  nvram_safe_get ("wl_ssid"), nvram_safe_get ("wl_wpa_psk"));
#else
		  nvram_nget ("wl%d_ssid", instance),
		  nvram_safe_get ("wl_wpa_psk"));
#endif
	nvram_nset (buf,"wl%d_wds%d", instance, i);
	i++;
      }

      /* Del unused entry */
      for (j = i; j < MAX_NVPARSE; j++)
	del_wds_wsec (instance, j);
    }
}

#ifdef HAVE_MSSID
int
start_guest_nas (void)
{

/*  char *unbridged_interfaces;
  char *next;
  char name[IFNAMSIZ], lan[IFNAMSIZ];
  int index;

	unbridged_interfaces = nvram_get("unbridged_ifnames");
	
	if (unbridged_interfaces)
		foreach(name,unbridged_interfaces,next){
			index = get_ipconfig_index(name);
			if (index < 0) 
				continue;
			snprintf(lan,sizeof(lan),"lan%d",index);
			start_nas(lan);
		}
*/
  return 0;
}
#endif
char *
getSecMode (char *prefix)
{
  char wep[32];
  char crypto[32];
  sprintf (wep, "%s_wep", prefix);
  sprintf (crypto, "%s_crypto", prefix);
  /* BugBug - should we bail when mode is wep ? */
  if (nvram_match (wep, "wep") || nvram_match (wep, "on")
      || nvram_match (wep, "restricted") || nvram_match (wep, "enabled"))
    return "1";
  else if (nvram_match (crypto, "tkip"))
    return "2";
  else if (nvram_match (crypto, "aes"))
    return "4";
  else if (nvram_match (crypto, "tkip+aes"))
    return "6";
  else
    return "0";
}

char *
getAuthMode (char *prefix)
{
  char akm[32];
  sprintf (akm, "%s_akm", prefix);
  if (strlen (nvram_safe_get (akm)) == 0 || nvram_match (akm, "disabled")
      || nvram_match (akm, "wep"))
    return NULL;
  if (nvram_match (akm, "radius"))
    return "32";
  else if (nvram_match (akm, "wpa"))
    return "2";
  else if (nvram_match (akm, "psk"))
    return "4";
  else if (nvram_match (akm, "psk2"))
    return "128";
  else if (nvram_match (akm, "psk psk2"))
    return "132";
  else if (nvram_match (akm, "wpa2"))
    return "64";
  else if (nvram_match (akm, "wpa wpa2"))
    return "66";
  else
    return "255";
}



char *
getKey (char *prefix)
{
  char akm[32];
  sprintf (akm, "%s_akm", prefix);
  if (nvram_match (akm, "wpa") || nvram_match (akm, "radius")
      || nvram_match (akm, "wpa2") || nvram_match (akm, "wpa wpa2"))
    return nvram_nget ("%s_radius_key", prefix);
  else if (nvram_match (akm, "psk") || nvram_match (akm, "psk2")
	   || nvram_match (akm, "psk psk2"))
    return nvram_nget ("%s_wpa_psk", prefix);
  else
    return "";
}

/*
static void start_nas_ap(char *prefix,char *type)
{
      char sec[32];
      sprintf(sec,"%s_security_mode",prefix);
      int i;
      for (i=0;i<strlen(sec);i++)
        if (sec[i]=='.')sec[i]='X';
	
      char *security_mode = nvram_safe_get (sec);

      if (strstr (security_mode, "psk") || strstr (security_mode, "wpa"))
        {
	char auth[32];
	sprintf(auth,"%s_auth",prefix);
	nvram_set (auth, "0");
	}
      convert_wds ();

      if (!type || !*type)
	{
	  if (nvram_match ("wl0_mode", "ap"))
	    type = "lan";
	  else
	    type = "wan";
	}

      snprintf (cfgfile, sizeof (cfgfile), "/tmp/nas.%s.conf", type);
      snprintf (pidfile, sizeof (pidfile), "/tmp/nas.%s.pid", type);

      {
	char *argv[] = { "/usr/sbin/nas", cfgfile, pidfile, type, NULL };
	pid_t pid;

	_eval (argv, NULL, 0, &pid);
	cprintf ("done\n");
      }
}
*/
void
start_nas_lan (int c)
{
  char wlname[32];
  sprintf (wlname, "wl%d", c);
  start_radius (wlname);	// quick fix, should be vif capable in future
  start_nas_single ("lan", wlname);

#ifdef HAVE_MSSID
  char *next;
  char var[80];
  char *vifs = nvram_nget ("wl%d_vifs", c);
  if (strlen (vifs))
    foreach (var, vifs, next)
    {
      start_nas_single ("lan", var);
    }
#endif
}



void
start_nas_wan (int c)
{
  char wlname[32];
  sprintf (wlname, "wl%d", c);
  start_nas_single ("wan", wlname);

#ifdef HAVE_MSSID
  char *next;
  char var[80];
  char vif[16];
  char *vifs = nvram_nget ("wl%d_vifs", c);
  if (strlen (vifs))
    foreach (var, vifs, next)
    {
      sprintf (vif, "%s_mode", var);
      if (nvram_match (vif, "sta")
	  || nvram_match (vif, "wet")
	  || nvram_match (vif, "apsta") || nvram_match (vif, "apstawet"))
	{
	  start_nas_single ("wan", var);
	}
      else
	{
	  start_nas_single ("lan", var);
	}
    }
#endif
}

#ifdef HAVE_WPA_SUPPLICANT
extern void setupSupplicant (char *prefix);
#endif
int
start_nas (void)
{
#ifdef HAVE_MSSID
  unlink ("/tmp/.nas");
#endif
  int cnt = get_wl_instances ();

  int c;
  for (c = 0; c < cnt; c++)
    {
      if (nvram_nmatch ("disabled", "wl%d_net_mode", c))
	continue;
      char wlname[32];
      sprintf (wlname, "wl%d", c);
      if (nvram_nmatch ("sta", "wl%d_mode", c)
	  || nvram_nmatch ("wet", "wl%d_mode", c)
	  || nvram_nmatch ("apsta", "wl%d_mode", c)
	  || nvram_nmatch ("apstawet", "wl%d_mode", c))
	{
	  cprintf ("start nas wan\n");
#ifdef HAVE_WPA_SUPPLICANT
	  if (nvram_nmatch ("8021X", "wl%d_akm", c)
	      && nvram_nmatch ("sta", "wl%d_mode", c))
	    setupSupplicant (wlname);
	  else
#endif
	    start_nas_wan (c);

	}
      else
	{
	  cprintf ("start nas lan\n");
	  start_nas_lan (c);
	}
    }
  return 1;
}


int
start_nas_single (char *type, char *prefix)
{
#ifdef HAVE_MSSID
  FILE *fnas;
#endif
#ifdef HAVE_NASCONF
  char conffile[64];
  FILE *conf;
#endif
  char pidfile[64];
  char *auth_mode = "255";	/* -m N = WPA authorization mode (N = 0: none, 1: 802.1x, 2: WPA PSK, 255: disabled) */
  char *sec_mode = { 0 };	/* -w N = security mode bitmask  (N = 1: WEP, 2: TKIP, 4: AES) */
  char *key = { 0 }, *iface =
  {
  0}, *mode =
  {
  0};
  if (nvram_nmatch ("disabled", "%s_net_mode", prefix))
    return 0;
  if (!strcmp (prefix, "wl0"))
    convert_wds (0);
  else
    convert_wds (1);
  {

    snprintf (pidfile, sizeof (pidfile), "/tmp/nas.%s%s.pid", prefix, type);
#ifdef HAVE_NASCONF
    snprintf (conffile, sizeof (conffile), "/tmp/nas.%s%s.conf", prefix,
	      type);
#endif
   
    char apmode[32];
    sprintf(apmode,"%s_mode",prefix);
    if (!strcmp (type, "wan") && nvram_match (apmode,"ap"))
      {
	return 0;
      }

//    if (!strcmp (type, "lan"))
//      iface = "br0";
//    else

    if (0 == type || 0 == *type)
      type = "lan";
    if (!strcmp (type, "lan") && nvram_invmatch (apmode, "ap"))
      iface = "br0";
    else
      {

	if (!strcmp (prefix, "wl0"))
	  {
	    iface = get_wl_instance_name (0);
	  }
	else if (!strcmp (prefix, "wl1"))
	  {
	    iface = get_wl_instance_name (1);
	  }
	else
	  {
	    iface = prefix;
	  }
      }

    sec_mode = getSecMode (prefix);
    auth_mode = getAuthMode (prefix);
    if (auth_mode == NULL)
      return 0;			//no nas required
    if (strcmp (nvram_safe_get (apmode), "sta")
	&& strcmp (nvram_safe_get (apmode), "wet")
	&& strcmp (nvram_safe_get (apmode), "apstawet")
	&& strcmp (nvram_safe_get (apmode), "apsta"))
      {
	mode = "-A";
	syslog (LOG_INFO,
		"NAS : NAS lan (%s interface) successfully started\n",
		prefix);
#ifdef HAVE_MSSID
	fnas = fopen ("/tmp/.nas", "a");
	fputc ('L', fnas);	//L as LAN
	fclose (fnas);
#endif
      }
    else
      {
	mode = "-S";
	syslog (LOG_INFO,
		"NAS : NAS wan (%s interface) successfully started\n",
		prefix);
#ifdef HAVE_MSSID
	fnas = fopen ("/tmp/.nas", "a");
	fputc ('W', fnas);	//W as WAN
	fclose (fnas);
#endif
      }

    char rekey[32];
    char ssid[32];
    char radius[32];
    char port[32];
    char index[32];

    sprintf (rekey, "%s_wpa_gtk_rekey", prefix);
    sprintf (ssid, "%s_ssid", prefix);
    sprintf (radius, "%s_radius_ipaddr", prefix);
    sprintf (port, "%s_radius_port", prefix);
    sprintf (index, "%s_key", prefix);

    key = getKey (prefix);

    {
      //char *argv[] = {"nas", "-P", pidfile, "-l", nvram_safe_get("lan_ifname"), "-H", "34954", "-i", iface, mode, "-m", auth_mode, "-k", key, "-s", nvram_safe_get("wl0_ssid"), "-w", sec_mode, "-g", nvram_safe_get("wl0_wpa_gtk_rekey"), "-h", nvram_safe_get("wl0_radius_ipaddr"), "-p", nvram_safe_get("wl0_radius_port"), NULL};
      pid_t pid;
      FILE *fp = { 0 };
      if (!strcmp (mode, "-S"))
	{
#ifndef HAVE_NASCONF
	  char **argv;

	  if (nvram_nmatch ("wet", "%s_mode", prefix)
	      || nvram_nmatch ("apstawet", "%s_mode", prefix))
	    {
	      argv = (char *[])
	      {
	      "nas", "-P", pidfile, "-H", "34954", "-l",
		  nvram_safe_get ("lan_ifname"), "-i", iface, mode, "-m",
		  auth_mode, "-k", key, "-s", nvram_safe_get (ssid), "-w",
		  sec_mode, "-g", nvram_safe_get (rekey), NULL};
	    }
	  else
	    {
	      argv = (char *[])
	      {
	      "nas", "-P", pidfile, "-H", "34954", "-i", iface, mode, "-m",
		  auth_mode, "-k", key, "-s", nvram_safe_get (ssid), "-w",
		  sec_mode, "-g", nvram_safe_get (rekey), NULL};

	    }
	  _evalpid (argv, NULL, 0, &pid);
#else
	  conf = fopen (conffile, "w");
	  fprintf (conf, "-H 34954 -i %s %s -m %s -k %s -s %s -w %s -g %s\n",
		   iface, mode, auth_mode, key, nvram_safe_get (ssid),
		   sec_mode, nvram_safe_get (rekey));
	  fclose (conf);
	  char *argv[] = { "nas", conffile, pidfile, "wan", NULL };
	  _evalpid (argv, NULL, 0, &pid);
#endif
	}
      else
	{
	  if (!strcmp (auth_mode, "2") || !strcmp (auth_mode, "64")
	      || !strcmp (auth_mode, "66"))
	    {
#ifndef HAVE_NASCONF
	      if (nvram_nmatch ("0", "%s_bridged", iface))
		{
		  char *argv[] =
		    { "nas", "-P", pidfile, "-H", "34954", "-i", iface, mode,
		    "-m",
		    auth_mode, "-r", key, "-s", nvram_safe_get (ssid), "-w",
		    sec_mode, "-g", nvram_safe_get (rekey), "-h",
		    nvram_safe_get (radius), "-p", nvram_safe_get (port),	// "-t", //radius rekey time
		    NULL
		  };
		  _evalpid (argv, NULL, 0, &pid);
		}
	      else
		{
		  char *argv[] = { "nas", "-P", pidfile, "-H", "34954", "-l",
		    nvram_safe_get ("lan_ifname"), "-i", iface, mode, "-m",
		    auth_mode, "-r", key, "-s", nvram_safe_get (ssid), "-w",
		    sec_mode, "-g", nvram_safe_get (rekey), "-h",
		    nvram_safe_get (radius), "-p", nvram_safe_get (port),	// "-t", //radius rekey time
		    NULL
		  };
		  _evalpid (argv, NULL, 0, &pid);
		}
#else
	      conf = fopen (conffile, "w");
	      fprintf (conf,
		       "-H 34954 -l %s -i %s %s -m %s -r %s -s %s -w %s -g %s -h %s -p %s\n",
		       nvram_safe_get ("lan_ifname"), iface, mode, auth_mode,
		       key, nvram_safe_get (ssid), sec_mode,
		       nvram_safe_get (rekey), nvram_safe_get (radius),
		       nvram_safe_get (port));
	      fclose (conf);
	      char *argv[] = { "nas", conffile, pidfile, "lan", NULL };
	      _evalpid (argv, NULL, 0, &pid);
#endif
	    }
	  else if (!strcmp (auth_mode, "32"))
	    {
	      int idx = atoi (nvram_safe_get (index));
	      char wepkey[32];
	      sprintf (wepkey, "%s_key%d", prefix, idx);
#ifndef HAVE_NASCONF
	      if (nvram_nmatch ("0", "%s_bridged", iface))
		{
		  char *argv[] =
		    { "nas", "-P", pidfile, "-H", "34954", "-i", iface, mode,
		    "-m",
		    auth_mode, "-r", key, "-s", nvram_safe_get (ssid), "-w",
		    sec_mode, "-I", nvram_safe_get (index), "-k",
		    nvram_safe_get (wepkey), "-h",
		    nvram_safe_get (radius), "-p", nvram_safe_get (port),	// "-t", //radius rekey time
		    NULL
		  };
		  _evalpid (argv, NULL, 0, &pid);
		}
	      else
		{
		  char *argv[] = { "nas", "-P", pidfile, "-H", "34954", "-l",
		    nvram_safe_get ("lan_ifname"), "-i", iface, mode, "-m",
		    auth_mode, "-r", key, "-s", nvram_safe_get (ssid), "-w",
		    sec_mode, "-I", nvram_safe_get (index), "-k",
		    nvram_safe_get (wepkey), "-h",
		    nvram_safe_get (radius), "-p", nvram_safe_get (port),	// "-t", //radius rekey time
		    NULL
		  };

		  _evalpid (argv, NULL, 0, &pid);

		}
#else
	      conf = fopen (conffile, "w");
	      fprintf (conf,
		       "-H 34954 -l %s -i %s %s -m %s -r %s -s %s -w %s -I %s -k %s -h %s -p %s\n",
		       nvram_safe_get ("lan_ifname"), iface, mode, auth_mode,
		       key, nvram_safe_get (ssid), sec_mode,
		       nvram_safe_get (index), nvram_safe_get (wepkey),
		       nvram_safe_get (radius), nvram_safe_get (port));
	      fclose (conf);
	      char *argv[] = { "nas", conffile, pidfile, "lan", NULL };
	      _evalpid (argv, NULL, 0, &pid);
#endif
	    }
	  else
	    {
#ifndef HAVE_NASCONF
	      if (nvram_nmatch ("0", "%s_bridged", iface))
		{
		  char *argv[] =
		    { "nas", "-P", pidfile, "-H", "34954", "-i", iface, mode,
		    "-m",
		    auth_mode, "-k", key, "-s", nvram_safe_get (ssid), "-w",
		    sec_mode, "-g",
		    nvram_safe_get (rekey), NULL
		  };
		  _evalpid (argv, NULL, 0, &pid);
		}
	      else
		{
		  char *argv[] = { "nas", "-P", pidfile, "-H", "34954", "-l",
		    nvram_safe_get ("lan_ifname"), "-i", iface, mode, "-m",
		    auth_mode, "-k", key, "-s", nvram_safe_get (ssid), "-w",
		    sec_mode, "-g",
		    nvram_safe_get (rekey), NULL
		  };
		  _evalpid (argv, NULL, 0, &pid);
		}
#else
	      conf = fopen (conffile, "w");
	      fprintf (conf,
		       "-H 34954 -l %s -i %s %s -m %s -k %s -s %s -w %s -g %s\n",
		       nvram_safe_get ("lan_ifname"), iface, mode, auth_mode,
		       key, nvram_safe_get (ssid), sec_mode,
		       nvram_safe_get (rekey));
	      fclose (conf);
	      char *argv[] = { "nas", conffile, pidfile, "lan", NULL };
	      _evalpid (argv, NULL, 0, &pid);
#endif
	    }


	}


      fp = fopen (pidfile, "w");
      if (fp)
	fprintf (fp, "%d", pid);
      fclose (fp);

      cprintf ("done\n");
    }
    return 0;
  }
}

int
stop_nas (void)
{
  int ret = 0;

#ifdef HAVE_MSSID
  unlink ("/tmp/.nas");
#endif

  if (pidof ("nas") > 0)
    syslog (LOG_INFO, "NAS : NAS daemon successfully stopped\n");

  if (pidof ("wrt-radauth") > 0)
    {
      syslog (LOG_INFO, "RADAUTH : RADAUTH daemon successfully stopped\n");
      killall ("wrt-radauth", SIGKILL);
    }

  int deadcount = 0;
  while (pidof ("nas") > 0 && (deadcount++) < 8)
    {
      /* NAS sometimes won't exit properly on a normal kill */
      //int ret = killps("nas",NULL);
      ret = killall ("nas", SIGTERM);
      sleep (2);
      //killps("nas","-9");
      killall ("nas", SIGKILL);
    }
#ifdef HAVE_WPA_SUPPLICANT
  killall ("wpa_supplicant", SIGKILL);
#endif
// clean
  unlink ("/tmp/nas.wl0wan.pid");
  unlink ("/tmp/nas.wl0lan.pid");
  unlink ("/tmp/nas.wl1wan.pid");
  unlink ("/tmp/nas.wl1lan.pid");
#ifdef HAVE_NASCONF
  unlink ("/tmp/nas.wl0wan.conf");
  unlink ("/tmp/nas.wl0lan.conf");
  unlink ("/tmp/nas.wl1wan.conf");
  unlink ("/tmp/nas.wl1lan.conf");
#endif
#ifdef HAVE_MSSID
  unlink ("/tmp/nas.wl0.1lan.pid");
  unlink ("/tmp/nas.wl0.2lan.pid");
  unlink ("/tmp/nas.wl0.3lan.pid");
  unlink ("/tmp/nas.wl1.1lan.pid");
  unlink ("/tmp/nas.wl1.2lan.pid");
  unlink ("/tmp/nas.wl1.3lan.pid");
#ifdef HAVE_NASCONF
  unlink ("/tmp/nas.wl0.1lan.conf");
  unlink ("/tmp/nas.wl0.2lan.conf");
  unlink ("/tmp/nas.wl0.3lan.conf");
  unlink ("/tmp/nas.wl1.1lan.conf");
  unlink ("/tmp/nas.wl1.2lan.conf");
  unlink ("/tmp/nas.wl1.3lan.conf");
#endif
#endif

  cprintf ("done\n");
  return ret;
}
