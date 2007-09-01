//#define CDEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <resolv.h>
#include <signal.h>



#include <utils.h>
#include <wlutils.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <cy_conf.h>
#include <code_pattern.h>
#include <bcmdevs.h>
#include <net/route.h>
#include <cy_conf.h>
#include <bcmdevs.h>
#include <linux/if_ether.h>
//#include <linux/mii.h>
#include <linux/sockios.h>

#define SIOCGMIIREG	0x8948	/* Read MII PHY register.       */
#define SIOCSMIIREG	0x8949	/* Write MII PHY register.      */
void show_hw_type (int type);

struct mii_ioctl_data
{
  unsigned short phy_id;
  unsigned short reg_num;
  unsigned short val_in;
  unsigned short val_out;
};


int
getcpurev (void)
{
  FILE *fp = fopen ("/proc/cpuinfo", "rb");
  if (fp == NULL)
    {
      return -1;
    }
  int cnt = 0;
  int b = 0;
  while (b != EOF)
    {
      b = getc (fp);
      if (b == ':')
	cnt++;
      if (cnt == 3)
	{
	  getc (fp);
	  char cpurev[13];
	  int i = 0;
	  for (i = 0; i < 12; i++)
	    cpurev[i] = getc (fp);
	  cpurev[i] = 0;
	  fclose (fp);
	  if (!strcmp (cpurev, "BCM4710 V0.0"))	//old 125 MHz
	    return 0;
	  if (!strcmp (cpurev, "BCM3302 V0.6"))	//4704
	    return 6;
	  if (!strcmp (cpurev, "BCM3302 V0.7"))
	    return 7;
	  if (!strcmp (cpurev, "BCM3302 V0.8"))
	    return 8;
	  return -1;
	}
    }
  fclose (fp);
  return -1;
}

int
startswith (char *source, char *cmp)
{
  int i;
  if (cmp == NULL)
    return 0;
  if (source == NULL)
    return 0;
  int slen = strlen (source);
  int clen = strlen (cmp);
  if (clen > slen)
    return 0;

  for (i = 0; i < clen; i++)
    if (source[i] != cmp[i])
      return 0;
  return 1;
}


void
setRouter (char *name)
{
#ifdef HAVE_POWERNOC_WORT54G
  nvram_set (NVROUTER, "WORT54G");
#elif HAVE_POWERNOC_WOAP54G
  nvram_set (NVROUTER, "WOAP54G");
#elif HAVE_OMNI
  nvram_set (NVROUTER, "Omni Wifi Router");
#elif HAVE_MAKSAT
  nvram_set (NVROUTER, "MAKSAT");
#else
  if (name)
    nvram_set (NVROUTER, name);
#endif
}

char *
getRouter ()
{
  char *n = nvram_get (NVROUTER);
  return n != NULL ? n : "Unknown Model";
}



int
internal_getRouterBrand ()
{

#ifdef HAVE_RB500
  setRouter ("Mikrotik RB500");
  return ROUTER_BOARD_500;
#elif HAVE_GEMTEK
  setRouter ("SuperGerry");
  return ROUTER_SUPERGERRY;
#elif HAVE_GATEWORX
  struct mii_ioctl_data *data;
  struct ifreq iwr;
  int s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
    {
      fprintf (stderr, "socket(SOCK_DRAGM)\n");
      setRouter ("Avila Gateworks");
      return ROUTER_BOARD_GATEWORX;
    }
  (void) strncpy (iwr.ifr_name, "ixp1", sizeof ("ixp1"));
  data = (struct mii_ioctl_data *) &iwr.ifr_data;
  data->phy_id = 1;
#define IX_ETH_ACC_MII_PHY_ID1_REG  0x2	/* PHY identifier 1 Register */
#define IX_ETH_ACC_MII_PHY_ID2_REG  0x3	/* PHY identifier 2 Register */
  data->reg_num = IX_ETH_ACC_MII_PHY_ID1_REG;
  ioctl (s, SIOCGMIIREG, &iwr);
  data->phy_id = 1;
  data->reg_num = IX_ETH_ACC_MII_PHY_ID1_REG;
  ioctl (s, SIOCGMIIREG, &iwr);
  int reg1 = data->val_out;
  data->phy_id = 1;
  data->reg_num = IX_ETH_ACC_MII_PHY_ID2_REG;
  ioctl (s, SIOCGMIIREG, &iwr);
  int reg2 = data->val_out;
  close (s);
  fprintf (stderr, "phy id %X:%X\n", reg1, reg2);
  if (reg1 == 0x2000 && reg2 == 0x5c90)
    {
      setRouter ("Avila GW2347");
      return ROUTER_BOARD_GATEWORX_SWAP;
    }
  else if (reg1 == 0x13 && reg2 == 0x7a11)
    {
      setRouter ("Avila GW2348-4/2");
      return ROUTER_BOARD_GATEWORX;
    }
  else if (reg1 == 0x22 && reg2 == 0x1450)	//kendin switch 
    {
      setRouter ("Avila GW2345");
      return ROUTER_BOARD_GATEWORX_GW2345;
    }
  else
    {
      setRouter ("Unknown");
      return ROUTER_BOARD_GATEWORX;
    }
#elif HAVE_X86
  setRouter ("Generic X86");
  return ROUTER_BOARD_X86;
#elif HAVE_XSCALE
  setRouter ("NewMedia Dual A/B/G");
  return ROUTER_BOARD_XSCALE;
#elif HAVE_MAGICBOX
  setRouter ("MagicBox");
  return ROUTER_BOARD_MAGICBOX;
#elif HAVE_FONERA
  struct mii_ioctl_data *data;
  struct ifreq iwr;
  int s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
    {
      fprintf (stderr, "socket(SOCK_DRAGM)\n");
      setRouter ("Fonera 2100/2200");
      return ROUTER_BOARD_FONERA;
    }
  (void) strncpy (iwr.ifr_name, "eth0", sizeof ("eth0"));
  data = (struct mii_ioctl_data *) &iwr.ifr_data;
  data->phy_id = 0x1f;
  data->reg_num = 0x1;
  ioctl (s, SIOCGMIIREG, &iwr);
  close(s);
  if (data->val_out&0xffff!=0xffff) // marvell phy
    {
    setRouter ("Fonera+");
    return ROUTER_BOARD_FONERA2200;    
    }else
    { 
    setRouter ("Fonera 2100/2200");
    return ROUTER_BOARD_FONERA;
    }
#elif HAVE_MERAKI
  setRouter ("Meraki Mini");
  return ROUTER_BOARD_MERAKI;
#elif HAVE_LS2
  setRouter ("Ubiquiti Litestation 2");
  return ROUTER_BOARD_LS2;
#elif HAVE_WHRAG108
  setRouter ("Buffalo WHR-HP-AG108");
  return ROUTER_BOARD_WHRAG108;
#elif HAVE_PB42
  setRouter ("Atheros PB42");
  return ROUTER_BOARD_PB42;
#elif HAVE_TW6600
  setRouter ("TW6600");
  return ROUTER_BOARD_TW6600;
#elif HAVE_CA8
  setRouter ("Wistron CA8-4");
  return ROUTER_BOARD_CA8;
#else

  uint boardnum = strtoul (nvram_safe_get ("boardnum"), NULL, 0);

  if (boardnum == 42 && nvram_match ("boardtype", "bcm94710ap"))
    {
      cprintf ("router is buffalo\n");
      setRouter ("Buffalo WBR-G54 / WLA-G54");
      return ROUTER_BUFFALO_WBR54G;
    }
#ifndef HAVE_BUFFALO
  if (nvram_match ("boardnum", "mn700") &&
      nvram_match ("boardtype", "bcm94710ap"))
    {
      cprintf ("router is Microsoft MN-700\n");
      setRouter ("Microsoft MN-700");
      return ROUTER_MICROSOFT_MN700;
    }

  if (nvram_match ("boardnum", "asusX") &&
      nvram_match ("boardtype", "bcm94710dev"))
    {
      cprintf ("router is Asus WL300g / WL500g\n");
      setRouter ("Asus WL-300g / WL-500g");
      return ROUTER_ASUS_WL500G;
    }

  if (boardnum == 44 && nvram_match ("boardtype", "bcm94710ap"))
    {
      cprintf ("router is Dell TrueMobile 2300\n");
      setRouter ("Dell TrueMobile 2300");
      return ROUTER_DELL_TRUEMOBILE_2300;
    }
#endif

  if (boardnum == 100 && nvram_match ("boardtype", "bcm94710dev"))
    {
      cprintf ("router is buffalo\n");
      setRouter ("Buffalo WLA-G54C");
      return ROUTER_BUFFALO_WLAG54C;
    }

#ifndef HAVE_BUFFALO
  if (boardnum == 45 && nvram_match ("boardtype", "bcm95365r"))
    {
      cprintf ("router is Asus WL-500GD\n");
      setRouter ("Asus WL-500g Deluxe");
      return ROUTER_ASUS_WL500GD;
    }
#endif
  if (nvram_match ("boardnum", "00") &&
      nvram_match ("boardtype", "0x0101") && nvram_match ("boardrev", "0x10"))
    {
      cprintf ("router is Buffalo wbr2\n");
      setRouter ("Buffalo WBR2-G54 / WBR2-G54S");
      return ROUTER_BUFFALO_WBR2G54S;
    }

  if (boardnum == 2 &&
      nvram_match ("boardtype", "0x0101") && nvram_match ("boardrev", "0x10"))
    {
      cprintf ("router is buffalo wla2-g54c\n");
      setRouter ("Buffalo WLA2-G54C / WLI3-TX1-G54");
      return ROUTER_BUFFALO_WLA2G54C;
    }
  if (boardnum == 0 && nvram_match("melco_id","29090") && nvram_match("boardflags","0x0010") && nvram_match("boardrev","0x10"))
    {
      cprintf ("router is Buffalo WLAH-G54\n");
      setRouter ("Buffalo WLAH-G54");
      return ROUTER_BUFFALO_WLAH_G54;    
    
    }
  if (boardnum == 0 && nvram_match("melco_id","31070") && nvram_match("boardflags","0x2288") && nvram_match("boardrev","0x10"))
    {
      cprintf ("router is Buffalo WAPM-HP-AM54G54\n");
      setRouter ("Buffalo WAPM-HP-AM54G54");
      return ROUTER_BUFFALO_WAPM_HP_AM54G54;    
    }
  if (nvram_match ("boardnum", "00") && nvram_match ("boardrev", "0x11") && nvram_match ("boardtype", "0x048e") && nvram_match ("melco_id", "32093")) 	
    {
      cprintf ("router is Buffalo WHR-G125\n");
      setRouter ("Buffalo WHR-G125");
      return ROUTER_BUFFALO_WHRG54S;
    }
       
  if (nvram_match ("boardnum", "00") &&
      nvram_match ("boardrev", "0x13") && nvram_match ("boardtype", "0x467"))
    {
      if (nvram_match ("boardflags", "0x1658") || nvram_match ("boardflags", "0x2658"))	//maybe the amp is on, it's hp anyway ???
	{
	  cprintf ("router is Buffalo WLI-TX4-G54HP\n");
	  setRouter ("Buffalo WLI-TX4-G54HP");
	  return ROUTER_BUFFALO_WLI_TX4_G54HP;
	}
      if (nvram_match ("boardflags", "0x2758")
	  && !nvram_match ("buffalo_hp", "1"))
	{
	  cprintf ("router is Buffalo WHR-G54S\n");
	  setRouter ("Buffalo WHR-G54S");
	  return ROUTER_BUFFALO_WHRG54S;
	}
#ifndef HAVE_BUFFALO
      if (nvram_match ("boardflags", "0x1758")
	  && !nvram_match ("buffalo_hp", "1"))
	{
	  cprintf ("router is Buffalo WHR-HP-G54\n");
	  nvram_set ("boardflags", "0x2758");
	  nvram_set ("buffalo_hp", "1");
	  setRouter ("Buffalo WHR-HP-G54");
	  return ROUTER_BUFFALO_WHRG54S;
	}
      if (nvram_match ("buffalo_hp", "1"))
	{
	  cprintf ("router is Buffalo WHR-HP-G54\n");
	  setRouter ("Buffalo WHR-HP-G54");
	  return ROUTER_BUFFALO_WHRG54S;
	}
#else
      if (nvram_match ("boardflags", "0x1758")
	  && !nvram_match ("buffalo_hp", "1"))
	{
	  cprintf ("router is Buffalo WHR-HP-G54DD\n");
//	  nvram_set ("boardflags", "0x2758");  /* removed, to be FCC/CE valid */
	  nvram_set ("buffalo_hp", "1");
	  setRouter ("Buffalo WHR-HP-G54DD");
	  return ROUTER_BUFFALO_WHRG54S;
	}
      if (nvram_match ("buffalo_hp", "1"))
	{
	  cprintf ("router is Buffalo WHR-HP-G54DD\n");
	  setRouter ("Buffalo WHR-HP-G54DD");
	  return ROUTER_BUFFALO_WHRG54S;
	}
#endif
    }

  if (nvram_match ("boardnum", "00") &&
      nvram_match ("boardrev", "0x10") && nvram_match ("boardtype", "0x470"))
    {
      cprintf ("router is Buffalo WHR-AM54G54\n");
      setRouter ("Buffalo WHR-AM54G54");
      return ROUTER_BUFFALO_WHRAM54G54;
    }


  if (boardnum == 42 && nvram_match ("boardtype", "0x042f"))
    {
      uint melco_id = strtoul (nvram_safe_get ("melco_id"), NULL, 0);

      if (nvram_match ("product_name", "WZR-RS-G54") || melco_id == 30083)
	{
	  cprintf ("router is Buffalo WZR-RS-G54\n");
	  setRouter ("Buffalo WZR-RS-G54");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
      if (nvram_match ("product_name", "WZR-HP-G54") || melco_id == 30026)
	{
	  cprintf ("router is Buffalo WZR-HP-G54\n");
	  setRouter ("Buffalo WZR-HP-G54");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
      if (nvram_match ("product_name", "WZR-G54") || melco_id == 30061)
	{
	  cprintf ("router is Buffalo WZR-G54\n");
	  setRouter ("Buffalo WZR-G54");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
      if (nvram_match ("melco_id", "290441dd"))
	{
	  cprintf ("router is Buffalo WHR2-A54G54\n");
	  setRouter ("Buffalo WHR2-A54G54");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
      if (nvram_match ("product_name", "WHR3-AG54")
	  || nvram_match ("product_name", "WHR3-B11") || melco_id == 29130)
	{
	  cprintf ("router is Buffalo WHR3-AG54\n");
	  setRouter ("Buffalo WHR3-AG54");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
      if (nvram_match ("product_name", "WVR-G54-NF") || melco_id == 28100)
	{
	  cprintf ("router is Buffalo WVR-G54-NF\n");
	  setRouter ("Buffalo WVR-G54-NF");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
      if (melco_id > 0)		//e.g. 29115
	{
	  cprintf ("router is Buffalo WZR series\n");
	  setRouter ("Buffalo WZR series");
	  return ROUTER_BUFFALO_WZRRSG54;
	}
    }

#ifndef HAVE_BUFFALO
  if (boardnum == 42 &&
      nvram_match ("boardtype", "0x042f") && nvram_match ("boardrev", "0x10"))
//      nvram_match ("boardflags","0x0018"))
    {
      cprintf ("router is Linksys WRTSL54GS\n");
      setRouter ("Linksys WRTSL54GS");
      return ROUTER_WRTSL54GS;
    }

  if (boardnum == 42 && nvram_match ("boardtype", "0x0101")
      && nvram_match ("boardrev", "0x10") && nvram_match ("boot_ver", "v3.6"))
    {
      cprintf ("router is Linksys WRT54G3G\n");
      setRouter ("Linksys WRT54G3G");
      return ROUTER_WRT54G3G;
    }

  if (boardnum == 45 &&
      nvram_match ("boardtype", "0x042f") && nvram_match ("boardrev", "0x10"))
    {
      cprintf ("router is Asus WL-500g Premium\n");
      setRouter ("Asus WL-500g Premium");
      return ROUTER_ASUS_WL500G_PRE;
    }


  char *et0 = nvram_safe_get ("et0macaddr");

  if (boardnum == 100 && nvram_match ("boardtype", "bcm94710r4"))
    {
      if (startswith (et0, "00:11:50"))
	{
	  cprintf ("router is Belkin F5D7130 / F5D7330\n");
	  setRouter ("Belkin F5D7130 / F5D7330");
	  return ROUTER_RT210W;
	}
      if (startswith (et0, "00:30:BD") || startswith (et0, "00:30:bd"))
	{
	  cprintf ("router is Belkin F5D7230 v1000\n");
	  setRouter ("Belkin F5D7230-4 v1000");
	  return ROUTER_RT210W;
	}
      if (startswith (et0, "00:01:E3") ||
	  startswith (et0, "00:01:e3") || startswith (et0, "00:90:96"))
	{
	  cprintf ("router is Siemens\n");
	  setRouter ("Siemens SE505 v1");
	  return ROUTER_RT210W;
	}
      else
	{
	  cprintf ("router is Askey generic\n");
	  setRouter ("RT210W generic");
	  return ROUTER_RT210W;
	}
    }
      
	if (nvram_match ("boardtype", "bcm94710r4") && nvram_match ("boardnum", ""))
    {
      cprintf ("router is Askey board RT2100W\n");
	  setRouter ("Askey board RT2100W-D65)");
	  return ROUTER_BRCM4702_GENERIC;
	}
	
	if (nvram_match ("boardtype", "0x0100") && nvram_match ("boardnum", ""))
    {
      cprintf ("router is Askey board RT2206D\n");
	  setRouter ("Askey board RT2206D-D56");
	  return ROUTER_BRCM4702_GENERIC;
	}

  if (nvram_match ("boardtype", "0x0101"))
    {
      if (startswith (et0, "00:11:50") ||
	  startswith (et0, "00:30:BD") || startswith (et0, "00:30:bd"))
	{
	  cprintf ("router is Belkin F5D7230-4 v1444\n");
	  setRouter ("Belkin F5D7230-4 v1444");
	  return ROUTER_RT480W;
	}
      if (startswith (et0, "00:01:E3") ||
	  startswith (et0, "00:01:e3") || startswith (et0, "00:90:96"))
	{
	  cprintf ("router is Siemens / Askey\n");
	  setRouter ("Siemens SE505 v2");
	  return ROUTER_RT480W;
	}
    }
    
 if (nvram_match ("boardtype", "0x456"))
    {
      if (startswith (et0, "00:11:50") ||
	  startswith (et0, "00:30:BD") || startswith (et0, "00:30:bd"))
	{
	  cprintf ("router is Belkin F5D7231-4 v1212UK\n");
	  setRouter ("Belkin F5D7231-4 v1212UK");
	  return ROUTER_RT480W;
	}
    }
#endif
  if (boardnum == 2 && nvram_match ("boardtype", "bcm94710dev") && nvram_match ("melco_id", "29016"))	//Buffalo WLI2-TX1-G54)
    {
      cprintf ("router is Buffalo WLI2-TX1-G54\n");
      setRouter ("Buffalo WLI2-TX1-G54");
      return ROUTER_BUFFALO_WLI2_TX1_G54;
    }
#ifndef HAVE_BUFFALO

  if (boardnum == 2 && nvram_match ("GemtekPmonVer", "10"))
    {
      if (startswith (et0, "00:0C:E5") ||
	  startswith (et0, "00:0c:e5") ||
	  startswith (et0, "00:0C:10") ||
	  startswith (et0, "00:0c:10") ||
	  startswith (et0, "00:0C:11") || startswith (et0, "00:0c:11"))
	{
	  cprintf ("router Motorola WE800G v1\n");
	  setRouter ("Motorola WE800G v1");
	  return ROUTER_MOTOROLA_WE800G;
	}
      else
	{
	  cprintf ("router is Linksys wap54g v1\n");
	  setRouter ("Linksys WAP54G v1");
	  return ROUTER_WAP54G_V1;
	}
    }

  if (boardnum == 2 && nvram_match ("GemtekPmonVer", "1"))
    {
      cprintf ("router is Sitecom wl105b\n");
      setRouter ("Sitecom WL-105(b)");
      return ROUTER_SITECOM_WL105B;
    }

  if (nvram_match ("GemtekPmonVer", "9"))	//Must be Motorola wr850g v1 or we800g v1 or Linksys wrt55ag v1
    {
      if (startswith (et0, "00:0C:E5") ||
	  startswith (et0, "00:0c:e5") ||
	  startswith (et0, "00:0C:10") ||
	  startswith (et0, "00:0c:10") ||
	  startswith (et0, "00:0C:11") ||
	  startswith (et0, "00:0c:11") ||
	  startswith (et0, "00:11:22") ||
	  startswith (et0, "00:0C:90") || startswith (et0, "00:0c:90"))
	{
	  if (!strlen (nvram_safe_get ("phyid_num")))
	    {
	      eval ("insmod", "switch-core");	//get phy type
	      eval ("insmod", "switch-robo");
	      eval ("rmmod", "switch-robo");
	      eval ("rmmod", "switch-core");
	      nvram_set ("boardnum", "2");
	      nvram_set ("boardtype", "bcm94710dev");
	    }
	  if (nvram_match ("phyid_num", "0x00000000"))
	    {
	      cprintf ("router Motorola WE800G v1\n");
	      setRouter ("Motorola WE800G v1");
	      return ROUTER_MOTOROLA_WE800G;
	    }
	  else			//phyid_num == 0xffffffff
	    {
	      cprintf ("router Motorola WR850G v1\n");
	      setRouter ("Motorola WR850G v1");
	      return ROUTER_MOTOROLA_V1;
	    }
	}
      else
	{
	  cprintf ("router is linksys WRT55AG\n");
	  setRouter ("Linksys WRT55AG v1");
	  return ROUTER_LINKSYS_WRT55AG;
	}
    }
#endif
  if (boardnum == 0 && nvram_match ("boardtype", "0x478") && nvram_match ("cardbus", "0") && nvram_match("boardrev","0x10") && nvram_match("boardflags","0x110") && nvram_match("melco_id","32027"))
    {
      setRouter ("Buffalo WZR-G144NH");
      return ROUTER_BUFFALO_WZRG144NH;
    }


  if (boardnum == 20060330 && nvram_match ("boardtype", "0x0472"))
    {
      setRouter ("Buffalo WZR-G300N");
      return ROUTER_BUFFALO_WZRG300N;
    }
#ifndef HAVE_BUFFALO

  if (boardnum == 8 &&
      nvram_match ("boardtype", "0x0472") && nvram_match ("cardbus", "1"))
    {
      setRouter ("Netgear WNR834B");
      return ROUTER_NETGEAR_WNR834B;
    }

  if (boardnum == 42 &&
      nvram_match ("boardtype", "0x0472") && nvram_match ("cardbus", "1"))
    {
      if (nvram_match ("boot_hw_model", "WRT150N"))
	{
	  setRouter ("Linksys WRT150N");
	  return ROUTER_WRT150N;
	}
      else
	{
	  setRouter ("Linksys WRT300N v1");
	  return ROUTER_WRT300N;
	}
    }

  if (boardnum == 42 &&
      nvram_match ("boardtype", "0x478") && nvram_match ("cardbus", "1"))
    {
      setRouter ("Linksys WRT350N");
      return ROUTER_WRT350N;
    }

  if (boardnum == 42 && nvram_match ("boardtype", "bcm94710dev"))
    {
      setRouter ("Linksys WRT54G 1.x");
      return ROUTER_WRT54G1X;
    }

  if (boardnum == 1 && nvram_match ("boardtype", "0x0446"))
    {
      setRouter ("U.S.Robotics USR5430");
      return ROUTER_USR_5430;
    }
      
  if (boardnum == 10496 && nvram_match ("boardtype", "0x456"))
    {
      setRouter ("U.S.Robotics USR5461");
      return ROUTER_USR_5461;
    } 
      
  if (boardnum == 1024 && nvram_match ("boardtype", "0x0446"))
    {
      char *cfe = nvram_safe_get ("cfe_version");
      if (strstr (cfe, "iewsonic"))
	{
	  cprintf ("router is Viewsonic WAPBR-100\n");
	  setRouter ("Viewsonic WAPBR-100");
	  return ROUTER_VIEWSONIC_WAPBR_100;
	}
      else
	{
	  cprintf ("router is Linksys WAP54G v2\n");
	  setRouter ("Linksys WAP54G v2");
	  return ROUTER_WAP54G_V2;
	}
    }

  if (nvram_invmatch ("CFEver", ""))
    {
      char *cfe = nvram_safe_get ("CFEver");
      if (!strncmp (cfe, "MotoWR", 6))
	{
	  cprintf ("router is motorola\n");
	  setRouter ("Motorola WR850G v2/v3");
	  return ROUTER_MOTOROLA;
	}
    }

  if (boardnum == 44 &&
      (nvram_match ("boardtype", "0x0101")
       || nvram_match ("boardtype", "0x0101\r")))
    {
      cprintf ("router is Dell TrueMobile 2300 v2\n");
      setRouter ("Dell TrueMobile 2300 v2");
      return ROUTER_DELL_TRUEMOBILE_2300_V2;
    }
#endif
  if (nvram_match ("boardtype", "bcm94710ap"))
    {
      cprintf ("router is Buffalo old 4710\n");
      setRouter ("Buffalo WBR-B11");
      return ROUTER_BUFFALO_WBR54G;
    }
#ifndef HAVE_BUFFALO

  setRouter ("Linksys WRT54G/GL/GS");
  cprintf ("router is wrt54g\n");
  return ROUTER_WRT54G;
#else
  eval("event","3","1","15");
  return 0;
#endif
#endif

}

int
has_mimo ()
{
  if (nvram_match ("wl0_phytypes", "n"))
    return 1;
  return 0;
}

static int router_type = -1;
int
getRouterBrand ()
{
  if (router_type == -1)
    router_type = internal_getRouterBrand ();
  return router_type;
}


int
diag_led_4702 (int type, int act)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42)
  return 0;
#else
  if (act == START_LED)
    {
      switch (type)
	{
	case DMZ:
	  system2 ("echo 1 > /proc/sys/diag");
	  break;
	}
    }
  else
    {
      switch (type)
	{
	case DMZ:
	  system2 ("echo 0 > /proc/sys/diag");
	  break;
	}
    }
  return 0;
#endif
}

int
C_led_4702 (int i)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42)
  return 0;
#else
  FILE *fp;
  char string[10];
  int flg;

  memset (string, 0, 10);
  /* get diag before set */
  if ((fp = fopen ("/proc/sys/diag", "r")))
    {
      fgets (string, sizeof (string), fp);
      fclose (fp);
    }
  else
    perror ("/proc/sys/diag");

  if (i)
    flg = atoi (string) | 0x10;
  else
    flg = atoi (string) & 0xef;

  memset (string, 0, 10);
  sprintf (string, "%d", flg);
  if ((fp = fopen ("/proc/sys/diag", "w")))
    {
      fputs (string, fp);
      fclose (fp);
    }
  else
    perror ("/proc/sys/diag");

  return 0;
#endif
}

unsigned int
read_gpio (char *device)
{
  FILE *fp;
  unsigned int val;

  if ((fp = fopen (device, "r")))
    {
      fread (&val, 4, 1, fp);
      fclose (fp);
      //fprintf(stderr, "----- gpio %s = [%X]\n",device,val); 
      return val;
    }
  else
    {
      perror (device);
      return 0;
    }
}

unsigned int
write_gpio (char *device, unsigned int val)
{
  FILE *fp;

  if ((fp = fopen (device, "w")))
    {
      fwrite (&val, 4, 1, fp);
      fclose (fp);
      //fprintf(stderr, "----- set gpio %s = [%X]\n",device,val); 
      return 1;
    }
  else
    {
      perror (device);
      return 0;
    }
}

static char hw_error = 0;
int
diag_led_4704 (int type, int act)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_MERAKI)|| defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42)
  return 0;
#else
  unsigned int control, in, outen, out;

#ifdef BCM94712AGR
  /* The router will crash, if we load the code into broadcom demo board. */
  return 1;
#endif
  //int brand;
  control = read_gpio ("/dev/gpio/control");
  in = read_gpio ("/dev/gpio/in");
  out = read_gpio ("/dev/gpio/out");
  outen = read_gpio ("/dev/gpio/outen");

  write_gpio ("/dev/gpio/outen", (outen & 0x7c) | 0x83);
  switch (type)
    {
    case DIAG:			// GPIO 1
      if (hw_error)
	{
	  write_gpio ("/dev/gpio/out", (out & 0x7c) | 0x00);
	  return 1;
	}

      if (act == STOP_LED)
	{			// stop blinking
	  write_gpio ("/dev/gpio/out", (out & 0x7c) | 0x83);
	  //cprintf("tallest:=====( DIAG STOP_LED !!)=====\n");
	}
      else if (act == START_LED)
	{			// start blinking
	  write_gpio ("/dev/gpio/out", (out & 0x7c) | 0x81);
	  //cprintf("tallest:=====( DIAG START_LED !!)=====\n");
	}
      else if (act == MALFUNCTION_LED)
	{			// start blinking
	  write_gpio ("/dev/gpio/out", (out & 0x7c) | 0x00);
	  hw_error = 1;
	  //cprintf("tallest:=====( DIAG MALFUNCTION_LED !!)=====\n");
	}
      break;

    }
  return 1;
#endif
}

int
diag_led_4712 (int type, int act)
{
  unsigned int control, in, outen, out, ctr_mask, out_mask;
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA)|| defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42)
  return 0;
#else


#ifdef BCM94712AGR
  /* The router will crash, if we load the code into broadcom demo board. */
  return 1;
#endif
  control = read_gpio ("/dev/gpio/control");
  in = read_gpio ("/dev/gpio/in");
  out = read_gpio ("/dev/gpio/out");
  outen = read_gpio ("/dev/gpio/outen");

  ctr_mask = ~(1 << type);
  out_mask = (1 << type);

  write_gpio ("/dev/gpio/control", control & ctr_mask);
  write_gpio ("/dev/gpio/outen", outen | out_mask);

  if (act == STOP_LED)
    {				// stop blinking
      //cprintf("%s: Stop GPIO %d\n", __FUNCTION__, type);
      write_gpio ("/dev/gpio/out", out | out_mask);
    }
  else if (act == START_LED)
    {				// start blinking
      //cprintf("%s: Start GPIO %d\n", __FUNCTION__, type);
      write_gpio ("/dev/gpio/out", out & ctr_mask);
    }

  return 1;
#endif
}

int
C_led_4712 (int i)
{
  if (i == 1)
    return diag_led (DIAG, START_LED);
  else
    return diag_led (DIAG, STOP_LED);
}

int
C_led (int i)
{
//show_hw_type(check_hw_type());

  if (getRouterBrand () == ROUTER_WRT54G1X
      || getRouterBrand () == ROUTER_LINKSYS_WRT55AG)
    return C_led_4702 (i);
  else if (getRouterBrand () == ROUTER_WRT54G)
    return C_led_4712 (i);
  else
    return 0;
}

int
diag_led (int type, int act)
{
  int brand = getRouterBrand ();

  if (brand == ROUTER_WRT54G || brand == ROUTER_WRT54G3G)
    return diag_led_4712 (type, act);
  else if (brand == ROUTER_WRT54G1X || brand == ROUTER_LINKSYS_WRT55AG)
    return diag_led_4702 (type, act);
  else if ((brand == ROUTER_WRTSL54GS || brand == ROUTER_WRT350N || brand == ROUTER_BUFFALO_WZRG144NH)
	   && type == DIAG)
    return diag_led_4704 (type, act);
  else
    {
      if (type == DMZ)
	{
	  if (act == START_LED)
	    return led_control (LED_DMZ, LED_ON);
	  if (act == STOP_LED)
	    return led_control (LED_DMZ, LED_OFF);
	  return 1;
	}
    }
  return 0;
}

int
led_control (int type, int act)
/* type: LED_POWER, LED_DIAG, LED_DMZ, LED_CONNECTED, LED_BRIDGE, LED_VPN, LED_SES, LED_SES2, LED_WLAN
 * act: LED_ON, LED_OFF, LED_FLASH */
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_X86) || defined(HAVE_CA8)
  return 0;
#else

#ifdef HAVE_GATEWORX
  int board = getRouterBrand ();
  char *gpio = "3";
  if (board == ROUTER_BOARD_GATEWORX_SWAP)
    gpio = "4";
#endif
  int use_gpio = 0x0f;
  int gpio_value;
  char val[4];
  char enable[16];
  char disable[16];

  int power_gpio = 0x0f;
  int diag_gpio = 0x0f;
  int dmz_gpio = 0x0f;
  int connected_gpio = 0x0f;
  int bridge_gpio = 0x0f;
  int vpn_gpio = 0x0f;
  int ses_gpio = 0x0f;		//use for SES1 (Linksys), AOSS (Buffalo) ....
  int ses2_gpio = 0x0f;
  int wlan_gpio = 0x0f;		//use this only if wlan led is not controlled by hardware!
  int v1func=0;
  switch (getRouterBrand ())	//gpio definitions here: 0xYZ, Y=0:normal, Y=1:inverted, Z:gpio number (f=disabled)
    {
#ifndef HAVE_BUFFALO
    case ROUTER_WRT54G:
      power_gpio = 0x01;
      dmz_gpio = 0x17;
      connected_gpio = 0x13;	//ses orange
      ses_gpio = 0x12;		//ses white
      ses2_gpio = 0x13;		//ses orange
      break;
    case ROUTER_WRT54G1X:
      connected_gpio = 0x13;
      v1func=1;
      break;
    case ROUTER_WRT350N:
      connected_gpio = 0x13;
      power_gpio = 0x01;
      ses2_gpio = 0x13;		//ses orange                    
      break;
    case ROUTER_LINKSYS_WRT55AG:
      connected_gpio = 0x13;
      break;
#endif
    case ROUTER_BUFFALO_WBR54G:
      diag_gpio = 0x17;
      break;
    case ROUTER_BUFFALO_WBR2G54S:
      diag_gpio = 0x01;
      ses_gpio = 0x06;
      break;
    case ROUTER_BUFFALO_WLA2G54C:
      diag_gpio = 0x14;
      ses_gpio = 0x13;
      break;
    case ROUTER_BUFFALO_WLAH_G54:
      diag_gpio = 0x17;
      ses_gpio = 0x16;
    break;    
    case ROUTER_BUFFALO_WAPM_HP_AM54G54:
      diag_gpio = 0x17;
      ses_gpio = 0x11;
    break;
    case ROUTER_BOARD_WHRAG108:
      diag_gpio = 0x17;
      bridge_gpio = 0x14;
      ses_gpio = 0x10;
      break;
    case ROUTER_BOARD_TW6600:
      diag_gpio = 0x17;
      bridge_gpio = 0x14;
      ses_gpio = 0x10;
      break;
    case ROUTER_BUFFALO_WHRG54S:
    case ROUTER_BUFFALO_WLI_TX4_G54HP:
      diag_gpio = 0x17;
      bridge_gpio = 0x11;
      ses_gpio = 0x16; 
    break; 
    case ROUTER_BUFFALO_WZRRSG54:
      diag_gpio = 0x17;
      vpn_gpio = 0x11;
      ses_gpio = 0x16;
      break;
    case ROUTER_BUFFALO_WZRG300N:
      diag_gpio = 0x17;
      bridge_gpio = 0x11;
      break;
    case ROUTER_BUFFALO_WZRG144NH:
      diag_gpio = 0x13;
      bridge_gpio = 0x11;
      ses_gpio = 0x12;
      break;
#ifndef HAVE_BUFFALO
    case ROUTER_MOTOROLA:
      power_gpio = 0x01;
      diag_gpio = 0x11;
      break;
    case ROUTER_RT210W:
      power_gpio = 0x15;
      connected_gpio = 0x10;
      wlan_gpio = 0x13;
      break;
    case ROUTER_RT480W:
      power_gpio = 0x15;
      connected_gpio = 0x10;
      break;
    case ROUTER_MICROSOFT_MN700:
      power_gpio = 0x06;
      break;
    case ROUTER_ASUS_WL500GD:
      diag_gpio = 0x00;  //power led off to indicate factory defaults
      break;
    case ROUTER_ASUS_WL500G_PRE:
      power_gpio = 0x11;
      break;
    case ROUTER_WRT54G3G:
    case ROUTER_WRTSL54GS:
      power_gpio = 0x01;
      dmz_gpio = 0x10;
      connected_gpio = 0x17;	//ses orange
      ses_gpio = 0x15;		//ses white
      ses2_gpio = 0x17;		//ses orange    
      break;
    case ROUTER_MOTOROLA_WE800G:
    case ROUTER_MOTOROLA_V1:
      diag_gpio = 0x13;
      wlan_gpio = 0x11;
      bridge_gpio = 0x15;
      break;
    case ROUTER_DELL_TRUEMOBILE_2300:
    case ROUTER_DELL_TRUEMOBILE_2300_V2:
      power_gpio = 0x17;
      wlan_gpio = 0x16;
      break;
    case ROUTER_NETGEAR_WNR834B:
      power_gpio = 0x14;
      diag_gpio = 0x15;
      wlan_gpio = 0x16;
      break;
    case ROUTER_SITECOM_WL105B:
      power_gpio = 0x03;
      wlan_gpio = 0x14;
      break;

    case ROUTER_WRT300N:
      diag_gpio = 0x11;  //power led blink to indicate fac.def.
      break;
    case ROUTER_ASUS_WL500G:
      power_gpio = 0x10;
      diag_gpio = 0x00;  //power led off to indicate factory defaults
      break;
#endif
    }
if (type==LED_DIAG && v1func==1) 
    {
    if (act==LED_ON)
	C_led(1);
    else    
	C_led(0);
    }
  switch (type)
    {
    case LED_POWER:
      use_gpio = power_gpio;
      break;
    case LED_DIAG:
      use_gpio = diag_gpio;
      break;
    case LED_DMZ:
      use_gpio = dmz_gpio;
      break;
    case LED_CONNECTED:
      use_gpio = connected_gpio;
      break;
    case LED_BRIDGE:
      use_gpio = bridge_gpio;
      break;
    case LED_VPN:
      use_gpio = vpn_gpio;
      break;
    case LED_SES:
      use_gpio = ses_gpio;
      break;
    case LED_SES2:
      use_gpio = ses2_gpio;
      break;
    case LED_WLAN:
      use_gpio = wlan_gpio;
      break;
    }
#ifndef HAVE_XSCALE
  if ((use_gpio & 0x0f) != 0x0f)
    {
      gpio_value = use_gpio & 0x0f;
      sprintf (val, "%d", gpio_value);
      sprintf (enable, "%s", (use_gpio & 0x10) == 0 ? "enable" : "disable");
      sprintf (disable, "%s", (use_gpio & 0x10) == 0 ? "disable" : "enable");
#endif
      switch (act)
	{
	case LED_ON:
#ifdef HAVE_XSCALE
	  if (type == LED_CONNECTED)
	    eval ("gpio", "-w", gpio, "0");
#else
	  eval ("gpio", enable, val);
#endif
	  break;
	case LED_OFF:
#ifdef HAVE_XSCALE
	  if (type == LED_CONNECTED)
	    eval ("gpio", "-w", gpio, "1");
#else
	  eval ("gpio", disable, val);
#endif
	  break;
	case LED_FLASH:	//will lit the led for 1 sec.
#ifdef HAVE_XSCALE
	  if (type == LED_CONNECTED)
	    {
	      eval ("gpio", "-w", gpio, "0");
	      sleep (1);
	      eval ("gpio", "-w", gpio, "1");
	    }
#else
	  eval ("gpio", enable, val);
	  sleep (1);
	  eval ("gpio", disable, val);
#endif
	  break;
	}
#ifndef HAVE_XSCALE
    }
#endif
  return 1;

#endif
}


char *
get_mac_from_ip (char *ip)
{
  FILE *fp;
  char line[100];
  char ipa[50];			// ip address
  char hwa[50];			// HW address / MAC
  char mask[50];		// ntemask   
  char dev[50];			// interface
  int type;			// HW type
  int flags;			// flags
  static char mac[20];


  if ((fp = fopen ("/proc/net/arp", "r")) == NULL)
    return NULL;

  // Bypass header -- read until newline 
  if (fgets (line, sizeof (line), fp) != (char *) NULL)
    {
      // Read the ARP cache entries.
      // IP address       HW type     Flags       HW address            Mask     Device
      // 192.168.1.1      0x1         0x2         00:90:4C:21:00:2A     *        eth0
      for (; fgets (line, sizeof (line), fp);)
	{
	  if (sscanf
	      (line, "%s 0x%x 0x%x %100s %100s %100s\n", ipa, &type, &flags,
	       hwa, mask, dev) != 6)
	    continue;
	  //cprintf("ip1=[%s] ip2=[%s] mac=[%s] (flags & ATF_COM)=%d\n", ip, ipa, hwa, (flags & ATF_COM));
	  if (strcmp (ip, ipa))
	    continue;
	  //if (!(flags & ATF_COM)) {       //ATF_COM = 0x02   completed entry (ha valid)
	  strcpy (mac, hwa);
	  fclose (fp);
	  return mac;
	  //}
	}
    }

  fclose (fp);
  return "";
}

struct dns_lists *
get_dns_list (void)
{
  char list[254];
  char *next, word[254];
  struct dns_lists *dns_list = NULL;
  int i, match = 0, altdns_index = 1;

  dns_list = (struct dns_lists *) malloc (sizeof (struct dns_lists));
  memset (dns_list, 0, sizeof (struct dns_lists));

  dns_list->num_servers = 0;

  // nvram_safe_get("wan_dns") ==> Set by user
  // nvram_safe_get("wan_get_dns") ==> Get from DHCP, PPPoE or PPTP
  // The nvram_safe_get("wan_dns") priority is higher than nvram_safe_get("wan_get_dns")
  snprintf (list, sizeof (list), "%s %s %s", nvram_safe_get ("sv_localdns"),
	    nvram_safe_get ("wan_dns"), nvram_safe_get ("wan_get_dns"));
  foreach (word, list, next)
  {
    if (strcmp (word, "0.0.0.0") && strcmp (word, ""))
      {
	match = 0;
	for (i = 0; i < dns_list->num_servers; i++)
	  {			// Skip same DNS
	    if (!strcmp (dns_list->dns_server[i], word))
	      match = 1;
	  }
	if (!match)
	  {
	    snprintf (dns_list->dns_server[dns_list->num_servers],
		      sizeof (dns_list->dns_server[dns_list->num_servers]),
		      "%s", word);
	    dns_list->num_servers++;
	  }
      }
    if (dns_list->num_servers == 3)
      break;			// We only need 3 DNS entries
  }

  /* if < 3 DNS servers found, try to insert alternates */
  while (dns_list->num_servers < 3 && altdns_index <= 3)
    {
      char altdnsvar[32] = { 0 };

      snprintf (altdnsvar, 31, "altdns%d", altdns_index);

      if (strlen (nvram_safe_get (altdnsvar)) > 0)
	{
	  snprintf (dns_list->dns_server[dns_list->num_servers],
		    sizeof (dns_list->dns_server[dns_list->num_servers]),
		    "%s", nvram_safe_get (altdnsvar));
	  dns_list->num_servers++;
	}
      altdns_index++;
    }
  return dns_list;
}

int
dns_to_resolv (void)
{
  FILE *fp_w;
  struct dns_lists *dns_list = NULL;
  int i = 0;

  /* Save DNS to resolv.conf */
  if (!(fp_w = fopen (RESOLV_FILE, "w")))
    {
      perror (RESOLV_FILE);
      return errno;
    }
  if (nvram_invmatch ("wan_get_domain", ""))
    {
      fprintf (fp_w, "search %s\n", nvram_safe_get ("wan_get_domain"));
    }
  else if (nvram_invmatch ("wan_domain", ""))
    {
      fprintf (fp_w, "search %s\n", nvram_safe_get ("wan_domain"));
    }
  if (nvram_invmatch ("lan_domain", ""))
    {
      fprintf (fp_w, "search %s\n", nvram_safe_get ("lan_domain"));
    }
  if (nvram_match ("dnsmasq_enable", "1"))
    {
      fprintf (fp_w, "nameserver %s\n", nvram_get ("lan_ipaddr"));
      fclose (fp_w);
      if (!(fp_w = fopen (RESOLV_FORW, "w")))
	{
	  perror (RESOLV_FORW);
	  return errno;
	}
    }

  dns_list = get_dns_list ();

  for (i = 0; i < dns_list->num_servers; i++)
    fprintf (fp_w, "nameserver %s\n", dns_list->dns_server[i]);

  /* Put a pseudo DNS IP to trigger Connect On Demand */
  if (dns_list->num_servers == 0 &&
      (nvram_match ("wan_proto", "pppoe") || nvram_match ("wan_proto", "pptp")
       || nvram_match ("wan_proto", "l2tp"))
      && nvram_match ("ppp_demand", "1"))
    fprintf (fp_w, "nameserver 1.1.1.1\n");

  fclose (fp_w);
  if (dns_list)
    free (dns_list);

  eval ("touch", "/tmp/hosts");

  return 1;
}

/* Example:
 * lan_ipaddr = 192.168.1.1
 * get_dns_ip("lan_ipaddr", 1); produces "168"
 */
int
get_single_ip (char *ipaddr, int which)
{
  int ip[4] = { 0, 0, 0, 0 };
  int ret;

  ret = sscanf (ipaddr, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);

  return ip[which];
}

char *
get_complete_lan_ip (char *ip)
{
  static char ipaddr[20];

  int i[4];

  if (sscanf
      (nvram_safe_get ("lan_ipaddr"), "%d.%d.%d.%d", &i[0], &i[1], &i[2],
       &i[3]) != 4)
    return "0.0.0.0";

  snprintf (ipaddr, sizeof (ipaddr), "%d.%d.%d.%s", i[0], i[1], i[2], ip);

  return ipaddr;
}

char *
get_wan_face (void)
{
  static char localwanface[IFNAMSIZ];
/*  if (nvram_match ("pptpd_client_enable", "1"))
    {
	strncpy (localwanface, "ppp0", IFNAMSIZ);
	return localwanface;
    }*/
  if (nvram_match ("wan_proto", "pptp") || nvram_match ("wan_proto", "l2tp")
      || nvram_match ("wan_proto", "pppoe"))
    {
      if (nvram_match ("pppd_pppifname", ""))
	strncpy (localwanface, "ppp0", IFNAMSIZ);
      else
	strncpy (localwanface, nvram_safe_get ("pppd_pppifname"), IFNAMSIZ);
    }
#ifndef HAVE_MADWIFI
  else if (nvram_match ("wl0_mode", "sta")
	   || nvram_match ("wl0_mode", "apsta")
	   || nvram_match ("wl0_mode", "apstawet")
	   || nvram_match ("wl0_mode", "wet"))
    {
    
      strcpy (localwanface, get_wdev ());

    }
#else
  else if (getSTA ())
    {
      if (nvram_match ("wifi_bonding", "1"))
	strcpy (localwanface, "bond0");
      else
	strcpy (localwanface, getSTA ());
    }
#endif
  else
    strncpy (localwanface, nvram_safe_get ("wan_ifname"), IFNAMSIZ);

  return localwanface;
}

int
get_ppp_pid (char *file)
{
  char buf[80];
  int pid = -1;
  if (file_to_buf (file, buf, sizeof (buf)))
    {
      char tmp[80], tmp1[80];
      snprintf (tmp, sizeof (tmp), "/var/run/%s.pid", buf);
      file_to_buf (tmp, tmp1, sizeof (tmp1));
      pid = atoi (tmp1);
    }
  return pid;
}

/*
 =====================================================================================
				by tallest 
 =====================================================================================
 */


int
osl_ifflags (const char *ifname)
{
  int sockfd;
  struct ifreq ifreq;
  int flags = 0;

  if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror ("socket");
      return flags;
    }

  strncpy (ifreq.ifr_name, ifname, IFNAMSIZ);
  if (ioctl (sockfd, SIOCGIFFLAGS, &ifreq) < 0)
    {
      flags = 0;
    }
  else
    {
      flags = ifreq.ifr_flags;
    }
  close (sockfd);
  return flags;
}

int
check_wan_link (int num)
{
  int wan_link = 0;

  if (nvram_match ("wan_proto", "pptp")
      || nvram_match ("wan_proto", "l2tp")
      || nvram_match ("wan_proto", "pppoe")
      || nvram_match ("wan_proto", "heartbeat"))
    {
      FILE *fp;
      char filename[80];
      char *name;

      if (num == 0)
	strcpy (filename, "/tmp/ppp/link");
      if ((fp = fopen (filename, "r")))
	{
	  int pid = -1;
	  fclose (fp);
	  if (nvram_match ("wan_proto", "heartbeat"))
	    {
	      char buf[20];
	      file_to_buf ("/tmp/ppp/link", buf, sizeof (buf));
	      pid = atoi (buf);
	    }
	  else
	    pid = get_ppp_pid (filename);

	  name = find_name_by_proc (pid);
	  if (!strncmp (name, "pppoecd", 7) ||	// for PPPoE
	      !strncmp (name, "pppd", 4) ||	// for PPTP
	      !strncmp (name, "bpalogin", 8))	// for HeartBeat
	    wan_link = 1;	//connect
	  else
	    {
	      printf ("The %s had been died, remove %s\n",
		      nvram_safe_get ("wan_proto"), filename);
	      wan_link = 0;	// For some reason, the pppoed had been died, by link file still exist.
	      unlink (filename);
	    }
	}
    }
  else
    {
      if (nvram_invmatch ("wan_ipaddr", "0.0.0.0"))
	wan_link = 1;
    }

  return wan_link;
}

int
get_int_len (int num)
{
  char buf[80];

  snprintf (buf, sizeof (buf), "%d", num);

  return strlen (buf);
}

int
file_to_buf (char *path, char *buf, int len)
{
  FILE *fp;

  memset (buf, 0, len);

  if ((fp = fopen (path, "r")))
    {
      fgets (buf, len, fp);
      fclose (fp);
      return 1;
    }

  return 0;
}

int
buf_to_file (char *path, char *buf)
{
  FILE *fp;

  if ((fp = fopen (path, "w")))
    {
      fprintf (fp, "%s", buf);
      fclose (fp);
      return 1;
    }

  return 0;
}


#define READ_BUF_SIZE 254
/* from busybox find_pid_by_name.c */
pid_t *
find_pid_by_name (char *pidName)
{
  DIR *dir;
  struct dirent *next;
  pid_t *pidList = NULL;
  int i = 0;

  dir = opendir ("/proc");

  while ((next = readdir (dir)) != NULL)
    {
      FILE *status;
      char filename[READ_BUF_SIZE];
      char buffer[READ_BUF_SIZE];
      char name[READ_BUF_SIZE];

      /* Must skip ".." since that is outside /proc */
      if (strcmp (next->d_name, "..") == 0)
	continue;

      /* If it isn't a number, we don't want it */
      if (!isdigit (*next->d_name))
	continue;

      sprintf (filename, "/proc/%s/status", next->d_name);
      if (!(status = fopen (filename, "r")))
	{
	  continue;
	}
      if (fgets (buffer, READ_BUF_SIZE - 1, status) == NULL)
	{
	  fclose (status);
	  continue;
	}
      fclose (status);

      /* Buffer should contain a string like "Name:   binary_name" */
      sscanf (buffer, "%*s %s", name);
      //printf("buffer=[%s] name=[%s]\n",buffer,name);
      if (strcmp (name, pidName) == 0)
	{
	  pidList = realloc (pidList, sizeof (pid_t) * (i + 2));
	  pidList[i++] = strtol (next->d_name, NULL, 0);
	}
    }

  if (pidList)
    pidList[i] = 0;
  else
    {
      pidList = realloc (pidList, sizeof (pid_t));
      pidList[0] = -1;
    }
  return pidList;

}

/* Find first process pid with same name from ps command */
int
find_pid_by_ps (char *pidName)
{
  FILE *fp;
  int pid = -1;
  char line[254];

  if ((fp = popen ("ps", "r")))
    {
      while (fgets (line, sizeof (line), fp) != NULL)
	{
	  if (strstr (line, pidName))
	    {
	      sscanf (line, "%d", &pid);
	      printf ("%s pid is %d\n", pidName, pid);
	      break;
	    }
	}
      pclose (fp);
    }

  return pid;
}

/* Find process name by pid from /proc directory */
char *
find_name_by_proc (int pid)
{
  FILE *fp;
  char line[254];
  char filename[80];
  static char name[80];

  snprintf (filename, sizeof (filename), "/proc/%d/status", pid);

  if ((fp = fopen (filename, "r")))
    {
      fgets (line, sizeof (line), fp);
      /* Buffer should contain a string like "Name:   binary_name" */
      sscanf (line, "%*s %s", name);
      fclose (fp);
      return name;
    }

  return "";
}

/* Find all process pid with same name from ps command */
int *
find_all_pid_by_ps (char *pidName)
{
  FILE *fp;
  int pid = -1;
  char line[254];
  int *pidList = NULL;
  int i = 0;
  printf ("Search for %s\n", pidName);
  if ((fp = popen ("ps", "r")))
    {
      while (fgets (line, sizeof (line), fp) != NULL)
	{
	  if (strstr (line, pidName))
	    {
	      sscanf (line, "%d", &pid);
	      printf ("%s pid is %d\n", pidName, pid);
	      pidList = realloc (pidList, sizeof (int) * (i + 2));
	      pidList[i++] = pid;
	    }
	}
      pclose (fp);
    }
  if (pidList)
    pidList[i] = 0;
  else
    {
      pidList = realloc (pidList, sizeof (int));
      pidList[0] = -1;
    }
  printf ("Search done...\n");

  return pidList;
}


int
count_processes (char *pidName)
{
  FILE *fp;
  char line[254];
  int i = 0;
  printf ("Search for %s\n", pidName);
  if ((fp = popen ("ps", "r")))
    {
      while (fgets (line, sizeof (line), fp) != NULL)
	{
	  if (strstr (line, pidName))
	    {
	      i++;
	    }
	}
      pclose (fp);
    }
  printf ("Search done... %d\n", i);

  return i;
}

void
encode (char *buf, int len)
{
  int i;
  char ch;

  for (i = 0; i < len; i++)
    {
      ch = (buf[i] & 0x03) << 6;
      buf[i] = (buf[i] >> 2);
      buf[i] &= 0x3f;
      buf[i] |= ch;
      buf[i] = ~buf[i];
    }
}

void
decode (char *buf, int len)
{
  int i;
  char ch;

  for (i = 0; i < len; i++)
    {
      ch = (buf[i] & 0xC0) >> 6;
      buf[i] = (buf[i] << 2) | ch;
      buf[i] = ~buf[i];
    }
}

/*	v1.41.7 => 014107
 *	v1.2	=> 0102
 */
long
convert_ver (char *ver)
{
  char buf[10];
  int v[3];
  int ret;

  ret = sscanf (ver, "v%d.%d.%d", &v[0], &v[1], &v[2]);

  if (ret == 2)
    {
      snprintf (buf, sizeof (buf), "%02d%02d", v[0], v[1]);
      return atol (buf);
    }
  else if (ret == 3)
    {
      snprintf (buf, sizeof (buf), "%02d%02d%02d", v[0], v[1], v[2]);
      return atol (buf);
    }
  else
    return -1;
}

/* To avoid user to download old image that is not support intel flash to new hardware with intel flash.
 */
int
check_flash (void)
{
  // The V2 image can support intel flash completely, so we don't want to check.
  if (check_hw_type () == BCM4712_CHIP)
    return FALSE;

  // The V1.X some images cann't support intel flash, so we want to avoid user to downgrade.
  if (nvram_match ("skip_amd_check", "1"))
    {
      if (strstr (nvram_safe_get ("flash_type"), "Intel")
	  && nvram_invmatch ("skip_intel_check", "1"))
	return TRUE;
      else
	return FALSE;
    }
  else				// Cann't downgrade to old firmware version, no matter AMD or Intel flash
    return TRUE;
}

int
check_action (void)
{
  char buf[80] = "";

  if (file_to_buf (ACTION_FILE, buf, sizeof (buf)))
    {
      if (!strcmp (buf, "ACT_TFTP_UPGRADE"))
	{
	  fprintf (stderr, "Upgrading from tftp now ...\n");
	  return ACT_TFTP_UPGRADE;
	}
#ifdef HAVE_HTTPS
      else if (!strcmp (buf, "ACT_WEBS_UPGRADE"))
	{
	  fprintf (stderr, "Upgrading from web (https) now ...\n");
	  return ACT_WEBS_UPGRADE;
	}
#endif
      else if (!strcmp (buf, "ACT_WEB_UPGRADE"))
	{
	  fprintf (stderr, "Upgrading from web (http) now ...\n");
	  return ACT_WEB_UPGRADE;
	}
      else if (!strcmp (buf, "ACT_SW_RESTORE"))
	{
	  fprintf (stderr, "Receiving restore command from web ...\n");
	  return ACT_SW_RESTORE;
	}
      else if (!strcmp (buf, "ACT_HW_RESTORE"))
	{
	  fprintf (stderr,
		   "Receiving restore commond from resetbutton ...\n");
	  return ACT_HW_RESTORE;
	}
      else if (!strcmp (buf, "ACT_NVRAM_COMMIT"))
	{
	  fprintf (stderr, "Committing nvram now ...\n");
	  return ACT_NVRAM_COMMIT;
	}
      else if (!strcmp (buf, "ACT_ERASE_NVRAM"))
	{
	  fprintf (stderr, "Erasing nvram now ...\n");
	  return ACT_ERASE_NVRAM;
	}
    }
  //fprintf(stderr, "Waiting for upgrading....\n");
  return ACT_IDLE;
}

int
check_now_boot (void)
{
  char *ver = nvram_safe_get ("pmon_ver");
  char *cfe = nvram_safe_get ("CFEver");
  // for 4712
  // The boot_ver value is lower v2.0 (no included)
  if (!strncmp (ver, "PMON", 4))
    {
      cprintf ("The boot is PMON\n");
      return PMON_BOOT;
    }
  // for 4712
  // The boot_ver value is higher v2.0 (included)
  else if (!strncmp (ver, "CFE", 3))
    {
      cprintf ("The boot is CFE\n");
      return CFE_BOOT;
    }
  else if (!strncmp (ver, "2", 1))
    {
      cprintf ("The boot is CFE %s\n", ver);
      return CFE_BOOT;
    }
  else if (!strncmp (cfe, "MotoWR", 6))
    {
      cprintf ("The boot is Motorola CFE\n");
      return CFE_BOOT;
    }
  else
    {
      cprintf ("The boot is UNKNOWN\n");
      return UNKNOWN_BOOT;
    }
}

void
show_hw_type (int type)
{
  cprintf ("The chipset is ");
  if (type == BCM4702_CHIP)
    cprintf ("BCM4702\n");
  else if (type == BCM5325E_CHIP)
    cprintf ("BCM4712L + BCM5325E\n");
  else if (type == BCM4704_BCM5325F_CHIP)
    cprintf ("BCM4704 + BCM5325F\n");
  else if (type == BCM5352E_CHIP)
    cprintf ("BCM5352E\n");
  else if (type == BCM4712_CHIP)
    cprintf ("BCM4712 + ADMtek\n");
  else if (type == BCM4704_BCM5325F_EWC_CHIP)
    cprintf ("BCM4704 + BCM5325F for EWC\n");
  else
    cprintf ("not defined\n");

}

int
check_hw_type (void)
{

  char *boardtype = nvram_safe_get ("boardtype");
  uint boardflags = strtoul (nvram_safe_get ("boardflags"), NULL, 0);
  uint btype = strtoul (boardtype, NULL, 0);

  if (!strncmp (boardtype, "bcm94710", 8))
    return BCM4702_CHIP;
  else if (btype == 0x0708 && !(boardflags & BFL_ENETADM))
    return BCM5325E_CHIP;
  else if (btype == 0x042f && !(boardflags & BFL_ENETADM))
    return BCM4704_BCM5325F_CHIP;
  else if (btype == 0x478)
    return BCM4705_BCM5397_EWC_CHIP;
  else if (btype == 0x0467)
    return BCM5352E_CHIP;
  else if (btype == 0x0101 || btype == 0x0446)	//0x446 is wap54g v2
    return BCM4712_CHIP;
  else if (btype == 0x0472 && !(boardflags & BFL_ENETADM))
    return BCM4704_BCM5325F_EWC_CHIP;
  else
    return NO_DEFINE_CHIP;
}

int
is_exist (char *filename)
{
  FILE *fp;

  if ((fp = fopen (filename, "r")))
    {
      fclose (fp);
      return 1;
    }
  return 0;
}

int
ct_openlog (const char *ident, int option, int facility, char *log_name)
{
  int level = atoi (nvram_safe_get (log_name));

  switch (level)
    {
    case CONSOLE_ONLY:
      break;
    }
  return level;
}


void
ct_syslog (int level, int enable, const char *fmt, ...)
{
  char buf[1000];
  va_list args;

  va_start (args, fmt);
  vsnprintf (buf, sizeof (buf), fmt, args);
  va_end (args);

  switch (enable)
    {
    case CONSOLE_ONLY:
      cprintf ("[%d] %s\n", getpid (), buf);	// print to console
      break;
    }
}

void
ct_logger (int level, const char *fmt, ...)
{
}

void
set_ip_forward (char c)
{
  FILE *fp;

  if ((fp = fopen ("/proc/sys/net/ipv4/ip_forward", "r+")))
    {
      fputc (c, fp);
      fclose (fp);
    }
  else
    {
      perror ("/proc/sys/net/ipv4/ip_forward");
    }
}


static char *device_name[] = {
  "eth0",
  "qos0"
};

char *
get_device_name (void)
{
  int i;

  switch (check_hw_type ())
    {
    case BCM5325E_CHIP:
    case BCM4704_BCM5325F_CHIP:
    case BCM5352E_CHIP:
      i = 0;
      break;
    case BCM4702_CHIP:
    case BCM4712_CHIP:
    default:
      i = 1;
      break;
    }

  return device_name[i];
}

char *
strncpyz (char *dest, char const *src, size_t size)
{
  if (!size--)
    return dest;
  strncpy (dest, src, size);
  dest[size] = 0;		/* Make sure the string is null terminated */
  return dest;
}

static int
sockets_open (int domain, int type, int protocol)
{
  int fd = socket (domain, type, protocol);

  if (fd < 0)
    cprintf ("sockets_open: no usable address was found.\n");
  return fd;
}

int
sys_netdev_ioctl (int family, int socket, char *if_name, int cmd,
		  struct ifreq *ifr)
{
  int rc, s;

  if ((s = socket) < 0)
    {
      if ((s = sockets_open (family, family == AF_PACKET ? SOCK_PACKET :
			     SOCK_DGRAM,
			     family == AF_PACKET ? htons (ETH_P_ALL) : 0)) <
	  0)
	{
	  cprintf ("sys_netdev_ioctl: failed\n");
	  return -1;
	}
    }
  strncpyz (ifr->ifr_name, if_name, IFNAMSIZ);
  rc = ioctl (s, cmd, ifr);
  if (socket < 0)
    close (s);
  return rc;
}

int
set_register_value (unsigned short port_addr, unsigned short option_content)
{
  struct ifreq ifr;
  struct mii_ioctl_data stats;

  stats.phy_id = port_addr;
  stats.val_in = option_content;

  ifr.ifr_data = (void *) &stats;

  if (sys_netdev_ioctl (AF_INET, -1, get_device_name (), SIOCSMIIREG, &ifr) <
      0)
    return -1;

  return 0;
}

unsigned long
get_register_value (unsigned short id, unsigned short num)
{
  struct ifreq ifr;
  struct mii_ioctl_data stats;

  stats.phy_id = id;
  stats.reg_num = num;
  stats.val_in = 0;
  stats.val_out = 0;

  ifr.ifr_data = (void *) &stats;

  sys_netdev_ioctl (AF_INET, -1, get_device_name (), SIOCGMIIREG, &ifr);

  return ((stats.val_in << 16) | stats.val_out);
}


struct wl_assoc_mac *
get_wl_assoc_mac (int *c)
{
  FILE *fp;
  struct wl_assoc_mac *wlmac = NULL;
  int count;
  char line[80];
  char list[2][20];

  wlmac = NULL;
  count = *c = 0;
  
#ifdef HAVE_MSSID
  char assoccmd[4][32] = {"wl assoclist", "wl -i wl0.1 assoclist", "wl -i wl0.2 assoclist", "wl -i wl0.3 assoclist"};
  int ifcnt = 4;
#else
  char assoccmd[1][16] = {"wl assoclist"};
  int ifcnt = 1;
#endif
  int i;
  int gotit = 0;
//  fprintf(stderr,"assoclist\n");

  for (i = 0; i < ifcnt; i++)
  {
  if ((fp = popen (assoccmd[i], "r")))
    {
	  gotit = 1;
      while (fgets (line, sizeof (line), fp) != NULL)
	{
	  strcpy (list[0], "");
	  strcpy (list[1], "");

	  if (sscanf (line, "%s %s", list[0], list[1]) != 2)	// assoclist 00:11:22:33:44:55
	    continue;
	  if (strcmp (list[0], "assoclist"))
	    continue;

	  wlmac = realloc (wlmac, sizeof (struct wl_assoc_mac) * (count + 1));

	  memset (&wlmac[count], 0, sizeof (struct wl_assoc_mac));
	  strncpy (wlmac[count].mac, list[1], sizeof (wlmac[0].mac));
	  count++;
	}

      pclose (fp);
	} 
  }
  
  if (gotit)
  { 
      //cprintf("Count of wl assoclist mac is %d\n", count);
      *c = count;
      return wlmac;
  }
  else
      return NULL;
}

struct mtu_lists mtu_list[] = {
#if COUNTRY == JAPAN
  {"pppoe", "576", "1454"},
#else
  {"pppoe", "576", "1492"},
#endif

  {"pptp", "576", "1460"},

  {"l2tp", "576", "1460"},

  {"dhcp", "576", "1500"},
  {"static", "576", "1500"},
  {"heartbeat", "576", "1500"},
  {"default", "576", "1500"},	// The value must be at last
};

struct mtu_lists *
get_mtu (char *proto)
{
  struct mtu_lists *v = NULL;

  for (v = mtu_list; v < &mtu_list[STRUCT_LEN (mtu_list)]; v++)
    {
      if (!strcmp (proto, v->proto))
	return v;
    }
  return v;			// Use default settings
}

void
set_host_domain_name (void)
{
  char *wan_hostname = nvram_safe_get ("wan_hostname");
  char *wan_domain = nvram_safe_get ("wan_domain");

  /* Allow you to use gethostname to get Host Name */
  /* If wan_hostname is blank then we do nothing, we leave to what it was set at boot */
  if (strlen (wan_hostname) > 0)
    sethostname (wan_hostname, strlen (wan_hostname));

  /* Allow you to use getdomainname to get Domain Name */
  if (strlen (wan_domain) > 0 && strlen (wan_domain) <= 64)	//no more than 64
    setdomainname (wan_domain, strlen (wan_domain));
  else
    {
      char *wan_get_domain = nvram_safe_get ("wan_get_domain");
      setdomainname (wan_get_domain, strlen (wan_get_domain));
    }
}

int
first_time (void)
{
  struct sysinfo info;

  sysinfo (&info);
  if (info.uptime < 20L)
    return 1;
  return 0;
}

int
check_vlan_support (void)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42)
  return 0;
#else


  int brand = getRouterBrand ();
  switch (brand)
    {
    case ROUTER_BUFFALO_WLAG54C:
    case ROUTER_BUFFALO_WLA2G54C:
#ifndef HAVE_BUFFALO
    case ROUTER_LINKSYS_WRT55AG:
    case ROUTER_MOTOROLA_V1:
    case ROUTER_MOTOROLA_WE800G:
    case ROUTER_WAP54G_V1:
    case ROUTER_SITECOM_WL105B:
    case ROUTER_BUFFALO_WLI2_TX1_G54:
    case ROUTER_BUFFALO_WLI_TX4_G54HP:
    case ROUTER_BRCM4702_GENERIC:
    case ROUTER_ASUS_WL500G:
#endif
      return 0;
      break;
    }

  uint boardflags = strtoul (nvram_safe_get ("boardflags"), NULL, 0);
  if (boardflags & BFL_ENETVLAN)
    return 1;

  if (nvram_match ("boardtype", "bcm94710dev")
      || nvram_match ("boardtype", "0x0101") || (boardflags & 0x0100))
    return 1;
  else
    return 0;
#endif
}

#ifdef CDEBUG
int
coreleft (void)
{
  struct sysinfo info;
  sysinfo (&info);
  return info.freeram;
}

int
mcoreleft (void)
{
  struct mallinfo minfo;
  minfo = mallinfo ();
  return minfo.uordblks;
  //int uordblks; /* total allocated space */

}
#endif

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

//#define WDS_DEBUG 1
#undef WDS_DEBUG
#ifdef WDS_DEBUG
FILE *fp;
#endif


int
wds_dev_config (int dev, int up)
{
  char wds_var[32] = "";
  char wds_enable_var[32] = "";
  char wds_dev[32] = "";
  char *wds = (void *) 0;
  char wds_gw_var[32] = "";
  char cmd[100] = "";
  char *gw = (void *) 0;
  int s = -1;
  struct ifreq ifr;

#ifdef WDS_DEBUG
  fp = fopen ("/tmp/.wds_debug.log", "a");
#endif

  memset (&ifr, 0, sizeof (struct ifreq));

  snprintf (wds_var, 31, "wl_wds%d", dev);
  snprintf (wds_enable_var, 31, "%s_enable", wds_var);

  if ((wds = nvram_safe_get (wds_enable_var)) == NULL ||
      strcmp (wds, "0") == 0)
    return -1;
#ifdef HAVE_MSSID
  snprintf (wds_dev, 31, "wds0.%d", dev + 1);
#else
  snprintf (wds_dev, 31, "wds0.491%d", 50 + dev + 1);
#endif
  snprintf (ifr.ifr_name, IFNAMSIZ, wds_dev);
#ifdef WDS_DEBUG
  fprintf (fp, "opening kernelsocket\n");
#endif
  if ((s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    return -1;

  if (up)
    {
      char wds_hwaddr_var[32] = "";
      char wds_ip_var[32] = "";
      char wds_netmask_var[32] = "";
      char *wds_list = (void *) 0;
      char *hwaddr = (void *) 0;
      char *ip = (void *) 0;
      char *netmask = (void *) 0;

#ifdef WDS_DEBUG
      fprintf (fp, "running up\n");
#endif

      wds_list = nvram_safe_get ("wl0_wds");
      if (wds_list == (void *) 0 || strlen (wds_list) <= 0)
	return 0;

      snprintf (wds_hwaddr_var, 31, "%s_hwaddr", wds_var);
      snprintf (wds_ip_var, 31, "%s_ipaddr", wds_var);
      snprintf (wds_netmask_var, 31, "%s_netmask", wds_var);

      hwaddr = nvram_safe_get (wds_hwaddr_var);
      ip = nvram_safe_get (wds_ip_var);
      netmask = nvram_safe_get (wds_netmask_var);

      if (!strstr (wds_list, hwaddr))
	return -1;

#ifdef WDS_DEBUG
      fprintf (fp, "checking validity\n");
#endif

      if (!sv_valid_hwaddr (hwaddr) || !sv_valid_ipaddr (ip)
	  || !sv_valid_ipaddr (netmask))
	return -1;

#ifdef WDS_DEBUG
      fprintf (fp, "valid mac %s ip %s nm %s\n", hwaddr, ip, netmask);
#endif

      snprintf (cmd, 99, "ifconfig %s down", wds_dev);
      system2 (cmd);

      snprintf (cmd, 99, "ifconfig %s %s netmask %s up", wds_dev, ip,
		netmask);
      system2 (cmd);

      snprintf (wds_gw_var, 31, "%s_gw", wds_var);
      gw = nvram_safe_get (wds_gw_var);
      if (strcmp (gw, "0.0.0.0") != 0)
	{
	  get_network (ip, netmask);
	  route_del (wds_dev, 0, ip, gw, netmask);
	  route_add (wds_dev, 0, ip, gw, netmask);
	}

    }
  else
    {
#ifdef WDS_DEBUG
      fprintf (fp, "running down\n");
#endif
      snprintf (cmd, 99, "ifconfig %s down", wds_dev);
      system2 (cmd);

    }

#ifdef WDS_DEBUG
  fprintf (fp, "running ioctl\n");
  fclose (fp);
#endif

  close (s);

  return 0;
}


int
ishexit (char c)
{

  if (strchr ("01234567890abcdefABCDEF", c) != (char *) 0)
    return 1;

  return 0;
}


/* Example:
 * legal_hwaddr("00:11:22:33:44:aB"); return true;
 * legal_hwaddr("00:11:22:33:44:5"); return false;
 * legal_hwaddr("00:11:22:33:44:HH"); return false;
 */
int
sv_valid_hwaddr (char *value)
{
  unsigned int hwaddr[6];
  int tag = TRUE;
  int i, count;

  /* Check for bad, multicast, broadcast, or null address */
  for (i = 0, count = 0; *(value + i); i++)
    {
      if (*(value + i) == ':')
	{
	  if ((i + 1) % 3 != 0)
	    {
	      tag = FALSE;
	      break;
	    }
	  count++;
	}
      else if (ishexit (*(value + i)))	/* one of 0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F */
	continue;
      else
	{
	  tag = FALSE;
	  break;
	}
    }

  if (!tag || i != 17 || count != 5)	/* must have 17's characters and 5's ':' */
    tag = FALSE;
  else if (sscanf (value, "%x:%x:%x:%x:%x:%x",
		   &hwaddr[0], &hwaddr[1], &hwaddr[2],
		   &hwaddr[3], &hwaddr[4], &hwaddr[5]) != 6)
    {
      tag = FALSE;
    }
  else
    tag = TRUE;
#ifdef WDS_DEBUG
  if (tag == FALSE)
    fprintf (fp, "failed valid_hwaddr\n");
#endif

  return tag;
}


int
sv_valid_range (char *value, int low, int high)
{
  if (!isdigit (value[0]) || atoi (value) < low || atoi (value) > high)
    return FALSE;
  else
    return TRUE;

}

int
sv_valid_statics (char *value)
{
  char ip[16] = { 0 }, mac[18] =
  {
  0}, hostname[255] =
  {
  0}, *p = value;

  if (NULL == value)
    return FALSE;

  do
    {
      while (isspace (*p++) && p - value < strlen (value))
	;

      if (p - value >= strlen (value))
	return FALSE;

      if (sscanf (p, "%15s%17s%254s", ip, mac, hostname) < 3)
	return FALSE;

      if (!sv_valid_ipaddr (ip) || !sv_valid_hwaddr (mac)
	  || strlen (hostname) <= 0)
	return FALSE;

    }
  while ((p = strpbrk (p, "\n\r")) && p - value < strlen (value));

  return TRUE;
}

/* Example:
 * legal_ipaddr("192.168.1.1"); return true;
 * legal_ipaddr("192.168.1.1111"); return false;
 */
int
sv_valid_ipaddr (char *value)
{
  struct in_addr ipaddr;
  int ip[4], ret = 0;

  ret = sscanf (value, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);

  if (ret != 4 || !inet_aton (value, &ipaddr))
    return FALSE;
  else
    return TRUE;

}

// note - networl address returned in ipaddr
void
get_network (char *ipaddr, char *netmask)
{
  int ip[4], mask[4];

  if (!ipaddr || !netmask)
    return;

  sscanf (ipaddr, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
  sscanf (netmask, "%d.%d.%d.%d", &mask[0], &mask[1], &mask[2], &mask[3]);

  ip[0] &= mask[0];
  ip[1] &= mask[1];
  ip[2] &= mask[2];
  ip[3] &= mask[3];

  sprintf (ipaddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
#ifdef WDS_DEBUG
  fprintf (fp, "get_network return %s\n", ipaddr);
#endif

}

// note - broadcast addr returned in ipaddr
void
get_broadcast (char *ipaddr, char *netmask)
{
  int ip2[4], mask2[4];
  unsigned char ip[4], mask[4];
  if (!ipaddr || !netmask)
    return;

  sscanf (ipaddr, "%d.%d.%d.%d", &ip2[0], &ip2[1], &ip2[2], &ip2[3]);
  sscanf (netmask, "%d.%d.%d.%d", &mask2[0], &mask2[1], &mask2[2], &mask2[3]);
  int i = 0;
  for (i = 0; i < 4; i++)
    {
      ip[i] = ip2[i];
      mask[i] = mask2[i];
//      ip[i] = (ip[i] & mask[i]) | !mask[i];
      ip[i] = (ip[i] & mask[i]) | (0xff & ~mask[i]);
    }

  sprintf (ipaddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
#ifdef WDS_DEBUG
  fprintf (fp, "get_broadcast return %s\n", value);
#endif

}

/* note: copied from Broadcom code and put in shared via this file */

int
route_manip (int cmd, char *name, int metric, char *dst, char *gateway,
	     char *genmask)
{
  int s;
  struct rtentry rt;

  //dprintf("cmd=[%d] name=[%s] ipaddr=[%s] netmask=[%s] gateway=[%s] metric=[%d]\n",cmd,name,dst,genmask,gateway,metric);

  /* Open a raw socket to the kernel */
  if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    goto err;

  /* Fill in rtentry */
  memset (&rt, 0, sizeof (rt));
  if (dst)
    inet_aton (dst, &sin_addr (&rt.rt_dst));
  if (gateway)
    inet_aton (gateway, &sin_addr (&rt.rt_gateway));
  if (genmask)
    inet_aton (genmask, &sin_addr (&rt.rt_genmask));
  rt.rt_metric = metric;
  rt.rt_flags = RTF_UP;
  if (sin_addr (&rt.rt_gateway).s_addr)
    rt.rt_flags |= RTF_GATEWAY;
  if (sin_addr (&rt.rt_genmask).s_addr == INADDR_BROADCAST)
    rt.rt_flags |= RTF_HOST;
  rt.rt_dev = name;

  /* Force address family to AF_INET */
  rt.rt_dst.sa_family = AF_INET;
  rt.rt_gateway.sa_family = AF_INET;
  rt.rt_genmask.sa_family = AF_INET;

  if (ioctl (s, cmd, &rt) < 0)
    goto err;

  close (s);
  return 0;

err:
  close (s);
  perror (name);
  return errno;
}

int
route_add (char *name, int metric, char *dst, char *gateway, char *genmask)
{
  return route_manip (SIOCADDRT, name, metric, dst, gateway, genmask);
}

int
route_del (char *name, int metric, char *dst, char *gateway, char *genmask)
{
  return route_manip (SIOCDELRT, name, metric, dst, gateway, genmask);
}
//#endif


#ifdef HAVE_MADWIFI
static char *stalist[] =
  { "ath0", "ath1", "ath2", "ath3", "ath4", "ath5", "ath6", "ath8", "ath9" };
char *
getSTA (void)
{
  int c = getifcount ("wifi");
  int i;
  for (i = 0; i < c; i++)
    {
      char mode[32];
      char netmode[32];
      sprintf (mode, "ath%d_mode", i);
      sprintf (netmode, "ath%d_net_mode", i);
      if (nvram_match (mode, "sta") && !nvram_match(netmode,"disabled"))
	{
	  return stalist[i];
	}

    }
  return NULL;
}

char *
getWET (void)
{
  int c = getifcount ("wifi");
  int i;
  for (i = 0; i < c; i++)
    {
      char mode[32];
      char netmode[32];
      sprintf (mode, "ath%d_mode", i);
      sprintf (netmode, "ath%d_net_mode", i);
      if (nvram_match (mode, "wet") && !nvram_match(netmode,"disabled"))
	{
	  return stalist[i];
	}

    }
  return NULL;
}
#else
char *
getSTA ()
{
  return NULL;
}

char *
getWET ()
{
  return NULL;
}

#endif


int
ifexists (const char *ifname)
{
  return getifcount (ifname) > 0 ? 1 : 0;
}

int
getifcount (const char *ifprefix)
{
/*  char devcall[128];

  sprintf (devcall, "cat /proc/net/dev|grep \"%s\"|wc -l", ifprefix);
  FILE *in = popen (devcall, "rb");
  if (in == NULL)
    return 0;
  int count;
  fscanf (in, "%d", &count);
  pclose (in);
  return count;*/
  char *iflist = malloc (256);
  memset (iflist, 0, 256);
  int c = getIfList (iflist, ifprefix);
  free (iflist);
  return c;
}

static void
skipline (FILE * in)
{
  while (1)
    {
      int c = getc (in);
      if (c == EOF)
	return;
      if (c == 0x0)
	return;
      if (c == 0xa)
	return;
    }
}

//returns a physical interfacelist filtered by ifprefix. if ifprefix is NULL, all valid interfaces will be returned
int
getIfList (char *buffer, const char *ifprefix)
{
  FILE *in = fopen("/proc/net/dev","rb");
  char ifname[32];
//skip the first 2 lines
  skipline (in);
  skipline (in);
  int ifcount = 0;
  int count = 0;
  while (1)
    {
      int c = getc (in);
      if (c == EOF)
	{
	  if (count)
	    buffer[strlen (buffer) - 1] = 0;	//fixup last space
	  fclose (in);
	  return count;
	}
      if (c == 0)
	{
	  if (count)
	    buffer[strlen (buffer) - 1] = 0;	//fixup last space
	  fclose (in);
	  return count;
	}
      if (c == 0x20)
	continue;
      if (c == ':')
	{
	  ifname[ifcount++] = 0;
	  int skip = 0;
	  if (ifprefix)
	    {
	      if (strncmp (ifname, ifprefix, strlen (ifprefix)))
		{
		  skip = 1;
		}
	    }
	  else
	    {
	      if (!strncmp (ifname, "wifi", 4))
		skip = 1;
	      if (!strncmp (ifname, "imq", 3))
		skip = 1;
	      if (!strncmp (ifname, "lo", 2))
		skip = 1;
	      if (!strncmp (ifname, "teql", 4))
		skip = 1;
	      if (!strncmp (ifname, "gre", 3))
		skip = 1;
	      if (!strncmp (ifname, "ppp", 3))
		skip = 1;
	      if (!strncmp (ifname, "tun", 3))
		skip = 1;
	      if (!strncmp (ifname, "tap", 3))
		skip = 1;
	    }
	  if (!skip)
	    {
	      strcat (buffer, ifname);
	      strcat (buffer, " ");
	      count++;
	    }
	  skip = 0;
	  ifcount = 0;
	  memset (ifname, 0, 32);
	  skipline (in);
	  continue;
	}
      ifname[ifcount++] = c;
    }
}

int
contains (const char *string, char value)
{
  if (string == NULL)
    return 0;
  int len = strlen (string);
  int i;
  for (i = 0; i < len; i++)
    {
      if (string[i] == value)
	return 1;
    }
  return 0;
}

int
haswifi (void)
{
#ifdef HAVE_NOWIFI
  return 0;
#elif HAVE_MADWIFI
  return getifcount ("wifi") > 0 ? 1 : 0;
#else
  return 1;
#endif
}

static uint32_t
str_to_addr (const char *addr)
{
  uint32_t split[4];
  uint32_t ip;

  sscanf (addr, "%d.%d.%d.%d", &split[0], &split[1], &split[2], &split[3]);

  ip = (split[0] << 24) | (split[1] << 16) | (split[2] << 8) | (split[3]);

  return htonl (ip);
}

void
getHostName (char *buf, char *ip)
{
  struct hostent *host;
  struct in_addr addr;
  res_init ();
  addr.s_addr = str_to_addr (ip);
  host = gethostbyaddr ((char *) &addr, 4, AF_INET);
  if (!host || !host->h_name)
    strcpy (buf, "unknown");
  else
    strcpy (buf, host->h_name);
}

void
getinterfacelist (const char *ifprefix, char *buffer)
{
  int count = getifcount (ifprefix);
  int i;
  for (i = 0; i < count; i++)
    {
      char ifname[32];
      sprintf (ifname, "%s%d", ifprefix, i);
      strcat (buffer, ifname);
      if (i < count - 1)
	strcat (buffer, " ");
    }
}

/*
 *     the following code was taken from:
 *
 *	Copyright (C) 2006 Jonathan Zarate
 *
 *	Licensed under GNU GPL v2 or later.	
 */

int
f_exists (const char *path)	// note: anything but a directory
{
  struct stat st;
  return (stat (path, &st) == 0) && (!S_ISDIR (st.st_mode));
}

int
f_read (const char *path, void *buffer, int max)
{
  int f;
  int n;

  if ((f = open (path, O_RDONLY)) < 0)
    return -1;
  n = read (f, buffer, max);
  close (f);
  return n;
}


int
f_read_string (const char *path, char *buffer, int max)
{
  if (max <= 0)
    return -1;
  int n = f_read (path, buffer, max - 1);
  buffer[(n > 0) ? n : 0] = 0;
  return n;
}


int
wait_file_exists (const char *name, int max, int invert)
{
  while (max-- > 0)
    {
      if (f_exists (name) ^ invert)
	return 1;
      sleep (1);
    }
  return 0;
}

char *
psname (int pid, char *buffer, int maxlen)
{
  char buf[512];
  char path[64];
  char *p;

  if (maxlen <= 0)
    return NULL;
  *buffer = 0;
  sprintf (path, "/proc/%d/stat", pid);
  if ((f_read_string (path, buf, sizeof (buf)) > 4)
      && ((p = strrchr (buf, ')')) != NULL))
    {
      *p = 0;
      if (((p = strchr (buf, '(')) != NULL) && (atoi (buf) == pid))
	{
	  strlcpy (buffer, p + 1, maxlen);
	}
    }
  return buffer;
}

static int
_pidof (const char *name, pid_t ** pids)
{
  const char *p;
  char *e;
  DIR *dir;
  struct dirent *de;
  pid_t i;
  int count;
  char buf[256];

  count = 0;
  *pids = NULL;
  if ((p = strchr (name, '/')) != NULL)
    name = p + 1;
  if ((dir = opendir ("/proc")) != NULL)
    {
      while ((de = readdir (dir)) != NULL)
	{
	  i = strtol (de->d_name, &e, 10);
	  if (*e != 0)
	    continue;
	  if (strcmp (name, psname (i, buf, sizeof (buf))) == 0)
	    {
	      if ((*pids =
		   realloc (*pids, sizeof (pid_t) * (count + 1))) == NULL)
		{
		  return -1;
		}
	      (*pids)[count++] = i;
	    }
	}
    }
  closedir (dir);
  return count;
}

int
pidof (const char *name)
{
  pid_t *pids;
  pid_t p;

  if (_pidof (name, &pids) > 0)
    {
      p = *pids;
      free (pids);
      return p;
    }
  return -1;
}

int
killall (const char *name, int sig)
{
  pid_t *pids;
  int i;
  int r;

  if ((i = _pidof (name, &pids)) > 0)
    {
      r = 0;
      do
	{
	  r |= kill (pids[--i], sig);
	}
      while (i > 0);
      free (pids);
      return r;
    }
  return -2;
}

int isGrep(char *string,char *cmp)
{
  char devcall[128];
  int res;
  sprintf (devcall, "%s|grep \"%s\"|/bin/wc -l",string,cmp);
//system(devcall);
  FILE *in = popen (devcall, "rb");
  fscanf (in, "%d", &res);
  fclose (in);
  return res > 0 ? 1 : 0;

}

int
softkill (char *name)
{
  killall (name, SIGKILL);
  return 0;
}
#ifdef HAVE_AQOS

static char *
get_wshaper_dev (void)
{
  if (nvram_match ("wshaper_dev", "WAN"))
    return get_wan_face ();
  else
    return "br0";
}

void add_userip(char *ip, int idx,char *upstream,char *downstream)
{
int base = 120+idx;
char up[32];
char down[32];
char ups[32];
char downs[32];
sprintf(up,"1:%d",base);
sprintf(down,"1:%d",base+1);
sprintf(ups,"%skbit",upstream);
sprintf(downs,"%skbit",downstream);

if (nvram_match("qos_type","0"))
    {
    eval("tc","class","add","dev",get_wshaper_dev(),"parent","1:","classid",up,"htb","rate",ups,"ceil",ups);
    eval("tc","filter","add","dev",get_wshaper_dev(),"parent","1:","protocol","ip","prio","1","u32","match","ip","src",ip,"flowid",up);
    eval("tc","class","add","dev","imq0","parent","1:","classid",down,"htb","rate",downs,"ceil",downs);
    eval("tc","filter","add","dev","imq0","parent","1:","protocol","ip","prio","1","u32","match","ip","dst",ip,"flowid",down);
    }else
    {
    eval("tc","class","add","dev",get_wshaper_dev(),"parent","1:","classid",up,"htb","rate",ups,"ceil",ups);
    eval("tc","filter","add","dev",get_wshaper_dev(),"parent","1:","protocol","ip","prio","1","u32","match","ip","src",ip,"flowid",up);
    eval("tc","class","add","dev","imq0","parent","1:","classid",down,"htb","rate",downs,"ceil",downs);
    eval("tc","filter","add","dev","imq0","parent","1:","protocol","ip","prio","1","u32","match","ip","dst",ip,"flowid",down);
    }
    
}

void add_usermac(char *mac, int idx,char *upstream,char *downstream)
{
unsigned char octet[6];
ether_atoe (mac, octet);

int base = 120+idx;
char up[32];
char down[32];
char ups[32];
char downs[32];
char oct2[32];
char oct4[32];
char doct2[32];
char doct4[32];
sprintf(up,"1:%d",base);
sprintf(down,"1:%d",base+1);
sprintf(ups,"%skbit",upstream);
sprintf(downs,"%skbit",downstream);

sprintf(oct2,"%X%X",octet[4],octet[5]);
sprintf(oct4,"%X%X%X%X",octet[0],octet[1],octet[2],octet[3]);

sprintf(doct4,"%X%X%X%X",octet[2],octet[3],octet[4],octet[5]);
sprintf(doct2,"%X%X",octet[0],octet[1]);

if (nvram_match("qos_type","0"))
    {
    eval("tc","class","add","dev",get_wshaper_dev(),"parent","1:","classid",up,"htb","rate",ups,"ceil",ups);
    eval("tc","filter","add","dev",get_wshaper_dev(),"parent","1:","protocol","ip","prio","1","u32","match","u16","0x0800","0xFFFF","at","-2","match","u16",oct2,"0xFFFF","at","-4","match","u32",oct4,"0xFFFFFFFF","at","-8","flowid",up);
    eval("tc","class","add","dev","imq0","parent","1:","classid",down,"htb","rate",downs,"ceil",downs);
    eval("tc","filter","add","dev","imq0","parent","1:","protocol","ip","prio","1","u32","match","u16","0x0800","0xFFFF","at","-2","match","u32",doct4,"0xFFFFFFFF","at","-12","match","u16",doct2,"0xFFFF","at","-14","flowid",down);
    }else
    {
    eval("tc","class","add","dev",get_wshaper_dev(),"parent","1:","classid",up,"htb","rate",ups,"ceil",ups);
    eval("tc","filter","add","dev",get_wshaper_dev(),"parent","1:","protocol","ip","prio","1","u32","match","u16","0x0800","0xFFFF","at","-2","match","u16",oct2,"0xFFFF","at","-4","match","u32",oct4,"0xFFFFFFFF","at","-8","flowid",up);
    eval("tc","class","add","dev","imq0","parent","1:","classid",down,"htb","rate",downs,"ceil",downs);
    eval("tc","filter","add","dev","imq0","parent","1:","protocol","ip","prio","1","u32","match","u16","0x0800","0xFFFF","at","-2","match","u32",doct4,"0xFFFFFFFF","at","-12","match","u16",doct2,"0xFFFF","at","-14","flowid",down);
    }

}


#endif
#ifdef HAVE_X86

static int fd;

void
SetEnvironment ()
{
  system ("stty ispeed 2400 < /dev/tts/1");
  system ("stty raw < /dev/tts/1");
}

int Cmd = 254;			/* EZIO Command */
int cls = 1;			/* Clear screen */
void
Cls ()
{
  write (fd, &Cmd, 1);
  write (fd, &cls, 1);
}

int init = 0x28;
void
Init ()
{
  write (fd, &Cmd, 1);
  write (fd, &init, 1);
}

int stopsend = 0x37;
void
StopSend ()
{
  write (fd, &Cmd, 1);
  write (fd, &init, 1);
}

int home = 2;			/* Home cursor */
void
Home ()
{
  write (fd, &Cmd, 1);
  write (fd, &home, 1);
}

int readkey = 6;		/* Read key */
void
ReadKey ()
{
  write (fd, &Cmd, 1);
  write (fd, &readkey, 1);
}

int blank = 8;			/* Blank display */
void
Blank ()
{
  write (fd, &Cmd, 1);
  write (fd, &blank, 1);
}

int hide = 12;			/* Hide cursor & display blanked characters */
void
Hide ()
{
  write (fd, &Cmd, 1);
  write (fd, &hide, 1);
}

int turn = 13;			/* Turn On (blinking block cursor) */
void
TurnOn ()
{
  write (fd, &Cmd, 1);
  write (fd, &turn, 1);
}

int show = 14;			/* Show underline cursor */
void
Show ()
{
  write (fd, &Cmd, 1);
  write (fd, &show, 1);
}

int movel = 16;			/* Move cursor 1 character left */
void
MoveL ()
{
  write (fd, &Cmd, 1);
  write (fd, &movel, 1);
}

int mover = 20;			/* Move cursor 1 character right */
void
MoveR ()
{
  write (fd, &Cmd, 1);
  write (fd, &mover, 1);
}

int scl = 24;			/* Scroll cursor 1 character left */
void
ScrollL ()
{
  write (fd, &Cmd, 1);
  write (fd, &scl, 1);
}

int scr = 28;			/* Scroll cursor 1 character right */
void
ScrollR ()
{
  write (fd, &Cmd, 1);
  write (fd, &scr, 1);
}

int setdis = 64;		/* Command */
void
SetDis ()
{
  write (fd, &Cmd, 1);
  write (fd, &setdis, 1);

}


int a, b;
void
ShowMessage (char *str1, char *str2)
{
  char nul[] = "                                       ";
  a = strlen (str1);
  b = 40 - a;
  write (fd, str1, a);
  write (fd, nul, b);
  write (fd, str2, strlen (str2));
}

void
initlcd ()
{

  fd = open ("/dev/tts/1", O_RDWR);
				  /** Open Serial port (COM2) */
  if (fd > 0)
    {
      close (fd);
      SetEnvironment ();	/* Set RAW mode */
      fd = open ("/dev/tts/1", O_RDWR);
      Init ();			/* Initialize EZIO twice */
      Init ();

      Cls ();			/* Clear screen */
    }
  close (fd);
}

void
lcdmessage (char *message)
{
  fd = open ("/dev/tts/1", O_RDWR);/** Open Serial port (COM2) */

  if (fd > 0)
    {
      Init ();			/* Initialize EZIO twice */
      Init ();
      SetDis ();
      Cls ();
      Home ();
      ShowMessage ("State", message);
      close (fd);
    }
}
void
lcdmessaged (char *dual, char *message)
{

  fd = open ("/dev/tts/1", O_RDWR);
				  /** Open Serial port (COM2) */

  if (fd > 0)
    {
      Init ();			/* Initialize EZIO twice */
      Init ();
      SetDis ();
      Cls ();			/* Clear screen */
      Home ();
      ShowMessage (dual, message);
      close (fd);
    }
}


#endif

