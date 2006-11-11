/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "libbb.h"

#ifndef HAVE_NOMESSAGE
void bb_perror_msg(const char *s, ...)
{
	va_list p;

	va_start(p, s);
	bb_vperror_msg(s, p);
	va_end(p);
}
#endif
