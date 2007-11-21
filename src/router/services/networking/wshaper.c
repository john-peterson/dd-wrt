/* wshaper.c
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
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wait.h>

#include <bcmdevs.h>
#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <utils.h>
#include <cy_conf.h>
#include <code_pattern.h>
#include <rc.h>


void
system_debug (char *sys)
{
  cprintf ("%s\n", sys);
  system2 (sys);
}

#define system(a) system_debug(a)

char *
get_wshaper_dev (void)
{
  if (nvram_match ("wshaper_dev", "WAN"))
    return get_wan_face ();
  else
    return "br0";
}

static char *
get_mtu_val (void)
{
  if (nvram_match ("wshaper_dev", "WAN")
      && nvram_match ("wan_proto", "pppoe"))
    return nvram_safe_get ("ppp_mtu");
  else if (nvram_match ("wshaper_dev", "WAN"))
    return nvram_safe_get ("wan_mtu");
  else
    return "1500";
}

#ifdef HAVE_SVQOS
void
svqos_reset_ports (void)
{
/*
system ("echo 1 > /proc/sys/dev/adm6996/port1/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/sys/dev/adm6996/port2/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/sys/dev/adm6996/port3/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/sys/dev/adm6996/port4/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/switch/eth0/port/1/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/switch/eth0/port/2/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/switch/eth0/port/3/enable 2>&1 > /dev/null");
  system ("echo 1 > /proc/switch/eth0/port/4/enable 2>&1 > /dev/null");

  system
    ("echo 0 > /proc/sys/dev/adm6996/port1/port-prio-enable 2>&1 > /dev/null");
  system
    ("echo 0 > /proc/sys/dev/adm6996/port2/port-prio-enable 2>&1 > /dev/null");
  system
    ("echo 0 > /proc/sys/dev/adm6996/port3/port-prio-enable 2>&1 > /dev/null");
  system
    ("echo 0 > /proc/sys/dev/adm6996/port4/port-prio-enable 2>&1 > /dev/null");

  system ("echo 100 > /proc/sys/dev/adm6996/port1/speed 2>&1 > /dev/null");
  system ("echo 100 > /proc/sys/dev/adm6996/port2/speed 2>&1 > /dev/null");
  system ("echo 100 > /proc/sys/dev/adm6996/port3/speed 2>&1 > /dev/null");
  system ("echo 100 > /proc/sys/dev/adm6996/port4/speed 2>&1 > /dev/null");

  system
    ("echo full > /proc/sys/dev/adm6996/port1/bandwidth 2>&1 > /dev/null");
  system
    ("echo full > /proc/sys/dev/adm6996/port2/bandwidth 2>&1 > /dev/null");
  system
    ("echo full > /proc/sys/dev/adm6996/port3/bandwidth 2>&1 > /dev/null");
  system
    ("echo full > /proc/sys/dev/adm6996/port4/bandwidth 2>&1 > /dev/null");
*/
#ifndef HAVE_XSCALE
#ifndef HAVE_MAGICBOX
#ifndef HAVE_FONERA
#ifndef HAVE_LS2
#ifndef HAVE_LS5
#ifndef HAVE_X86
#ifndef HAVE_WHRAG108
#ifndef HAVE_CA8
#ifndef HAVE_PB42
#ifndef HAVE_TW6600
  if (nvram_match ("portprio_support", "1"))
    {
      system2 ("echo 1 > /proc/switch/eth0/port/1/enable 2>&1 > /dev/null");
      system2 ("echo 1 > /proc/switch/eth0/port/2/enable 2>&1 > /dev/null");
      system2 ("echo 1 > /proc/switch/eth0/port/3/enable 2>&1 > /dev/null");
      system2 ("echo 1 > /proc/switch/eth0/port/4/enable 2>&1 > /dev/null");

      system2
	("echo 0 > /proc/switch/eth0/port/1/prio-enable 2>&1 > /dev/null");
      system2
	("echo 0 > /proc/switch/eth0/port/2/prio-enable 2>&1 > /dev/null");
      system2
	("echo 0 > /proc/switch/eth0/port/3/prio-enable 2>&1 > /dev/null");
      system2
	("echo 0 > /proc/switch/eth0/port/4/prio-enable 2>&1 > /dev/null");


      system2 ("echo AUTO > /proc/switch/eth0/port/1/media 2>&1 > /dev/null");
      system2 ("echo AUTO > /proc/switch/eth0/port/2/media 2>&1 > /dev/null");
      system2 ("echo AUTO > /proc/switch/eth0/port/3/media 2>&1 > /dev/null");
      system2 ("echo AUTO > /proc/switch/eth0/port/4/media 2>&1 > /dev/null");

      system2
	("echo FULL > /proc/switch/eth0/port/1/bandwidth 2>&1 > /dev/null");
      system2
	("echo FULL > /proc/switch/eth0/port/2/bandwidth 2>&1 > /dev/null");
      system2
	("echo FULL > /proc/switch/eth0/port/3/bandwidth 2>&1 > /dev/null");
      system2
	("echo FULL > /proc/switch/eth0/port/4/bandwidth 2>&1 > /dev/null");
    }
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
}

int
svqos_set_ports (void)
{
#ifndef HAVE_XSCALE
#ifndef HAVE_MAGICBOX
#ifndef HAVE_FONERA
#ifndef HAVE_LS2
#ifndef HAVE_LS5
#ifndef HAVE_WHRAG108
#ifndef HAVE_CA8
#ifndef HAVE_X86
#ifndef HAVE_TW6600
#ifndef HAVE_PB42
  if (nvram_match ("portprio_support", "1"))
    {
      int loop = 1;
      char cmd[255] = { 0 }, nvram_var[32] =
      {
      0}, *level;

      svqos_reset_ports ();

      for (loop = 1; loop < 5; loop++)
	{
	  snprintf (nvram_var, 31, "svqos_port%dbw", loop);

	  if (strcmp ("0", nvram_safe_get (nvram_var)))
	    snprintf (cmd, 254,
		      "echo %s > /proc/switch/eth0/port/%d/bandwidth 2>&1 > /dev/null",
		      nvram_safe_get (nvram_var), loop);
	  else
	    snprintf (cmd, 254, "echo 0 > /proc/switch/eth0/port/%d/enable",
		      loop);
	  system2 (cmd);

	  snprintf (cmd, 254,
		    "echo 1 > /proc/switch/eth0/port/%d/prio-enable 2>&1 > /dev/null",
		    loop);
	  system2 (cmd);

	  snprintf (nvram_var, 31, "svqos_port%dprio", loop);
	  level = nvram_safe_get (nvram_var);
	  snprintf (cmd, 254,
		    "echo %d > /proc/switch/eth0/port/%d/prio 2>&1 > /dev/null",
		    atoi (level) / 10 - 1, loop);
	  system2 (cmd);
	}
    }
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

  return 0;
}

#ifdef HAVE_AQOS

extern void add_userip (char *ip, int idx, char *upstream, char *downstream);
extern void add_usermac (char *mac, int idx, char *upstream,
			 char *downstream);

void
aqos_tables (void)
{
  char *qos_ipaddr = nvram_safe_get ("svqos_ips");
  char *qos_mac = nvram_safe_get ("svqos_macs");
  FILE *outips;
  FILE *outmacs;
  outips = fopen ("/tmp/aqos_ips", "wb");
  outmacs = fopen ("/tmp/aqos_macs", "wb");
  char level[32], level2[32], data[32];
  int qosidx = 0;
  do
    {
      if (sscanf (qos_mac, "%31s %31s %31s |", data, level, level2) < 2)
	break;
      fprintf (outmacs, "%s\n", data);
      add_usermac (data, qosidx, level, level2);
      qosidx += 2;


    }
  while ((qos_mac = strpbrk (++qos_mac, "|")) && qos_mac++);
  do
    {
      if (sscanf (qos_ipaddr, "%31s %31s %31s |", data, level, level2) < 2)
	break;
      fprintf (outips, "%s\n", data);

      add_userip (data, qosidx, level, level2);
      qosidx += 2;

    }
  while ((qos_ipaddr = strpbrk (++qos_ipaddr, "|")) && qos_ipaddr++);
  fclose (outips);
  fclose (outmacs);


}


#endif



int
svqos_iptables (void)
{
  char *qos_svcs = nvram_safe_get ("svqos_svcs");
  char *qos_ipaddr = nvram_safe_get ("svqos_ips");
  char *qos_mac = nvram_safe_get ("svqos_macs");
  char name[32], type[32], data[32], level[32], level2[32];;
  char cmd[1024];
  int ilevel, ilevel2;
  char *dev = get_wshaper_dev ();

  system2 ("/usr/sbin/iptables -t mangle -F SVQOS_OUT");
  system2 ("/usr/sbin/iptables -t mangle -X SVQOS_OUT");
  system2 ("/usr/sbin/iptables -t mangle -N SVQOS_OUT");

  system2 ("/usr/sbin/iptables -t mangle -F SVQOS_IN");
  system2 ("/usr/sbin/iptables -t mangle -X SVQOS_IN");
  system2 ("/usr/sbin/iptables -t mangle -N SVQOS_IN");


  snprintf (cmd, 1023,
	    "/usr/sbin/iptables -t mangle -D PREROUTING -i %s -j SVQOS_IN",
	    dev);
  system2 (cmd);

  snprintf (cmd, 1023,
	    "/usr/sbin/iptables -t mangle -I PREROUTING -i %s -j SVQOS_IN",
	    dev);
  system2 (cmd);

  // enable IMQ device for ingress policing
  eval("insmod","imq");
  eval("insmod","ipt_IMQ");
  snprintf (cmd, 1023,
	    "/usr/sbin/iptables -t mangle -D PREROUTING -i %s -j IMQ --todev 0",
	    dev);
  system2 (cmd);

  snprintf (cmd, 1023,
	    "/usr/sbin/iptables -t mangle -I PREROUTING -i %s -j IMQ --todev 0",
	    dev);
  system2 (cmd);

  snprintf (cmd, 1023,
	    "/usr/sbin/iptables -t mangle -D POSTROUTING -o %s -j SVQOS_OUT",
	    dev);
  system2 (cmd);

  snprintf (cmd, 1023,
	    "/usr/sbin/iptables -t mangle -I POSTROUTING -o %s -j SVQOS_OUT",
	    dev);
  system2 (cmd);
eval("insmod","ipt_mark");
eval("insmod","ipt_CONNMARK");
  system2
    ("/usr/sbin/iptables -t mangle -A SVQOS_OUT -j CONNMARK --restore-mark");
  system2
    ("/usr/sbin/iptables -t mangle -A SVQOS_OUT -m mark ! --mark 0 -j RETURN");

  system2
    ("/usr/sbin/iptables -t mangle -A SVQOS_IN -j CONNMARK --restore-mark");
  system2
    ("/usr/sbin/iptables -t mangle -A SVQOS_IN -m mark ! --mark 0 -j RETURN");

  // if OSPF is active put it into the Express bucket for outgoing QoS
  if (nvram_match ("wk_mode", "ospf"))
    system2
      ("/usr/sbin/iptables -t mangle -A SVQOS_OUT -p ospf -m mark --mark 0 -j MARK --set-mark 20");

  // non-TCP and TCP ACK packets are all 1:10 for ingress policing
  //system("/usr/sbin/iptables -t mangle -A SVQOS_IN -p ! tcp -m mark --mark 0 -j MARK --set-mark 10 2>&1 > /dev/null");
  //system("/usr/sbin/iptables -t mangle -A SVQOS_IN -m length --length :64 -m mark --mark 0 -j CLASSIFY --set-class 1:10 2>&1 > /dev/null");
  //system("/usr/sbin/iptables -t mangle -A POSTROUTING -m length --length 0:64 -m mark --mark 0 -j CLASSIFY --set-class 1:10 2>&1 > /dev/null");

  /* mac format is "mac level | mac level |" ..etc */
#ifndef HAVE_AQOS
  char *wl0mode = nvram_get ("wl0_mode");
  if (wl0mode == NULL)
    wl0mode = "";

  if (strcmp (dev, "br0") && strcmp (wl0mode, "wet")
      && strcmp (wl0mode, "apstawet"))
    {
      eval ("rmmod", "ebt_dnat");
      eval ("rmmod", "ebt_snat");
      eval ("rmmod", "ebt_mark_m");
      eval ("rmmod", "ebt_mark");
      eval ("rmmod", "ebtable_filter");
      eval ("rmmod", "ebtable_nat");
      eval ("rmmod", "ebtables");
    }
  else
    {
      eval ("insmod", "ebtables");
      eval ("insmod", "ebtable_nat");
      eval ("insmod", "ebtable_filter");
      eval ("insmod", "ebt_mark");
      eval ("insmod", "ebt_mark_m");
      eval ("insmod", "ebt_snat");
      eval ("insmod", "ebt_dnat");
    }
#endif
#ifndef HAVE_AQOS
  do
    {
      if (sscanf (qos_mac, "%31s %31s |", data, level) < 2)
	break;
      if (strcmp (dev, "br0"))
	{
          eval("insmod","ipt_mac");
	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -D PREROUTING -m mac --mac-source %s -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A PREROUTING -m mac --mac-source %s -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -D PREROUTING -j CONNMARK --save-mark");
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A PREROUTING -j CONNMARK --save-mark");
	  system2 (cmd);
	}
      else
	{
#ifndef HAVE_EBTABLES
	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t nat -D PREROUTING -s %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t nat -A PREROUTING -s %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t nat -D POSTROUTING -d %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t nat -A POSTROUTING -d %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);
#else


	  snprintf (cmd, 1023,
		    "/usr/sbin/ebtables -t nat -D PREROUTING -s %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/ebtables -t nat -A PREROUTING -s %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/ebtables -t nat -D POSTROUTING -d %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/ebtables -t nat -A POSTROUTING -d %s -j mark --set-mark %s",
		    data, level);
	  system2 (cmd);
#endif
	}

    }
  while ((qos_mac = strpbrk (++qos_mac, "|")) && qos_mac++);
#endif

  /* ipaddr format is "ipaddr level | ipaddr level |" ..etc */
#ifndef HAVE_AQOS
  do
    {

      if (sscanf (qos_ipaddr, "%31s %31s |", data, level) < 2)
	break;
      snprintf (cmd, 1023,
		"/usr/sbin/iptables -t mangle -A SVQOS_OUT -s %s -m mark --mark 0 -j MARK --set-mark %s",
		data, level);
      system2 (cmd);

      snprintf (cmd, 1023,
		"/usr/sbin/iptables -t mangle -A SVQOS_OUT -d %s -m mark --mark 0 -j MARK --set-mark %s",
		data, level);
      system2 (cmd);

      snprintf (cmd, 1023,
		"/usr/sbin/iptables -t mangle -A SVQOS_IN -s %s -m mark --mark 0 -j MARK --set-mark %s",
		data, level);
      system2 (cmd);

      snprintf (cmd, 1023,
		"/usr/sbin/iptables -t mangle -A SVQOS_IN -d %s -m mark --mark 0 -j MARK --set-mark %s",
		data, level);
      system2 (cmd);


    }
  while ((qos_ipaddr = strpbrk (++qos_ipaddr, "|")) && qos_ipaddr++);
#endif

  /* services format is "name type data level | name type data level |" ..etc */
  do
    {

      if (sscanf (qos_svcs, "%31s %31s %31s %31s ", name, type, data, level) <
	  4)
	break;

      // udp is managed on egress only
      if (strstr (type, "udp") || strstr (type, "both"))
	{
	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_OUT -p udp -m udp --dport %s -m mark --mark 0 -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_OUT -p udp -m udp --sport %s -m mark --mark 0 -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);
	}

      // tcp and L7 is managed on both ingress and egress
      if (strstr (type, "tcp") || strstr (type, "both"))
	{
	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_OUT -p tcp -m tcp --dport %s -m mark --mark 0 -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_OUT -p tcp -m tcp --sport %s -m mark --mark 0 -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_IN -p tcp -m tcp --dport %s -m mark --mark 0 -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_IN -p tcp -m tcp --sport %s -m mark --mark 0 -j MARK --set-mark %s",
		    data, level);
	  system2 (cmd);
	}

      if (strstr (type, "l7"))
	{
	  eval("insmod","ipt_layer7");
	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_OUT -m layer7 --l7proto %s -m mark --mark 0 -j MARK --set-mark %s",
		    name, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_IN -m layer7 --l7proto %s -m mark --mark 0 -j MARK --set-mark %s",
		    name, level);
	  system2 (cmd);
	}

      if (strstr (type, "p2p"))
	{

	  char *proto = NULL;
	  char *realname = name;

	  if (!strcasecmp (realname, "gnutella"))
	    proto = "gnu";
	  else if (!strcasecmp (realname, "bearshare"))
	    proto = "gnu";
	  else if (!strcasecmp (realname, "edonkey"))
	    proto = "edk";
	  else if (!strcasecmp (realname, "kazaa"))
	    proto = "kazaa";
	  else if (!strcasecmp (realname, "directconnect"))
	    proto = "dc";
	  else if (!strcasecmp (realname, "bittorrent"))
	    proto = "bit";
	  else if (!strcasecmp (realname, "applejuice"))
	    proto = "apple";
	  else if (!strcasecmp (realname, "soulseek"))
	    proto = "soul";
	  else if (!strcasecmp (realname, "ares"))
	    proto = "ares";
	  else if (!strcasecmp (realname, "mute"))
	    proto = "mute";
	  else if (!strcasecmp (realname, "waste"))
	    proto = "waste";
	  else if (!strcasecmp (realname, "xdcc"))
	    proto = "xdcc";

	    eval("insmod","ipt_ipp2p");
	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_OUT -p tcp -m mark --mark 0 -m ipp2p --%s -j MARK --set-mark %s",
		    proto, level);
	  system2 (cmd);

	  snprintf (cmd, 1023,
		    "/usr/sbin/iptables -t mangle -A SVQOS_IN -p tcp -m mark --mark 0 -m ipp2p --%s -j MARK --set-mark %s",
		    proto, level);
	  system2 (cmd);
	}


    }
  while ((qos_svcs = strpbrk (++qos_svcs, "|")) && qos_svcs++);

  // set port priority and port bandwidth
  svqos_set_ports ();

  system2
    ("/usr/sbin/iptables -t mangle -A SVQOS_OUT -j CONNMARK --save-mark");
  system2 ("/usr/sbin/iptables -t mangle -A SVQOS_OUT -j RETURN");

  system2
    ("/usr/sbin/iptables -t mangle -A SVQOS_IN -j CONNMARK --save-mark");
  system2 ("/usr/sbin/iptables -t mangle -A SVQOS_IN -j RETURN");

  return 0;
}
#endif
int
start_wshaper (void)
{
  int ret = 0;
  char *dev_val;
  char *dl_val;
  char *ul_val;
  char ulcalc1_val[32] = "";
  char ulcalc2_val[32] = "";
  char *nopriohostsrc_val;
  char *nopriohostdst_val;
  char *noprioportsrc_val;
  char *noprioportdst_val;
  int ulcalc1 = 0;
  int ulcalc2 = 0;
  char *mtu_val = "1500";

  stop_wshaper ();

  if (!nvram_invmatch ("wshaper_enable", "0"))
    return 0;

  dev_val = get_wshaper_dev ();

  if ((dl_val = nvram_safe_get ("wshaper_downlink")) == NULL &&
      atoi (dl_val) > 0)
    return 0;

  if ((ul_val = nvram_safe_get ("wshaper_uplink")) == NULL &&
      atoi (ul_val) > 0)
    return 0;

  nopriohostsrc_val = nvram_safe_get ("wshaper_nopriohostsrc");
  nopriohostdst_val = nvram_safe_get ("wshaper_nopriohostdst");
  noprioportsrc_val = nvram_safe_get ("wshaper_noprioportsrc");
  noprioportdst_val = nvram_safe_get ("wshaper_noprioportdst");

  ulcalc1 = 8 * atoi (ul_val) / 10;
  ulcalc2 = atoi (ul_val) / 12;

  sprintf (ulcalc1_val, "%d", ulcalc1);
  sprintf (ulcalc2_val, "%d", ulcalc2);

  if (nvram_match ("wshaper_dev", "WAN"))
    mtu_val = get_mtu_val ();
  else
    mtu_val = "1500";

#ifdef HAVE_WSHAPER
  ret =
    eval ("/usr/sbin/wshaper", dl_val, ul_val, dev_val, ulcalc1_val,
	  ulcalc2_val, nopriohostsrc_val, nopriohostdst_val,
	  noprioportsrc_val, noprioportdst_val);
#elif defined(HAVE_SVQOS)
  svqos_iptables ();
  if (nvram_match ("qos_type", "0"))
    ret = eval ("/usr/sbin/svqos", dl_val, ul_val, dev_val, mtu_val, "0");
  else
    ret = eval ("/usr/sbin/svqos2", ul_val, dl_val, dev_val, mtu_val, "0");
#ifdef HAVE_AQOS
  aqos_tables ();
#endif
#endif
  nvram_set ("qos_done", "1");
  return ret;
}

int
stop_wshaper (void)
{
  int ret = 0;
  nvram_set ("qos_done", "0");
#ifdef HAVE_WSHAPER
  char script_name[] = "/usr/sbin/wshaper";
#elif defined(HAVE_SVQOS)
  char *script_name;
  if (nvram_match ("qos_type", "0"))
    script_name = "/usr/sbin/svqos";
  else
    script_name = "/usr/sbin/svqos2";
#endif

  ret = eval (script_name, "stop", "XX", "br0");
#ifdef HAVE_RB500
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_XSCALE
  ret = eval (script_name, "stop", "XX", "ixp0");
  ret = eval (script_name, "stop", "XX", "ixp1");
  ret = eval (script_name, "stop", "XX", "ath0");
  ret = eval (script_name, "stop", "XX", "ath1");
#elif HAVE_MAGICBOX
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_LS2
  ret = eval (script_name, "stop", "XX", "vlan1");
  ret = eval (script_name, "stop", "XX", "vlan2");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_LS5
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_DIR300
  ret = eval (script_name, "stop", "XX", "vlan1");
  ret = eval (script_name, "stop", "XX", "vlan2");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_MR3202A
  ret = eval (script_name, "stop", "XX", "vlan0");
  ret = eval (script_name, "stop", "XX", "vlan1");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_FONERA
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_WHRAG108
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "eth1");
  ret = eval (script_name, "stop", "XX", "ath0");
  ret = eval (script_name, "stop", "XX", "ath1");
#elif HAVE_PB42
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "eth1");
  ret = eval (script_name, "stop", "XX", "ath0");
  ret = eval (script_name, "stop", "XX", "ath1");
#elif HAVE_TW6600
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
  ret = eval (script_name, "stop", "XX", "ath1");
#elif HAVE_CA8
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
#elif HAVE_X86
  ret = eval (script_name, "stop", "XX", "eth0");
  ret = eval (script_name, "stop", "XX", "ath0");
#else
  ret = eval (script_name, "stop", "XX", "vlan1");
  ret = eval (script_name, "stop", "XX", "eth1");
#endif
  ret = eval (script_name, "stop", "XX", "ppp0");

  stop_firewall ();
  start_firewall ();
  char *dev = get_wshaper_dev ();
  char *wl0mode = nvram_get ("wl0_mode");
  if (wl0mode == NULL)
    wl0mode = "";
  if (strcmp (dev, "br0") && strcmp (wl0mode, "wet") && getWET () == NULL
      && strcmp (wl0mode, "apstawet"))
    {
      eval ("rmmod", "ebt_dnat");
      eval ("rmmod", "ebt_snat");
      eval ("rmmod", "ebt_mark_m");
      eval ("rmmod", "ebt_mark");
      eval ("rmmod", "ebtable_filter");
      eval ("rmmod", "ebtable_nat");
      eval ("rmmod", "ebtables");
    }
  eval("rmmod","imq");
  return ret;
}
