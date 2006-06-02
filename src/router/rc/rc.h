/*
 * Router rc control script
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.h,v 1.8 2005/11/30 11:54:21 seg Exp $
 */

#ifndef _rc_h_
#define _rc_h_

//#include <code_pattern.h>
#include <bcmconfig.h>
#include <netinet/in.h>

#include <fcntl.h>

#include <cy_conf.h>

/* AhMan  March 18 2005 */
#define PPPOE0          0

/* udhcpc scripts */
extern int udhcpc_main (int argc, char **argv);

extern void runStartup (char *folder, char *extension);

extern void shutdown_system (void);

extern void start_dhcpc (char *wan_ifname);

/* ppp scripts */
extern int ipup_main (int argc, char **argv);
extern int ipdown_main (int argc, char **argv);

/* http functions */
extern int http_get (const char *server, char *buf, size_t count,
		     off_t offset);
extern int http_post (const char *server, char *buf, size_t count);
extern int http_stats (const char *url);
extern void addHost (char *host, char *ip);
/* init */
extern int console_init (void);
extern pid_t ddrun_shell (int timeout, int nowait);
extern void signal_init (void);
extern void fatal_signal (int sig);

/* interface */
extern int ifconfig (char *ifname, int flags, char *addr, char *netmask);
extern int route_add (char *name, int metric, char *dst, char *gateway,
		      char *genmask);
extern int route_del (char *name, int metric, char *dst, char *gateway,
		      char *genmask);

/* network */
extern void start_lan (void);
extern void stop_lan (void);
extern void start_wan (int status);
extern void start_wan_done (char *ifname);
extern void stop_wan (void);
extern int hotplug_net (void);

/* services */
#ifdef HAVE_CHILLI
extern int start_chilli (void);
extern int stop_chilli (void);
#endif


extern int check_process (char *name);
static int wlconf_up (char *name);
extern int stop_l2tp (void);
extern int start_udhcpd (void);
extern int stop_udhcpd (void);
extern int start_dns (void);
extern int stop_dns (void);
extern int start_dnsmasq (void);
extern int stop_dnsmasq (void);
extern int start_dhcpfwd (void);
extern int stop_dhcpfwd (void);
extern int start_ntpc (void);
extern int stop_ntpc (void);
extern int start_services (void);
extern int stop_services (void);

extern int config_vlan (void);
extern void config_loopback (void);

extern int start_splashd (void);
extern int stop_splashd (void);

/* lonewolf mods */
extern int flush_interfaces (void);
extern int start_nonstd_interfaces (void);
extern int setup_vlans (void);
/* end lonewolf mods */

extern int start_nas (char *type);
#ifdef HAVE_SPUTNIK_APD
extern int start_sputnik (void);
extern int stop_sputnik (void);
extern int start_sputnik_apd (void);
extern int stop_sputnik_apd (void);
#endif
/* firewall */
#ifdef __CONFIG_NETCONF__
extern int start_firewall (void);
extern int stop_firewall (void);
#else
//#define start_firewall() do {} while (0)
//#define stop_firewall() do {} while (0)
#endif

/* routes */
extern int set_routes (void);

#define GOT_IP			0x01
#define RELEASE_IP		0x02
#define	GET_IP_ERROR		0x03
#define RELEASE_WAN_CONTROL	0x04
#define SET_LED(val)
/*

#define SET_LED(val) \
{ \
	int filep; \
	if(check_hw_type() == BCM4704_BCM5325F_CHIP) { \
		if ((filep = open("/dev/ctmisc", O_RDWR,0))) \
		{ \
			ioctl(filep, val, 0); \
			close(filep); \
		} \
	} \
}
*/
////////////////////////////////////////////////////////////
#define BOOT 0
#define REDIAL 1
#define DELAY_PING

extern int start_resetbutton (void);
extern int stop_resetbutton (void);

extern int start_tftpd (void);
extern int stop_tftpd (void);

extern int start_cron (void);
extern int stop_cron (void);

extern int start_zebra (void);
extern int stop_zebra (void);

extern int start_redial (void);
extern int stop_redial (void);

extern int start_ddns (void);
extern int stop_ddns (void);

extern int start_upnp (void);
extern int stop_upnp (void);

extern int start_ntp (void);
extern int stop_ntp (void);

extern int start_pptp (int status);
extern int stop_pptp (void);

extern int start_syslog (void);
extern int stop_syslog (void);

extern int start_process_monitor (void);
extern int stop_process_monitor (void);

extern int stop_dhcpc ();

extern int stop_pppoe (void);


void start_custom_script (void);

extern int start_httpd (void);
extern int stop_httpd (void);

// Sveasoft additions
#ifdef HAVE_TELNET
extern int start_telnetd (void);
extern int stop_telnetd (void);
#endif

extern int start_ipv6 (void);

#ifdef HAVE_SSHD
extern int start_sshd (void);
extern int stop_sshd (void);
#endif

#ifdef HAVE_RADVD
extern int start_radvd (void);
extern int stop_radvd (void);
#endif


extern int httpd_main (int argc, char **argv);
extern int bird_main (int argc, char **argv);
extern int dnsmasq_main (int argc, char **argv);
#ifdef HAVE_IPROUTE2
extern int tc_main (int argc, char **argv);
#endif
#ifdef HAVE_DHCPFWD
extern int dhcpforward_main (int argc, char **argv);
#endif
#ifdef HAVE_PPPD
extern int pppd_main (int argc, char **argv);
#endif

#ifdef HAVE_SSHD
extern int dropbearconvert_main (int argc, char **argv);
extern int cli_main (int argc, char **argv);
extern int dropbearkey_main (int argc, char **argv);
extern int dropbear_main (int argc, char **argv);
extern int scp_main (int argc, char **argv);
#endif

#ifdef HAVE_PPTP
extern int pptpd_main (int argc, char **argv);
extern int start_pptpd (void);
extern int stop_pptpd (void);
#endif

#ifdef HAVE_SNMP
extern int start_snmp (void);
extern int stop_snmp (void);
#endif

#ifdef HAVE_WOL
extern int start_wol (void);
extern int stop_wol (void);
#endif

extern int start_shorewall (void);
extern int stop_shorewall (void);

extern int start_wshaper (void);
extern int stop_wshaper (void);

extern int start_wland (void);
extern int stop_wland (void);
extern int is_running (char *process_name);

extern int create_rc_file (char *name);
extern int wland_main (int argc, char **argv);

extern void load_vpn_modules (void);
extern void unload_vpn_modules (void);
extern void depend_vpn_modules (void);


extern int start_service (char *name);
extern int stop_service (char *name);
extern void startstop (char *name);

extern int zebra_ospf_init (void);
extern int zebra_ripd_init (void);

#define RC_FIREWALL "rc_firewall"
#define RC_STARTUP  "rc_startup"
#define RC_SHUTDOWN "rc_shutdown"

// end Sveasoft additions

extern int filtersync_main (void);
extern int filter_add (int seq);
extern int filter_del (int seq);
extern int resetbutton_main (int argc, char **argv);
//extern int ntp_main(int argc, char **argv);
void ntp_main (timer_t t, int arg);
extern int ipupdate_main (int argc, char **argv);
extern int gpio_main (int argc, char **argv);
extern int redial_main (int argc, char **argv);

extern int del_routes (char *route);

extern int start_single_service (void);

extern int write_boot (const char *path, const char *mtd);
extern void do_mssid (char *lan_ifname);
extern int init_mtu (char *wan_proto);
extern int force_to_dial (void);
extern char *range (char *start, char *end);


static int start_heartbeat (int status);
extern int stop_heartbeat (void);
extern int hb_connect_main (int argc, char **argv);
extern int hb_disconnect_main (int argc, char **argv);
extern int check_ps_main (int argc, char **argv);
extern int listen_main (int argc, char **argv);
extern int ddns_success_main (int argc, char **argv);
extern int process_monitor_main (void);
void cfe_default (void);
//extern int nvram_restore(const char *path, char *mtd);

#endif /* _rc_h_ */
