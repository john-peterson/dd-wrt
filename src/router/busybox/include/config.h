/*
 * Automatically generated header file: don't edit
 */

/* Version Number */
#define BB_VER "1.01"
#define BB_BT "2006.02.22-10:40+0000"

#define HAVE_DOT_CONFIG 1

/*
 * General Configuration
 */
#undef CONFIG_FEATURE_BUFFERS_USE_MALLOC
#define CONFIG_FEATURE_BUFFERS_GO_ON_STACK 1
#undef CONFIG_FEATURE_BUFFERS_GO_IN_BSS
#define CONFIG_FEATURE_VERBOSE_USAGE 1
#undef CONFIG_FEATURE_INSTALLER
#undef CONFIG_LOCALE_SUPPORT
#define CONFIG_FEATURE_DEVFS 1
#define CONFIG_FEATURE_DEVPTS 1
#undef CONFIG_FEATURE_CLEAN_UP
#define CONFIG_FEATURE_SUID 1
#undef CONFIG_FEATURE_SUID_CONFIG
#undef CONFIG_SELINUX

/*
 * Build Options
 */
#undef CONFIG_STATIC
#undef CONFIG_LFS
#define USING_CROSS_COMPILER 1
#define CROSS_COMPILER_PREFIX "mipsel-linux-uclibc-"
#define EXTRA_CFLAGS_OPTIONS ""

/*
 * Installation Options
 */
#undef CONFIG_INSTALL_NO_USR
#define PREFIX "./_install"

/*
 * Archival Utilities
 */
#undef CONFIG_AR
#undef CONFIG_BUNZIP2
#undef CONFIG_CPIO
#undef CONFIG_DPKG
#undef CONFIG_DPKG_DEB
#define CONFIG_GUNZIP 1
#undef CONFIG_FEATURE_GUNZIP_UNCOMPRESS
#define CONFIG_GZIP 1
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
#define CONFIG_UNZIP 1

/*
 * Common options for cpio and tar
 */
#undef CONFIG_FEATURE_UNARCHIVE_TAPE

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
#define CONFIG_MD5SUM 1
#define CONFIG_MKDIR 1
#define CONFIG_MKFIFO 1
#define CONFIG_MKNOD 1
#define CONFIG_MV 1
#undef CONFIG_OD
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
#define CONFIG_STTY 1
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
#define CONFIG_TRUE 1
#define CONFIG_TTY 1
#define CONFIG_UNAME 1
#define CONFIG_UNIQ 1
#define CONFIG_USLEEP 1
#define CONFIG_UUDECODE 1
#define CONFIG_UUENCODE 1
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
#undef CONFIG_SETKEYCODES

/*
 * Debian Utilities
 */
#undef CONFIG_MKTEMP
#undef CONFIG_PIPE_PROGRESS
#undef CONFIG_READLINK
#undef CONFIG_RUN_PARTS
#undef CONFIG_START_STOP_DAEMON
#define CONFIG_WHICH 1

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
#undef CONFIG_HALT
#undef CONFIG_POWEROFF
#define CONFIG_REBOOT 1
#define CONFIG_MESG 1

/*
 * Login/Password Management Utilities
 */
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
 * Common options for adduser, deluser, login, su
 */
#undef CONFIG_FEATURE_SHADOWPASSWDS

/*
 * Miscellaneous Utilities
 */
#define CONFIG_ADJTIMEX 1
#undef CONFIG_CROND
#undef CONFIG_CRONTAB
#define CONFIG_DC 1
#undef CONFIG_DEVFSD
#undef CONFIG_LAST
#undef CONFIG_HDPARM
#undef CONFIG_MAKEDEVS
#undef CONFIG_MT
#define CONFIG_RX 1
#define CONFIG_STRINGS 1
#define CONFIG_TIME 1
#define CONFIG_WATCHDOG 1

/*
 * Linux Module Utilities
 */
#define CONFIG_INSMOD 1
#define CONFIG_FEATURE_2_4_MODULES 1
#undef CONFIG_FEATURE_2_6_MODULES
#undef CONFIG_FEATURE_INSMOD_VERSION_CHECKING
#undef CONFIG_FEATURE_INSMOD_KSYMOOPS_SYMBOLS
#undef CONFIG_FEATURE_INSMOD_LOADINKMEM
#undef CONFIG_FEATURE_INSMOD_LOAD_MAP
#define CONFIG_LSMOD 1
#define CONFIG_FEATURE_QUERY_MODULE_INTERFACE 1
#define CONFIG_MODPROBE 1
#define CONFIG_RMMOD 1
#undef CONFIG_FEATURE_CHECK_TAINTED_MODULE

/*
 * Networking Utilities
 */
#undef CONFIG_FEATURE_IPV6
#undef CONFIG_ARPING
#define CONFIG_FTPGET 1
#define CONFIG_FTPPUT 1
#define CONFIG_HOSTNAME 1
#undef CONFIG_HTTPD
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
#undef CONFIG_IP
#undef CONFIG_IPCALC
#undef CONFIG_IPADDR
#undef CONFIG_IPLINK
#undef CONFIG_IPROUTE
#undef CONFIG_IPTUNNEL
#define CONFIG_NAMEIF 1
#define CONFIG_NC 1
#define CONFIG_NETSTAT 1
#define CONFIG_NSLOOKUP 1
#define CONFIG_PING 1
#define CONFIG_FEATURE_FANCY_PING 1
#define CONFIG_ROUTE 1
#define CONFIG_TELNET 1
#define CONFIG_FEATURE_TELNET_TTYPE 1
#define CONFIG_FEATURE_TELNET_AUTOLOGIN 1
#undef CONFIG_TELNETD
#undef CONFIG_TFTP
#define CONFIG_TRACEROUTE 1
#undef CONFIG_FEATURE_TRACEROUTE_VERBOSE
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

/*
 * Process Utilities
 */
#define CONFIG_FREE 1
#define CONFIG_KILL 1
#define CONFIG_KILLALL 1
#define CONFIG_PIDOF 1
#define CONFIG_PS 1
#undef CONFIG_RENICE
#define CONFIG_TOP 1
#define FEATURE_CPU_USAGE_PERCENTAGE 1
#define CONFIG_UPTIME 1
#define CONFIG_SYSCTL 1

/*
 * Another Bourne-like Shell
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
#define CONFIG_ASH_ALIAS 1
#define CONFIG_ASH_MATH_SUPPORT 1
#undef CONFIG_ASH_MATH_SUPPORT_64
#define CONFIG_ASH_GETOPTS 1
#define CONFIG_ASH_CMDCMD 1
#undef CONFIG_ASH_MAIL
#define CONFIG_ASH_OPTIMIZE_FOR_SIZE 1
#undef CONFIG_ASH_RANDOM_SUPPORT
#undef CONFIG_HUSH
#undef CONFIG_LASH
#undef CONFIG_MSH

/*
 * Bourne Shell Options
 */
#undef CONFIG_FEATURE_SH_EXTRA_QUIET
#undef CONFIG_FEATURE_SH_STANDALONE_SHELL
#define CONFIG_FEATURE_COMMAND_EDITING 1
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
#undef CONFIG_FDFLUSH
#undef CONFIG_FDFORMAT
#undef CONFIG_FDISK
#undef CONFIG_FREERAMDISK
#undef CONFIG_FSCK_MINIX
#undef CONFIG_MKFS_MINIX
#define CONFIG_GETOPT 1
#define CONFIG_HEXDUMP 1
#undef CONFIG_HWCLOCK
#undef CONFIG_LOSETUP
#undef CONFIG_MKSWAP
#define CONFIG_MORE 1
#define CONFIG_FEATURE_USE_TERMIOS 1
#define CONFIG_PIVOT_ROOT 1
#undef CONFIG_RDATE
#undef CONFIG_SWAPONOFF
#define CONFIG_MOUNT 1
#undef CONFIG_NFSMOUNT
#define CONFIG_UMOUNT 1
#define CONFIG_FEATURE_MOUNT_FORCE 1

/*
 * Common options for mount/umount
 */
#define CONFIG_FEATURE_MOUNT_LOOP 1
#undef CONFIG_FEATURE_MTAB_SUPPORT

/*
 * Debugging Options
 */
#undef CONFIG_DEBUG
