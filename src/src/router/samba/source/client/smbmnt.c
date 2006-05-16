/*
 *  smbmnt.c
 *
 *  Copyright (C) 1995-1998 by Paal-Kr. Engstad and Volker Lendecke
 *  extensively modified by Tridge
 *
 */

#include "includes.h"

#include <mntent.h>

#include <asm/types.h>
#include <asm/posix_types.h>
#include <linux/smb.h>
#include <linux/smb_mount.h>
#include <asm/unistd.h>

#ifndef	MS_MGC_VAL
/* This may look strange but MS_MGC_VAL is what we are looking for and
	is what we need from <linux/fs.h> under libc systems and is
	provided in standard includes on glibc systems.  So...  We
	switch on what we need...  */
#include <linux/fs.h>
#endif

static uid_t mount_uid;
static gid_t mount_gid;
static int mount_ro;
static unsigned mount_fmask;
static unsigned mount_dmask;
static int user_mount;

static void
help(void)
{
        printf("\n");
        printf("usage: smbmnt mount-point [options]\n");
        printf("-s share       share name on server\n"
               "-r             mount read-only\n"
               "-u uid         mount as uid\n"
               "-g gid         mount as gid\n"
               "-f mask        permission mask for files\n"
               "-d mask        permission mask for directories\n"
               "-h             print this help text\n");
}

static int
parse_args(int argc, char *argv[], struct smb_mount_data *data, char **share)
{
        int opt;

        while ((opt = getopt (argc, argv, "s:u:g:rf:d:")) != EOF)
	{
                switch (opt)
		{
                case 's':
                        *share = optarg;
                        break;
                case 'u':
			if (!user_mount) {
				mount_uid = strtol(optarg, NULL, 0);
			}
                        break;
                case 'g':
			if (!user_mount) {
				mount_gid = strtol(optarg, NULL, 0);
			}
                        break;
                case 'r':
                        mount_ro = 1;
                        break;
                case 'f':
                        mount_fmask = strtol(optarg, NULL, 8);
                        break;
                case 'd':
                        mount_dmask = strtol(optarg, NULL, 8);
                        break;
                default:
                        return -1;
                }
        }
        return 0;
        
}

static char *
fullpath(const char *p)
{
        char path[MAXPATHLEN];

	if (strlen(p) > MAXPATHLEN-1) {
		return NULL;
	}

        if (realpath(p, path) == NULL) {
		fprintf(stderr,"Failed to find real path for mount point\n");
		exit(1);
	}
	return strdup(path);
}

/* Check whether user is allowed to mount on the specified mount point. If it's
   OK then we change into that directory - this prevents race conditions */
static int mount_ok(char *mount_point)
{
	SMB_STRUCT_STAT st;

	if (chdir(mount_point) != 0) {
		return -1;
	}

        if (sys_stat(".", &st) != 0) {
		return -1;
        }

        if (!S_ISDIR(st.st_mode)) {
                errno = ENOTDIR;
                return -1;
        }

        if ((getuid() != 0) && 
	    ((getuid() != st.st_uid) || 
	     ((st.st_mode & S_IRWXU) != S_IRWXU))) {
                errno = EPERM;
                return -1;
        }

        return 0;
}

 int main(int argc, char *argv[])
{
	char *mount_point, *share_name = NULL;
	FILE *mtab;
	int fd;
	unsigned int flags;
	struct smb_mount_data data;
	struct mntent ment;

	memset(&data, 0, sizeof(struct smb_mount_data));

	if (argc < 2) {
		help();
		exit(1);
	}

	if (argv[1][0] == '-') {
		help();
		exit(1);
	}

	if (getuid() != 0) {
		user_mount = 1;
	}

        if (geteuid() != 0) {
                fprintf(stderr, "smbmnt must be installed suid root for direct user mounts (%d,%d)\n", getuid(), geteuid());
                exit(1);
        }

	mount_uid = getuid();
	mount_gid = getgid();
	mount_fmask = umask(0);
        umask(mount_fmask);
	mount_fmask = ~mount_fmask;

        mount_point = fullpath(argv[1]);

        argv += 1;
        argc -= 1;

        if (mount_ok(mount_point) != 0) {
                fprintf(stderr, "cannot mount on %s: %s\n",
                        mount_point, strerror(errno));
                exit(1);
        }

	data.version = SMB_MOUNT_VERSION;

        /* getuid() gives us the real uid, who may umount the fs */
        data.mounted_uid = getuid();

        if (parse_args(argc, argv, &data, &share_name) != 0) {
                help();
                return -1;
        }

        data.uid = mount_uid;
        data.gid = mount_gid;
        data.file_mode = (S_IRWXU|S_IRWXG|S_IRWXO) & mount_fmask;
        data.dir_mode  = (S_IRWXU|S_IRWXG|S_IRWXO) & mount_dmask;

        if (mount_dmask == 0) {
                data.dir_mode = data.file_mode;
                if ((data.dir_mode & S_IRUSR) != 0)
                        data.dir_mode |= S_IXUSR;
                if ((data.dir_mode & S_IRGRP) != 0)
                        data.dir_mode |= S_IXGRP;
                if ((data.dir_mode & S_IROTH) != 0)
                        data.dir_mode |= S_IXOTH;
        }

	flags = MS_MGC_VAL;

	if (mount_ro) flags |= MS_RDONLY;

	if (mount(share_name, ".", "smbfs", flags, (char *)&data) < 0)
	{
		switch (errno) {
		case ENODEV:
			fprintf(stderr, "ERROR: smbfs filesystem not supported by the kernel\n");
			break;
		default:
			perror("mount error");
		}
		fprintf(stderr, "Please refer to the smbmnt(8) manual page\n");
		return -1;
	}

        ment.mnt_fsname = share_name ? share_name : "none";
        ment.mnt_dir = mount_point;
        ment.mnt_type = "smbfs";
        ment.mnt_opts = "";
        ment.mnt_freq = 0;
        ment.mnt_passno= 0;

        mount_point = ment.mnt_dir;

	if (mount_point == NULL)
	{
		fprintf(stderr, "Mount point too long\n");
		return -1;
	}
	
        if ((fd = open(MOUNTED"~", O_RDWR|O_CREAT|O_EXCL, 0600)) == -1)
        {
                fprintf(stderr, "Can't get "MOUNTED"~ lock file");
                return 1;
        }
        close(fd);
	
        if ((mtab = setmntent(MOUNTED, "a+")) == NULL)
        {
                fprintf(stderr, "Can't open " MOUNTED);
                return 1;
        }

        if (addmntent(mtab, &ment) == 1)
        {
                fprintf(stderr, "Can't write mount entry");
                return 1;
        }
        if (fchmod(fileno(mtab), 0644) == -1)
        {
                fprintf(stderr, "Can't set perms on "MOUNTED);
                return 1;
        }
        endmntent(mtab);

        if (unlink(MOUNTED"~") == -1)
        {
                fprintf(stderr, "Can't remove "MOUNTED"~");
                return 1;
        }

	return 0;
}	
