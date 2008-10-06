/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 2008 Bernhard Fischer
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"

/* Open file and write string str to it, close file.
 * Die on any open or write-error.  */
void FAST_FUNC xopen_xwrite_close(const char* file, const char* str)
{
	int fd = xopen(file, O_WRONLY);

	xwrite(fd, str, strlen(str));
	close(fd);
}
