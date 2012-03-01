/* 
 * USB hotplug service    Copyright 2007, Broadcom Corporation  All Rights
 * Reserved.    THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO
 * WARRANTIES OF ANY  KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR
 * OTHERWISE. BROADCOM  SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS  FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT
 * CONCERNING THIS SOFTWARE.    $Id$  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <typedefs.h>
#include <shutils.h>
#include <bcmnvram.h>

static bool usb_ufd_connected(char *str);
static int usb_process_path(char *path, char *fs, char *target);
static void usb_unmount(char *dev);
int usb_add_ufd(char *dev);

#define DUMPFILE	"/tmp/disktype.dump"

#ifdef HAVE_X86
static char *getdisc(void)	// works only for squashfs 
{
	int i;
	static char ret[4];
	unsigned char *disks[] =
	    { "sda2", "sdb2", "sdc2", "sdd2", "sde2", "sdf2", "sdg2", "sdh2",
		"sdi2"
	};
	for (i = 0; i < 9; i++) {
		char dev[64];

		sprintf(dev, "/dev/%s", disks[i]);
		FILE *in = fopen(dev, "rb");

		if (in == NULL)
			continue;	// no second partition or disc does not
		// exist, skipping
		char buf[4];

		fread(buf, 4, 1, in);
		if (buf[0] == 'h' && buf[1] == 's' && buf[2] == 'q'
		    && buf[3] == 't') {
			fclose(in);
			// filesystem detected
			strncpy(ret, disks[i], 3);
			return ret;
		}
		fclose(in);
	}
	return NULL;
}
#endif
void start_hotplug_usb(void)
{
	char *device, *interface;
	char *action;
	int class, subclass, protocol;

	if (!(nvram_match("usb_automnt", "1")))
		return;

	if (!(action = getenv("ACTION")) || !(device = getenv("TYPE")))
		return;
	//sysprintf("echo action %s type %s >> /tmp/hotplugs", action, device);

	sscanf(device, "%d/%d/%d", &class, &subclass, &protocol);

	if (class == 0) {
		if (!(interface = getenv("INTERFACE")))
			goto skip;
		sscanf(interface, "%d/%d/%d", &class, &subclass, &protocol);
	}
      skip:;
	//sysprintf("echo action %d type %d >> /tmp/hotplugs", class, subclass);

	/* 
	 * If a new USB device is added and it is of storage class 
	 */
	if (class == 8 && subclass == 6) {
		if (!strcmp(action, "add"))
			usb_add_ufd(NULL);
		if (!strcmp(action, "remove"))
			usb_unmount(NULL);
	}

	if (class == 0 && subclass == 0) {
		if (!strcmp(action, "add"))
			usb_add_ufd(NULL);
		if (!strcmp(action, "remove"))
			usb_unmount(NULL);
	}

	return;
}

void start_hotplug_block(void)
{
	char *devpath;
	char *action;

	if (!(nvram_match("usb_automnt", "1")))
		return;

	if (!(action = getenv("ACTION")))
		return;
	if (!(devpath = getenv("DEVPATH")))
		return;
	char *slash = strrchr(devpath, '/');
	//sysprintf("echo action %s devpath %s >> /tmp/hotplugs", action,
	//        devpath);

	char devname[64];
	sprintf(devname, "/dev/%s", slash + 1);
	char sysdev[128];
	sprintf(sysdev, "/sys%s/partition", devpath);
	FILE *fp = fopen(sysdev, "rb");
	if (fp) {
		fclose(fp);	// skip partitions
		return;
	}
	sprintf(sysdev, "/sys%s/dev", devpath);
	fp = fopen(sysdev, "rb");
	if (fp) {
		int major;
		int minor;
		fscanf(fp, "%d:%d", &major, &minor);
		sysprintf("mknod %s b %d %d\n", devname, major, minor);
		fclose(fp);	// skip partitions
	}
	//sysprintf("echo add devname %s >> /tmp/hotplugs", devname);
	if (!strcmp(action, "add"))
		usb_add_ufd(devname);
	if (!strcmp(action, "remove"))
		usb_unmount(devname);

	return;
}

    /* 
     *   Check if the UFD is still connected because the links created in
     * /dev/discs are not removed when the UFD is  unplugged.  
     */
static bool usb_ufd_connected(char *str)
{
	uint host_no;
	char proc_file[128];
	FILE *fp;
	char line[256];
	int i;

	/* 
	 * Host no. assigned by scsi driver for this UFD 
	 */
	host_no = atoi(str);
	sprintf(proc_file, "/proc/scsi/usb-storage-%d/%d", host_no, host_no);

	if ((fp = fopen(proc_file, "r"))) {
		while (fgets(line, sizeof(line), fp) != NULL) {
			if (strstr(line, "Attached: Yes")) {
				fclose(fp);
				return TRUE;
			}
		}
		fclose(fp);
	}
	//in 2.6 kernels its a little bit different; find 1st
	for (i = 0; i < 16; i++) {
		sprintf(proc_file, "/proc/scsi/usb-storage/%d", i);
		if (f_exists(proc_file)) {
			return TRUE;
		}
	}
	return FALSE;

}

    /* 
     *   Mount the path and look for the WCN configuration file.  If it
     * exists launch wcnparse to process the configuration.  
     */
static int usb_process_path(char *path, char *fs, char *target)
{
	int ret = ENOENT;
	char mount_point[32];
	eval("stopservice", "samba3");
	eval("stopservice", "ftpsrv");

	if (nvram_match("usb_mntpoint", "mnt") && target)
		sprintf(mount_point, "/mnt/%s", target);
	else
		sprintf(mount_point, "/%s",
			nvram_default_get("usb_mntpoint", "mnt"));
#ifdef HAVE_NTFS3G
	if (!strcmp(fs, "ntfs"))
		insmod("fuse");
#endif
	if (!strcmp(fs, "ext2")) {
		insmod("mbcache");
		insmod("ext2");
	}
#ifdef HAVE_USB_ADVANCED
	if (!strcmp(fs, "ext3")) {
		insmod("mbcache");
		insmod("ext2");
		insmod("jbd");
		insmod("ext3");
	}
	if (!strcmp(fs, "ext4")) {
		insmod("crc16");
		insmod("mbcache");
		insmod("jbd2");
		insmod("ext4");
	}
	if (!strcmp(fs, "btrfs")) {
		insmod("libcrc32c");
		insmod("lzo_compress");
		insmod("lzo_decompress");
		insmod("btrfs");
	}
#endif
	if (!strcmp(fs, "vfat")) {
		insmod("nls_base");
		insmod("nls_cp932");
		insmod("nls_cp936");
		insmod("nls_cp950");
		insmod("nls_cp437");
		insmod("nls_iso8859-1");
		insmod("nls_iso8859-2");
		insmod("nls_utf8");
		insmod("fat");
		insmod("vfat");
		insmod("msdos");
	}
	if (!strcmp(fs, "xfs")) {
		insmod("xfs");
	}
	if (!strcmp(fs, "udf")) {
		insmod("crc-itu-t");
		insmod("udf");
	}
	if (!strcmp(fs, "iso9660")) {
		insmod("isofs");
	}
	if (target)
		sysprintf("mkdir -p /tmp/mnt/%s", target);
	else
		sysprintf("mkdir -p /tmp/mnt");
#ifdef HAVE_NTFS3G
	if (!strcmp(fs, "ntfs")) {
		ret = eval("ntfs-3g", path, mount_point);
	} else
#endif
		ret =
		    eval("/bin/mount", "-t", fs, "-o", "iocharset=utf8", path,
			 mount_point);

	if (ret != 0) {		//give it another try
#ifdef HAVE_NTFS3G
		if (!strcmp(fs, "ntfs")) {
			ret = eval("ntfs-3g", path, mount_point);
		} else
#endif
			ret = eval("/bin/mount", path, mount_point);	//guess fs
	}
	system("echo 4096 > /proc/sys/vm/min_free_kbytes");	// avoid out of memory problems which could lead to broken wireless, so we limit the minimum free ram to 4096. everything else can be used for fs cache
	eval("startservice", "samba3");
	eval("startservice", "ftpsrv");
	return ret;
}

static void usb_unmount(char *path)
{
	char mount_point[32];
	eval("stopservice", "samba3");
	eval("stopservice", "ftpsrv");
	system("echo 1 > /proc/sys/vm/drop_caches");	// flush fs cache
/* todo: how to unmount correct drive */
	if (!path)
		sprintf(mount_point, "/%s",
			nvram_default_get("usb_mntpoint", "mnt"));
	else
		strcpy(mount_point, path);
	eval("/bin/umount", mount_point);
	eval("rm", "-f", DUMPFILE);
	eval("startservice", "samba3");
	eval("startservice", "ftpsrv");
	return;
}

    /* 
     * Handle hotplugging of UFD 
     */
int usb_add_ufd(char *devpath)
{
	DIR *dir = NULL;
	FILE *fp = NULL;
	char line[256];
	struct dirent *entry;
	char path[128];
	char *fs = NULL;
	int is_part = 0;
	char part[10], *partitions, *next;
	struct stat tmp_stat;
	int i, found = 0;
	int mounted[16];
	memset(mounted, 0, sizeof(mounted));

	if (devpath) {
		int rcnt = 0;
	      retry:;
		fp = fopen(devpath, "rb");
		if (!fp && rcnt < 10) {
//                      sysprintf("echo not available, try again %s >> /tmp/hotplugs",devpath);
			sleep(1);
			rcnt++;
			goto retry;
		}
//              sysprintf("echo open devname %s>> /tmp/hotplugs", devpath);
	} else {
		for (i = 1; i < 16; i++) {	//it needs some time for disk to settle down and /dev/discs is created
			if ((dir = opendir("/dev/discs")) != NULL
			    || (fp = fopen("/dev/sda", "rb")) != NULL
			    || (fp = fopen("/dev/sdb", "rb")) != NULL
			    || (fp = fopen("/dev/sdc", "rb")) != NULL
			    || (fp = fopen("/dev/sr0", "rb")) != NULL
			    || (fp = fopen("/dev/sr1", "rb")) != NULL
			    || (fp = fopen("/dev/sdd", "rb")) != NULL) {
				break;
			} else {
				sleep(1);
			}
		}
	}
	int new = 0;
	if (fp) {
		fclose(fp);
		new = 1;
		if (dir)
			closedir(dir);
	}

	/* 
	 * Scan through entries in the directories 
	 */

	for (i = 1; i < 16; i++) {	//it needs some time for disk to settle down and /dev/discs/discs%d is created
		if (new) {
			dir = opendir("/dev");
		}
		if (dir == NULL)	// i is 16 here and not 15 if timeout happens
			return EINVAL;
		while ((entry = readdir(dir)) != NULL) {
			int is_mounted = 0;
			if (mounted[i])
				continue;

#ifdef HAVE_X86
			char check[32];
			strcpy(check, getdisc());
			if (!strncmp(entry->d_name, check, 5)) {
				fprintf(stderr,
					"skip %s, since its the system drive\n",
					check);
				continue;
			}
#endif
			if (devpath) {
				char devname[64];
				sprintf(devname, "/dev/%s", entry->d_name);
				//sysprintf
				//  ("echo cmp devname %s >> /tmp/hotplugs"
				//   ,entry->d_name);
				if (strcmp(devname, devpath))
					continue;	// skip all non matching devices
				//  sysprintf
				//      ("echo take devname %s >> /tmp/hotplugs",
				//       devname);
			}
			if (!new && (strncmp(entry->d_name, "disc", 4)))
				continue;
			if (new && (strncmp(entry->d_name, "sd", 2))
			    && (strncmp(entry->d_name, "sr", 2))
			    && (strncmp(entry->d_name, "mmcblk", 6)))
				continue;
			mounted[i] = 1;

			/* 
			 * Files created when the UFD is inserted are not removed when
			 * it is removed. Verify the device  is still inserted.  Strip
			 * the "disc" and pass the rest of the string.  
			 */
			if (new) {
				//everything else would cause a memory fault
				if ((strncmp(entry->d_name, "mmcblk", 6))
				    && usb_ufd_connected(entry->d_name) ==
				    FALSE)
					continue;
			} else {
				if (usb_ufd_connected(entry->d_name + 4) ==
				    FALSE)
					continue;
			}
			if (new) {
				//              sysprintf("echo test %s >> /tmp/hotplugs",entry->d_name);
				if (strlen(entry->d_name) != 3
				    && (strncmp(entry->d_name, "mmcblk", 6)))
					continue;
				if (strlen(entry->d_name) != 7
				    && !(strncmp(entry->d_name, "mmcblk", 6)))
					continue;
				//              sysprintf("echo test success %s >> /tmp/hotplugs",entry->d_name);
				sprintf(path, "/dev/%s", entry->d_name);
			} else {
				sprintf(path, "/dev/discs/%s/disc",
					entry->d_name);
			}
			sysprintf("/usr/sbin/disktype %s > %s", path, DUMPFILE);
			//set spindown time
			sysprintf("hdparm -S 120 %s", path);

			/* 
			 * Check if it has file system 
			 */
			fs = NULL;
			if ((fp = fopen(DUMPFILE, "r"))) {
				while (fgets(line, sizeof(line), fp) != NULL) {
					if (strstr(line, "Partition"))
						is_part = 1;
					fprintf(stderr,
						"[USB Device] partition: %s\n",
						line);

					if (strstr(line, "file system")) {
						fprintf(stderr,
							"[USB Device] file system: %s\n",
							line);
						if (strstr(line, "FAT")) {
							fs = "vfat";
							break;
						} else if (strstr(line, "Ext2")) {
							fs = "ext2";
							break;
						} else if (strstr(line, "XFS")) {
							fs = "xfs";
							break;
						} else if (strstr(line, "UDF")) {
							fs = "udf";
							break;
						} else
						    if (strstr(line, "ISO9660"))
						{
							fs = "iso9660";
							break;
						} else if (strstr(line, "Ext3")) {
#ifdef HAVE_USB_ADVANCED
							fs = "ext3";
#else
							fs = "ext2";
#endif
							break;
						}
#ifdef HAVE_USB_ADVANCED
						else if (strstr(line, "Ext4")) {
							fs = "ext4";
							break;
						} else
						    if (strstr(line, "Btrfs")) {
							fs = "btrfs";
							break;
						}
#endif
#ifdef HAVE_NTFS3G
						else if (strstr(line, "NTFS")) {
							fs = "ntfs";
							break;
						}
#endif
					}

				}
				fclose(fp);
			}

			char targetname[64];
			if (fs) {
				/* 
				 * If it is partioned, mount first partition else raw disk 
				 */
				if (is_part && !new) {
					partitions =
					    "part1 part2 part3 part4 part5 part6";
					foreach(part, partitions, next) {
						sprintf(path,
							"/dev/discs/%s/%s",
							entry->d_name, part);
						if (stat(path, &tmp_stat))
							continue;
						sprintf(targetname, "%s_%s",
							entry->d_name, part);

						if (usb_process_path
						    (path, fs, targetname)
						    == 0) {
							is_mounted = 1;
							break;
						}
					}
				}
				if (is_part && new) {
					partitions = "1 2 3 4 5 6";
					foreach(part, partitions, next) {
						sprintf(path, "/dev/%s%s",
							entry->d_name, part);
						if (stat(path, &tmp_stat))
							continue;
						sprintf(targetname, "%s_part%s",
							entry->d_name, part);
						if (usb_process_path
						    (path, fs, targetname)
						    == 0) {
							is_mounted = 1;
							break;
						}
					}
				} else {
					if (new)
						sprintf(targetname, "disc%s",
							entry->d_name);
					else
						sprintf(targetname, "%s",
							entry->d_name);
					if (usb_process_path
					    (path, fs, targetname) == 0)
						is_mounted = 1;
				}

			}

			if ((fp = fopen(DUMPFILE, "a"))) {
				if (fs && is_mounted) {
					fprintf(fp,
						"Status: <b>Mounted on /%s</b>\n",
						nvram_safe_get("usb_mntpoint"));
					i = 16;
				} else if (fs)
					fprintf(fp,
						"Status: <b>Not mounted</b>\n");
				else
					fprintf(fp,
						"Status: <b>Not mounted - Unsupported file system or disk not formated</b>\n");
				fclose(fp);
			}

			if (is_mounted && !nvram_match("usb_runonmount", "")) {
				sprintf(path, "%s",
					nvram_safe_get("usb_runonmount"));
				if (stat(path, &tmp_stat) == 0)	//file exists
				{
					setenv("PATH",
					       "/sbin:/bin:/usr/sbin:/usr/bin:/jffs/sbin:/jffs/bin:/jffs/usr/sbin:/jffs/usr/bin:/mmc/sbin:/mmc/bin:/mmc/usr/sbin:/mmc/usr/bin:/opt/bin:/opt/sbin:/opt/usr/bin:/opt/usr/sbin",
					       1);
					setenv("LD_LIBRARY_PATH",
					       "/lib:/usr/lib:/jffs/lib:/jffs/usr/lib:/mmc/lib:/mmc/usr/lib:/opt/lib:/opt/usr/lib",
					       1);

					system(path);
				}
			}
		}
		if (i < 4)
			sleep(1);
		if (new)
			closedir(dir);
	}
	return 0;
}
