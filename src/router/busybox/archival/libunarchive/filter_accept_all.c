/* vi: set sw=4 ts=4: */
/*
 * Copyright (C) 2002 by Glenn McGrath
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"
#include "unarchive.h"

/* Accept any non-null name, its not really a filter at all */
char filter_accept_all(archive_handle_t *archive_handle)
{
	if (archive_handle->file_header->name)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}
