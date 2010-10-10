/*
 * mst.h - Exports for multi sector transfer fixup functions.
 *         Originated from the Linux-NTFS project.
 *
 * Copyright (c) 2000-2002 Anton Altaparmakov
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the NTFS-3G
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _NTFS_MST_H
#define _NTFS_MST_H

#include "types.h"
#include "layout.h"

extern int ntfs_mst_post_read_fixup(NTFS_RECORD *b, const u32 size);
extern int ntfs_mst_pre_write_fixup(NTFS_RECORD *b, const u32 size);
extern void ntfs_mst_post_write_fixup(NTFS_RECORD *b);

#endif /* defined _NTFS_MST_H */

