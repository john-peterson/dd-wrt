/* vi: set sw=4 ts=4: */
/*
 * password utility routines.
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Licensed under the GPL v2, see the file LICENSE in this tarball.
 */


#ifdef L_bb_getgrgid
  /* Hacked by Tito Ragusa (c) 2004 <farmatito@tiscali.it> to make it more
  * flexible :
  *
  * if bufsize is > 0 char *group cannot be set to NULL.
  *                   On success groupname is written on static allocated buffer
  *                   group (and a pointer to it is returned).
  *                   On failure gid as string is written to static allocated
  *                   buffer group and NULL is returned.
  * if bufsize is = 0 char *group can be set to NULL.
  *                   On success groupname is returned.
  *                   On failure NULL is returned.
  * if bufsize is < 0 char *group can be set to NULL.
  *                   On success groupname is returned.
  *                   On failure an error message is printed and
  *                   the program exits.
  */

#include "libbb.h"
#include "grp_.h"

/* gets a groupname given a gid */
char * bb_getgrgid(char *group, long gid, int bufsize)
{
	struct group *mygroup = getgrgid(gid);

	return  bb_getug(group, (mygroup) ?
			mygroup->gr_name : (char *)mygroup, gid, bufsize, 'g');
}
#endif /* L_bb_getgrgid */

#ifdef L_bb_xgetgrnam
#include <stdio.h>
#include <string.h>
#include "libbb.h"
#include "pwd_.h"
#include "grp_.h"


/* returns a gid given a group name */
long bb_xgetgrnam(const char *name)
{
	struct group *mygroup;

	mygroup  = getgrnam(name);
	if (mygroup==NULL)
		bb_error_msg_and_die("unknown group name: %s", name);

	return (mygroup->gr_gid);
}
#endif /* L_bb_xgetgrnam */

#ifdef L_bb_xgetpwnam
#include <stdio.h>
#include <string.h>
#include "libbb.h"
#include "pwd_.h"
#include "grp_.h"


/* returns a uid given a username */
long bb_xgetpwnam(const char *name)
{
	struct passwd *myuser;

	myuser  = getpwnam(name);
	if (myuser==NULL)
		bb_error_msg_and_die("unknown user name: %s", name);

	return myuser->pw_uid;
}
#endif /* L_bb_xgetpwnam */

#ifdef L_bb_getpwuid
 /* Hacked by Tito Ragusa (c) 2004 <farmatito@tiscali.it> to make it more
  * flexible :
  *
  * if bufsize is > 0 char *name can not be set to NULL.
  *                   On success username is written on the static allocated
  *                   buffer name (and a pointer to it is returned).
  *                   On failure uid as string is written to the static
  *                   allocated buffer name and NULL is returned.
  * if bufsize is = 0 char *name can be set to NULL.
  *                   On success username is returned.
  *                   On failure NULL is returned.
  * if bufsize is < 0 char *name can be set to NULL
  *                   On success username is returned.
  *                   On failure an error message is printed and
  *                   the program exits.
  */

#include "libbb.h"
#include "pwd_.h"

/* gets a username given a uid */
char * bb_getpwuid(char *name, long uid, int bufsize)
{
	struct passwd *myuser = getpwuid(uid);

	return  bb_getug(name, (myuser) ?
			myuser->pw_name : (char *)myuser , uid, bufsize, 'u');
}
#endif /* L_bb_getpwuid */

#ifdef L_bb_getug
 /*
  * if bufsize is > 0 char *buffer can not be set to NULL.
  *                   If idname is not NULL it is written on the static
  *                   allocated buffer (and a pointer to it is returned).
  *                   if idname is NULL, id as string is written to the static
  *                   allocated buffer and NULL is returned.
  * if bufsize is = 0 char *buffer can be set to NULL.
  *                   If idname exists a pointer to it is returned,
  *                   else NULL is returned.
  * if bufsize is < 0 char *buffer can be set to NULL.
  *                   If idname exists a pointer to it is returned,
  *                   else an error message is printed and the program exits.
  */

#include <stdio.h>
#include <assert.h>
#include "libbb.h"


/* internal function for bb_getpwuid and bb_getgrgid */
char * bb_getug(char *buffer, char *idname, long id, int bufsize, char prefix)
{
	if(bufsize > 0 ) {
		assert(buffer!=NULL);
		if(idname) {
			return safe_strncpy(buffer, idname, bufsize);
		}
		snprintf(buffer, bufsize, "%ld", id);
	} else if(bufsize < 0 && !idname) {
		bb_error_msg_and_die("unknown %cid %ld", prefix, id);
	}
	return idname;
}
#endif /* L_bb_getug */


#ifdef L_get_ug_id
/* indirect dispatcher for pwd helpers.  */
#include <stdlib.h>
#include "libbb.h"

extern unsigned long get_ug_id(const char *s,
		long (*__bb_getxxnam)(const char *))
{
	unsigned long r;
	char *p;

	r = strtoul(s, &p, 10);
	if (*p || (s == p)) {
		r = __bb_getxxnam(s);
	}

	return r;
}
#endif /* L_get_ug_id */

/* END CODE */
/*
Local Variables:
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/

