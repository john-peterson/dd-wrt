/* include/net-snmp/net-snmp-config.h.  Generated automatically by configure.  */
/* include/net-snmp/net-snmp-config.h.in.  Generated automatically from configure.in by autoheader.  */
#ifndef NET_SNMP_CONFIG_H
#define NET_SNMP_CONFIG_H

/* config.h:  a general config file */

/* Default (SNMP) version number for the tools to use */
#define DEFAULT_SNMP_VERSION 3

/* don't change these values! */
#define SNMPV1      0xAAAA       /* readable by anyone */
#define SNMPV2ANY   0xA000       /* V2 Any type (includes NoAuth) */
#define SNMPV2AUTH  0x8000       /* V2 Authenticated requests only */

/* default list of mibs to load */

#define DEFAULT_MIBS "SNMPv2-MIB:RFC1213-MIB:SNMP-VIEW-BASED-ACM-MIB:SNMP-COMMUNITY-MIB:HOST-RESOURCES-MIB:HOST-RESOURCES-TYPES:UCD-SNMP-MIB:UCD-DEMO-MIB:UCD-DLMOD-MIB"

/* default location to look for mibs to load using the above tokens
   and/or those in the MIBS envrionment variable*/

#define DEFAULT_MIBDIRS "$HOME/.snmp/mibs:/tmp/snmp/share/snmp/mibs"

/* default mib files to load, specified by path. */

/* #undef DEFAULT_MIBFILES */

/* should we compile to use special opaque types: float, double,
   counter64, i64, ui64, union? */
/* #undef OPAQUE_SPECIAL_TYPES */

/* comment the next line if you are compiling with libsnmp.h 
   and are not using the UC-Davis SNMP library. */
#define UCD_SNMP_LIBRARY 1

/* define if you want to compile support for both authentication and
   privacy support. */
/* #undef SCAPI_AUTHPRIV */

/* define if you are using the MD5 code ...*/
#define USE_INTERNAL_MD5 1

/* add in recent CMU library extensions (not complete) */
/* #undef CMU_COMPATIBLE */

/* add in recent resource lock functions (not complete) */
/* #undef _REENTRANT */

/* debugging stuff */
#define SNMP_NO_DEBUGGING 1           /* if defined, we optimize the code
                                      to exclude all debugging calls. */
#define SNMP_ALWAYS_DEBUG 1        /* Always print debugging information and
                                      ignore the -D flag passed to the cmds */

/* reverse encoding BER packets is both faster and more efficient in space. */
#define USE_REVERSE_ASNENCODING       1
#define DEFAULT_ASNENCODING_DIRECTION 1 /* 1 = reverse, 0 = forwards */

/* PERSISTENT_DIRECTORY: If defined, the library is capabile of saving
   persisant information to this directory in the form of configuration
   lines: PERSISTENT_DIRECTORY/NAME.persistent.conf */
#define PERSISTENT_DIRECTORY "/tmp/snmp-persist"

/* PERSISTENT_MASK: the umask permissions to set up the persistent files with */
#define PERSISTENT_MASK 077

/* AGENT_DIRECTORY_MODE: the mode the agents should use to create
   directories with. Since the data stored here is probably sensitive, it
   probably should be read-only by root/administrator. */
#define AGENT_DIRECTORY_MODE 0700

/* MAX_PERSISTENT_BACKUPS:
 *   The maximum number of persistent backups the library will try to
 *   read from the persistent cache directory.  If an application fails to
 *   close down successfully more than this number of times, data will be lost.
 */
#define MAX_PERSISTENT_BACKUPS 10


/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* #undef _ALL_SOURCE */
#endif

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
/* #undef CRAY_STACKSEG_END */

/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if you have the getmntent function.  */
#define HAVE_GETMNTENT 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define as __inline if that's what the C compiler calls it.  */
/* #undef inline */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #undef off_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

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

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

/* define if you are embedding perl in the main agent */
/* #undef NETSNMP_EMBEDDED_PERL */

/* define the system type include file here */
#define SYSTEM_INCLUDE_FILE "net-snmp/system/generic.h"

/* define the machine (cpu) type include file here */
#define MACHINE_INCLUDE_FILE "net-snmp/machine/generic.h"

#define SNMPLIBPATH "/tmp/snmp/lib/snmp"
#define SNMPSHAREPATH "/tmp/snmp/share/snmp"
#define SNMPCONFPATH "/tmp/snmp"
#define SNMPDLMODPATH "/tmp/snmp/lib/snmp/dlmod"

/* LOGFILE:  If defined it closes stdout/err/in and opens this in out/err's
   place.  (stdin is closed so that sh scripts won't wait for it) */

#define LOGFILE "/dev/null"

/* default system contact */
#define SYS_CONTACT "root"

/* system location */
#define SYS_LOC "Unknown"

/* Use libwrap to handle allow/deny hosts? */
/* #undef USE_LIBWRAP */

/* Use dmalloc to do malloc debugging? */
/* #undef HAVE_DMALLOC_H */

/* location of UNIX kernel */
#define KERNEL_LOC "unknown"

/* location of mount table list */
#define ETC_MNTTAB "/etc/mtab"

/* location of swap device (ok if not found) */
/* #undef DMEM_LOC */

#define PSCMD "/bin/ps -e"

/* Where is the uname command */
#define UNAMEPROG "/bin/uname"

/* testing code sections. */
/* #undef SNMP_TESTING_CODE */ 

/* If you don't have root access don't exit upon kmem errors */
/* #undef NO_ROOT_ACCESS */

/* If we don't want to use kmem. */
#define NO_KMEM_USAGE 1

/* If you don't want the agent to report on variables it doesn't have data for */
#define NO_DUMMY_VALUES 1

/* Define if statfs takes 2 args and the second argument has
   type struct fs_data. [Ultrix] */
/* #undef STAT_STATFS_FS_DATA */

/* Define if the TCP timer constants in <netinet/tcp_timer.h>
   depend on the integer variable `hz'.  [FreeBSD 4.x] */
/* #undef TCPTV_NEEDS_HZ */

/* Define if you have the AES_cfb128_encrypt function.  */
/* #undef HAVE_AES_CFB128_ENCRYPT */

/* Define if you have the bcopy function.  */
#define HAVE_BCOPY 1

/* Define if you have the cgetnext function.  */
/* #undef HAVE_CGETNEXT */

/* Define if you have the dlopen function.  */
/* #undef HAVE_DLOPEN */

/* Define if you have the eval_pv function.  */
/* #undef HAVE_EVAL_PV */

/* Define if you have the execv function.  */
#define HAVE_EXECV 1

/* Define if you have the fork function.  */
#define HAVE_FORK 1

/* Define if you have the getaddrinfo function.  */
#define HAVE_GETADDRINFO 1

/* Define if you have the getdtablesize function.  */
#define HAVE_GETDTABLESIZE 1

/* Define if you have the getfsstat function.  */
/* #undef HAVE_GETFSSTAT */

/* Define if you have the getgrnam function.  */
#define HAVE_GETGRNAM 1

/* Define if you have the gethostname function.  */
#define HAVE_GETHOSTNAME 1

/* Define if you have the getipnodebyname function.  */
/* #undef HAVE_GETIPNODEBYNAME */

/* Define if you have the getloadavg function.  */
/* #undef HAVE_GETLOADAVG */

/* Define if you have the getpagesize function.  */
#define HAVE_GETPAGESIZE 1

/* Define if you have the getpid function.  */
#define HAVE_GETPID 1

/* Define if you have the getpwnam function.  */
#define HAVE_GETPWNAM 1

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the if_freenameindex function.  */
#define HAVE_IF_FREENAMEINDEX 1

/* Define if you have the if_nameindex function.  */
#define HAVE_IF_NAMEINDEX 1

/* Define if you have the index function.  */
#define HAVE_INDEX 1

/* Define if you have the knlist function.  */
/* #undef HAVE_KNLIST */

/* Define if you have the kvm_getprocs function.  */
/* #undef HAVE_KVM_GETPROCS */

/* Define if you have the kvm_openfiles function.  */
/* #undef HAVE_KVM_OPENFILES */

/* Define if you have the lrand48 function.  */
#define HAVE_LRAND48 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memmove function.  */
#define HAVE_MEMMOVE 1

/* Define if you have the mkstemp function.  */
#define HAVE_MKSTEMP 1

/* Define if you have the nlist function.  */
/* #undef HAVE_NLIST */

/* Define if you have the perl_eval_pv function.  */
/* #undef HAVE_PERL_EVAL_PV */

/* Define if you have the rand function.  */
#define HAVE_RAND 1

/* Define if you have the random function.  */
#define HAVE_RANDOM 1

/* Define if you have the regcomp function.  */
#define HAVE_REGCOMP 1

/* Define if you have the rpmGetPath function.  */
/* #undef HAVE_RPMGETPATH */

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the setenv function.  */
#define HAVE_SETENV 1

/* Define if you have the setgid function.  */
#define HAVE_SETGID 1

/* Define if you have the setgroups function.  */
#define HAVE_SETGROUPS 1

/* Define if you have the setitimer function.  */
#define HAVE_SETITIMER 1

/* Define if you have the setlocale function.  */
#define HAVE_SETLOCALE 1

/* Define if you have the setmntent function.  */
#define HAVE_SETMNTENT 1

/* Define if you have the setsid function.  */
#define HAVE_SETSID 1

/* Define if you have the setuid function.  */
#define HAVE_SETUID 1

/* Define if you have the sigaction function.  */
#define HAVE_SIGACTION 1

/* Define if you have the sigalrm function.  */
/* #undef HAVE_SIGALRM */

/* Define if you have the sigblock function.  */
#define HAVE_SIGBLOCK 1

/* Define if you have the sighold function.  */
#define HAVE_SIGHOLD 1

/* Define if you have the signal function.  */
#define HAVE_SIGNAL 1

/* Define if you have the sigset function.  */
#define HAVE_SIGSET 1

/* Define if you have the snprintf function.  */
#define HAVE_SNPRINTF 1

/* Define if you have the socket function.  */
#define HAVE_SOCKET 1

/* Define if you have the statfs function.  */
#define HAVE_STATFS 1

/* Define if you have the statvfs function.  */
#define HAVE_STATVFS 1

/* Define if you have the strcasestr function.  */
#define HAVE_STRCASESTR 1

/* Define if you have the strchr function.  */
#define HAVE_STRCHR 1

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strlcpy function.  */
#define HAVE_STRLCPY 1

/* Define if you have the strncasecmp function.  */
#define HAVE_STRNCASECMP 1

/* Define if you have the strtol function.  */
#define HAVE_STRTOL 1

/* Define if you have the strtoul function.  */
#define HAVE_STRTOUL 1

/* Define if you have the system function.  */
#define HAVE_SYSTEM 1

/* Define if you have the tcgetattr function.  */
#define HAVE_TCGETATTR 1

/* Define if you have the uname function.  */
#define HAVE_UNAME 1

/* Define if you have the usleep function.  */
#define HAVE_USLEEP 1

/* Define if you have the vsnprintf function.  */
#define HAVE_VSNPRINTF 1

/* Define if you have the <arpa/inet.h> header file.  */
#define HAVE_ARPA_INET_H 1

/* Define if you have the <asm/page.h> header file.  */
#define HAVE_ASM_PAGE_H 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <dlfcn.h> header file.  */
#define HAVE_DLFCN_H 1

/* Define if you have the <err.h> header file.  */
#define HAVE_ERR_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <fstab.h> header file.  */
/* #undef HAVE_FSTAB_H */

/* Define if you have the <getopt.h> header file.  */
#define HAVE_GETOPT_H 1

/* Define if you have the <grp.h> header file.  */
#define HAVE_GRP_H 1

/* Define if you have the <inet/mib2.h> header file.  */
/* #undef HAVE_INET_MIB2_H */

/* Define if you have the <io.h> header file.  */
/* #undef HAVE_IO_H */

/* Define if you have the <ioctls.h> header file.  */
/* #undef HAVE_IOCTLS_H */

/* Define if you have the <kstat.h> header file.  */
/* #undef HAVE_KSTAT_H */

/* Define if you have the <kvm.h> header file.  */
/* #undef HAVE_KVM_H */

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <linux/hdreg.h> header file.  */
#define HAVE_LINUX_HDREG_H 1

/* Define if you have the <linux/tasks.h> header file.  */
/* #undef HAVE_LINUX_TASKS_H */

/* Define if you have the <locale.h> header file.  */
#define HAVE_LOCALE_H 1

/* Define if you have the <machine/param.h> header file.  */
/* #undef HAVE_MACHINE_PARAM_H */

/* Define if you have the <machine/pte.h> header file.  */
/* #undef HAVE_MACHINE_PTE_H */

/* Define if you have the <machine/types.h> header file.  */
/* #undef HAVE_MACHINE_TYPES_H */

/* Define if you have the <malloc.h> header file.  */
#define HAVE_MALLOC_H 1

/* Define if you have the <mntent.h> header file.  */
#define HAVE_MNTENT_H 1

/* Define if you have the <mtab.h> header file.  */
/* #undef HAVE_MTAB_H */

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <net/if.h> header file.  */
#define HAVE_NET_IF_H 1

/* Define if you have the <net/if_dl.h> header file.  */
/* #undef HAVE_NET_IF_DL_H */

/* Define if you have the <net/if_mib.h> header file.  */
/* #undef HAVE_NET_IF_MIB_H */

/* Define if you have the <net/if_types.h> header file.  */
/* #undef HAVE_NET_IF_TYPES_H */

/* Define if you have the <net/if_var.h> header file.  */
/* #undef HAVE_NET_IF_VAR_H */

/* Define if you have the <net/route.h> header file.  */
#define HAVE_NET_ROUTE_H 1

/* Define if you have the <netdb.h> header file.  */
#define HAVE_NETDB_H 1

/* Define if you have the <netinet/icmp_var.h> header file.  */
/* #undef HAVE_NETINET_ICMP_VAR_H */

/* Define if you have the <netinet/if_ether.h> header file.  */
#define HAVE_NETINET_IF_ETHER_H 1

/* Define if you have the <netinet/in.h> header file.  */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <netinet/in_systm.h> header file.  */
#define HAVE_NETINET_IN_SYSTM_H 1

/* Define if you have the <netinet/in_var.h> header file.  */
/* #undef HAVE_NETINET_IN_VAR_H */

/* Define if you have the <netinet/ip.h> header file.  */
#define HAVE_NETINET_IP_H 1

/* Define if you have the <netinet/ip6.h> header file.  */
#define HAVE_NETINET_IP6_H 1

/* Define if you have the <netinet/ip_icmp.h> header file.  */
#define HAVE_NETINET_IP_ICMP_H 1

/* Define if you have the <netinet/ip_var.h> header file.  */
/* #undef HAVE_NETINET_IP_VAR_H */

/* Define if you have the <netinet/tcp.h> header file.  */
#define HAVE_NETINET_TCP_H 1

/* Define if you have the <netinet/tcp_fsm.h> header file.  */
/* #undef HAVE_NETINET_TCP_FSM_H */

/* Define if you have the <netinet/tcp_timer.h> header file.  */
/* #undef HAVE_NETINET_TCP_TIMER_H */

/* Define if you have the <netinet/tcp_var.h> header file.  */
/* #undef HAVE_NETINET_TCP_VAR_H */

/* Define if you have the <netinet/tcpip.h> header file.  */
/* #undef HAVE_NETINET_TCPIP_H */

/* Define if you have the <netinet/udp.h> header file.  */
#define HAVE_NETINET_UDP_H 1

/* Define if you have the <netinet/udp_var.h> header file.  */
/* #undef HAVE_NETINET_UDP_VAR_H */

/* Define if you have the <netinet6/in6_pcb.h> header file.  */
/* #undef HAVE_NETINET6_IN6_PCB_H */

/* Define if you have the <netinet6/in6_var.h> header file.  */
/* #undef HAVE_NETINET6_IN6_VAR_H */

/* Define if you have the <netinet6/ip6_var.h> header file.  */
/* #undef HAVE_NETINET6_IP6_VAR_H */

/* Define if you have the <netinet6/nd6.h> header file.  */
/* #undef HAVE_NETINET6_ND6_H */

/* Define if you have the <netinet6/tcp6.h> header file.  */
/* #undef HAVE_NETINET6_TCP6_H */

/* Define if you have the <netinet6/tcp6_fsm.h> header file.  */
/* #undef HAVE_NETINET6_TCP6_FSM_H */

/* Define if you have the <netinet6/tcp6_timer.h> header file.  */
/* #undef HAVE_NETINET6_TCP6_TIMER_H */

/* Define if you have the <netinet6/tcp6_var.h> header file.  */
/* #undef HAVE_NETINET6_TCP6_VAR_H */

/* Define if you have the <netipx/ipx.h> header file.  */
#define HAVE_NETIPX_IPX_H 1

/* Define if you have the <nlist.h> header file.  */
/* #undef HAVE_NLIST_H */

/* Define if you have the <openssl/aes.h> header file.  */
/* #undef HAVE_OPENSSL_AES_H */

/* Define if you have the <openssl/des.h> header file.  */
/* #undef HAVE_OPENSSL_DES_H */

/* Define if you have the <openssl/evp.h> header file.  */
/* #undef HAVE_OPENSSL_EVP_H */

/* Define if you have the <openssl/hmac.h> header file.  */
/* #undef HAVE_OPENSSL_HMAC_H */

/* Define if you have the <osreldate.h> header file.  */
/* #undef HAVE_OSRELDATE_H */

/* Define if you have the <pkginfo.h> header file.  */
/* #undef HAVE_PKGINFO_H */

/* Define if you have the <pkglocs.h> header file.  */
/* #undef HAVE_PKGLOCS_H */

/* Define if you have the <pthread.h> header file.  */
#define HAVE_PTHREAD_H 1

/* Define if you have the <pwd.h> header file.  */
#define HAVE_PWD_H 1

/* Define if you have the <regex.h> header file.  */
#define HAVE_REGEX_H 1

/* Define if you have the <rpmio.h> header file.  */
/* #undef HAVE_RPMIO_H */

/* Define if you have the <search.h> header file.  */
#define HAVE_SEARCH_H 1

/* Define if you have the <sgtty.h> header file.  */
#define HAVE_SGTTY_H 1

/* Define if you have the <stdarg.h> header file.  */
#define HAVE_STDARG_H 1

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/cdefs.h> header file.  */
#define HAVE_SYS_CDEFS_H 1

/* Define if you have the <sys/conf.h> header file.  */
/* #undef HAVE_SYS_CONF_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/diskio.h> header file.  */
/* #undef HAVE_SYS_DISKIO_H */

/* Define if you have the <sys/dkio.h> header file.  */
/* #undef HAVE_SYS_DKIO_H */

/* Define if you have the <sys/dmap.h> header file.  */
/* #undef HAVE_SYS_DMAP_H */

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/filio.h> header file.  */
/* #undef HAVE_SYS_FILIO_H */

/* Define if you have the <sys/fixpoint.h> header file.  */
/* #undef HAVE_SYS_FIXPOINT_H */

/* Define if you have the <sys/fs.h> header file.  */
/* #undef HAVE_SYS_FS_H */

/* Define if you have the <sys/hashing.h> header file.  */
/* #undef HAVE_SYS_HASHING_H */

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/loadavg.h> header file.  */
/* #undef HAVE_SYS_LOADAVG_H */

/* Define if you have the <sys/mbuf.h> header file.  */
/* #undef HAVE_SYS_MBUF_H */

/* Define if you have the <sys/mntent.h> header file.  */
/* #undef HAVE_SYS_MNTENT_H */

/* Define if you have the <sys/mnttab.h> header file.  */
/* #undef HAVE_SYS_MNTTAB_H */

/* Define if you have the <sys/mount.h> header file.  */
#define HAVE_SYS_MOUNT_H 1

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/pool.h> header file.  */
/* #undef HAVE_SYS_POOL_H */

/* Define if you have the <sys/proc.h> header file.  */
/* #undef HAVE_SYS_PROC_H */

/* Define if you have the <sys/protosw.h> header file.  */
/* #undef HAVE_SYS_PROTOSW_H */

/* Define if you have the <sys/pstat.h> header file.  */
/* #undef HAVE_SYS_PSTAT_H */

/* Define if you have the <sys/queue.h> header file.  */
#define HAVE_SYS_QUEUE_H 1

/* Define if you have the <sys/select.h> header file.  */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the <sys/socket.h> header file.  */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/socketvar.h> header file.  */
#define HAVE_SYS_SOCKETVAR_H 1

/* Define if you have the <sys/sockio.h> header file.  */
/* #undef HAVE_SYS_SOCKIO_H */

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/statfs.h> header file.  */
#define HAVE_SYS_STATFS_H 1

/* Define if you have the <sys/statvfs.h> header file.  */
#define HAVE_SYS_STATVFS_H 1

/* Define if you have the <sys/stream.h> header file.  */
/* #undef HAVE_SYS_STREAM_H */

/* Define if you have the <sys/swap.h> header file.  */
#define HAVE_SYS_SWAP_H 1

/* Define if you have the <sys/sysctl.h> header file.  */
#define HAVE_SYS_SYSCTL_H 1

/* Define if you have the <sys/sysmp.h> header file.  */
/* #undef HAVE_SYS_SYSMP_H */

/* Define if you have the <sys/tcpipstats.h> header file.  */
/* #undef HAVE_SYS_TCPIPSTATS_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/un.h> header file.  */
#define HAVE_SYS_UN_H 1

/* Define if you have the <sys/user.h> header file.  */
#define HAVE_SYS_USER_H 1

/* Define if you have the <sys/utsname.h> header file.  */
#define HAVE_SYS_UTSNAME_H 1

/* Define if you have the <sys/vfs.h> header file.  */
#define HAVE_SYS_VFS_H 1

/* Define if you have the <sys/vm.h> header file.  */
/* #undef HAVE_SYS_VM_H */

/* Define if you have the <sys/vmmac.h> header file.  */
/* #undef HAVE_SYS_VMMAC_H */

/* Define if you have the <sys/vmmeter.h> header file.  */
/* #undef HAVE_SYS_VMMETER_H */

/* Define if you have the <sys/vmparam.h> header file.  */
/* #undef HAVE_SYS_VMPARAM_H */

/* Define if you have the <sys/vmsystm.h> header file.  */
/* #undef HAVE_SYS_VMSYSTM_H */

/* Define if you have the <sys/vnode.h> header file.  */
/* #undef HAVE_SYS_VNODE_H */

/* Define if you have the <syslog.h> header file.  */
#define HAVE_SYSLOG_H 1

/* Define if you have the <ufs/ffs/fs.h> header file.  */
/* #undef HAVE_UFS_FFS_FS_H */

/* Define if you have the <ufs/fs.h> header file.  */
/* #undef HAVE_UFS_FS_H */

/* Define if you have the <ufs/ufs/dinode.h> header file.  */
/* #undef HAVE_UFS_UFS_DINODE_H */

/* Define if you have the <ufs/ufs/inode.h> header file.  */
/* #undef HAVE_UFS_UFS_INODE_H */

/* Define if you have the <ufs/ufs/quota.h> header file.  */
/* #undef HAVE_UFS_UFS_QUOTA_H */

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <utmpx.h> header file.  */
/* #undef HAVE_UTMPX_H */

/* Define if you have the <utsname.h> header file.  */
/* #undef HAVE_UTSNAME_H */

/* Define if you have the <uvm/uvm_extern.h> header file.  */
/* #undef HAVE_UVM_UVM_EXTERN_H */

/* Define if you have the <uvm/uvm_param.h> header file.  */
/* #undef HAVE_UVM_UVM_PARAM_H */

/* Define if you have the <vm/swap_pager.h> header file.  */
/* #undef HAVE_VM_SWAP_PAGER_H */

/* Define if you have the <vm/vm.h> header file.  */
/* #undef HAVE_VM_VM_H */

/* Define if you have the <vm/vm_extern.h> header file.  */
/* #undef HAVE_VM_VM_EXTERN_H */

/* Define if you have the <vm/vm_param.h> header file.  */
/* #undef HAVE_VM_VM_PARAM_H */

/* Define if you have the <winsock.h> header file.  */
/* #undef HAVE_WINSOCK_H */

/* Define if you have the <xti.h> header file.  */
/* #undef HAVE_XTI_H */

/* Define if you have the RSAglue library (-lRSAglue).  */
/* #undef HAVE_LIBRSAGLUE */

/* Define if you have the crypto library (-lcrypto).  */
/* #undef HAVE_LIBCRYPTO */

/* Define if you have the dl library (-ldl).  */
#define HAVE_LIBDL 1

/* Define if you have the efence library (-lefence).  */
/* #undef HAVE_LIBEFENCE */

/* Define if you have the elf library (-lelf).  */
/* #undef HAVE_LIBELF */

/* Define if you have the kstat library (-lkstat).  */
/* #undef HAVE_LIBKSTAT */

/* Define if you have the kvm library (-lkvm).  */
/* #undef HAVE_LIBKVM */

/* Define if you have the m library (-lm).  */
#define HAVE_LIBM 1

/* Define if you have the mld library (-lmld).  */
/* #undef HAVE_LIBMLD */

/* Define if you have the nsl library (-lnsl).  */
/* #undef HAVE_LIBNSL */

/* Define if you have the rsaref library (-lrsaref).  */
/* #undef HAVE_LIBRSAREF */

/* Define if you have the z library (-lz).  */
/* #undef HAVE_LIBZ */

/* define if you have getdevs() */
/* #undef HAVE_GETDEVS */

/* define if you have <netinet/in_pcb.h> */
/* #undef HAVE_NETINET_IN_PCB_H */

/* define if you have <sys/disklabel.h> */
/* #undef HAVE_SYS_DISKLABEL_H */

/* define if you are using linux and /proc/net/dev has the compressed
   field, which exists in linux kernels 2.2 and greater. */
/* #undef PROC_NET_DEV_HAS_COMPRESSED */

/* define rtentry to ortentry on SYSV machines (alphas) */
#define RTENTRY struct rtentry

/* Use BSD 4.4 routing table entries? */
/* #undef RTENTRY_4_4 */

/* Does struct sigaction have a sa_sigaction field? */
#define STRUCT_SIGACTION_HAS_SA_SIGACTION 1

/* Does struct sockaddr have a sa_len field? */
/* #undef STRUCT_SOCKADDR_HAS_SA_LEN */

/* Does struct sockaddr have a sa_family2 field? */
/* #undef STRUCT_SOCKADDR_HAS_SA_UNION_SA_GENERIC_SA_FAMILY2 */

/* rtentry structure tests */
/* #undef RTENTRY_RT_NEXT */
#define STRUCT_RTENTRY_HAS_RT_DST 1
/* #undef STRUCT_RTENTRY_HAS_RT_UNIT */
/* #undef STRUCT_RTENTRY_HAS_RT_USE */
/* #undef STRUCT_RTENTRY_HAS_RT_REFCNT */
/* #undef STRUCT_RTENTRY_HAS_RT_HASH */

/* ifnet structure tests */
/* #undef STRUCT_IFNET_HAS_IF_BAUDRATE */
/* #undef STRUCT_IFNET_HAS_IF_BAUDRATE_IFS_VALUE */
#define STRUCT_IFNET_HAS_IF_SPEED 1
#define STRUCT_IFNET_HAS_IF_TYPE 1
/* #undef STRUCT_IFNET_HAS_IF_IMCASTS */
#define STRUCT_IFNET_HAS_IF_IQDROPS 1
/* #undef STRUCT_IFNET_HAS_IF_LASTCHANGE_TV_SEC */
/* #undef STRUCT_IFNET_HAS_IF_NOPROTO */
/* #undef STRUCT_IFNET_HAS_IF_OMCASTS */
/* #undef STRUCT_IFNET_HAS_IF_XNAME */
#define STRUCT_IFNET_HAS_IF_OBYTES 1
#define STRUCT_IFNET_HAS_IF_IBYTES 1
/* #undef STRUCT_IFNET_HAS_IF_ADDRLIST */

/* tcpstat.tcps_rcvmemdrop */
/* #undef STRUCT_TCPSTAT_HAS_TCPS_RCVMEMDROP */

/* udpstat.udps_discard */
/* #undef STRUCT_UDPSTAT_HAS_UDPS_DISCARD */

/* udpstat.udps_discard */
/* #undef STRUCT_UDPSTAT_HAS_UDPS_NOPORT */

/* udpstat.udps_discard */
/* #undef STRUCT_UDPSTAT_HAS_UDPS_NOPORTBCAST */

/* udpstat.udps_discard */
/* #undef STRUCT_UDPSTAT_HAS_UDPS_FULLSOCK */

/* arphd.at_next */
/* #undef STRUCT_ARPHD_HAS_AT_NEXT */

/* ifaddr.ifa_next */
#define STRUCT_IFADDR_HAS_IFA_NEXT 1

/* ifnet.if_mtu */
/* #undef STRUCT_IFNET_HAS_IF_MTU */

/* swdevt.sw_nblksenabled */
/* #undef STRUCT_SWDEVT_HAS_SW_NBLKSENABLED */

/* nlist.n_value */
/* #undef STRUCT_NLIST_HAS_N_VALUE */

/* ipstat structure tests */
/* #undef STRUCT_IPSTAT_HAS_IPS_CANTFORWARD */
/* #undef STRUCT_IPSTAT_HAS_IPS_CANTFRAG */
/* #undef STRUCT_IPSTAT_HAS_IPS_DELIVERED */
/* #undef STRUCT_IPSTAT_HAS_IPS_FRAGDROPPED */
/* #undef STRUCT_IPSTAT_HAS_IPS_FRAGTIMEOUT */
/* #undef STRUCT_IPSTAT_HAS_IPS_LOCALOUT */
/* #undef STRUCT_IPSTAT_HAS_IPS_NOPROTO */
/* #undef STRUCT_IPSTAT_HAS_IPS_NOROUTE */
/* #undef STRUCT_IPSTAT_HAS_IPS_ODROPPED */
/* #undef STRUCT_IPSTAT_HAS_IPS_OFRAGMENTS */
/* #undef STRUCT_IPSTAT_HAS_IPS_REASSEMBLED */

/* vfsstat.f_frsize */
#define STRUCT_STATVFS_HAS_F_FRSIZE 1

/* vfsstat.f_files */
#define STRUCT_STATVFS_HAS_F_FILES 1

/* statfs inode structure tests*/
#define STRUCT_STATFS_HAS_F_FILES 1
#define STRUCT_STATFS_HAS_F_FFREE 1
/* #undef STRUCT_STATFS_HAS_F_FAVAIL */

/* des_ks_struct.weak_key */
/* #undef STRUCT_DES_KS_STRUCT_HAS_WEAK_KEY */

/* ifnet needs to have _KERNEL defined */
/* #undef IFNET_NEEDS_KERNEL */

/* sysctl works to get boottime, etc... */
/* #undef CAN_USE_SYSCTL */

/* type check for in_addr_t */
/* #undef in_addr_t */

/* define if SIOCGIFADDR exists in sys/ioctl.h */
#define SYS_IOCTL_H_HAS_SIOCGIFADDR 1

/* define if your compiler (processor) defines __FUNCTION__ for you */
#define HAVE_CPP_UNDERBAR_FUNCTION_DEFINED 1

/* Mib-2 tree Info */
/* These are the system information variables. */

#define VERS_DESC   "unknown"             /* overridden at run time */
#define SYS_NAME    "unknown"             /* overridden at run time */

/* comment out the second define to turn off functionality for any of
   these: (See README for details) */

/*   proc PROCESSNAME [MAX] [MIN] */
#define PROCMIBNUM 2

/*   exec/shell NAME COMMAND      */
#define SHELLMIBNUM 8

/*   swap MIN                     */
#define MEMMIBNUM 4

/*   disk DISK MINSIZE            */
#define DISKMIBNUM 9

/*   load 1 5 15                  */
#define LOADAVEMIBNUM 10

/* which version are you using? This mibloc will tell you */
#define VERSIONMIBNUM 100

/* Reports errors the agent runs into */
/* (typically its "can't fork, no mem" problems) */
#define ERRORMIBNUM 101

/* The sub id of EXTENSIBLEMIB returned to queries of
   .iso.org.dod.internet.mgmt.mib-2.system.sysObjectID.0 */
#define AGENTID 250

/* This ID is returned after the AGENTID above.  IE, the resulting
   value returned by a query to sysObjectID is
   EXTENSIBLEMIB.AGENTID.???, where ??? is defined below by OSTYPE */

#define HPUX9ID 1
#define SUNOS4ID 2 
#define SOLARISID 3
#define OSFID 4
#define ULTRIXID 5
#define HPUX10ID 6
#define NETBSD1ID 7
#define FREEBSDID 8
#define IRIXID 9
#define LINUXID 10
#define BSDIID 11
#define OPENBSDID 12
#define WIN32ID 13
#define HPUX11ID 14
#define UNKNOWNID 255

#ifdef hpux9
#define OSTYPE HPUX9ID
#endif
#ifdef hpux10
#define OSTYPE HPUX10ID
#endif
#ifdef hpux11
#define OSTYPE HPUX11ID
#endif
#ifdef sunos4
#define OSTYPE SUNOS4ID
#endif
#ifdef solaris2
#define OSTYPE SOLARISID
#endif
#if defined(osf3) || defined(osf4)
#define OSTYPE OSFID
#endif
#ifdef ultrix4
#define OSTYPE ULTRIXID
#endif
#ifdef netbsd1
#define OSTYPE NETBSD1ID
#endif
#ifdef freebsd2
#define OSTYPE FREEBSDID
#endif
#if defined(irix6) || defined(irix5)
#define OSTYPE IRIXID
#endif
#ifdef linux
#define OSTYPE LINUXID
#endif
#if defined(bsdi2) || defined(bsdi3) || defined(bsdi4)
#define OSTYPE BSDIID
#endif
#ifdef openbsd2
#define OSTYPE OPENBSDID
#endif
/* unknown */
#ifndef OSTYPE
#define OSTYPE UNKNOWNID
#endif

/* The enterprise number has been assigned by the IANA group.   */
/* Optionally, this may point to the location in the tree your  */
/* company/organization has been allocated.                     */
/* The assigned enterprise number for the NET_SNMP MIB modules. */
#define ENTERPRISE_OID			8072
#define ENTERPRISE_MIB			1,3,6,1,4,1,8072
#define ENTERPRISE_DOT_MIB		1.3.6.1.4.1.8072
#define ENTERPRISE_DOT_MIB_LENGTH	7

/* The assigned enterprise number for sysObjectID. */
#define SYSTEM_MIB		1,3,6,1,4,1,8072,3,2,OSTYPE
#define SYSTEM_DOT_MIB		1.3.6.1.4.1.8072.3.2.OSTYPE
#define SYSTEM_DOT_MIB_LENGTH	10

/* The assigned enterprise number for notifications. */
#define NOTIFICATION_MIB		1,3,6,1,4,1,8072,4
#define NOTIFICATION_DOT_MIB		1.3.6.1.4.1.8072.4
#define NOTIFICATION_DOT_MIB_LENGTH	8

/* this is the location of the ucdavis mib tree.  It shouldn't be
   changed, as the places it is used are expected to be constant
   values or are directly tied to the UCD-SNMP-MIB. */
#define UCDAVIS_OID		2021
#define UCDAVIS_MIB		1,3,6,1,4,1,2021
#define UCDAVIS_DOT_MIB		1.3.6.1.4.1.2021
#define UCDAVIS_DOT_MIB_LENGTH	7

/* how long to wait (seconds) for error querys before reseting the error trap.*/
#define ERRORTIMELENGTH 600 

/* Exec command to fix PROC problems */
/* %s will be replaced by the process name in error */

/* #define PROCFIXCMD "/usr/bin/perl /local/scripts/fixproc %s" */

/* Exec command to fix EXEC problems */
/* %s will be replaced by the exec/script name in error */

/* #define EXECFIXCMD "/usr/bin/perl /local/scripts/fixproc %s" */

/* Should exec output Cashing be used (speeds up things greatly), and
   if so, After how many seconds should the cache re-newed?  Note:
   Don't define CASHETIME to disable cashing completely */

#define EXCACHETIME 30
#define CACHEFILE ".snmp-exec-cache"
#define MAXCACHESIZE (200*80)   /* roughly 200 lines max */

#define MAXDISKS 50                      /* can't scan more than this number */

/* misc defaults */

/* default of 100 meg minimum if the minimum size is not specified in
   the config file */
#define DEFDISKMINIMUMSPACE 100000

#define DEFMAXLOADAVE 12.0      /* default maximum load average before error */

#define MAXREADCOUNT 100   /* max times to loop reading output from
                              execs.  Because of sleep(1)s, this will also
                              be time to wait (in seconds) for exec to finish */

#define SNMPBLOCK 1       /* Set to 1 if you want snmpgets to block and never
                             timeout.  Original CMU code had this
                             hardcoded into the code as = 1 */

#define RESTARTSLEEP 5    /* How long to wait after a snmpset to
                             EXTENSIBLEMIB.VERSIONMIBNUM.VERRESTARTAGENT
                             before restarting the agent.  This is
                             necessary to finish the snmpset reply
                             before restarting. */

/* Number of community strings to store */
#define NUM_COMMUNITIES	5

/* UNdefine to allow specifying zero-length community string */
/* #define NO_ZEROLENGTH_COMMUNITY 1 */

/* #define EXIT_ON_BAD_KLREAD  */
/* define to exit the agent on a bad kernel read */

#define LASTFIELD -1      /* internal define */

/* configure options specified */
#define CONFIGURE_OPTIONS "\" --quiet --prefix=/tmp/snmp --host=mipsel --with-cc=mipsel-linux-uclibc-gcc --with-ar=mipsel-linux-uclibc-ar --with-endianness=little '--with-cflags=-Os -mtune=mips32 -mips32 -pipe -funit-at-a-time -DCAN_USE_SYSCTL=1' --enable-mini-agent --disable-debugging --disable-privacy --without-opaque-special-types --with-persistent-directory=/tmp/snmp-persist --with-default-snmp-version=3 --with-sys-contact=root --with-sys-location=Unknown --with-logfile=/dev/null --with-out-transports=UDPIPv6,TCPIPv6,AAL5PVC,IPX,TCP,Unix --enable-shared=no --enable-static --with-gnu-ld --enable-internal-md5 --with-copy-persistent-files=no --without-openssl -sysconfdir=/tmp --with-mib-modules=mibII,host,mibII/ip,mibII/tcp,mibII/udp,mibII/icmp,mibII/var_route,mibII/kernel_linux,qos,ucd_snmp,ieee802dot11,avc802dot11 --with-out-mib-modules=snmpv3mibs,agent_mips,agentx,notification,utilities,target --disable-ipv6 --with-defaults --without-efence --without-rsaref --without-kmem-usage --without-rpm --without-dmalloc\""

/* got socklen_t? */
#define HAVE_SOCKLEN_T 1

/* got in_addr_t? */
#define HAVE_IN_ADDR_T 1

#ifndef HAVE_STRCHR
#ifdef HAVE_INDEX
# define strchr index
# define strrchr rindex
#endif
#endif

#ifndef HAVE_INDEX
#ifdef HAVE_STRCHR
# define index strchr
# define rindex strrchr
#endif
#endif

#ifndef HAVE_MEMCPY
#ifdef HAVE_BCOPY
# define memcpy(d, s, n) bcopy ((s), (d), (n))
# define memmove(d, s, n) bcopy ((s), (d), (n))
# define memcmp bcmp
#endif
#endif

#ifndef HAVE_MEMMOVE
#ifdef HAVE_MEMCPY
# define memmove memcpy
#endif
#endif

#ifndef HAVE_BCOPY
#ifdef HAVE_MEMCPY
# define bcopy(s, d, n) memcpy ((d), (s), (n))
# define bzero(p,n) memset((p),(0),(n))
# define bcmp memcmp
#endif
#endif

/* If you have openssl 0.9.7 or above, you likely have AES support. */
/* #undef USE_OPENSSL */
#if defined(USE_OPENSSL) && defined(HAVE_OPENSSL_AES_H) && defined(HAVE_AES_CFB128_ENCRYPT)
#define HAVE_AES 1
#endif

/* define random functions */

#ifndef HAVE_RANDOM
#ifdef HAVE_LRAND48
#define random lrand48
#define srandom(s) srand48(s)
#else
#ifdef HAVE_RAND
#define random rand
#define srandom(s) srand(s)
#endif
#endif
#endif

/* define signal if DNE */

#ifndef HAVE_SIGNAL
#ifdef HAVE_SIGSET
#define signal(a,b) sigset(a,b)
#endif
#endif

/* define if you have librpm and libdb */
/* #undef HAVE_LIBDB */
/* #undef HAVE_LIBRPM */

/* define if you have pkginfo */
/* #undef HAVE_PKGINFO */

/* define if you have gethostbyname */
#define HAVE_GETHOSTBYNAME 1

/* define if you have getservbyname */
#define HAVE_GETSERVBYNAME 1

/* printing system */
/* #undef HAVE_LPSTAT */
/* #undef LPSTAT_PATH */
/* #undef HAVE_PRINTCAP */

/*  Pluggable transports.  */

/*  This is defined if support for the UDP/IP transport domain is
    available.   */
#define SNMP_TRANSPORT_UDP_DOMAIN 1

/*  This is defined if support for the TCP/IP transport domain is
    available.  */
/* #undef SNMP_TRANSPORT_TCP_DOMAIN */

/*  This is defined if support for the Unix transport domain
    (a.k.a. "local IPC") is available.  */
/* #undef SNMP_TRANSPORT_UNIX_DOMAIN */

/*  This is defined if support for the AAL5 PVC transport domain is
    available.  */
/* #undef SNMP_TRANSPORT_AAL5PVC_DOMAIN */

/*  This is defined if support for the IPX transport domain is
    available.  */
/* #undef SNMP_TRANSPORT_IPX_DOMAIN */

/*  This is defined if support for the UDP/IPv6 transport domain is
    available.  */
/* #undef SNMP_TRANSPORT_UDPIPV6_DOMAIN */

/*  This is defined if support for the TCP/IPv6 transport domain is
    available.  */
/* #undef SNMP_TRANSPORT_TCPIPV6_DOMAIN */

/* define this if the USM security module is available */
#define SNMP_SECMOD_USM 1

/* define this if the KSM (kerberos based snmp) security module is available */
/* #undef SNMP_SECMOD_KSM */

/* define this if we're using the new MIT crypto API */
/* #undef MIT_NEW_CRYPTO */

/* define if you want to build with reentrant/threaded code */
/* #undef NS_REENTRANT */

/* Not-to-be-compiled macros for use by configure only */
#define config_require(x)
#define config_arch_require(x,y)
#define config_parse_dot_conf(w,x,y,z)
#define config_add_mib(x)
  
#ifdef WIN32
#define ENV_SEPARATOR ";"
#define ENV_SEPARATOR_CHAR ';'
#else
#define ENV_SEPARATOR ":"
#define ENV_SEPARATOR_CHAR ':'
#endif

#include SYSTEM_INCLUDE_FILE
#include MACHINE_INCLUDE_FILE

#if defined(HAVE_NLIST) && defined(STRUCT_NLIST_HAS_N_VALUE) && !defined(DONT_USE_NLIST) && !defined(NO_KMEM_USAGE)
#define CAN_USE_NLIST
#endif

#if HAVE_DMALLOC_H
#define DMALLOC_FUNC_CHECK
#endif

/* #undef INET6 */

#ifndef NETSNMP_INLINE
#   define NETSNMP_NO_INLINE
#   define NETSNMP_INLINE
#endif

#endif /* NET_SNMP_CONFIG_H */
