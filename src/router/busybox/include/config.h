/*
 * Automatically generated header file: don't edit
 */

/* Version Number */
#define BB_VER "1.01"
#define BB_BT "2006.04.04-00:14+0000"

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
#undef CONFIG_SELINUX

/*
 * Build Options
 */
#undef CONFIG_STATIC
#undef CONFIG_LFS
#define USING_CROSS_COMPILER 1
#define CROSS_COMPILER_PREFIX "mipsel-linux-uclibc-"
#define EXTRA_CFLAGS_OPTIONS "$(COPTS)"

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
#undef CONFIG_GUNZIP
#undef CONFIG_GZIP
#undef CONFIG_RPM2CPIO
#undef CONFIG_RPM
#undef CONFIG_TAR
#undef CONFIG_UNCOMPRESS
#undef CONFIG_UNZIP

/*
 * Coreutils
 */
#define CONFIG_BASENAME 1
#undef CONFIG_CAL
#define CONFIG_CAT 1
#undef CONFIG_CHGRP
#define CONFIG_CHMOD 1
#undef CONFIG_CHOWN
#undef CONFIG_CHROOT
#undef CONFIG_CMP
#define CONFIG_CP 1
#undef CONFIG_CUT
#undef CONFIG_DATE
#undef CONFIG_DD
#undef CONFIG_DF
#undef CONFIG_DIRNAME
#undef CONFIG_DOS2UNIX
#undef CONFIG_DU
#define CONFIG_ECHO 1
#undef CONFIG_FEATURE_FANCY_ECHO
#undef CONFIG_ENV
#undef CONFIG_EXPR
#undef CONFIG_FALSE
#undef CONFIG_FOLD
#define CONFIG_HEAD 1
#undef CONFIG_FEATURE_FANCY_HEAD
#undef CONFIG_HOSTID
#undef CONFIG_ID
#undef CONFIG_INSTALL
#undef CONFIG_LENGTH
#undef CONFIG_LN
#undef CONFIG_LOGNAME
#define CONFIG_LS 1
#define CONFIG_FEATURE_LS_FILETYPES 1
#undef CONFIG_FEATURE_LS_FOLLOWLINKS
#undef CONFIG_FEATURE_LS_RECURSIVE
#undef CONFIG_FEATURE_LS_SORTFILES
#define CONFIG_FEATURE_LS_TIMESTAMPS 1
#undef CONFIG_FEATURE_LS_USERNAME
#define CONFIG_FEATURE_LS_COLOR 1
#undef CONFIG_MD5SUM
#define CONFIG_MKDIR 1
#undef CONFIG_MKFIFO
#undef CONFIG_MKNOD
#define CONFIG_MV 1
#undef CONFIG_OD
#undef CONFIG_PRINTF
#define CONFIG_PWD 1
#undef CONFIG_REALPATH
#define CONFIG_RM 1
#define CONFIG_RMDIR 1
#undef CONFIG_SEQ
#undef CONFIG_SHA1SUM
#define CONFIG_SLEEP 1
#define CONFIG_FEATURE_FANCY_SLEEP 1
#define CONFIG_SORT 1
#undef CONFIG_STTY
#undef CONFIG_SYNC
#undef CONFIG_TAIL
#undef CONFIG_TEE
#define CONFIG_TEST 1

/*
 * test (forced enabled for use with shell)
 */
#undef CONFIG_FEATURE_TEST_64
#define CONFIG_TOUCH 1
#undef CONFIG_TR
#undef CONFIG_TRUE
#undef CONFIG_TTY
#undef CONFIG_UNAME
#undef CONFIG_UNIQ
#undef CONFIG_USLEEP
#undef CONFIG_UUDECODE
#undef CONFIG_UUENCODE
#undef CONFIG_WATCH
#define CONFIG_WC 1
#undef CONFIG_WHO
#undef CONFIG_WHOAMI
#undef CONFIG_YES

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
 * Console Utilities
 */
#undef CONFIG_CHVT
#undef CONFIG_CLEAR
#undef CONFIG_DEALLOCVT
#undef CONFIG_DUMPKMAP
#undef CONFIG_LOADFONT
#undef CONFIG_LOADKMAP
#undef CONFIG_OPENVT
#undef CONFIG_RESET
#undef CONFIG_SETKEYCODES

/*
 * Debian Utilities
 */
#undef CONFIG_MKTEMP
#undef CONFIG_PIPE_PROGRESS
#undef CONFIG_READLINK
#undef CONFIG_RUN_PARTS
#undef CONFIG_START_STOP_DAEMON
#undef CONFIG_WHICH

/*
 * Editors
 */
#define CONFIG_AWK 1
#undef CONFIG_FEATURE_AWK_MATH
#undef CONFIG_PATCH
#define CONFIG_SED 1
#undef CONFIG_VI

/*
 * Finding Utilities
 */
#undef CONFIG_FIND
#define CONFIG_GREP 1
#undef CONFIG_FEATURE_GREP_EGREP_ALIAS
#undef CONFIG_FEATURE_GREP_FGREP_ALIAS
#undef CONFIG_FEATURE_GREP_CONTEXT
#undef CONFIG_XARGS

/*
 * Init Utilities
 */
#undef CONFIG_INIT
#undef CONFIG_HALT
#undef CONFIG_POWEROFF
#define CONFIG_REBOOT 1
#undef CONFIG_MESG

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
#undef CONFIG_ADJTIMEX
#undef CONFIG_CROND
#undef CONFIG_CRONTAB
#undef CONFIG_DC
#undef CONFIG_DEVFSD
#undef CONFIG_LAST
#undef CONFIG_HDPARM
#undef CONFIG_MAKEDEVS
#undef CONFIG_MT
#undef CONFIG_RX
#undef CONFIG_STRINGS
#define CONFIG_TIME 1
#undef CONFIG_WATCHDOG

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
#undef CONFIG_MODPROBE
#define CONFIG_RMMOD 1
#undef CONFIG_FEATURE_CHECK_TAINTED_MODULE

/*
 * Networking Utilities
 */
#undef CONFIG_FEATURE_IPV6
#undef CONFIG_ARPING
#undef CONFIG_FTPGET
#undef CONFIG_FTPPUT
#define CONFIG_HOSTNAME 1
#undef CONFIG_HTTPD
#define CONFIG_IFCONFIG 1
#define CONFIG_FEATURE_IFCONFIG_STATUS 1
#undef CONFIG_FEATURE_IFCONFIG_SLIP
#undef CONFIG_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ
#undef CONFIG_FEATURE_IFCONFIG_HW
#define CONFIG_FEATURE_IFCONFIG_BROADCAST_PLUS 1
#undef CONFIG_IFUPDOWN
#undef CONFIG_INETD
#undef CONFIG_IP
#undef CONFIG_IPCALC
#undef CONFIG_IPADDR
#undef CONFIG_IPLINK
#undef CONFIG_IPROUTE
#undef CONFIG_IPTUNNEL
#undef CONFIG_NAMEIF
#undef CONFIG_NC
#undef CONFIG_NETSTAT
#undef CONFIG_NSLOOKUP
#define CONFIG_PING 1
#define CONFIG_FEATURE_FANCY_PING 1
#define CONFIG_ROUTE 1
#undef CONFIG_TELNET
#undef CONFIG_TELNETD
#undef CONFIG_TFTP
#define CONFIG_TRACEROUTE 1
#undef CONFIG_FEATURE_TRACEROUTE_VERBOSE
#define CONFIG_VCONFIG 1
#define CONFIG_WGET 1
#undef CONFIG_FEATURE_WGET_STATUSBAR
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
#undef CONFIG_PIDOF
#define CONFIG_PS 1
#undef CONFIG_RENICE
#undef CONFIG_TOP
#define CONFIG_UPTIME 1
#undef CONFIG_SYSCTL

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
#undef CONFIG_HEXDUMP
#undef CONFIG_HWCLOCK
#undef CONFIG_LOSETUP
#undef CONFIG_MKSWAP
#define CONFIG_MORE 1
#define CONFIG_FEATURE_USE_TERMIOS 1
#undef CONFIG_PIVOT_ROOT
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
