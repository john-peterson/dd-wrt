/*
 * proftp.c
 *
 * Copyright (C) 2008 dd-wrt
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
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
#ifdef HAVE_SAMBA3
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>
#include <utils.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <services.h>
#include <samba3.h>

void start_samba3(void)
{
	struct samba3_share *cs, *csnext;
	struct samba3_shareuser *csu, *csunext;
	struct samba3_user *samba3users, *cu, *cunext;
	struct samba3_share *samba3shares;
	FILE *fp;
	if (!nvram_match("samba3_enable", "1"))
		return;

	sysprintf
	    ("grep -q nobody /etc/passwd || echo \"nobody:*:65534:65534:nobody:/var:/bin/false\" >> /etc/passwd");
	sysprintf("mkdir -p /var/samba");
	sysprintf("touch /var/samba/smbpasswd");
	if (nvram_match("samba3_advanced", "1")) {
		write_nvram("/tmp/smb.conf", "samba3_conf");
	} else {
		samba3users = getsamba3users();
		for (cu = samba3users; cu; cu = cunext) {
			if (strlen(cu->username)) {
				sysprintf
				    ("grep -q \"%s\" /etc/passwd || echo \"%s\"\":*:1000:1000:\"%s\":/var:/bin/false\" >> /etc/passwd",
				     cu->username, cu->username, cu->username);
				sysprintf("smbpasswd \"%s\" \"%s\"",
					  cu->username, cu->password);
			}
			cunext = cu->next;
			free(cu);

		}
		fp = fopen("/tmp/smb.conf", "wb");
		fprintf(fp,
			"[global]\n"
			"server string = %s\n"
			"workgroup = %s\n"
			"bind interfaces only = Yes\n"
			"map to guest = Bad User\n"
			"smb passwd file = /var/samba/smbpasswd\n"
			"private dir = /var/samba\n"
			"passdb backend = smbpasswd\n"
			"log file = /var/smbd.log\n"
			"max log size = 1000\n"
			"socket options = TCP_NODELAY\n"
			"printing = none\n"
			"load printers = No\n"
			"usershare allow guests = Yes\n",
			nvram_safe_get("samba3_srvstr"),
			nvram_safe_get("samba3_workgrp"));

		samba3shares = getsamba3shares();
		for (cs = samba3shares; cs; cs = csnext) {
			fprintf(stderr, "add share %s\n", cs->label);
			if (strlen(cs->label)) {
				fprintf(fp, "[%s]\n", cs->label);
				fprintf(fp, "comment = %s\n", cs->label);
				fprintf(fp, "path = %s\n", cs->mp);
				fprintf(fp, "read only = %s\n",
					!strcmp(cs->access_perms,
						"ro") ? "Yes" : "No");
				fprintf(fp, "guest ok = %s\n",
					cs->public == 1 ? "Yes" : "No");
				fprintf(fp, "valid users =");
				int first = 0;
				for (csu = cs->users; csu; csu = csunext) {
					fprintf(stderr, "permitt user %s\n",
						csu->username);
					if (first)
						fprintf(fp, ",");
					first = 1;
					fprintf(fp, " %s", csu->username);
					csunext = csu->next;
					free(csu);
				}
				free(cs->users);
				fprintf(fp, "\n");
				fprintf(fp, "force user = root\n");
			}
			csnext = cs->next;
			free(cs);
		}
		fclose(fp);
	}
	sysprintf("chmod 777 /jffs\n");
	eval("/usr/sbin/nmbd", "-D", "--configfile=/tmp/smb.conf");
	eval("/usr/sbin/smbd", "-D", "--configfile=/tmp/smb.conf");
	syslog(LOG_INFO, "Samba3 : samba started\n");
	return;
}

void stop_samba3(void)
{
	stop_process("smbd", "samba");
	stop_process("nmbd", "nmbd");
}
#endif

#ifdef TEST
void main(int argc, char *argv[])
{
	start_samba3();
}
#endif
