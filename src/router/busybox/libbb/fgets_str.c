/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) many different people.
 * If you wrote this, please acknowledge your work.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"

static char *xmalloc_fgets_internal(FILE *file, const char *terminating_string, int chop_off)
{
	char *linebuf = NULL;
	const int term_length = strlen(terminating_string);
	int end_string_offset;
	int linebufsz = 0;
	int idx = 0;
	int ch;

	while (1) {
		ch = fgetc(file);
		if (ch == EOF) {
			if (idx == 0)
				return linebuf; /* NULL */
			break;
		}

		if (idx >= linebufsz) {
			linebufsz += 200;
			linebuf = xrealloc(linebuf, linebufsz);
		}

		linebuf[idx] = ch;
		idx++;

		/* Check for terminating string */
		end_string_offset = idx - term_length;
		if (end_string_offset >= 0
		 && memcmp(&linebuf[end_string_offset], terminating_string, term_length) == 0
		) {
			if (chop_off)
				idx -= term_length;
			break;
		}
	}
	/* Grow/shrink *first*, then store NUL */
	linebuf = xrealloc(linebuf, idx + 1);
	linebuf[idx] = '\0';
	return linebuf;
}

/* Read up to TERMINATING_STRING from FILE and return it,
 * including terminating string.
 * Non-terminated string can be returned if EOF is reached.
 * Return NULL if EOF is reached immediately.  */
char *xmalloc_fgets_str(FILE *file, const char *terminating_string)
{
	return xmalloc_fgets_internal(file, terminating_string, 0);
}

char *xmalloc_fgetline_str(FILE *file, const char *terminating_string)
{
	return xmalloc_fgets_internal(file, terminating_string, 1);
}
