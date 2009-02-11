/*
 * Broadcom UPnP module, InternetGatewayDevice.h
 *
 * Copyright (C) 2008, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: InternetGatewayDevice.h,v 1.8 2008/08/25 08:16:14 Exp $
 */
#ifndef __INTERNETGATEWAYDEVICE_H__
#define __INTERNETGATEWAYDEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/* Control structure */
#define UPNP_PM_SIZE        32
#define UPNP_PM_DESC_SIZE   256

typedef struct upnp_portmap {
    char            remote_host[sizeof("255.255.255.255")];
    unsigned short  external_port;
    char            protocol[8];
    unsigned short  internal_port;
    char            internal_client[sizeof("255.255.255.255")];
    unsigned int    enable;
    char            description[UPNP_PM_DESC_SIZE];
    unsigned long   duration;
	unsigned long	book_time;
} UPNP_PORTMAP;

typedef	struct upnp_portmap_ctrl {
	unsigned long	pm_seconds;
	int	num;
	int	limit;
 	UPNP_PORTMAP	pmlist[1];
} UPNP_PORTMAP_CTRL;
#define UPNP_PORTMAP_CTRL_SIZE (sizeof(UPNP_PORTMAP_CTRL) - sizeof(UPNP_PORTMAP)) 


/* Functions */
int upnp_portmap_add
(
	UPNP_CONTEXT	*context,
	char            *remote_host,
	unsigned short  external_port,
	char            *protocol,
	unsigned short  internal_port,
	char            *internal_client,
	unsigned int    enable,
	char            *description,
	unsigned long   duration
);

int upnp_portmap_del
(
	UPNP_CONTEXT	*context,
	char            *remote_host,
	unsigned short  external_port,
	char            *protocol
);

UPNP_PORTMAP *upnp_portmap_find
(
	UPNP_CONTEXT	*context,
	char            *remote_host,
	unsigned short  external_port,
	char            *protocol
);

unsigned short	upnp_portmap_num(UPNP_CONTEXT *context);
UPNP_PORTMAP    *upnp_portmap_with_index(UPNP_CONTEXT *context, int index);

/* OSL dependent function */
struct _if_stats;

extern int upnp_osl_igd_status();
extern int upnp_osl_wan_ifstats(struct _if_stats *);
extern int upnp_osl_wan_link_status();
extern unsigned int upnp_osl_wan_max_bitrates(unsigned long *rx, unsigned long *tx);
extern int upnp_osl_wan_ip(struct in_addr *inaddr);
extern int upnp_osl_wan_isup();
extern int upnp_osl_wan_uptime();
extern void upnp_osl_nat_config(UPNP_PORTMAP *map);

/* << TABLE BEGIN */
/*
 * WARNNING: DON'T MODIFY THE FOLLOWING TABLES
 * AND DON'T REMOVE TAG :
 *          "<< TABLE BEGIN"
 *          ">> TABLE END"
 */

extern UPNP_DEVICE InternetGatewayDevice;

int InternetGatewayDevice_common_init(UPNP_CONTEXT *context);
int InternetGatewayDevice_open(UPNP_CONTEXT *context);
int InternetGatewayDevice_close(UPNP_CONTEXT *context);
int InternetGatewayDevice_request(UPNP_CONTEXT *context, void *cmd);
int InternetGatewayDevice_timeout(UPNP_CONTEXT *context, time_t now);
int InternetGatewayDevice_notify(UPNP_CONTEXT *context, UPNP_SERVICE *service);
/* >> TABLE END */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTERNETGATEWAYDEVICE_H__ */
