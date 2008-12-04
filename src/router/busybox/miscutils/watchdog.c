/* vi: set sw=4 ts=4: */
/*
 * Mini watchdog implementation for busybox
 *
 * Copyright (C) 2003  Paul Mundt <lethal@linux-sh.org>
 * Copyright (C) 2006  Bernhard Reutner-Fischer <busybox@busybox.net>
 * Copyright (C) 2008  Darius Augulis <augulis.darius@gmail.com>
 *
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */

#include "libbb.h"
#include "linux/watchdog.h"

#define OPT_FOREGROUND  (1 << 0)
#define OPT_STIMER      (1 << 1)
#define OPT_HTIMER      (1 << 2)

static void watchdog_shutdown(int sig UNUSED_PARAM)
{
	static const char V = 'V';

	write(3, &V, 1);	/* Magic, see watchdog-api.txt in kernel */
	if (ENABLE_FEATURE_CLEAN_UP)
		close(3);
	exit(EXIT_SUCCESS);
}

int watchdog_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int watchdog_main(int argc, char **argv)
{
	static const struct suffix_mult suffixes[] = {
		{ "ms", 1 },
		{ "", 1000 },
		{ }
	};

	unsigned opts;
	unsigned stimer_duration; /* how often to restart */
	unsigned htimer_duration = 60000; /* reboots after N ms if not restarted */
	char *st_arg;
	char *ht_arg;

	opt_complementary = "=1"; /* must have exactly 1 argument */
	opts = getopt32(argv, "Ft:T:", &st_arg, &ht_arg);

	if (opts & OPT_HTIMER)
		htimer_duration = xatou_sfx(ht_arg, suffixes);
	stimer_duration = htimer_duration / 2;
	if (opts & OPT_STIMER)
		stimer_duration = xatou_sfx(st_arg, suffixes);

	bb_signals(BB_FATAL_SIGS, watchdog_shutdown);

	/* Use known fd # - avoid needing global 'int fd' */
	xmove_fd(xopen(argv[argc - 1], O_WRONLY), 3);

	/* WDIOC_SETTIMEOUT takes seconds, not milliseconds */
	htimer_duration = htimer_duration / 1000;
	ioctl_or_warn(3, WDIOC_SETTIMEOUT, &htimer_duration);
#if 0
	ioctl_or_warn(3, WDIOC_GETTIMEOUT, &htimer_duration);
	printf("watchdog: SW timer is %dms, HW timer is %dms\n",
		stimer_duration, htimer_duration * 1000);
#endif

	if (!(opts & OPT_FOREGROUND)) {
		bb_daemonize_or_rexec(DAEMON_CHDIR_ROOT, argv);
	}

	while (1) {
		/*
		 * Make sure we clear the counter before sleeping, as the counter value
		 * is undefined at this point -- PFM
		 */
		write(3, "", 1); /* write zero byte */
		usleep(stimer_duration * 1000L);
	}
	return EXIT_SUCCESS; /* - not reached, but gcc 4.2.1 is too dumb! */
}
