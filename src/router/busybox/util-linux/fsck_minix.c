/* vi: set sw=4 ts=4: */
/*
 * fsck.c - a file system consistency checker for Linux.
 *
 * (C) 1991, 1992 Linus Torvalds.
 *
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */

/*
 * 09.11.91  -  made the first rudimentary functions
 *
 * 10.11.91  -  updated, does checking, no repairs yet.
 *		Sent out to the mailing-list for testing.
 *
 * 14.11.91  -	Testing seems to have gone well. Added some
 *		correction-code, and changed some functions.
 *
 * 15.11.91  -  More correction code. Hopefully it notices most
 *		cases now, and tries to do something about them.
 *
 * 16.11.91  -  More corrections (thanks to Mika Jalava). Most
 *		things seem to work now. Yeah, sure.
 *
 *
 * 19.04.92  -	Had to start over again from this old version, as a
 *		kernel bug ate my enhanced fsck in february.
 *
 * 28.02.93  -	added support for different directory entry sizes..
 *
 * Sat Mar  6 18:59:42 1993, faith@cs.unc.edu: Output namelen with
 *                           super-block information
 *
 * Sat Oct  9 11:17:11 1993, faith@cs.unc.edu: make exit status conform
 *                           to that required by fsutil
 *
 * Mon Jan  3 11:06:52 1994 - Dr. Wettstein (greg%wind.uucp@plains.nodak.edu)
 *			      Added support for file system valid flag.  Also
 *			      added program_version variable and output of
 *			      program name and version number when program
 *			      is executed.
 *
 * 30.10.94 - added support for v2 filesystem
 *            (Andreas Schwab, schwab@issan.informatik.uni-dortmund.de)
 *
 * 10.12.94  -  added test to prevent checking of mounted fs adapted
 *              from Theodore Ts'o's (tytso@athena.mit.edu) e2fsck
 *              program.  (Daniel Quinlan, quinlan@yggdrasil.com)
 *
 * 01.07.96  - Fixed the v2 fs stuff to use the right #defines and such
 *	       for modern libcs (janl@math.uio.no, Nicolai Langfeldt)
 *
 * 02.07.96  - Added C bit fiddling routines from rmk@ecs.soton.ac.uk
 *             (Russell King).  He made them for ARM.  It would seem
 *	       that the ARM is powerful enough to do this in C whereas
 *             i386 and m64k must use assembly to get it fast >:-)
 *	       This should make minix fsck system-independent.
 *	       (janl@math.uio.no, Nicolai Langfeldt)
 *
 * 04.11.96  - Added minor fixes from Andreas Schwab to avoid compiler
 *             warnings.  Added mc68k bitops from
 *	       Joerg Dorchain <dorchain@mpi-sb.mpg.de>.
 *
 * 06.11.96  - Added v2 code submitted by Joerg Dorchain, but written by
 *             Andreas Schwab.
 *
 * 1999-02-22 Arkadiusz Mi�kiewicz <misiek@misiek.eu.org>
 * - added Native Language Support
 *
 *
 * I've had no time to add comments - hopefully the function names
 * are comments enough. As with all file system checkers, this assumes
 * the file system is quiescent - don't use it on a mounted device
 * unless you can be sure nobody is writing to it (and remember that the
 * kernel can write to it when it searches for files).
 *
 * Usage: fsck [-larvsm] device
 *	-l for a listing of all the filenames
 *	-a for automatic repairs (not implemented)
 *	-r for repairs (interactive) (not implemented)
 *	-v for verbose (tells how many files)
 *	-s for super-block info
 *	-m for minix-like "mode not cleared" warnings
 *	-f force filesystem check even if filesystem marked as valid
 *
 * The device may be a block device or a image of one, but this isn't
 * enforced (but it's not much fun on a character device :-).
 */

#include "busybox.h"
#include <mntent.h>

/*
 * This is the original minix inode layout on disk.
 * Note the 8-bit gid and atime and ctime.
 */
struct minix_inode {
	uint16_t i_mode;
	uint16_t i_uid;
	uint32_t i_size;
	uint32_t i_time;
	uint8_t  i_gid;
	uint8_t  i_nlinks;
	uint16_t i_zone[9];
};

/*
 * The new minix inode has all the time entries, as well as
 * long block numbers and a third indirect block (7+1+1+1
 * instead of 7+1+1). Also, some previously 8-bit values are
 * now 16-bit. The inode is now 64 bytes instead of 32.
 */
struct minix2_inode {
	uint16_t i_mode;
	uint16_t i_nlinks;
	uint16_t i_uid;
	uint16_t i_gid;
	uint32_t i_size;
	uint32_t i_atime;
	uint32_t i_mtime;
	uint32_t i_ctime;
	uint32_t i_zone[10];
};

enum {
	MINIX_ROOT_INO = 1,
	MINIX_LINK_MAX = 250,
	MINIX2_LINK_MAX = 65530,

	MINIX_I_MAP_SLOTS = 8,
	MINIX_Z_MAP_SLOTS = 64,
	MINIX_SUPER_MAGIC = 0x137F,		/* original minix fs */
	MINIX_SUPER_MAGIC2 = 0x138F,		/* minix fs, 30 char names */
	MINIX2_SUPER_MAGIC = 0x2468,		/* minix V2 fs */
	MINIX2_SUPER_MAGIC2 = 0x2478,		/* minix V2 fs, 30 char names */
	MINIX_VALID_FS = 0x0001,		/* Clean fs. */
	MINIX_ERROR_FS = 0x0002,		/* fs has errors. */

	MINIX_INODES_PER_BLOCK = ((BLOCK_SIZE)/(sizeof (struct minix_inode))),
	MINIX2_INODES_PER_BLOCK = ((BLOCK_SIZE)/(sizeof (struct minix2_inode))),

	MINIX_V1 = 0x0001,		/* original minix fs */
	MINIX_V2 = 0x0002 		/* minix V2 fs */
};

#define INODE_VERSION(inode)	inode->i_sb->u.minix_sb.s_version

/*
 * minix super-block data on disk
 */
struct minix_super_block {
	uint16_t s_ninodes;
	uint16_t s_nzones;
	uint16_t s_imap_blocks;
	uint16_t s_zmap_blocks;
	uint16_t s_firstdatazone;
	uint16_t s_log_zone_size;
	uint32_t s_max_size;
	uint16_t s_magic;
	uint16_t s_state;
	uint32_t s_zones;
};

struct minix_dir_entry {
	uint16_t inode;
	char name[0];
};


#define NAME_MAX         255   /* # chars in a file name */

#define MINIX_INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct minix_inode)))

#ifndef BLKGETSIZE
#define BLKGETSIZE _IO(0x12,96)    /* return device size */
#endif

#ifndef __linux__
#define volatile
#endif

enum { ROOT_INO = 1 };

#define UPPER(size,n) ((size+((n)-1))/(n))
#define INODE_SIZE (sizeof(struct minix_inode))
#ifdef CONFIG_FEATURE_MINIX2
#define INODE_SIZE2 (sizeof(struct minix2_inode))
#define INODE_BLOCKS UPPER(INODES, (version2 ? MINIX2_INODES_PER_BLOCK \
				    : MINIX_INODES_PER_BLOCK))
#else
#define INODE_BLOCKS UPPER(INODES, (MINIX_INODES_PER_BLOCK))
#endif
#define INODE_BUFFER_SIZE (INODE_BLOCKS * BLOCK_SIZE)

#define BITS_PER_BLOCK (BLOCK_SIZE<<3)

static char *program_version = "1.2 - 11/11/96";
static char *device_name;
static int IN;
static int repair, automatic, verbose, list, show, warn_mode, force;
static int directory, regular, blockdev, chardev, links, symlinks, total;

static int changed;			/* flags if the filesystem has been changed */
static int errors_uncorrected;	/* flag if some error was not corrected */
static int dirsize = 16;
static int namelen = 14;
static struct termios termios;
static int termios_set;

static char *inode_buffer;
#define Inode (((struct minix_inode *) inode_buffer)-1)
#define Inode2 (((struct minix2_inode *) inode_buffer)-1)
static char super_block_buffer[BLOCK_SIZE];

#define Super (*(struct minix_super_block *)super_block_buffer)
#define INODES ((unsigned long)Super.s_ninodes)
#ifdef CONFIG_FEATURE_MINIX2
static int version2;
#define ZONES ((unsigned long)(version2 ? Super.s_zones : Super.s_nzones))
#else
#define ZONES ((unsigned long)(Super.s_nzones))
#endif
#define IMAPS ((unsigned long)Super.s_imap_blocks)
#define ZMAPS ((unsigned long)Super.s_zmap_blocks)
#define FIRSTZONE ((unsigned long)Super.s_firstdatazone)
#define ZONESIZE ((unsigned long)Super.s_log_zone_size)
#define MAXSIZE ((unsigned long)Super.s_max_size)
#define MAGIC (Super.s_magic)
#define NORM_FIRSTZONE (2+IMAPS+ZMAPS+INODE_BLOCKS)

static char *inode_map;
static char *zone_map;

static unsigned char *inode_count;
static unsigned char *zone_count;

static void recursive_check(unsigned int ino);
#ifdef CONFIG_FEATURE_MINIX2
static void recursive_check2(unsigned int ino);
#endif

static int bit(char *a, unsigned int i)
{
	  return (a[i >> 3] & (1<<(i & 7))) != 0;
}
#define inode_in_use(x) (bit(inode_map,(x)))
#define zone_in_use(x) (bit(zone_map,(x)-FIRSTZONE+1))

#define mark_inode(x) (setbit(inode_map,(x)),changed=1)
#define unmark_inode(x) (clrbit(inode_map,(x)),changed=1)

#define mark_zone(x) (setbit(zone_map,(x)-FIRSTZONE+1),changed=1)
#define unmark_zone(x) (clrbit(zone_map,(x)-FIRSTZONE+1),changed=1)

static void leave(int) ATTRIBUTE_NORETURN;
static void leave(int status)
{
	if (termios_set)
		tcsetattr(0, TCSANOW, &termios);
	exit(status);
}

static void die(const char *str)
{
	bb_error_msg("%s", str);
	leave(8);
}

/* File-name data */
enum { MAX_DEPTH = 32 };
static int name_depth;
static char *current_name;
static char *name_component[MAX_DEPTH+1];

/* Wed Feb  9 15:17:06 MST 2000 */
/* dynamically allocate name_list (instead of making it static) */
static void alloc_current_name(void)
{
	current_name = xmalloc(MAX_DEPTH * (BUFSIZ + 1));
	current_name[0] = '/';
	current_name[1] = '\0';
	name_component[0] = &current_name[0];
}

#ifdef CONFIG_FEATURE_CLEAN_UP
/* execute this atexit() to deallocate name_list[] */
/* piptigger was here */
static void free_current_name(void)
{
	free(current_name);
}
#endif

static void push_filename(const char *name)
{
	//  /dir/dir/dir/file
	//  ^   ^   ^
	// [0] [1] [2] <-name_component[i]
	if (name_depth < MAX_DEPTH) {
		int len;
		char *p = name_component[name_depth];
		*p++ = '/';
		len = sprintf(p, "%.*s", namelen, name);
		name_component[name_depth + 1] = p + len;
	}
	name_depth++;
}

static void pop_filename(void) {
	name_depth--;
	if (name_depth < MAX_DEPTH) {
		*name_component[name_depth] = '\0';
		if (!name_depth) {
			current_name[0] = '/';
			current_name[1] = '\0';
		}
	}
}

static int ask(const char *string, int def)
{
	int c;

	if (!repair) {
		puts("");
		errors_uncorrected = 1;
		return 0;
	}
	if (automatic) {
		puts("");
		if (!def)
			errors_uncorrected = 1;
		return def;
	}
	printf(def ? "%s (y/n)? " : "%s (n/y)? ", string);
	for (;;) {
		fflush(stdout);
		if ((c = getchar()) == EOF) {
			if (!def)
				errors_uncorrected = 1;
			return def;
		}
		c = toupper(c);
		if (c == 'Y') {
			def = 1;
			break;
		} else if (c == 'N') {
			def = 0;
			break;
		} else if (c == ' ' || c == '\n')
			break;
	}
	if (def)
		printf("y\n");
	else {
		printf("n\n");
		errors_uncorrected = 1;
	}
	return def;
}

/*
 * Make certain that we aren't checking a filesystem that is on a
 * mounted partition.  Code adapted from e2fsck, Copyright (C) 1993,
 * 1994 Theodore Ts'o.  Also licensed under GPL.
 */
static void check_mount(void)
{
	FILE *f;
	struct mntent *mnt;
	int cont;
	int fd;

	if ((f = setmntent(MOUNTED, "r")) == NULL)
		return;
	while ((mnt = getmntent(f)) != NULL)
		if (strcmp(device_name, mnt->mnt_fsname) == 0)
			break;
	endmntent(f);
	if (!mnt)
		return;

	/*
	 * If the root is mounted read-only, then /etc/mtab is
	 * probably not correct; so we won't issue a warning based on
	 * it.
	 */
	fd = open(MOUNTED, O_RDWR);
	if (fd < 0 && errno == EROFS)
		return;
	else
		close(fd);

	printf("%s is mounted. ", device_name);
	cont = 0;
	if (isatty(0) && isatty(1))
		cont = ask("Do you really want to continue", 0);
	if (!cont) {
		printf("Check aborted\n");
		exit(0);
	}
	return;
}

/*
 * check_zone_nr checks to see that *nr is a valid zone nr. If it
 * isn't, it will possibly be repaired. Check_zone_nr sets *corrected
 * if an error was corrected, and returns the zone (0 for no zone
 * or a bad zone-number).
 */
static int check_zone_nr2(uint32_t *nr, int *corrected)
{
	const char *msg;
	if (!*nr)
		return 0;
	if (*nr < FIRSTZONE)
		msg = "< FIRSTZONE";
	else if (*nr >= ZONES)
		msg = ">= ZONES";
	else
		return *nr;
	printf("Zone nr %s in file '%s'. ", msg, current_name);
	if (ask("Remove block", 1)) {
		*nr = 0;
		*corrected = 1;
	}
	return 0;
}

static int check_zone_nr(uint16_t *nr, int *corrected)
{
	uint32_t nr32 = *nr;
	int r = check_zone_nr2(&nr32, corrected);
	*nr = (uint16_t)nr32;
	return r;
}

/*
 * read-block reads block nr into the buffer at addr.
 */
static void read_block(unsigned int nr, char *addr)
{
	if (!nr) {
		memset(addr, 0, BLOCK_SIZE);
		return;
	}
	if (BLOCK_SIZE * nr != lseek(IN, BLOCK_SIZE * nr, SEEK_SET)) {
		printf("%s: cannot seek to block in file '%s'\n",
				bb_msg_read_error, current_name);
		errors_uncorrected = 1;
		memset(addr, 0, BLOCK_SIZE);
	} else if (BLOCK_SIZE != read(IN, addr, BLOCK_SIZE)) {
		printf("%s: bad block in file '%s'\n",
				bb_msg_read_error, current_name);
		errors_uncorrected = 1;
		memset(addr, 0, BLOCK_SIZE);
	}
}

/*
 * write_block writes block nr to disk.
 */
static void write_block(unsigned int nr, char *addr)
{
	if (!nr)
		return;
	if (nr < FIRSTZONE || nr >= ZONES) {
		printf("Internal error: trying to write bad block\n"
			   "Write request ignored\n");
		errors_uncorrected = 1;
		return;
	}
	if (BLOCK_SIZE * nr != lseek(IN, BLOCK_SIZE * nr, SEEK_SET))
		die("Seek failed in write_block");
	if (BLOCK_SIZE != write(IN, addr, BLOCK_SIZE)) {
		printf("%s: bad block in file '%s'\n",
				bb_msg_write_error, current_name);
		errors_uncorrected = 1;
	}
}

/*
 * map_block calculates the absolute block nr of a block in a file.
 * It sets 'changed' if the inode has needed changing, and re-writes
 * any indirect blocks with errors.
 */
static int map_block(struct minix_inode *inode, unsigned int blknr)
{
	uint16_t ind[BLOCK_SIZE >> 1];
	uint16_t dind[BLOCK_SIZE >> 1];
	int blk_chg, block, result;

	if (blknr < 7)
		return check_zone_nr(inode->i_zone + blknr, &changed);
	blknr -= 7;
	if (blknr < 512) {
		block = check_zone_nr(inode->i_zone + 7, &changed);
		read_block(block, (char *) ind);
		blk_chg = 0;
		result = check_zone_nr(blknr + ind, &blk_chg);
		if (blk_chg)
			write_block(block, (char *) ind);
		return result;
	}
	blknr -= 512;
	block = check_zone_nr(inode->i_zone + 8, &changed);
	read_block(block, (char *) dind);
	blk_chg = 0;
	result = check_zone_nr(dind + (blknr / 512), &blk_chg);
	if (blk_chg)
		write_block(block, (char *) dind);
	block = result;
	read_block(block, (char *) ind);
	blk_chg = 0;
	result = check_zone_nr(ind + (blknr % 512), &blk_chg);
	if (blk_chg)
		write_block(block, (char *) ind);
	return result;
}

#ifdef CONFIG_FEATURE_MINIX2
static int map_block2(struct minix2_inode *inode, unsigned int blknr)
{
	uint32_t ind[BLOCK_SIZE >> 2];
	uint32_t dind[BLOCK_SIZE >> 2];
	uint32_t tind[BLOCK_SIZE >> 2];
	int blk_chg, block, result;

	if (blknr < 7)
		return check_zone_nr2(inode->i_zone + blknr, &changed);
	blknr -= 7;
	if (blknr < 256) {
		block = check_zone_nr2(inode->i_zone + 7, &changed);
		read_block(block, (char *) ind);
		blk_chg = 0;
		result = check_zone_nr2(blknr + ind, &blk_chg);
		if (blk_chg)
			write_block(block, (char *) ind);
		return result;
	}
	blknr -= 256;
	if (blknr >= 256 * 256) {
		block = check_zone_nr2(inode->i_zone + 8, &changed);
		read_block(block, (char *) dind);
		blk_chg = 0;
		result = check_zone_nr2(dind + blknr / 256, &blk_chg);
		if (blk_chg)
			write_block(block, (char *) dind);
		block = result;
		read_block(block, (char *) ind);
		blk_chg = 0;
		result = check_zone_nr2(ind + blknr % 256, &blk_chg);
		if (blk_chg)
			write_block(block, (char *) ind);
		return result;
	}
	blknr -= 256 * 256;
	block = check_zone_nr2(inode->i_zone + 9, &changed);
	read_block(block, (char *) tind);
	blk_chg = 0;
	result = check_zone_nr2(tind + blknr / (256 * 256), &blk_chg);
	if (blk_chg)
		write_block(block, (char *) tind);
	block = result;
	read_block(block, (char *) dind);
	blk_chg = 0;
	result = check_zone_nr2(dind + (blknr / 256) % 256, &blk_chg);
	if (blk_chg)
		write_block(block, (char *) dind);
	block = result;
	read_block(block, (char *) ind);
	blk_chg = 0;
	result = check_zone_nr2(ind + blknr % 256, &blk_chg);
	if (blk_chg)
		write_block(block, (char *) ind);
	return result;
}
#endif

static void write_super_block(void)
{
	/*
	 * Set the state of the filesystem based on whether or not there
	 * are uncorrected errors.  The filesystem valid flag is
	 * unconditionally set if we get this far.
	 */
	Super.s_state |= MINIX_VALID_FS;
	if (errors_uncorrected)
		Super.s_state |= MINIX_ERROR_FS;
	else
		Super.s_state &= ~MINIX_ERROR_FS;

	if (BLOCK_SIZE != lseek(IN, BLOCK_SIZE, SEEK_SET))
		die("Seek failed in write_super_block");
	if (BLOCK_SIZE != write(IN, super_block_buffer, BLOCK_SIZE))
		die("Unable to write super-block");
}

static void write_tables(void)
{
	write_super_block();

	if (IMAPS * BLOCK_SIZE != write(IN, inode_map, IMAPS * BLOCK_SIZE))
		die("Unable to write inode map");
	if (ZMAPS * BLOCK_SIZE != write(IN, zone_map, ZMAPS * BLOCK_SIZE))
		die("Unable to write zone map");
	if (INODE_BUFFER_SIZE != write(IN, inode_buffer, INODE_BUFFER_SIZE))
		die("Unable to write inodes");
}

static void get_dirsize(void)
{
	int block;
	char blk[BLOCK_SIZE];
	int size;

#ifdef CONFIG_FEATURE_MINIX2
	if (version2)
		block = Inode2[ROOT_INO].i_zone[0];
	else
#endif
		block = Inode[ROOT_INO].i_zone[0];
	read_block(block, blk);
	for (size = 16; size < BLOCK_SIZE; size <<= 1) {
		if (strcmp(blk + size + 2, "..") == 0) {
			dirsize = size;
			namelen = size - 2;
			return;
		}
	}
	/* use defaults */
}

static void read_superblock(void)
{
	if (BLOCK_SIZE != lseek(IN, BLOCK_SIZE, SEEK_SET))
		die("Seek failed");
	if (BLOCK_SIZE != read(IN, super_block_buffer, BLOCK_SIZE))
		die("Unable to read super block");
	/* already initialized to:
	namelen = 14;
	dirsize = 16;
	version2 = 0;
	*/
	if (MAGIC == MINIX_SUPER_MAGIC) {
	} else if (MAGIC == MINIX_SUPER_MAGIC2) {
		namelen = 30;
		dirsize = 32;
#ifdef CONFIG_FEATURE_MINIX2
	} else if (MAGIC == MINIX2_SUPER_MAGIC) {
		version2 = 1;
	} else if (MAGIC == MINIX2_SUPER_MAGIC2) {
		namelen = 30;
		dirsize = 32;
		version2 = 1;
#endif
	} else
		die("Bad magic number in super-block");
	if (ZONESIZE != 0 || BLOCK_SIZE != 1024)
		die("Only 1k blocks/zones supported");
	if (IMAPS * BLOCK_SIZE * 8 < INODES + 1)
		die("Bad s_imap_blocks field in super-block");
	if (ZMAPS * BLOCK_SIZE * 8 < ZONES - FIRSTZONE + 1)
		die("Bad s_zmap_blocks field in super-block");
}

static void read_tables(void)
{
	inode_map = xzalloc(IMAPS * BLOCK_SIZE);
	zone_map = xzalloc(ZMAPS * BLOCK_SIZE);
	inode_buffer = xmalloc(INODE_BUFFER_SIZE);
	inode_count = xmalloc(INODES + 1);
	zone_count = xmalloc(ZONES);
	if (IMAPS * BLOCK_SIZE != read(IN, inode_map, IMAPS * BLOCK_SIZE))
		die("Unable to read inode map");
	if (ZMAPS * BLOCK_SIZE != read(IN, zone_map, ZMAPS * BLOCK_SIZE))
		die("Unable to read zone map");
	if (INODE_BUFFER_SIZE != read(IN, inode_buffer, INODE_BUFFER_SIZE))
		die("Unable to read inodes");
	if (NORM_FIRSTZONE != FIRSTZONE) {
		printf("Warning: Firstzone!=Norm_firstzone\n");
		errors_uncorrected = 1;
	}
	get_dirsize();
	if (show) {
		printf("%ld inodes\n"
			"%ld blocks\n"
			"Firstdatazone=%ld (%ld)\n"
			"Zonesize=%d\n"
			"Maxsize=%ld\n"
			"Filesystem state=%d\n"
			"namelen=%d\n\n",
			INODES,
			ZONES,
			FIRSTZONE, NORM_FIRSTZONE,
			BLOCK_SIZE << ZONESIZE,
			MAXSIZE,
			Super.s_state,
			namelen);
	}
}

static struct minix_inode *get_inode(unsigned int nr)
{
	struct minix_inode *inode;

	if (!nr || nr > INODES)
		return NULL;
	total++;
	inode = Inode + nr;
	if (!inode_count[nr]) {
		if (!inode_in_use(nr)) {
			printf("Inode %d is marked as 'unused', but it is used "
					"for file '%s'\n", nr, current_name);
			if (repair) {
				if (ask("Mark as 'in use'", 1))
					mark_inode(nr);
			} else {
				errors_uncorrected = 1;
			}
		}
		if (S_ISDIR(inode->i_mode))
			directory++;
		else if (S_ISREG(inode->i_mode))
			regular++;
		else if (S_ISCHR(inode->i_mode))
			chardev++;
		else if (S_ISBLK(inode->i_mode))
			blockdev++;
		else if (S_ISLNK(inode->i_mode))
			symlinks++;
		else if (S_ISSOCK(inode->i_mode));
		else if (S_ISFIFO(inode->i_mode));
		else {
			printf("%s has mode %05o\n", current_name, inode->i_mode);
		}

	} else
		links++;
	if (!++inode_count[nr]) {
		printf("Warning: inode count too big\n");
		inode_count[nr]--;
		errors_uncorrected = 1;
	}
	return inode;
}

#ifdef CONFIG_FEATURE_MINIX2
static struct minix2_inode *get_inode2(unsigned int nr)
{
	struct minix2_inode *inode;

	if (!nr || nr > INODES)
		return NULL;
	total++;
	inode = Inode2 + nr;
	if (!inode_count[nr]) {
		if (!inode_in_use(nr)) {
			printf("Inode %d is marked as 'unused', but it is used "
					"for file '%s'\n", nr, current_name);
			if (repair) {
				if (ask("Mark as 'in use'", 1))
					mark_inode(nr);
				else
					errors_uncorrected = 1;
			}
		}
		if (S_ISDIR(inode->i_mode))
			directory++;
		else if (S_ISREG(inode->i_mode))
			regular++;
		else if (S_ISCHR(inode->i_mode))
			chardev++;
		else if (S_ISBLK(inode->i_mode))
			blockdev++;
		else if (S_ISLNK(inode->i_mode))
			symlinks++;
		else if (S_ISSOCK(inode->i_mode));
		else if (S_ISFIFO(inode->i_mode));
		else {
			printf("%s has mode %05o\n", current_name, inode->i_mode);
		}
	} else
		links++;
	if (!++inode_count[nr]) {
		printf("Warning: inode count too big\n");
		inode_count[nr]--;
		errors_uncorrected = 1;
	}
	return inode;
}
#endif

static void check_root(void)
{
	struct minix_inode *inode = Inode + ROOT_INO;

	if (!inode || !S_ISDIR(inode->i_mode))
		die("Root inode isn't a directory");
}

#ifdef CONFIG_FEATURE_MINIX2
static void check_root2(void)
{
	struct minix2_inode *inode = Inode2 + ROOT_INO;

	if (!inode || !S_ISDIR(inode->i_mode))
		die("Root inode isn't a directory");
}
#endif

static int add_zone(uint16_t *znr, int *corrected)
{
	int result;
	int block;

	result = 0;
	block = check_zone_nr(znr, corrected);
	if (!block)
		return 0;
	if (zone_count[block]) {
		printf("Already used block is reused in file '%s'. ",
				current_name);
		if (ask("Clear", 1)) {
			*znr = 0;
			block = 0;
			*corrected = 1;
			return 0;
		}
	}
	if (!zone_in_use(block)) {
		printf("Block %d in file '%s' is marked as 'unused'. ",
				block, current_name);
		if (ask("Correct", 1))
			mark_zone(block);
	}
	if (!++zone_count[block])
		zone_count[block]--;
	return block;
}

#ifdef CONFIG_FEATURE_MINIX2
static int add_zone2(uint32_t *znr, int *corrected)
{
	int result;
	int block;

	result = 0;
	block = check_zone_nr2(znr, corrected);
	if (!block)
		return 0;
	if (zone_count[block]) {
		printf("Already used block is reused in file '%s'. ",
				current_name);
		if (ask("Clear", 1)) {
			*znr = 0;
			block = 0;
			*corrected = 1;
			return 0;
		}
	}
	if (!zone_in_use(block)) {
		printf("Block %d in file '%s' is marked as 'unused'. ",
				block, current_name);
		if (ask("Correct", 1))
			mark_zone(block);
	}
	if (!++zone_count[block])
		zone_count[block]--;
	return block;
}
#endif

static void add_zone_ind(uint16_t *znr, int *corrected)
{
	static char blk[BLOCK_SIZE];
	int i, chg_blk = 0;
	int block;

	block = add_zone(znr, corrected);
	if (!block)
		return;
	read_block(block, blk);
	for (i = 0; i < (BLOCK_SIZE >> 1); i++)
		add_zone(i + (uint16_t *) blk, &chg_blk);
	if (chg_blk)
		write_block(block, blk);
}

#ifdef CONFIG_FEATURE_MINIX2
static void add_zone_ind2(uint32_t *znr, int *corrected)
{
	static char blk[BLOCK_SIZE];
	int i, chg_blk = 0;
	int block;

	block = add_zone2(znr, corrected);
	if (!block)
		return;
	read_block(block, blk);
	for (i = 0; i < BLOCK_SIZE >> 2; i++)
		add_zone2(i + (uint32_t *) blk, &chg_blk);
	if (chg_blk)
		write_block(block, blk);
}
#endif

static void add_zone_dind(uint16_t *znr, int *corrected)
{
	static char blk[BLOCK_SIZE];
	int i, blk_chg = 0;
	int block;

	block = add_zone(znr, corrected);
	if (!block)
		return;
	read_block(block, blk);
	for (i = 0; i < (BLOCK_SIZE >> 1); i++)
		add_zone_ind(i + (uint16_t *) blk, &blk_chg);
	if (blk_chg)
		write_block(block, blk);
}

#ifdef CONFIG_FEATURE_MINIX2
static void add_zone_dind2(uint32_t *znr, int *corrected)
{
	static char blk[BLOCK_SIZE];
	int i, blk_chg = 0;
	int block;

	block = add_zone2(znr, corrected);
	if (!block)
		return;
	read_block(block, blk);
	for (i = 0; i < BLOCK_SIZE >> 2; i++)
		add_zone_ind2(i + (uint32_t *) blk, &blk_chg);
	if (blk_chg)
		write_block(block, blk);
}

static void add_zone_tind2(uint32_t *znr, int *corrected)
{
	static char blk[BLOCK_SIZE];
	int i, blk_chg = 0;
	int block;

	block = add_zone2(znr, corrected);
	if (!block)
		return;
	read_block(block, blk);
	for (i = 0; i < BLOCK_SIZE >> 2; i++)
		add_zone_dind2(i + (uint32_t *) blk, &blk_chg);
	if (blk_chg)
		write_block(block, blk);
}
#endif

static void check_zones(unsigned int i)
{
	struct minix_inode *inode;

	if (!i || i > INODES)
		return;
	if (inode_count[i] > 1)		/* have we counted this file already? */
		return;
	inode = Inode + i;
	if (!S_ISDIR(inode->i_mode) && !S_ISREG(inode->i_mode) &&
		!S_ISLNK(inode->i_mode)) return;
	for (i = 0; i < 7; i++)
		add_zone(i + inode->i_zone, &changed);
	add_zone_ind(7 + inode->i_zone, &changed);
	add_zone_dind(8 + inode->i_zone, &changed);
}

#ifdef CONFIG_FEATURE_MINIX2
static void check_zones2(unsigned int i)
{
	struct minix2_inode *inode;

	if (!i || i > INODES)
		return;
	if (inode_count[i] > 1)		/* have we counted this file already? */
		return;
	inode = Inode2 + i;
	if (!S_ISDIR(inode->i_mode) && !S_ISREG(inode->i_mode)
		&& !S_ISLNK(inode->i_mode))
		return;
	for (i = 0; i < 7; i++)
		add_zone2(i + inode->i_zone, &changed);
	add_zone_ind2(7 + inode->i_zone, &changed);
	add_zone_dind2(8 + inode->i_zone, &changed);
	add_zone_tind2(9 + inode->i_zone, &changed);
}
#endif

static void check_file(struct minix_inode *dir, unsigned int offset)
{
	static char blk[BLOCK_SIZE];
	struct minix_inode *inode;
	int ino;
	char *name;
	int block;

	block = map_block(dir, offset / BLOCK_SIZE);
	read_block(block, blk);
	name = blk + (offset % BLOCK_SIZE) + 2;
	ino = *(uint16_t *) (name - 2);
	if (ino > INODES) {
		printf("%s contains a bad inode number for file '%.*s'. ",
				current_name, namelen, name);
		if (ask("Remove", 1)) {
			*(uint16_t *) (name - 2) = 0;
			write_block(block, blk);
		}
		ino = 0;
	}
	push_filename(name);
	inode = get_inode(ino);
	pop_filename();
	if (!offset) {
		if (!inode || strcmp(".", name)) {
			printf("%s: bad directory: '.' isn't first\n", current_name);
			errors_uncorrected = 1;
		} else
			return;
	}
	if (offset == dirsize) {
		if (!inode || strcmp("..", name)) {
			printf("%s: bad directory: '..' isn't second\n", current_name);
			errors_uncorrected = 1;
		} else
			return;
	}
	if (!inode)
		return;
	push_filename(name);
	if (list) {
		if (verbose)
			printf("%6d %07o %3d ", ino, inode->i_mode, inode->i_nlinks);
		printf("%s%s\n", current_name, S_ISDIR(inode->i_mode) ? ":" : "");
	}
	check_zones(ino);
	if (inode && S_ISDIR(inode->i_mode))
		recursive_check(ino);
	pop_filename();
	return;
}

#ifdef CONFIG_FEATURE_MINIX2
static void check_file2(struct minix2_inode *dir, unsigned int offset)
{
	static char blk[BLOCK_SIZE];
	struct minix2_inode *inode;
	int ino;
	char *name;
	int block;

	block = map_block2(dir, offset / BLOCK_SIZE);
	read_block(block, blk);
	name = blk + (offset % BLOCK_SIZE) + 2;
	ino = *(uint16_t *) (name - 2);
	if (ino > INODES) {
		printf("%s contains a bad inode number for file '%.*s'. ",
				current_name, namelen, name);
		if (ask("Remove", 1)) {
			*(uint16_t *) (name - 2) = 0;
			write_block(block, blk);
		}
		ino = 0;
	}
	push_filename(name);
	inode = get_inode2(ino);
	pop_filename();
	if (!offset) {
		if (!inode || strcmp(".", name)) {
			printf("%s: bad directory: '.' isn't first\n", current_name);
			errors_uncorrected = 1;
		} else
			return;
	}
	if (offset == dirsize) {
		if (!inode || strcmp("..", name)) {
			printf("%s: bad directory: '..' isn't second\n", current_name);
			errors_uncorrected = 1;
		} else
			return;
	}
	if (!inode)
		return;
	push_filename(name);
	if (list) {
		if (verbose)
			printf("%6d %07o %3d ", ino, inode->i_mode, inode->i_nlinks);
		printf("%s%s\n", current_name, S_ISDIR(inode->i_mode) ? ":" : "");
	}
	check_zones2(ino);
	if (inode && S_ISDIR(inode->i_mode))
		recursive_check2(ino);
	pop_filename();
	return;
}
#endif

static void recursive_check(unsigned int ino)
{
	struct minix_inode *dir;
	unsigned int offset;

	dir = Inode + ino;
	if (!S_ISDIR(dir->i_mode))
		die("Internal error");
	if (dir->i_size < 2 * dirsize) {
		printf("%s: bad directory: size<32", current_name);
		errors_uncorrected = 1;
	}
	for (offset = 0; offset < dir->i_size; offset += dirsize)
		check_file(dir, offset);
}

#ifdef CONFIG_FEATURE_MINIX2
static void recursive_check2(unsigned int ino)
{
	struct minix2_inode *dir;
	unsigned int offset;

	dir = Inode2 + ino;
	if (!S_ISDIR(dir->i_mode))
		die("Internal error");
	if (dir->i_size < 2 * dirsize) {
		printf("%s: bad directory: size<32", current_name);
		errors_uncorrected = 1;
	}
	for (offset = 0; offset < dir->i_size; offset += dirsize)
		check_file2(dir, offset);
}
#endif

static int bad_zone(int i)
{
	char buffer[1024];

	if (BLOCK_SIZE * i != lseek(IN, BLOCK_SIZE * i, SEEK_SET))
		die("Seek failed in bad_zone");
	return (BLOCK_SIZE != read(IN, buffer, BLOCK_SIZE));
}

static void check_counts(void)
{
	int i;

	for (i = 1; i <= INODES; i++) {
		if (warn_mode && Inode[i].i_mode && !inode_in_use(i)) {
			printf("Inode %d has non-zero mode. ", i);
			if (ask("Clear", 1)) {
				Inode[i].i_mode = 0;
				changed = 1;
			}
		}
		if (!inode_count[i]) {
			if (!inode_in_use(i))
				continue;
			printf("Unused inode %d is marked as 'used' in the bitmap. ", i);
			if (ask("Clear", 1))
				unmark_inode(i);
			continue;
		}
		if (!inode_in_use(i)) {
			printf("Inode %d is used, but marked as 'unused' in the bitmap. ", i);
			if (ask("Set", 1))
				mark_inode(i);
		}
		if (Inode[i].i_nlinks != inode_count[i]) {
			printf("Inode %d (mode=%07o), i_nlinks=%d, counted=%d. ",
				   i, Inode[i].i_mode, Inode[i].i_nlinks, inode_count[i]);
			if (ask("Set i_nlinks to count", 1)) {
				Inode[i].i_nlinks = inode_count[i];
				changed = 1;
			}
		}
	}
	for (i = FIRSTZONE; i < ZONES; i++) {
		if (zone_in_use(i) == zone_count[i])
			continue;
		if (!zone_count[i]) {
			if (bad_zone(i))
				continue;
			printf("Zone %d is marked 'in use', but no file uses it. ", i);
			if (ask("Unmark", 1))
				unmark_zone(i);
			continue;
		}
		printf("Zone %d: %sin use, counted=%d\n",
			   i, zone_in_use(i) ? "" : "not ", zone_count[i]);
	}
}

#ifdef CONFIG_FEATURE_MINIX2
static void check_counts2(void)
{
	int i;

	for (i = 1; i <= INODES; i++) {
		if (warn_mode && Inode2[i].i_mode && !inode_in_use(i)) {
			printf("Inode %d has non-zero mode. ", i);
			if (ask("Clear", 1)) {
				Inode2[i].i_mode = 0;
				changed = 1;
			}
		}
		if (!inode_count[i]) {
			if (!inode_in_use(i))
				continue;
			printf("Unused inode %d is marked as 'used' in the bitmap. ", i);
			if (ask("Clear", 1))
				unmark_inode(i);
			continue;
		}
		if (!inode_in_use(i)) {
			printf("Inode %d is used, but marked as 'unused' in the bitmap. ", i);
			if (ask("Set", 1))
				mark_inode(i);
		}
		if (Inode2[i].i_nlinks != inode_count[i]) {
			printf("Inode %d (mode=%07o), i_nlinks=%d, counted=%d. ",
				   i, Inode2[i].i_mode, Inode2[i].i_nlinks,
				   inode_count[i]);
			if (ask("Set i_nlinks to count", 1)) {
				Inode2[i].i_nlinks = inode_count[i];
				changed = 1;
			}
		}
	}
	for (i = FIRSTZONE; i < ZONES; i++) {
		if (zone_in_use(i) == zone_count[i])
			continue;
		if (!zone_count[i]) {
			if (bad_zone(i))
				continue;
			printf("Zone %d is marked 'in use', but no file uses it. ", i);
			if (ask("Unmark", 1))
				unmark_zone(i);
			continue;
		}
		printf("Zone %d: %sin use, counted=%d\n",
			   i, zone_in_use(i) ? "" : "not ", zone_count[i]);
	}
}
#endif

static void check(void)
{
	memset(inode_count, 0, (INODES + 1) * sizeof(*inode_count));
	memset(zone_count, 0, ZONES * sizeof(*zone_count));
	check_zones(ROOT_INO);
	recursive_check(ROOT_INO);
	check_counts();
}

#ifdef CONFIG_FEATURE_MINIX2
static void check2(void)
{
	memset(inode_count, 0, (INODES + 1) * sizeof(*inode_count));
	memset(zone_count, 0, ZONES * sizeof(*zone_count));
	check_zones2(ROOT_INO);
	recursive_check2(ROOT_INO);
	check_counts2();
}
#endif

int fsck_minix_main(int argc, char **argv)
{
	struct termios tmp;
	int retcode = 0;

	alloc_current_name();
#ifdef CONFIG_FEATURE_CLEAN_UP
	/* Don't bother to free memory.  Exit does
	 * that automagically, so we can save a few bytes */
	atexit(free_current_name);
#endif

	if (INODE_SIZE * MINIX_INODES_PER_BLOCK != BLOCK_SIZE)
		die("Bad inode size");
#ifdef CONFIG_FEATURE_MINIX2
	if (INODE_SIZE2 * MINIX2_INODES_PER_BLOCK != BLOCK_SIZE)
		die("Bad v2 inode size");
#endif
	while (argc-- > 1) {
		argv++;
		if (argv[0][0] != '-') {
			if (device_name)
				bb_show_usage();
			else
				device_name = argv[0];
		} else
			while (*++argv[0])
				switch (argv[0][0]) {
				case 'l':
					list = 1;
					break;
				case 'a':
					automatic = 1;
					repair = 1;
					break;
				case 'r':
					automatic = 0;
					repair = 1;
					break;
				case 'v':
					verbose = 1;
					break;
				case 's':
					show = 1;
					break;
				case 'm':
					warn_mode = 1;
					break;
				case 'f':
					force = 1;
					break;
				default:
					bb_show_usage();
				}
	}
	if (!device_name)
		bb_show_usage();
	check_mount();				/* trying to check a mounted filesystem? */
	if (repair && !automatic) {
		if (!isatty(0) || !isatty(1))
			die("Need terminal for interactive repairs");
	}
	IN = open(device_name, repair ? O_RDWR : O_RDONLY);
	if (IN < 0){
		printf("Unable to open device '%s'\n", device_name);
		leave(8);
	}
	sync(); /* paranoia? */
	read_superblock();

	/*
	 * Determine whether or not we should continue with the checking.
	 * This is based on the status of the filesystem valid and error
	 * flags and whether or not the -f switch was specified on the
	 * command line.
	 */
	printf("%s, %s\n", applet_name, program_version);
	if (!(Super.s_state & MINIX_ERROR_FS) &&
		(Super.s_state & MINIX_VALID_FS) && !force) {
		if (repair)
			printf("%s is clean, check is skipped\n", device_name);
		return retcode;
	} else if (force)
		printf("Forcing filesystem check on %s\n", device_name);
	else if (repair)
		printf("Filesystem on %s is dirty, needs checking\n",
			   device_name);

	read_tables();

	if (repair && !automatic) {
		tcgetattr(0, &termios);
		tmp = termios;
		tmp.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(0, TCSANOW, &tmp);
		termios_set = 1;
	}
#ifdef CONFIG_FEATURE_MINIX2
	if (version2) {
		check_root2();
		check2();
	} else
#endif
	{
		check_root();
		check();
	}
	if (verbose) {
		int i, free_cnt;

		for (i = 1, free_cnt = 0; i <= INODES; i++)
			if (!inode_in_use(i))
				free_cnt++;
		printf("\n%6ld inodes used (%ld%%)\n", (INODES - free_cnt),
			   100 * (INODES - free_cnt) / INODES);
		for (i = FIRSTZONE, free_cnt = 0; i < ZONES; i++)
			if (!zone_in_use(i))
				free_cnt++;
		printf("%6ld zones used (%ld%%)\n\n"
			   "%6d regular files\n"
			   "%6d directories\n"
			   "%6d character device files\n"
			   "%6d block device files\n"
			   "%6d links\n"
			   "%6d symbolic links\n"
			   "------\n"
			   "%6d files\n",
			   (ZONES - free_cnt), 100 * (ZONES - free_cnt) / ZONES,
			   regular, directory, chardev, blockdev,
			   links - 2 * directory + 1, symlinks,
			   total - 2 * directory + 1);
	}
	if (changed) {
		write_tables();
		printf("FILE SYSTEM HAS BEEN CHANGED\n");
		sync();
	} else if (repair)
		write_super_block();

	if (repair && !automatic)
		tcsetattr(0, TCSANOW, &termios);

	if (changed)
		retcode += 3;
	if (errors_uncorrected)
		retcode += 4;
	return retcode;
}
