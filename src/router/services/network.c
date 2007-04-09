
/*
 * Network services
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: network.c,v 1.8 2005/11/30 11:54:21 seg Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#ifdef HAVE_MSSID
#include <math.h>
#endif
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <sys/sysinfo.h>

typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;

typedef u_int64_t __u64;
typedef u_int32_t __u32;
typedef u_int16_t __u16;
typedef u_int8_t __u8;
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <sys/sysinfo.h>

#include <string.h>
#include <linux/version.h>

#include <linux/sockios.h>
#include <linux/ethtool.h>
//#include <libbridge.h>

#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <code_pattern.h>
#include <wlutils.h>
#include <utils.h>
#include <rc.h>
#include <ledcontrol.h>
#include <cy_conf.h>
#include <cymac.h>
#include <bcmutils.h>
#include <nvparse.h>
#include <etsockio.h>
#include <bcmparams.h>

extern int br_add_bridge (const char *brname);
extern int br_del_bridge (const char *brname);
extern int br_add_interface (const char *br, const char *dev);
extern int br_del_interface (const char *br, const char *dev);
extern int br_set_stp_state (const char *br, int stp_state);


#define PTABLE_MAGIC 0xbadc0ded
#define PTABLE_SLT1 1
#define PTABLE_SLT2 2
#define PTABLE_ACKW 3
#define PTABLE_ADHM 4
#define PTABLE_END 0xffffffff

/* phy types */
#define	PHY_TYPE_A		0
#define	PHY_TYPE_B		1
#define	PHY_TYPE_G		2
#define	PHY_TYPE_NULL		0xf

#define WL_IOCTL(name, cmd, buf, len) (wl_ioctl((name), (cmd), (buf), (len)))

#define TXPWR_MAX 251
#define TXPWR_DEFAULT 28

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)
#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)
/* configure loopback interface */
void
config_loopback (void)
{
  /* Bring up loopback interface */
  ifconfig ("lo", IFUP, "127.0.0.1", "255.0.0.0");

  /* Add to routing table */
  route_add ("lo", 0, "127.0.0.0", "0.0.0.0", "255.0.0.0");
}

char *
getMacAddr (char *ifname, char *mac)
{
  unsigned char hwbuff[16];
  int i = wl_hwaddr (ifname, hwbuff);
  if (i < 0)
    return NULL;
  sprintf (mac, "%02X:%02X:%02X:%02X:%02X:%02X", hwbuff[0], hwbuff[1],
	   hwbuff[2], hwbuff[3], hwbuff[4], hwbuff[5]);

}

#ifdef HAVE_MSSID
static unsigned long ptable[128];
static unsigned long kmem_offset;
static inline void
wlc_get_mem_offset (void)
{
  FILE *f;
  char s[64];

  /* yes, i'm lazy ;) */
  f = popen ("grep '\\[wl]' /proc/ksyms | sort", "r");
  if (fgets (s, 64, f) == 0)
    {
      return;
    }
  pclose (f);

  s[8] = 0;
  kmem_offset = strtoul (s, NULL, 16);

  /* sanity check */
  if (kmem_offset < 0xc0000000)
    kmem_offset = 0;
  return;
}
static int
ptable_init (void)
{
  struct stat statbuf;
  int fd;

  if (ptable[0] == PTABLE_MAGIC)
    return 0;

  if ((fd = open ("/etc/patchtable.bin", O_RDONLY)) < 0)
    return -1;

  if (fstat (fd, &statbuf) < 0)
    goto failed;

  if (statbuf.st_size < 512)
    goto failed;

//      if (lseek(fd, statbuf.st_size - 512, SEEK_SET) < 0) {
//              perror("lseek");
//              goto failed;
//      }

  if (read (fd, ptable, 512) < 512)
    goto failed;

  if (ptable[0] != PTABLE_MAGIC)
    goto failed;

  close (fd);

  wlc_get_mem_offset ();
  if (kmem_offset == 0)
    return -1;

  return 0;

failed:
  close (fd);

  return -1;
}

static inline unsigned long
wlc_kmem_read (unsigned long offset)
{
  int fd;
  unsigned long ret;

  if ((fd = open ("/dev/kmem", O_RDONLY)) < 0)
    return -1;

  lseek (fd, 0x70000000, SEEK_SET);
  lseek (fd, (kmem_offset - 0x70000000) + offset, SEEK_CUR);
  read (fd, &ret, 4);
  close (fd);

  return ret;
}

static inline void
wlc_kmem_write (unsigned long offset, unsigned long value)
{
  int fd;

  if ((fd = open ("/dev/kmem", O_WRONLY)) < 0)
    return;

  lseek (fd, 0x70000000, SEEK_SET);
  lseek (fd, (kmem_offset - 0x70000000) + offset, SEEK_CUR);
  write (fd, &value, 4);
  close (fd);
}

static int
wlc_patcher_getval (unsigned long key, unsigned long *val)
{
  unsigned long *pt = &ptable[1];
  unsigned long tmp;

  if (ptable_init () < 0)
    {
      fprintf (stderr, "Could not load the ptable\n");
      return -1;
    }

  while (*pt != PTABLE_END)
    {
      if (*pt == key)
	{
	  tmp = wlc_kmem_read (pt[1]);

	  if (tmp == pt[2])
	    *val = 0xffffffff;
	  else
	    *val = tmp;

	  return 0;
	}
      pt += 3;
    }

  return -1;
}

static int
wlc_patcher_setval (unsigned long key, unsigned long val)
{
  unsigned long *pt = &ptable[1];

  if (ptable_init () < 0)
    {
      fprintf (stderr, "Could not load the ptable\n");
      return -1;
    }

  if (val != 0xffffffff)
    val = (pt[2] & ~(0xffff)) | (val & 0xffff);

  while (*pt != PTABLE_END)
    {
      if (*pt == key)
	{
	  if (val == 0xffffffff)	/* default */
	    val = pt[2];

	  wlc_kmem_write (pt[1], val);
	}
      pt += 3;
    }

  return 0;
}

/*
static int get_wlc_slottime(wlc_param param, void *data, void *value)
{
	int *val = (int *) value;
	int ret = 0;

		ret = wlc_patcher_getval(PTABLE_SLT1, (unsigned long *) val);
		if (*val != 0xffffffff)
			*val &= 0xffff;
	}
	return ret;
}
*/
static int
set_wlc_slottime (int value)
{
  int ret = 0;

  wlc_patcher_setval (PTABLE_SLT1, value);
  wlc_patcher_setval (PTABLE_SLT2, ((value == -1) ? value : value + 510));
  return ret;
}


static int
wlc_noack (int value)
{
  int ret = 0;

//      if ((param & PARAM_MODE) == SET) {
  wlc_patcher_setval (PTABLE_ACKW, (value ? 1 : 0));
//      } else if ((param & PARAM_MODE) == GET) {
//              ret = wlc_patcher_getval(PTABLE_ACKW, (unsigned long *) val);
//              *val &= 0xffff;
//              *val = (*val ? 1 : 0);
//      }

  return ret;
}

#endif



#ifndef HAVE_MADWIFI
static int notify_nas (char *type, char *ifname, char *action);
#endif

void
start_dhcpc (char *wan_ifname)
{
  pid_t pid;
  char *wan_hostname = nvram_get ("wan_hostname");
  char *vendorclass = nvram_get ("dhcpc_vendorclass");
  char *requestip = nvram_get ("dhcpc_requestip");
  symlink ("/sbin/rc", "/tmp/udhcpc");

  nvram_set ("wan_get_dns", "");
  killall ("udhcpc", SIGTERM);

  char *dhcp_argv[] = { "udhcpc",
    "-i", wan_ifname,
    "-p", "/var/run/udhcpc.pid",
    "-s", "/tmp/udhcpc",
    NULL, NULL,
    NULL, NULL,
    NULL, NULL,
    NULL
  };

  int i = 7;

  if (vendorclass != NULL && strlen (vendorclass) > 0)
    {
      dhcp_argv[i] = "-V";
      i++;
      dhcp_argv[i] = vendorclass;
      i++;
    }

  if (requestip != NULL && strlen (requestip) > 0)
    {
      dhcp_argv[i] = "-r";
      i++;
      dhcp_argv[i] = requestip;
      i++;
    }

  if (wan_hostname != NULL && strlen (wan_hostname) > 0)
    {
      dhcp_argv[i] = "-H";
      i++;
      dhcp_argv[i] = wan_hostname;
      i++;
    }

  _eval (dhcp_argv, NULL, 0, &pid);

}

#ifdef HAVE_MSSID
/* Enable WET DHCP relay for ethernet clients */
static int
enable_dhcprelay (char *ifname)
{
  char name[80], *next;

  dprintf ("%s\n", ifname);

  /* WET interface is meaningful only in bridged environment */
  if (strncmp (ifname, "br", 2) == 0)
    {
      foreach (name, nvram_safe_get ("lan_ifnames"), next)
      {

	char mode[] = "wlXXXXXXXXXX_mode";
	int unit;

	/* make sure the interface is indeed of wl */
	if (wl_probe (name))
	  continue;

	/* get the instance number of the wl i/f */
	wl_ioctl (name, WLC_GET_INSTANCE, &unit, sizeof (unit));
	snprintf (mode, sizeof (mode), "wl%d_mode", unit);

	/* enable DHCP relay, there should be only one WET i/f */
	if (nvram_match (mode, "wet") || nvram_match (mode, "apstawet"))
	  {
	    uint32 ip;
	    inet_aton (nvram_safe_get ("lan_ipaddr"), (struct in_addr *) &ip);
	    if (wl_iovar_setint (name, "wet_host_ipv4", ip))
	      perror ("wet_host_ipv4");
	    break;
	  }
      }
    }
  return 0;
}
#endif

static int
wlconf_up (char *name)
{


  char tmp[100];
  int phytype, gmode, val, ret;
  char *afterburner;
#ifdef HAVE_MADWIFI
  return -1;
#endif
  if (!strncmp (name, "vlan", 4))
    return -1;
  if (!strncmp (name, "br", 2))
    return -1;
#ifdef HAVE_ONLYCLIENT
  if (nvram_match ("wl_mode", "ap"))
    {
      cprintf ("this version does only support the client mode\n");
      nvram_set ("wl_mode", "sta");
      nvram_commit ();
    }
#endif
  if (nvram_match ("wl0_mode", "infra"))
    {
      nvram_set ("wl_infra", "0");
      nvram_set ("wl0_infra", "0");
    }
  else
    {
      nvram_set ("wl_infra", "1");
      nvram_set ("wl0_infra", "1");
    }
  ret = eval ("wlconf", name, "up");
/*  eval("wl","radio","off");
  eval("wl","atten","0","0","60");
  eval("wl","lrl","16");
  eval("wl","srl","16");
  eval("wl","interference","0");
  eval("wl","radio","on");*/
  gmode = atoi (nvram_safe_get ("wl0_gmode"));

  /* Get current phy type */
  WL_IOCTL (name, WLC_GET_PHYTYPE, &phytype, sizeof (phytype));

  // set preamble type for b cards
  if (phytype == PHY_TYPE_B || gmode == 0)
    {
      if (nvram_match ("wl0_plcphdr", "long"))
	val = WLC_PLCP_LONG;
      else if (nvram_match ("wl0_plcphdr", "short"))
	val = WLC_PLCP_SHORT;
      else
	val = WLC_PLCP_AUTO;
      WL_IOCTL (name, WLC_SET_PLCPHDR, &val, sizeof (val));
    }
  // adjust txpwr and txant
  val = atoi (nvram_safe_get ("txpwr"));
  if (val < 0 || val > TXPWR_MAX)
    val = TXPWR_DEFAULT;
#ifndef HAVE_MSSID
  val |= WL_TXPWR_OVERRIDE;	// set the power override bit

  WL_IOCTL (name, WLC_SET_TXPWR, &val, sizeof (val));
  WL_IOCTL (name, WLC_CURRENT_PWR, &val, sizeof (val));
#else
//convert mw to qdbm and set override flag
  float value = 10 * log (val) / M_LN10;
  value *= 4;
  value += 0.5;
  val = (int) value;
  val |= WL_TXPWR_OVERRIDE;
  wl_iovar_setint (name, "qtxpower", val);

//eval("wl","txpwr1","-m","-o",nvram_safe_get("txpwr"));
#endif
  /* Set txant */
  val = atoi (nvram_safe_get ("txant"));
  if (val < 0 || val > 3 || val == 2)
    val = 3;
  WL_IOCTL (name, WLC_SET_TXANT, &val, sizeof (val));

/*  if (nvram_match ("boardtype", "bcm94710dev"))
    {
      if (val == 0)
	val = 1;
      if (val == 1)
	val = 0;
    }
*/
  val = atoi (nvram_safe_get ("wl0_antdiv"));
  WL_IOCTL (name, WLC_SET_ANTDIV, &val, sizeof (val));

  /* search for "afterburner" string */
  afterburner = nvram_safe_get ("wl0_afterburner");
  if (!strcmp (afterburner, "on"))
    eval ("wl", "afterburner_override", "1");
  else if (!strcmp (afterburner, "off"))
    eval ("wl", "afterburner_override", "0");
  else if (!strcmp (afterburner, "auto"))
    eval ("wl", "afterburner_override", "-1");

  char *shortslot = nvram_safe_get("wl0_shortslot"); 

  if (!strcmp (afterburner, "auto"))
    eval ("wl", "shortslot_override", "-1");
  else if (!strcmp (afterburner, "long"))
    eval ("wl", "shortslot_override", "0");
  else if (!strcmp (afterburner, "short"))
    eval ("wl", "shortslot_override", "1");



  // Set ACK Timing. Thx to Nbd
  char *v;
  if ((v = nvram_get ("wl0_distance")))
    {
      rw_reg_t reg;
      uint32 shm;

      val = atoi (v);
      if (v == 0)
	{
#ifdef HAVE_MSSID
	  //  wlc_noack (0);
#else
	  eval ("/etc/txackset.sh", "0");	// disable ack timing
#endif
	  return 0;
	}
      else
	{
#ifdef HAVE_MSSID
	  //  wlc_noack (1);
#else
	  eval ("/etc/txackset.sh", "1");	// enable ack timing
#endif
	}


#ifdef HAVE_MSSID
      // set_wlc_slottime (val);
#else
      val = 9 + (val / 150) + ((val % 150) ? 1 : 0);

      shm = 0x10;
      shm |= (val << 16);
      WL_IOCTL (name, 197, &shm, sizeof (shm));

      reg.byteoff = 0x684;
      reg.val = val + 510;
      reg.size = 2;
      WL_IOCTL (name, 102, &reg, sizeof (reg));
#endif
    }

/*		if (nvram_match("wl0_mode","sta") || nvram_match("wl0_mode","infra"))
		{
		val = 0;
		WL_IOCTL(name, WLC_SET_WET, &val, sizeof(val));
		if (nvram_match("wl_mode", "infra")){
			val = 0;
			WL_IOCTL(name, WLC_SET_INFRA, &val, sizeof(val));
		}
		else{
			val = 1;
			WL_IOCTL(name, WLC_SET_INFRA, &val, sizeof(val));
		}
		}
*/

  if (nvram_match ("wl0_mode", "infra"))
    {
      eval ("wl", "infra", "0");
      eval ("wl", "ssid", nvram_safe_get ("wl0_ssid"));
    }
#ifdef HAVE_MSSID
#ifndef HAVE_MADWIFI
  eval ("wl", "vlan_mode", "0");
#endif
#endif
  return ret;
}

int
isClient (void)
{
#ifndef HAVE_MADWIFI
  if (nvram_match ("wl0_mode", "sta") || nvram_match ("wl0_mode", "apsta")
      || nvram_match ("wl0_mode", "apstawet"))
    return 1;
#else
  if (getSTA ())
    return 1;
#endif
  return 0;

}

void
start_wlconf (void)
{
#ifdef HAVE_MSSID
if (nvram_invmatch ("wl0_net_mode", "disabled"))
#else
if (nvram_invmatch ("wl_net_mode", "disabled"))
#endif
  wlconf_up (nvram_safe_get ("wl0_ifname"));
}

//#ifdef HAVE_PORTSETUP

static void
do_portsetup (char *lan, char *ifname)
{
  char var[64];
  char var2[64];
  sprintf (var, "%s_bridged", ifname);
  if (nvram_default_match (var, "1", "1"))
    {
      br_add_interface (getBridge (ifname), ifname);
    }
  else
    {
      sprintf (var, "%s_ipaddr", ifname);
      sprintf (var2, "%s_netmask", ifname);
      ifconfig (ifname, IFUP, nvram_safe_get (var), nvram_safe_get (var2));
    }

}

//#endif


void
start_lan (void)
{
  struct ifreq ifr;
  unsigned char mac[20];
  int s;
  char eabuf[32];
  if ((s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    return;
#ifdef HAVE_RB500
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("wan_proto", "disabled"))
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames",
		 "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 ath0 ath1 ath2 ath3 ath4 ath5");
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames",
		 "eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 ath0 ath1 ath2 ath3 ath4 ath5");
      nvram_set ("wan_ifname", "eth0");
      nvram_set ("wan_ifnames", "eth0");
    }


  strncpy (ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
#endif

#ifdef HAVE_MAGICBOX
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("wan_proto", "disabled"))
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "eth0 eth1 ath0");
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "eth1 ath0");
      nvram_set ("wan_ifname", "eth0");
      nvram_set ("wan_ifnames", "eth0");
    }


  strncpy (ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
  strcpy (mac, nvram_safe_get ("et0macaddr"));
  MAC_ADD (mac);
  ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
  ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
  strncpy (ifr.ifr_name, "eth1", IFNAMSIZ);
  ioctl (s, SIOCSIFHWADDR, &ifr);
#endif
#ifdef HAVE_FONERA
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("wan_proto", "disabled"))
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "eth0 ath0");
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "ath0");
      nvram_set ("wan_ifname", "eth0");
      nvram_set ("wan_ifnames", "eth0");
    }


  strncpy (ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
  strcpy (mac, nvram_safe_get ("et0macaddr"));
#endif
#ifdef HAVE_LS2
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("wan_proto", "disabled"))
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "eth0 ath0");
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "ath0");
      nvram_set ("wan_ifname", "eth0");
      nvram_set ("wan_ifnames", "eth0");
    }


  strncpy (ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
  strcpy (mac, nvram_safe_get ("et0macaddr"));
#endif
#ifdef HAVE_WHRAG108
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("wan_proto", "disabled"))
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "eth0 eth1 ath0 ath1");
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else
    {
      nvram_set ("lan_ifname", "br0");
      nvram_set ("lan_ifnames", "eth0 ath0 ath1");
      nvram_set ("wan_ifname", "eth1");
      nvram_set ("wan_ifnames", "eth1");
    }


  strncpy (ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
  strcpy (mac, nvram_safe_get ("et0macaddr"));
#endif
#ifdef HAVE_GATEWORX
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("wan_proto", "disabled"))
    {
      if (getRouterBrand () == ROUTER_BOARD_GATEWORX_SWAP)
	{
	  nvram_set ("lan_ifname", "br0");
	  if (nvram_match ("intel_eth", "1"))
	    nvram_set ("lan_ifnames", "ixp0 eth0 eth1 ath0 ath1 ath2 ath3");
	  else
	    nvram_set ("lan_ifnames", "ixp0 ath0 ath1 ath2 ath3");
	  nvram_set ("wan_ifname", "");
	  nvram_set ("wan_ifnames", "");
	}
      else if (getRouterBrand () == ROUTER_BOARD_GATEWORX_GW2345)
	{
	  nvram_set ("lan_ifname", "br0");
	  if (nvram_match ("intel_eth", "1"))
	    nvram_set ("lan_ifnames",
		       "ixp0 ixp1 eth0 eth1 ath0 ath1 ath2 ath3");
	  else
	    nvram_set ("lan_ifnames", "ixp0 ixp1 ath0 ath1 ath2 ath3");
	  nvram_set ("wan_ifname", "");
	  nvram_set ("wan_ifnames", "");
	}
      else
	{
	  nvram_set ("lan_ifname", "br0");
	  if (nvram_match ("intel_eth", "1"))
	    nvram_set ("lan_ifnames",
		       "ixp0 ixp1 eth0 eth1 ath0 ath1 ath2 ath3");
	  else
	    nvram_set ("lan_ifnames", "ixp0 ixp1 ath0 ath1 ath2 ath3");
	  nvram_set ("wan_ifname", "");
	  nvram_set ("wan_ifnames", "");
	}
    }
  else
    {
      if (getRouterBrand () == ROUTER_BOARD_GATEWORX_SWAP)
	{
	  nvram_set ("lan_ifname", "br0");
	  if (nvram_match ("intel_eth", "1"))
	    nvram_set ("lan_ifnames", "eth0 eth1 ixp0 ath0 ath1 ath2 ath3");
	  else
	    nvram_set ("lan_ifnames", "ixp0 ath0 ath1 ath2 ath3");
	  if (nvram_get ("wan_ifname2") != NULL)
	    {
	      nvram_set ("wan_ifname", nvram_safe_get ("wan_ifname2"));
	      nvram_set ("wan_ifnames", nvram_safe_get ("wan_ifname2"));

	    }
	  else
	    {
	      nvram_set ("wan_ifname", "ixp0");
	      nvram_set ("wan_ifnames", "ixp0");
	    }
	}
      else if (getRouterBrand () == ROUTER_BOARD_GATEWORX_GW2345)
	{
	  nvram_set ("lan_ifname", "br0");
	  if (nvram_match ("intel_eth", "1"))
	    nvram_set ("lan_ifnames",
		       "eth0 eth1 ixp0 ixp1 ath0 ath1 ath2 ath3");
	  else
	    nvram_set ("lan_ifnames", "ixp0 ixp1 ath0 ath1 ath2 ath3");
	  if (nvram_get ("wan_ifname2") != NULL)
	    {
	      nvram_set ("wan_ifname", nvram_safe_get ("wan_ifname2"));
	      nvram_set ("wan_ifnames", nvram_safe_get ("wan_ifname2"));

	    }
	  else
	    {
	      nvram_set ("wan_ifname", "ixp1");
	      nvram_set ("wan_ifnames", "ixp1");
	    }
	}
      else
	{
	  nvram_set ("lan_ifname", "br0");
	  if (nvram_match ("intel_eth", "1"))
	    nvram_set ("lan_ifnames",
		       "eth0 eth1 ixp0 ixp1 ath0 ath1 ath2 ath3");
	  else
	    nvram_set ("lan_ifnames", "ixp0 ixp1 ath0 ath1 ath2 ath3");

	  if (nvram_get ("wan_ifname2") != NULL)
	    {
	      nvram_set ("wan_ifname", nvram_safe_get ("wan_ifname2"));
	      nvram_set ("wan_ifnames", nvram_safe_get ("wan_ifname2"));

	    }
	  else
	    {
	      nvram_set ("wan_ifname", "ixp1");
	      nvram_set ("wan_ifnames", "ixp1");
	    }
	}
    }
  strncpy (ifr.ifr_name, "ixp1", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
  /*
     strncpy (ifr.ifr_name, "ixp1", IFNAMSIZ);
     ioctl (s, SIOCGIFHWADDR, &ifr);
     nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
     strcpy (mac, nvram_safe_get ("et0macaddr"));
     MAC_ADD (mac);
     ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
     ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
     strncpy (ifr.ifr_name, "eth1", IFNAMSIZ);
     ioctl (s, SIOCSIFHWADDR, &ifr); */
#endif
#ifdef HAVE_X86
  if (getSTA () || getWET () || nvram_match ("ath0_mode", "wdssta"))
    {
      nvram_set ("lan_ifname", "br0");
#ifdef HAVE_NOWIFI
      nvram_set ("lan_ifnames",
		 "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10");
#else
      if (nvram_match ("wifi_bonding", "1"))
	nvram_set ("lan_ifnames",
		   "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 bond0");
      else
	nvram_set ("lan_ifnames",
		   "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 ath0 ath1 ath2 ath3 ath4 ath5 ath6 ath7 ath8");

#endif
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else if (nvram_match ("wan_proto", "disabled"))
    {
      nvram_set ("lan_ifname", "br0");
#ifdef HAVE_NOWIFI
      nvram_set ("lan_ifnames",
		 "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10");
#else
      if (nvram_match ("wifi_bonding", "1"))
	nvram_set ("lan_ifnames",
		   "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 bond0");
      else
	nvram_set ("lan_ifnames",
		   "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 ath0 ath1 ath2 ath3 ath4 ath5 ath6 ath7 ath8");
#endif
      nvram_set ("wan_ifname", "");
      nvram_set ("wan_ifnames", "");
    }
  else
    {
      nvram_set ("lan_ifname", "br0");
#ifdef HAVE_NOWIFI
      nvram_set ("lan_ifnames",
		 "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10");
#else
      if (nvram_match ("wifi_bonding", "1"))
	nvram_set ("lan_ifnames",
		   "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 bond0");
      else
	nvram_set ("lan_ifnames",
		   "eth0 eth1 eth2 eth3 eth4 eth5 eth6 eth7 eth8 eth9 eth10 ath0 ath1 ath2 ath3 ath4 ath5 ath6 ath7 ath8");
#endif
      if (nvram_get ("wan_ifname2") != NULL)
	{
	  nvram_set ("wan_ifname", nvram_safe_get ("wan_ifname2"));
	  nvram_set ("wan_ifnames", nvram_safe_get ("wan_ifname2"));

	}
      else
	{
#ifdef HAVE_GW700
	  nvram_set ("wan_ifname", "eth1");
	  nvram_set ("wan_ifnames", "eth1");
#else
	  nvram_set ("wan_ifname", "eth0");
	  nvram_set ("wan_ifnames", "eth0");
#endif
	}
    }
  strncpy (ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl (s, SIOCGIFHWADDR, &ifr);
  nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));


  /*
     strncpy (ifr.ifr_name, "ixp1", IFNAMSIZ);
     ioctl (s, SIOCGIFHWADDR, &ifr);
     nvram_set ("et0macaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
     strcpy (mac, nvram_safe_get ("et0macaddr"));
     MAC_ADD (mac);
     ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
     ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
     strncpy (ifr.ifr_name, "eth1", IFNAMSIZ);
     ioctl (s, SIOCSIFHWADDR, &ifr); */
#endif
  char *lan_ifname = strdup (nvram_safe_get ("lan_ifname"));
  char *wan_ifname = strdup (nvram_safe_get ("wan_ifname"));
  char *lan_ifnames = strdup (nvram_safe_get ("lan_ifnames"));
  char name[80], *next, *svbuf;
  char realname[80];
  char wl_face[10];

  strcpy (lan_ifname, nvram_safe_get ("lan_ifname"));
  strcpy (wan_ifname, nvram_safe_get ("wan_ifname"));
  strcpy (lan_ifnames, nvram_safe_get ("lan_ifnames"));

// Motorola doesnt like this
//      if(nvram_match("wl0_gmode", "-1"))
//              eval("rmmod", "wl");
//      else
//              eval("insmod", "wl");

  cprintf ("%s\n", lan_ifname);


  // If running in client-mode, remove old WAN-configuration
  if (nvram_match ("wl0_mode", "sta") || nvram_match ("wl0_mode", "apsta"))
    {
      //#ifdef HAVE_SKYTRON
      //ifconfig(wan_ifname,IFUP,"172.16.1.1","255.255.255.0");
      //#else
      ifconfig (wan_ifname, IFUP, "0.0.0.0", NULL);
      //#endif

    }

  // find wireless interface
  diag_led (DIAG, STOP_LED);	// stop that blinking
  strcpy (wl_face, get_wdev ());
#ifdef HAVE_MADWIFI
#ifndef HAVE_NOWIFI
  deconfigure_wifi ();
#endif
#else
  eval ("wlconf", wl_face, "down");
#endif
//      eval("rmmod", "wl");
//      eval("insmod", "wl");

  /* Write wireless mac */
  cprintf ("Write wireless mac\n");

  /* you gotta bring it down before you can set its MAC */
  cprintf ("configure wl_face\n");
  ifconfig (wl_face, 0, 0, 0);
#ifndef HAVE_MADWIFI

  if (nvram_match ("mac_clone_enable", "1") &&
      nvram_invmatch ("def_whwaddr", "00:00:00:00:00:00") &&
      nvram_invmatch ("def_whwaddr", ""))
    {
      ether_atoe (nvram_safe_get ("def_whwaddr"), ifr.ifr_hwaddr.sa_data);

    }
  else
    {
      if (nvram_match ("port_swap", "1"))
	strcpy (mac, nvram_safe_get ("et1macaddr"));
      else
	strcpy (mac, nvram_safe_get ("et0macaddr"));
      MAC_ADD (mac);
      MAC_ADD (mac);		// The wireless mac equal lan mac add 2
      ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
      if (nvram_match ("wl0_hwaddr", "") || !nvram_get ("wl0_hwaddr"))
	{
	  nvram_set ("wl0_hwaddr", mac);
	  nvram_commit ();
	}
    }
  ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
  strncpy (ifr.ifr_name, wl_face, IFNAMSIZ);

  if (ioctl (s, SIOCSIFHWADDR, &ifr) == -1)
    perror ("Write wireless mac fail : ");
  else
    cprintf ("Write wireless mac successfully\n");

#endif
  if (nvram_match ("wl_mode", "sta"))
    {
      unsigned char mac[20];
      if (nvram_match ("port_swap", "1"))
	strcpy (mac, nvram_safe_get ("et1macaddr"));
      else
	strcpy (mac, nvram_safe_get ("et0macaddr"));
      MAC_ADD (mac);
      nvram_set ("wan_hwaddr", mac);
    }

  ifconfig (wl_face, IFUP, 0, 0);
#ifdef HAVE_MICRO
  br_init ();
#endif
  /* Bring up bridged interface */
  if (strncmp (lan_ifname, "br0", 3) == 0)
    {
      br_add_bridge (lan_ifname);
#ifdef HAVE_MICRO
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      br_set_bridge_forward_delay (lan_ifname, &tv);
#else
      eval ("brctl", "setfd", lan_ifname, "0");
#endif
      //eval ("brctl", "addbr", lan_ifname);
      //eval ("brctl", "setfd", lan_ifname, "0");
      if (check_hw_type () != BCM4702_CHIP)
	{
	  br_set_stp_state (lan_ifname, 0);
	  //eval ("brctl", "stp", lan_ifname, "off");
	}
      else
	br_set_stp_state (lan_ifname, 1);

      foreach (name, lan_ifnames, next)
      {
	if (nvram_match ("wan_ifname", name))
	  continue;
	if (!ifexists (name))
	  continue;
#if defined(HAVE_MADWIFI) && !defined(HAVE_RB500) && !defined(HAVE_XSCALE) && !defined(HAVE_MAGICBOX) && !defined(HAVE_FONERA) && !defined(HAVE_WHRAG108) && !defined(HAVE_X86) && !defined(HAVE_LS2)
	if (!strcmp (name, "eth2"))
	  {
	    strcpy (realname, "ath0");
	  }
	else
#endif
	  strcpy (realname, name);

	cprintf ("name=[%s] lan_ifname=[%s]\n", realname, lan_ifname);

	/* Bring up interface */
	if (ifconfig (realname, IFUP, "0.0.0.0", NULL))
	  continue;

	/* Set the logical bridge address to that of the first interface */

#ifndef HAVE_MADWIFI
	strncpy (ifr.ifr_name, lan_ifname, IFNAMSIZ);
	if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0 &&
	    memcmp (ifr.ifr_hwaddr.sa_data, "\0\0\0\0\0\0",
		    ETHER_ADDR_LEN) == 0 && strcmp (wl_face, realname) == 0)
	  {
	    strncpy (ifr.ifr_name, realname, IFNAMSIZ);
	    if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
	      {
		strncpy (ifr.ifr_name, lan_ifname, IFNAMSIZ);
		ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
		ioctl (s, SIOCSIFHWADDR, &ifr);
		cprintf ("=====> set %s hwaddr to %s\n", lan_ifname,
			 realname);
	      }
	    else
	      perror (lan_ifname);
	  }
	else
	  perror (lan_ifname);
#endif
	/* If not a wl i/f then simply add it to the bridge */
#ifndef HAVE_MADWIFI
	if (wlconf_up (name))
	  {
#ifdef HAVE_PORTSETUP
	    do_portsetup (lan_ifname, name);
#else
	    br_add_interface (getBridge (name), name);
#endif
	  }
	else
	  {

	    if (nvram_match ("mac_clone_enable", "1") &&
		nvram_invmatch ("def_whwaddr", "00:00:00:00:00:00") &&
		nvram_invmatch ("def_whwaddr", ""))
	      {
		ether_atoe (nvram_safe_get ("def_whwaddr"),
			    ifr.ifr_hwaddr.sa_data);

#ifndef HAVE_MADWIFI
	      }
	    else
	      {
		if (nvram_match ("port_swap", "1"))
		  strcpy (mac, nvram_safe_get ("et1macaddr"));
		else
		  strcpy (mac, nvram_safe_get ("et0macaddr"));
		MAC_ADD (mac);
		MAC_ADD (mac);	// The wireless mac equal lan mac add 2
		ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
		if (nvram_match ("wl0_hwaddr", "")
		    || !nvram_get ("wl0_hwaddr"))
		  {
		    nvram_set ("wl0_hwaddr", mac);
		    nvram_commit ();
		  }
	      }
	    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
	    strncpy (ifr.ifr_name, wl_face, IFNAMSIZ);

	    if (ioctl (s, SIOCSIFHWADDR, &ifr) == -1)
	      perror ("Write wireless mac fail : ");
	    else
	      cprintf ("Write wireless mac successfully\n");

#else

		ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
		strncpy (ifr.ifr_name, wl_face, IFNAMSIZ);

		if (ioctl (s, SIOCSIFHWADDR, &ifr) == -1)
		  perror ("Write wireless mac fail : ");
		else
		  cprintf ("Write wireless mac successfully\n");
	      }
#endif
#ifdef HAVE_MSSID
/*	    char tmac[16];
	    sprintf (tmac, "%s_hwaddr", "wl0");
	    nvram_set (tmac, mac);

	    char *next2;
	    char var[80];
	    char *vifs = nvram_safe_get ("wl0_vifs");
	    if (vifs != NULL)
	      foreach (var, vifs, next2)
	      {
		sprintf (tmac, "%s_hwaddr", var);
		MAC_ADD (mac);
		nvram_set (tmac, mac);
		ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
		ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
		strncpy (ifr.ifr_name, var, IFNAMSIZ);
		if (ioctl (s, SIOCSIFHWADDR, &ifr) == -1)
		  perror ("Write wireless mac fail : ");
		else
		  cprintf ("Write wireless mac successfully\n");
	      }*/
#endif


#else
	cprintf ("configure %s\n", name);
	if (strcmp (name, "wl0"))	//check if the interface is a buffalo wireless
	  {
//#ifdef HAVE_PORTSETUP
	    do_portsetup (lan_ifname, name);
//#else
//          br_add_interface (lan_ifname, name);
//#endif
	    //eval ("brctl", "addif", lan_ifname, name);
	  }
	else
	  {

#endif
	    /* get the instance number of the wl i/f */
	    char wl_name[] = "wlXXXXXXXXXX_mode";
	    int unit;
#ifndef HAVE_MADWIFI
	    wl_ioctl (name, WLC_GET_INSTANCE, &unit, sizeof (unit));
#else
	    unit = 0;
#endif
	    snprintf (wl_name, sizeof (wl_name), "wl%d_mode", unit);
	    /* Do not attach the main wl i/f if in wds or client/adhoc */

	    led_control (LED_BRIDGE, LED_OFF);
	    if (nvram_match (wl_name, "wet")
		|| nvram_match (wl_name, "apstawet"))
	      {
		ifconfig (name, IFUP | IFF_ALLMULTI, NULL, NULL);	// from up
		br_add_interface (getBridge (name), name);
		led_control (LED_BRIDGE, LED_ON);
#ifdef HAVE_MSSID
		enable_dhcprelay (lan_ifname);
#endif
	      }


	    if (nvram_match (wl_name, "ap"))
	      {

		br_add_interface (getBridge (name), name);	//eval ("brctl", "addif", lan_ifname, name);
#ifdef HAVE_MSSID
		do_mssid (lan_ifname);
#endif
	      }
#ifdef HAVE_MSSID
	    if (nvram_match (wl_name, "apsta"))
	      {
#ifndef HAVE_MADWIFI
		eval ("wl", "ap", "0");
		eval ("wl", "infra", "1");
		wl_ioctl (wl_name, WLC_SCAN, svbuf, sizeof (svbuf));
		wlconf_up (name);
#endif
		//eval("wlconf", name, "up");
		ifconfig (name, IFUP | IFF_ALLMULTI, NULL, NULL);
#ifndef HAVE_MADWIFI
		eval ("wl", "ap", "0");
#ifndef HAVE_MSSID
		eval ("wl", "ssid", nvram_get ("wl_ssid"));
#else
		eval ("wl", "ssid", nvram_get ("wl0_ssid"));
#endif
#endif
//              eval ("brctl", "addif", lan_ifname, name);
#ifndef HAVE_FON
		if (nvram_match ("fon_enable", "0"))
		  do_mssid (lan_ifname);
#endif
	      }
#endif

	    /* if client/wet mode, turn off ap mode et al */
	    if (nvram_match (wl_name, "infra"))
	      {
#ifndef HAVE_MADWIFI
		eval ("wl", "ap", "0");
		eval ("wl", "infra", "0");
		wl_ioctl (wl_name, WLC_SCAN, svbuf, sizeof (svbuf));
		wlconf_up (name);
#endif
		eval ("wl", "infra", "0");
		eval ("wl", "ssid", nvram_safe_get ("wl0_ssid"));
		ifconfig (name, IFUP | IFF_ALLMULTI, NULL, NULL);
	      }

	    if (nvram_match (wl_name, "sta"))
	      {
#ifndef HAVE_MADWIFI
		eval ("wl", "ap", "0");
		eval ("wl", "infra", "1");
		wl_ioctl (wl_name, WLC_SCAN, svbuf, sizeof (svbuf));
		wlconf_up (name);
#endif
		//eval("wlconf", name, "up");
		ifconfig (name, IFUP | IFF_ALLMULTI, NULL, NULL);
#ifndef HAVE_MADWIFI
		eval ("wl", "ap", "0");
#ifndef HAVE_MSSID
		eval ("wl", "ssid", nvram_get ("wl_ssid"));
#else
		eval ("wl", "ssid", nvram_get ("wl0_ssid"));
#endif
#endif
	      }

	  }

      }
    }

  free (lan_ifname);
  free (lan_ifnames);
#ifdef HAVE_MADWIFI
#ifndef HAVE_NOWIFI
  configure_wifi ();

  if (nvram_match ("mac_clone_enable", "1") &&
      nvram_invmatch ("def_whwaddr", "00:00:00:00:00:00") &&
      nvram_invmatch ("def_whwaddr", ""))
    {
      ether_atoe (nvram_safe_get ("def_whwaddr"), ifr.ifr_hwaddr.sa_data);
      ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
      strncpy (ifr.ifr_name, wl_face, IFNAMSIZ);

      if (ioctl (s, SIOCSIFHWADDR, &ifr) == -1)
	perror ("Write wireless mac fail : ");
      else
	cprintf ("Write wireless mac successfully\n");
    }

#endif
#endif
  lan_ifname = strdup (nvram_safe_get ("lan_ifname"));
  lan_ifnames = strdup (nvram_safe_get ("lan_ifnames"));

  /* specific non-bridged lan i/f */
  if (strcmp (lan_ifname, ""))
    {				// FIXME
      /* Bring up interface */
      ifconfig (lan_ifname, IFUP, NULL, NULL);
#ifndef HAVE_MADWIFI
      /* config wireless i/f */
      if (!wlconf_up (lan_ifname))
	{
	  char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	  int unit;
	  /* get the instance number of the wl i/f */
	  wl_ioctl (lan_ifname, WLC_GET_INSTANCE, &unit, sizeof (unit));
	  snprintf (prefix, sizeof (prefix), "wl%d_", unit);
	  /* Receive all multicast frames in WET mode */
	  if (nvram_match (strcat_r (prefix, "mode", tmp), "sta"))
	    ifconfig (lan_ifname, IFUP | IFF_ALLMULTI, NULL, NULL);
	  if (nvram_match (strcat_r (prefix, "mode", tmp), "apsta"))
	    ifconfig (lan_ifname, IFUP | IFF_ALLMULTI, NULL, NULL);

	}
#endif

    }

  /* Bring up and configure LAN interface */
  ifconfig (lan_ifname, IFUP, nvram_safe_get ("lan_ipaddr"),
	    nvram_safe_get ("lan_netmask"));

  char staticlan[32];
  sprintf (staticlan, "%s:0", lan_ifname);
#ifdef HAVE_FONERA
  if (nvram_match ("ath0_mode", "sta") || nvram_match ("ath0_mode", "wdssta")
      || nvram_match ("ath0_mode", "wet")
      || nvram_match ("wan_proto", "disabled"))
    {
#endif

      eval ("ifconfig", "eth0:0", "down");
//add fallback ip
      eval ("ifconfig", staticlan, "169.254.255.1", "netmask",
	    "255.255.255.0");

#ifdef HAVE_FONERA
    }
  else
    eval ("ifconfig", staticlan, "0.0.0.0", "down");
#endif

  /* Get current LAN hardware address */

  strncpy (ifr.ifr_name, lan_ifname, IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      nvram_set ("lan_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
#ifdef HAVE_RB500
      nvram_set ("et0macaddr", nvram_safe_get ("lan_hwaddr"));
#endif
#ifdef HAVE_XSCALE
#ifndef HAVE_GATEWORX
      nvram_set ("et0macaddr", nvram_safe_get ("lan_hwaddr"));
#endif
#endif
#ifdef HAVE_MAGICBOX
      nvram_set ("et0macaddr", nvram_safe_get ("lan_hwaddr"));
#endif
#ifdef HAVE_FONERA
      nvram_set ("et0macaddr", nvram_safe_get ("lan_hwaddr"));
#endif
#ifdef HAVE_LS2
      nvram_set ("et0macaddr", nvram_safe_get ("lan_hwaddr"));
#endif
#ifdef HAVE_WHRAG108
      nvram_set ("et0macaddr", nvram_safe_get ("lan_hwaddr"));
#endif
    }
#ifdef HAVE_RB500
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif
#ifdef HAVE_X86
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif
#ifdef HAVE_XSCALE
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif
#ifdef HAVE_MAGICBOX
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif
#ifdef HAVE_FONERA
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif
#ifdef HAVE_LS2
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif
#ifdef HAVE_WHRAG108
  strncpy (ifr.ifr_name, "ath0", IFNAMSIZ);
  if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
    {
      char eabuf[32];
      nvram_set ("wl0_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
    }
#endif

  close (s);
  cprintf ("%s %s\n",
	   nvram_safe_get ("lan_ipaddr"), nvram_safe_get ("lan_netmask"));

  /* Sveasoft - create separate WDS subnet bridge if enabled */
#ifdef HAVE_MADWIFI
  int cnt = getifcount ("wifi");
  int c;
  for (c = 0; c < cnt; c++)
#endif
    {
#ifdef HAVE_MADWIFI
      char br1enable[32];
      char br1ipaddr[32];
      char br1netmask[32];
      sprintf (br1enable, "ath%d_br1_enable", c);
      sprintf (br1ipaddr, "ath%d_br1_ipaddr", c);
      sprintf (br1netmask, "ath%d_br1_netmask", c);
#else
      char *br1enable = "wl_br1_enable";
      char *br1ipaddr = "wl_br1_ipaddr";
      char *br1netmask = "wl_br1_netmask";
#endif
      if (nvram_get (br1enable) == NULL)
	nvram_set (br1enable, "0");
      if (nvram_get (br1ipaddr) == NULL)
	nvram_set (br1ipaddr, "0.0.0.0");
      if (nvram_get (br1netmask) == NULL)
	nvram_set (br1netmask, "255.255.255.0");
      if (nvram_match (br1enable, "1"))
	{
	  ifconfig ("br1", 0, 0, 0);

	  //  eval ("ifconfig", "br1", "down");
	  br_del_bridge ("br1");
	  br_add_bridge ("br1");

#ifdef HAVE_MICRO
	  struct timeval tv;
	  tv.tv_sec = 0;
	  tv.tv_usec = 0;
	  br_set_bridge_forward_delay (lan_ifname, &tv);
#else
	  eval ("brctl", "setfd", lan_ifname, "0");
#endif

	  //eval ("brctl", "delbr", "br1");
	  //eval ("brctl", "addbr", "br1");
	  //eval ("brctl", "setfd", "br1", "0");

	  if (nvram_match ("lan_stp", "0"))
	    br_set_stp_state ("br1", 0);	//eval ("brctl", "stp", "br1", "off");
	  else
	    br_set_stp_state ("br1", 1);	//eval ("brctl", "stp", "br1", "off");

	  /* Bring up and configure br1 interface */
	  if (nvram_invmatch (br1ipaddr, "0.0.0.0"))
	    {
	      ifconfig ("br1", IFUP, nvram_safe_get (br1ipaddr),
			nvram_safe_get (br1netmask));

	      if (nvram_match ("lan_stp", "0"))
		br_set_stp_state ("br1", 0);	//eval ("brctl", "stp", "br1", "off");
	      else
		br_set_stp_state ("br1", 1);	//eval ("brctl", "stp", "br1", "off");


	      sleep (2);
#ifndef HAVE_MADWIFI
	      notify_nas ("lan", "br1", "up");
#endif
	    }

	}
    }
  /* Sveasoft - Bring up and configure wds interfaces */
  /* logic - if separate ip defined bring it up */
  /*         else if flagged for br1 and br1 is enabled add to br1 */
  /*         else add it to the br0 bridge */
#ifdef HAVE_MADWIFI
  for (c = 0; c < cnt; c++)
#endif
    {

      for (s = 1; s <= MAX_WDS_DEVS; s++)
	{
	  char wdsvarname[32] = { 0 };
	  char wdsdevname[32] = { 0 };
	  char *dev;
#ifdef HAVE_MADWIFI
	  char br1enable[32];
	  sprintf (wdsvarname, "ath%d_wds%d_enable", c, s);
	  sprintf (wdsdevname, "ath%d_wds%d_if", c, s);
	  sprintf (br1enable, "ath%d_br1_enable", c);
	  if (nvram_get (wdsvarname) == NULL)
	    nvram_set (wdsvarname, "0");
#else
	  sprintf (wdsvarname, "wl_wds%d_enable", s);
	  sprintf (wdsdevname, "wl_wds%d_if", s);
	  char *br1enable = "wl_br1_enable";
#endif
	  dev = nvram_safe_get (wdsdevname);
	  if (strlen (dev) == 0)
	    continue;
	  ifconfig (dev, 0, 0, 0);

	  //  eval ("ifconfig", dev, "down");
	  if (nvram_match (wdsvarname, "1"))
	    {
	      char wdsip[32] = { 0 };
	      char wdsbc[32] = { 0 };
	      char wdsnm[32] = { 0 };
#ifdef HAVE_MADWIFI
	      snprintf (wdsip, 31, "ath%d_wds%d_ipaddr", c, s);
	      snprintf (wdsnm, 31, "ath%d_wds%d_netmask", c, s);
#else
	      snprintf (wdsip, 31, "wl_wds%d_ipaddr", s);
	      snprintf (wdsnm, 31, "wl_wds%d_netmask", s);
#endif

	      snprintf (wdsbc, 31, "%s", nvram_safe_get (wdsip));
	      get_broadcast (wdsbc, nvram_safe_get (wdsnm));
	      eval ("ifconfig", dev, nvram_safe_get (wdsip), "broadcast",
		    wdsbc, "netmask", nvram_safe_get (wdsnm), "up");
	    }
	  else if (nvram_match (wdsvarname, "2")
		   && nvram_match (br1enable, "1"))
	    {
	      eval ("ifconfig", dev, "up");
	      sleep (1);
	      br_add_interface ("br1", dev);
	    }
	  else if (nvram_match (wdsvarname, "3"))
	    {
	      ifconfig (dev, IFUP, 0, 0);
	      sleep (1);
	      br_add_interface ("br0", dev);
	    }
	}
    }
#ifdef HAVE_XSCALE
#define HAVE_RB500
#endif
#ifdef HAVE_MAGICBOX
#define HAVE_RB500
#endif
#ifdef HAVE_FONERA
#define HAVE_RB500
#endif
#ifdef HAVE_LS2
#define HAVE_RB500
#endif
#ifdef HAVE_WHRAG108
#define HAVE_RB500
#endif
#ifndef HAVE_RB500
  /* Set QoS mode */
  if ((s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0)
    {
      int i, qos;
      caddr_t ifrdata;
      struct ethtool_drvinfo info;

      qos = (strcmp (nvram_safe_get ("wl_wme"), "on")) ? 0 : 1;
      for (i = 1; i <= DEV_NUMIFS; i++)
	{
	  ifr.ifr_ifindex = i;
	  if (ioctl (s, SIOCGIFNAME, &ifr))
	    continue;
	  if (ioctl (s, SIOCGIFHWADDR, &ifr))
	    continue;
	  if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
	    continue;
	  /* get flags */
	  if (ioctl (s, SIOCGIFFLAGS, &ifr))
	    continue;
	  /* if up(wan not up yet at this point) */
	  if (ifr.ifr_flags & IFF_UP)
	    {
	      ifrdata = ifr.ifr_data;
	      memset (&info, 0, sizeof (info));
	      info.cmd = ETHTOOL_GDRVINFO;
	      ifr.ifr_data = (caddr_t) & info;
	      if (ioctl (s, SIOCETHTOOL, &ifr) >= 0)
		{
		  /* currently only need to set QoS to et devices */
		  if (!strncmp (info.driver, "et", 2))
		    {
		      ifr.ifr_data = (caddr_t) & qos;
		      ioctl (s, SIOCSETCQOS, &ifr);
		    }
		}
	      ifr.ifr_data = ifrdata;
	    }
	}
    }

#undef HAVE_RB500
#endif
  /* Sveasoft - set default IP gateway defined */
  if (strcmp (nvram_safe_get ("lan_gateway"), "0.0.0.0"))
    eval ("/usr/sbin/ip", "ro", "add", "default", "via",
	  nvram_safe_get ("lan_gateway"), "dev", "br0");

#ifdef HAVE_MSSID
#ifndef HAVE_MADWIFI
  eval ("wl", "vlan_mode", "0");
#endif
#endif
  /* Bring up local host interface */
  config_loopback ();

  /* Set additional lan static routes if need */
  start_set_routes ();
#ifndef HAVE_MADWIFI
#ifndef HAVE_MSSID
  eval ("/usr/sbin/wl", "radio",
	nvram_invmatch ("wl_net_mode", "disabled") ? "on" : "off");
#else
  eval ("/usr/sbin/wl", "radio",
	nvram_invmatch ("wl0_net_mode", "disabled") ? "on" : "off");
#endif
#endif
  /* Disable wireless will cause diag led blink, so we want to stop it. */
  if (check_hw_type () == BCM4712_CHIP)
    {
      diag_led (DIAG, STOP_LED);
      /* Light or go out the DMZ led even if there is no wan ip. */
      if (nvram_invmatch ("dmz_ipaddr", "")
	  && nvram_invmatch ("dmz_ipaddr", "0"))
	diag_led (DMZ, START_LED);
      else
	diag_led (DMZ, STOP_LED);
    }

#ifndef HAVE_MADWIFI
#ifndef HAVE_MSSID
  if (nvram_match ("wl0_mode", "wet") || nvram_match ("wl0_mode", "sta"))
    {
      system2 ("wl wep sw");
      sleep (1);
      system2 ("wl wep hw");
    }
#endif
#endif
  if (nvram_match ("lan_stp", "0"))
    br_set_stp_state ("br0", 0);
  else
    br_set_stp_state ("br0", 1);

  //system ("/usr/sbin/brctl stp br0 off");

  free (lan_ifnames);
  free (lan_ifname);
  eval ("rm", "/tmp/hosts");
  addHost ("localhost", "127.0.0.1");
  if (strlen (nvram_safe_get ("wan_hostname")) > 0)
    addHost (nvram_safe_get ("wan_hostname"), nvram_safe_get ("lan_ipaddr"));
  else if (strlen (nvram_safe_get ("router_name")) > 0)
    addHost (nvram_safe_get ("router_name"), nvram_safe_get ("lan_ipaddr"));
#ifdef HAVE_MICRO
  br_shutdown ();
#endif

}

void
stop_lan (void)
{
  char *lan_ifname = nvram_safe_get ("lan_ifname");
  char name[80], *next;

  cprintf ("%s\n", lan_ifname);
  /* Bring down LAN interface */
  ifconfig (lan_ifname, 0, NULL, NULL);
#ifdef HAVE_MICRO
  br_init ();
#endif

#ifdef HAVE_MSSID
#ifndef HAVE_MADWIFI
  br_del_interface (lan_ifname, "wl0.1");
  ifconfig ("wl0.1", 0, NULL, NULL);
  br_del_interface (lan_ifname, "wl0.2");
  ifconfig ("wl0.2", 0, NULL, NULL);
  br_del_interface (lan_ifname, "wl0.3");
  ifconfig ("wl0.3", 0, NULL, NULL);
  br_del_interface (lan_ifname, "wl0.4");
  ifconfig ("wl0.4", 0, NULL, NULL);
#endif
#endif
  /* Bring down bridged interfaces */
  if (strncmp (lan_ifname, "br", 2) == 0)
    {
      foreach (name, nvram_safe_get ("lan_ifnames"), next)
      {
	if (nvram_match ("wan_ifname", name))
	  continue;
	if (!ifexists (name))
	  continue;
	eval ("wlconf", name, "down");
	ifconfig (name, 0, NULL, NULL);
	br_del_interface (lan_ifname, name);
	//eval ("brctl", "delif", lan_ifname, name);
      }
      br_del_bridge (lan_ifname);
      //eval ("brctl", "delbr", lan_ifname);
    }
  /* Bring down specific interface */
  else if (strcmp (lan_ifname, ""))
    eval ("wlconf", lan_ifname, "down");
#ifdef HAVE_MICRO
  br_shutdown ();
#endif

  cprintf ("done\n");
}

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

int
wan_valid (char *ifname)
{
  char name[80], *next;

  foreach (name, nvram_safe_get ("wan_ifnames"), next)
    if (ifname && !strcmp (ifname, name))
    return 1;

  if (nvram_match ("wl_mode", "sta"))
    {
      return nvram_match ("wl0_ifname", ifname);
    }
  return 0;
}


void
start_wan (int status)
{
  FILE *fp;
  char *wan_ifname = get_wan_face ();
  char *wan_proto = nvram_safe_get ("wan_proto");
  int s;
  struct ifreq ifr;

#ifdef HAVE_PPPOE
#ifdef HAVE_RB500
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : nvram_safe_get ("wan_ifname");
#else

#ifdef HAVE_XSCALE
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : "ixp1";
#elif HAVE_MAGICBOX
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : "eth0";
#elif HAVE_FONERA
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : "eth0";
#elif HAVE_LS2
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : "eth0";
#elif HAVE_WHRAG108
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : "eth1";
#elif HAVE_X86
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : nvram_safe_get ("wan_ifname");
#else
  char *pppoe_wan_ifname = nvram_invmatch ("pppoe_wan_ifname",
					   "") ?
    nvram_safe_get ("pppoe_wan_ifname") : "vlan1";
#endif
  /* Rewritten by Eko, May 10, 2006 */
  int brand = getRouterBrand ();
  switch (brand)
    {
    case ROUTER_BUFFALO_WBR54G:
    case ROUTER_MICROSOFT_MN700:
    case ROUTER_BUFFALO_WZRRSG54:
    case ROUTER_WRTSL54GS:
    case ROUTER_WRT300N:
    case ROUTER_BUFFALO_WLAH_G54: 
    case ROUTER_BUFFALO_WAPM_HP_AM54G54:   
    case ROUTER_WRT150N:
    case ROUTER_BUFFALO_WZRG300N:
    case ROUTER_NETGEAR_WNR834B:
    case ROUTER_MOTOROLA_V1:
    case ROUTER_RT210W:
    case ROUTER_BRCM4702_GENERIC:
    case ROUTER_DELL_TRUEMOBILE_2300:
      if (!strcmp (nvram_safe_get ("pppoe_wan_ifname"), ""))
	pppoe_wan_ifname = "eth1";
      break;
    case ROUTER_BUFFALO_WLI2_TX1_G54:
    case ROUTER_MOTOROLA_WE800G:
    case ROUTER_BUFFALO_WLAG54C:
    case ROUTER_WAP54G_V1:
    case ROUTER_SITECOM_WL105B:
      if (!strcmp (nvram_safe_get ("pppoe_wan_ifname"), ""))
	pppoe_wan_ifname = "eth0";
      break;
    }
#endif
#ifndef HAVE_MADWIFI
  if (nvram_match ("wl0_mode", "wet") || nvram_match ("wl0_mode", "apstawet"))
    {
      dns_to_resolv ();
      return;
    }
  if (isClient ())
    {
      pppoe_wan_ifname = get_wdev ();
    }

#else
  if (isClient ())
    {
      pppoe_wan_ifname = getSTA ();
    }
#endif
/*	if (strcmp(wan_proto,"pppoe"))
	    {
	    eval("insmod","slhc");	    
	    eval("insmod","ppp_generic");
	    eval("insmod","bsd_comp");
	    eval("insmod","ppp_async");
	    eval("insmod","ppp_deflate");
	    eval("insmod","ppp_mppe_mppc");
	    eval("insmod","ppp_synctty");
	    eval("insmod","pppox");
	    eval("insmod","pppoe");
	    }
*/
#endif
//fprintf(stderr,"%s %s\n", wan_ifname, wan_proto);

  if ((s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    return;
  /* Check PPPoE version, RP or linksys */
#ifdef HAVE_PPPOE
  if (nvram_match ("wan_proto", "pppoe"))
    strncpy (ifr.ifr_name, pppoe_wan_ifname, IFNAMSIZ);
  else
#endif
    strncpy (ifr.ifr_name, wan_ifname, IFNAMSIZ);

  /* Set WAN hardware address before bringing interface up */
  memset (ifr.ifr_hwaddr.sa_data, 0, ETHER_ADDR_LEN);

  ifconfig (wan_ifname, 0, NULL, NULL);
#ifdef HAVE_FONERA
  char staticlan[32];
  sprintf (staticlan, "%s:0", wan_ifname);
  if (!nvram_match ("ath0_mode", "sta")
      && !nvram_match ("ath0_mode", "wdssta")
      && !nvram_match ("ath0_mode", "wet")
      && !nvram_match ("wan_proto", "disabled"))
    {
      eval ("ifconfig", "br0:0", "down");
      eval ("ifconfig", staticlan, "169.254.255.1", "netmask",
	    "255.255.255.0");
    }
  else
    eval ("ifconfig", staticlan, "0.0.0.0", "down");
#endif

//fprintf(stderr,"%s %s\n", wan_ifname, wan_proto);

  if (nvram_match ("mac_clone_enable", "1") &&
      nvram_invmatch ("def_hwaddr", "00:00:00:00:00:00") &&
      nvram_invmatch ("def_hwaddr", ""))
    {
      ether_atoe (nvram_safe_get ("def_hwaddr"), ifr.ifr_hwaddr.sa_data);
    }
#ifndef HAVE_MADWIFI
  else
    {
      unsigned char mac[20];
      if (nvram_match ("port_swap", "1"))
	strcpy (mac, nvram_safe_get ("et1macaddr"));
      else
	strcpy (mac, nvram_safe_get ("et0macaddr"));
      MAC_ADD (mac);		// The wan mac equal lan mac add 1
      ether_atoe (mac, ifr.ifr_hwaddr.sa_data);
    }

  if (memcmp (ifr.ifr_hwaddr.sa_data, "\0\0\0\0\0\0", ETHER_ADDR_LEN))
    {
      ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
      ioctl (s, SIOCSIFHWADDR, &ifr);
      cprintf ("Write WAN mac successfully\n");
    }
  else
    perror ("Write WAN mac fail : ");


#endif
//fprintf(stderr,"%s %s\n", wan_ifname, wan_proto);

  /* Set MTU */
  init_mtu (wan_proto);		// add by honor 2002/12/27
//fprintf(stderr,"%s %s\n", wan_ifname, wan_proto);

  // Set our Interface to the right MTU
#ifdef HAVE_PPPOE
  if (nvram_match ("wan_proto", "pppoe"))
    {
      ifr.ifr_mtu = 1500;	// default ethernet frame size
    }
  else
#endif
    ifr.ifr_mtu = atoi (nvram_safe_get ("wan_mtu"));
  // fprintf(stderr,"set mtu for %s to %d\n",ifr.ifr_name,ifr.ifr_mtu);
  ioctl (s, SIOCSIFMTU, &ifr);

  if (strcmp (wan_proto, "disabled") == 0)
    {
      start_wan_done (wan_ifname);
      return;
    }

  /* Bring up WAN interface */
#ifdef HAVE_PPPOE
  /* AhMan  March 19 2005 */
  /* ice-man March 19 2005 */
  /* pppoe_wan interface must be up in order to use any pppoe client */
  if (strcmp (wan_proto, "pppoe") == 0)
    ifconfig (pppoe_wan_ifname, IFUP, NULL, NULL);
  else
    {
      ifconfig (wan_ifname, IFUP, NULL, NULL);
    }
#else

  ifconfig (wan_ifname, IFUP, NULL, NULL);
  if (nvram_match ("wl0_mode", "infra"))
    {
      eval ("wl", "infra", "0");
      eval ("wl", "ssid", nvram_safe_get ("wl0_ssid"));
    }

#endif
  set_host_domain_name ();

  // Remove the current value of pppd_pppifname
  nvram_set ("pppd_pppifname", "");

  /* Configure WAN interface */
#ifdef HAVE_PPPOE
  if ((strcmp (wan_proto, "pppoe") == 0))
    {
      char username[80], passwd[80];
      char idletime[20], retry_num[20];

      snprintf (idletime, sizeof (idletime), "%d",
		atoi (nvram_safe_get ("ppp_idletime")) * 60);
      snprintf (retry_num, sizeof (retry_num), "%d",
		(atoi (nvram_safe_get ("ppp_redialperiod")) / 5) - 1);

      if (nvram_match ("aol_block_traffic", "0"))
	{
	  snprintf (username, sizeof (username), "%s",
		    nvram_safe_get ("ppp_username"));
	  snprintf (passwd, sizeof (passwd), "%s",
		    nvram_safe_get ("ppp_passwd"));
	}
      else
	{
	  if (!strcmp (nvram_safe_get ("aol_username"), ""))
	    {
	      snprintf (username, sizeof (username), "%s",
			nvram_safe_get ("ppp_username"));
	      snprintf (passwd, sizeof (passwd), "%s",
			nvram_safe_get ("ppp_passwd"));
	    }
	  else
	    {
	      snprintf (username, sizeof (username), "%s",
			nvram_safe_get ("aol_username"));
	      snprintf (passwd, sizeof (passwd), "%s",
			nvram_safe_get ("aol_passwd"));
	    }
	}

      mkdir ("/tmp/ppp", 0777);
      int timeout = 5;

      // Lets open option file and enter all the parameters.
      fp = fopen ("/tmp/ppp/options.pppoe", "w");
#if 0
      // pty is used by pppd to initiate PPPoE binary for connection negotiation
      fprintf (fp, "pty '/usr/sbin/pppoe -I %s", pppoe_wan_ifname);


      // This allows us to select a specific PPPoE service provider (only available via command line setting nvram variable ppp_service to service provider
      if (nvram_invmatch ("pppoe_service", ""))
	fprintf (fp, " -S %s", nvram_safe_get ("pppoe_service"));

      // This allows us to select a specific PPPoE access concentrator (only available via command line setting nvram variable ppp_ac to concentrators name
      if (nvram_invmatch ("pppoe_ac", ""))
	fprintf (fp, " -C %s", nvram_safe_get ("pppoe_ac"));

      // This allows us to clamp MSS to a specific value.
      // Warning!!! This _may_ break IPSec connections using this interface if using this options.
      // if at all possible clampmss should be used in iptables during tcp handshaking (syn)
      // If IPSec used on this interface, rely on lower MTU and clamp MSS using iptables on traffic before IPSec tunnel
      // Do not forget about IPSec overhead when clamping with iptables.
      if (nvram_invmatch ("pppoe_clampmss", ""))
	if (atoi (nvram_safe_get ("pppoe_clampmss")) > 0)
	  fprintf (fp, " -m %s", nvram_safe_get ("pppoe_clampmss"));

      // Experimental synchronous transfer mode
      // Will not work on all ISP's or kernel's.
      // If it does, consider yourself lucky and enjoy greatly reduced CPU usage and faster troughtput
      if (nvram_match ("pppoe_synchronous", "1"))
	fprintf (fp, " -s'\nsync\n");
      else
	fprintf (fp, "'\n");
#endif
      // rp-pppoe kernelmode plugin
      fprintf (fp, "plugin /usr/lib/rp-pppoe.so");

      if (nvram_invmatch ("pppoe_service", ""))
	fprintf (fp, " rp_pppoe_service %s",
		 nvram_safe_get ("pppoe_service"));
      fprintf (fp, "\n");
      fprintf (fp, "nic-%s\n", pppoe_wan_ifname);

      // Those are default options we use + user/passwd
      // By using user/password options we dont have to deal with chap/pap secrets files.
      if (nvram_match("ppp_compression","1"))
            {
	    fprintf(fp,"mppc\n");
	    }
	    else
	    {
	    fprintf(fp,"nomppc\n");
	    }
      fprintf (fp, "noipdefault\n"
	       "noauth\n"
	       "defaultroute\n"
	       "noaccomp\n"
	       "noccp\n" "nobsdcomp\n" "nodeflate\n" "maxfail 0\n"
//             "nocrtscts\n"
//             "lock\n"
	       "nopcomp\n"
	       "novj\n"
	       "novjccomp\n"
	       "nomppe\n"
	       "usepeerdns\n"
	       "user '%s'\n" "password '%s'\n", username, passwd);

      // This is a tricky one. When used it could improve speed of PPPoE but not all ISP's can support it.
      // default-asyncmap escapes all control characters. By using asyncmap 0 PPPD will not escape any control characters
      // Not all ISP's can handle this. By default use default-asyncmap
      // and if ppp_asyncmap=1 do not escape
      if (nvram_match ("ppp_asyncmap", "1"))
	fprintf (fp, "asyncmap 0\n");
      else
	fprintf (fp, "default-asyncmap\n");


      // Allow users some control on PPP interface MTU and MRU
      // If pppoe_ppp_mtu > 0 will set mtu of pppX interface to the value in the nvram variable
      // If pppoe_ppp_mru > 0 will set mru of pppX interface to the value in the nvram variable
      // if none is specified PPPD will autonegotiate the values with ISP (sometimes not desirable)
      // Do not forget this should be at least 8 bytes less then physycal interfaces mtu.


      // if MRU is not Auto force MTU/MRU of interface to value selected by theuser on web page
      if (nvram_match ("mtu_enable", "1"))
	{
	  if (atoi (nvram_safe_get ("wan_mtu")) > 0)
	    {
	      fprintf (fp, "mtu %s\n", nvram_safe_get ("wan_mtu"));
	      fprintf (fp, "mru %s\n", nvram_safe_get ("wan_mtu"));
	    }

	}
      else
	{
	  // If MRU set to Auto we still allow custom MTU/MRU settings for expirienced users
	  if (nvram_invmatch ("pppoe_ppp_mtu", ""))
	    if (atoi (nvram_safe_get ("pppoe_ppp_mtu")) > 0)
	      fprintf (fp, "mtu %s\n", nvram_safe_get ("pppoe_ppp_mtu"));
	  if (nvram_invmatch ("pppoe_ppp_mru", ""))
	    if (atoi (nvram_safe_get ("pppoe_ppp_mru")) > 0)
	      fprintf (fp, "mru %s\n", nvram_safe_get ("pppoe_ppp_mru"));
	}

      // Allow runtime debugging
      if (nvram_match ("ppp_debug", "1"))
	fprintf (fp, "debug\n");

      // Demand dial.. This is not pretty.
      // The first problems i see is that if connection is lost it would take PPPoE (idletime * 2) * 3 to notice it.
      // In other words if idle is set to 30 seconds, it would take 30*2*3 (180) seconds to detect the lost connection.
      // We have to increase the lcp-echo-interval to idletime*2 so that we do not upset the idletime counter.
      // When not using demand dialing, it only takes 15 seconds to detect the lost connection.
      if (nvram_match ("ppp_demand", "1"))
	fprintf (fp, "demand\n"
		 "idle %s\n"
		 "10.112.112.112:10.112.112.113\n"
		 "lcp-echo-interval %d\n"
		 "lcp-echo-failure 3\n"
		 "ipcp-accept-remote\n"
		 "ipcp-accept-local\n"
		 "connect true\n" "ktune\n", idletime, atoi (idletime) * 2);
      else
	fprintf (fp, "persist\n"
		 "lcp-echo-interval 5\n" "lcp-echo-failure 10\n");

      fclose (fp);

      symlink ("/sbin/rc", "/tmp/ppp/ip-up");
      symlink ("/sbin/rc", "/tmp/ppp/ip-down");
      unlink ("/tmp/ppp/log");

      //Clean pppoe linksys client files - Added by ice-man (Wed Jun 1)
      unlink ("/tmp/ppp/connect-log");
      unlink ("/tmp/ppp/set-pppoepid");

      stop_dhcpc ();
      stop_pptp ();
//      system("export LINUX_PLUGIN=/usr/lib/rp-pppoe.so");     
      eval ("/usr/sbin/pppd", "file", "/tmp/ppp/options.pppoe");

      // This is horrible.
      // What if pppoe recconects with ppp1?

      /* Pretend that the WAN interface is up */
      if (nvram_match ("ppp_demand", "1"))
	{
	  /* Wait for ppp0 to be created */
	  while (ifconfig ("ppp0", IFUP, NULL, NULL) && timeout--)
	    sleep (1);
	  strncpy (ifr.ifr_name, "ppp0", IFNAMSIZ);

	  /* Set temporary IP address */
	  timeout = 3;
	  while (ioctl (s, SIOCGIFADDR, &ifr) && timeout--)
	    {
	      perror ("ppp0");
	      printf ("Wait ppp inteface to init (1) ...\n");
	      sleep (1);
	    };
	  nvram_set ("wan_ipaddr", inet_ntoa (sin_addr (&ifr.ifr_addr)));
	  nvram_set ("wan_netmask", "255.255.255.255");

	  /* Set temporary P-t-P address */
	  timeout = 3;
	  while (ioctl (s, SIOCGIFDSTADDR, &ifr) && timeout--)
	    {
	      perror ("ppp0");
	      printf ("Wait ppp inteface to init (2) ...\n");
	      sleep (1);
	    }
	  nvram_set ("wan_gateway", inet_ntoa (sin_addr (&ifr.ifr_dstaddr)));

	  start_wan_done ("ppp0");

	  // if user press Connect" button from web, we must force to dial
	  if (nvram_match ("action_service", "start_pppoe"))
	    {
	      sleep (3);
	      start_force_to_dial ();
	      nvram_set ("action_service", "");
	    }
	}
      else
	{
	  if (status != REDIAL)
	    {
	      start_redial ();
	    }
	}
    }
  else
#endif
  if (strcmp (wan_proto, "dhcp") == 0)
    {
      start_dhcpc (wan_ifname);
    }
#ifdef HAVE_PPTP
  else if (strcmp (wan_proto, "pptp") == 0)
    {
      start_pptp (status);
    }
#endif
#ifdef HAVE_L2TP
  else if (strcmp (wan_proto, "l2tp") == 0)
    {
      start_dhcpc (wan_ifname);
    }
#endif
#ifdef HAVE_HEARTBEAT
  else if (strcmp (wan_proto, "heartbeat") == 0)
    {
      start_dhcpc (wan_ifname);
    }
#endif
  else
    {
      ifconfig (wan_ifname, IFUP,
		nvram_safe_get ("wan_ipaddr"),
		nvram_safe_get ("wan_netmask"));
      start_wan_done (wan_ifname);
    }
  cprintf ("dhcp client ready\n");

  /* Get current WAN hardware address */
//  fprintf(stderr,"get wan addr of %s\n",wan_ifname);
  strncpy (ifr.ifr_name, wan_ifname, IFNAMSIZ);
  cprintf ("get current hardware adress");
  {
    if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
      {
	char eabuf[32];
	nvram_set ("wan_hwaddr", ether_etoa (ifr.ifr_hwaddr.sa_data, eabuf));
//        fprintf(stderr,"write wan addr %s\n",nvram_safe_get("wan_hwaddr"));
      }

  }

  close (s);

  // set_ip_forward('1');

//===================================================================================
//Tallest move herei(from "start_lan" function ). Fixed when wireless disable, wireless LED dosen't off.
//===================================================================================
  /* Disable wireless will cause diag led blink, so we want to stop it. */

  cprintf ("diag led control\n");
  if ((check_hw_type () == BCM4712_CHIP)
      || (check_hw_type () == BCM5325E_CHIP))
    {
      //Barry will put disable WLAN here
      if (nvram_match ("wl_gmode", "-1"))
	{
	  diag_led (WL, STOP_LED);
#if 0
	  eval ("wlled", "0 0 1");
	  eval ("wlled", "0 1 1");
#endif
	}
      diag_led (DIAG, STOP_LED);
    }
  /* Light or go out the DMZ led even if there is no wan ip. */
  if (nvram_match ("dmz_enable", "1") &&
      nvram_invmatch ("dmz_ipaddr", "") && nvram_invmatch ("dmz_ipaddr", "0"))
    diag_led (DMZ, START_LED);
  else
    diag_led (DMZ, STOP_LED);

  cprintf ("%s %s\n", nvram_safe_get ("wan_ipaddr"),
	   nvram_safe_get ("wan_netmask"));

  if (nvram_match ("wan_proto", "l2tp"))
    {
      /* Delete all default routes */
      while (route_del (get_wan_face (), 0, NULL, NULL, NULL) == 0);
    }
  cprintf ("wep handling\n");
#ifndef HAVE_MSSID
  if (nvram_match ("wl0_mode", "wet") || nvram_match ("wl0_mode", "sta")
      || nvram_match ("wl0_mode", "apsta"))
    {
      system2 ("wl wep sw");
      sleep (1);
      system2 ("wl wep hw");
    }
#endif
  cprintf ("disable stp if needed\n");
  if (nvram_match ("lan_stp", "0"))
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_set_stp_state ("br0", 0);	//system ("/usr/sbin/brctl stp br0 off");
#ifdef HAVE_MICRO
      br_shutdown ();
#endif

    }
  else
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_set_stp_state ("br0", 1);	//system ("/usr/sbin/brctl stp br0 off");
#ifdef HAVE_MICRO
      br_shutdown ();
#endif

    }

  cprintf ("done()()()\n");
}

void
start_wan_boot (void)
{
  start_wan (BOOT);
}

void
start_wan_redial (void)
{
  start_wan (REDIAL);
}

void
start_wan_service (void)
{
  stop_process_monitor ();
  stop_ddns ();
  cprintf ("start process monitor\n");
  start_process_monitor ();
  sleep (5);
  cprintf ("start ddns\n");
  start_ddns ();
}


void
start_wan_done (char *wan_ifname)
{
  cprintf ("%s %s\n", wan_ifname, nvram_safe_get ("wan_proto"));

  if (nvram_match ("wan_proto", "l2tp"))
    {
      /* Delete all default routes */
      while (route_del (nvram_safe_get ("wan_ifname"), 0, NULL, NULL, NULL) ==
	     0);
    }

  /* Delete all default routes */
  while (route_del (wan_ifname, 0, NULL, NULL, NULL) == 0);

  if ((nvram_match ("wan_proto", "pppoe")) && check_wan_link (1))
    {
      while (route_del
	     (nvram_safe_get ("wan_ifname_1"), 0, NULL, NULL, NULL) == 0);
    }

  if (nvram_invmatch ("wan_proto", "disabled"))
    {
      int timeout = 5;
      /* Set default route to gateway if specified */
      char *gateway = nvram_match ("wan_proto",
				   "pptp") ? nvram_safe_get ("pptp_get_ip") :
	nvram_safe_get ("wan_gateway");
      while (route_add (wan_ifname, 0, "0.0.0.0", gateway, "0.0.0.0")
	     && timeout--)
	{
	  if ((nvram_match ("wan_proto", "pppoe"))
	      && nvram_match ("ppp_demand", "1"))
	    {
	      printf ("Wait ppp interface to init (3) ...\n");
	      sleep (1);
	    }

	}
    }

  /* Delete all default routes */
//      while (route_del(wan_ifname, 0, NULL, NULL, NULL) == 0);

  /* Set default route to gateway if specified */
/*	while (strcmp(nvram_safe_get("wan_proto"), "disabled") && route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get("wan_gateway"), "0.0.0.0") && timeout-- ){
		if (nvram_match("wan_proto", "pppoe") && nvram_match("ppp_demand", "1")) {
			printf("Wait ppp interface to init (3) ...\n");
			sleep(1);
		}
	}
*/
  if (nvram_match ("wan_proto", "pptp"))
    {
      route_del (nvram_safe_get ("wan_iface"), 0,
		 nvram_safe_get ("wan_gateway"), NULL, "255.255.255.255");
      route_del (nvram_safe_get ("wan_iface"), 0,
		 nvram_safe_get ("pptp_server_ip"), NULL, "255.255.255.255");
      route_add (nvram_safe_get ("wan_iface"), 0,
		 nvram_safe_get ("pptp_get_ip"), NULL, "255.255.255.255");
    }
  else if (nvram_match ("wan_proto", "l2tp"))
    {
      route_del (nvram_safe_get ("wan_iface"), 0,
		 nvram_safe_get ("wan_gateway"), NULL, "255.255.255.255");
      route_add (nvram_safe_get ("wan_iface"), 0,
		 nvram_safe_get ("l2tp_get_ip"), NULL, "255.255.255.255");
      route_add (nvram_safe_get ("wan_ifname"), 0, nvram_safe_get ("l2tp_server_ip"), nvram_safe_get ("wan_gateway_buf"), "255.255.255.255");	//fixed routing problem in Israel by kanki
    }

  /* save dns to resolv.conf */
  cprintf ("dns to resolv\n");
  dns_to_resolv ();

  cprintf ("restart dhcp server\n");
  /* Restart DHCP server */
  stop_udhcpd ();
  start_udhcpd ();
  cprintf ("restart dns proxy\n");
  /* Restart DNS proxy 
     stop_dnsmasq ();
     start_dnsmasq (); */
  cprintf ("start firewall\n");
  /* Start firewall */
  start_firewall ();
  cprintf ("start icmp proxy\n");
#ifdef HAVE_MULTICAST
  stop_igmp_proxy ();
  start_igmp_proxy ();
#endif
  cprintf ("ready\n");


  /* Set additional wan static routes if need */

  start_set_routes ();
  cprintf ("routes done\n");
  if (nvram_match ("wan_proto", "pppoe") || nvram_match ("wan_proto", "pptp")
      || nvram_match ("wan_proto", "l2tp"))
    {
      if (nvram_match ("ppp_demand", "1"))
	{			// ntp and ddns will trigger DOD, so we must stop them when wan is unavaile.
	  FILE *fp;
	  if ((fp = fopen ("/tmp/ppp/link", "r")))
	    {
	      start_wan_service ();
	      fclose (fp);
	    }
	}
      else
	{
	  start_wan_service ();
	}
    }
  else
    {
      start_wan_service ();
    }

#ifdef HAVE_BIRD
  stop_zebra ();
#endif
#ifdef HAVE_UPNP
  stop_upnp ();
#endif
  stop_cron ();
  stop_wshaper ();
  cprintf ("start zebra\n");
#ifdef HAVE_BIRD
  start_zebra ();
#endif
  cprintf ("start upnp\n");
#ifdef HAVE_UPNP
  start_upnp ();
#endif
  cprintf ("start cron\n");
  start_cron ();
  cprintf ("start wshaper\n");
  start_wshaper ();

  if (nvram_match ("wan_proto", "pptp"))
    {

      if (nvram_invmatch ("pptp_customipup", ""))
	{

	  // We not going to assume that /tmp/ppp is created..
	  mkdir ("/tmp/ppp", 0700);

	  // Create our custom pptp ipup script and change its attributes
	  nvram2file ("pptp_customipup", "/tmp/ppp/sh_pptp_customipup");
	  chmod ("/tmp/ppp/sh_pptp_customipup", 0744);

	  // Execute our custom ipup script
	  system2 ("/tmp/ppp/sh_pptp_customipup");

	}
    }
  cprintf ("std on\n");
  if (check_hw_type () == BCM4702_CHIP)
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_set_stp_state (nvram_safe_get ("lan_ifname"), 0);
      //eval ("brctl", "stp", nvram_safe_get ("lan_ifname"), "off");
    }
  else
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_set_stp_state (nvram_safe_get ("lan_ifname"), 1);


    }
  cprintf ("check wan link\n");
  if (check_wan_link (0))
    SET_LED (GOT_IP);
  else if ((!check_wan_link (0)) && nvram_match ("wan_proto", "auto"))
    {
      SET_LED (GET_IP_ERROR);
    }

  cprintf ("running custom DD-WRT ipup scripts\n");
  runStartup ("/etc/config", ".ipup");
#ifdef HAVE_RB500
  runStartup ("/usr/local/etc/config", ".ipup");
#else
  runStartup ("/jffs/etc/config", ".ipup");
  runStartup ("/mmc/etc/config", ".ipup");
#endif
  cprintf ("trigger gpio");

  led_control (LED_CONNECTED, LED_ON);

  double sys_uptime;
  FILE *up;

  up = fopen ("/proc/uptime", "r");
  fscanf (up, "%lf", &sys_uptime);
  fclose (up);

  up = fopen ("/tmp/.wanuptime", "w");
  fprintf (up, "%lf", sys_uptime);
  fclose (up);



  cprintf ("done\n");
  char *wani = nvram_safe_get ("wan_iface");
  if (strlen (wani) == 0)
    nvram_set ("wan_iface", nvram_safe_get ("wan_ifname"));


#ifdef HAVE_OPENVPN
  cprintf ("starting openvpn\n");
  stop_openvpn ();
  start_openvpn ();
  cprintf ("done\n");

#endif
#ifdef HAVE_NEWMEDIA
  stop_openvpnserverwan ();
  start_openvpnserverwan ();
#endif
  stop_dhcpfwd ();
  start_dhcpfwd ();
  nvram_set ("wanup", "1");
#ifdef HAVE_SPUTNIK_APD
  stop_sputnik ();
  start_sputnik ();
#endif

#ifdef HAVE_FON
#ifdef HAVE_MICRO
  br_init ();
#endif

#ifndef HAVE_MSSID
  br_del_interface (nvram_safe_get ("lan_ifname"), get_wdev ());
  //eval ("brctl", "delif", nvram_safe_get ("lan_ifname"), getwlif ());
  ifconfig (get_wdev (), IFUP | IFF_ALLMULTI, "0.0.0.0", NULL);
#else
  if (nvram_match ("wl0_mode", "apsta"))
    {
      br_del_interface (nvram_safe_get ("lan_ifname"), "wl0.1");
//      eval ("brctl", "delif", nvram_safe_get ("lan_ifname"), "wl0.1");
      ifconfig ("wl0.1", IFUP | IFF_ALLMULTI, "0.0.0.0", NULL);
    }
  else if (nvram_match ("wl0_mode", "ap"))
    {
      br_del_interface (nvram_safe_get ("lan_ifname"), get_wdev ());
//      eval ("brctl", "delif", nvram_safe_get ("lan_ifname"), getwlif ());
      ifconfig (get_wdev, IFUP | IFF_ALLMULTI, "0.0.0.0", NULL);
    }
  stop_chilli ();
  start_chilli ();
#endif
#else
  if (nvram_match ("fon_enable", "1")
      || (nvram_match ("chilli_nowifibridge", "1")
	  && nvram_match ("chilli_enable", "1")))
    {
#ifndef HAVE_MSSID
      br_del_interface (nvram_safe_get ("lan_ifname"), get_wdev ());
//      eval ("brctl", "delif", nvram_safe_get ("lan_ifname"), getwlif ());
      ifconfig (get_wdev (), IFUP | IFF_ALLMULTI, "0.0.0.0", NULL);
#else
      if (nvram_match ("wl0_mode", "apsta"))
	{
	  br_del_interface (nvram_safe_get ("lan_ifname"), "wl0.1");
//        eval ("brctl", "delif", nvram_safe_get ("lan_ifname"), "wl0.1");
	  ifconfig ("wl0.1", IFUP | IFF_ALLMULTI, "0.0.0.0", NULL);
	}
      else if (nvram_match ("wl0_mode", "ap"))
	{
	  br_del_interface (nvram_safe_get ("lan_ifname"), get_wdev ());
//        eval ("brctl", "delif", nvram_safe_get ("lan_ifname"), get_wdev ());
	  ifconfig (get_wdev (), IFUP | IFF_ALLMULTI, "0.0.0.0", NULL);
	}
      stop_chilli ();
      start_chilli ();
#endif
    }

#endif
#ifdef HAVE_MICRO
  br_shutdown ();
#endif
#ifdef HAVE_MADWIFI
#ifndef HAVE_NOWIFI
  start_hostapdwan ();
#endif
#endif

}

void
stop_wan (void)
{
  char *wan_ifname = get_wan_face ();
  nvram_set ("wanup", "0");

  led_control (LED_CONNECTED, LED_OFF);
  unlink ("/tmp/.wanuptime");

  cprintf ("%s %s\n", wan_ifname, nvram_safe_get ("wan_proto"));
#ifdef HAVE_NEWMEDIA
  stop_openvpnserverwan ();
#endif
#ifdef HAVE_OPENVPN
  stop_openvpn ();
#endif
  stop_dhcpfwd ();
  /* Stop firewall */
  stop_firewall ();
  /* Kill any WAN client daemons or callbacks */
#ifdef HAVE_PPPOE
  stop_pppoe ();
#endif
#ifdef HAVE_L2TP
  stop_l2tp ();
#endif
  stop_dhcpc ();
#ifdef HAVE_HEARTBEAT
  stop_heartbeat ();
#endif
#ifdef HAVE_PPTP
  stop_pptp ();
#endif
#ifdef HAVE_SPUTNIK_APD
  stop_sputnik ();
#endif
  stop_ntpc ();
  stop_redial ();
  nvram_set ("wan_get_dns", "");

  // Reset pppd's pppX interface
  nvram_set ("pppd_pppifname", "");

  /* Bring down WAN interfaces */
  ifconfig (wan_ifname, 0, NULL, NULL);
#ifdef HAVE_PPP
/*	    eval("rmmod","pppoe");
	    eval("rmmod","pppox");
	    eval("mrmod","ppp_synctty");
	    eval("rmmod","ppp_mppe_mppc");
	    eval("rmmod","ppp_deflate");
	    eval("rmmod","ppp_async");
	    eval("rmmod","bsd_comp");
	    eval("rmmod","ppp_generic");
	    eval("rmmod","slhc");	 */
#endif
#ifndef HAVE_FON
  if (nvram_match ("fon_enable", "1")
      || (nvram_match ("chilli_nowifibridge", "1")
	  && nvram_match ("chilli_enable", "1")))
#endif
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_add_interface (getBridge (get_wdev ()), get_wdev ());
#ifdef HAVE_MICRO
      br_shutdown ();
#endif

    }
//    eval ("brctl", "addif", nvram_safe_get ("lan_ifname"), getwlif ());

  cprintf ("done\n");
}

void start_set_routes (void)
{
  char word[80], *tmp;
  char *ipaddr, *netmask, *gateway, *metric, *ifname;

  foreach (word, nvram_safe_get ("static_route"), tmp)
  {
    netmask = word;
    ipaddr = strsep (&netmask, ":");
    if (!ipaddr || !netmask)
      continue;
    gateway = netmask;
    netmask = strsep (&gateway, ":");
    if (!netmask || !gateway)
      continue;
    metric = gateway;
    gateway = strsep (&metric, ":");
    if (!gateway || !metric)
      continue;
    ifname = metric;
    metric = strsep (&ifname, ":");
    if (!metric || !ifname)
      continue;
    if (!strcmp(ipaddr,"0.0.0.0"))
	eval("route","add","default","gw",gateway);
    route_add (ifname, atoi (metric) + 1, ipaddr, gateway, netmask);
  }
}


#ifndef HAVE_MADWIFI
static int
notify_nas (char *type, char *ifname, char *action)
{
  char *argv[] = { "nas4not", type, ifname, action,
    NULL,			/* role */
    NULL,			/* crypto */
    NULL,			/* auth */
    NULL,			/* passphrase */
    NULL,			/* ssid */
    NULL
  };
  char *str = NULL;
  int retries = 10;
  char tmp[100], prefix[] = "wlXXXXXXXXXX_";
  int unit;
  char remote[ETHER_ADDR_LEN];
  char ssid[48], pass[80], auth[16], crypto[16], role[8];
  int i;

  /* the wireless interface must be configured to run NAS */
  wl_ioctl (ifname, WLC_GET_INSTANCE, &unit, sizeof (unit));
  snprintf (prefix, sizeof (prefix), "wl%d_", unit);
  if (nvram_match (strcat_r (prefix, "akm", tmp), "") &&
      nvram_match (strcat_r (prefix, "auth_mode", tmp), "none"))
    return 0;

  while (retries-- > 0 && !(str = file2str ("/tmp/nas.wl0lan.pid")))
    sleep (1);
  if (!str)
    {
      return -1;
    }
  free (str);
  sleep (3);
  /* find WDS link configuration */
  wl_ioctl (ifname, WLC_WDS_GET_REMOTE_HWADDR, remote, ETHER_ADDR_LEN);
  for (i = 0; i < MAX_NVPARSE; i++)
    {
      char mac[ETHER_ADDR_STR_LEN];
      uint8 ea[ETHER_ADDR_LEN];

      if (get_wds_wsec (unit, i, mac, role, crypto, auth, ssid, pass) &&
	  ether_atoe (mac, ea) && !bcmp (ea, remote, ETHER_ADDR_LEN))
	{
	  argv[4] = role;
	  argv[5] = crypto;
	  argv[6] = auth;
	  argv[7] = pass;
	  argv[8] = ssid;
	  break;
	}
    }

  /* did not find WDS link configuration, use wireless' */
  if (i == MAX_NVPARSE)
    {
      /* role */
      argv[4] = "auto";
      /* crypto */
      argv[5] = nvram_safe_get (strcat_r (prefix, "crypto", tmp));
      /* auth mode */
      argv[6] = nvram_safe_get (strcat_r (prefix, "akm", tmp));
      /* passphrase */
      argv[7] = nvram_safe_get (strcat_r (prefix, "wpa_psk", tmp));
      /* ssid */
      argv[8] = nvram_safe_get (strcat_r (prefix, "ssid", tmp));
    }
  int pid;
  return _eval (argv, ">/dev/console", 0, &pid);
}
#endif
/*
static int
notify_nas(char *type, char *ifname, char *action)
{
	char *argv[] = {"nas4not", type, ifname, action,
			NULL,	
			NULL,	
			NULL,	
			NULL,	
			NULL,	
			NULL};
	char *str = NULL;
	int retries = 10;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	int unit;
	char remote[ETHER_ADDR_LEN];
	char ssid[48], pass[80], auth[16], crypto[16], role[8];
	int i;

	wl_ioctl(ifname, WLC_GET_INSTANCE, &unit, sizeof(unit));
	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
#ifdef WPA2_WMM
	if (nvram_match(strcat_r(prefix, "akm", tmp), "") &&
	    nvram_match(strcat_r(prefix, "auth_mode", tmp), "none"))
#else
	if (nvram_match(strcat_r(prefix, "auth_mode", tmp), "open") ||
	    nvram_match(strcat_r(prefix, "auth_mode", tmp), "shared"))
#endif
		return 0;

	wl_ioctl(ifname, WLC_WDS_GET_REMOTE_HWADDR, remote, ETHER_ADDR_LEN);
	for (i = 0; i < MAX_NVPARSE; i ++) {
		char mac[ETHER_ADDR_STR_LEN];
		uint8 ea[ETHER_ADDR_LEN];

		if (get_wds_wsec(unit, i, mac, role, crypto, auth, ssid, pass) &&
		    ether_atoe(mac, ea) && !bcmp(ea, remote, ETHER_ADDR_LEN)) {
			argv[4] = role;
			argv[5] = crypto;
			argv[6] = auth;
			argv[7] = pass;
			argv[8] = ssid;
			break;
		}
	}

	if (i == MAX_NVPARSE) {
		
		argv[4] = "auto";
		
		argv[5] = nvram_safe_get(strcat_r(prefix, "crypto", tmp));
		
#ifdef WPA2_WMM
		argv[6] = nvram_safe_get(strcat_r(prefix, "akm", tmp));
#else
		argv[6] = nvram_safe_get(strcat_r(prefix, "auth_mode", tmp));
#endif
		
		argv[7] = nvram_safe_get(strcat_r(prefix, "wpa_psk", tmp));
		
		argv[8] = nvram_safe_get(strcat_r(prefix, "ssid", tmp));
	}

	while (retries -- > 0 && !(str = file2str("/tmp/nas.lan.pid")))
		sleep(1);
	if (str) {
		int pid;
		free(str);
		return _eval(argv, ">/dev/console", 0, &pid);
	}
	return -1;

}
*/
int
start_hotplug_net (void)
{
#ifdef HAVE_MADWIFI
  return 0;
#else

//      char *lan_ifname = nvram_safe_get("lan_ifname");
  char *interface, *action;

  if (!(interface = getenv ("INTERFACE")) || !(action = getenv ("ACTION")))
    return EINVAL;

  if (strncmp (interface, "wds", 3))
    return 0;
  cprintf ("action: %s\n", action);
  if (!strcmp (action, "register"))
    {
#ifdef HAVE_MICRO
      br_init ();
#endif
      /* Bring up the interface and add to the bridge */
      ifconfig (interface, IFUP, NULL, NULL);
      sleep (2);

      /* Bridge WDS interfaces if lazywds active */

      if (!strncmp (interface, "wds", 3) && nvram_match ("wl_lazywds", "1"))
	br_add_interface ("br0", interface);	//eval ("brctl", "addif", "br0", interface);
      /* Notify NAS of adding the interface */
      sleep (5);
#ifndef HAVE_MADWIFI
      notify_nas ("lan", interface, "up");
#endif
      if (nvram_match ("lan_stp", "0"))
	br_set_stp_state ("br0", 0);	//system ("/usr/sbin/brctl stp br0 off");
      else
	br_set_stp_state ("br0", 1);	//system ("/usr/sbin/brctl stp br0 off");
#ifdef HAVE_MICRO
      br_shutdown ();
#endif

    }
  cprintf ("config done()\n");
  return 0;
#endif
}

int
init_mtu (char *wan_proto)
{
  if (strcmp (wan_proto, "pppoe") == 0)
    {				// 576 < mtu < 1454(linksys japan) | 1492(other)
      if (nvram_match ("mtu_enable", "0"))
	{			// Auto
	  nvram_set ("mtu_enable", "1");
#if COUNTRY == JAPAN
	  nvram_set ("wan_mtu", "1454");	// set max value
#else
	  nvram_set ("wan_mtu", "1492");	// set max value
#endif


	}
      else
	{			// Manual
#if COUNTRY == JAPAN
	  if (atoi (nvram_safe_get ("wan_mtu")) > 1454)
	    {
	      nvram_set ("wan_mtu", "1454");
	    }
#else
	  if (atoi (nvram_safe_get ("wan_mtu")) > 1492)
	    {
	      nvram_set ("wan_mtu", "1492");
	    }
#endif
	  if (atoi (nvram_safe_get ("wan_mtu")) < 576)
	    {
	      nvram_set ("wan_mtu", "576");
	    }
	}
    }
  else if (strcmp (wan_proto, "pptp") == 0 || strcmp (wan_proto, "l2tp") == 0)
    {				// 1200 < mtu < 1400 (1460)
      if (nvram_match ("mtu_enable", "0"))
	{			// Auto
	  nvram_set ("mtu_enable", "1");
	  nvram_set ("wan_mtu", "1460");	// set max value (linksys request to set to 1460) 2003/06/23
	}
      else
	{			// Manual
	  if (atoi (nvram_safe_get ("wan_mtu")) > 1460)
	    {
	      nvram_set ("wan_mtu", "1460");
	    }
	  if (atoi (nvram_safe_get ("wan_mtu")) < 1200)
	    {
	      nvram_set ("wan_mtu", "1200");
	    }
	}
    }
  else
    {				// 576 < mtu < 1500
      if (nvram_match ("mtu_enable", "0"))
	{			// Auto
	  nvram_set ("wan_mtu", "1500");	// set max value
	}
      else
	{			// Manual
	  if (atoi (nvram_safe_get ("wan_mtu")) > 1500)
	    {
	      nvram_set ("wan_mtu", "1500");
	    }
	  if (atoi (nvram_safe_get ("wan_mtu")) < 576)
	    {
	      nvram_set ("wan_mtu", "576");
	    }
	}
    }
  return 0;
}

#ifndef HAVE_MADWIFI
void
start_wds_check (void)
{
  int s = 0;

  /* Sveasoft - Bring up and configure wds interfaces */
  /* logic - if separate ip defined bring it up */
  /*         else if flagged for br1 and br1 is enabled add to br1 */
  /*         else add it to the br0 bridge */
  for (s = 1; s <= MAX_WDS_DEVS; s++)
    {
      char wdsvarname[32] = { 0 };
      char wdsdevname[32] = { 0 };
      char *dev;
      struct ifreq ifr;


      sprintf (wdsvarname, "wl_wds%d_enable", s);
      sprintf (wdsdevname, "wl_wds%d_if", s);
      dev = nvram_safe_get (wdsdevname);

      if (nvram_invmatch (wdsvarname, "1"))
	continue;

      memset (&ifr, 0, sizeof (struct ifreq));

      snprintf (ifr.ifr_name, IFNAMSIZ, wdsdevname);
      ioctl (s, SIOCGIFFLAGS, &ifr);

      if ((ifr.ifr_flags & (IFF_RUNNING | IFF_UP)) == (IFF_RUNNING | IFF_UP))
	continue;

      /* P2P WDS type */
      if (nvram_match (wdsvarname, "1"))
	{
	  char wdsip[32] = { 0 };
	  char wdsbc[32] = { 0 };
	  char wdsnm[32] = { 0 };

	  snprintf (wdsip, 31, "wl_wds%d_ipaddr", s);
	  snprintf (wdsnm, 31, "wl_wds%d_netmask", s);

	  snprintf (wdsbc, 31, "%s", nvram_safe_get (wdsip));
	  get_broadcast (wdsbc, nvram_safe_get (wdsnm));
	  eval ("ifconfig", dev, nvram_safe_get (wdsip), "broadcast", wdsbc,
		"netmask", nvram_safe_get (wdsnm), "up");
	}
      /* Subnet WDS type */
      else if (nvram_match (wdsvarname, "2")
	       && nvram_match ("wl_br1_enable", "1"))
	{
	  eval ("ifconfig", dev, "up");
#ifdef HAVE_MICRO
	  br_init ();
#endif

	  br_add_interface ("br1", dev);
#ifdef HAVE_MICRO
	  br_shutdown ();
#endif

	  //  eval("killall","-9","nas");
	  //eval ("brctl", "addif", "br1", dev);
//        notify_nas ("lan", "br1", "up");
	}
      /* LAN WDS type */
      else if (nvram_match (wdsvarname, "3"))
	{
	  eval ("ifconfig", dev, "up");
#ifdef HAVE_MICRO
	  br_init ();
#endif
	  br_add_interface ("br0", dev);
//        eval("killall","-9","nas");
//        eval ("brctl", "addif", "br0", dev);
#ifdef HAVE_MICRO
	  br_shutdown ();
#endif

//        notify_nas ("lan", "br0", "up");
	}

    }

  if (nvram_match ("lan_stp", "0"))
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_set_stp_state ("br0", 0);
#ifdef HAVE_MICRO
      br_shutdown ();
#endif

    }
  else
    {
#ifdef HAVE_MICRO
      br_init ();
#endif

      br_set_stp_state ("br0", 1);
#ifdef HAVE_MICRO
      br_shutdown ();
#endif

    }

  //system ("/usr/sbin/brctl stp br0 off");

  return;
}
#endif
