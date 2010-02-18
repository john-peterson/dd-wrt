/* fsusage.h -- declarations for file system space usage info

   Copyright (C) 1991, 1992, 1997, 2003, 2004, 2005, 2006 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Space usage statistics for a file system.  Blocks are 512-byte. */

#if !defined MC_FSUSAGE_H
# define MC_FSUSAGE_H

# if HAVE_INTTYPES_H
#  include <inttypes.h>
# endif

struct fs_usage
{
  uintmax_t fsu_blocksize;	/* Size of a block.  */
  uintmax_t fsu_blocks;		/* Total blocks. */
  uintmax_t fsu_bfree;		/* Free blocks available to superuser. */
  uintmax_t fsu_bavail;		/* Free blocks available to non-superuser. */
  int fsu_bavail_top_bit_set;	/* 1 if fsu_bavail represents a value < 0.  */
  uintmax_t fsu_files;		/* Total file nodes. */
  uintmax_t fsu_ffree;		/* Free file nodes. */
};

int get_fs_usage (char const *file, char const *disk, struct fs_usage *fsp);

#endif
