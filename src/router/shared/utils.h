#ifndef UTILS_H
#define UTILS_H

#ifdef CDEBUG
#include <shutils.h>
#include <malloc.h>
#include <cy_conf.h>
#endif

/* 2 byte router ID number; Eko 4.jul.06

X X X X X X X X   R R R P N N N N   = 0xXXXX
---------------   ----- - -------
 router num         |   |  gpio num (1111 = f = disable resetbutton)
                    |   |
                    |   |--- resetbutton polarity (0: normal, 1 inversed)
                    |
                    |-------- reserved for future use (maybe USB supp)
*/

// Linksys WRT54G, WRT54GS and WRT54GL all models (v2 - v6) except WRT54G v1.0, v1.1 (fccid: Linksys Q87-WRT54G..., Q87_WT54G...)
#define ROUTER_WRT54G 0x0116

// Linksys WRT54Gv8, WRT54GSv7 (5354 cpu, fccid: Linksys Q87-WRT54GV8, Q87-WRT54GSV7)
#define ROUTER_WRT54G_V8 0x0216

// Linksys WRT54G v1.0 and v1.1, Alnet ALL0277 (4702 cpu, fccid: v1: Linksys PKW-WM54G; v1.1: Linksys Q87-WRT54G11)
#define ROUTER_WRT54G1X 0x0316

// Linksys WRTSL54GS (4704 cpu, BCM5325E switch, fccid: Linksys Q87-WTSLGS, same without USB: Q87-WRTH54GS) 
#define ROUTER_WRTSL54GS 0x0416

// Linksys WRT55AG v1 (4702 cpu, dual minipci (Broadcom + Atheros), fccid: Linksys PKW-WRT55AG)
#define ROUTER_LINKSYS_WRT55AG 0x0516

// Asus WL-500G-Deluxe (5365 cpu), (fccid: Asus MSQWL500GD)
#define ROUTER_ASUS_WL500GD 0x0606

// Buffalo WBR-G54, WLA-G54 (4702 cpu, WLA-G54 has no switch)
#define ROUTER_BUFFALO_WBR54G 0x0704

// Buffalo WBR2-G54 (4712 + ADM6996 switch, fccid: Buffalo FDI-04600142-0),
// Buffalo WBR2-G54S, WLA2-G54L (= WLA2-G54 in Japan)
#define ROUTER_BUFFALO_WBR2G54S 0x0807

// Buffalo WLA2-G54C == WLI3-TX1-G54 (4712 cpu, no switch, fccid: Buffalo FDI-09101669-0)
#define ROUTER_BUFFALO_WLA2G54C 0x0910

// Buffalo WHR-G54S (fccid: Buffalo FDI-04600264-0) and WHR-HP-G54 (fccid: Buffalo FDI-09101577-0)
// Buffalo WHR-G125, WHR-HP-G125 (5354 cpu, fccid: Buffalo FDI-09101584-0)
#define ROUTER_BUFFALO_WHRG54S 0x0a04

// Buffalo WZR-RS-G54 (4704 cpu), WZR-G54, WZR-HP-G54 (4704 cpu, fccid: Buffalo FDI-09101457-0), 
// Buffalo WHR3-AG54, WVR-G54-NF, WHR2-A54G54
#define ROUTER_BUFFALO_WZRRSG54 0x0b04

// Motorola WR850G v1 (4702 cpu, fccid Motorola ACQWR850G) 
#define ROUTER_MOTOROLA_V1 0x0c10

// Motorola WR850G v2/v3, WR850GP (4712 cpu, ADM6996 switch, fccid Motorola ACQWR850GV2)
#define ROUTER_MOTOROLA 0x0d15

// RT210W and RT211W generic and branded (4702 cpu, fccid: Askey H8N-RT210W, H8N-RT211W)
// H8N-RT210W: Siemens se505 v1, Belkin F5D7230-4 v1000, v1xxx < 1444
// H8N-RT211W: Belkin F5D7130
#define ROUTER_RT210W 0x0e0f

// RT480W generic and branded (4712 cpu, ADM6996 switch, fccid: Askey H8NRT480W),
// Siemens se505 v2, Belkin F5D7230-4 v1444 (2MB flash, fccid: Belkin K7S-F5D72304)
#define ROUTER_RT480W 0x0f0f

// Microtik RouterBOARD 500
#define ROUTER_BOARD_500 0x100f

// NMN A/B/G Router Protoype (266 Mhz Xscale, dual minipci)
#define ROUTER_BOARD_XSCALE 0x1101

// Generic BRCM 4702 boards: e.g. Asus WL300g (4702 cpu, fccid: Asus MSQWL300G)
#define ROUTER_BRCM4702_GENERIC 0x120f

// Buffalo WLI_TX4_G54HP bridge
#define ROUTER_BUFFALO_WLI_TX4_G54HP 0x1304

// Microsoft MN-700 (4702 cpu), (fccid: Microsoft C3KMN700)
#define ROUTER_MICROSOFT_MN700 0x1417

// Buffalo WLA-G54C == WLI-TX1-G54 (4702 cpu - no switch, fccid: QDS-BRCM1005)
#define ROUTER_BUFFALO_WLAG54C 0x1510

// Asus WL-500g Premium (4704 cpu, BCM5325E switch, (fccid: Asus MSQWL500GP)
#define ROUTER_ASUS_WL500G_PRE 0x1600

// Buffalo WZR-G300N MIMO Router (radio fccid: Buffalo FDI-09101466-0)
#define ROUTER_BUFFALO_WZRG300N 0x1704

// Linksys WRT300N v1 (4704 cpu, BCM5325F switch, fccid: Linksys Q87-WRT300N or Q87-WRT300NV1)
#define ROUTER_WRT300N 0x1816

// Buffalo WHR-AM54G54
#define ROUTER_BUFFALO_WHRAM54G54 0x190f

// Magicbox PowerPC board
#define ROUTER_BOARD_MAGICBOX 0x1a01

// Magicbox PowerPC board 2.0
//#define ROUTER_BOARD_MAGICBOX20 0x1b01

// Buffalo WLI2-TX1-G54 Access point (4702 cpu, no switch)
#define ROUTER_BUFFALO_WLI2_TX1_G54 0x1c10

// NMN A/B/G Router Protoype (266 Mhz Xscale, four minipci)
#define ROUTER_BOARD_GATEWORX 0x1d04

// Motorola WE800G v1 (4702 cpu, no switch, minipci radio, fccid: Motorola ACQWE800G, F2NWE800G)
#define ROUTER_MOTOROLA_WE800G 0x1e10

// Generic x86 PC / WRAP / Soekris
#define ROUTER_BOARD_X86 0x1f0f

// ?
#define ROUTER_SUPERGERRY 0x200f

// Linksys WRT350N (4705 cpu, Gbit switch, PCMCIA radio card, fccid: Linksys Q87-WRT350N)
#define ROUTER_WRT350N 0x2116

// Linksys WAP54G v1 and v1.1 (4702 cpu, no switch, fccid: Linksys PKW-WAP54G)
// 2 different boards (same fccid): WX5510_Vxx and WX5541_Vxx
#define ROUTER_WAP54G_V1 0x2210

// Linksys WAP54G v2 (4712 cpu, no switch, fccid: Linksys Q87-WAP54GV2)
#define ROUTER_WAP54G_V2 0x2310

// ViewSonic WAPBR-100 (4712 cpu, no switch, fccid: Viewsonic GSS-VS10407)
#define ROUTER_VIEWSONIC_WAPBR_100 0x2417

// Dell TrueMobile 2300 v1 (4702 cpu, BCM5325 switch, mini pci radio, fccid: Gemtek MXF-R920401G)
#define ROUTER_DELL_TRUEMOBILE_2300 0x2510

// Dell TrueMobile 2300 v2 (4712 cpu, ADM6996 switch, fccid: Gemtek MXF-R921212G)
// Sparklan WX-6615GT (4712 cpu, ADM6996 switch, fccid: Gemtek MXF-R930116G)
#define ROUTER_DELL_TRUEMOBILE_2300_V2 0x2610

// Fonera (Atheros SoC, no switch, fccid: Accton HED-FON2100)
#define ROUTER_BOARD_FONERA 0x2701

// Buffalo WHR-HP-AG108 (Atheros A+G, Kendin KS8895XA switch, fccid: Buffalo FDI-09101540-0)
#define ROUTER_BOARD_WHRAG108 0x2801

// swapped phy definition 
#define ROUTER_BOARD_GATEWORX_SWAP 0x290f

// Netgear WNR834B (4704 cpu, BCM5325 switch, cardbus radio,  fccid: Netgear PY306100032)
#define ROUTER_NETGEAR_WNR834B 0x2a17

// swapped phy definition and Kendin switch
#define ROUTER_BOARD_GATEWORX_GW2345 0x2b0f

// Linksys WRT54G3G  (4712 cpu, 5325E switch, PCMCIA slot, fccid: Linksys Q87-WRT54G3G)
#define ROUTER_WRT54G3G 0x2c16

// Sitecom WL-105(b) (4702 cpu, no switch, fccid: Gemtek MXF-A910910AB - no A-band)
#define ROUTER_SITECOM_WL105B 0x2d10

// Linksys WRT150N (4704 cpu, BCM5325F switch, (fccid: Linksys Q87-WRT150N)
#define ROUTER_WRT150N 0x2e16

// ?
#define ROUTER_BOARD_LS2 0x2f01

// Buffalo WAPM-HP-AM54G54 (only available on japanese market, dual radio 2.4 and 5 GHz Broadcom)
#define ROUTER_BUFFALO_WAPM_HP_AM54G54 0x3005

// Buffalo WLAH-G54 (only available on japanese market, single radio 2.4 ghz)
#define ROUTER_BUFFALO_WLAH_G54 0x3104

// Buffalo WZR-G144HH (only available on japanese market, 802.11n, Gigabit switch. similar to WRT350N)
#define ROUTER_BUFFALO_WZRG144NH 0x3217

// U.S.Robotics USR5430 bridge (4712 cpu, no switch, fccid: ?)
#define ROUTER_USR_5430 0x3310

// U.S.Robotics USR5461 (5350 cpu, fccid: ?)
#define ROUTER_USR_5461 0x340f

// Meraki Mini (Atheros SoC, no switch, fccid: Meraki UDX-MERAKI-MINI)
#define ROUTER_BOARD_MERAKI 0x350f

// Asus WL500g (4702 cpu, fccid: Asus MSQWL500G)
#define ROUTER_ASUS_WL500G 0x3606

// ?
#define ROUTER_BOARD_CA8 0x3706

// Fonera (Atheros SoC,...)
#define ROUTER_BOARD_FONERA2200 0x3801

// ?
#define ROUTER_BOARD_TW6600 0x3901

// Atheros PB42 prototype (AR5416 MIMO Wifi, 400 Mhz Mips CPU, 32 MB RAM)
#define ROUTER_BOARD_PB42 0x3a0f

// Asus WL-500W (MIMO, 4704 cpu, fccid: Asus MSQWL500W)
#define ROUTER_ASUS_WL500W 0x3b06

// Asus WL550gE (5352E cpu, fccid: Asus MSQWL550GE)
#define ROUTER_ASUS_WL550GE 0x3c06

// Sitecom WL-111 (4702 cpu, fccid: ?)
#define ROUTER_SITECOM_WL111 0x3d10

// D-Link DIR-320 (5354 cpu, fccid: ?)
#define ROUTER_DLINK_DIR320 0x3e17

// Linksys WTR54GS travel router (5350 cpu, fccid: v1, v2: Q87-WTR54GS; v2.1 Q87-WTR54GSV21)
#define ROUTER_LINKSYS_WTR54GS 0x3f13

// Belkin F5D7230 v2000 (4712 cpu, 5325E switch, serial flash, fccid: K7SF5D7234A)
#define ROUTER_BELKIN_F5D7230_V2000 0x400f

// Belkin F5D7231-4 v1212UK (5352E cpu, fccid: Askey H8NRT2406W)
// Belkin F5D7231-4 v2000 (5352E cpu, fccid: Belkin K7SF5D7231B)
#define ROUTER_BELKIN_F5D7231 0x410f

// Linksys WAP54G v3, WAP54G v3.1 (5352E cpu, fccid: Linksys Q87-WAP54GV3, Q87-WAP54GV31)
#define ROUTER_WAP54G_V3 0x4210

// Asus WL-520GU / WL-520GC (5354 cpu, fccid: Asus MSQWL520GUGC)
#define ROUTER_ASUS_WL520GUGC 0x4312

// Netgear WG602 v3 (5350 cpu, fccid: Netgear PY3WG602V3)
#define ROUTER_NETGEAR_WG602_V3 0x4411

// Linksys WRT600N (4704 cpu, Gigabit switch, dual radio 2.4 and 5 GHz Broadcom, fccid: Linksys Q87-WRT600NV1, Q87-WRT600NV11)
#define ROUTER_WRT600N 0x4517

#define NVROUTER "DD_BOARD"

extern char *getBridge (char *ifname);
extern char *getRealBridge (char *ifname);
extern char *getSTA (void);
extern char *getWET (void);
extern int contains (const char *string, char value);
extern int getcpurev (void);
extern int cpu_plltype (void);
extern int check_vlan_support (void);
extern int startswith (char *source, char *cmp);
extern int count_occurences (char *source, int cmp);
extern int pos_nthoccurence (char *source, int cmp, int which);
extern char *substring (int start, int stop, const char *src, char *dst);
extern int getRouterBrand (void);
extern int diag_led (int type, int act);
extern int C_led (int i);
extern int get_single_ip (char *ipaddr, int which);
extern char *get_mac_from_ip (char *ip);
extern struct dns_lists *get_dns_list (void);
extern int dns_to_resolv (void);
extern char *get_wan_face (void);
extern int check_wan_link (int num);
extern char *get_complete_lan_ip (char *ip);
extern int get_int_len (int num);
extern int file_to_buf (char *path, char *buf, int len);
extern int buf_to_file (char *path, char *buf);
extern pid_t *find_pid_by_name (char *pidName);
extern int find_pid_by_ps (char *pidName);
extern int *find_all_pid_by_ps (char *pidName);
extern char *find_name_by_proc (int pid);
extern int get_ppp_pid (char *file);
extern long convert_ver (char *ver);
extern int check_flash (void);
extern int check_action (void);
extern int check_now_boot (void);
extern int check_hw_type (void);
extern int is_exist (char *filename);
extern void set_ip_forward (char c);
struct mtu_lists *get_mtu (char *proto);
extern void set_host_domain_name (void);

extern void encode (char *buf, int len);
extern void decode (char *buf, int len);
extern char * zencrypt (char *passwd);

extern void getLANMac (char *newmac);
extern void getWirelessMac (char *newmac);
extern void getWANMac (char *newmac);

extern int led_control (int type, int act);
enum
{ LED_POWER, LED_DIAG, LED_DMZ, LED_CONNECTED, LED_BRIDGE, LED_VPN,
  LED_SES, LED_SES2, LED_WLAN
};
enum
{ LED_ON, LED_OFF, LED_FLASH };

#ifdef CDEBUG
extern int mcoreleft (void);
extern int coreleft (void);
static void
cdebug (char *function)
{
  // FILE *in = fopen ("/tmp/console.log", "a");
  fprintf (stderr, "free ram in %s = %d (malloc %d)\n", function, coreleft (),
	   mcoreleft ());
  // fclose (in);
}

#else
#define cdebug(a)
#endif
extern int first_time (void);

extern int set_register_value (unsigned short port_addr,
			       unsigned short option_content);
extern unsigned long get_register_value (unsigned short id,
					 unsigned short num);
//extern int sys_netdev_ioctl(int family, int socket, char *if_name, int cmd, struct ifreq *ifr);

int ct_openlog (const char *ident, int option, int facility, char *log_name);
void ct_syslog (int level, int enable, const char *fmt, ...);
void ct_logger (int level, const char *fmt, ...);
struct wl_assoc_mac *get_wl_assoc_mac (int *c);

extern struct detect_wans *detect_protocol (char *wan_face, char *lan_face,
					    char *type);

enum
{ WL = 0,
  DIAG = 1,
//  SES_LED1 = 2,
//  SES_LED2 = 3,
  SES_BUTTON = 4,
  DMZ = 7
};

enum
{ START_LED, STOP_LED, MALFUNCTION_LED };

typedef enum
{ ACT_IDLE,
  ACT_TFTP_UPGRADE,
  ACT_WEB_UPGRADE,
#ifdef HAVE_HTTPS
  ACT_WEBS_UPGRADE,
#endif
  ACT_SW_RESTORE,
  ACT_HW_RESTORE,
  ACT_ERASE_NVRAM,
  ACT_NVRAM_COMMIT,
  ACT_UNKNOWN
} ACTION;

enum
{ UNKNOWN_BOOT = -1, PMON_BOOT, CFE_BOOT };

enum
{ BCM4702_CHIP, BCM4712_CHIP, BCM5325E_CHIP, BCM5350_CHIP, BCM5365_CHIP, BCM4704_BCM5325F_CHIP,
  BCM5352E_CHIP, BCM4712_BCM5325E_CHIP, BCM4704_BCM5325F_EWC_CHIP,
  BCM4705_BCM5397_EWC_CHIP, BCM5354G_CHIP,NO_DEFINE_CHIP
};

enum
{ FIRST, SECOND };

enum
{ SYSLOG_LOG = 1, SYSLOG_DEBUG, CONSOLE_ONLY, LOG_CONSOLE, DEBUG_CONSOLE };

#define ACTION(cmd)	buf_to_file(ACTION_FILE, cmd)

struct dns_lists
{
  int num_servers;
  char dns_server[4][16];
};

#define NOT_USING	0
#define USING		1

struct wl_assoc_mac
{
  char mac[18];
};

struct mtu_lists
{
  char *proto;			/* protocol */
  char *min;			/* min mtu */
  char *max;			/* max mtu */
};

struct detect_wans
{
  int proto;
  int count;
  char *name;
  char desc[1024];
};

#define	PROTO_DHCP	0
#define	PROTO_STATIC	1
#define	PROTO_PPPOE	2
#define	PROTO_PPTP	3
#define	PROTO_L2TP	4
#define	PROTO_HB	5
#define	PROTO_ERROR	-1

#define PPP_PSEUDO_IP	"10.64.64.64"
#define PPP_PSEUDO_NM	"255.255.255.255"
#define PPP_PSEUDO_GW	"10.112.112.112"

#define PING_TMP	"/tmp/ping.log"
//#define TRACEROUTE_TMP        "/tmp/traceroute.log"
#define MAX_BUF_LEN	254

#define RESOLV_FILE	"/tmp/resolv.conf"
#define RESOLV_FORW	"/tmp/resolv.dnsmasq"
#define HOSTS_FILE	"/tmp/hosts"

#define LOG_FILE	"/var/log/mess"

#define ACTION_FILE	"/tmp/action"


#define split(word, wordlist, next, delim) \
	for (next = wordlist, \
	     strncpy(word, next, sizeof(word)), \
	     word[(next=strstr(next, delim)) ? strstr(word, delim) - word : sizeof(word) - 1] = '\0', \
	     next = next ? next + sizeof(delim) - 1 : NULL ; \
	     strlen(word); \
	     next = next ? : "", \
	     strncpy(word, next, sizeof(word)), \
	     word[(next=strstr(next, delim)) ? strstr(word, delim) - word : sizeof(word) - 1] = '\0', \
	     next = next ? next + sizeof(delim) - 1 : NULL)

#define STRUCT_LEN(name)    sizeof(name)/sizeof(name[0])

#define printHEX(str,len) { \
	int i; \
	for (i=0 ; i<len ; i++) { \
		printf("%02X ", (unsigned char)*(str+i)); \
		if(((i+1)%16) == 0) printf("- "); \
		if(((i+1)%32) == 0) printf("\n"); \
	} \
	printf("\n\n"); \
}


#define printASC(str,len) { \
	int i; \
	for (i=0 ; i<len ; i++) { \
		printf("%c", (unsigned char)*(str+i)); \
		if(((i+1)%16) == 0) printf("- "); \
		if(((i+1)%32) == 0) printf("\n"); \
	} \
	printf("\n\n"); \
}

#ifdef HAVE_X86
void lcdmessage (char *message);
void initlcd (void);
void lcdmessaged (char *dual, char *message);
#else
#define initlcd()
#define lcdmessage(a)
#define lcdmessaged(a,b)
#endif

#ifdef HAVE_AQOS
extern void add_usermac (char *mac, int idx, char *upstream,
			 char *downstream);
extern void add_userip (char *ip, int idx, char *upstream, char *downstream);
#endif

void getHostName (char *buf, char *ip);
int ishexit (char c);
int haswifi (void);
int sv_valid_hwaddr (char *value);
int sv_valid_ipaddr (char *value);
int sv_valid_range (char *value, int low, int high);
int sv_valid_statics (char *value);
void get_network (char *ipaddr, char *netmask);
void get_broadcast (char *ipaddr, char *netmask);
int route_manip (int cmd, char *name, int metric, char *dst, char *gateway,
		 char *genmask);
int route_add (char *name, int metric, char *dst, char *gateway,
	       char *genmask);
int route_del (char *name, int metric, char *dst, char *gateway,
	       char *genmask);
extern char *psname (int pid, char *buffer, int maxlen);
extern int pidof (const char *name);
extern int killall (const char *name, int sig);
extern int getifcount (const char *ifprefix);
extern int getIfList (char *buffer, const char *ifprefix);
extern int ifexists (const char *ifname);
extern void getinterfacelist (const char *ifprefix, char *buffer);
extern int count_processes (char *pidName);

int isGrep (char *string, char *cmp);
int softkill (char *name);

#define MAX_WDS_DEVS 10
#endif
