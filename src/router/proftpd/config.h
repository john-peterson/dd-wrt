/* config.h.  Generated by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */
/* -*- C -*- */
/* @configure_input@ */

#ifndef		config_h_included
#define		config_h_included

/*************************************************************************
 * This section is automatically generated by 'configure'.  Adjust these
 * if configure didn't make a correct guess for your system.
 *************************************************************************/

/* Define to be the build options. */
#define PR_BUILD_OPTS "--host=mipsel-linux --prefix=/tmp/proftpd"

/* Define to be the build platform. */
#define PR_PLATFORM "LINUX"

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef ino_t */

/* Define to `int' if <sys/types.h> doesn't define. */
#define HAVE_INTPTR_T 1
#if !defined(HAVE_INTPTR_T)
# define intptr_t	int
#endif /* HAVE_INTPTR_T */

/* Define to `int' if <sys/socket.h> doesn't define. */
/* #undef socklen_t */

/* Define to `mode_t' if <sys/types.h> doesn't define. */
#define umode_t mode_t

/* Define if you have AIX send_file() semantics. */
/* #undef HAVE_AIX_SENDFILE */

/* Define if you have BSD POSIX ACLs. */
/* #undef HAVE_BSD_POSIX_ACL */

/* Define if you have BSD sendfile() semantics. */
/* #undef HAVE_BSD_SENDFILE */

/* Define if you have Linux POSIX ACLs. */
/* #undef HAVE_LINUX_POSIX_ACL */

/* Define if you have Linux sendfile() semantics. */
/* #undef HAVE_LINUX_SENDFILE */

/* Define if you have Solaris POSIX ACLs. */
/* #undef HAVE_SOLARIS_POSIX_ACL */

/* Define if you have Solaris sendfile() semantics. */
/* #undef HAVE_SOLARIS_SENDFILE */

/* Define if your <syslog.h> defines the LOG_CRON macro */
#define HAVE_LOG_CRON 1

/* Define if your <syslog.h> defines the LOG_FTP macro */
#define HAVE_LOG_FTP 1

/* Define if you want support for PAM based authentication */
/* #undef HAVE_PAM */

/* Define if you have the <linux/capability.h> header file. */
/* #undef HAVE_LINUX_CAPABILITY_H */

/* Define if your DIR structure has member dd_fd */
/* #undef HAVE_STRUCT_DIR_DD_FD */

/* Define if your DIR structure has member __dd_fd */
/* #undef HAVE_STRUCT_DIR___DD_FD */

/* Define if your spwd structure has member warn */
#define HAVE_SPWD_SP_WARN 1

/* Define if your spwd structure has member inact */
#define HAVE_SPWD_SP_INACT 1

/* Define if your spwd structure has member expire */
#define HAVE_SPWD_SP_EXPIRE 1

/* Define if your system has __progname */
//#define HAVE___PROGNAME 1

/* Define if your system has _pw_stayopen variable (IRIX specific?) */
/* #undef HAVE__PW_STAYOPEN */

/* Define if your system has POSIX ACL support */
/* #undef HAVE_POSIX_ACL */

/* Define if your system has the sendfile function */
/* #undef HAVE_SENDFILE */

/* Define this if you have the setpassent function */
/* #undef HAVE_SETPASSENT */

/* Define if your system has the setspent function.  */
#define HAVE_SETSPENT 1

/* Define if your DIR structure has member d_fd */
/* #undef HAVE_STRUCT_DIR_D_FD */

/* Define if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define if you already have a typedef for timer_t */
#define HAVE_TIMER_T 1

/* Define if your struct utmp has ut_host */
#define HAVE_UT_UT_HOST 1

/* Define if your struct utmp uses ut_user and not ut_name */
#define HAVE_UTMAXTYPE 1

#define PF_ARGV_NONE		0
#define PF_ARGV_NEW		1
#define PF_ARGV_WRITEABLE	2
#define PF_ARGV_PSTAT		3
#define PF_ARGV_PSSTRINGS	4

/* If you don't have setproctitle, PF_ARGV_TYPE needs to be set to either
 * PF_ARGV_NEW (replace argv[] arguments), PF_ARGV_WRITEABLE (overwrite
 * argv[]), PF_ARGV_PSTAT (use the pstat function), or PF_ARGV_PSSTRINGS
 * (use PS_STRINGS).
 *
 * configure should, we hope <wink>, detect this for you.
 */
#define PF_ARGV_TYPE PF_ARGV_WRITEABLE

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
/* #undef CRAY_STACKSEG_END */

/* Define to the type of elements in the array set by `getgroups'.
   Usually this is either `int' or `gid_t'.  */
#define GETGROUPS_T gid_t

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define as __inline if that's what the C compiler calls it.  */
/* #undef inline */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef mode_t */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #undef off_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define if the `setpgrp' function takes no argument.  */
#define SETPGRP_VOID 1

/* Define if the `setgrent` function returns void.  */
#define SETGRENT_VOID 1

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
 STACK_DIRECTION > 0 => grows toward higher addresses
 STACK_DIRECTION < 0 => grows toward lower addresses
 STACK_DIRECTION = 0 => direction of growth unknown
 */
/* #undef STACK_DIRECTION */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* The number of bytes in an int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a long long.  */
#define SIZEOF_LONG_LONG 8

/* The number of bytes in an off_t.  */
#define SIZEOF_OFF_T 8

/* The number of bytes in a size_t.  */
#define SIZEOF_SIZE_T 4

/* The number of bytes in a time_t.  */
#define SIZEOF_TIME_T 4

/* Define if you have the bcopy function.  */
#define HAVE_BCOPY 1

/* Define if you have the crypt function.  */
#define HAVE_CRYPT 1

/* Define if you have the endprotoent function.  */
#define HAVE_ENDPROTOENT 1

/* Define if you have the fconvert function.  */
/* #undef HAVE_FCONVERT */

/* Define if you have the fcvt function.  */
/* #undef HAVE_FCVT */

/* Define if you have the fdatasync function.  */
#define HAVE_FDATASYNC 1

/* Define if you have the fgetgrent function.  */
#define HAVE_FGETGRENT 1

/* Define if you have the fgetpwent function.  */
#define HAVE_FGETPWENT 1

/* Define if you have the flock function.  */
#define HAVE_FLOCK 1

/* Define if you have the fpathconf function.  */
#define HAVE_FPATHCONF 1

/* Define if you have the fgetspent function.  */
#define HAVE_FGETSPENT 1

/* Define if you have the freeaddrinfo function.  */
#define HAVE_FREEADDRINFO 1

/* Define if you have the gai_strerror function.  */
#define HAVE_GAI_STRERROR 1

/* Define if you have the getaddrinfo function.  */
#define HAVE_GETADDRINFO 1

/* Define if you have the getcwd function.  */
#define HAVE_GETCWD 1

/* Define if you have the getenv function.  */
#define HAVE_GETENV 1

/* Define if you have the gethostbyname2 function.  */
#define HAVE_GETHOSTBYNAME2 1

/* Define if you have the gethostname function.  */
#define HAVE_GETHOSTNAME 1

/* Define if you have the getnameinfo function.  */
#define HAVE_GETNAMEINFO 1

/* Define if you have the getopt function.  */
#define HAVE_GETOPT 1

/* Define if you have the getopt_long function.  */
#define HAVE_GETOPT_LONG 1

/* Define if you have the getprpwent function.  */
/* #undef HAVE_GETPRPWENT */

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the hstrerror function.  */
#define HAVE_HSTRERROR 1

/* Define if you have the iconv function.  */
#define HAVE_ICONV 1

/* Define if you have the inet_aton function.  */
#define HAVE_INET_ATON 1

/* Define if you have the inet_ntop function.  */
#define HAVE_INET_NTOP 1

/* Define if you have the inet_pton function.  */
#define HAVE_INET_PTON 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the mempcpy function.  */
#define HAVE_MEMPCPY 1

/* Define if you have the mkdir function.  */
#define HAVE_MKDIR 1

/* Define if you have the mkstemp function.  */
#define HAVE_MKSTEMP 1

/* Define if you have the mlock function.  */
#define HAVE_MLOCK 1

/* Define if you have the mlockall function.  */
#define HAVE_MLOCKALL 1

/* Define if you have the munlock function.  */
#define HAVE_MUNLOCK 1

/* Define if you have the munlockall function.  */
#define HAVE_MUNLOCKALL 1

/* Define if you have the nl_langinfo function.  */
#define HAVE_NL_LANGINFO 1

/* Define if you have the pathconf function.  */
#define HAVE_PATHCONF 1

/* Define if you have the pstat function.  */
/* #undef HAVE_PSTAT */

/* Define if you have the regcomp function.  */
#define HAVE_REGCOMP 1

/* Define if you have the rmdir function.  */
#define HAVE_RMDIR 1

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the set_auth_parameters function.  */
/* #undef HAVE_SET_AUTH_PARAMETERS */

/* Define if you have the setegid function.  */
#define HAVE_SETEGID 1

/* Define if you have the setenv function.  */
#define HAVE_SETENV 1

/* Define if you have the seteuid function.  */
#define HAVE_SETEUID 1

/* Define if you have the setgroupent function.  */
/* #undef HAVE_SETGROUPENT */

/* Define if you have the setgroups function.  */
#define HAVE_SETGROUPS 1

/* Define if you have the setpgid function.  */
#define HAVE_SETPGID 1

/* Define if you have the setproctitle function.  */
/* #undef HAVE_SETPROCTITLE */

/* Define if your system has the setprotoent function.  */
#define HAVE_SETPROTOENT 1

/* Define if you have the setsid function.  */
#define HAVE_SETSID 1

/* Define if you have the siginterrupt function.  */
#define HAVE_SIGINTERRUPT 1

/* Define if you have the snprintf function.  */
#define HAVE_SNPRINTF 1

/* Define if you have the socket function.  */
#define HAVE_SOCKET 1

/* Define if you have the statfs function.  */
#define HAVE_STATFS 1

/* Define if you have the strchr function.  */
#define HAVE_STRCHR 1

/* Define if you have the strcoll function.  */
#define HAVE_STRCOLL 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strsep function.  */
#define HAVE_STRSEP 1

/* Define if you have the strtol function.  */
#define HAVE_STRTOL 1

/* Define if you have the strtoull function.  */
#define HAVE_STRTOULL 1

/* Define if you have the tzset function.  */
#define HAVE_TZSET 1

/* Define if you have the unsetenv function.  */
#define HAVE_UNSETENV 1

/* Define if you have the vsnprintf function.  */
#define HAVE_VSNPRINTF 1

/* Define if you have the <acl/libacl.h> header file.  */
/* #undef HAVE_ACL_LIBACL_H */

/* Define if you have the <arpa/inet.h> header file.  */
#define HAVE_ARPA_INET_H 1

/* Define if you have the <bstring.h> header file.  */
/* #undef HAVE_BSTRING_H */

/* Define if you have the <crypt.h> header file.  */
#define HAVE_CRYPT_H 1

/* Define if you have the <ctype.h> header file.  */
#define HAVE_CTYPE_H 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <errno.h> header file.  */
#define HAVE_ERRNO_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <floatingpoint.h> header file.  */
/* #undef HAVE_FLOATINGPOINT_H */

/* Define if you have the <getopt.h> header file.  */
#define HAVE_GETOPT_H 1

/* Define if you have the <hpsecurity.h> header file.  */
/* #undef HAVE_HPSECURITY_H */

/* Define if you have the <krb.h> header file.  */
/* #undef HAVE_KRB_H */

/* Define if you have the <iconv.h> header file.  */
#define HAVE_ICONV_H 1

/* Define if you have the <inttypes.h> header file.  */
#define HAVE_INTTYPES_H 1

/* Define if you have the <langinfo.h> header file.  */
#define HAVE_LANGINFO_H 1

/* Define if you have the <libutil.h> header file.  */
/* #undef HAVE_LIBUTIL_H */

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <ncurses.h> header file.  */
/* #undef HAVE_NCURSES_H */

/* Define if you have the <curses.h> header file.  */
/* #undef HAVE_CURSES_H */

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <netdb.h> header file.  */
#define HAVE_NETDB_H 1

/* Define if you have the <netinet/in.h> header file.  */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <netinet/in_systm.h> header file.  */
#define HAVE_NETINET_IN_SYSTM_H 1

/* Define if you have the <netinet/ip.h> header file.  */
#define HAVE_NETINET_IP_H 1

/* Define if you have the <netinet/tcp.h> header file.  */
#define HAVE_NETINET_TCP_H 1

/* Define if you have the <prot.h> header file.  */
/* #undef HAVE_PROT_H */

/* Define if you have the <regex.h> header file.  */
#define HAVE_REGEX_H 1

/* Define if you have the <security/pam_appl.h> header file.  */
/* #undef HAVE_SECURITY_PAM_APPL_H */

/* Define if you have the <pam/pam_appl.h> header file.  */
/* #undef HAVE_PAM_PAM_APPL_H */

/* Define if you have the <shadow.h> header file.  */
#define HAVE_SHADOW_H 1

/* Define if you have the <sia.h> header file.  */
/* #undef HAVE_SIA_H */

/* Define if you have the <siad.h> header file.  */
/* #undef HAVE_SIAD_H */

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <strings.h> header file.  */
#define HAVE_STRINGS_H 1

/* Define if you have the <stropts.h> header file.  */
//#define HAVE_STROPTS_H 1

/* Define if you have the <sys/acl.h> header file.  */
/* #undef HAVE_SYS_ACL_H */

/* Define if you have the <sys/dir.h> header file.  */
#define HAVE_SYS_DIR_H 1

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/mman.h> header file.  */
#define HAVE_SYS_MMAN_H 1

/* Define if you have the <sys/mount.h> header file.  */
#define HAVE_SYS_MOUNT_H 1

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/pstat.h> header file.  */
/* #undef HAVE_SYS_PSTAT_H */

/* Define if you have the <sys/security.h> header file.  */
/* #undef HAVE_SYS_SECURITY_H */

/* Define if you have the <sys/select.h> header file.  */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the <sys/sendfile.h> header file.  */
/* #undef HAVE_SYS_SENDFILE_H */

/* Define if you have the <sys/socket.h> header file.  */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/statvfs.h> header file.  */
#define HAVE_SYS_STATVFS_H 1

/* Define if you have the <sys/termio.h> header file.  */
/* #undef HAVE_SYS_TERMIO_H */

/* Define if you have the <sys/termios.h> header file.  */
#define HAVE_SYS_TERMIOS_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the <sys/uio.h> header file.  */
#define HAVE_SYS_UIO_H 1

/* Define if you have the <sys/vfs.h> header file.  */
#define HAVE_SYS_VFS_H 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <utime.h> header file.  */
#define HAVE_UTIME_H

/* Define if you have the <utmp.h> header file.  */
#define HAVE_UTMP_H 1

/* Define if you have the <utmpx.h> header file.  */
//#define HAVE_UTMPX_H 1

/* Define if you have the "vmsdir.h" header file.  */
/* #undef HAVE_VMSDIR_H */

/* Define if you have the acl library (-lacl).  */
/* #undef HAVE_LIBACL */

/* Define if you have the bind library (-lbind).  */
/* #undef HAVE_LIBBIND */

/* Define if you have the crypt library (-lcrypt).  */
#define HAVE_LIBCRYPT 1

/* Define if you have the curses library (-lcurses).  */
/* #undef HAVE_LIBCURSES */

/* Define if you have the UnixWare gen library (-lgen).  */
/* #undef HAVE_LIBGEN */

/* Define if you have the ncurses library (-lncurses).  */
/* #undef HAVE_LIBNCURSES */

/* Define if you have the nsl library (-lnsl).  */
/* #undef HAVE_LIBNSL */

/* Define if you have the resolv library (-lresolv).  */
/* #undef HAVE_LIBRESOLV */

/* Define if you have the sec library (-lsec).  */
/* #undef HAVE_LIBSEC */

/* Define if you have the security library (-lsecurity).  */
/* #undef HAVE_LIBSECURITY */

/* Define if you have the addrinfo struct.  */
#define HAVE_STRUCT_ADDRINFO 

/* Define if you have the sockaddr_storage struct.  */
#define HAVE_STRUCT_SS 

/* Define if you have the ss_family sockaddr_storage struct member.  */
#define HAVE_SS_FAMILY 1

/* Define if you have the __ss_family sockaddr_storage struct member.  */
/* #undef HAVE___SS_FAMILY */

/* Define if you have the ss_len sockaddr_storage struct member.  */
/* #undef HAVE_SS_LEN */

/* Define if you have the __ss_len sockaddr_storage struct member.  */
/* #undef HAVE___SS_LEN */

/* Define if you have the sin_len sockaddr_in struct member. */
/* #undef SIN_LEN */

/* Define if you have the socket library (-lsocket).  */
/* #undef HAVE_LIBSOCKET */
#ifndef STDIN_FILENO
#define STDIN_FILENO 	0
#endif /* STDIN_FILENO */

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 	1
#endif /* STDOUT_FILENO */

#ifndef STDERR_FILENO
#define STDERR_FILENO	2
#endif /* STDERR_FILENO */

#ifndef PR_CONFIG_DIR
#define PR_CONFIG_DIR "/tmp/proftpd/etc"
#endif /* PR_CONFIG_DIR */

#ifndef PR_LIBEXEC_DIR
#define PR_LIBEXEC_DIR "/tmp/proftpd/libexec"
#endif /* PR_LIBEXEC_DIR */

#ifndef PR_RUN_DIR
#define PR_RUN_DIR "/tmp/proftpd/var/proftpd"
#endif /* PR_RUN_DIR */

#ifndef PR_CONFIG_FILE_PATH
#define PR_CONFIG_FILE_PATH "/tmp/proftpd/etc/proftpd.conf"
#endif /* PR_CONFIG_FILE_PATH */

#ifndef PR_PID_FILE_PATH
#define PR_PID_FILE_PATH "/tmp/proftpd/var/proftpd.pid"
#endif /* PR_PID_FILE_PATH */

/* Number of bits in a file offset, on hosts where this is settable. */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif /* _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define for use of hstrerror on AIX-style hosts. */
/* #undef _USE_IRS */

/* Define if auto-detection of shadow passwords is wanted.  */
/* #undef PR_USE_AUTO_SHADOW */

/* Define if controls support is desired.  */
/* #undef PR_USE_CTRLS */

/* Define if curses support, if available, should be used.  */
#define PR_USE_CURSES 1

/* Define if you are a developer. */
/* #undef PR_USE_DEVEL */

/* Define if DSO support is desired.  */
/* #undef PR_USE_DSO */

/* Define if use of POSIX ACL support is desired.  */
/* #undef PR_USE_FACL */

/* Define if use of builtin getaddrinfo() is desired.  */
/* #undef PR_USE_GETADDRINFO */

/* Define if use of builtin getnameinfo() is desired.  */
/* #undef PR_USE_GETNAMEINFO */

/* Define if IPv6 support is desired.  */
/* #undef PR_USE_IPV6 */

/* Define if largefile support is desired.  */
#define PR_USE_LARGEFILES 1

/* Define if the %llu format should be used.  */
/* #undef HAVE_LLU */

/* Define if the %lu format should be used.  */
#define HAVE_LU 1

/* Define if ncurses support, if available, should be used.  */
#define PR_USE_NCURSES 1

/* Define if sendfile support, if available, should be used.  */
#define PR_USE_SENDFILE 0

/* Define if using /etc/shadow files.  */
#define PR_USE_SHADOW 1

/* Define if using Tru64's C2 SIA authentication.  */
/* #undef PR_USE_SIA */

/* Tunable parameters */
/* #undef PR_TUNABLE_BUFFER_SIZE */
/* #undef PR_TUNABLE_NEW_POOL_SIZE */
/* #undef PR_TUNABLE_SCOREBOARD_BUFFER_SIZE */
/* #undef PR_TUNABLE_TIMEOUTIDENT */
/* #undef PR_TUNABLE_TIMEOUTIDLE */
/* #undef PR_TUNABLE_TIMEOUTLINGER */
/* #undef PR_TUNABLE_TIMEOUTLOGIN */
/* #undef PR_TUNABLE_TIMEOUTNOXFER */
/* #undef PR_TUNABLE_TIMEOUTSTALLED */
/* #undef PR_TUNABLE_XFER_SCOREBOARD_UPDATES */

#ifndef PR_TUNABLE_RCVBUFSZ
/* # undef PR_TUNABLE_RCVBUFSZ */
#endif

#ifndef PR_TUNABLE_SNDBUFSZ
/* # undef PR_TUNABLE_SNDBUFSZ */
#endif

#ifndef PR_TUNABLE_XFER_BUFFER_SIZE
/* # undef PR_TUNABLE_XFER_BUFFER_SIZE */
#endif

#endif /* config_h_included */
