/* vi: set sw=4 ts=4: */
/*
 * bb_perror_nomsg_and_die implementation for busybox
 *
 * Copyright (C) 2003  Manuel Novoa III  <mjn3@codepoet.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* gcc warns about a null format string, therefore we provide
 * modified definition without "attribute (format)"
 * instead of including libbb.h */
//#include "libbb.h"
#ifndef HAVE_NOMESSAGE
extern void bb_perror_msg_and_die(const char *s, ...);

void bb_perror_nomsg_and_die(void)
{
	bb_perror_msg_and_die(0);
}
#endif