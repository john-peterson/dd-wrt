/*
 * volume_id - reads filesystem label and uuid
 *
 * Copyright (C) 2005 Kay Sievers <kay.sievers@vrfy.org>
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by the
 *	Free Software Foundation version 2 of the License.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "libvolume_id.h"
#include "util.h"

struct isw_meta {
	uint8_t		sig[32];
	uint32_t	check_sum;
	uint32_t	mpb_size;
	uint32_t	family_num;
	uint32_t	generation_num;
} PACKED;

#define ISW_SIGNATURE		"Intel Raid ISM Cfg Sig. "


int volume_id_probe_intel_software_raid(struct volume_id *id, uint64_t off, uint64_t size)
{
	const uint8_t *buf;
	uint64_t meta_off;
	struct isw_meta *isw;

	info("probing at offset 0x%llx, size 0x%llx",
	    (unsigned long long) off, (unsigned long long) size);

	if (size < 0x10000)
		return -1;

	meta_off = ((size / 0x200)-2) * 0x200;
	buf = volume_id_get_buffer(id, off + meta_off, 0x200);
	if (buf == NULL)
		return -1;

	isw = (struct isw_meta *) buf;
	if (memcmp(isw->sig, ISW_SIGNATURE, sizeof(ISW_SIGNATURE)-1) != 0)
		return -1;

	volume_id_set_usage(id, VOLUME_ID_RAID);
	memcpy(id->type_version, &isw->sig[sizeof(ISW_SIGNATURE)-1], 6);
	id->type = "isw_raid_member";

	return 0;
}
