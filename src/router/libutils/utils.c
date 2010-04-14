/*
 * utils.c
 *
 * Copyright (C) 2007 Sebastian Gottschall <gottschall@dd-wrt.com>
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
// #include <linux/mii.h>
#include <linux/sockios.h>
#include <cymac.h>
#include <broadcom.h>
#define SIOCGMIIREG	0x8948	/* Read MII PHY register.  */
#define SIOCSMIIREG	0x8949	/* Write MII PHY register.  */

struct mii_ioctl_data {
	unsigned short phy_id;
	unsigned short reg_num;
	unsigned short val_in;
	unsigned short val_out;
};

#ifdef HAVE_FONERA
static void inline getBoardMAC(char *mac)
{
	// 102
	int i;
	char op[32];
	unsigned char data[256];
	FILE *in;

	sprintf(op, "/dev/mtdblock/%d", getMTD("board_config"));
	in = fopen(op, "rb");
	if (in == NULL)
		return;
	fread(data, 256, 1, in);
	fclose(in);
	sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", data[102] & 0xff,
		data[103] & 0xff, data[104] & 0xff, data[105] & 0xff,
		data[106] & 0xff, data[107] & 0xff);
}
#endif

int count_processes(char *pidName)
{
	FILE *fp;
	char line[254];
	char safename[64];

	sprintf(safename, " %s ", pidName);
	char zombie[64];

	sprintf(zombie, "Z   [%s]", pidName);	// do not count zombies
	int i = 0;

	cprintf("Search for %s\n", pidName);
	if ((fp = popen("ps", "r"))) {
		while (fgets(line, sizeof(line), fp) != NULL) {
			if (strstr(line, safename) && !strstr(line, zombie)) {
				i++;
			}
		}
		pclose(fp);
	}
	cprintf("Search done... %d\n", i);

	return i;
}

/*
 * This function returns the number of days for the given month in the given
 * year 
 */
unsigned int daysformonth(unsigned int month, unsigned int year)
{
	return (30 + (((month & 9) == 8) || ((month & 9) == 1)) -
		(month == 2) - (!(((year % 4) == 0)
				  && (((year % 100) != 0)
				      || ((year % 400) == 0)))
				&& (month == 2)));
}

#ifdef HAVE_AQOS

static char *get_wshaper_dev(void)
{
	if (nvram_match("wshaper_dev", "WAN"))
		return get_wan_face();
	else
		return "br0";
}

void add_userip(char *ip, int idx, char *upstream, char *downstream)
{
	int base = 120 + idx;
	char up[32];
	char down[32];
	char ups[32];
	char downs[32];

	if (nvram_match("qos_type", "1")) {
		sprintf(up, "1:%d", base);
		sprintf(down, "1:%d", base + 1);
		sprintf(ups, "%skbit", upstream);
		sprintf(downs, "%skbit", downstream);
		sysprintf
		    ("tc class add dev %s parent 1:1 classid 1:%d htb rate %skbit ceil %skbit",
		     "imq0", base, upstream, upstream);
		sysprintf
		    ("tc qdisc add dev %s parent 1:%d sfq quantum 1514b perturb 15",
		     "imq0", base);
		sysprintf
		    ("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match ip src %s flowid 1:%d",
		     "imq0", ip, base);

		sysprintf
		    ("tc class add dev imq0 parent 1:1 classid 1:%d htb rate %skbit ceil %skbit",
		     base + 1, downstream, downstream);
		sysprintf
		    ("tc qdisc add dev imq0 parent 1:%d sfq quantum 1514b perturb 15",
		     base + 1, base + 1);
		sysprintf
		    ("tc filter add dev imq0 protocol ip parent 1:0 prio 1 u32 match ip dst %s flowid 1:%d",
		     ip, base + 1);

	} else {
		sprintf(up, "1:%d", base);
		sprintf(down, "1:%d", base + 1);
		sprintf(ups, "%skbit", upstream);
		sprintf(downs, "%skbit", downstream);
		sysprintf
		    ("tc class add dev %s parent 1:2 classid 1:%d htb rate %skbit ceil %skbit",
		     "imq0", base, upstream, upstream);
		sysprintf
		    ("tc qdisc add dev %s parent 1:%d sfq quantum 1514b perturb 15",
		     "imq0", base);
		sysprintf
		    ("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match ip src %s flowid 1:%d",
		     "imq0", ip, base);

		sysprintf
		    ("tc class add dev imq0 parent 1:2 classid 1:%d htb rate %skbit ceil %skbit",
		     base + 1, downstream, downstream);
		sysprintf
		    ("tc qdisc add dev imq0 parent 1:%d sfq quantum 1514b perturb 15",
		     base + 1, base + 1);
		sysprintf
		    ("tc filter add dev imq0 protocol ip parent 1:0 prio 1 u32 match ip dst %s flowid 1:%d",
		     ip, base + 1);
	}
}

void add_usermac(char *mac, int idx, char *upstream, char *downstream)
{
	unsigned char octet[6];

	ether_atoe(mac, octet);
	int base = 120 + idx;
	char up[32];
	char down[32];
	char ups[32];
	char downs[32];
	char oct2[32];
	char oct4[32];
	char doct2[32];
	char doct4[32];

	sprintf(up, "1:%d", base);
	sprintf(down, "1:%d", base + 1);
	sprintf(ups, "%skbit", upstream);
	sprintf(downs, "%skbit", downstream);

	sprintf(oct2, "%02X%02X", octet[4], octet[5]);
	sprintf(oct4, "%02X%02X%02X%02X", octet[0], octet[1], octet[2],
		octet[3]);

	sprintf(doct4, "%02X%02X%02X%02X", octet[2], octet[3], octet[4],
		octet[5]);
	sprintf(doct2, "%02X%02X", octet[0], octet[1]);

	if (nvram_match("qos_type", "1")) {
		// up
		sysprintf("tc class add dev %s parent 1:1 classid 1:%d htb rate %skbit ceil %skbit", "imq0", base, upstream, upstream);	// 
		sysprintf
		    ("tc qdisc add dev %s parent 1:%d sfq quantum 1514b perturb 15",
		     "imq0", base);
		sysprintf
		    ("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u16 0x%s 0xFFFF at -4 match u32 0x%s 0xFFFFFFFF at -8 flowid 1:%d",
		     "imq0", oct2, oct4, base);

		// down
		if (strcmp(get_wshaper_dev(), "br0")) {
			/*
			 * use separate root class, since no other class is created for br0
			 * if qos is wan based 
			 */
			sysprintf
			    ("tc class add dev br0 parent 1: classid 1:%d htb rate %skbit ceil %skbit",
			     base + 1, downstream, downstream);
			sysprintf
			    ("tc qdisc add dev br0 parent 1:%d sfq quantum 1514b perturb 15",
			     base + 1, base + 1);
			sysprintf
			    ("tc filter add dev br0 protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u32 0x%s 0xFFFFFFFF at -12 match u16 0x%s 0xFFFF at -14 flowid 1:%d",
			     doct4, doct2, base + 1);
		} else {
			/*
			 * use root class of br0 interface which was created by the wshaper 
			 */
			// br0 -> imq0 changed, in lan-briding, we need to use IMQ
			sysprintf
			    ("tc class add dev imq0 parent 1:1 classid 1:%d htb rate %skbit ceil %skbit",
			     base + 1, downstream, downstream);
			sysprintf
			    ("tc qdisc add dev imq0 parent 1:%d sfq quantum 1514b perturb 15",
			     base + 1, base + 1);
			sysprintf
			    ("tc filter add dev imq0 protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u32 0x%s 0xFFFFFFFF at -12 match u16 0x%s 0xFFFF at -14 flowid 1:%d",
			     doct4, doct2, base + 1);
		}

	} else {
		// up
		sysprintf("tc class add dev %s parent 1:2 classid 1:%d htb rate %skbit ceil %skbit", "imq0", base, upstream, upstream);	// 
		sysprintf
		    ("tc qdisc add dev %s parent 1:%d sfq quantum 1514b perturb 15",
		     "imq0", base);
		sysprintf
		    ("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u16 0x%s 0xFFFF at -4 match u32 0x%s 0xFFFFFFFF at -8 flowid 1:%d",
		     "imq0", oct2, oct4, base);

		// down
		if (strcmp(get_wshaper_dev(), "br0")) {
			/*
			 * use separate root class, since no other class is created for br0
			 * if qos is wan based 
			 */
			sysprintf
			    ("tc class add dev br0 parent 1: classid 1:%d htb rate %skbit ceil %skbit",
			     base + 1, downstream, downstream);
			sysprintf
			    ("tc qdisc add dev br0 parent 1:%d sfq quantum 1514b perturb 15",
			     base + 1, base + 1);
			sysprintf
			    ("tc filter add dev br0 protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u32 0x%s 0xFFFFFFFF at -12 match u16 0x%s 0xFFFF at -14 flowid 1:%d",
			     doct4, doct2, base + 1);
		} else {
			/*
			 * use root class of br0 interface which was created by the wshaper 
			 */
			// br0 -> imq0 changed, in lan-briding, we need to use IMQ
			sysprintf
			    ("tc class add dev imq0 parent 1:2 classid 1:%d htb rate %skbit ceil %skbit",
			     base + 1, downstream, downstream);
			sysprintf
			    ("tc qdisc add dev imq0 parent 1:%d sfq quantum 1514b perturb 15",
			     base + 1, base + 1);
			sysprintf
			    ("tc filter add dev imq0 protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u32 0x%s 0xFFFFFFFF at -12 match u16 0x%s 0xFFFF at -14 flowid 1:%d",
			     doct4, doct2, base + 1);
		}
	}
	/*
	 * mac downstream matching can only be made directly on the connected
	 * interface 
	 */
	char iflist[256];

	getIfList(iflist, NULL);
	static char word[256];
	char *next, *wordlist;

	foreach(word, iflist, next) {
		if (nvram_nmatch("0", "%s_bridged", word)) {
			sysprintf
			    ("tc class add dev %s parent 1: classid 1:%d htb rate %skbit ceil %skbit",
			     word, base + 1, downstream, downstream);
			sysprintf
			    ("tc qdisc add dev %s parent 1:%d sfq quantum 1514b perturb 15",
			     word, base + 1, base + 1);
			sysprintf
			    ("tc filter add dev %s protocol ip parent 1:0 prio 1 u32 match u16 0x0800 0xFFFF at -2 match u32 0x%s 0xFFFFFFFF at -12 match u16 0x%s 0xFFFF at -14 flowid 1:%d",
			     word, doct4, doct2, base + 1);
		}
	}

}

#endif
int buf_to_file(char *path, char *buf)
{
	FILE *fp;

	if ((fp = fopen(path, "w"))) {
		fprintf(fp, "%s", buf);
		fclose(fp);
		return 1;
	}

	return 0;
}

int check_action(void)
{
	char buf[80] = "";

	if (file_to_buf(ACTION_FILE, buf, sizeof(buf))) {
		if (!strcmp(buf, "ACT_TFTP_UPGRADE")) {
			fprintf(stderr, "Upgrading from tftp now ...\n");
			return ACT_TFTP_UPGRADE;
		}
#ifdef HAVE_HTTPS
		else if (!strcmp(buf, "ACT_WEBS_UPGRADE")) {
			fprintf(stderr, "Upgrading from web (https) now ...\n");
			return ACT_WEBS_UPGRADE;
		}
#endif
		else if (!strcmp(buf, "ACT_WEB_UPGRADE")) {
			fprintf(stderr, "Upgrading from web (http) now ...\n");
			return ACT_WEB_UPGRADE;
		} else if (!strcmp(buf, "ACT_SW_RESTORE")) {
			fprintf(stderr,
				"Receiving restore command from web ...\n");
			return ACT_SW_RESTORE;
		} else if (!strcmp(buf, "ACT_HW_RESTORE")) {
			fprintf(stderr,
				"Receiving restore command from resetbutton ...\n");
			return ACT_HW_RESTORE;
		} else if (!strcmp(buf, "ACT_NVRAM_COMMIT")) {
			fprintf(stderr, "Committing nvram now ...\n");
			return ACT_NVRAM_COMMIT;
		} else if (!strcmp(buf, "ACT_ERASE_NVRAM")) {
			fprintf(stderr, "Erasing nvram now ...\n");
			return ACT_ERASE_NVRAM;
		}
	}
	// fprintf(stderr, "Waiting for upgrading....\n");
	return ACT_IDLE;
}

int check_vlan_support(void)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX)  || defined(HAVE_RB600) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42) || defined(HAVE_LS5) || defined(HAVE_LSX) || defined(HAVE_DANUBE) || defined(HAVE_STORM) || defined(HAVE_ADM5120) || defined(HAVE_RT2880) || defined(HAVE_OPENRISC)
	return 0;
#else

	int brand = getRouterBrand();

	switch (brand) {
#ifndef HAVE_BUFFALO
	case ROUTER_ASUS_WL500GD:
		return 1;
		break;
#endif
	case ROUTER_BUFFALO_WLAG54C:
	case ROUTER_BUFFALO_WLA2G54C:
#ifndef HAVE_BUFFALO
	case ROUTER_LINKSYS_WRT55AG:
	case ROUTER_MOTOROLA_V1:
	case ROUTER_MOTOROLA_WE800G:
	case ROUTER_WAP54G_V1:
	case ROUTER_SITECOM_WL105B:
	case ROUTER_SITECOM_WL111:
	case ROUTER_BUFFALO_WLI2_TX1_G54:
	case ROUTER_BUFFALO_WLI_TX4_G54HP:
	case ROUTER_BRCM4702_GENERIC:
	case ROUTER_ASUS_WL500G:
	case ROUTER_BELKIN_F5D7230_V2000:
	case ROUTER_ASKEY_RT220XD:
#endif
		return 0;
		break;
	}

	unsigned long boardflags =
	    strtoul(nvram_safe_get("boardflags"), NULL, 0);

	if (boardflags & BFL_ENETVLAN)
		return 1;

	if (nvram_match("boardtype", "bcm94710dev")
	    || nvram_match("boardtype", "0x0101") || (boardflags & 0x0100))
		return 1;
	else
		return 0;
#endif
}

void setRouter(char *name)
{
#ifdef HAVE_POWERNOC_WORT54G
	nvram_set(NVROUTER, "WORT54G");
#elif HAVE_POWERNOC_WOAP54G
	nvram_set(NVROUTER, "WOAP54G");
#elif HAVE_ERC
	nvram_set(NVROUTER, "ServiceGate v1.0");
#elif HAVE_OMNI
	nvram_set(NVROUTER, "Omni Wifi Router");
#elif HAVE_ALFA_BRANDING
	nvram_set(NVROUTER, "WLAN base-station");
	if (name)
		nvram_set("DD_BOARD2", name);
#elif HAVE_MAKSAT
	if (name)
		nvram_set("DD_BOARD2", name);
#ifdef HAVE_MAKSAT_BLANK
	nvram_set(NVROUTER, "default");
#else
	nvram_set(NVROUTER, "MAKSAT");
#endif
#elif HAVE_TMK
	if (name)
		nvram_set("DD_BOARD2", name);
	nvram_set(NVROUTER, "KMT-WAS");
#elif HAVE_TRIMAX
	if (name)
		nvram_set("DD_BOARD2", name);
	nvram_set(NVROUTER, "Trimax Wireless");
#elif HAVE_WIKINGS
	if (name)
		nvram_set("DD_BOARD2", name);
#ifdef HAVE_SUB3
	nvram_set(NVROUTER, "ExcelMin");
#elif HAVE_SUB6
	nvram_set(NVROUTER, "ExcelMed");
#else
	nvram_set(NVROUTER, "Excellent");
#endif
#else
	if (name)
		nvram_set(NVROUTER, name);
#endif
}

char *getRouter()
{
	char *n = nvram_get(NVROUTER);

	return n != NULL ? n : "Unknown Model";
}

int internal_getRouterBrand()
{
#if defined(HAVE_ALLNETWRT) && !defined(HAVE_ECB9750)
	unsigned long boardnum = strtoul(nvram_safe_get("boardnum"), NULL, 0);

	if (boardnum == 8 && nvram_match("boardtype", "0x048e")
	    && nvram_match("boardrev", "0x11")) {
		cprintf("router is ALLNET01\n");
		setRouter("ALLNET EUROWRT 54");
		return ROUTER_ALLNET01;
	}
	eval("event", "3", "1", "15");
	return 0;
#elif defined(HAVE_ALLNETWRT) && defined(HAVE_EOC5610)
	setRouter("Allnet Outdoor A/B/G CPE");
	return ROUTER_BOARD_LS2;
#else
#ifdef HAVE_NP28G
	setRouter("Compex NP28G");
	return ROUTER_BOARD_NP28G;
#elif HAVE_WP54G
	setRouter("Compex WP54G");
	return ROUTER_BOARD_WP54G;
#elif HAVE_ADM5120
	if (!nvram_match("DD_BOARD", "OSBRiDGE 5LXi"))
		setRouter("Tonze AP-120");
	return ROUTER_BOARD_ADM5120;
#elif HAVE_RB500
	setRouter("Mikrotik RB500");
	return ROUTER_BOARD_500;
#elif HAVE_GEMTEK
	setRouter("SuperGerry");
	return ROUTER_SUPERGERRY;
#elif HAVE_MI424WR
	setRouter("Actiontec MI424WR");
	return ROUTER_BOARD_GATEWORX_GW2345;
#elif HAVE_TONZE
	setRouter("Tonze AP-425");
	return ROUTER_BOARD_GATEWORX;
#elif HAVE_NOP8670
	setRouter("Senao NOP-8670");
	return ROUTER_BOARD_GATEWORX;
#elif HAVE_WRT300NV2
	setRouter("Linksys WRT300N v2");
	return ROUTER_BOARD_GATEWORX;
#elif HAVE_WG302V1
	setRouter("Netgear WG302v1");
	return ROUTER_BOARD_GATEWORX;
#elif HAVE_WG302
	setRouter("Netgear WG302v2");
	return ROUTER_BOARD_GATEWORX;
#elif HAVE_GATEWORX
	char *filename = "/sys/devices/platform/IXP4XX-I2C.0/i2c-adapter:i2c-0/0-0051/eeprom";	/* bank2=0x100 
												 */
	FILE *file = fopen(filename, "r");

	if (file)		// new detection scheme
	{
		fseek(file, 32, SEEK_SET);
		char gwid[9];

		gwid[8] = 0;
		int ret = fread(gwid, 8, 1, file);

		if (ret < 1) {
			fclose(file);
			goto old_way;
		}
		fclose(file);
		if (!strncmp(gwid, "GW2347", 6)) {
			setRouter("Gateworks Avila GW2347");
			return ROUTER_BOARD_GATEWORX_SWAP;
		}
		if (!strncmp(gwid, "GW2357", 6)) {
			setRouter("Gateworks Avila GW2357");
			return ROUTER_BOARD_GATEWORX_SWAP;
		}
		if (!strncmp(gwid, "GW2353", 6)) {
			setRouter("Gateworks Avila GW2353");
			return ROUTER_BOARD_GATEWORX;
		}
		if (!strncmp(gwid, "GW2348-2", 8)) {
			setRouter("Gateworks Avila GW2348-2");
			return ROUTER_BOARD_GATEWORX;
		}
		if (!strncmp(gwid, "GW2348-4", 8)) {
			setRouter("Gateworks Avila GW2348-4");
			return ROUTER_BOARD_GATEWORX;
		}
		if (!strncmp(gwid, "GW2348", 6)) {
			setRouter("Gateworks Avila GW2348-4/2");
			return ROUTER_BOARD_GATEWORX;
		}
		if (!strncmp(gwid, "GW2358", 6)) {
			setRouter("Gateworks Cambria GW2358-4");
			return ROUTER_BOARD_GATEWORX;
		}
		if (!strncmp(gwid, "GW2350", 6)) {
			setRouter("Gateworks Cambria GW2350");
			return ROUTER_BOARD_GATEWORX;
		}
		if (!strncmp(gwid, "GW2369", 6)) {
			setRouter("Gateworks Avila GW2369");
			return ROUTER_BOARD_GATEWORX_GW2369;
		}
		if (!strncmp(gwid, "GW2355", 6)) {
			setRouter("Gateworks Avila GW2355");
			return ROUTER_BOARD_GATEWORX_GW2345;
		}
		if (!strncmp(gwid, "GW2345", 6)) {
			setRouter("Gateworks Avila GW2345");
			return ROUTER_BOARD_GATEWORX_GW2345;
		}
	}
      old_way:;
	struct mii_ioctl_data *data;
	struct ifreq iwr;
	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s < 0) {
		fprintf(stderr, "socket(SOCK_DRAGM)\n");
		setRouter("Gateworks Avila");
		return ROUTER_BOARD_GATEWORX;
	}
	(void)strncpy(iwr.ifr_name, "ixp0", sizeof("ixp0"));
	data = (struct mii_ioctl_data *)&iwr.ifr_data;
	data->phy_id = 1;
#define IX_ETH_ACC_MII_PHY_ID1_REG  0x2	/* PHY identifier 1 Register */
#define IX_ETH_ACC_MII_PHY_ID2_REG  0x3	/* PHY identifier 2 Register */
	data->reg_num = IX_ETH_ACC_MII_PHY_ID1_REG;
	ioctl(s, SIOCGMIIREG, &iwr);
	data->phy_id = 1;
	data->reg_num = IX_ETH_ACC_MII_PHY_ID1_REG;
	ioctl(s, SIOCGMIIREG, &iwr);
	int reg1 = data->val_out;

	data->phy_id = 1;
	data->reg_num = IX_ETH_ACC_MII_PHY_ID2_REG;
	ioctl(s, SIOCGMIIREG, &iwr);
	int reg2 = data->val_out;

	close(s);
	fprintf(stderr, "phy id %X:%X\n", reg1, reg2);
	if (reg1 == 0x2000 && reg2 == 0x5c90) {
		setRouter("Avila GW2347");
		return ROUTER_BOARD_GATEWORX_SWAP;
	} else if (reg1 == 0x13 && reg2 == 0x7a11) {
#if HAVE_ALFA_BRANDING
		setRouter("WLAN base-station");
#else
		setRouter("Gateworks Avila GW2348-4/2");
#endif
		return ROUTER_BOARD_GATEWORX;
	} else if (reg1 == 0x143 && reg2 == 0xbc31)	// broadcom phy
	{
		setRouter("ADI Engineering Pronghorn Metro");
		return ROUTER_BOARD_GATEWORX;
	} else if (reg1 == 0x22 && reg2 == 0x1450)	// kendin switch 
	{
		setRouter("Gateworks Avila GW2345");
		return ROUTER_BOARD_GATEWORX_GW2345;
	} else if (reg1 == 0x0 && reg2 == 0x8201)	// realtek 
	{
		setRouter("Compex WP188");
		return ROUTER_BOARD_GATEWORX;
	} else {
		setRouter("Unknown");
		return ROUTER_BOARD_GATEWORX;
	}
#elif HAVE_RT2880

#ifdef HAVE_ECB9750
#ifdef HAVE_ALLNETWRT
	setRouter("Allnet 802.11n Router");
#else
	setRouter("Senao ECB-9750");
#endif
	return ROUTER_BOARD_ECB9750;
#elif HAVE_ALLNET11N
	setRouter("Allnet 802.11n Router");
	return ROUTER_BOARD_WHRG300N;
#elif HAVE_AR670W
	setRouter("Airlink 101 AR670W");
	return ROUTER_BOARD_AR670W;
#elif HAVE_AR690W
	setRouter("Airlink 101 AR690W");
	return ROUTER_BOARD_AR690W;
#elif HAVE_BR6574N
	setRouter("Edimax BR-6574N");
	return ROUTER_BOARD_BR6574N;
#elif HAVE_ESR6650
	setRouter("Senao ESR-6650");
	return ROUTER_BOARD_ESR6650;
#elif HAVE_EAP9550
	setRouter("Senao EAP-9550");
	return ROUTER_BOARD_EAP9550;
#elif HAVE_ESR9752
	setRouter("Senao ESR-9752");
	return ROUTER_BOARD_ESR9752;
#elif HAVE_ACXNR22
	setRouter("Aceex NR22");
	return ROUTER_BOARD_ACXNR22;
#elif HAVE_W502U
	setRouter("Alfa W502U");
	return ROUTER_BOARD_W502U;
#elif HAVE_DIR615
	setRouter("Dlink-DIR615 rev d");
	return ROUTER_BOARD_DIR615D;
#elif HAVE_DIR600
#ifdef HAVE_DIR300
	setRouter("Dlink-DIR300 rev b");
#else
	setRouter("Dlink-DIR600 rev b");
#endif
	return ROUTER_BOARD_DIR600B;
#elif HAVE_ASUSRTN13U
	setRouter("Asus RT-N13U");
	return ROUTER_BOARD_WHRG300N;
#elif HAVE_WHRG300N
	setRouter("Buffalo WHR-G300N");
	return ROUTER_BOARD_WHRG300N;
#else
	setRouter("Generic RT2880");
	return ROUTER_BOARD_RT2880;
#endif
#elif HAVE_X86
#ifdef HAVE_CORENET
	setRouter("CORENET X86i");
	return ROUTER_BOARD_X86;
#else
	setRouter("Generic X86");
	return ROUTER_BOARD_X86;
#endif
#elif HAVE_XSCALE
	setRouter("NewMedia Dual A/B/G");
	return ROUTER_BOARD_XSCALE;
#elif HAVE_MAGICBOX
	setRouter("OpenRB PowerPC Board");
	return ROUTER_BOARD_MAGICBOX;
#elif HAVE_RB1000
	setRouter("Mikrotik RB1000");
	return ROUTER_BOARD_RB600;
#elif HAVE_RB800
	setRouter("Mikrotik RB800");
	return ROUTER_BOARD_RB600;
#elif HAVE_RB600
	setRouter("Mikrotik RB600");
	return ROUTER_BOARD_RB600;
#elif HAVE_GWMF54G2
	setRouter("Planex GW-MF54G2");
	char mac[32];
	getBoardMAC(mac);
	if (!strncmp(mac, "00:19:3B", 8) || !strncmp(mac, "00:02:6F", 8)
	    || !strncmp(mac, "00:15:6D", 8)) {
		fprintf(stderr, "unsupported board\n");
		sys_reboot();
	}
	return ROUTER_BOARD_FONERA;
#elif HAVE_WRT54GV7
	setRouter("Linksys WRT54G v7");
	return ROUTER_BOARD_FONERA;
#elif HAVE_WRK54G
	setRouter("Linksys WRK54G v3");
	return ROUTER_BOARD_FONERA;
#elif HAVE_WGT624
	setRouter("Netgear WGT624 v4");
	return ROUTER_BOARD_FONERA;
#elif HAVE_WPE53G
	setRouter("Compex WPE53G");
	return ROUTER_BOARD_FONERA;
#elif HAVE_NP25G
	setRouter("Compex NP25G");
	return ROUTER_BOARD_FONERA;
#elif HAVE_MR3202A
	setRouter("MR3202A");
	return ROUTER_BOARD_FONERA;
#elif HAVE_DLM101
	setRouter("Doodle Labs DLM-101");
	return ROUTER_BOARD_FONERA;
#elif HAVE_AR430W
	setRouter("Airlink-101 AR430W");
	return ROUTER_BOARD_FONERA;
#elif HAVE_DIR400
	setRouter("D-Link DIR-400");
	return ROUTER_BOARD_FONERA2200;
#elif HAVE_WRT54G2
	setRouter("Linksys WRT54G2 v1.1");
	return ROUTER_BOARD_FONERA;
#elif HAVE_RTG32
	setRouter("Asus RT-G32");
	return ROUTER_BOARD_FONERA;
#elif HAVE_DIR300
	setRouter("D-Link DIR-300");
	return ROUTER_BOARD_FONERA;
#elif HAVE_CNC
	setRouter("WiFi4You Outdoor AP");
	return ROUTER_BOARD_FONERA;
#elif defined(HAVE_CORENET) && defined(HAVE_NS2)
	setRouter("CORENET XNS2");
	return ROUTER_BOARD_LS2;
#elif defined(HAVE_CORENET) && defined(HAVE_LC2)
	setRouter("CORENET XLO2");
	return ROUTER_BOARD_LS2;
#elif defined(HAVE_CORENET) && defined(HAVE_EOC2610)
	setRouter("CORENET XC61");
	return ROUTER_BOARD_FONERA;
#elif defined(HAVE_CORENET) && defined(HAVE_EOC1650)
	setRouter("CORENET XC65");
	return ROUTER_BOARD_FONERA;
#elif defined(HAVE_CORENET) && defined(HAVE_BS2)
	setRouter("CORENET XBU2");
	return ROUTER_BOARD_LS2;
#elif defined(HAVE_CORENET) && defined(HAVE_BS2HP)
	setRouter("CORENET MBU2i");
	return ROUTER_BOARD_LS2;
#elif HAVE_WBD500
	setRouter("Wiligear WBD-500");
	return ROUTER_BOARD_FONERA;
#elif HAVE_EOC1650
	setRouter("Senao EOC-1650");
	return ROUTER_BOARD_FONERA;
#elif HAVE_EOC2610
#ifdef HAVE_TRIMAX
	setRouter("TMAX-1200");
#else
	setRouter("Senao EOC-2610");
#endif
	return ROUTER_BOARD_FONERA;
#elif HAVE_ECB3500
	setRouter("Senao ECB-3500");
	return ROUTER_BOARD_FONERA;
#elif HAVE_EAP3660
	setRouter("Senao EAP-3660");
	return ROUTER_BOARD_FONERA;
#elif HAVE_MR3201A
	setRouter("Accton MR3201A");
	return ROUTER_BOARD_FONERA;
#elif HAVE_FONERA
	struct mii_ioctl_data *data;
	struct ifreq iwr;
	char mac[32];
	getBoardMAC(mac);
	if (!strncmp(mac, "00:19:3B", 8) || !strncmp(mac, "00:02:6F", 8)
	    || !strncmp(mac, "00:15:6D", 8) || !strncmp(mac, "00:C0:CA", 8)) {
		fprintf(stderr, "unsupported board\n");
		sys_reboot();
	}
	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s < 0) {
		fprintf(stderr, "socket(SOCK_DRAGM)\n");
		setRouter("Fonera 2100/2200");
		return ROUTER_BOARD_FONERA;
	}
	(void)strncpy(iwr.ifr_name, "eth0", sizeof("eth0"));
	data = (struct mii_ioctl_data *)&iwr.ifr_data;
	data->phy_id = 0x10;
	data->reg_num = 0x2;
	ioctl(s, SIOCGMIIREG, &iwr);
	data->phy_id = 0x10;
	data->reg_num = 0x2;
	ioctl(s, SIOCGMIIREG, &iwr);
	if (data->val_out == 0x0141) {
		data->phy_id = 0x10;
		data->reg_num = 0x3;
		ioctl(s, SIOCGMIIREG, &iwr);
		close(s);
		if ((data->val_out & 0xfc00) != 0x0c00)	// marvell phy
		{
			setRouter("Fonera 2100/2200");
			return ROUTER_BOARD_FONERA;
		} else {
			setRouter("Fonera 2201");
			return ROUTER_BOARD_FONERA2200;
		}
	} else {
		setRouter("Fonera 2100/2200");
		return ROUTER_BOARD_FONERA;
	}
#elif HAVE_MERAKI
	setRouter("Meraki Mini");
	return ROUTER_BOARD_MERAKI;
#elif HAVE_BWRG1000
	setRouter("Bountiful BWRG-1000");
	return ROUTER_BOARD_LS2;
#elif HAVE_WHRHPG300N
#ifdef HAVE_BUFFALO
	setRouter("WHR-HP-G300N");
#else
	setRouter("Buffalo WHR-HP-G300N");
#endif
	nvram_default_get("ath0_rxantenna", "3");
	nvram_default_get("ath0_txantenna", "3");
	return ROUTER_BOARD_WHRHPGN;
#elif HAVE_WHRG300NV2
#ifdef HAVE_BUFFALO
	setRouter("WHR-G300N");
#else
	setRouter("Buffalo WHR-G300N");
#endif
	nvram_default_get("ath0_rxantenna", "3");
	nvram_default_get("ath0_txantenna", "3");
	return ROUTER_BOARD_WHRHPGN;
#elif HAVE_WHRHPGN
#ifdef HAVE_BUFFALO
	setRouter("WHR-HP-GN");
#else
	setRouter("Buffalo WHR-HP-GN");
#endif
	nvram_default_get("ath0_rxantenna", "1");
	nvram_default_get("ath0_txantenna", "1");
	return ROUTER_BOARD_WHRHPGN;
#elif HAVE_UBNTM
	typedef struct UBNTDEV {
		char *devicename;	// device name 
		unsigned short devid;	// pci subdevice id
		char *rxchain;	// rx chainmask
		char *txchain;	// tx chainmask
		int dddev;	// dd-wrt device id
	};

	struct UBNTDEV dev[] = {
		{"Ubiquiti Nanostation M2", 0xe002, "3", "3", ROUTER_BOARD_NS2M},	//
		{"Ubiquiti Nanostation M2", 0xe012, "3", "3", ROUTER_BOARD_NS2M},	//
		{"Ubiquiti Nanostation M5", 0xe005, "3", "3", ROUTER_BOARD_NS5M},	//
		{"Ubiquiti Rocket M2", 0xe102, "3", "3", ROUTER_BOARD_R2M},	//
		{"Ubiquiti Rocket M2", 0xe112, "3", "3", ROUTER_BOARD_R2M},	//
		{"Ubiquiti Rocket M2", 0xe1b2, "3", "3", ROUTER_BOARD_R2M},	//
		{"Ubiquiti Rocket M5", 0xe105, "3", "3", ROUTER_BOARD_R5M},	//
		{"Ubiquiti Rocket M5", 0xe1b5, "3", "3", ROUTER_BOARD_R5M},	//
		{"Ubiquiti Bullet M2", 0xe202, "1", "1", ROUTER_BOARD_BS5M},	//
		{"Ubiquiti Bullet M5", 0xe202, "1", "1", ROUTER_BOARD_BS5M},	//
		{"Ubiquiti Airgrid 2M", 0xe212, "1", "1", ROUTER_BOARD_BS2M},	//
		{"Ubiquiti Airgrid 5M", 0xe215, "1", "1", ROUTER_BOARD_BS5M},	//
		{"Ubiquiti Pico M2", 0xe302, "1", "1", ROUTER_BOARD_BS2M},	//
		{"Ubiquiti Pico M5", 0xe305, "1", "1", ROUTER_BOARD_BS5M},	//
		{"Ubiquiti Airwire", 0xe405, "3", "3", ROUTER_BOARD_BS5M},	//
		{"Ubiquiti Loco M5", 0xe0a5, "3", "3", ROUTER_BOARD_NS5M},	//
		{"Ubiquiti Litestation M25", 0xe115, "3", "3", ROUTER_BOARD_NS5M},	//
		{"Ubiquiti AP 1000N", 0xe402, "3", "3", ROUTER_BOARD_R2M},	//
		{"Ubiquiti Simple AP", 0xe4a2, "3", "3", ROUTER_BOARD_R2M},	//
		{"Ubiquiti PowerBridge M5", 0xe1a5, "3", "3", ROUTER_BOARD_R5M},	//
		{"Ubiquiti NanoBridge M5", 0xe235, "3", "3", ROUTER_BOARD_BS5M},	//
		{"Ubiquiti NanoBridge M2", 0xe232, "3", "3", ROUTER_BOARD_BS2M},	//
		{"Ubiquiti Rocket M365", 0xe1b3, "3", "3", ROUTER_BOARD_R5M},	//
		{"Ubiquiti Rocket M900", 0xe1b9, "3", "3", ROUTER_BOARD_R2M},	//
		{NULL, 0, NULL, NULL, 0},	//
	};

	FILE *fp =
	    fopen("/sys/bus/pci/devices/0000:00:00.0/subsystem_device", "rb");
	if (fp == NULL)
		return ROUTER_BOARD_PB42;
	int device;
	fscanf(fp, "0x%04X", &device);
	fclose(fp);
	int devcnt = 0;
	while (dev[devcnt].devicename != NULL) {
		if (dev[devcnt].devid == device) {
			nvram_set("ath0_rxantenna", dev[devcnt].rxchain);
			nvram_set("ath0_txantenna", dev[devcnt].txchain);
			setRouter(dev[devcnt].devicename);
			return dev[devcnt].dddev;
		}
		devcnt++;
	}
	setRouter("Ubiquiti Unknown Model");
	return ROUTER_BOARD_PB42;
#elif HAVE_NS2
	setRouter("Ubiquiti Nanostation 2");
	return ROUTER_BOARD_LS2;
#elif HAVE_EOC5610
	setRouter("Senao EOC-5610");
	return ROUTER_BOARD_LS2;
#elif HAVE_NS5
	setRouter("Ubiquiti Nanostation 5");
	return ROUTER_BOARD_LS2;
#elif HAVE_SOLO51
	setRouter("Alfa SoLo48-N");
	return ROUTER_BOARD_LS2;
#elif HAVE_NS3
	setRouter("Ubiquiti Nanostation 3");
	return ROUTER_BOARD_LS2;
#elif HAVE_BS5
	setRouter("Ubiquiti Bullet 5");
	return ROUTER_BOARD_LS2;
#elif HAVE_BS2
	setRouter("Ubiquiti Bullet 2");
	return ROUTER_BOARD_LS2;
#elif HAVE_PICO2
	setRouter("Ubiquiti PicoStation 2");
	return ROUTER_BOARD_LS2;
#elif HAVE_PICO2HP
	setRouter("Ubiquiti PicoStation 2 HP");
	return ROUTER_BOARD_LS2;
#elif HAVE_PICO5
	setRouter("Ubiquiti PicoStation 5");
	return ROUTER_BOARD_LS2;
#elif HAVE_MS2
	setRouter("Ubiquiti MiniStation");
	return ROUTER_BOARD_LS2;
#elif HAVE_BS2HP
	setRouter("Ubiquiti Bullet 2 HP");
	return ROUTER_BOARD_LS2;
#elif HAVE_LC2
	setRouter("Ubiquiti Nanostation Loco 2");
	return ROUTER_BOARD_LS2;
#elif HAVE_LC5
	setRouter("Ubiquiti Nanostation Loco 5");
	return ROUTER_BOARD_LS2;
#elif HAVE_PS2
	setRouter("Ubiquiti Powerstation 2");
	return ROUTER_BOARD_LS2;
#elif HAVE_PS5
	setRouter("Ubiquiti Powerstation 5");
	return ROUTER_BOARD_LS2;
#elif HAVE_LS2
	setRouter("Ubiquiti Litestation 2");
	return ROUTER_BOARD_LS2;
#elif HAVE_LS5
	setRouter("Ubiquiti Litestation 5");
	return ROUTER_BOARD_LS2;
#elif HAVE_WHRAG108
	setRouter("Buffalo WHR-HP-AG108");
	return ROUTER_BOARD_WHRAG108;
#elif HAVE_PB42
	setRouter("Atheros PB42");
	return ROUTER_BOARD_PB42;
#elif HAVE_RSPRO
	setRouter("Ubiquiti RouterStation Pro");
	return ROUTER_BOARD_PB42;
#elif HAVE_RS
#ifdef HAVE_DDLINK
	setRouter("ddlink1x1");
#else
	setRouter("Ubiquiti RouterStation");
#endif
	return ROUTER_BOARD_PB42;
#elif HAVE_WRT160NL
	setRouter("Linksys WRT160NL");
	return ROUTER_BOARD_PB42;
#elif HAVE_TG2521
	setRouter("ZCom TG-2521");
	return ROUTER_BOARD_PB42;
#elif HAVE_WZRG300NH
#ifdef HAVE_BUFFALO
	setRouter("WZR-HP-G300NH");
#else
	setRouter("Buffalo WZR-HP-G300NH");
#endif
	return ROUTER_BOARD_PB42;
#elif HAVE_WNDR3700
	setRouter("Netgear WNDR3700");
	return ROUTER_BOARD_PB42;
#elif HAVE_DIR825
	setRouter("Dlink DIR-825");
	return ROUTER_BOARD_PB42;
#elif HAVE_WRT400
	setRouter("Linksys WRT400N");
	return ROUTER_BOARD_PB42;
#elif HAVE_DIR615C1
	setRouter("D-Link DIR-615-C1");
	return ROUTER_BOARD_PB42;
#elif HAVE_DIR615E
	setRouter("D-Link DIR-615-E1");
	return ROUTER_BOARD_PB42;
#elif HAVE_TEW652BRP
	setRouter("Trendnet TEW-652BRP");
	return ROUTER_BOARD_PB42;
#elif HAVE_TEW632BRP
	setRouter("Trendnet TEW-632BRP");
	return ROUTER_BOARD_PB42;
#elif HAVE_WR1043
	setRouter("TP-Link TL-WR1043ND");
	return ROUTER_BOARD_PB42;
#elif HAVE_AP83
	setRouter("Atheros AP83");
	return ROUTER_BOARD_PB42;
#elif HAVE_WP543
	setRouter("Compex WP543");
	return ROUTER_BOARD_PB42;
#elif HAVE_JA76PF
	setRouter("JJPLUS JA76PF");
	return ROUTER_BOARD_PB42;
#elif HAVE_LSX
	setRouter("Ubiquiti Litestation-SR71");
	return ROUTER_BOARD_PB42;
#elif HAVE_DANUBE
	setRouter("Infineon Danube");
	return ROUTER_BOARD_DANUBE;
#elif HAVE_WBD222
	setRouter("Wiligear WBD-222");
	return ROUTER_BOARD_STORM;
#elif HAVE_STORM
	setRouter("Wiligear WBD-111");
	return ROUTER_BOARD_STORM;
#elif HAVE_OPENRISC
	setRouter("Alekto OpenRisc");
	return ROUTER_BOARD_OPENRISC;
#elif HAVE_TW6600
	setRouter("AW-6660");
	return ROUTER_BOARD_TW6600;
#elif HAVE_ALPHA
	setRouter("Alfa Networks AP48");
	return ROUTER_BOARD_CA8;
#elif HAVE_USR5453
	setRouter("US Robotics USR5453");
	return ROUTER_BOARD_CA8;
#elif HAVE_RDAT81
	setRouter("Wistron RDAT-81");
	return ROUTER_BOARD_RDAT81;
#elif HAVE_RCAA01
	setRouter("Airlive WLA-9000AP");
	return ROUTER_BOARD_RCAA01;
#elif HAVE_CA8PRO
	setRouter("Wistron CA8-4 PRO");
	return ROUTER_BOARD_CA8PRO;
#elif HAVE_CA8
#ifdef HAVE_WHA5500CPE
	setRouter("Airlive WHA-5500CPE");
#elif HAVE_AIRMAX5
	setRouter("Airlive AirMax 5");
#else
	setRouter("Airlive WLA-5000AP");
#endif
	return ROUTER_BOARD_CA8;
#else

	unsigned long boardnum = strtoul(nvram_safe_get("boardnum"), NULL, 0);
	unsigned long melco_id = strtoul(nvram_safe_get("melco_id"), NULL, 0);

	if (boardnum == 42 && nvram_match("boardtype", "bcm94710ap")) {
		cprintf("router is buffalo\n");
		setRouter("Buffalo WBR-G54 / WLA-G54");
		return ROUTER_BUFFALO_WBR54G;
	}
#ifndef HAVE_BUFFALO
	if (nvram_match("boardnum", "mn700") &&
	    nvram_match("boardtype", "bcm94710ap")) {
		cprintf("router is Microsoft MN-700\n");
		setRouter("Microsoft MN-700");
		return ROUTER_MICROSOFT_MN700;
	}

	if (nvram_match("boardnum", "asusX") &&
	    nvram_match("boardtype", "bcm94710dev")) {
		cprintf("router is Asus WL300g / WL500g\n");
		setRouter("Asus WL-300g / WL-500g");
		return ROUTER_ASUS_WL500G;
	}

	if (boardnum == 44 && nvram_match("boardtype", "bcm94710ap")) {
		cprintf("router is Dell TrueMobile 2300\n");
		setRouter("Dell TrueMobile 2300");
		return ROUTER_DELL_TRUEMOBILE_2300;
	}
#endif

	if (boardnum == 100 && nvram_match("boardtype", "bcm94710dev")) {
		cprintf("router is buffalo\n");
		setRouter("Buffalo WLA-G54C");
		return ROUTER_BUFFALO_WLAG54C;
	}
#ifndef HAVE_BUFFALO
	if (boardnum == 45 && nvram_match("boardtype", "bcm95365r")) {
		cprintf("router is Asus WL-500GD\n");
		setRouter("Asus WL-500g Deluxe");
		return ROUTER_ASUS_WL500GD;
	}

	if (boardnum == 45 && nvram_match("boardtype", "0x04EC")
	    && nvram_match("boardrev", "0x1402")) {
		cprintf("router is Asus RT-N10\n");
		setRouter("Asus RT-N10");
		return ROUTER_ASUS_RTN10;
	}

	if (boardnum == 45 && nvram_match("boardtype", "0x04CD")
	    && nvram_match("boardrev", "0x1201")) {
		cprintf("router is Asus RT-N12\n");
		setRouter("Asus RT-N12");
		return ROUTER_ASUS_RTN12;
	}

	if (boardnum == 45 && nvram_match("boardtype", "0x04cf")
	    && nvram_match("boardrev", "0x1218")) {
		cprintf("router is Asus RT-N16\n");
		setRouter("Asus RT-N16");
		return ROUTER_ASUS_RTN16;
	}

	if (boardnum == 45 && nvram_match("boardtype", "0x0472")
	    && nvram_match("boardrev", "0x23") && nvram_match("parkid", "1")) {
		cprintf("router is Asus WL-500W\n");
		setRouter("Asus WL-500W");
		return ROUTER_ASUS_WL500W;
	}

	if (boardnum == 45 && nvram_match("boardtype", "0x467")) {
		char *hwver0 = nvram_safe_get("hardware_version");

		if (startswith(hwver0, "WL320G")) {
			cprintf("router is Asus WL-320gE/gP\n");
			setRouter("Asus WL-320gE/gP");
			return ROUTER_ASUS_WL550GE;
		} else {
			cprintf("router is Asus WL-550gE\n");
			setRouter("Asus WL-550gE");
			return ROUTER_ASUS_WL550GE;
		}
	}
#endif
	if (nvram_match("boardnum", "00") && nvram_match("boardtype", "0x0101")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Buffalo wbr2\n");
		setRouter("Buffalo WBR2-G54 / WBR2-G54S");
		return ROUTER_BUFFALO_WBR2G54S;
	}

	if (boardnum == 2 && nvram_match("boardtype", "0x0101")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is buffalo wla2-g54c\n");
		setRouter("Buffalo WLA2-G54C / WLI3-TX1-G54");
		return ROUTER_BUFFALO_WLA2G54C;
	}
	if (boardnum == 0 && melco_id == 29090
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Buffalo WLAH-G54\n");
		setRouter("Buffalo WLAH-G54");
		return ROUTER_BUFFALO_WLAH_G54;

	}
	if (boardnum == 0 && melco_id == 31070
	    && nvram_match("boardflags", "0x2288")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Buffalo WAPM-HP-AM54G54\n");
		setRouter("Buffalo WAPM-HP-AM54G54");
		return ROUTER_BUFFALO_WAPM_HP_AM54G54;
	}
	if (nvram_match("boardnum", "00") && nvram_match("boardrev", "0x11")
	    && nvram_match("boardtype", "0x048e") && melco_id == 32093) {
		cprintf("router is Buffalo WHR-G125\n");
		setRouter("Buffalo WHR-G125");
		return ROUTER_BUFFALO_WHRG54S;
	}

	if (nvram_match("boardnum", "00") && nvram_match("boardrev", "0x10")
	    && nvram_match("boardtype", "0x048e") && melco_id == 32139) {
		cprintf("router is Buffalo WCA-G\n");
		setRouter("Buffalo WCA-G");
		return ROUTER_BUFFALO_WCAG;	//vlan1 is lan, vlan0 is unused, implementation not done. will me made after return to germany
	}

	if (nvram_match("boardnum", "00") && nvram_match("boardrev", "0x11")
	    && nvram_match("boardtype", "0x048e") && melco_id == 32064) {
		cprintf("router is Buffalo WHR-HP-G125\n");
		setRouter("Buffalo WHR-HP-G125");
		return ROUTER_BUFFALO_WHRG54S;
	}

	if (nvram_match("boardnum", "00") && nvram_match("boardrev", "0x13")
	    && nvram_match("boardtype", "0x467")) {
		if (nvram_match("boardflags", "0x1658")
		    || nvram_match("boardflags", "0x2658")
		    || nvram_match("boardflags", "0x3658")) {
			cprintf("router is Buffalo WLI-TX4-G54HP\n");
			setRouter("Buffalo WLI-TX4-G54HP");
			return ROUTER_BUFFALO_WLI_TX4_G54HP;
		}
		if (!nvram_match("buffalo_hp", "1")
		    && nvram_match("boardflags", "0x2758")) {
			cprintf("router is Buffalo WHR-G54S\n");
			setRouter("Buffalo WHR-G54S");
			return ROUTER_BUFFALO_WHRG54S;
		}
		if (nvram_match("buffalo_hp", "1")
		    || nvram_match("boardflags", "0x1758")) {
#ifndef HAVE_BUFFALO
			cprintf("router is Buffalo WHR-HP-G54\n");
			setRouter("Buffalo WHR-HP-G54");
#else
			cprintf("router is Buffalo WHR-HP-G54DD\n");
#ifdef BUFFALO_JP
			setRouter("Buffalo AS-A100");
#else
			setRouter("Buffalo WHR-HP-G54DD");
#endif
#endif
			return ROUTER_BUFFALO_WHRG54S;
		}
	}

	if (nvram_match("boardnum", "00") && nvram_match("boardrev", "0x10")
	    && nvram_match("boardtype", "0x470")) {
		cprintf("router is Buffalo WHR-AM54G54\n");
		setRouter("Buffalo WHR-AM54G54");
		return ROUTER_BUFFALO_WHRAM54G54;
	}

	if (boardnum == 42 && nvram_match("boardtype", "0x042f")) {

		if (nvram_match("product_name", "WZR-RS-G54")
		    || melco_id == 30083) {
			cprintf("router is Buffalo WZR-RS-G54\n");
			setRouter("Buffalo WZR-RS-G54");
			return ROUTER_BUFFALO_WZRRSG54;
		}
		if (nvram_match("product_name", "WZR-HP-G54")
		    || melco_id == 30026) {
			cprintf("router is Buffalo WZR-HP-G54\n");
			setRouter("Buffalo WZR-HP-G54");
			return ROUTER_BUFFALO_WZRRSG54;
		}
		if (nvram_match("product_name", "WZR-G54") || melco_id == 30061) {
			cprintf("router is Buffalo WZR-G54\n");
			setRouter("Buffalo WZR-G54");
			return ROUTER_BUFFALO_WZRRSG54;
		}
		if (nvram_match("melco_id", "290441dd")) {
			cprintf("router is Buffalo WHR2-A54G54\n");
			setRouter("Buffalo WHR2-A54G54");
			return ROUTER_BUFFALO_WZRRSG54;
		}
		if (nvram_match("product_name", "WHR3-AG54")
		    || nvram_match("product_name", "WHR3-B11")
		    || melco_id == 29130) {
			cprintf("router is Buffalo WHR3-AG54\n");
			setRouter("Buffalo WHR3-AG54");
			return ROUTER_BUFFALO_WZRRSG54;
		}
		if (nvram_match("product_name", "WVR-G54-NF")
		    || melco_id == 28100) {
			cprintf("router is Buffalo WVR-G54-NF\n");
			setRouter("Buffalo WVR-G54-NF");
			return ROUTER_BUFFALO_WZRRSG54;
		}
		if (nvram_match("product_name", "WZR-G108") || melco_id == 31095
		    || melco_id == 30153) {
			cprintf("router is Buffalo WZR-G108\n");
			setRouter("Buffalo WZR-G108");
			return ROUTER_BRCM4702_GENERIC;
		}
		if (melco_id > 0)	// e.g. 29115
		{
			cprintf("router is Buffalo WZR series\n");
			setRouter("Buffalo WZR series");
			return ROUTER_BUFFALO_WZRRSG54;
		}
	}
#ifndef HAVE_BUFFALO
	if (boardnum == 42 && nvram_match("boardtype", "0x042f")
	    && nvram_match("boardrev", "0x10"))
		// nvram_match ("boardflags","0x0018"))
	{
		cprintf("router is Linksys WRTSL54GS\n");
		setRouter("Linksys WRTSL54GS");
		return ROUTER_WRTSL54GS;
	}

	if (boardnum == 42 && nvram_match("boardtype", "0x0101")
	    && nvram_match("boardrev", "0x10")
	    && nvram_match("boot_ver", "v3.6")) {
		cprintf("router is Linksys WRT54G3G\n");
		setRouter("Linksys WRT54G3G");
		return ROUTER_WRT54G3G;
	}

	if (nvram_match("boardtype", "0x042f")
	    && nvram_match("boardrev", "0x10")) {
		char *hwver = nvram_safe_get("hardware_version");

		if (boardnum == 45 || startswith(hwver, "WL500gp")
		    || startswith(hwver, "WL500gH")) {
			cprintf("router is Asus WL-500g Premium\n");
			setRouter("Asus WL-500g Premium");
			return ROUTER_ASUS_WL500G_PRE;
		}
	}

	char *et0 = nvram_safe_get("et0macaddr");

	if (boardnum == 100 && nvram_match("boardtype", "bcm94710r4")) {
		if (startswith(et0, "00:11:50")) {
			cprintf("router is Belkin F5D7130 / F5D7330\n");
			setRouter("Belkin F5D7130 / F5D7330");
			return ROUTER_RT210W;
		}
		if (startswith(et0, "00:30:BD") || startswith(et0, "00:30:bd")) {
			cprintf("router is Belkin F5D7230 v1000\n");
			setRouter("Belkin F5D7230-4 v1000");
			return ROUTER_RT210W;
		}
		if (startswith(et0, "00:01:E3") ||
		    startswith(et0, "00:01:e3") || startswith(et0, "00:90:96"))
		{
			cprintf("router is Siemens\n");
			setRouter("Siemens SE505 v1");
			return ROUTER_RT210W;
		} else {
			cprintf("router is Askey generic\n");
			setRouter("RT210W generic");
			return ROUTER_RT210W;
		}
	}

	if (nvram_match("boardtype", "bcm94710r4")
	    && nvram_match("boardnum", "")) {
		cprintf("router is Askey board RT2100W\n");
		setRouter("Askey board RT2100W-D65)");
		return ROUTER_BRCM4702_GENERIC;
	}

	if (boardnum == 0 && nvram_match("boardtype", "0x0100")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Askey board RT2205(6)D-D56\n");
		if (startswith(et0, "00:11:50") ||
		    startswith(et0, "00:30:BD") || startswith(et0, "00:30:bd"))
		{
			setRouter("Askey board RT2205(6)D-D56");
		} else {
			setRouter("Belkin board F5D8230");
		}
		return ROUTER_ASKEY_RT220XD;
	}

	if (nvram_match("boardtype", "0x0101")) {
		if (startswith(et0, "00:11:50") ||
		    startswith(et0, "00:30:BD") || startswith(et0, "00:30:bd"))
		{
			if (nvram_match("Belkin_ver", "2000")) {
				cprintf("router is Belkin F5D7230-4 v2000\n");
				setRouter("Belkin F5D7230-4 v2000");
				return ROUTER_BELKIN_F5D7230_V2000;
			} else {
				cprintf("router is Belkin F5D7230-4 v1444\n");
				setRouter("Belkin F5D7230-4 v1444");
				return ROUTER_RT480W;
			}
		}
		if (startswith(et0, "00:01:E3") ||
		    startswith(et0, "00:01:e3") || startswith(et0, "00:90:96"))
		{
			cprintf("router is Siemens / Askey\n");
			setRouter("Siemens SE505 v2");
			return ROUTER_RT480W;
		}
	}
	if (boardnum == 1 && nvram_match("boardtype", "0x456")
	    && nvram_match("test_led_gpio", "2")) {
		cprintf("router is Belkin F5D7230-4 v3000\n");
		setRouter("Belkin F5D7230-4 v3000");
		return ROUTER_BELKIN_F5D7230_V3000;
	}

	if (nvram_match("boardtype", "0x456")
	    && nvram_match("hw_model", "F5D7231-4")) {
		cprintf("router is Belkin F5D7231-4 v1212UK\n");
		setRouter("Belkin F5D7231-4 v1212UK");
		return ROUTER_BELKIN_F5D7231;
	}

	if (boardnum == 8 && nvram_match("boardtype", "0x0467"))	// fccid:
		// K7SF5D7231B
	{
		cprintf("router is Belkin F5D7231-4 v2000\n");
		setRouter("Belkin F5D7231-4 v2000");
		return ROUTER_BELKIN_F5D7231_V2000;
	}

	if (nvram_match("boardtype", "0x467")) {
		if (startswith(et0, "00:11:50") ||
		    startswith(et0, "00:30:BD") || startswith(et0, "00:30:bd"))
		{
			cprintf("router is Belkin F5D7231-4 v2000\n");
			setRouter("Belkin F5D7231-4 v2000");
			return ROUTER_BELKIN_F5D7231;
		}
	}
#endif
	if (boardnum == 2 && nvram_match("boardtype", "bcm94710dev") && melco_id == 29016)	// Buffalo 
		// WLI2-TX1-G54)
	{
		cprintf("router is Buffalo WLI2-TX1-G54\n");
		setRouter("Buffalo WLI2-TX1-G54");
		return ROUTER_BUFFALO_WLI2_TX1_G54;
	}
#ifndef HAVE_BUFFALO

	char *gemtek = nvram_safe_get("GemtekPmonVer");
	unsigned long gemteknum = strtoul(gemtek, NULL, 0);

	if (boardnum == 2 && (gemteknum == 10 || gemteknum == 11) &&
	    (startswith(et0, "00:0C:E5") ||
	     startswith(et0, "00:0c:e5") ||
	     startswith(et0, "00:11:22") ||
	     startswith(et0, "00:0C:10") ||
	     startswith(et0, "00:0c:10") ||
	     startswith(et0, "00:0C:11") || startswith(et0, "00:0c:11"))) {
		cprintf("router Motorola WE800G v1\n");
		setRouter("Motorola WE800G v1");
		return ROUTER_MOTOROLA_WE800G;
	}

	if (boardnum == 2
	    && (startswith(gemtek, "RC") || gemteknum == 1 || gemteknum == 10))
	{
		cprintf("router is Linksys wap54g v1.x\n");
		setRouter("Linksys WAP54G v1.x");
		return ROUTER_WAP54G_V1;
	}

	if (boardnum == 2 && gemteknum == 1) {
		cprintf("router is Sitecom wl-105b\n");
		setRouter("Sitecom WL-105(b)");
		return ROUTER_SITECOM_WL105B;
	}

	if (boardnum == 2 && gemteknum == 7
	    && nvram_match("boardtype", "bcm94710dev")) {
		cprintf("router is Sitecom wl-111\n");
		setRouter("Sitecom WL-111");
		return ROUTER_SITECOM_WL111;
	}

	if (gemteknum == 9)	// Must be Motorola wr850g v1 or we800g v1 or 
		// Linksys wrt55ag v1
	{
		if (startswith(et0, "00:0C:E5") ||
		    startswith(et0, "00:0c:e5") ||
		    startswith(et0, "00:0C:10") ||
		    startswith(et0, "00:0c:10") ||
		    startswith(et0, "00:0C:11") ||
		    startswith(et0, "00:0c:11") ||
		    startswith(et0, "00:11:22") ||
		    startswith(et0, "00:0C:90") || startswith(et0, "00:0c:90"))
		{
			if (!strlen(nvram_safe_get("phyid_num"))) {
				insmod("switch-core");	// get phy type
				insmod("switch-robo");
				rmmod("switch-robo");
				rmmod("switch-core");
				nvram_set("boardnum", "2");
				nvram_set("boardtype", "bcm94710dev");
			}
			if (nvram_match("phyid_num", "0x00000000")) {
				cprintf("router Motorola WE800G v1\n");
				setRouter("Motorola WE800G v1");
				return ROUTER_MOTOROLA_WE800G;
			} else	// phyid_num == 0xffffffff
			{
				cprintf("router Motorola WR850G v1\n");
				setRouter("Motorola WR850G v1");
				return ROUTER_MOTOROLA_V1;
			}
		} else {
			cprintf("router is linksys WRT55AG\n");
			setRouter("Linksys WRT55AG v1");
			return ROUTER_LINKSYS_WRT55AG;
		}
	}
#endif
	if (boardnum == 0 && nvram_match("boardtype", "0x478")
	    && nvram_match("cardbus", "0") && nvram_match("boardrev", "0x10")
	    && nvram_match("boardflags", "0x110") && melco_id == 32027) {
		setRouter("Buffalo WZR-G144NH");
		return ROUTER_BUFFALO_WZRG144NH;
	}

	if (boardnum == 20060330 && nvram_match("boardtype", "0x0472")) {
		setRouter("Buffalo WZR-G300N");
		return ROUTER_BUFFALO_WZRG300N;
	}
#ifndef HAVE_BUFFALO

	if (boardnum == 8 && nvram_match("boardtype", "0x0472")
	    && nvram_match("cardbus", "1")) {
		cprintf("router is Netgear WNR834B\n");
		setRouter("Netgear WNR834B");
		return ROUTER_NETGEAR_WNR834B;
	}

	if (boardnum == 1 && nvram_match("boardtype", "0x0472")
	    && nvram_match("boardrev", "0x23")) {
		if (nvram_match("cardbus", "1")) {
			cprintf("router is Netgear WNR834B v2\n");
			setRouter("Netgear WNR834B v2");
			return ROUTER_NETGEAR_WNR834BV2;
		} else {
			cprintf("router is Netgear WNDR-3300\n");
			setRouter("Netgear WNDR3300");
			return ROUTER_NETGEAR_WNDR3300;
		}
	}

	if (boardnum == 42)	// Get Linksys N models
	{
		if (nvram_match("boot_hw_model", "WRT300N")
		    && nvram_match("boot_hw_ver", "1.1")) {
			setRouter("Linksys WRT300N v1.1");
			return ROUTER_WRT300NV11;
		} else if (nvram_match("boot_hw_model", "WRT150N")
			   && nvram_match("boot_hw_ver", "1")) {
			setRouter("Linksys WRT150N v1");
			return ROUTER_WRT150N;
		} else if (nvram_match("boot_hw_model", "WRT150N")
			   && nvram_match("boot_hw_ver", "1.1")) {
			setRouter("Linksys WRT150N v1.1");
			// return ROUTER_WRT150NV11;
			return ROUTER_WRT150N;
		} else if (nvram_match("boot_hw_model", "WRT150N")
			   && nvram_match("boot_hw_ver", "1.2")) {
			setRouter("Linksys WRT150N v1.2");
			// return ROUTER_WRT150NV12;
			return ROUTER_WRT150N;
		} else if (nvram_match("boot_hw_model", "WRT160N")
			   && nvram_match("boot_hw_ver", "1.0")) {
			setRouter("Linksys WRT160N");
			return ROUTER_WRT160N;
		} else if (nvram_match("boot_hw_model", "WRT160N")
			   && nvram_match("boot_hw_ver", "3.0")) {
			setRouter("Linksys WRT160Nv3");
			return ROUTER_WRT160NV3;
		} else if (nvram_match("boot_hw_model", "WRT310N")
			   && nvram_match("boot_hw_ver", "1.0")) {
			setRouter("Linksys WRT310N");
			return ROUTER_WRT310N;
		} else if (nvram_match("boot_hw_model", "WRT310N")
			   && nvram_match("boot_hw_ver", "2.0")) {
			setRouter("Linksys WRT310Nv2");
			return ROUTER_WRT310NV2;
		}
	}

	if (boardnum == 42 && nvram_match("boardtype", "0x0472")
	    && nvram_match("cardbus", "1")) {
		setRouter("Linksys WRT300N v1");
		return ROUTER_WRT300N;
	}

	if (boardnum == 42 &&
	    nvram_match("boardtype", "0x478") && nvram_match("cardbus", "1")) {
		cprintf("router is Linksys WRT350N\n");
		setRouter("Linksys WRT350N");
		return ROUTER_WRT350N;
	}

	if (nvram_match("boardnum", "20070615") &&
	    nvram_match("boardtype", "0x478") && nvram_match("cardbus", "0")
	    && nvram_match("switch_type", "BCM5395")) {
		cprintf("router is Linksys WRT600N v1.1\n");
		setRouter("Linksys WRT600N v1.1");
		return ROUTER_WRT600N;
	}

	if (nvram_match("boardnum", "20070615") &&
	    nvram_match("boardtype", "0x478") && nvram_match("cardbus", "0")) {
		cprintf("router is Linksys WRT600N\n");
		setRouter("Linksys WRT600N");
		return ROUTER_WRT600N;
	}

	if (nvram_match("boardtype", "0x478")
	    && nvram_match("boot_hw_model", "WRT610N")) {
		cprintf("router is Linksys WRT610N\n");
		setRouter("Linksys WRT610N");
		return ROUTER_WRT610N;
	}

	if (nvram_match("boardtype", "0x04cf")
	    && nvram_match("boot_hw_model", "WRT610N")) {
		cprintf("router is Linksys WRT610Nv2\n");
		setRouter("Linksys WRT610Nv2");
		return ROUTER_WRT610NV2;
	}

	if (boardnum == 42 && nvram_match("boardtype", "bcm94710dev")) {
		cprintf("router is Linksys WRT54G v1.x\n");
		setRouter("Linksys WRT54G v1.x");
		return ROUTER_WRT54G1X;
	}

	if ((boardnum == 1 || boardnum == 0)
	    && nvram_match("boardtype", "0x0446")) {
		cprintf("router is U.S. Robotics USR5430\n");
		setRouter("U.S.Robotics USR5430");
		return ROUTER_USR_5430;
	}

	if (boardnum == 1 && nvram_match("boardtype", "0x456")
	    && nvram_match("test_led_gpio", "0")) {
		cprintf("router is Netgear WG602 v3\n");
		setRouter("Netgear WG602 v3");
		return ROUTER_NETGEAR_WG602_V3;
	}

	if (boardnum == 10496 && nvram_match("boardtype", "0x456")) {
		cprintf("router is U.S. Robotics USR5461\n");
		setRouter("U.S.Robotics USR5461");
		return ROUTER_USR_5461;
	}

	if (boardnum == 10500 && nvram_match("boardtype", "0x456")) {
		cprintf("router is U.S. Robotics USR5432\n");
		setRouter("U.S.Robotics USR5432");
		return ROUTER_USR_5461;	// should work in the same way
	}

	if (boardnum == 10506 && nvram_match("boardtype", "0x456")) {
		cprintf("router is U.S. Robotics USR5451\n");
		setRouter("U.S.Robotics USR5451");
		return ROUTER_USR_5461;	// should work in the same way
	}

	if (boardnum == 10512 && nvram_match("boardtype", "0x456")) {
		cprintf("router is U.S. Robotics USR5441\n");
		setRouter("U.S.Robotics USR5441");
		return ROUTER_USR_5461;	// should work in the same way
	}

	if ((boardnum == 35324 || boardnum == 38256)
	    && nvram_match("boardtype", "0x048e")) {
		cprintf("router is U.S. Robotics USR5465\n");
		setRouter("U.S.Robotics USR5465");
		return ROUTER_USR_5465;
	}

	if (boardnum == 35334 && nvram_match("boardtype", "0x048e")) {
		cprintf("router is U.S. Robotics USR5455\n");
		setRouter("U.S.Robotics USR5455");
		return ROUTER_USR_5465;	// should work in the same way
	}

	if (boardnum == 1024 && nvram_match("boardtype", "0x0446")) {
		char *cfe = nvram_safe_get("cfe_version");

		if (strstr(cfe, "WRE54G")) {
			cprintf("router is Linksys WRE54Gv1\n");
			setRouter("Linksys WRE54G v1");
			return ROUTER_WAP54G_V2;
		} else if (strstr(cfe, "iewsonic")) {
			cprintf("router is Viewsonic WAPBR-100\n");
			setRouter("Viewsonic WAPBR-100");
			return ROUTER_VIEWSONIC_WAPBR_100;
		} else {
			cprintf("router is Linksys WAP54G v2\n");
			setRouter("Linksys WAP54G v2");
			return ROUTER_WAP54G_V2;
		}
	}

	if (nvram_invmatch("CFEver", "")) {
		char *cfe = nvram_safe_get("CFEver");

		if (!strncmp(cfe, "MotoWR", 6)) {
			cprintf("router is motorola\n");
			setRouter("Motorola WR850G v2/v3");
			return ROUTER_MOTOROLA;
		}
	}

	if (boardnum == 44 && (nvram_match("boardtype", "0x0101")
			       || nvram_match("boardtype", "0x0101\r"))) {
		char *cfe = nvram_safe_get("CFEver");

		if (!strncmp(cfe, "GW_WR110G", 9)) {
			cprintf("router is Sparklan WX-6615GT\n");
			setRouter("Sparklan WX-6615GT");
			return ROUTER_DELL_TRUEMOBILE_2300_V2;
		} else {
			cprintf("router is Dell TrueMobile 2300 v2\n");
			setRouter("Dell TrueMobile 2300 v2");
			return ROUTER_DELL_TRUEMOBILE_2300_V2;
		}
	}
#endif
	if (nvram_match("boardtype", "bcm94710ap")) {
		cprintf("router is Buffalo old 4710\n");
		setRouter("Buffalo WBR-B11");
		return ROUTER_BUFFALO_WBR54G;
	}
#ifndef HAVE_BUFFALO
	if (boardnum == 0 && nvram_match("boardtype", "0x048e") &&	// cfe sets boardnum="", strtoul -> 0
	    nvram_match("boardrev", "0x35")) {
		cprintf("router is D-Link DIR-320\n");
		setRouter("D-Link DIR-320");
		// apply some fixes
		if (nvram_get("vlan2ports") != NULL) {
			nvram_unset("vlan2ports");
			nvram_unset("vlan2hwname");
		}
		return ROUTER_DLINK_DIR320;
	}
	if (nvram_match("model_name", "DIR-330") &&
	    nvram_match("boardrev", "0x10")) {
		cprintf("router is D-Link DIR-330\n");
		setRouter("D-Link DIR-330");
		nvram_set("wan_ifnames", "eth0");	// quirk
		nvram_set("wan_ifname", "eth0");
		if (nvram_match("et0macaddr", "00:90:4c:4e:00:0c")) {
			FILE *in = fopen("/dev/mtdblock/1", "rb");

			fseek(in, 0x7a0022, SEEK_SET);
			char mac[32];

			fread(mac, 32, 1, in);
			fclose(in);
			mac[17] = 0;
			if (sv_valid_hwaddr(mac)) {
				nvram_set("et0macaddr", mac);
				fprintf(stderr, "restore D-Link MAC\n");
				nvram_commit();
				sys_reboot();
			}
		}
		/*
		 * if (nvram_get("vlan2ports")!=NULL) { nvram_unset("vlan2ports");
		 * nvram_unset("vlan2hwname"); }
		 */
		return ROUTER_DLINK_DIR330;
	}
	if (boardnum == 42 && nvram_match("boardtype", "0x048e")
	    && nvram_match("boardrev", "0x10")) {
		if (nvram_match("boardflags", "0x20750")) {
			cprintf("router is wrt54g2v1/v1.3/gs2v1\n");
			setRouter("Linksys WRT54G2 / GS2");
		} else {
			cprintf("router is wrt54gv8/gsv7\n");
			setRouter("Linksys WRT54Gv8 / GSv7");
		}
		return ROUTER_WRT54G_V8;
	}

	if (boardnum == 8 && nvram_match("boardtype", "0x048e")
	    && nvram_match("boardrev", "0x11")) {
		cprintf("router is ALLNET01\n");
		setRouter("ALLNET EUROWRT 54");
		return ROUTER_ALLNET01;
	}

	if (boardnum == 01 && nvram_match("boardtype", "0x048e")
	    && nvram_match("boardrev", "0x11")
	    && (nvram_match("boardflags", "0x650")
		|| nvram_match("boardflags", "0x0458"))) {
		cprintf("router is Netgear WG602 v4\n");
		setRouter("Netgear WG602 v4");
		return ROUTER_NETGEAR_WG602_V4;
	}

	if (boardnum == 1 && nvram_match("boardtype", "0x048e")
	    && nvram_match("boardrev", "0x35")
	    && nvram_match("parefldovoltage", "0x28")) {
		cprintf("NetCore NW618 / Rosewill RNX-GX4\n");
		setRouter("NetCore NW618 / Rosewill RNX-GX4");
		return ROUTER_NETCORE_NW618;
	}

	if (boardnum == 42 && nvram_match("boardtype", "0x048E")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Linksys WRH54G\n");
		setRouter("Linksys WRH54G");
		return ROUTER_LINKSYS_WRH54G;
	}

	if (nvram_match("boardnum", "00") && nvram_match("boardtype", "0x048E")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Linksys WRT54G v8.1\n");
		setRouter("Linksys WRT54G v8.1");
		return ROUTER_WRT54G_V81;
	}

	if (boardnum == 45 && nvram_match("boardtype", "0x456")) {
		cprintf("router is Asus WL-520G\n");
		setRouter("Asus WL-520G");
		return ROUTER_ASUS_WL520G;
	}

	if (nvram_match("boardtype", "0x48E")
	    && nvram_match("boardrev", "0x10")) {
		char *hwver = nvram_safe_get("hardware_version");

		if (boardnum == 45 && startswith(hwver, "WL500GPV2")) {
			cprintf("router is Asus WL-500G Premium V2\n");
			setRouter("Asus WL-500G Premium V2");
			return ROUTER_ASUS_WL500G_PRE_V2;
		} else if (boardnum == 45 && startswith(hwver, "WL330GE")) {
			cprintf("router is Asus WL-330GE\n");
			setRouter("Asus WL-330GE");
			return ROUTER_ASUS_330GE;
		} else if (boardnum == 45 || startswith(hwver, "WL500GU")
			   || startswith(hwver, "WL500GC")) {
			cprintf("router is Asus WL-520GU/GC\n");
			setRouter("Asus WL-520GU/GC");
			return ROUTER_ASUS_WL520GUGC;
		}
	}

	if ((boardnum == 83258 || boardnum == 1 || boardnum == 0123)	//or 01 or 001 or 0x01
	    && (nvram_match("boardtype", "0x048e") || nvram_match("boardtype", "0x48E")) && (nvram_match("boardrev", "0x11") || nvram_match("boardrev", "0x10")) && (nvram_match("boardflags", "0x750") || nvram_match("boardflags", "0x0750")) && nvram_match("sdram_init", "0x000A"))	//16 MB ram
	{
		cprintf("router is Netgear WGR614v8/L/WW\n");
		setRouter("Netgear WGR614v8/L/WW");
		return ROUTER_NETGEAR_WGR614L;
	}

	if (boardnum == 3805 && nvram_match("boardtype", "0x48E")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is Netgear WGR614v9\n");
		setRouter("Netgear WGR614v9");
		return ROUTER_NETGEAR_WGR614V9;
	}

	if (boardnum == 56 && nvram_match("boardtype", "0x456")
	    && nvram_match("boardrev", "0x10")) {
		cprintf("router is wtr54gs\n");
		setRouter("Linksys WTR54GS");
		return ROUTER_LINKSYS_WTR54GS;
	}

	if (nvram_match("boardnum", "WAP54GV3_8M_0614")
	    && (nvram_match("boardtype", "0x0467")
		|| nvram_match("boardtype", "0x467"))
	    && nvram_match("WAPver", "3")) {
		cprintf("router is WAP54G v3.x\n");
		setRouter("Linksys WAP54G v3.x");
		return ROUTER_WAP54G_V3;
	}

	if ((boardnum == 1 || boardnum == 3500)
	    && nvram_match("boardtype", "0x04CF")
	    && (nvram_match("boardrev", "0x1213")
		|| nvram_match("boardrev", "02"))) {
		cprintf("router is wnr3500v2/U/L\n");
		setRouter("Netgear WNR3500v2/U/L");
		return ROUTER_NETGEAR_WNR3500L;
	}

	if ((boardnum == 42 || boardnum == 66)
	    && nvram_match("boardtype", "0x04EF")
	    && (nvram_match("boardrev", "0x1304")
		|| nvram_match("boardrev", "0x1305"))) {
		cprintf("router is wrt320n\n");
		setRouter("Linksys WRT320N");
		return ROUTER_WRT320N;
	}

	if (boardnum == 94703 && nvram_match("boardtype", "0x04c0")
	    && nvram_match("boardrev", "0x1100")) {
		cprintf("router Dynex DX-NRUTER\n");
		setRouter("Dynex DX-NRUTER");
		return ROUTER_DYNEX_DX_NRUTER;
	}

	setRouter("Linksys WRT54G/GL/GS");
	cprintf("router is wrt54g\n");
	return ROUTER_WRT54G;
#else
	eval("event", "3", "1", "15");
	return 0;
#endif
#endif
#endif
}

static int router_type = -1;
int getRouterBrand()
{
	if (router_type == -1)
		router_type = internal_getRouterBrand();
	return router_type;
}

int get_ppp_pid(char *file)
{
	char buf[80];
	int pid = -1;

	if (file_to_buf(file, buf, sizeof(buf))) {
		char tmp[80], tmp1[80];

		snprintf(tmp, sizeof(tmp), "/var/run/%s.pid", buf);
		file_to_buf(tmp, tmp1, sizeof(tmp1));
		pid = atoi(tmp1);
	}
	return pid;
}

int check_wan_link(int num)
{
	int wan_link = 0;

	if ((nvram_match("wan_proto", "pptp")
	     || nvram_match("wan_proto", "l2tp")
	     || nvram_match("wan_proto", "pppoe")
	     || nvram_match("wan_proto", "3g")
	     || nvram_match("wan_proto", "heartbeat"))
	    && !nvram_match("3gdata", "hso")) {
		FILE *fp;
		char filename[80];
		char *name;

		if (num == 0)
			strcpy(filename, "/tmp/ppp/link");
		if ((fp = fopen(filename, "r"))) {
			int pid = -1;

			fclose(fp);
			if (nvram_match("wan_proto", "heartbeat")) {
				char buf[20];

				file_to_buf("/tmp/ppp/link", buf, sizeof(buf));
				pid = atoi(buf);
			} else
				pid = get_ppp_pid(filename);

			name = find_name_by_proc(pid);
			if (!strncmp(name, "pppoecd", 7) ||	// for PPPoE
			    !strncmp(name, "pppd", 4) ||	// for PPTP
			    !strncmp(name, "bpalogin", 8))	// for HeartBeat
				wan_link = 1;	// connect
			else {
				printf("The %s had been died, remove %s\n",
				       nvram_safe_get("wan_proto"), filename);
				wan_link = 0;	// For some reason, the pppoed had been died, 
				// by link file still exist.
				unlink(filename);
			}
		}
	} else {
		if (nvram_invmatch("wan_ipaddr", "0.0.0.0"))
			wan_link = 1;
	}

	return wan_link;
}


#ifdef HAVE_BUFFALO
void *getUEnv(char *name)
{
#ifdef HAVE_WZRG300NH
#define UOFFSET 0x40000
#else
#define UOFFSET 0x3E000
#endif
	static char res[64];
	memset(res, 0, sizeof(res));
	//fprintf(stderr,"[u-boot env]%s\n",name);
	FILE *fp = fopen("/dev/mtdblock/0", "rb");
	fseek(fp, UOFFSET, SEEK_SET);
	char *mem = malloc(0x2000);
	fread(mem, 0x2000, 1, fp);
	fclose(fp);
	int s = (0x2000 - 1) - strlen(name);
	int i;
	int l = strlen(name);
	for (i = 0; i < s; i++) {
		if (!strncmp(mem + i, name, l)) {
			strncpy(res, mem + i + l + 1,sizeof(res)-1);
			free(mem);
			return res;
		}
	}
	free(mem);
	return NULL;
}
#endif

char *get_wan_ipaddr(void)
{
	char *wan_ipaddr;
	int wan_link = check_wan_link(0);

	if (nvram_match("wan_proto", "pptp")) {
		wan_ipaddr =
		    wan_link ? nvram_safe_get("pptp_get_ip") :
		    nvram_safe_get("wan_ipaddr");
	} else if (!strcmp(nvram_safe_get("wan_proto"), "pppoe")) {
		wan_ipaddr =
		    wan_link ? nvram_safe_get("wan_ipaddr") : "0.0.0.0";
	} else if (!strcmp(nvram_safe_get("wan_proto"), "3g")) {
		wan_ipaddr =
		    wan_link ? nvram_safe_get("wan_ipaddr") : "0.0.0.0";
	} else if (nvram_match("wan_proto", "l2tp")) {
		wan_ipaddr =
		    wan_link ? nvram_safe_get("l2tp_get_ip") :
		    nvram_safe_get("wan_ipaddr");
	} else {
		wan_ipaddr = nvram_safe_get("wan_ipaddr");
	}
	return wan_ipaddr;
}

/*
 * Find process name by pid from /proc directory 
 */
char *find_name_by_proc(int pid)
{
	FILE *fp;
	char line[254];
	char filename[80];
	static char name[80];

	snprintf(filename, sizeof(filename), "/proc/%d/status", pid);

	if ((fp = fopen(filename, "r"))) {
		fgets(line, sizeof(line), fp);
		/*
		 * Buffer should contain a string like "Name: binary_name" 
		 */
		sscanf(line, "%*s %s", name);
		fclose(fp);
		return name;
	}

	return "";
}

int diag_led_4702(int type, int act)
{

#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_RB600) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42) || defined(HAVE_LS5) || defined(HAVE_FONERA) || defined(HAVE_LSX) || defined(HAVE_DANUBE) || defined(HAVE_STORM) || defined(HAVE_ADM5120) || defined(HAVE_RT2880) || defined(HAVE_OPENRISC)
	return 0;
#else
	if (act == START_LED) {
		switch (type) {
		case DMZ:
			system2("echo 1 > /proc/sys/diag");
			break;
		}
	} else {
		switch (type) {
		case DMZ:
			system2("echo 0 > /proc/sys/diag");
			break;
		}
	}
	return 0;
#endif
}

int C_led_4702(int i)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_RB600) || defined(HAVE_FONERA) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42) || defined(HAVE_LS5) || defined(HAVE_LSX) || defined(HAVE_DANUBE) || defined(HAVE_STORM) || defined(HAVE_ADM5120) || defined(HAVE_RT2880) || defined(HAVE_OPENRISC)
	return 0;
#else
	FILE *fp;
	char string[10];
	int flg;

	memset(string, 0, 10);
	/*
	 * get diag before set 
	 */
	if ((fp = fopen("/proc/sys/diag", "r"))) {
		fgets(string, sizeof(string), fp);
		fclose(fp);
	} else
		perror("/proc/sys/diag");

	if (i)
		flg = atoi(string) | 0x10;
	else
		flg = atoi(string) & 0xef;

	memset(string, 0, 10);
	sprintf(string, "%d", flg);
	if ((fp = fopen("/proc/sys/diag", "w"))) {
		fputs(string, fp);
		fclose(fp);
	} else
		perror("/proc/sys/diag");

	return 0;
#endif
}

unsigned int read_gpio(char *device)
{
	FILE *fp;
	unsigned int val;

	if ((fp = fopen(device, "r"))) {
		fread(&val, 4, 1, fp);
		fclose(fp);
		// fprintf(stderr, "----- gpio %s = [%X]\n",device,val); 
		return val;
	} else {
		perror(device);
		return 0;
	}
}

unsigned int write_gpio(char *device, unsigned int val)
{
	FILE *fp;

	if ((fp = fopen(device, "w"))) {
		fwrite(&val, 4, 1, fp);
		fclose(fp);
		// fprintf(stderr, "----- set gpio %s = [%X]\n",device,val); 
		return 1;
	} else {
		perror(device);
		return 0;
	}
}

static char hw_error = 0;
int diag_led_4704(int type, int act)
{
#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_RB600) || defined(HAVE_FONERA) || defined(HAVE_MERAKI)|| defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42) || defined(HAVE_LS5) || defined(HAVE_LSX) || defined(HAVE_DANUBE) || defined(HAVE_STORM) || defined(HAVE_ADM5120) || defined(HAVE_RT2880) || defined(HAVE_OPENRISC)
	return 0;
#else
	unsigned int control, in, outen, out;

#ifdef BCM94712AGR
	/*
	 * The router will crash, if we load the code into broadcom demo board. 
	 */
	return 1;
#endif
	// int brand;
	control = read_gpio("/dev/gpio/control");
	in = read_gpio("/dev/gpio/in");
	out = read_gpio("/dev/gpio/out");
	outen = read_gpio("/dev/gpio/outen");

	write_gpio("/dev/gpio/outen", (outen & 0x7c) | 0x83);
	switch (type) {
	case DIAG:		// GPIO 1
		if (hw_error) {
			write_gpio("/dev/gpio/out", (out & 0x7c) | 0x00);
			return 1;
		}

		if (act == STOP_LED) {	// stop blinking
			write_gpio("/dev/gpio/out", (out & 0x7c) | 0x83);
			// cprintf("tallest:=====( DIAG STOP_LED !!)=====\n");
		} else if (act == START_LED) {	// start blinking
			write_gpio("/dev/gpio/out", (out & 0x7c) | 0x81);
			// cprintf("tallest:=====( DIAG START_LED !!)=====\n");
		} else if (act == MALFUNCTION_LED) {	// start blinking
			write_gpio("/dev/gpio/out", (out & 0x7c) | 0x00);
			hw_error = 1;
			// cprintf("tallest:=====( DIAG MALFUNCTION_LED !!)=====\n");
		}
		break;

	}
	return 1;
#endif
}

int diag_led_4712(int type, int act)
{
	unsigned int control, in, outen, out, ctr_mask, out_mask;

#if defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_RB600) || defined(HAVE_FONERA)|| defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_WHRAG108) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_TW6600) || defined(HAVE_PB42) || defined(HAVE_LS5) || defined(HAVE_LSX) || defined(HAVE_DANUBE) || defined(HAVE_STORM) || defined(HAVE_ADM5120) || defined(HAVE_RT2880) || defined(HAVE_OPENRISC)
	return 0;
#else

#ifdef BCM94712AGR
	/*
	 * The router will crash, if we load the code into broadcom demo board. 
	 */
	return 1;
#endif
	control = read_gpio("/dev/gpio/control");
	in = read_gpio("/dev/gpio/in");
	out = read_gpio("/dev/gpio/out");
	outen = read_gpio("/dev/gpio/outen");

	ctr_mask = ~(1 << type);
	out_mask = (1 << type);

	write_gpio("/dev/gpio/control", control & ctr_mask);
	write_gpio("/dev/gpio/outen", outen | out_mask);

	if (act == STOP_LED) {	// stop blinking
		// cprintf("%s: Stop GPIO %d\n", __FUNCTION__, type);
		write_gpio("/dev/gpio/out", out | out_mask);
	} else if (act == START_LED) {	// start blinking
		// cprintf("%s: Start GPIO %d\n", __FUNCTION__, type);
		write_gpio("/dev/gpio/out", out & ctr_mask);
	}

	return 1;
#endif
}

int C_led_4712(int i)
{
	if (i == 1)
		return diag_led(DIAG, START_LED);
	else
		return diag_led(DIAG, STOP_LED);
}

int C_led(int i)
{
	int brand = getRouterBrand();

	if (brand == ROUTER_WRT54G1X || brand == ROUTER_LINKSYS_WRT55AG)
		return C_led_4702(i);
	else if (brand == ROUTER_WRT54G)
		return C_led_4712(i);
	else
		return 0;
}

int diag_led(int type, int act)
{
	int brand = getRouterBrand();

	if (brand == ROUTER_WRT54G || brand == ROUTER_WRT54G3G
	    || brand == ROUTER_WRT300NV11)
		return diag_led_4712(type, act);
	else if (brand == ROUTER_WRT54G1X || brand == ROUTER_LINKSYS_WRT55AG)
		return diag_led_4702(type, act);
	else if ((brand == ROUTER_WRTSL54GS
		  || brand == ROUTER_WRT310N || brand == ROUTER_WRT350N
		  || brand == ROUTER_BUFFALO_WZRG144NH) && type == DIAG)
		return diag_led_4704(type, act);
	else {
		if (type == DMZ) {
			if (act == START_LED)
				return led_control(LED_DMZ, LED_ON);
			if (act == STOP_LED)
				return led_control(LED_DMZ, LED_OFF);
			return 1;
		}
	}
	return 0;
}

#ifdef HAVE_MADWIFI
static char *stalist[] = {
	"ath0", "ath1", "ath2", "ath3", "ath4", "ath5", "ath6", "ath8", "ath9"
};

char *getWifi(char *ifname)
{
	if (!strncmp(ifname, "ath0", 4))
		return "wifi0";
	if (!strncmp(ifname, "ath1", 4))
		return "wifi1";
	if (!strncmp(ifname, "ath2", 4))
		return "wifi2";
	if (!strncmp(ifname, "ath3", 4))
		return "wifi3";
	return NULL;
}

char *getWDSSTA(void)
{

	int c = getifcount("wifi");
	int i;

	for (i = 0; i < c; i++) {
		char mode[32];
		char netmode[32];

		sprintf(mode, "ath%d_mode", i);
		sprintf(netmode, "ath%d_net_mode", i);
		if (nvram_match(mode, "wdssta")
		    && !nvram_match(netmode, "disabled")) {
			return stalist[i];
		}

	}
	return NULL;
}

char *getSTA(void)
{

#ifdef HAVE_WAVESAT
	if (nvram_match("ofdm_mode", "sta"))
		return "ofdm";
#endif
	int c = getifcount("wifi");
	int i;

	for (i = 0; i < c; i++) {
		char mode[32];
		char netmode[32];

		sprintf(mode, "ath%d_mode", i);
		sprintf(netmode, "ath%d_net_mode", i);
		if (nvram_match(mode, "sta")
		    && !nvram_match(netmode, "disabled")) {
			return stalist[i];
		}

	}
	return NULL;
}

char *getWET(void)
{
#ifdef HAVE_WAVESAT
	if (nvram_match("ofdm_mode", "bridge"))
		return "ofdm";
#endif
	int c = getifcount("wifi");
	int i;

	for (i = 0; i < c; i++) {
		char mode[32];
		char netmode[32];

		sprintf(mode, "ath%d_mode", i);
		sprintf(netmode, "ath%d_net_mode", i);
		if (nvram_match(mode, "wet")
		    && !nvram_match(netmode, "disabled")) {
			return stalist[i];
		}

	}
	return NULL;
}

#elif HAVE_RT2880

char *getSTA()
{
	int c = get_wl_instances();
	int i;

	for (i = 0; i < c; i++) {
		if (nvram_nmatch("sta", "wl%d_mode", i)) {
			if (!nvram_nmatch("disabled", "wl%d_net_mode", i))
				return "ra0";
		}

		if (nvram_nmatch("apsta", "wl%d_mode", i)) {
			if (!nvram_nmatch("disabled", "wl%d_net_mode", i))
				return "apcli0";
		}

	}
	return NULL;
}

char *getWET()
{
	int c = get_wl_instances();
	int i;

	for (i = 0; i < c; i++) {
		if (!nvram_nmatch("disabled", "wl%d_net_mode", i)
		    && nvram_nmatch("wet", "wl%d_mode", i))
			return "ra0";

		if (!nvram_nmatch("disabled", "wl%d_net_mode", i)
		    && nvram_nmatch("apstawet", "wl%d_mode", i))
			return "apcli0";

	}
	return NULL;
}

#else
char *getSTA()
{
	int c = get_wl_instances();
	int i;

	for (i = 0; i < c; i++) {
		if (nvram_nmatch("sta", "wl%d_mode", i)
		    || nvram_nmatch("apsta", "wl%d_mode", i)) {
			if (!nvram_nmatch("disabled", "wl%d_net_mode", i))
				return get_wl_instance_name(i);
			// else
			// return nvram_nget ("wl%d_ifname", i);
		}

	}
	return NULL;
}

char *getWET()
{
	int c = get_wl_instances();
	int i;

	for (i = 0; i < c; i++) {
		if (nvram_nmatch("wet", "wl%d_mode", i)
		    || nvram_nmatch("apstawet", "wl%d_mode", i)) {
			if (!nvram_nmatch("disabled", "wl%d_net_mode", i))
				return get_wl_instance_name(i);
			// else
			// return nvram_nget ("wl%d_ifname", i);

		}

	}
	return NULL;
}

#endif
// note - broadcast addr returned in ipaddr
void get_broadcast(char *ipaddr, char *netmask)
{
	int ip2[4], mask2[4];
	unsigned char ip[4], mask[4];

	if (!ipaddr || !netmask)
		return;

	sscanf(ipaddr, "%d.%d.%d.%d", &ip2[0], &ip2[1], &ip2[2], &ip2[3]);
	sscanf(netmask, "%d.%d.%d.%d", &mask2[0], &mask2[1], &mask2[2],
	       &mask2[3]);
	int i = 0;

	for (i = 0; i < 4; i++) {
		ip[i] = ip2[i];
		mask[i] = mask2[i];
		// ip[i] = (ip[i] & mask[i]) | !mask[i];
		ip[i] = (ip[i] & mask[i]) | (0xff & ~mask[i]);
	}

	sprintf(ipaddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
#ifdef WDS_DEBUG
	fprintf(fp, "get_broadcast return %s\n", value);
#endif

}

char *get_wan_face(void)
{
	static char localwanface[IFNAMSIZ];
	if (nvram_match("wan_proto", "disabled"))
		return "br0";

	/*
	 * if (nvram_match ("pptpd_client_enable", "1")) { strncpy (localwanface, 
	 * "ppp0", IFNAMSIZ); return localwanface; }
	 */
	if (nvram_match("wan_proto", "pptp")
	    || nvram_match("wan_proto", "l2tp")
	    || nvram_match("wan_proto", "3g")
	    || nvram_match("wan_proto", "pppoe")) {
		if (nvram_match("pppd_pppifname", ""))
			strncpy(localwanface, "ppp0", IFNAMSIZ);
		else
			strncpy(localwanface, nvram_safe_get("pppd_pppifname"),
				IFNAMSIZ);
	}
#ifndef HAVE_MADWIFI
	else if (getSTA()) {
		strcpy(localwanface, getSTA());
	}
#else
	else if (getSTA()) {
		if (nvram_match("wifi_bonding", "1"))
			strcpy(localwanface, "bond0");
		else
			strcpy(localwanface, getSTA());
	}
#endif
	else
		strncpy(localwanface, nvram_safe_get("wan_ifname"), IFNAMSIZ);

	return localwanface;
}

static int _pidof(const char *name, pid_t ** pids)
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
	if ((p = strchr(name, '/')) != NULL)
		name = p + 1;
	if ((dir = opendir("/proc")) != NULL) {
		while ((de = readdir(dir)) != NULL) {
			i = strtol(de->d_name, &e, 10);
			if (*e != 0)
				continue;
			if (strcmp(name, psname(i, buf, sizeof(buf))) == 0) {
				if ((*pids =
				     realloc(*pids,
					     sizeof(pid_t) * (count + 1))) ==
				    NULL) {
					return -1;
				}
				(*pids)[count++] = i;
			}
		}
	}
	closedir(dir);
	return count;
}

int pidof(const char *name)
{
	pid_t *pids;
	pid_t p;

	if (_pidof(name, &pids) > 0) {
		p = *pids;
		free(pids);
		return p;
	}
	return -1;
}

int killall(const char *name, int sig)
{
	pid_t *pids;
	int i;
	int r;

	if ((i = _pidof(name, &pids)) > 0) {
		r = 0;
		do {
			r |= kill(pids[--i], sig);
		}
		while (i > 0);
		free(pids);
		return r;
	}
	return -2;
}

void set_ip_forward(char c)
{
	FILE *fp;

	if ((fp = fopen("/proc/sys/net/ipv4/ip_forward", "r+"))) {
		fputc(c, fp);
		fclose(fp);
	} else {
		perror("/proc/sys/net/ipv4/ip_forward");
	}
}

int ifexists(const char *ifname)
{
	return getifcount(ifname) > 0 ? 1 : 0;
}

int getifcount(const char *ifprefix)
{
	/*
	 * char devcall[128];
	 * 
	 * sprintf (devcall, "cat /proc/net/dev|grep \"%s\"|wc -l", ifprefix);
	 * FILE *in = popen (devcall, "rb"); if (in == NULL) return 0; int count;
	 * fscanf (in, "%d", &count); pclose (in); return count;
	 */
	char *iflist = malloc(256);

	memset(iflist, 0, 256);
	int c = getIfList(iflist, ifprefix);

	free(iflist);
	return c;
}

static void skipline(FILE * in)
{
	while (1) {
		int c = getc(in);

		if (c == EOF)
			return;
		if (c == 0x0)
			return;
		if (c == 0xa)
			return;
	}
}

/*
 * strips trailing char(s) c from string
 */
void strtrim_right(char *p, int c)
{
	char *end;
	int len;

	len = strlen(p);
	while (*p && len) {
		end = p + len - 1;
		if (c == *end)
			*end = 0;
		else
			break;
		len = strlen(p);
	}
	return;
}

// returns a physical interfacelist filtered by ifprefix. if ifprefix is
// NULL, all valid interfaces will be returned
int getIfList(char *buffer, const char *ifprefix)
{
	FILE *in = fopen("/proc/net/dev", "rb");
	char ifname[32];

	// skip the first 2 lines
	skipline(in);
	skipline(in);
	int ifcount = 0;
	int count = 0;

	while (1) {
		int c = getc(in);

		if (c == EOF) {
			if (count)
				buffer[strlen(buffer) - 1] = 0;	// fixup last space
			fclose(in);
			return count;
		}
		if (c == 0) {
			if (count)
				buffer[strlen(buffer) - 1] = 0;	// fixup last space
			fclose(in);
			return count;
		}
		if (c == 0x20)
			continue;
		if (c == ':' || ifcount == 30) {
			ifname[ifcount++] = 0;
			int skip = 0;

			if (ifprefix) {
				if (strncmp(ifname, ifprefix, strlen(ifprefix))) {
					skip = 1;
				}
			} else {
				if (!strncmp(ifname, "wifi", 4))
					skip = 1;
				if (!strncmp(ifname, "ifb", 3))
					skip = 1;
				if (!strncmp(ifname, "imq", 3))
					skip = 1;
				if (!strncmp(ifname, "etherip", 7))
					skip = 1;
				if (!strncmp(ifname, "lo", 2))
					skip = 1;
				if (!strncmp(ifname, "teql", 4))
					skip = 1;
				if (!strncmp(ifname, "gre", 3))
					skip = 1;
				if (!strncmp(ifname, "ppp", 3))
					skip = 1;
				if (!strncmp(ifname, "tun", 3))
					skip = 1;
				if (!strncmp(ifname, "tap", 3))
					skip = 1;
			}
			if (!skip) {
				strcat(buffer, ifname);
				strcat(buffer, " ");
				count++;
			}
			skip = 0;
			ifcount = 0;
			memset(ifname, 0, 32);
			skipline(in);
			continue;
		}
		if (ifcount < 30)
			ifname[ifcount++] = c;
	}
}

/*
 * Example: legal_hwaddr("00:11:22:33:44:aB"); return true;
 * legal_hwaddr("00:11:22:33:44:5"); return false;
 * legal_hwaddr("00:11:22:33:44:HH"); return false; 
 */
int sv_valid_hwaddr(char *value)
{
	unsigned int hwaddr[6];
	int tag = TRUE;
	int i, count;

	/*
	 * Check for bad, multicast, broadcast, or null address 
	 */
	for (i = 0, count = 0; *(value + i); i++) {
		if (*(value + i) == ':') {
			if ((i + 1) % 3 != 0) {
				tag = FALSE;
				break;
			}
			count++;
		} else if (ishexit(*(value + i)))	/* one of 0 1 2 3 4 5 6 7 8 9 
							 * a b c d e f A B C D E F */
			continue;
		else {
			tag = FALSE;
			break;
		}
	}

	if (!tag || i != 17 || count != 5)	/* must have 17's characters and 5's
						 * ':' */
		tag = FALSE;
	else if (sscanf(value, "%x:%x:%x:%x:%x:%x",
			&hwaddr[0], &hwaddr[1], &hwaddr[2],
			&hwaddr[3], &hwaddr[4], &hwaddr[5]) != 6) {
		tag = FALSE;
	} else
		tag = TRUE;
#ifdef WDS_DEBUG
	if (tag == FALSE)
		fprintf(fp, "failed valid_hwaddr\n");
#endif

	return tag;
}

int led_control(int type, int act)
/*
 * type: LED_POWER, LED_DIAG, LED_DMZ, LED_CONNECTED, LED_BRIDGE, LED_VPN,
 * LED_SES, LED_SES2, LED_WLAN act: LED_ON, LED_OFF, LED_FLASH 
 */
{
#if (defined(HAVE_GEMTEK) || defined(HAVE_RB500) || defined(HAVE_MAGICBOX)  || defined(HAVE_RB600) || defined(HAVE_MERAKI) || defined(HAVE_LS2) || defined(HAVE_X86) || defined(HAVE_CA8) || defined(HAVE_LS5))  && (!defined(HAVE_DIR300) && !defined(HAVE_WRT54G2) && !defined(HAVE_RTG32) && !defined(HAVE_DIR400) && !defined(HAVE_BWRG1000))
	return 0;
#else
	int use_gpio = 0x0ff;
	int gpio_value;
	int enable;
	int disable;

	int power_gpio = 0x0ff;
	int diag_gpio = 0x0ff;
	int dmz_gpio = 0x0ff;
	int connected_gpio = 0x0ff;
	int bridge_gpio = 0x0ff;
	int vpn_gpio = 0x0ff;
	int ses_gpio = 0x0ff;	// use for SES1 (Linksys), AOSS (Buffalo)
	int ses2_gpio = 0x0ff;
	int wlan_gpio = 0x0ff;	// use this only if wlan led is not controlled by hardware!
	int usb_gpio = 0x0ff;
	int sec0_gpio = 0x0ff;	// security leds, wrt600n
	int sec1_gpio = 0x0ff;
	int v1func = 0;
	int connblue = nvram_match("connblue", "1") ? 1 : 0;

	switch (getRouterBrand())	// gpio definitions here: 0xYZ,
		// Y=0:normal, Y=1:inverted, Z:gpio
		// number (f=disabled)
	{
#ifndef HAVE_BUFFALO
	case ROUTER_BOARD_PB42:
#ifdef HAVE_WR1043
		diag_gpio = 0x102;
		ses_gpio = 0x005;
//              usb_gpio = 0x101;
#endif
#ifdef HAVE_WRT160NL
		power_gpio = 0x10e;
		connected_gpio = 0x109;
		ses_gpio = 0x108;
#endif
#ifdef HAVE_TG2521
		ses_gpio = 0x103;
		diag_gpio = 0x103;
#endif
#ifdef HAVE_TEW632BRP
		diag_gpio = 0x101;
		ses_gpio = 0x103;
#endif
#ifdef HAVE_WP543
		diag_gpio = 0x107;
		connected_gpio = 0x106;
#endif
#ifdef HAVE_DIR825
		power_gpio = 0x102;
		diag_gpio = 0x101;
		connected_gpio = 0x10b;
		ses_gpio = 0x104;
#endif
#ifdef HAVE_WNDR3700
		power_gpio = 0x102;
		diag_gpio = 0x101;
		connected_gpio = 0x106;
		ses_gpio = 0x104;
#endif
#ifdef HAVE_WZRG300NH
		diag_gpio = 0x101;
		connected_gpio = 0x112;
		ses_gpio = 0x111;
#endif
#ifdef HAVE_DIR615E
		power_gpio = 0x006;
		diag_gpio = 0x001;
		connected_gpio = 0x007;
		ses_gpio = 0x100;
#endif
#ifdef HAVE_WRT400
		power_gpio = 0x001;
		diag_gpio = 0x105;
		ses_gpio = 0x104;
		connected_gpio = 0x007;
#endif
		break;
	case ROUTER_ALLNET01:
		connected_gpio = 0x100;
		break;
	case ROUTER_BOARD_WP54G:
		diag_gpio = 0x102;
		connected_gpio = 0x107;
		break;
	case ROUTER_BOARD_NP28G:
		diag_gpio = 0x102;
		connected_gpio = 0x106;
		break;
	case ROUTER_BOARD_GATEWORX_GW2369:
		connected_gpio = 0x102;
		break;
	case ROUTER_BOARD_GATEWORX:
#ifdef HAVE_WG302V1
		diag_gpio = 0x104;
		wlan_gpio = 0x105;
#elif HAVE_WG302
		diag_gpio = 0x102;
		wlan_gpio = 0x104;
#else
		if (nvram_match("DD_BOARD", "Cambria GW2350")
		    || nvram_match("DD_BOARD2", "Cambria GW2350"))
			connected_gpio = 0x105;
		else if (nvram_match("DD_BOARD", "Cambria GW2358-4")
			 || nvram_match("DD_BOARD2", "Cambria GW2358-4"))
			connected_gpio = 0x118;
		else
			connected_gpio = 0x003;
#endif
		break;
	case ROUTER_BOARD_GATEWORX_SWAP:
		connected_gpio = 0x004;
		break;
	case ROUTER_BOARD_STORM:
		connected_gpio = 0x005;
		diag_gpio = 0x003;
		break;
	case ROUTER_LINKSYS_WRH54G:
		diag_gpio = 0x101;	// power led blink / off to indicate factory
		// defaults
		break;
	case ROUTER_WRT54G:
	case ROUTER_WRT54G_V8:
		power_gpio = 0x001;
		dmz_gpio = 0x107;
		connected_gpio = 0x103;	// ses orange
		ses_gpio = 0x102;	// ses white
		ses2_gpio = 0x103;	// ses orange
		break;
	case ROUTER_WRT54G_V81:
		power_gpio = 0x101;
		dmz_gpio = 0x102;
		connected_gpio = 0x104;	// ses orange
		ses_gpio = 0x103;	// ses white
		ses2_gpio = 0x104;	// ses orange
		break;
	case ROUTER_WRT54G1X:
		connected_gpio = 0x103;
		v1func = 1;
		break;
	case ROUTER_WRT350N:
		connected_gpio = 0x103;
		power_gpio = 0x001;
		ses2_gpio = 0x103;	// ses orange
		sec0_gpio = 0x109;
		usb_gpio = 0x10b;
		break;
	case ROUTER_WRT600N:
		power_gpio = 0x102;
		diag_gpio = 0x002;
		usb_gpio = 0x103;
		sec0_gpio = 0x109;
		sec1_gpio = 0x10b;
		break;
	case ROUTER_LINKSYS_WRT55AG:
		connected_gpio = 0x103;
		break;
	case ROUTER_DLINK_DIR330:
		diag_gpio = 0x106;
		connected_gpio = 0x100;
		usb_gpio = 0x104;
		break;
	case ROUTER_BOARD_DIR600B:
		diag_gpio = 0x10d;
		connected_gpio = 0x108;
		power_gpio = 0x109;
		break;
	case ROUTER_BOARD_DIR615D:
		diag_gpio = 0x108;
		connected_gpio = 0x10d;
		ses_gpio = 0x10b;
		power_gpio = 0x109;
		break;
	case ROUTER_BOARD_W502U:
		connected_gpio = 0x10d;
		break;
	case ROUTER_BOARD_OPENRISC:
		diag_gpio = 0x003;
		break;
#else
	case ROUTER_BOARD_PB42:
#ifdef HAVE_WZRG300NH
		diag_gpio = 0x101;
		connected_gpio = 0x112;
		usb_gpio = 0x100;
		ses_gpio = 0x111;
#endif
		break;
#endif
	case ROUTER_BOARD_WHRG300N:
		diag_gpio = 0x107;
		connected_gpio = 0x109;
		ses_gpio = 0x10e;
		break;
	case ROUTER_BOARD_WHRHPGN:
		diag_gpio = 0x101;
		connected_gpio = 0x106;
		ses_gpio = 0x100;
		break;
	case ROUTER_BUFFALO_WBR54G:
		diag_gpio = 0x107;
		break;
	case ROUTER_BUFFALO_WBR2G54S:
		diag_gpio = 0x001;
		ses_gpio = 0x006;
		break;
	case ROUTER_BUFFALO_WLA2G54C:
		diag_gpio = 0x104;
		ses_gpio = 0x103;
		break;
	case ROUTER_BUFFALO_WLAH_G54:
		diag_gpio = 0x107;
		ses_gpio = 0x106;
		break;
	case ROUTER_BUFFALO_WAPM_HP_AM54G54:
		diag_gpio = 0x107;
		ses_gpio = 0x101;
		break;
	case ROUTER_BOARD_WHRAG108:
		diag_gpio = 0x107;
		bridge_gpio = 0x104;
		ses_gpio = 0x100;
		break;
	case ROUTER_BUFFALO_WHRG54S:
	case ROUTER_BUFFALO_WLI_TX4_G54HP:
		diag_gpio = 0x107;
		if (nvram_match("DD_BOARD", "Buffalo WHR-G125")) {
			connected_gpio = 0x101;
			sec0_gpio = 0x106;
		} else {
			bridge_gpio = 0x101;
			ses_gpio = 0x106;
		}
		break;
	case ROUTER_BUFFALO_WZRRSG54:
		diag_gpio = 0x107;
		vpn_gpio = 0x101;
		ses_gpio = 0x106;
		break;
	case ROUTER_BUFFALO_WZRG300N:
		diag_gpio = 0x107;
		bridge_gpio = 0x101;
		break;
	case ROUTER_BUFFALO_WZRG144NH:
		diag_gpio = 0x103;
		bridge_gpio = 0x101;
		ses_gpio = 0x102;
		break;
#ifndef HAVE_BUFFALO
#ifdef HAVE_DIR300
	case ROUTER_BOARD_FONERA:
		diag_gpio = 0x003;
		bridge_gpio = 0x004;
		ses_gpio = 0x001;
		break;
#endif
#ifdef HAVE_WRT54G2
	case ROUTER_BOARD_FONERA:
		bridge_gpio = 0x004;
		ses_gpio = 0x104;
		diag_gpio = 0x103;
		break;
#endif
#ifdef HAVE_RTG32
	case ROUTER_BOARD_FONERA:
		break;
#endif
#ifdef HAVE_BWRG1000
	case ROUTER_BOARD_LS2:
		diag_gpio = 0x007;
		break;
#endif
#ifdef HAVE_DIR400
	case ROUTER_BOARD_FONERA2200:
		diag_gpio = 0x003;
		bridge_gpio = 0x004;
		ses_gpio = 0x001;
		break;
#endif
#ifdef HAVE_WRK54G
	case ROUTER_BOARD_FONERA:
		diag_gpio = 0x107;
		dmz_gpio = 0x005;
		break;
#endif
	case ROUTER_BOARD_TW6600:
		diag_gpio = 0x107;
		bridge_gpio = 0x104;
		ses_gpio = 0x100;
		break;
	case ROUTER_MOTOROLA:
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate factory
		// defaults
		break;
	case ROUTER_RT210W:
		power_gpio = 0x105;
		diag_gpio = 0x005;	// power led blink / off to indicate factory
		// defaults
		connected_gpio = 0x100;
		wlan_gpio = 0x103;
		break;
	case ROUTER_RT480W:
	case ROUTER_BELKIN_F5D7230_V2000:
	case ROUTER_BELKIN_F5D7231:
		power_gpio = 0x105;
		diag_gpio = 0x005;	// power led blink / off to indicate factory
		// defaults
		connected_gpio = 0x100;
		break;
	case ROUTER_MICROSOFT_MN700:
		power_gpio = 0x006;
		diag_gpio = 0x106;	// power led blink / off to indicate factory
		// defaults
		break;
	case ROUTER_ASUS_WL500GD:
	case ROUTER_ASUS_WL520GUGC:
		diag_gpio = 0x000;	// power led blink / off to indicate factory
		// defaults
		break;
	case ROUTER_ASUS_WL500G_PRE:
		power_gpio = 0x101;
		diag_gpio = 0x001;	// power led blink / off to indicate factory
		// defaults
		break;
	case ROUTER_ASUS_WL550GE:
		power_gpio = 0x102;
		diag_gpio = 0x002;	// power led blink / off to indicate factory
		// defaults
		break;
	case ROUTER_WRT54G3G:
	case ROUTER_WRTSL54GS:
		power_gpio = 0x001;
		dmz_gpio = 0x100;
		connected_gpio = 0x107;	// ses orange
		ses_gpio = 0x105;	// ses white
		ses2_gpio = 0x107;	// ses orange 
		break;
	case ROUTER_MOTOROLA_WE800G:
	case ROUTER_MOTOROLA_V1:
		diag_gpio = 0x103;
		wlan_gpio = 0x101;
		bridge_gpio = 0x105;
		break;
	case ROUTER_DELL_TRUEMOBILE_2300:
	case ROUTER_DELL_TRUEMOBILE_2300_V2:
		power_gpio = 0x107;
		diag_gpio = 0x007;	// power led blink / off to indicate factory
		// defaults
		wlan_gpio = 0x106;
		break;
	case ROUTER_NETGEAR_WNR834B:
		power_gpio = 0x104;
		diag_gpio = 0x105;
		wlan_gpio = 0x106;
		break;
	case ROUTER_SITECOM_WL105B:
		power_gpio = 0x003;
		diag_gpio = 0x103;	// power led blink / off to indicate factory
		// defaults
		wlan_gpio = 0x104;
		break;
	case ROUTER_WRT300N:
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate fac.def.
		break;
	case ROUTER_WRT150N:
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate fac.def.
		sec0_gpio = 0x105;
		break;
	case ROUTER_WRT300NV11:
		ses_gpio = 0x105;
		// diag_gpio = 0x11; //power led blink / off to indicate fac.def.
		break;
	case ROUTER_WRT310N:
		connected_gpio = 0x103;	//ses orange
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate fac.def.
		ses_gpio = 0x109;	// ses blue
		break;
	case ROUTER_WRT310NV2:
		connected_gpio = 0x102;	// ses orange
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate fac.def.
		ses_gpio = 0x104;	// ses blue
		break;
	case ROUTER_WRT160N:
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate fac.def. 
		connected_gpio = 0x103;	// ses orange
		ses_gpio = 0x105;	// ses blue
		break;
	case ROUTER_WRT160NV3:
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power led blink / off to indicate fac.def. 
		connected_gpio = 0x102;	// ses orange
		ses_gpio = 0x104;	// ses blue
		break;
	case ROUTER_ASUS_WL500G:
		power_gpio = 0x100;
		diag_gpio = 0x000;	// power led blink /off to indicate factory
		// defaults
		break;
	case ROUTER_ASUS_WL500W:
		power_gpio = 0x105;
		diag_gpio = 0x005;	// power led blink /off to indicate factory
		// defaults
		break;
	case ROUTER_LINKSYS_WTR54GS:
		diag_gpio = 0x001;
		break;
	case ROUTER_WAP54G_V1:
		diag_gpio = 0x103;
		wlan_gpio = 0x104;	// LINK led
		break;
	case ROUTER_WAP54G_V3:
		ses_gpio = 0x10c;
		connected_gpio = 0x006;
		break;
	case ROUTER_NETGEAR_WNR834BV2:
		power_gpio = 0x002;
		diag_gpio = 0x003;	// power led amber 
		connected_gpio = 0x007;	// WAN led green 
		break;
	case ROUTER_NETGEAR_WNDR3300:
		power_gpio = 0x005;
		diag_gpio = 0x105;	// power led blink /off to indicate factory defaults
		connected_gpio = 0x007;	// WAN led green 
		break;
	case ROUTER_ASKEY_RT220XD:
		wlan_gpio = 0x100;
		dmz_gpio = 0x101;	// not soldered 
		break;
	case ROUTER_WRT610N:
		power_gpio = 0x001;
		connected_gpio = 0x103;	// ses amber
		ses_gpio = 0x109;	// ses blue
		usb_gpio = 0x100;
		break;
	case ROUTER_WRT610NV2:
		power_gpio = 0x005;
		connected_gpio = 0x100;	// ses amber
		ses_gpio = 0x103;	// ses blue
		usb_gpio = 0x007;
		break;
	case ROUTER_USR_5461:
		usb_gpio = 0x001;
		break;
	case ROUTER_USR_5465:
		//usb_gpio = 0x002; //or 0x001 ??
		break;
	case ROUTER_NETGEAR_WGR614L:
	case ROUTER_NETGEAR_WGR614V9:
		// power_gpio = 0x107;       // don't use - resets router
		diag_gpio = 0x006;
		connected_gpio = 0x104;
		break;
	case ROUTER_NETGEAR_WG602_V4:
		power_gpio = 0x101;	// trick: make lan led green for 100Mbps
		break;
	case ROUTER_BELKIN_F5D7231_V2000:
		connected_gpio = 0x104;
		diag_gpio = 0x001;	// power led blink /off to indicate factory defaults
		break;
	case ROUTER_NETGEAR_WNR3500L:
		power_gpio = 0x003;	//power led green
		diag_gpio = 0x007;	// power led amber
		ses_gpio = 0x001;	// WPS led green
		connected_gpio = 0x002;	//wan led green
		break;
	case ROUTER_WRT320N:
		power_gpio = 0x002;	//power/diag (disabled=blink)
		ses_gpio = 0x103;	// ses blue
		connected_gpio = 0x104;	//ses orange
		break;
	case ROUTER_ASUS_RTN12:
		power_gpio = 0x102;
		diag_gpio = 0x002;	// power blink
		break;
	case ROUTER_ASUS_RTN10:
	case ROUTER_ASUS_RTN16:
	case ROUTER_NETCORE_NW618:
		power_gpio = 0x101;
		diag_gpio = 0x001;	// power blink
		break;
	case ROUTER_DYNEX_DX_NRUTER:
		power_gpio = 0x001;
		diag_gpio = 0x101;	// power blink
		connected_gpio = 0x100;
		sec0_gpio = 0x103;
		break;
#endif
	}
	if (type == LED_DIAG && v1func == 1) {
		if (act == LED_ON)
			C_led(1);
		else
			C_led(0);
	}

	switch (type) {
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
		use_gpio = connblue ? ses_gpio : connected_gpio;
		break;
	case LED_BRIDGE:
		use_gpio = bridge_gpio;
		break;
	case LED_VPN:
		use_gpio = vpn_gpio;
		break;
	case LED_SES:
		use_gpio = connblue ? connected_gpio : ses_gpio;
		break;
	case LED_SES2:
		use_gpio = ses2_gpio;
		break;
	case LED_WLAN:
		use_gpio = wlan_gpio;
		break;
	case LED_USB:
		use_gpio = usb_gpio;
		break;
	case LED_SEC0:
		use_gpio = sec0_gpio;
		break;
	case LED_SEC1:
		use_gpio = sec1_gpio;
		break;
	}
	if ((use_gpio & 0x0ff) != 0x0ff) {
		gpio_value = use_gpio & 0x0ff;
		enable = (use_gpio & 0x100) == 0 ? 1 : 0;
		disable = (use_gpio & 0x100) == 0 ? 0 : 1;
		switch (act) {
		case LED_ON:
			set_gpio(gpio_value, enable);
			break;
		case LED_OFF:
			set_gpio(gpio_value, disable);
			break;
		case LED_FLASH:	// will lit the led for 1 sec.
			set_gpio(gpio_value, enable);
			sleep(1);
			set_gpio(gpio_value, disable);
			break;
		}
	}
	return 1;

#endif
}

int file_to_buf(char *path, char *buf, int len)
{
	FILE *fp;

	memset(buf, 0, len);

	if ((fp = fopen(path, "r"))) {
		fgets(buf, len, fp);
		fclose(fp);
		return 1;
	}

	return 0;
}

int ishexit(char c)
{

	if (strchr("01234567890abcdefABCDEF", c) != (char *)0)
		return 1;

	return 0;
}

int getMTD(char *name)
{
	char buf[128];
	int device;

	sprintf(buf, "cat /proc/mtd|grep \"%s\"", name);
	FILE *fp = popen(buf, "rb");

	fscanf(fp, "%s", &buf[0]);
	device = buf[3] - '0';
	pclose(fp);
	return device;
}

int insmod(char *module)
{
	return eval("insmod", module);
}

void rmmod(char *module)
{
	eval("rmmod", module);
}

#include "revision.h"

char *getSoftwareRevision(void)
{
	return "" SVN_REVISION "";
}

#ifdef HAVE_OLED
void initlcd()
{

}

void lcdmessage(char *message)
{
	eval("oled-print", "DD-WRT v24 sp2", "build:" SVN_REVISION,
	     "3G/UMTS Router", message);
}

void lcdmessaged(char *dual, char *message)
{

}

#endif

#if 0

static int fd;

void SetEnvironment()
{
	system("stty ispeed 2400 < /dev/tts/1");
	system("stty raw < /dev/tts/1");
}

int Cmd = 254;			/* EZIO Command */
int cls = 1;			/* Clear screen */
void Cls()
{
	write(fd, &Cmd, 1);
	write(fd, &cls, 1);
}

int init = 0x28;
void Init()
{
	write(fd, &Cmd, 1);
	write(fd, &init, 1);
}

int stopsend = 0x37;
void StopSend()
{
	write(fd, &Cmd, 1);
	write(fd, &init, 1);
}

int home = 2;			/* Home cursor */
void Home()
{
	write(fd, &Cmd, 1);
	write(fd, &home, 1);
}

int readkey = 6;		/* Read key */
void ReadKey()
{
	write(fd, &Cmd, 1);
	write(fd, &readkey, 1);
}

int blank = 8;			/* Blank display */
void Blank()
{
	write(fd, &Cmd, 1);
	write(fd, &blank, 1);
}

int hide = 12;			/* Hide cursor & display blanked characters */
void Hide()
{
	write(fd, &Cmd, 1);
	write(fd, &hide, 1);
}

int turn = 13;			/* Turn On (blinking block cursor) */
void TurnOn()
{
	write(fd, &Cmd, 1);
	write(fd, &turn, 1);
}

int show = 14;			/* Show underline cursor */
void Show()
{
	write(fd, &Cmd, 1);
	write(fd, &show, 1);
}

int movel = 16;			/* Move cursor 1 character left */
void MoveL()
{
	write(fd, &Cmd, 1);
	write(fd, &movel, 1);
}

int mover = 20;			/* Move cursor 1 character right */
void MoveR()
{
	write(fd, &Cmd, 1);
	write(fd, &mover, 1);
}

int scl = 24;			/* Scroll cursor 1 character left */
void ScrollL()
{
	write(fd, &Cmd, 1);
	write(fd, &scl, 1);
}

int scr = 28;			/* Scroll cursor 1 character right */
void ScrollR()
{
	write(fd, &Cmd, 1);
	write(fd, &scr, 1);
}

int setdis = 64;		/* Command */
void SetDis()
{
	write(fd, &Cmd, 1);
	write(fd, &setdis, 1);

}

int a, b;
void ShowMessage(char *str1, char *str2)
{
	char nul[] = "                                       ";

	a = strlen(str1);
	b = 40 - a;
	write(fd, str1, a);
	write(fd, nul, b);
	write(fd, str2, strlen(str2));
}

void initlcd()
{

	fd = open("/dev/tts/1", O_RDWR);

				  /** Open Serial port (COM2) */
	if (fd > 0) {
		close(fd);
		SetEnvironment();	/* Set RAW mode */
		fd = open("/dev/tts/1", O_RDWR);
		Init();		/* Initialize EZIO twice */
		Init();

		Cls();		/* Clear screen */
	}
	close(fd);
}

void lcdmessage(char *message)
{

	fd = open("/dev/tts/1", O_RDWR);
				   /** Open Serial port (COM2) */

	if (fd > 0) {
		Init();		/* Initialize EZIO twice */
		Init();
		SetDis();
		Cls();
		Home();
		ShowMessage("State", message);
		close(fd);
	}
}

void lcdmessaged(char *dual, char *message)
{

	fd = open("/dev/tts/1", O_RDWR);

				  /** Open Serial port (COM2) */

	if (fd > 0) {
		Init();		/* Initialize EZIO twice */
		Init();
		SetDis();
		Cls();		/* Clear screen */
		Home();
		ShowMessage(dual, message);
		close(fd);
	}
}

#endif
static int i64c(int i)
{
	i &= 0x3f;
	if (i == 0)
		return '.';
	if (i == 1)
		return '/';
	if (i < 12)
		return ('0' - 2 + i);
	if (i < 38)
		return ('A' - 12 + i);
	return ('a' - 38 + i);
}

int crypt_make_salt(char *p, int cnt, int x)
{
	x += getpid() + time(NULL);
	do {
		/*
		 * x = (x*1664525 + 1013904223) % 2^32 generator is lame (low-order
		 * bit is not "random", etc...), but for our purposes it is good
		 * enough 
		 */
		x = x * 1664525 + 1013904223;
		/*
		 * BTW, Park and Miller's "minimal standard generator" is x = x*16807 
		 * % ((2^31)-1) It has no problem with visibly alternating lowest bit
		 * but is also weak in cryptographic sense + needs div, which needs
		 * more code (and slower) on many CPUs 
		 */
		*p++ = i64c(x >> 16);
		*p++ = i64c(x >> 22);
	}
	while (--cnt);
	*p = '\0';
	return x;
}

#define MD5_OUT_BUFSIZE 36

char *zencrypt(char *passwd)
{
	char salt[sizeof("$N$XXXXXXXX")];	/* "$N$XXXXXXXX" or "XX" */
	static char passout[MD5_OUT_BUFSIZE];

	strcpy(salt, "$1$");
	crypt_make_salt(salt + 3, 4, 0);
	strcpy(passout, crypt((unsigned char *)passwd, (unsigned char *)salt));
	return passout;
}

int has_gateway(void)
{
	if (nvram_match("wk_mode", "gateway"))
		return 1;
	if (nvram_match("wk_mode", "olsr") && nvram_match("olsrd_gateway", "1"))
		return 1;
	return 0;
}
