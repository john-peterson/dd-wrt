/*
 * minidlna.c
 *
 * Copyright (C) 2012 dd-wrt
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
#ifdef HAVE_MINIDLNA
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

void start_dlna(void)
{
	if (!nvram_match("dlna_enable", "1"))
		return;
	mkdir("/tmp/db", 0700);
	FILE *fp = fopen("/tmp/minidlna.conf", "wb");
	fprintf(fp, "port=8200\n");
	fprintf(fp, "network_interface=br0\n");
	fprintf(fp, "media_dir=/mnt\n");	// comma separted list
	fprintf(fp, "friendly_name=%s:DLNA\n",nvram_safe_get("DD_BOARD")); //enter any name you want here, but should be unique within a network
	fprintf(fp,
		"album_art_names=Cover.jpg/cover.jpg/AlbumArtSmall.jpg/albumartsmall.jpg/AlbumArt.jpg/albumart.jpg/Album.jpg/album.jpg/Folder.jpg/folder.jpg/Thumb.jpg/thumb.jpg\n");
	fprintf(fp, "inotify=yes\n");
	fprintf(fp, "enable_tivo=no\n");
	fprintf(fp, "strict_dlna=no\n");
	fprintf(fp, "notify_interval=900\n");
	fprintf(fp, "serial=12345678\nmodel_number=1\n");
	fclose(fp);
	eval("minidlna", "-f", "/tmp/minidlna.conf");
	syslog(LOG_INFO, "minidlna : DLNA Media Server successfully started\n");

	return;
}

void stop_dlna(void)
{
	stop_process("minidlna", "DLNA Media Server");
}
#endif
