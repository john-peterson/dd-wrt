/*
 * pass4.c -- pass #4 of e2fsck: Check reference counts
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 *
 * Pass 4 frees the following data structures:
 * 	- A bitmap of which inodes are in bad blocks.	(inode_bb_map)
 * 	- A bitmap of which inodes are imagic inodes.	(inode_imagic_map)
 */

#include "e2fsck.h"
#include "problem.h"

/*
 * This routine is called when an inode is not connected to the
 * directory tree.
 * 
 * This subroutine returns 1 then the caller shouldn't bother with the
 * rest of the pass 4 tests.
 */
static int disconnect_inode(e2fsck_t ctx, ext2_ino_t i)
{
	ext2_filsys fs = ctx->fs;
	struct ext2_inode	inode;
	struct problem_context	pctx;

	e2fsck_read_inode(ctx, i, &inode, "pass4: disconnect_inode");
	clear_problem_context(&pctx);
	pctx.ino = i;
	pctx.inode = &inode;
	
	/*
	 * Offer to delete any zero-length files that does not have
	 * blocks.  If there is an EA block, it might have useful
	 * information, so we won't prompt to delete it, but let it be
	 * reconnected to lost+found.
	 */
	if (!inode.i_blocks && (LINUX_S_ISREG(inode.i_mode) ||
				LINUX_S_ISDIR(inode.i_mode))) {
		if (fix_problem(ctx, PR_4_ZERO_LEN_INODE, &pctx)) {
			ext2fs_icount_store(ctx->inode_link_info, i, 0);
			inode.i_links_count = 0;
			inode.i_dtime = ctx->now;
			e2fsck_write_inode(ctx, i, &inode,
					   "disconnect_inode");
			/*
			 * Fix up the bitmaps...
			 */
			e2fsck_read_bitmaps(ctx);
			ext2fs_unmark_inode_bitmap(ctx->inode_used_map, i);
			ext2fs_unmark_inode_bitmap(ctx->inode_dir_map, i);
			ext2fs_inode_alloc_stats2(fs, i, -1,
						  LINUX_S_ISDIR(inode.i_mode));
			return 0;
		}
	}
	
	/*
	 * Prompt to reconnect.
	 */
	if (fix_problem(ctx, PR_4_UNATTACHED_INODE, &pctx)) {
		if (e2fsck_reconnect_file(ctx, i))
			ext2fs_unmark_valid(fs);
	} else {
		/*
		 * If we don't attach the inode, then skip the
		 * i_links_test since there's no point in trying to
		 * force i_links_count to zero.
		 */
		ext2fs_unmark_valid(fs);
		return 1;
	}
	return 0;
}


void e2fsck_pass4(e2fsck_t ctx)
{
	ext2_filsys fs = ctx->fs;
	ext2_ino_t	i;
	struct ext2_inode	inode;
#ifdef RESOURCE_TRACK
	struct resource_track	rtrack;
#endif
	struct problem_context	pctx;
	__u16	link_count, link_counted;
	char	*buf = 0;
	int	group, maxgroup;
	
#ifdef RESOURCE_TRACK
	init_resource_track(&rtrack);
#endif

#ifdef MTRACE
	mtrace_print("Pass 4");
#endif

	clear_problem_context(&pctx);

	if (!(ctx->options & E2F_OPT_PREEN))
		fix_problem(ctx, PR_4_PASS_HEADER, &pctx);

	group = 0;
	maxgroup = fs->group_desc_count;
	if (ctx->progress)
		if ((ctx->progress)(ctx, 4, 0, maxgroup))
			return;
	
	for (i=1; i <= fs->super->s_inodes_count; i++) {
		if (ctx->flags & E2F_FLAG_SIGNAL_MASK)
			return;
		if ((i % fs->super->s_inodes_per_group) == 0) {
			group++;
			if (ctx->progress)
				if ((ctx->progress)(ctx, 4, group, maxgroup))
					return;
		}
		if (i == EXT2_BAD_INO ||
		    (i > EXT2_ROOT_INO && i < EXT2_FIRST_INODE(fs->super)))
			continue;
		if (!(ext2fs_test_inode_bitmap(ctx->inode_used_map, i)) ||
		    (ctx->inode_imagic_map &&
		     ext2fs_test_inode_bitmap(ctx->inode_imagic_map, i)) ||
		    (ctx->inode_bb_map &&
		     ext2fs_test_inode_bitmap(ctx->inode_bb_map, i)))
			continue;
		ext2fs_icount_fetch(ctx->inode_link_info, i, &link_count);
		ext2fs_icount_fetch(ctx->inode_count, i, &link_counted);
		if (link_counted == 0) {
			if (!buf)
				buf = e2fsck_allocate_memory(ctx,
				     fs->blocksize, "bad_inode buffer");
			if (e2fsck_process_bad_inode(ctx, 0, i, buf))
				continue;
			if (disconnect_inode(ctx, i))
				continue;
			ext2fs_icount_fetch(ctx->inode_link_info, i,
					    &link_count);
			ext2fs_icount_fetch(ctx->inode_count, i,
					    &link_counted);
		}
		if (link_counted != link_count) {
			e2fsck_read_inode(ctx, i, &inode, "pass4");
			pctx.ino = i;
			pctx.inode = &inode;
			if (link_count != inode.i_links_count) {
				pctx.num = link_count;
				fix_problem(ctx,
					    PR_4_INCONSISTENT_COUNT, &pctx);
			}
			pctx.num = link_counted;
			if (fix_problem(ctx, PR_4_BAD_REF_COUNT, &pctx)) {
				inode.i_links_count = link_counted;
				e2fsck_write_inode(ctx, i, &inode, "pass4");
			}
		}
	}
	ext2fs_free_icount(ctx->inode_link_info); ctx->inode_link_info = 0;
	ext2fs_free_icount(ctx->inode_count); ctx->inode_count = 0;
	ext2fs_free_inode_bitmap(ctx->inode_bb_map);
	ctx->inode_bb_map = 0;
	ext2fs_free_inode_bitmap(ctx->inode_imagic_map);
	ctx->inode_imagic_map = 0;
	if (buf)
		ext2fs_free_mem(&buf);
#ifdef RESOURCE_TRACK
	if (ctx->options & E2F_OPT_TIME2) {
		e2fsck_clear_progbar(ctx);
		print_resource_track(_("Pass 4"), &rtrack);
	}
#endif
}

