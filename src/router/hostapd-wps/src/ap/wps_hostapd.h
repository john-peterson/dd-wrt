/*
 * hostapd / WPS integration
 * Copyright (c) 2008, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef WPS_HOSTAPD_H
#define WPS_HOSTAPD_H

#ifdef CONFIG_WPS

int hostapd_init_wps(struct hostapd_data *hapd,
		     struct hostapd_bss_config *conf);
void hostapd_deinit_wps(struct hostapd_data *hapd);
void hostapd_update_wps(struct hostapd_data *hapd);
int hostapd_wps_add_pin(struct hostapd_data *hapd, const char *uuid,
			const char *pin, int timeout);
int hostapd_wps_button_pushed(struct hostapd_data *hapd);
int hostapd_wps_start_oob(struct hostapd_data *hapd, char *device_type,
			  char *path, char *method, char *name);
int hostapd_wps_get_mib_sta(struct hostapd_data *hapd, const u8 *addr,
			    char *buf, size_t buflen);

#else /* CONFIG_WPS */

static inline int hostapd_init_wps(struct hostapd_data *hapd,
				   struct hostapd_bss_config *conf)
{
	return 0;
}

static inline void hostapd_deinit_wps(struct hostapd_data *hapd)
{
}

static inline void hostapd_update_wps(struct hostapd_data *hapd)
{
}

static inline int hostapd_wps_get_mib_sta(struct hostapd_data *hapd,
					  const u8 *addr,
					  char *buf, size_t buflen)
{
	return 0;
}

static inline int hostapd_wps_button_pushed(struct hostapd_data *hapd)
{
	return 0;
}

#endif /* CONFIG_WPS */

#endif /* WPS_HOSTAPD_H */
