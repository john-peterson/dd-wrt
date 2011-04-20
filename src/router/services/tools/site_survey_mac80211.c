/*
 * site_survey_madwifi.c
 *
 * Copyright (C) 2011 Christian Scheele <chris@dd-wrt.com>
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

#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <getopt.h>
#include <err.h>
#include <shutils.h>

#include <typedefs.h>
#include <bcmnvram.h>


int site_survey_main_mac802211(int argc, char *argv[]) {
	unlink(SITE_SURVEY_DB);
	char *sta = nvram_safe_get("wifi_display");
	if (!nvram_nmatch("disabled", "%s_net_mode", sta
		&& (nvram_nmatch("ap", "%s_mode", sta))
		|| nvram_nmatch("wdsap", "%s_mode", sta))) {
		mac80211_site_survey(sta,1);
	}
	else {
		mac80211_site_survey(sta,0);
	}
	return 0;
}
