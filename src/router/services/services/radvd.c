/*
 * radvd.c
 *
 * Copyright (C) 2009 Sebastian Gottschall <gottschall@dd-wrt.com>
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
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>		/* AhMan March 18 2005 */
#include <sys/socket.h>
#include <sys/mount.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <wait.h>
#include <net/route.h>		/* AhMan March 18 2005 */
#include <sys/types.h>
#include <signal.h>

#include <bcmnvram.h>
#include <bcmconfig.h>
#include <netconf.h>
#include <shutils.h>
#include <utils.h>
#include <cy_conf.h>
#include <code_pattern.h>
#include <rc.h>
#include <wlutils.h>
#include <nvparse.h>
#include <syslog.h>
#include <services.h>

#ifdef HAVE_RADVD
void start_radvd(void)
{
	int ret = 0;
	int c = 0;
	char *buf, *buf2;
	int i;
	FILE *fp;

	if (!nvram_match("radvd_enable", "1"))
		return;
	if (!nvram_match("ipv6_enable", "1"))
		return;
	buf = nvram_get("radvd_conf");
	if (buf != NULL)
		writenvram("radvd_conf", "/tmp/radvd.conf");

	system2("sync");

	ret = eval("radvd", "-C", "/tmp/radvd.conf");
	dd_syslog(LOG_INFO, "radvd : RADV daemon successfully started\n");

	cprintf("done\n");
	return;
}

void stop_radvd(void)
{

	stop_process("radvd", "RADV daemon");
	unlink("/var/run/radvd.pid");

	cprintf("done\n");
	return;
}
#endif
