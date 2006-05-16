/* 
 * QNX4 file system, Linux implementation.
 * 
 * Version : 0.1
 * 
 * Using parts of the xiafs filesystem.
 * 
 * History :
 * 
 * 30-06-1998 by Frank DENIS : ugly filler.
 */

#include <linux/config.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/qnx4_fs.h>
#include <linux/fs.h>
#include <linux/locks.h>
#include <asm/uaccess.h>

#ifdef CONFIG_QNX4FS_RW

void qnx4_truncate(struct inode *inode)
{
	if (!(S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode) ||
	      S_ISLNK(inode->i_mode))) {
		return;
	}
	if (!(S_ISDIR(inode->i_mode))) {
		/* TODO */
	}
	QNX4DEBUG(("qnx4: qnx4_truncate called\n"));
	inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	mark_inode_dirty(inode);
}

#endif
