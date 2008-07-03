/* vi: set sw=4 ts=4: */
/*
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"
#include "unarchive.h"

void seek_by_jump(const archive_handle_t *archive_handle, unsigned amount)
{
	if (lseek(archive_handle->src_fd, (off_t) amount, SEEK_CUR) == (off_t) -1) {
		if (errno == ESPIPE)
			seek_by_read(archive_handle, amount);
		else
			bb_perror_msg_and_die("seek failure");
	}
}
