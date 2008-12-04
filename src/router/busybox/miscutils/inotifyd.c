/* vi: set sw=4 ts=4: */
/*
 * simple inotify daemon
 * reports filesystem changes via userspace agent
 *
 * Copyright (C) 2008 by Vladimir Dronnikov <dronnikov@gmail.com>
 *
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */

/*
 * Use as follows:
 * # inotifyd /user/space/agent dir/or/file/being/watched[:mask] ...
 *
 * When a filesystem event matching the specified mask is occured on specified file (or directory)
 * a userspace agent is spawned and given the following parameters:
 * $1. actual event(s)
 * $2. file (or directory) name
 * $3. name of subfile (if any), in case of watching a directory
 *
 * E.g. inotifyd ./dev-watcher /dev:n
 *
 * ./dev-watcher can be, say:
 * #!/bin/sh
 * echo "We have new device in here! Hello, $3!"
 *
 * See below for mask names explanation.
 */

#include "libbb.h"
#include <sys/inotify.h>

static const char mask_names[] ALIGN1 =
	"a"	// 0x00000001	File was accessed
	"c"	// 0x00000002	File was modified
	"e"	// 0x00000004	Metadata changed
	"w"	// 0x00000008	Writtable file was closed
	"0"	// 0x00000010	Unwrittable file closed
	"r"	// 0x00000020	File was opened
	"m"	// 0x00000040	File was moved from X
	"y"	// 0x00000080	File was moved to Y
	"n"	// 0x00000100	Subfile was created
	"d"	// 0x00000200	Subfile was deleted
	"D"	// 0x00000400	Self was deleted
	"M"	// 0x00000800	Self was moved
;

extern int inotify_init(void);
extern int inotify_add_watch(int fd, const char *path, uint32_t mask);

int inotifyd_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int inotifyd_main(int argc UNUSED_PARAM, char **argv)
{
	int n;
	unsigned mask = IN_ALL_EVENTS; // assume we want all events
	struct pollfd pfd;
	char **watched = ++argv; // watched name list
	const char *args[] = { *argv, NULL, NULL, NULL, NULL };

	// sanity check: agent and at least one watch must be given
	if (!argv[1])
		bb_show_usage();

	// open inotify
	pfd.fd = inotify_init();
	if (pfd.fd < 0)
		bb_perror_msg_and_die("no kernel support");

	// setup watched
	while (*++argv) {
		char *path = *argv;
		char *masks = strchr(path, ':');
		// if mask is specified ->
		if (masks) {
			*masks = '\0'; // split path and mask
			// convert mask names to mask bitset
			mask = 0;
			while (*++masks) {
				int i = strchr(mask_names, *masks) - mask_names;
				if (i >= 0) {
					mask |= (1 << i);
				}
			}
		}
		// add watch
		n = inotify_add_watch(pfd.fd, path, mask);
		if (n < 0)
			bb_perror_msg_and_die("add watch (%s) failed", path);
		//bb_error_msg("added %d [%s]:%4X", n, path, mask);
	}

	// setup signals
	bb_signals(BB_FATAL_SIGS, record_signo);

	// do watch
	pfd.events = POLLIN;
	while (1) {
		ssize_t len;
		void *buf;
		struct inotify_event *ie;

 again:
		if (bb_got_signal)
			break;
		n = poll(&pfd, 1, -1);
		/* Signal interrupted us? */
		if (n < 0 && errno == EINTR)
			goto again;
		// Under Linux, above if() is not necessary.
		// Non-fatal signals, e.g. SIGCHLD, when set to SIG_DFL,
		// are not interrupting poll().
		// Thus we can just break if n <= 0 (see below),
		// because EINTR will happen only on SIGTERM et al.
		// But this might be not true under other Unixes,
		// and is generally way too subtle to depend on.
		if (n <= 0) // strange error?
			break;

		// read out all pending events
		xioctl(pfd.fd, FIONREAD, &len);
#define eventbuf bb_common_bufsiz1
		ie = buf = (len <= sizeof(eventbuf)) ? eventbuf : xmalloc(len);
		len = full_read(pfd.fd, buf, len);
		// process events. N.B. events may vary in length
		while (len > 0) {
			int i;
			char events[sizeof(mask_names)];
			char *s = events;
			unsigned m = ie->mask;

			for (i = 0; i < sizeof(mask_names)-1; ++i, m >>= 1) {
				if (m & 1)
					*s++ = mask_names[i];
			}
			*s = '\0';
			//bb_error_msg("exec %s %08X\t%s\t%s\t%s", agent,
			// ie->mask, events, watched[ie->wd], ie->len ? ie->name : "");
			args[1] = events;
			args[2] = watched[ie->wd];
			args[3] = ie->len ? ie->name : NULL;
			wait4pid(xspawn((char **)args));
			// next event
			i = sizeof(struct inotify_event) + ie->len;
			len -= i;
			ie = (void*)((char*)ie + i);
		}
		if (eventbuf != buf)
			free(buf);
	}

	return EXIT_SUCCESS;
}
