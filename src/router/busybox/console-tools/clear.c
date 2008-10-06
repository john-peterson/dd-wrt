/* vi: set sw=4 ts=4: */
/*
 * Mini clear implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 *
 */

/* no options, no getopt */

#include "libbb.h"

int clear_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int clear_main(int argc UNUSED_PARAM, char **argv UNUSED_PARAM)
{
	return printf("\033[H\033[J") != 6;
}
