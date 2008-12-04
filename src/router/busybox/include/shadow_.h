/* vi: set sw=4 ts=4: */
/* Copyright (C) 1996, 1997, 1998, 1999 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* Declaration of types and functions for shadow password suite */

#ifndef BB_SHADOW_H
#define BB_SHADOW_H 1

#if __GNUC_PREREQ(4,1)
# pragma GCC visibility push(hidden)
#endif

/* This file is #included after #include <shadow.h>
 * We will use libc-defined structures, but will #define function names
 * so that function calls are directed to bb_internal_XXX replacements
 */

/* Paths to the user database files */
#ifndef _PATH_SHADOW
#define _PATH_SHADOW "/etc/shadow"
#endif

#define setspent    bb_internal_setspent
#define endspent    bb_internal_endspent
#define getspent    bb_internal_getspent
#define getspnam    bb_internal_getspnam
#define sgetspent   bb_internal_sgetspent
#define fgetspent   bb_internal_fgetspent
#define putspent    bb_internal_putspent
#define getspent_r  bb_internal_getspent_r
#define getspnam_r  bb_internal_getspnam_r
#define sgetspent_r bb_internal_sgetspent_r
#define fgetspent_r bb_internal_fgetspent_r
#define lckpwdf     bb_internal_lckpwdf
#define ulckpwdf    bb_internal_ulckpwdf


/* All function names below should be remapped by #defines above
 * in order to not collide with libc names. */


/* Open database for reading */
extern void setspent(void);

/* Close database */
extern void endspent(void);

/* Get next entry from database, perhaps after opening the file */
extern struct spwd *getspent(void);

/* Get shadow entry matching NAME */
extern struct spwd *getspnam(const char *__name);

/* Read shadow entry from STRING */
extern struct spwd *sgetspent(const char *__string);

/* Read next shadow entry from STREAM */
extern struct spwd *fgetspent(FILE *__stream);

/* Write line containing shadow password entry to stream */
extern int putspent(const struct spwd *__p, FILE *__stream);

/* Reentrant versions of some of the functions above */
extern int getspent_r(struct spwd *__result_buf, char *__buffer,
		       size_t __buflen, struct spwd **__result);

extern int getspnam_r(const char *__name, struct spwd *__result_buf,
		       char *__buffer, size_t __buflen,
		       struct spwd **__result);

extern int sgetspent_r(const char *__string, struct spwd *__result_buf,
			char *__buffer, size_t __buflen,
			struct spwd **__result);

extern int fgetspent_r(FILE *__stream, struct spwd *__result_buf,
			char *__buffer, size_t __buflen,
			struct spwd **__result);
/* Protect password file against multi writers */
extern int lckpwdf(void);

/* Unlock password file */
extern int ulckpwdf(void);

#if __GNUC_PREREQ(4,1)
# pragma GCC visibility pop
#endif

#endif /* shadow.h */
