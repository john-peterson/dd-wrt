/*
 * backup.c
 *
 * Copyright (C) 2010 Sebastian Gottschall <gottschall@dd-wrt.com>
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
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <syslog.h>

#include <typedefs.h>
#include <netconf.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>

static char *filter[] = { "lan_ifnames",
	"lan_ifname",
	"wan_ifnames",
	"wan_ifname",
	"et0macaddr",
	"et1macaddr",
	"il0macaddr",
	"boardnum",
	"boardtype",
	"boardrev",
	"melco_id",
	"product_name",
	"phyid_num",
	"cardbus",
	"CFEver",
	"clkfreq",
	"boardflags",
	"boardflags2",
	"sromrev",
	"sdram_config",
	"sdram_init",
	"sdram_refresh",
	"sdram_ncdl",
	"boot_wait",
	"wait_time",
	"et0phyaddr",
	"et0mdcport",
	"vlan0ports",
	"vlan1ports",
	"vlan2ports",
	"vlan0hwname",
	"vlan1hwname",
	"vlan2hwname",
	"wl_use_coregpio",
	"wl0gpio0",
	"wl0gpio1",
	"wl0gpio2",
	"wl0gpio3",
	"wl0gpio4",
	"reset_gpio",
	"af_hash",
	NULL
};

int nvram_critical(char *name)
{
	int a = 0;

	while (filter[a] != NULL) {
		if (!strcmp(name, filter[a++])) {
			return 1;
		}
	}
	return 0;
}

void nvram_clear(void)
{
	char *buf = (char *)safe_malloc(NVRAM_SPACE);

	nvram_getall(buf, NVRAM_SPACE);
	nvram_open();
	char *p = buf;
	int i;

	while (strlen(p) != 0) {
		int len = strlen(p);

		for (i = 0; i < len; i++)
			if (p[i] == '=')
				p[i] = 0;
		if (!nvram_critical(p))
			nvram_immed_set(p, NULL);
		p += len + 1;
	}
	nvram_close();
}

static void save(FILE * fp, char *p, int not)
{
	int i;
	while (strlen(p) != 0) {
		int len = strlen(p);
		if (len > 2 && (!!strncmp(p, "wl_", 3)) == not) {
			p += len + 1;
			continue;
		}
		for (i = 0; i < len; i++)
			if (p[i] == '=')
				p[i] = 0;
		char *name = p;
		fputc(strlen(name), fp);
		fwrite(name, 1, strlen(name), fp);
		char *val = nvram_safe_get(name);
		fputc(strlen(val) & 255, fp);
		fputc(strlen(val) >> 8, fp);
		fwrite(val, 1, strlen(val), fp);
		p += len + 1;
	}

}

int nvram_restore(char *filename)
{
	char sign[7];
	char *nvram_ver = NULL;
#ifdef HAVE_REGISTER
	if (!isregistered_real()) {
		return -1;
	}
#endif
	sign[6] = 0;
	FILE *fp = fopen(filename, "rb");
	if (!fp)
		return -1;
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int count = 0;

	fread(sign, 6, 1, fp);
	len -= 6;
	if (!strcmp(sign, "DD-WRT")) {
		nvram_clear();
		nvram_open();
		unsigned char b;

		fread((char *)&b, 1, 1, fp);
		count = b;
		fread((char *)&b, 1, 1, fp);
		count += ((unsigned int)b << 8);
		len -= 2;
		int i;

		for (i = 0; i < count && len > 0; i++) {
			unsigned short l = 0;
			unsigned char c = 0;

			fread((char *)&c, 1, 1, fp);
			char *name = (char *)safe_malloc(c + 1);

			fread(name, c, 1, fp);
			name[c] = 0;
			len -= (c + 1);

			fread((char *)&b, 1, 1, fp);
			l = b;
			fread((char *)&b, 1, 1, fp);
			l += ((unsigned int)b << 8);

			char *value = (char *)safe_malloc(l + 1);

			fread(value, l, 1, fp);
			len -= (l + 2);
			value[l] = 0;
			// cprintf("setting %s to %s\n",name,value);
			if (!strcmp(name, "nvram_ver"))
				nvram_ver = value;

			if (!nvram_critical(name)) {
				nvram_immed_set(name, value);
			}
			free(value);
			free(name);
		}
		nvram_close();
	} else {
		return -2;
	}

	if (nvram_ver == NULL) {
		nvram_set("http_passwd",
			  zencrypt(nvram_safe_get("http_passwd")));
		nvram_set("http_username",
			  zencrypt(nvram_safe_get("http_username")));
		if (nvram_get("newhttp_passwd") != NULL) {
			nvram_set("newhttp_passwd",
				  zencrypt(nvram_safe_get("newhttp_passwd")));
			nvram_set("newhttp_username",
				  zencrypt(nvram_safe_get("newhttp_username")));
		}
	}
	nvram_commit();

	return 0;

}

int nvram_backup(char *filename)
{

	int backupcount = 0;
	char sign[7] = { "DD-WRT" };
#ifdef HAVE_REGISTER
	if (!isregistered_real()) {
		return -1;
	}
#endif
	char *buf = (char *)safe_malloc(NVRAM_SPACE);

	nvram_getall(buf, NVRAM_SPACE);
	char *p = buf;
	int i;

	for (i = 0; i < NVRAM_SPACE; i++) {
		if (i > 0 && buf[i] == 0 && buf[i - 1] == 0) {
			break;
		}
		if (buf[i] == 0)
			backupcount++;
	}
	FILE *fp = fopen(filename, "wb");
	if (!fp)
		return -1;

	fwrite(sign, 6, 1, fp);
	fputc(backupcount & 255, fp);	// high byte
	fputc(backupcount >> 8, fp);	// low byte
	//first save all "wl_" prefixed parameters
	save(fp, p, 1);
	nvram_getall(buf, NVRAM_SPACE);
	p = buf;
	//now save anything else (this should prevent problems with backups, since wl0 parameters are getting higher priority now which solves restore problems with wds etc.
	save(fp, p, 0);
	free(buf);
	fclose(fp);
	return 0;
}
