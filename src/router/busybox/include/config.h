/*
 * Automatically generated header file: don't edit
 */

/* Version Number */
#define BB_VER "1.1.0"
#define BB_BT "2006.03.13-11:04+0000"

#define HAVE_DOT_CONFIG 1

/*
 * General Configuration
 */
#undef CONFIG_FEATURE_BUFFERS_USE_MALLOC
#define CONFIG_FEATURE_BUFFERS_GO_ON_STACK 1
#undef CONFIG_FEATURE_BUFFERS_GO_IN_BSS
#undef CONFIG_FEATURE_VERBOSE_USAGE
#undef CONFIG_FEATURE_INSTALLER
#undef CONFIG_LOCALE_SUPPORT
#define CONFIG_FEATURE_DEVFS 1
#define CONFIG_FEATURE_DEVPTS 1
#undef CONFIG_FEATURE_CLEAN_UP
#define CONFIG_FEATURE_SUID 1
#undef CONFIG_FEATURE_SUID_CONFIG
#undef CONFIG_FEATURE_SUID_CONFIG_QUIET
#undef CONFIG_SELINUX

/*
 * Build Options
 */
#undef CONFIG_STATIC
#undef CONFIG_LFS
#define USING_CROSS_COMPILER 1
#define CROSS_COMPILER_PREFIX "mipsel-linux-uclibc-"
#define EXTRA_CFLAGS_OPTIONS "$(COPTS)"
#define CONFIG_FEATURE_SUSv2 1
#undef CONFIG_FEATURE_SUSv2_OBSOLETE

/*
 * Installation Options
 */
#undef CONFIG_INSTALL_NO_USR
#define CONFIG_INSTALL_APPLET_SYMLINKS 1
#undef CONFIG_INSTALL_APPLET_HARDLINKS
#undef CONFIG_INSTALL_APPLET_DONT
#define PREFIX "./_install"

/*
 * Archival Utilities
 */
#undef CONFIG_AR
#undef CONFIG_FEATURE_AR_LONG_FILENAMES
#undef CONFIG_BUNZIP2
#undef CONFIG_CPIO
#undef CONFIG_DPKG
#undef CONFIG_DPKG_DEB
#undef CONFIG_FEATURE_DPKG_DEB_EXTRACT_ONLY
#undef CONFIG_GUNZIP
#undef CONFIG_FEATURE_GUNZIP_UNCOMPRESS
#undef CONFIG_GZIP
#undef CONFIG_RPM2CPIO
#undef CONFIG_RPM
#define CONFIG_TAR 1
#define CONFIG_FEATURE_TAR_CREATE 1
#define CONFIG_FEATURE_TAR_BZIP2 1
#undef CONFIG_FEATURE_TAR_FROM
#define CONFIG_FEATURE_TAR_GZIP 1
#undef CONFIG_FEATURE_TAR_COMPRESS
#define CONFIG_FEATURE_TAR_OLDGNU_COMPATABILITY 1
#define CONFIG_FEATURE_TAR_GNU_EXTENSIONS 1
#undef CONFIG_FEATURE_TAR_LONG_OPTIONS
#undef CONFIG_UNCOMPRESS
#undef CONFIG_UNZIP

/*
 * Common options for cpio and tar
 */
#undef CONFIG_FEATURE_UNARCHIVE_TAPE
#undef CONFIG_FEATURE_DEB_TAR_GZ
#undef CONFIG_FEATURE_DEB_TAR_BZ2

/*
 * Coreutils
 */
#define CONFIG_BASENAME 1
#undef CONFIG_CAL
#define CONFIG_CAT 1
#define CONFIG_CHGRP 1
#define CONFIG_CHMOD 1
#define CONFIG_CHOWN 1
#define CONFIG_CHROOT 1
#define CONFIG_CMP 1
#undef CONFIG_COMM
#define CONFIG_CP 1
#define CONFIG_CUT 1
#define CONFIG_DATE 1

/*
 * date (forced enabled for use with watch)
 */
#define CONFIG_FEATURE_DATE_ISOFMT 1
#define CONFIG_DD 1
#define CONFIG_DF 1
#define CONFIG_DIRNAME 1
#undef CONFIG_DOS2UNIX
#undef CONFIG_UNIX2DOS
#define CONFIG_DU 1
#define CONFIG_FEATURE_DU_DEFALT_BLOCKSIZE_1K 1
#define CONFIG_ECHO 1
#define CONFIG_FEATURE_FANCY_ECHO 1
#define CONFIG_ENV 1
#define CONFIG_EXPR 1
#define CONFIG_FALSE 1
#undef CONFIG_FOLD
#define CONFIG_HEAD 1
#undef CONFIG_FEATURE_FANCY_HEAD
#undef CONFIG_HOSTID
#define CONFIG_ID 1
#define CONFIG_INSTALL 1
#undef CONFIG_LENGTH
#define CONFIG_LN 1
#undef CONFIG_LOGNAME
#define CONFIG_LS 1
#define CONFIG_FEATURE_LS_FILETYPES 1
#define CONFIG_FEATURE_LS_FOLLOWLINKS 1
#define CONFIG_FEATURE_LS_RECURSIVE 1
#define CONFIG_FEATURE_LS_SORTFILES 1
#define CONFIG_FEATURE_LS_TIMESTAMPS 1
#define CONFIG_FEATURE_LS_USERNAME 1
#define CONFIG_FEATURE_LS_COLOR 1
#define CONFIG_FEATURE_LS_COLOR_IS_DEFAULT 1
#define CONFIG_MD5SUM 1
#define CONFIG_MKDIR 1
#define CONFIG_MKFIFO 1
#undef CONFIG_MKNOD
#define CONFIG_MV 1
#undef CONFIG_NICE
#define CONFIG_NOHUP 1
#undef CONFIG_OD
#undef CONFIG_PRINTENV
#define CONFIG_PRINTF 1
#define CONFIG_PWD 1
#undef CONFIG_REALPATH
#define CONFIG_RM 1
#define CONFIG_RMDIR 1
#undef CONFIG_SEQ
#undef CONFIG_SHA1SUM
#define CONFIG_SLEEP 1
#define CONFIG_FEATURE_FANCY_SLEEP 1
#define CONFIG_SORT 1
#undef CONFIG_FEATURE_SORT_BIG
#undef CONFIG_STAT
#undef CONFIG_FEATURE_STAT_FORMAT
#undef CONFIG_STTY
#undef CONFIG_SUM
#define CONFIG_SYNC 1
#define CONFIG_TAIL 1
#define CONFIG_FEATURE_FANCY_TAIL 1
#define CONFIG_TEE 1
#define CONFIG_FEATURE_TEE_USE_BLOCK_IO 1
#define CONFIG_TEST 1

/*
 * test (forced enabled for use with shell)
 */
#undef CONFIG_FEATURE_TEST_64
#define CONFIG_TOUCH 1
#define CONFIG_TR 1
#undef CONFIG_FEATURE_TR_CLASSES
#undef CONFIG_FEATURE_TR_EQUIV
#define CONFIG_TRUE 1
#define CONFIG_TTY 1
#define CONFIG_UNAME 1
#define CONFIG_UNIQ 1
#define CONFIG_USLEEP 1
#undef CONFIG_UUDECODE
#undef CONFIG_UUENCODE
#define CONFIG_WATCH 1
#define CONFIG_WC 1
#undef CONFIG_WHO
#define CONFIG_WHOAMI 1
#define CONFIG_YES 1

/*
 * Common options for cp and mv
 */
#define CONFIG_FEATURE_PRESERVE_HARDLINKS 1

/*
 * Common options for ls and more
 */
#define CONFIG_FEATURE_AUTOWIDTH 1

/*
 * Common options for df, du, ls
 */
#define CONFIG_FEATURE_HUMAN_READABLE 1

/*
 * Common options for md5sum, sha1sum
 */
#undef CONFIG_FEATURE_MD5_SHA1_SUM_CHECK

/*
 * Console Utilities
 */
#undef CONFIG_CHVT
#define CONFIG_CLEAR 1
#undef CONFIG_DEALLOCVT
#undef CONFIG_DUMPKMAP
#undef CONFIG_LOADFONT
#undef CONFIG_LOADKMAP
#undef CONFIG_OPENVT
#define CONFIG_RESET 1
#define CONFIG_SETCONSOLE 1
#undef CONFIG_SETKEYCODES

/*
 * Debian Utilities
 */
#undef CONFIG_MKTEMP
#undef CONFIG_PIPE_PROGRESS
#undef CONFIG_READLINK
#undef CONFIG_FEATURE_READLINK_FOLLOW
#define CONFIG_RUN_PARTS 1
#undef CONFIG_START_STOP_DAEMON
#define CONFIG_WHICH 1

/*
 * Linux Ext2 FS Progs
 */
#undef CONFIG_CHATTR
#undef CONFIG_E2FSCK
#undef CONFIG_FSCK
#undef CONFIG_LSATTR
#undef CONFIG_MKE2FS
#undef CONFIG_TUNE2FS
#undef CONFIG_E2LABEL
#undef CONFIG_FINDFS

/*
 * Editors
 */
#define CONFIG_AWK 1
#undef CONFIG_FEATURE_AWK_MATH
#undef CONFIG_PATCH
#define CONFIG_SED 1
#define CONFIG_VI 1
#define CONFIG_FEATURE_VI_COLON 1
#define CONFIG_FEATURE_VI_YANKMARK 1
#define CONFIG_FEATURE_VI_SEARCH 1
#define CONFIG_FEATURE_VI_USE_SIGNALS 1
#define CONFIG_FEATURE_VI_DOT_CMD 1
#define CONFIG_FEATURE_VI_READONLY 1
#define CONFIG_FEATURE_VI_SETOPTS 1
#define CONFIG_FEATURE_VI_SET 1
#define CONFIG_FEATURE_VI_WIN_RESIZE 1
#define CONFIG_FEATURE_VI_OPTIMIZE_CURSOR 1

/*
 * Finding Utilities
 */
#define CONFIG_FIND 1
#define CONFIG_FEATURE_FIND_MTIME 1
#define CONFIG_FEATURE_FIND_PERM 1
#define CONFIG_FEATURE_FIND_TYPE 1
#define CONFIG_FEATURE_FIND_XDEV 1
#define CONFIG_FEATURE_FIND_NEWER 1
#define CONFIG_FEATURE_FIND_INUM 1
#undef CONFIG_FEATURE_FIND_EXEC
#define CONFIG_GREP 1
#define CONFIG_FEATURE_GREP_EGREP_ALIAS 1
#define CONFIG_FEATURE_GREP_FGREP_ALIAS 1
#define CONFIG_FEATURE_GREP_CONTEXT 1
#define CONFIG_XARGS 1
#define CONFIG_FEATURE_XARGS_SUPPORT_CONFIRMATION 1
#define CONFIG_FEATURE_XARGS_SUPPORT_QUOTES 1
#define CONFIG_FEATURE_XARGS_SUPPORT_TERMOPT 1
#define CONFIG_FEATURE_XARGS_SUPPORT_ZERO_TERM 1

/*
 * Init Utilities
 */
#undef CONFIG_INIT
#undef CONFIG_FEATURE_USE_INITTAB
#undef CONFIG_FEATURE_INITRD
#undef CONFIG_FEATURE_INIT_COREDUMPS
#undef CONFIG_FEATURE_INIT_SCTTY
#undef CONFIG_FEATURE_EXTRA_QUIET
#undef CONFIG_HALT
#define CONFIG_MESG 1
#undef CONFIG_POWEROFF
#define CONFIG_REBOOT 1

/*
 * Login/Password Management Utilities
 */
#undef CONFIG_FEATURE_SHADOWPASSWDS
#undef CONFIG_USE_BB_SHADOW
#undef CONFIG_USE_BB_PWD_GRP
#undef CONFIG_ADDGROUP
#undef CONFIG_DELGROUP
#undef CONFIG_ADDUSER
#undef CONFIG_DELUSER
#undef CONFIG_GETTY
#undef CONFIG_FEATURE_UTMP
#undef CONFIG_FEATURE_WTMP
#define CONFIG_LOGIN 1
#undef CONFIG_FEATURE_SECURETTY
#undef CONFIG_PASSWD
#undef CONFIG_SU
#undef CONFIG_SULOGIN
#undef CONFIG_VLOCK

/*
 * Miscellaneous Utilities
 */
#define CONFIG_ADJTIMEX 1
#undef CONFIG_BBCONFIG
#undef CONFIG_CROND
#undef CONFIG_FEATURE_CROND_CALL_SENDMAIL
#undef CONFIG_CRONTAB
#define CONFIG_DC 1
#undef CONFIG_DEVFSD
#undef CONFIG_DEVFSD_MODLOAD
#undef CONFIG_DEVFSD_FG_NP
#undef CONFIG_DEVFSD_VERBOSE
#undef CONFIG_EJECT
#undef CONFIG_LAST
#undef CONFIG_LESS
#undef CONFIG_FEATURE_LESS_BRACKETS
#undef CONFIG_FEATURE_LESS_FLAGS
#undef CONFIG_FEATURE_LESS_FLAGCS
#undef CONFIG_FEATURE_LESS_MARKS
#undef CONFIG_FEATURE_LESS_REGEXP
#undef CONFIG_HDPARM
#undef CONFIG_FEATURE_HDPARM_GET_IDENTITY
#undef CONFIG_FEATURE_HDPARM_HDIO_SCAN_HWIF
#undef CONFIG_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF
#undef CONFIG_FEATURE_HDPARM_HDIO_DRIVE_RESET
#undef CONFIG_FEATURE_HDPARM_HDIO_TRISTATE_HWIF
#undef CONFIG_FEATURE_HDPARM_HDIO_GETSET_DMA
#undef CONFIG_MAKEDEVS
#undef CONFIG_FEATURE_MAKEDEVS_LEAF
#undef CONFIG_FEATURE_MAKEDEVS_TABLE
#undef CONFIG_MOUNTPOINT
#undef CONFIG_MT
#undef BB_APPLET_RUNLEVEL
#define CONFIG_RX 1
#define CONFIG_STRINGS 1
#undef CONFIG_SETSID
#define CONFIG_TIME 1
#undef CONFIG_WATCHDOG

/*
 * Linux Module Utilities
 */
#define CONFIG_INSMOD 1
#undef CONFIG_FEATURE_INSMOD_VERSION_CHECKING
#undef CONFIG_FEATURE_INSMOD_KSYMOOPS_SYMBOLS
#undef CONFIG_FEATURE_INSMOD_LOADINKMEM
#undef CONFIG_FEATURE_INSMOD_LOAD_MAP
#undef CONFIG_FEATURE_INSMOD_LOAD_MAP_FULL
#define CONFIG_RMMOD 1
#define CONFIG_LSMOD 1
#undef CONFIG_FEATURE_LSMOD_PRETTY_2_6_OUTPUT
#undef CONFIG_MODPROBE
#undef CONFIG_FEATURE_MODPROBE_MULTIPLE_OPTIONS

/*
 * Options common to multiple modutils
 */
#undef CONFIG_FEATURE_CHECK_TAINTED_MODULE
#define CONFIG_FEATURE_2_4_MODULES 1
#undef CONFIG_FEATURE_2_6_MODULES
#define CONFIG_FEATURE_QUERY_MODULE_INTERFACE 1

/*
 * Networking Utilities
 */
#undef CONFIG_FEATURE_IPV6
#undef CONFIG_ARPING
#define CONFIG_ETHER_WAKE 1
#undef CONFIG_FAKEIDENTD
#undef CONFIG_FTPGET
#undef CONFIG_FTPPUT
#define CONFIG_HOSTNAME 1
#undef CONFIG_HTTPD
#undef CONFIG_FEATURE_HTTPD_USAGE_FROM_INETD_ONLY
#undef CONFIG_FEATURE_HTTPD_BASIC_AUTH
#undef CONFIG_FEATURE_HTTPD_AUTH_MD5
#undef CONFIG_FEATURE_HTTPD_RELOAD_CONFIG_SIGHUP
#undef CONFIG_FEATURE_HTTPD_SETUID
#undef CONFIG_FEATURE_HTTPD_CONFIG_WITH_MIME_TYPES
#undef CONFIG_FEATURE_HTTPD_CGI
#undef CONFIG_FEATURE_HTTPD_SET_REMOTE_PORT_TO_ENV
#undef CONFIG_FEATURE_HTTPD_ENCODE_URL_STR
#define CONFIG_IFCONFIG 1
#define CONFIG_FEATURE_IFCONFIG_STATUS 1
#undef CONFIG_FEATURE_IFCONFIG_SLIP
#undef CONFIG_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ
#undef CONFIG_FEATURE_IFCONFIG_HW
#define CONFIG_FEATURE_IFCONFIG_BROADCAST_PLUS 1
#define CONFIG_IFUPDOWN 1
#undef CONFIG_FEATURE_IFUPDOWN_IP
#define CONFIG_FEATURE_IFUPDOWN_IP_BUILTIN 1
#define CONFIG_FEATURE_IFUPDOWN_IPV4 1
#define CONFIG_FEATURE_IFUPDOWN_IPV6 1
#undef CONFIG_FEATURE_IFUPDOWN_IPX
#undef CONFIG_FEATURE_IFUPDOWN_MAPPING
#undef CONFIG_INETD
#undef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_ECHO
#undef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DISCARD
#undef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_TIME
#undef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DAYTIME
#undef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_CHARGEN
#undef CONFIG_FEATURE_INETD_RPC
#undef CONFIG_IP
#undef CONFIG_FEATURE_IP_ADDRESS
#undef CONFIG_FEATURE_IP_LINK
#undef CONFIG_FEATURE_IP_ROUTE
#undef CONFIG_FEATURE_IP_TUNNEL
#undef CONFIG_IPCALC
#undef CONFIG_FEATURE_IPCALC_FANCY
#undef CONFIG_IPADDR
#undef CONFIG_IPLINK
#undef CONFIG_IPROUTE
#undef CONFIG_IPTUNNEL
#define CONFIG_NAMEIF 1
#define CONFIG_NC 1
#undef CONFIG_NC_GAPING_SECURITY_HOLE
#define CONFIG_NETSTAT 1
#define CONFIG_NSLOOKUP 1
#define CONFIG_PING 1
#define CONFIG_FEATURE_FANCY_PING 1
#undef CONFIG_PING6
#undef CONFIG_FEATURE_FANCY_PING6
#define CONFIG_ROUTE 1
#define CONFIG_TELNET 1
#define CONFIG_FEATURE_TELNET_TTYPE 1
#define CONFIG_FEATURE_TELNET_AUTOLOGIN 1
#undef CONFIG_TELNETD
#undef CONFIG_FEATURE_TELNETD_INETD
#undef CONFIG_TFTP
#undef CONFIG_FEATURE_TFTP_GET
#undef CONFIG_FEATURE_TFTP_PUT
#undef CONFIG_FEATURE_TFTP_BLOCKSIZE
#undef CONFIG_FEATURE_TFTP_DEBUG
#define CONFIG_TRACEROUTE 1
#undef CONFIG_FEATURE_TRACEROUTE_VERBOSE
#define CONFIG_FEATURE_TRACEROUTE_SOURCE_ROUTE 1
#undef CONFIG_FEATURE_TRACEROUTE_USE_ICMP
#define CONFIG_VCONFIG 1
#define CONFIG_WGET 1
#define CONFIG_FEATURE_WGET_STATUSBAR 1
#undef CONFIG_FEATURE_WGET_AUTHENTICATION
#undef CONFIG_FEATURE_WGET_IP6_LITERAL

/*
 * udhcp Server/Client
 */
#undef CONFIG_UDHCPD
#undef CONFIG_UDHCPC
#undef CONFIG_DUMPLEASES
#undef CONFIG_FEATURE_UDHCP_SYSLOG
#undef CONFIG_FEATURE_UDHCP_DEBUG
#undef CONFIG_ZCIP

/*
 * Process Utilities
 */
#define CONFIG_FREE 1
#undef CONFIG_FUSER
#define CONFIG_KILL 1
#define CONFIG_KILLALL 1
#define CONFIG_PIDOF 1
#undef CONFIG_FEATURE_PIDOF_SINGLE
#undef CONFIG_FEATURE_PIDOF_OMIT
#define CONFIG_PS 1
#define CONFIG_FEATURE_PS_WIDE 1
#undef CONFIG_RENICE
#undef CONFIG_BB_SYSCTL
#define CONFIG_TOP 1
#define CONFIG_FEATURE_TOP_CPU_USAGE_PERCENTAGE 1
#define CONFIG_UPTIME 1

/*
 * Shells
 */
#define CONFIG_FEATURE_SH_IS_ASH 1
#undef CONFIG_FEATURE_SH_IS_HUSH
#undef CONFIG_FEATURE_SH_IS_LASH
#undef CONFIG_FEATURE_SH_IS_MSH
#undef CONFIG_FEATURE_SH_IS_NONE
#define CONFIG_ASH 1

/*
 * Ash Shell Options
 */
#define CONFIG_ASH_JOB_CONTROL 1
#undef CONFIG_ASH_READ_NCHARS
#undef CONFIG_ASH_READ_TIMEOUT
#define CONFIG_ASH_ALIAS 1
#define CONFIG_ASH_MATH_SUPPORT 1
#undef CONFIG_ASH_MATH_SUPPORT_64
#define CONFIG_ASH_GETOPTS 1
#define CONFIG_ASH_CMDCMD 1
#define CONFIG_ASH_BUILTIN_ECHO 1
#undef CONFIG_ASH_MAIL
#define CONFIG_ASH_OPTIMIZE_FOR_SIZE 1
#undef CONFIG_ASH_RANDOM_SUPPORT
#undef CONFIG_ASH_EXPAND_PRMT
#undef CONFIG_HUSH
#undef CONFIG_LASH
#undef CONFIG_MSH

/*
 * Bourne Shell Options
 */
#undef CONFIG_FEATURE_SH_EXTRA_QUIET
#undef CONFIG_FEATURE_SH_STANDALONE_SHELL
#define CONFIG_FEATURE_COMMAND_EDITING 1
#undef CONFIG_FEATURE_COMMAND_EDITING_VI
#define CONFIG_FEATURE_COMMAND_HISTORY 15
#define CONFIG_FEATURE_COMMAND_SAVEHISTORY 1
#define CONFIG_FEATURE_COMMAND_TAB_COMPLETION 1
#undef CONFIG_FEATURE_COMMAND_USERNAME_COMPLETION
#define CONFIG_FEATURE_SH_FANCY_PROMPT 1

/*
 * System Logging Utilities
 */
#define CONFIG_SYSLOGD 1
#define CONFIG_FEATURE_ROTATE_LOGFILE 1
#define CONFIG_FEATURE_REMOTE_LOG 1
#define CONFIG_FEATURE_IPC_SYSLOG 1
#define CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE 16
#define CONFIG_LOGREAD 1
#undef CONFIG_FEATURE_LOGREAD_REDUCED_LOCKING
#define CONFIG_KLOGD 1
#define CONFIG_LOGGER 1

/*
 * Linux System Utilities
 */
#define CONFIG_DMESG 1
#undef CONFIG_FBSET
#undef CONFIG_FEATURE_FBSET_FANCY
#undef CONFIG_FEATURE_FBSET_READMODE
#undef CONFIG_FDFLUSH
#undef CONFIG_FDFORMAT
#undef CONFIG_FDISK
#undef FDISK_SUPPORT_LARGE_DISKS
#undef CONFIG_FEATURE_FDISK_WRITABLE
#undef CONFIG_FEATURE_AIX_LABEL
#undef CONFIG_FEATURE_SGI_LABEL
#undef CONFIG_FEATURE_SUN_LABEL
#undef CONFIG_FEATURE_OSF_LABEL
#undef CONFIG_FEATURE_FDISK_ADVANCED
#undef CONFIG_FREERAMDISK
#undef CONFIG_FSCK_MINIX
#undef CONFIG_MKFS_MINIX
#undef CONFIG_FEATURE_MINIX2
#define CONFIG_GETOPT 1
#define CONFIG_HEXDUMP 1
#undef CONFIG_HWCLOCK
#undef CONFIG_FEATURE_HWCLOCK_LONGOPTIONS
#undef CONFIG_FEATURE_HWCLOCK_ADJTIME_FHS
#undef CONFIG_IPCRM
#undef CONFIG_IPCS
#undef CONFIG_LOSETUP
#undef CONFIG_MDEV
#undef CONFIG_FEATURE_MDEV_CONF
#undef CONFIG_MKSWAP
#define CONFIG_MORE 1
#define CONFIG_FEATURE_USE_TERMIOS 1
#define CONFIG_PIVOT_ROOT 1
#undef CONFIG_SWITCH_ROOT
#undef CONFIG_RDATE
#undef CONFIG_READPROFILE
#undef CONFIG_SWAPONOFF
#define CONFIG_MOUNT 1
#undef CONFIG_FEATURE_MOUNT_NFS
#define CONFIG_UMOUNT 1

/*
 * Common options for mount/umount
 */
#define CONFIG_FEATURE_MOUNT_LOOP 1
#undef CONFIG_FEATURE_MTAB_SUPPORT

/*
 * Debugging Options
 */
#undef CONFIG_DEBUG
#undef CONFIG_NO_DEBUG_LIB
#undef CONFIG_DMALLOC
#undef CONFIG_EFENCE
