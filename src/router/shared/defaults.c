
/*
 *********************************************************
 *   Copyright 2003, CyberTAN  Inc.  All Rights Reserved *
 *********************************************************

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of CyberTAN Inc.
 the contents of this file may not be disclosed to third parties,
 copied or duplicated in any form without the prior written
 permission of CyberTAN Inc.

 This software should be used as a reference only, and it not
 intended for production use!


 THIS SOFTWARE IS OFFERED "AS IS", AND CYBERTAN GRANTS NO WARRANTIES OF ANY
 KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE.  CYBERTAN
 SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE
*/

/*
 * Router default NVRAM values
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: defaults.c,v 1.11 2005/11/30 11:53:42 seg Exp $
 */

#include <epivers.h>
#include <string.h>
#include <bcmnvram.h>
#include <typedefs.h>
#include <wlioctl.h>
#include <stdio.h>
#include <ezc.h>

#include <code_pattern.h>
#include <cy_conf.h>

#define XSTR(s) STR(s)
#define STR(s) #s


#ifdef HAVE_SKYTEL
#define HAVE_POWERNOC_WORT54G 1
#define HAVE_POWERNOC 1
#endif

struct nvram_tuple router_defaults[] = {
#ifdef HAVE_FON
  {"fon_revision", "13", 0},
  {"router_style", "fon.css", 0},
#else
  {"router_style", "cyan.css", 0},
#endif
  /* OS parameters */
  {"os_name", "", 0},		/* OS name string */
  {"os_version", EPI_VERSION_STR, 0},	/* OS revision */
  {"os_date", __DATE__, 0},	/* OS date */
  {"ct_modules", "", 0},	/* CyberTAN kernel modules */

  /* Miscellaneous parameters */
  {"timer_interval", "3600", 0},	/* Timer interval in seconds */
  //{ "time_zone", "PST8PDT", 0 },                /* Time zone (GNU TZ format) */
#ifdef HAVE_SKYTRON
  {"ntp_server", "ntp0.fau.de", 0},	/* NTP server *//* Modify */
  {"time_zone", "+01 2 2", 0},	/* Time zone (GNU TZ format) Japan */
  {"daylight_time", "1", 0},	/* Automatically adjust clock for daylight */
#else
  {"ntp_server", "", 0},	/* NTP server *//* Modify */

#if COUNTRY == JAPAN
  {"time_zone", "+09 1 0", 0},	/* Time zone (GNU TZ format) Japan */
  {"daylight_time", "0", 0},	/* Automatically adjust clock for daylight */
#else
  {"time_zone", "-08 1 1", 0},	/* Time zone (GNU TZ format) USA */
  {"daylight_time", "1", 0},	/* Automatically adjust clock for daylight */
#endif
#endif
  {"log_level", "0", 0},	/* Bitmask 0:off 1:denied 2:accepted */
#ifdef HAVE_SKYTRON
  {"upnp_enable", "1", 0},	/* 0:Disable 1:Enable */
#else
  {"upnp_enable", "0", 0},	/* 0:Disable 1:Enable */
#endif
  {"ezc_enable", "1", 0},	/* Enable EZConfig updates */
  {"ezc_version", EZC_VERSION_STR, 0},	/* EZConfig version */
  {"is_default", "1", 0},	/* is it default setting: 1:yes 0:no */
  {"os_server", "", 0},		/* URL for getting upgrades */
  {"stats_server", "", 0},	/* URL for posting stats */
  {"console_loglevel", "1", 0},	/* Kernel panics only */

  /* Big switches */
  {"router_disable", "0", 0},	/* lan_proto=static lan_stp=0 wan_proto=disabled */
  {"fw_disable", "0", 0},	/* Disable firewall (allow new connections from the WAN) */

  /* TCP/IP parameters */
  {"log_enable", "0", 0},	/* 0:Disable 1:Eanble *//* Add */
  {"log_ipaddr", "0", 0},	/* syslog recipient */

  /* LAN H/W parameters */
  {"lan_ifname", "", 0},	/* LAN interface name */
  {"lan_ifnames", "", 0},	/* Enslaved LAN interfaces */
  {"lan_hwnames", "", 0},	/* LAN driver names (e.g. et0) */
  {"lan_hwaddr", "", 0},	/* LAN interface MAC address */

  /* LAN TCP/IP parameters */
#ifdef HAVE_POWERNOC_WOAP54G
  {"lan_proto", "static", 0},	/* [static|dhcp] */
#else
  {"lan_proto", "dhcp", 0},	/* [static|dhcp] */
#endif
#ifdef HAVE_SKYTRON
  {"lan_ipaddr", "192.168.0.1", 0},	/* LAN IP address */
#elif HAVE_NEWMEDIA
  {"lan_ipaddr", "172.31.28.3", 0},	/* LAN IP address */
#elif HAVE_FON
  {"lan_ipaddr", "192.168.10.1", 0},	/* LAN IP address */
#elif HAVE_34TELECOM
  {"lan_ipaddr", "192.168.1.4", 0},	/* LAN IP address */
#else
  {"lan_ipaddr", "192.168.1.1", 0},	/* LAN IP address */
#endif
  {"lan_netmask", "255.255.255.0", 0},	/* LAN netmask */
  {"lan_gateway", "0.0.0.0", 0},	/* LAN gateway */
  {"sv_localdns", "0.0.0.0", 0},	/* Local DNS */
#ifdef HAVE_SKYTRON
  {"lan_stp", "0", 0},		/* LAN spanning tree protocol */
#else
  {"lan_stp", "1", 0},		/* LAN spanning tree protocol */
#endif
  {"lan_wins", "", 0},		/* x.x.x.x x.x.x.x ... */
#ifdef HAVE_SKYTRON
  {"lan_domain", "local", 0},	/* LAN domain name */
#else
  {"lan_domain", "", 0},	/* LAN domain name */
#endif
  {"lan_lease", "86400", 0},	/* LAN lease time in seconds */

  /* WAN H/W parameters */
  {"wan_ifname", "", 0},	/* WAN interface name */
  {"wan_ifnames", "", 0},	/* WAN interface names */
  {"wan_hwname", "", 0},	/* WAN driver name (e.g. et1) */
  {"wan_hwaddr", "", 0},	/* WAN interface MAC address */

  /* WAN TCP/IP parameters */

#ifdef HAVE_SKYTRON
  {"wan_proto", "static", 0},	/* [static|dhcp|pppoe|disabled] */
  {"wan_ipaddr", "10.254.254.254", 0},	/* WAN IP address */
  {"wan_netmask", "255.0.0.0", 0},	/* WAN netmask */
  {"wan_gateway", "10.0.0.1", 0},	/* WAN gateway */
  {"wan_dns", "213.146.232.2 213.146.230.2", 0},	/* x.x.x.x x.x.x.x ... */
#else
  {"wan_proto", "dhcp", 0},	/* [static|dhcp|pppoe|disabled] */

  {"wan_ipaddr", "0.0.0.0", 0},	/* WAN IP address */
  {"wan_netmask", "0.0.0.0", 0},	/* WAN netmask */
  {"wan_gateway", "0.0.0.0", 0},	/* WAN gateway */
  {"wan_dns", "", 0},		/* x.x.x.x x.x.x.x ... */
#endif

#ifdef HAVE_POWERNOC_WORT54G
  {"wan_wins", "0.0.0.0", 0},	/* x.x.x.x x.x.x.x ... */
#else
  {"wan_wins", "", 0},		/* x.x.x.x x.x.x.x ... */
#endif
#ifdef HAVE_SKYTRON
  {"wan_hostname", "skymax254b", 0},	/* WAN hostname */
  {"wan_domain", "local", 0},	/* WAN domain name */
#else
  {"wan_hostname", "", 0},	/* WAN hostname */
  {"wan_domain", "", 0},	/* WAN domain name */
#endif
  {"wan_lease", "86400", 0},	/* WAN lease time in seconds */
  {"static_route", "", 0},	/* Static routes (ipaddr:netmask:gateway:metric:ifname ...) */
  {"static_route_name", "", 0},	/* Static routes name ($NAME:name) */
  {"ses_enable", "1", 0},	/* enable ses */
  {"ses_event", "2", 0},	/* initial ses event */

  {"wan_primary", "1", 0},	/* Primary wan connection */
  {"wan_unit", "0", 0},		/* Last configured connection */

  /* Filters */
  {"filter_maclist", "", 0},	/* xx:xx:xx:xx:xx:xx ... */
  {"filter_macmode", "deny", 0},	/* "allow" only, "deny" only, or "disabled" (allow all) */
  {"filter_client0", "", 0},	/* [lan_ipaddr0-lan_ipaddr1|*]:lan_port0-lan_port1,proto,enable,day_start-day_end,sec_start-sec_end,desc */

  {"filter", "on", 0},		/* [on | off] Firewall Protection */
  {"filter_port", "", 0},	/* [lan_ipaddr|*]:lan_port0-lan_port1 */
  {"filter_rule1", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule2", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule3", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule4", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule5", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule6", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule7", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule8", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule9", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_rule10", "", 0},	/* $STAT: $NAME:$$ */
  {"filter_tod1", "", 0},	/* Filter Time of the day */
  {"filter_tod2", "", 0},	/* Filter Time of the day */
  {"filter_tod3", "", 0},	/* Filter Time of the day */
  {"filter_tod4", "", 0},	/* Filter Time of the day */
  {"filter_tod5", "", 0},	/* Filter Time of the day */
  {"filter_tod6", "", 0},	/* Filter Time of the day */
  {"filter_tod7", "", 0},	/* Filter Time of the day */
  {"filter_tod8", "", 0},	/* Filter Time of the day */
  {"filter_tod9", "", 0},	/* Filter Time of the day */
  {"filter_tod10", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf1", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf2", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf3", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf4", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf5", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf6", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf7", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf8", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf9", "", 0},	/* Filter Time of the day */
  {"filter_tod_buf10", "", 0},	/* Filter Time of the day */
  {"filter_ip_grp1", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp2", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp3", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp4", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp5", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp6", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp7", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp8", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp9", "", 0},	/* Filter IP group 1 */
  {"filter_ip_grp10", "", 0},	/* Filter IP group 1 */
  {"filter_mac_grp1", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp2", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp3", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp4", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp5", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp6", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp7", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp8", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp9", "", 0},	/* Filter MAC group 1 */
  {"filter_mac_grp10", "", 0},	/* Filter MAC group 1 */
  {"filter_web_host1", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host2", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host3", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host4", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host5", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host6", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host7", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host8", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host9", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_host10", "", 0},	/* Website Blocking by URL Address */
  {"filter_web_url1", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url2", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url3", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url4", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url5", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url6", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url7", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url8", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url9", "", 0},	/* Website Blocking by keyword */
  {"filter_web_url10", "", 0},	/* Website Blocking by keyword */
  {"filter_port_grp1", "", 0},	/* Blocked Services */
  {"filter_port_grp2", "", 0},	/* Blocked Services */
  {"filter_port_grp3", "", 0},	/* Blocked Services */
  {"filter_port_grp4", "", 0},	/* Blocked Services */
  {"filter_port_grp5", "", 0},	/* Blocked Services */
  {"filter_port_grp6", "", 0},	/* Blocked Services */
  {"filter_port_grp7", "", 0},	/* Blocked Services */
  {"filter_port_grp8", "", 0},	/* Blocked Services */
  {"filter_port_grp9", "", 0},	/* Blocked Services */
  {"filter_port_grp10", "", 0},	/* Blocked Services */
  {"filter_dport_grp1", "", 0},	/* Blocked Services */
  {"filter_dport_grp2", "", 0},	/* Blocked Services */
  {"filter_dport_grp3", "", 0},	/* Blocked Services */
  {"filter_dport_grp4", "", 0},	/* Blocked Services */
  {"filter_dport_grp5", "", 0},	/* Blocked Services */
  {"filter_dport_grp6", "", 0},	/* Blocked Services */
  {"filter_dport_grp7", "", 0},	/* Blocked Services */
  {"filter_dport_grp8", "", 0},	/* Blocked Services */
  {"filter_dport_grp9", "", 0},	/* Blocked Services */
  {"filter_dport_grp10", "", 0},	/* Blocked Services */

  /* Services List */
  {"filter_services", "", 0},
  {"filter_services0",
   "$NAME:006:100bao$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:aim$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:013:aimwebcontent$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:010:applejuice$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:004:ares$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:011:audiogalaxy$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:015:battlefield1942$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:012:battlefield2$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:bgp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:biff$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:010:bittorrent$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:008:ciscovpn$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:citrix$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:code_red$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:020:counterstrike-source$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:cvs$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:018:dayofdefeat-source$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:dhcp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:013:directconnect$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:003:dns$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:doom3$PROT:002:l7$PORT:003:0:0<&nbsp;>",
   0},
  {"filter_services1",
   "$NAME:007:edonkey$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:003:exe$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:fasttrack$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:finger$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:flash$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:freenet$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:ftp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:gif$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:gkrellm$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:011:gnucleuslan$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:gnutella$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:007:goboogy$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:gopher$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:h323$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:020:halflife2-deathmatch$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:hddtemp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:hotline$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:html$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:http$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:011:http-itunes$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:httpaudio$PROT:002:l7$PORT:003:0:0<&nbsp;>",
   0},
  {"filter_services2",
   "$NAME:012:httpcachehit$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:013:httpcachemiss$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:httpvideo$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:ident$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:imap$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:ipp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:irc$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:jabber$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:jpeg$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:kugoo$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:live365$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:lpd$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:012:msnmessenger$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:016:msn-filetransfer$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:mute$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:napster$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:nbns$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:ncp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:netbios$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:nimda$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:nntp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:ntp$PROT:002:l7$PORT:003:0:0<&nbsp;>",
   0},
  {"filter_services3",
   "$NAME:003:ogg$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:openft$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:pdf$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:perl$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:poco$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:pop3$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:010:postscript$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:pressplay$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:002:qq$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:014:quake-halflife$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:quake1$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:quicktime$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:rar$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:rdp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:rlogin$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:rpm$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:rtf$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:rtsp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:shoutcast$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:sip$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:skypeout$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:012:skypetoskype$PROT:002:l7$PORT:003:0:0<&nbsp;>",
   0},
  {"filter_services4",
   "$NAME:003:smb$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:smtp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:snmp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:snmp-mon$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:snmp-trap$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:socks$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:soribada$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:soulseek$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:004:ssdp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:ssh$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:ssl$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:subspace$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:tar$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:telnet$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:tesla$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:tftp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:tls$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:tsp$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:unknown$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:012:validcertssl$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:vnc$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:winmx$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:005:whois$PROT:002:l7$PORT:003:0:0<&nbsp;>",
   0},
  {"filter_services5",
   "$NAME:003:x11$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:xboxlive$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:xunlei$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:yahoo$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:zip$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:zmaap$PROT:002:l7$PORT:003:0:0<&nbsp;>",
   0},
  {"filter_services6",
   "",
   0},
/*  {"filter_services0",
   "$NAME:003:Aim$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:010:Applejuice$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:011:Audiogalaxy$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:009:Bearshare$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Biff$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:010:BitTorrent$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:Citrix$PROT:003:tcp$PORT:009:1494:1494<&nbsp;>$NAME:013:CounterStrike$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:Cvs$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:eDonkey$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:DHCP$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:013:DirectConnect$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:Dns$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Doom$PROT:003:tcp$PORT:007:666:666<&nbsp;>$NAME:009:FastTrack$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:Flash$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:FTP$PROT:003:tcp$PORT:005:21:21<&nbsp;>$NAME:003:Gif$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:Gkrelm$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:011:Gnucleuslan$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:008:Gnutella$PROT:003:p2p$PORT:003:0:0<&nbsp;>$NAME:007:Goboogy$PROT:002",
   0},
  {"filter_services1",
   ":l7$PORT:003:0:0<&nbsp;>$NAME:004:H323$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:014:Quake-Halflife$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:Hotline$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Html$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Http$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:HTTPS$PROT:003:tcp$PORT:007:443:443<&nbsp;>$NAME:003:IKE$PROT:003:udp$PORT:007:500:500<&nbsp;>$NAME:003:Irc$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:IMAP$PROT:003:tcp$PORT:007:143:143<&nbsp;>$NAME:008:Kerberos$PROT:003:tcp$PORT:005:88:88<&nbsp;>$NAME:004:LDAP$PROT:003:tcp$PORT:007:389:389<&nbsp;>$NAME:010:LotusNotes$PROT:003:tcp$PORT:009:1352:1352<&nbsp;>$NAME:006:Jabber$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Jpeg$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:Live365$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:MS SQL$PROT:003:tcp$PORT:009:1433:1433<&nbsp;>$NAME:003:MSN$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:Mysql$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:007:Netbios$PROT:003:tcp$PORT:007:136:139<&nbsp;>$NAME:003:NFS$PROT:003:tcp$PORT:009:2049:2049<&nbsp;>$NAME:004:NNTP$PROT:003:tcp$PORT:007:119:119<",
   0},
  {"filter_services2",
   "&nbsp;>$NAME:003:NTP$PROT:003:tcp$PORT:007:123:123<&nbsp;>$NAME:003:Ogg$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:Oracle$PROT:003:tcp$PORT:009:1521:1521<&nbsp;>$NAME:007:Packet8$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:PCAny$PROT:003:tcp$PORT:009:5632:5632<&nbsp;>$NAME:003:PDF$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Perl$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:Ping$PROT:004:icmp$PORT:003:0:0<&nbsp;>$NAME:004:POP3$PROT:003:tcp$PORT:007:110:110<&nbsp;>$NAME:010:Postscript$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:Quake$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:Quake3$PROT:003:tcp$PORT:011:27960:27960<&nbsp;>$NAME:009:Quicktime$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:Radius$PROT:003:tcp$PORT:009:1812:1813<&nbsp;>$NAME:003:Rpm$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:Rtf$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:SFTP$PROT:003:tcp$PORT:007:115:115<&nbsp;>$NAME:003:SIP$PROT:003:tcp$PORT:009:5060:5061<&nbsp;>$NAME:005:Skype$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:SMB$PROT:003:tcp$PORT:007:445:445<&nbsp;>$NAME:004:SMTP$PROT:003:tcp$PORT:005:25:25<&nbsp;>",
   0},
  {"filter_services3",
   "$NAME:004:SNMP$PROT:003:udp$PORT:007:161:161<&nbsp;>$NAME:005:Socks$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:004:SPOP$PROT:003:udp$PORT:007:995:995<&nbsp;>$NAME:003:SSH$PROT:003:tcp$PORT:005:22:22<&nbsp;>$NAME:003:SSL$PROT:003:tcp$PORT:007:443:443<&nbsp;>$NAME:003:Tar$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:006:Telnet$PROT:003:tcp$PORT:005:23:23<&nbsp;>$NAME:007:TermSrv$PROT:003:tcp$PORT:009:1604:1604<&nbsp;>$NAME:004:TFTP$PROT:003:udp$PORT:005:69:69<&nbsp;>$NAME:006:Unreal$PROT:003:tcp$PORT:009:7778:7778<&nbsp;>$NAME:003:VNC$PROT:003:tcp$PORT:009:5900:5900<&nbsp;>$NAME:005:WinMx$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:003:X11$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:Yahoo$PROT:002:l7$PORT:003:0:0<&nbsp;>$NAME:005:Zebra$PROT:003:tcp$PORT:009:2600:2606<&nbsp;>",
   0},
  {"filter_services4", "", 0},*/

  /* Port forwards */
  {"dmz_enable", "0", 0},	/* Enable (1) or Disable (0) */
  {"dmz_ipaddr", "0", 0},	/* x.x.x.x (equivalent to 0-60999>dmz_ipaddr:0-60999) */
  {"autofw_port0", "", 0},	/* out_proto:out_port,in_proto:in_port0-in_port1>to_port0-to_port1,enable,desc */

  /* DHCP server parameters */
  {"dhcp_start", "100", 0},	/* First assignable DHCP address */
  //{ "dhcp_end", "150", 0 },             /* Last assignable DHCP address */      /* Remove */
  {"dhcp_num", "50", 0},	/* Number of DHCP Users *//* Add */
#ifdef HAVE_SKYTRON
  {"dhcp_lease", "10", 0},	/* LAN lease time in minutes */
#else
  {"dhcp_lease", "1440", 0},	/* LAN lease time in minutes */
#endif
  {"dhcp_domain", "wan", 0},	/* Use WAN domain name first if available (wan|lan) */
  {"dhcp_wins", "wan", 0},	/* Use WAN WINS first if available (wan|lan) */
  {"wan_get_dns", "", 0},	/* DNS IP address which get by dhcpc *//* Add */


  /* Web server parameters */

  {"http_username", "root", 0},	/* Username */

#ifdef HAVE_SKYTRON
  {"skyhttp_username", "skytron", 0},	/* Username */
  {"skyhttp_passwd", "skytron", 0},	/* Password */
  {"http_passwd", "skymax", 0},	/* Password */
#elif HAVE_NEWMEDIA
  {"http_passwd", "nmn4711", 0},	/* Password */
#else
  {"http_passwd", "admin", 0},	/* Password */
#endif

  {"http_wanport", "8080", 0},	/* WAN port to listen on */
  {"http_lanport", "80", 0},	/* LAN port to listen on */
  {"http_enable", "1", 0},	/* HTTP server enable/disable */
#ifdef HAVE_HTTPS
  {"https_enable", "0", 0},	/* HTTPS server enable/disable */
#endif
  {"http_method", "post", 0},	/* HTTP method */
#ifdef HAVE_SAGAR
  {"web_wl_filter", "1", 0},	/* Allow/Deny Wireless Access Web */
#else
  {"web_wl_filter", "0", 0},	/* Allow/Deny Wireless Access Web */
#endif
  /* PPPoE parameters */
  {"pppoe_ifname", "", 0},	/* PPPoE enslaved interface */
  {"ppp_username", "", 0},	/* PPP username */
  {"ppp_passwd", "", 0},	/* PPP password */
  {"ppp_idletime", "5", 0},	/* Dial on demand max idle time (mins) */
  {"ppp_keepalive", "0", 0},	/* Restore link automatically */
  {"ppp_demand", "0", 0},	/* Dial on demand */
  {"ppp_redialperiod", "30", 0},	/* Redial Period  (seconds) */
  {"ppp_mru", "1500", 0},	/* Negotiate MRU to this value */
  {"ppp_mtu", "1500", 0},	/* Negotiate MTU to the smaller of this value or the peer MRU */
  {"ppp_service", "", 0},	/* PPPoE service name */
  {"ppp_ac", "", 0},		/* PPPoE access concentrator name */
  {"ppp_static", "0", 0},	/* Enable / Disable Static IP  */
  {"ppp_static_ip", "", 0},	/* PPPoE Static IP */
  {"ppp_get_ac", "", 0},	/* PPPoE Server ac name */
  {"ppp_get_srv", "", 0},	/* PPPoE Server service name */

  /* Wireless parameters */
  {"wl_ifname", "", 0},		/* Interface name */
  {"wl_hwaddr", "", 0},		/* MAC address */
  {"wl_phytype", "g", 0},	/* Current wireless band ("a" (5 GHz), "b" (2.4 GHz), or "g" (2.4 GHz)) *//* Modify */
  {"wl_corerev", "", 0},	/* Current core revision */
  {"wl_phytypes", "", 0},	/* List of supported wireless bands (e.g. "ga") */
  {"wl_radioids", "", 0},	/* List of radio IDs */
#ifdef HAVE_WTS
  {"wl_ssid", "www.wts.com.ve", 0},	/* Service set ID (network name) */
#elif HAVE_SKYTEL
  {"wl_ssid", "skytel", 0},	/* Service set ID (network name) */
#elif HAVE_POWERNOC
  {"wl_ssid", "powernoc", 0},	/* Service set ID (network name) */
#elif HAVE_SKYTRON
  {"wl_ssid", "SKYTRON Network", 0},	/* Service set ID (network name) */
#elif HAVE_SAGAR
  {"wl_ssid", "hotspot-internet", 0},	/* Service set ID (network name) */
#elif HAVE_NEWMEDIA
  {"wl_ssid", "changeme", 0},	/* Service set ID (network name) */
#elif HAVE_34TELECOM
  {"wl_ssid", "Lobo", 0},	/* Service set ID (network name) */
#elif HAVE_FON
  {"wl_ssid", "FON", 0},	/* Service set ID (network name) */
#else
  {"wl_ssid", "dd-wrt", 0},	/* Service set ID (network name) */
#endif
  {"wl_country", "Japan", 0},	/* Country (default obtained from driver) */
#ifdef HAVE_NEWMEDIA
  {"wl_radio", "1", 0},		/* Enable (1) or disable (0) radio */
  {"wl_closed", "1", 0},	/* Closed (hidden) network */
#else
#ifdef HAVE_MADWIFI
  {"ath0_radio", "1", 0},	/* Enable (1) or disable (0) radio */
  {"ath0_closed", "0", 0},	/* Closed (hidden) network */
  {"ath1_radio", "1", 0},	/* Enable (1) or disable (0) radio */
  {"ath1_closed", "0", 0},	/* Closed (hidden) network */
  {"ath2_radio", "1", 0},	/* Enable (1) or disable (0) radio */
  {"ath2_closed", "0", 0},	/* Closed (hidden) network */
  {"ath3_radio", "1", 0},	/* Enable (1) or disable (0) radio */
  {"ath3_closed", "0", 0},	/* Closed (hidden) network */
  {"ath4_radio", "1", 0},	/* Enable (1) or disable (0) radio */
  {"ath4_closed", "0", 0},	/* Closed (hidden) network */
  {"ath5_radio", "1", 0},	/* Enable (1) or disable (0) radio */
  {"ath5_closed", "0", 0},	/* Closed (hidden) network */
#else
  {"wl_radio", "1", 0},		/* Enable (1) or disable (0) radio */
  {"wl_closed", "0", 0},	/* Closed (hidden) network */
#endif

#endif


#ifdef HAVE_SAGAR
  {"wl_ap_isolate", "1", 0},	/* AP isolate mode */
#elif HAVE_FON
  {"wl_ap_isolate", "1", 0},	/* AP isolate mode */
#else
  {"wl_ap_isolate", "0", 0},	/* AP isolate mode */
#endif
#ifdef HAVE_POWERNOC_WORT54G
  {"wl_mode", "sta", 0},	/* AP mode (ap|sta|wds) */
#elif HAVE_SKYTRON
  {"wl_mode", "sta", 0},
#else
#ifndef HAVE_MADWIFI
  {"wl_mode", "ap", 0},		/* AP mode (ap|sta|wds) */
#else
  {"ath0_mode", "ap", 0},	/* AP mode (ap|sta|wds) */
  {"ath1_mode", "ap", 0},	/* AP mode (ap|sta|wds) */
  {"ath2_mode", "ap", 0},	/* AP mode (ap|sta|wds) */
  {"ath3_mode", "ap", 0},	/* AP mode (ap|sta|wds) */
  {"ath4_mode", "ap", 0},	/* AP mode (ap|sta|wds) */
  {"ath5_mode", "ap", 0},	/* AP mode (ap|sta|wds) */
#endif
#endif
  {"wl_lazywds", "0", 0},	/* Enable "lazy" WDS mode (0|1) */
  {"wl_wds", "", 0},		/* xx:xx:xx:xx:xx:xx ... */
  {"wl_wep", "off", 0},		/* Data encryption (off|wep|tkip|aes) */
  {"wl_crypto", "off", 0},	/* Data encryption (off|wep|tkip|aes) */
  {"wl_auth", "0", 0},		/* Shared key authentication optional (0) or required (1) */
  {"wl_key", "1", 0},		/* Current WEP key */
  {"wl_key1", "", 0},		/* 5/13 char ASCII or 10/26 char hex */
  {"wl_key2", "", 0},		/* 5/13 char ASCII or 10/26 char hex */
  {"wl_key3", "", 0},		/* 5/13 char ASCII or 10/26 char hex */
  {"wl_key4", "", 0},		/* 5/13 char ASCII or 10/26 char hex */
  {"wl_maclist", "", 0},	/* xx:xx:xx:xx:xx:xx ... */
  {"wl_macmode", "disabled", 0},	/* "allow" only, "deny" only, or "disabled" (allow all) */
  {"wl_macmode1", "disabled", 0},	/* "disabled" or "other" for WEBB *//* Add */
#ifdef HAVE_MADWIFI
  {"ath0_channel", "0", 0},	/* Channel number */
  {"ath1_channel", "0", 0},	/* Channel number */
#else
  {"wl_channel", "6", 0},	/* Channel number */
#endif
  {"wl_rate", "0", 0},		/* Rate (bps, 0 for auto) */
  {"wl_rateset", "default", 0},	/* "default" or "all" or "12" */
  {"wl_frag", "2346", 0},	/* Fragmentation threshold */
#ifdef HAVE_POWERNOC_WORT54G
  {"wl_rts", "65", 0},		/* RTS threshold */
#else
  {"wl_rts", "2347", 0},	/* RTS threshold */
#endif
  {"wl_dtim", "1", 0},		/* DTIM period (3.11.5) *//* It is best value for WiFi test */
  {"wl_bcn", "100", 0},		/* Beacon interval */
  {"wl_plcphdr", "long", 0},	/* 802.11b PLCP preamble type */

#ifdef HAVE_NEWMEDIA
  {"wl_net_mode", "disabled", 0},	/* Wireless mode (mixed|g-only|b-only|disable) */
#else
  {"wl_net_mode", "mixed", 0},	/* Wireless mode (mixed|g-only|b-only|disable) */
#endif

#ifdef HAVE_SAGAR
  {"wl_gmode", XSTR (GMODE_LEGACY_B), 0},	/* 54g mode */
#elif HAVE_NEWMEDIA
  {"wl_gmode", "-1", 0},	/* 54g mode */
#else
  {"wl_gmode", XSTR (GMODE_AUTO), 0},	/* 54g mode */
#endif
  {"wl_gmode_protection", "off", 0},	/* 802.11g RTS/CTS protection (off|auto) */
#ifdef HAVE_SKYTEL
  {"wl_frameburst", "on", 0},	/* BRCM Frambursting mode (off|on) */
#else
  {"wl_frameburst", "off", 0},	/* BRCM Frambursting mode (off|on) */
#endif
  {"wl_infra", "1", 0},		/* Network Type (BSS/IBSS) */

  {"wl_passphrase", "", 0},	/* Passphrase *//* Add */
  {"wl_wep_bit", "64", 0},	/* WEP encryption [64 | 128] *//* Add */
  {"wl_wep_buf", "", 0},	/* save all settings for web *//* Add */
  {"wl_wep_gen", "", 0},	/* save all settings for generate button *//* Add */
  {"wl_wep_last", "", 0},	/* Save last wl_wep mode *//* Add */
  {"wl_active_mac", "", 0},	/* xx:xx:xx:xx:xx:xx ... *//* Add */
  {"wl_mac_list", "", 0},	/* filter MAC *//* Add */
  {"wl_mac_deny", "", 0},	/* filter MAC *//* Add */


  /* WPA parameters */
  {"security_mode", "disabled", 0},	/* WPA mode (disabled|radius|wpa|psk|wep) for WEB *//* Add */
  {"security_mode_last", "", 0},	/* Save last WPA mode *//* Add */
  {"wl_auth_mode", "disabled", 0},	/* WPA mode (disabled|radius|wpa|psk) */
  {"wl_akm", "", 0},
  {"wl_wpa_psk", "", 0},	/* WPA pre-shared key */
  {"wl_wpa_gtk_rekey", "3600", 0},	/* WPA GTK rekey interval *//* Modify */
  {"wl_radius_ipaddr", "", 0},	/* RADIUS server IP address */
  {"wl_radius_key", "", 0},	/* RADIUS shared secret */
  {"wl_radius_port", "1812", 0},	/* RADIUS server UDP port */
#ifdef HAVE_SKYTEL
  {"wl_afterburner", "auto", 0},	/* Afterburner/Speedbooster */
#else
  {"wl_afterburner", "off", 0},	/* Afterburner/Speedbooster */
#endif
  {"wl_unit", "0", 0},		/* Last configured interface */

  /* Restore defaults */
  {"restore_defaults", "0", 0},	/* Set to 0 to not restore defaults on boot */

  ////////////////////////////////////////
  {"filter", "on", 0},		/* Firewall Protection [on|off] */
#ifdef HAVE_WTS
  {"router_name", "WTS", 0},	/* Router name string */
#elif  HAVE_SKYTEL
  {"router_name", "ST54G", 0},
#elif  HAVE_POWERNOC_WORT54G
  {"router_name", "WORT54G", 0},
#elif  HAVE_POWERNOC_WOAP54G
  {"router_name", "WOAP54G", 0},
#elif  HAVE_SKYTRON
  {"router_name", "skymax254b", 0},
#elif  HAVE_34TELECOM
  {"router_name", "MiuraBasic", 0},
#else
  {"router_name", MODEL_NAME, 0},	/* Router name string */
#endif
  {"ntp_mode", "auto", 0},	/* NTP server [manual | auto] */
  {"pptp_server_ip", "", 0},	/* as same as WAN gateway */
  {"pptp_get_ip", "", 0},	/* IP Address assigned by PPTP server */

  /* for firewall */

#ifdef HAVE_SKYTRON
  {"filter", "off", 0},		/* Firewall Protection [on|off] */
  {"block_wan", "0", 0},	/* Block WAN Request [1|0] */
  {"ident_pass", "1", 0},	/* IDENT passthrough [1|0] */
  {"block_proxy", "0", 0},	/* Block Proxy [1|0] */
  {"block_java", "0", 0},	/* Block Java [1|0] */
  {"block_activex", "0", 0},	/* Block ActiveX [1|0] */
  {"block_cookie", "0", 0},	/* Block Cookie [1|0] */
  {"multicast_pass", "1", 0},	/* Multicast Pass Through [1|0] */
  {"ipsec_pass", "1", 0},	/* IPSec Pass Through [1|0] */
  {"pptp_pass", "1", 0},	/* PPTP Pass Through [1|0] */
  {"l2tp_pass", "1", 0},	/* L2TP Pass Through [1|0] */
  {"remote_management", "1", 0},	/* Remote Management [1|0] */
#elif HAVE_SAGAR
  {"filter", "off", 0},		/* Firewall Protection [on|off] */
  {"block_wan", "0", 0},	/* Block WAN Request [1|0] */
  {"ident_pass", "1", 0},	/* IDENT passthrough [1|0] */
  {"block_proxy", "0", 0},	/* Block Proxy [1|0] */
  {"block_java", "0", 0},	/* Block Java [1|0] */
  {"block_activex", "0", 0},	/* Block ActiveX [1|0] */
  {"block_cookie", "0", 0},	/* Block Cookie [1|0] */
  {"multicast_pass", "1", 0},	/* Multicast Pass Through [1|0] */
  {"ipsec_pass", "1", 0},	/* IPSec Pass Through [1|0] */
  {"pptp_pass", "1", 0},	/* PPTP Pass Through [1|0] */
  {"l2tp_pass", "1", 0},	/* L2TP Pass Through [1|0] */
  {"remote_management", "1", 0},	/* Remote Management [1|0] */
#else
  {"filter", "on", 0},		/* Firewall Protection [on|off] */
  {"block_wan", "1", 0},	/* Block WAN Request [1|0] */
  {"ident_pass", "0", 0},	/* IDENT passthrough [1|0] */
  {"block_proxy", "0", 0},	/* Block Proxy [1|0] */
  {"block_java", "0", 0},	/* Block Java [1|0] */
  {"block_activex", "0", 0},	/* Block ActiveX [1|0] */
  {"block_cookie", "0", 0},	/* Block Cookie [1|0] */
  {"multicast_pass", "1", 0},	/* Multicast Pass Through [1|0] */
  {"ipsec_pass", "1", 0},	/* IPSec Pass Through [1|0] */
  {"pptp_pass", "1", 0},	/* PPTP Pass Through [1|0] */
  {"l2tp_pass", "1", 0},	/* L2TP Pass Through [1|0] */
  {"remote_management", "0", 0},	/* Remote Management [1|0] */
#endif
#ifdef HAVE_SAGAR
  {"remote_mgt_https", "1", 0},	/* Remote Management use https [1|0] */// add
#elif HAVE_HTTPS
  {"remote_mgt_https", "0", 0},	/* Remote Management use https [1|0] */// add
#endif

  {"mtu_enable", "0", 0},	/* WAN MTU [1|0] */
  {"wan_mtu", "1500", 0},	/* Negotiate MTU to the smaller of this value or the peer MRU */

  /* for forward */
  {"forward_port", "", 0},	/* name:[on|off]:[tcp|udp|both]:wan_port>lan_ipaddr:lan_port0 */
  {"forward_spec", "", 0},	/* name:[on|off]:[tcp|udp|both]:wan_port>lan_ipaddr:lan_port0 */
  {"forward_upnp", "", 0},	/* name:[on|off]:[tcp|udp|both]:wan_port>lan_ipaddr:lan_port0 */

  {"port_trigger", "", 0},	/* name:[on|off]:[tcp|udp|both]:wan_port0-wan_port1>lan_port0-lan_port1 */

  /* for dynamic route */
  {"wk_mode", "gateway", 0},	/* Network mode [gateway|router] */
  {"dr_setting", "0", 0},	/* [ Disable | WAN | LAN | Both ] */
  {"dr_lan_tx", "0", 0},	/* Dynamic-Routing LAN out */
  {"dr_lan_rx", "0", 0},	/* Dynamic-Routing LAN in  */
  {"dr_wan_tx", "0", 0},	/* Dynamic-Routing WAN out */
  {"dr_wan_rx", "0", 0},	/* Dynamic-Routing WAN in  */

  /* for mac clone */
  {"mac_clone_enable", "0", 0},	/* User define WAN interface MAC address */
  {"def_hwaddr", "00:00:00:00:00:00", 0},	/* User define WAN interface MAC address */

  /* for DDNS */
  /* for dyndns */
  {"ddns_enable", "0", 0},	/* 0:Disable 1:dyndns 2:tzo */
  {"ddns_username", "", 0},	/* DynDNS Username */
  {"ddns_passwd", "", 0},	/* DynDNS Password */
  {"ddns_hostname", "", 0},	/* DynDNS Hostname */
  {"ddns_dyndnstype", "", 0},	/* DynDNS Type */
  {"ddns_wildcard", "", 0},	/* DynDNS Wildcard */
  /* for tzo */
  {"ddns_username_2", "", 0},	/* TZO Email Address */
  {"ddns_passwd_2", "", 0},	/* TZO Password Key */
  {"ddns_hostname_2", "", 0},	/* TZO Domain Name */
  /* for last value */
  {"ddns_enable_buf", "", 0},	/* 0:Disable 1:Eanble */
  {"ddns_username_buf", "", 0},	/* DDNS username */
  {"ddns_passwd_buf", "", 0},	/* DDNS password */
  {"ddns_hostname_buf", "", 0},	/* DDNS hostname */

  {"ddns_status", "", 0},	/* DDNS status */
  {"ddns_interval", "60", 0},	/* DDNS timer interval in second */
  {"ddns_cache", "", 0},	/* DDNS cache data */

  /* for AOL */
  {"aol_block_traffic", "0", 0},	/* 0:Disable 1:Enable for global */
  {"aol_block_traffic1", "0", 0},	/* 0:Disable 1:Enable for "ppp_username" */
  {"aol_block_traffic2", "0", 0},	/* 0:Disable 1:Enable for "Parental control" */
  {"skip_amd_check", "0", 0},	/* 0:Disable 1:Enable */
  {"skip_intel_check", "0", 0},	/* 0:Disable 1:Enable */

  {"l2tp_server_ip", "", 0},	/* L2TP auth server (IP Address) */
  {"l2tp_get_ip", "", 0},	/* IP Address assigned by L2TP server */
  {"wan_gateway_buf", "0.0.0.0", 0},	/* save the default gateway for DHCP */


  {"hb_server_ip", "", 0},	/* heartbeat auth server (IP Address) */
  {"hb_server_domain", "", 0},	/* heartbeat auth server (domain name) */
  {"samba_mount", "0", 0},
  {"samba_share", "//yourserverip/yourshare", 0},
  {"samba_user", "username/computer", 0},
  {"samba_password", "iwer573495u7340", 0},
  {"samba_script", "yourscript", 0},
  {"rflow_enable", "0", 0},
  {"status_auth", "1", 0},
  {"macupd_enable", "0", 0},
  {"wl_radauth", "0", 0},
  /* Sveasoft added defaults */
  {"expert_mode", "0", 0},
  {"rc_startup", "", 0},
  {"rc_firewall", "", 0},
  {"rc_shutdown", "", 0},
#ifdef HAVE_POWERNOC
  {"txpwr", "200", 0},
#elif HAVE_SKYTRON
  {"txpwr", "251", 0},
#elif HAVE_SAGAR
  {"txpwr", "100", 0},
#else
  {"txpwr", "28", 0},
#endif
  {"txant", "3", 0},
  {"wl_antdiv", "3", 0},
  {"apwatchdog_enable", "0", 0},
  {"apwatchdog_interval", "15", 0},
  {"boot_wait", "on", 0},
#ifdef HAVE_SKYTEL
  {"cron_enable", "0", 0},
#else
  {"cron_enable", "1", 0},
#endif
  {"dhcpd_options", "", 0},
  {"local_dns", "0", 0},
#ifdef HAVE_POWERNOC_WOAP54G
  {"dnsmasq_enable", "0", 0},
#elif HAVE_FON
  {"dnsmasq_enable", "0", 0},
#else
  {"dnsmasq_enable", "1", 0},
#endif
  {"dnsmasq_options", "", 0},
  {"httpd_enable", "1", 0},
#ifdef HAVE_HTTPS
  {"httpsd_enable", "1", 0},
#endif
#ifdef HAVE_POWERNOC_WOAP54G
  {"loopback_enable", "0", 0},
#else
  {"loopback_enable", "1", 0},
#endif
#ifdef HAVE_POWERNOC_WOAP54G
  {"nas_enable", "0", 0},
#else
  {"nas_enable", "1", 0},
#endif
  {"ntp_enable", "1", 0},
  {"pptpd_enable", "0", 0},
  {"pptpd_lip", "", 0},
  {"pptpd_rip", "", 0},
  {"pptpd_auth", "", 0},
  {"pptp_encrypt", "0", 0},
  {"resetbutton_enable", "1", 0},
#ifdef HAVE_SKYTRON
  {"telnetd_enable", "0", 0},
#else
  {"telnetd_enable", "1", 0},
#endif
  {"ipv6_enable", "0", 0},
  {"radvd_enable", "0", 0},
  {"radvd_conf", "", 0},
#ifdef HAVE_FONBETA
  {"chilli_enable", "1", 0},
  {"chilli_url", "https://beta.fon.com/test2/index.php", 0},
  {"chilli_radius", "beta.fon.com", 0},
  {"chilli_backup", "beta.fon.com", 0},
  {"chilli_pass", "garrafon", 0},
  {"chilli_dns1", "0.0.0.0", 0},
  {"chilli_interface", "wlan", 0},
  {"chilli_radiusnasid", "", 0},
  {"chilli_uamsecret", "garrafon", 0},
  {"chilli_uamanydns", "1", 0},
  {"chilli_uamallowed",
   "www.fon.com,acceso.fon.com,en.fon.com,es.fon.com,www.paypal.com,www.paypalobjects.com",
   0},
  {"chilli_macauth", "", 0},
  {"chilli_additional", "", 0},
#elif HAVE_FON
  {"chilli_enable", "1", 0},
  {"chilli_url", "https://login.fon.com/cp/index.php", 0},
  {"chilli_radius", "emilio.fon.com", 0},
  {"chilli_backup", "emilio.fon.com", 0},
  {"chilli_pass", "garrafon", 0},
  {"chilli_dns1", "0.0.0.0", 0},
  {"chilli_interface", "wlan", 0},
  {"chilli_radiusnasid", "", 0},
  {"chilli_uamsecret", "garrafon", 0},
  {"chilli_uamanydns", "1", 0},
  {"chilli_uamallowed",
   "www.fon.com,acceso.fon.com,en.fon.com,es.fon.com,www.paypal.com,www.paypalobjects.com",
   0},
  {"chilli_macauth", "", 0},
  {"chilli_additional", "", 0},
#else
  {"chilli_enable", "0", 0},
  {"chilli_url", "", 0},
  {"chilli_radius", "0.0.0.0", 0},
  {"chilli_backup", "0.0.0.0", 0},
  {"chilli_pass", "", 0},
  {"chilli_dns1", "0.0.0.0", 0},
  {"chilli_interface", "wan", 0},
  {"chilli_radiusnasid", "", 0},
  {"chilli_uamsecret", "", 0},
  {"chilli_uamanydns", "0", 0},
  {"chilli_uamallowed", "", 0},
  {"chilli_macauth", "", 0},
  {"chilli_additional", "", 0},
#endif

#ifdef HAVE_SKYTEL
  {"sshd_enable", "0", 0},
#elif HAVE_34TELECOM
  {"sshd_enable", "1", 0},
#elif HAVE_POWERNOC
  {"sshd_enable", "1", 0},
#elif HAVE_SAGAR
  {"sshd_enable", "1", 0},
#elif HAVE_SKYTRON
  {"sshd_enable", "1", 0},
#else
  {"sshd_enable", "0", 0},
#endif
  {"sshd_port", "22", 0},
  {"sshd_passwd_auth", "1", 0},
  {"sshd_rsa_host_key", "", 0},
  {"sshd_dss_host_key", "", 0},
  {"sshd_authorized_keys", "", 0},
  {"syslogd_enable", "0", 0},
  {"syslogd_rem_ip", "", 0},

  {"wl_wds1_enable", "0", 0},
  {"wl_wds2_enable", "0", 0},
  {"wl_wds3_enable", "0", 0},
  {"wl_wds4_enable", "0", 0},
  {"wl_wds5_enable", "0", 0},
  {"wl_wds6_enable", "0", 0},
  {"wl_wds7_enable", "0", 0},
  {"wl_wds8_enable", "0", 0},
  {"wl_wds9_enable", "0", 0},
  {"wl_wds10_enable", "0", 0},

  {"wl_wds1_hwaddr", "", 0},
  {"wl_wds2_hwaddr", "", 0},
  {"wl_wds3_hwaddr", "", 0},
  {"wl_wds4_hwaddr", "", 0},
  {"wl_wds5_hwaddr", "", 0},
  {"wl_wds6_hwaddr", "", 0},
  {"wl_wds7_hwaddr", "", 0},
  {"wl_wds8_hwaddr", "", 0},
  {"wl_wds9_hwaddr", "", 0},
  {"wl_wds10_hwaddr", "", 0},

  {"wl_wds1_ipaddr", "", 0},
  {"wl_wds2_ipaddr", "", 0},
  {"wl_wds3_ipaddr", "", 0},
  {"wl_wds4_ipaddr", "", 0},
  {"wl_wds5_ipaddr", "", 0},
  {"wl_wds6_ipaddr", "", 0},
  {"wl_wds7_ipaddr", "", 0},
  {"wl_wds8_ipaddr", "", 0},
  {"wl_wds9_ipaddr", "", 0},
  {"wl_wds10_ipaddr", "", 0},

  {"wl_wds1_netmask", "", 0},
  {"wl_wds2_netmask", "", 0},
  {"wl_wds3_netmask", "", 0},
  {"wl_wds4_netmask", "", 0},
  {"wl_wds5_netmask", "", 0},
  {"wl_wds6_netmask", "", 0},
  {"wl_wds7_netmask", "", 0},
  {"wl_wds8_netmask", "", 0},
  {"wl_wds9_netmask", "", 0},
  {"wl_wds10_netmask", "", 0},

  {"wl_wds1_desc", "", 0},
  {"wl_wds2_desc", "", 0},
  {"wl_wds3_desc", "", 0},
  {"wl_wds4_desc", "", 0},
  {"wl_wds5_desc", "", 0},
  {"wl_wds6_desc", "", 0},
  {"wl_wds7_desc", "", 0},
  {"wl_wds8_desc", "", 0},
  {"wl_wds9_desc", "", 0},
  {"wl_wds10_desc", "", 0},

  {"wl_wds1_ospf", "", 0},
  {"wl_wds2_ospf", "", 0},
  {"wl_wds3_ospf", "", 0},
  {"wl_wds4_ospf", "", 0},
  {"wl_wds5_ospf", "", 0},
  {"wl_wds6_ospf", "", 0},
  {"wl_wds7_ospf", "", 0},
  {"wl_wds8_ospf", "", 0},
  {"wl_wds9_ospf", "", 0},
  {"wl_wds10_ospf", "", 0},

  {"wl_br1_enable", "0", 0},
  {"wl_br1_nat", "0", 0},

  {"wl0_wds", "", 0},
  {"wl0_wds0", "", 0},
  {"wl0_wds1", "", 0},
  {"wl0_wds2", "", 0},
  {"wl0_wds3", "", 0},
  {"wl0_wds4", "", 0},
  {"wl0_wds5", "", 0},
  {"wl0_wds6", "", 0},
  {"wl0_wds7", "", 0},
  {"wl0_wds8", "", 0},
  {"wl0_wds9", "", 0},
  {"wl_wds_if1", "", 0},
  {"wl_wds_if2", "", 0},
  {"wl_wds_if3", "", 0},
  {"wl_wds_if4", "", 0},
  {"wl_wds_if5", "", 0},
  {"wl_wds_if6", "", 0},
  {"wl_wds_if7", "", 0},
  {"wl_wds_if8", "", 0},
  {"wl_wds_if9", "", 0},
  {"wl_wds_if10", "", 0},
  {"wl_wds_if11", "", 0},
  {"wl_wds_if12", "", 0},
  {"wl_wds_if13", "", 0},
  {"wl_wds_if14", "", 0},
  {"wl_wds_if15", "", 0},
  {"wl_wds_if16", "", 0},

  {"wds0.49150", "", 0},
  {"wds0.49151", "", 0},
  {"wds0.49152", "", 0},
  {"wds0.49153", "", 0},
  {"wds0.49154", "", 0},
  {"wds0.49155", "", 0},
  {"wds0.49156", "", 0},
  {"wds0.49157", "", 0},
  {"wds0.49158", "", 0},
  {"wds0.49159", "", 0},
  {"wds0.491510", "", 0},
  {"wds0.491511", "", 0},
  {"wds0.491512", "", 0},
  {"wds0.491513", "", 0},
  {"wds0.491514", "", 0},
  {"wds0.491515", "", 0},

  {"bird_ospf",
   "Please read the BIRD setup instructions at http://bird.network.cz/bird.html",
   0},

  {"sh_interfaces", "loc br0\nloc eth0\nloc eth2\nnet wds0.2", 0},
  {"sh_masq", "br0 wds0.2", 0},
  {"sh_policy", "loc net ACCEPT\nnet all DROP info\nall all REJECT info", 0},
  {"sh_routestopped", "br0 -\neth0 -\neth2 -\nwds0.2 -", 0},
  {"sh_rules", "", 0},
  {"sh_zones", "net Net Internet\nloc Local Local Networks\ndmz DMZ Dmz Zone",
   0},

#ifdef HAVE_SKYTRON
  {"wshaper_enable", "1", 0},
#else
  {"wshaper_enable", "0", 0},
#endif
  {"wshaper_dev", "WAN", 0},
#ifdef HAVE_SKYTRON
  {"wshaper_downlink", "800", 0},
  {"wshaper_uplink", "800", 0},
#else
  {"wshaper_downlink", "0", 0},
  {"wshaper_uplink", "0", 0},
#endif
  {"wshaper_nopriohostsrc", "", 0},
  {"wshaper_nopriohostdst", "", 0},
  {"wshaper_noprioportsrc", "", 0},
  {"wshaper_noprioportdst", "", 0},
  {"zebra_enable", "1", 0},

  {"svqos_svcs", "", 0},
  {"svqos_ips", "", 0},
  {"svqos_macs", "", 0},

  {"svqos_port1bw", "full", 0},
  {"svqos_port2bw", "full", 0},
  {"svqos_port3bw", "full", 0},
  {"svqos_port4bw", "full", 0},

  {"svqos_port1prio", "10", 0},
  {"svqos_port2prio", "10", 0},
  {"svqos_port3prio", "10", 0},
  {"svqos_port4prio", "10", 0},
#ifdef HAVE_SAGAR
  {"snmpd_enable", "1", 0},
#else
  {"snmpd_enable", "0", 0},
#endif
  {"snmpd_syslocation", "Unknown", 0},
  {"snmpd_syscontact", "root", 0},
#ifdef CONFIG_BRANDING
  {"snmpd_sysname", "anonymous", 0},
#else
  {"snmpd_sysname", "dd-wrt", 0},
#endif
  {"snmpd_rocommunity", "public", 0},
  {"snmpd_rwcommunity", "private", 0},
  {"snmpd_conf", "See http://www.net-snmp.org for expert snmpd.conf options",
   0},

  {"wol_enable", "0", 0},
  {"wol_interval", "86400", 0},
  {"wol_hostname", "", 0},
  {"wol_macs", "", 0},
  {"wol_passwd", "", 0},

  {"hs_enable", "", 0},
  {"hs_exempt", "", 0},
  {"hs_urls", "", 0},
  {"hs_redirect", "", 0},
  {"hs_html", "", 0},
  {"hs_image", "", 0},

  {"def_whwaddr", "00:00:00:00:00:00", 0},	/* User define wireless interface MAC address */

  {"sv_restore_defaults", "0", 0},	// fix for vlan stuff side effects

  {"ospfd_conf", "", 0},
  {"zebra_conf", "", 0},
  {"ospfd_copt", "0", 0},
  {"zebra_copt", "0", 0},
  {"zebra_log", "0", 0},
  {"dyn_default", "0", 0},

  {"altdns1", "", 0},
  {"altdns2", "", 0},
  {"altdns3", "", 0},

  {"log_accepted", "0", 0},	/* 0:Disable 1:Eanble */
  {"log_dropped", "0", 0},	/* 0:Disable 1:Eanble */
  {"log_rejected", "0", 0},	/* 0:Disable 1:Eanble */
  /* end Sveasoft added defaults */

  /* start lonewolf mods */
  {"port0vlans", "1", 0},
  {"port1vlans", "0", 0},
  {"port2vlans", "0", 0},
  {"port3vlans", "0", 0},
  {"port4vlans", "0", 0},
  {"port5vlans", "0 1 16", 0},
  {"vlans", "0", 0},
  {"trunking", "0", 0},
  /* end lonewolf mods */
  //DD-WRT start
  {"manual_boot_nv", "0", 0},
#ifdef HAVE_WTS
  {"status_auth", "0", 0},
#else
  {"status_auth", "1", 0},
#endif
  {"ipv6_enable", "0", 0},
  {"ipv6_enable0", "0", 0},
  {"enable_jffs2", "0", 0},
  {"clean_jffs2", "0", 0},
  {"kaid_enable", "0", 0},
  {"kaid_macs", "", 0},
  {"language", "english", 0},
  {"macupd_ip", "0.0.0.0", 0},
  {"macupd_port", "0", 0},
  {"macupd_interval", "10", 0},
  {"mmc_enable", "0", 0},
  {"mmc_enable0", "0", 0},
  {"ip_conntrack_max", "512", 0},
  {"ip_conntrack_tcp_timeouts", "3600", 0},
  {"ip_conntrack_udp_timeouts", "3600", 0},
  {"rflow_ip", "0.0.0.0", 0},
  {"rflow_port", "0", 0},
  {"rflow_if", "br0", 0},
  {"pppoe_ver", "0", 0},
  {"schedule_enable", "0", 0},
  {"schedule_time", "1", 0},
  {"schedule_hour_time", "1", 0},
  {"schedule_minutes", "00", 0},
  {"schedule_hours", "00", 0},
  {"schedule_weekdays", "00", 0},
  {"smtp_redirect_enable", "0", 0},
  {"smtp_redirect_destination", "0.0.0.0", 0},
  {"smtp_source_network", "0.0.0.0", 0},
  {"wds_watchdog_enable", "0", 0},
  {"wds_watchdog_interval_sec", "1000", 0},
  {"wds_watchdog_ips", "", 0},
  {"dhcpfwd_enable", "0", 0},
  {"dhcpfwd_ip", "0.0.0.0", 0},
  {"NC_enable", "0", 0},
#ifdef CONFIG_BRANDING
  {"NC_GatewayName", "GATEWAY", 0},
#else
  {"NC_GatewayName", "DD-WRT", 0},
#endif
#ifdef CONFIG_BRANDING
  {"NC_HomePage", "", 0},
#else
  {"NC_HomePage", "http://www.dd-wrt.com", 0},
#endif
  {"NC_ExcludePorts", "25", 0},
  {"NC_Verbosity", "0", 0},
  {"NC_LoginTimeout", "86400", 0},
#ifdef CONFIG_BRANDING
  {"NC_AllowedWebHosts", "google.com", 0},
#else
  {"NC_AllowedWebHosts", "dd-wrt.com", 0},
#endif
  {"NC_RouteOnly", "0", 0},
  {"NC_DocumentRoot", "/jffs/nocat", 0},
  {"NC_SplashURL", "http://externalsite", 0},
  {"wl_wme", "off", 0},		/* WME mode (off|on) */
  /* WME parameters */
  /* EDCA parameters for STA */
  {"wl_wme_sta_bk", "15 1023 7 0 0 off", 0},	/* WME STA AC_BK paramters */
  {"wl_wme_sta_be", "15 1023 3 0 0 off", 0},	/* WME STA AC_BE paramters */
  {"wl_wme_sta_vi", "7 15 2 6016 3008 off", 0},	/* WME STA AC_VI paramters */
  {"wl_wme_sta_vo", "3 7 2 3264 1504 off", 0},	/* WME STA AC_VO paramters */

  /* EDCA parameters for AP */
  {"wl_wme_ap_bk", "15 1023 7 0 0 off", 0},	/* WME AP AC_BK paramters */
  {"wl_wme_ap_be", "15 63 3 0 0 off", 0},	/* WME AP AC_BE paramters */
  {"wl_wme_ap_vi", "7 15 1 6016 3008 off", 0},	/* WME AP AC_VI paramters */
  {"wl_wme_ap_vo", "3 7 1 3264 1504 off", 0},	/* WME AP AC_VO paramters */

  {"wl_wme_no_ack", "off", 0},	/* WME No-Acknowledgmen mode */
  {"wl_maxassoc", "128", 0},	/* Max associations driver could support */
#ifdef HAVE_ZEROIP
  {"shat_enable", "0", 0},
  {"shat_range", "192.168.2.96/27", 0},
  {"shat_shield", "NO", 0},
#endif
#ifdef HAVE_SKYTROM
  {"dhcp_dnsmasq", "1", 0},
  {"enable_game", "1", 0},
#elif HAVE_POWERNOC
  {"dhcp_dnsmasq", "1", 0},
  {"enable_game", "0", 0},
//#elif HAVE_FON
//  {"dhcp_dnsmasq", "1", 0},
//  {"enable_game", "0", 0},
#elif HAVE_WTS
  {"dhcp_dnsmasq", "1", 0},
  {"enable_game", "0", 0},
#else
  {"dhcp_dnsmasq", "0", 0},
  {"enable_game", "0", 0},
#endif

  {"pptp_use_dhcp", "0", 0},	/* pptp will use dhcp to obtain ip address, netmask and gateway */
  {"forward_entries", "0", 0},
  {"forwardspec_entries", "0", 0},
  {"trigger_entries", "0", 0},
#ifdef HAVE_SKYTRON
  {"sip_port", "5060", 0},
  {"sip_domain", "sip.skytron.de", 0},
#else
  {"sip_port", "5060", 0},
  {"sip_domain", "", 0},
#endif
#ifdef HAVE_AQOS
  {"default_level", "5000", 0},	//set a useful value to prevent deadlock
#endif
  {"static_leases", "", 0},
  {"static_leasenum", "0", 0},
  {"dhcpc_vendorclass", "", 0},	//vendor class id for client (optional)
  {"maskmac", "1", 0},
  {"fullswitch", "0", 0},
#ifdef HAVE_OPENVPN
  {"openvpn_enable", "0", 0},
  {"openvpn_remoteip", "0.0.0.0", 0},
  {"openvpn_remoteport", "1194", 0},
  {"openvpn_ca", "", 0},
  {"openvpn_client", "", 0},
  {"openvpn_key", "", 0},
  {"openvpn_lzo", "0", 0},
  {"openvpn_proto", "udp", 0},
  {"openvpn_mtu", "1500", 0},
  {"openvpn_extramtu", "32", 0},
  {"openvpn_mssfix", "1450", 0},
#endif
#ifdef HAVE_NEWMEDIA
  {"openvpn_config", "", 0},
  {"openvpn_dh", "", 0},
  {"openvpn_tlsauth", "", 0},
  {"openvpn_onwan", "0", 0},
  {"newhttp_username", "", 0},
  {"newhttp_passwd", "nmn4711", 0},
  {"ral", "212.65.2.116 194.231.229.20", 0},
#endif
#ifdef HAVE_34TELECOM
  {"newhttp_passwd", "hdslklas9a", 0},
#endif
#ifdef HAVE_MADWIFI
  {"ath0_turbo", "1", 0},
  {"ath1_turbo", "1", 0},
  {"ath2_turbo", "1", 0},
  {"ath3_turbo", "1", 0},
  {"ath4_turbo", "1", 0},
  {"ath5_turbo", "1", 0},

  {"ath0_regdomain", "96", 0},
  {"ath1_regdomain", "96", 0},
  {"ath2_regdomain", "96", 0},
  {"ath3_regdomain", "96", 0},
  {"ath4_regdomain", "96", 0},
  {"ath5_regdomain", "96", 0},

#endif
#ifdef HAVE_FON
  {"fon_usernames", "0", 0},
  {"fon_userlist", "", 0},
#endif
  {0, 0, 0}
};

#ifdef HAVE_SKYTEL
#undef HAVE_POWERNOC_WORT54G
#undef HAVE_POWERNOC
#endif
