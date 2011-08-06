/* mountlist.h -- declarations for list of mounted filesystems
   Copyright (C) 1991, 1992 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/** \file mountlist.h
 *  \brief Header: list of mounted filesystems
 */

#ifndef MC__MOUNTLIST_H
#define MC__MOUNTLIST_H

/*** typedefs(not structures) and defined constants **********************************************/

/*** enums ***************************************************************************************/

/*** structures declarations (and typedefs of structures)*****************************************/

/* Filesystem status */
struct my_statfs
{
    int type;
    char *typename;
    const char *mpoint;
    const char *device;
    int avail;
    int total;
    int nfree;
    int nodes;
};

/*** global variables defined in .c file *********************************************************/

/*** declarations of public functions ************************************************************/

/*** inline functions ****************************************************************************/

void init_my_statfs (void);
void my_statfs (struct my_statfs *myfs_stats, const char *path);
void free_my_statfs (void);

#endif /* MC__MOUNTLIST_H */
