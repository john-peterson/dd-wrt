/*
 * Broadcom wireless network adapter utility functions
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: wlutils.h,v 1.3 2005/11/11 09:26:19 seg Exp $
 */

#ifndef _wlutils_h_
#define _wlutils_h_

#include <typedefs.h>
#include <wlioctl.h>
#ifndef WLC_IOCTL_SMLEN
#define	WLC_IOCTL_SMLEN		256		/* "small" length ioctl buffer required */
#endif
#ifndef BCME_BUFTOOSHORT
#define BCME_BUFTOOSHORT		-14	/* Buffer too short */
#endif

/* Sveasoft addition - get wireless interface */
extern char *get_wdev (void);

/*
 * Pass a wlioctl request to the specified interface.
 * @param	name	interface name
 * @param	cmd	WLC_GET_MAGIC <= cmd < WLC_LAST
 * @param	buf	buffer for passing in and/or receiving data
 * @param	len	length of buf
 * @return	>= 0 if successful or < 0 otherwise
 */
extern int wl_ioctl (char *name, int cmd, void *buf, int len);

/*
 * Get the MAC (hardware) address of the specified interface.
 * @param	name	interface name
 * @param	hwaddr	6-byte buffer for receiving address
 * @return	>= 0 if successful or < 0 otherwise
 */
extern int wl_hwaddr (char *name, unsigned char *hwaddr);

/*
 * Probe the specified interface.
 * @param	name	interface name
 * @return	>= 0 if a Broadcom wireless device or < 0 otherwise
 */
extern int wl_probe (char *name);


/* Returns the list of associated stations in the pre-existing buffer list */
int getassoclist (char *name, unsigned char *list);

/*
 * Set/Get named variable.
 * @param	name	interface name
 * @param	var	variable name
 * @param	val	variable value/buffer
 * @param	len	variable value/buffer length
 * @return	success == 0, failure != 0
 */
extern int wl_set_val (char *name, char *var, void *val, int len);
extern int wl_get_val (char *name, char *var, void *val, int len);
extern int wl_set_int (char *name, char *var, int val);
extern int wl_get_int (char *name, char *var, int *val);


/*
 * Set/Get named variable.
 * @param	ifname		interface name
 * @param	iovar		variable name
 * @param	param		input param value/buffer
 * @param	paramlen	input param value/buffer length
 * @param	bufptr		io buffer
 * @param	buflen		io buffer length
 * @param	val		val or val pointer for int routines
 * @return	success == 0, failure != 0
 */
extern int wl_iovar_setbuf (char *ifname, char *iovar, void *param,
			    int paramlen, void *bufptr, int buflen);
extern int wl_iovar_getbuf (char *ifname, char *iovar, void *param,
			    int paramlen, void *bufptr, int buflen);
extern int wl_iovar_set (char *ifname, char *iovar, void *param,
			 int paramlen);
extern int wl_iovar_get (char *ifname, char *iovar, void *bufptr, int buflen);
extern int wl_iovar_setint (char *ifname, char *iovar, int val);


//extern int wl_iovar_getint (char *ifname, char *iovar, int *val);

extern int wl_bssiovar_setbuf (char *ifname, char *iovar, int bssidx,
			       void *param, int paramlen, void *bufptr,
			       int buflen);
extern int wl_bssiovar_getbuf (char *ifname, char *iovar, int bssidx,
			       void *param, int paramlen, void *bufptr,
			       int buflen);
extern int wl_bssiovar_get (char *ifname, char *iovar, int bssidx,
			    void *outbuf, int len);
extern int wl_bssiovar_set (char *ifname, char *iovar, int bssidx,
			    void *param, int paramlen);
extern int wl_bssiovar_setint (char *ifname, char *iovar, int bssidx,
			       int val);


#endif /* _wlutils_h_ */
