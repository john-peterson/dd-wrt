/* vi: set sw=4 ts=4: */
/*
 * readahead implementation for busybox
 *
 * Preloads the given files in RAM, to reduce access time.
 * Does this by calling the readahead(2) system call.
 *
 * Copyright (C) 2006  Michael Opdenacker <michael@free-electrons.com>
 *
 * Licensed under GPLv2 or later, see file License in this tarball for details.
 */

#include "libbb.h"

int readahead_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int readahead_main(int argc, char **argv)
{
	int retval = EXIT_SUCCESS;

	if (argc == 1) bb_show_usage();

	while (*++argv) {
		int fd = open_or_warn(*argv, O_RDONLY);
		if (fd >= 0) {
			int r = readahead(fd, 0, fdlength(fd));
			close(fd);
			if (r >= 0) continue;
		}
		retval = EXIT_FAILURE;
	}

	return retval;
}
