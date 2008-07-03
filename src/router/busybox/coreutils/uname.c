/* vi: set sw=4 ts=4: */
/* uname -- print system information
 * Copyright (C) 1989-1999 Free Software Foundation, Inc.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* BB_AUDIT SUSv3 compliant */
/* http://www.opengroup.org/onlinepubs/007904975/utilities/uname.html */

/* Option		Example

   -s, --sysname	SunOS
   -n, --nodename	rocky8
   -r, --release	4.0
   -v, --version
   -m, --machine	sun
   -a, --all		SunOS rocky8 4.0  sun

   The default behavior is equivalent to '-s'.

   David MacKenzie <djm@gnu.ai.mit.edu> */

/* Busyboxed by Erik Andersen */

/* Further size reductions by Glenn McGrath and Manuel Novoa III. */

/* Mar 16, 2003      Manuel Novoa III   (mjn3@codepoet.org)
 *
 * Now does proper error checking on i/o.  Plus some further space savings.
 */

#include <sys/utsname.h>
#include "libbb.h"

typedef struct {
	struct utsname name;
	char processor[8];			/* for "unknown" */
} uname_info_t;

static const char options[] ALIGN1 = "snrvmpa";
static const unsigned short utsname_offset[] = {
	offsetof(uname_info_t, name.sysname),
	offsetof(uname_info_t, name.nodename),
	offsetof(uname_info_t, name.release),
	offsetof(uname_info_t, name.version),
	offsetof(uname_info_t, name.machine),
	offsetof(uname_info_t, processor)
};

int uname_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int uname_main(int argc ATTRIBUTE_UNUSED, char **argv)
{
	uname_info_t uname_info;
#if defined(__sparc__) && defined(__linux__)
	char *fake_sparc = getenv("FAKE_SPARC");
#endif
	const unsigned short *delta;
	char toprint;

	toprint = getopt32(argv, options);

	if (argv[optind]) { /* coreutils-6.9 compat */
		bb_show_usage();
	}

	if (toprint & (1 << 6)) { /* -a => all opts on */
		toprint = 0x3f;
	}

	if (toprint == 0) { /* no opts => -s (sysname) */
		toprint = 1;
	}

	uname(&uname_info.name); /* never fails */

#if defined(__sparc__) && defined(__linux__)
	if (fake_sparc && (fake_sparc[0] | 0x20) == 'y') {
		strcpy(uname_info.name.machine, "sparc");
	}
#endif

	strcpy(uname_info.processor, "unknown");

	delta = utsname_offset;
	do {
		if (toprint & 1) {
			/* printf would not be safe here */
			fputs((char *)(&uname_info) + *delta, stdout);
			if (toprint > 1) {
				bb_putchar(' ');
			}
		}
		++delta;
	} while (toprint >>= 1);
	bb_putchar('\n');

	fflush_stdout_and_exit(EXIT_SUCCESS); /* coreutils-6.9 compat */
}
