/*
 * include/asm-microblaze/unistd.h -- System call numbers and invocation mechanism
 *
 *  Copyright (C) 2003       John Williams <jwilliams@itee.uq.edu.au>
 *  Copyright (C) 2001,2002  NEC Corporation
 *  Copyright (C) 2001,2002  Miles Bader <miles@gnu.org>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of this
 * archive for more details.
 *
 * Written by Miles Bader <miles@gnu.org>
 * Microblaze port by John Williams
 */

#ifndef __MICROBLAZE_UNISTD_H__
#define __MICROBLAZE_UNISTD_H__

#define __NR_exit		  1
#define __NR_fork		  2
#define __NR_read		  3
#define __NR_write		  4
#define __NR_open		  5
#define __NR_close		  6
#define __NR_waitpid		  7
#define __NR_creat		  8
#define __NR_link		  9
#define __NR_unlink		 10
#define __NR_execve		 11
#define __NR_chdir		 12
#define __NR_time		 13
#define __NR_mknod		 14
#define __NR_chmod		 15
#define __NR_chown		 16
#define __NR_break		 17
#define __NR_lseek		 19
#define __NR_getpid		 20
#define __NR_mount		 21
#define __NR_umount		 22
#define __NR_setuid		 23
#define __NR_getuid		 24
#define __NR_stime		 25
#define __NR_ptrace		 26
#define __NR_alarm		 27
#define __NR_pause		 29
#define __NR_utime		 30
#define __NR_stty		 31
#define __NR_gtty		 32
#define __NR_access		 33
#define __NR_nice		 34
#define __NR_ftime		 35
#define __NR_sync		 36
#define __NR_kill		 37
#define __NR_rename		 38
#define __NR_mkdir		 39
#define __NR_rmdir		 40
#define __NR_dup		 41
#define __NR_pipe		 42
#define __NR_times		 43
#define __NR_prof		 44
#define __NR_brk		 45
#define __NR_setgid		 46
#define __NR_getgid		 47
#define __NR_signal		 48
#define __NR_geteuid		 49
#define __NR_getegid		 50
#define __NR_acct		 51
#define __NR_umount2		 52
#define __NR_lock		 53
#define __NR_ioctl		 54
#define __NR_fcntl		 55
#define __NR_mpx		 56
#define __NR_setpgid		 57
#define __NR_umask		 60
#define __NR_chroot		 61
#define __NR_ustat		 62
#define __NR_dup2		 63
#define __NR_getppid		 64
#define __NR_getpgrp		 65
#define __NR_setsid		 66
#define __NR_sigaction		 67
#define __NR_sgetmask		 68
#define __NR_ssetmask		 69
#define __NR_setreuid		 70
#define __NR_setregid		 71
#define __NR_sigsuspend		 72
#define __NR_sigpending		 73
#define __NR_sethostname	 74
#define __NR_setrlimit		 75
#define __NR_old_getrlimit	 76
#define __NR_getrusage		 77
#define __NR_gettimeofday	 78
#define __NR_settimeofday	 79
#define __NR_getgroups		 80
#define __NR_setgroups		 81
#define __NR_select		 82
#define __NR_symlink		 83
#define __NR_readlink		 85
#define __NR_uselib		 86
#define __NR_swapon		 87
#define __NR_reboot		 88
#define __NR_readdir		 89
#define __NR_mmap		 90
#define __NR_munmap		 91
#define __NR_truncate		 92
#define __NR_ftruncate		 93
#define __NR_fchmod		 94
#define __NR_fchown		 95
#define __NR_getpriority	 96
#define __NR_setpriority	 97
#define __NR_profil		 98
#define __NR_statfs		 99
#define __NR_fstatfs		100
#define __NR_socketcall		102
#define __NR_syslog		103
#define __NR_setitimer		104
#define __NR_getitimer		105
#define __NR_stat		106
#define __NR_lstat		107
#define __NR_fstat		108
#define __NR_vhangup		111
#define __NR_wait4		114
#define __NR_swapoff		115
#define __NR_sysinfo		116
#define __NR_ipc		117
#define __NR_fsync		118
#define __NR_sigreturn		119
#define __NR_clone		120
#define __NR_setdomainname	121
#define __NR_uname		122
#define __NR_cacheflush		123
#define __NR_adjtimex		124
#define __NR_mprotect		125
#define __NR_sigprocmask	126
#define __NR_create_module	127
#define __NR_init_module	128
#define __NR_delete_module	129
#define __NR_get_kernel_syms	130
#define __NR_quotactl		131
#define __NR_getpgid		132
#define __NR_fchdir		133
#define __NR_bdflush		134
#define __NR_sysfs		135
#define __NR_personality	136
#define __NR_afs_syscall	137 /* Syscall for Andrew File System */
#define __NR_setfsuid		138
#define __NR_setfsgid		139
#define __NR__llseek		140
#define __NR_getdents		141
#define __NR__newselect		142
#define __NR_flock		143
#define __NR_msync		144
#define __NR_readv		145
#define __NR_writev		146
#define __NR_getsid		147
#define __NR_fdatasync		148
#define __NR__sysctl		149
#define __NR_mlock		150
#define __NR_munlock		151
#define __NR_mlockall		152
#define __NR_munlockall		153
#define __NR_sched_setparam		154
#define __NR_sched_getparam		155
#define __NR_sched_setscheduler		156
#define __NR_sched_getscheduler		157
#define __NR_sched_yield		158
#define __NR_sched_get_priority_max	159
#define __NR_sched_get_priority_min	160
#define __NR_sched_rr_get_interval	161
#define __NR_nanosleep		162
#define __NR_mremap		163
#define __NR_setresuid		164
#define __NR_getresuid		165
#define __NR_query_module	167
#define __NR_poll		168
#define __NR_nfsservctl		169
#define __NR_setresgid		170
#define __NR_getresgid		171
#define __NR_prctl		172
#define __NR_rt_sigreturn	173
#define __NR_rt_sigaction	174
#define __NR_rt_sigprocmask	175
#define __NR_rt_sigpending	176
#define __NR_rt_sigtimedwait	177
#define __NR_rt_sigqueueinfo	178
#define __NR_rt_sigsuspend	179
#define __NR_pread		180
#define __NR_pwrite		181
#define __NR_lchown		182
#define __NR_getcwd		183
#define __NR_capget		184
#define __NR_capset		185
#define __NR_sigaltstack	186
#define __NR_sendfile		187
#define __NR_getpmsg		188	/* some people actually want streams */
#define __NR_putpmsg		189	/* some people actually want streams */
#define __NR_vfork		190
#define __NR_getrlimit		191
#define __NR_mmap2		192
#define __NR_truncate64		193
#define __NR_ftruncate64	194
#define __NR_stat64		195
#define __NR_lstat64		196
#define __NR_fstat64		197
#define __NR_chown32		198
#define __NR_getuid32		199
#define __NR_getgid32		200
#define __NR_geteuid32		201
#define __NR_getegid32		202
#define __NR_setreuid32		203
#define __NR_setregid32		204
#define __NR_getgroups32	205
#define __NR_setgroups32	206
#define __NR_fchown32		207
#define __NR_setresuid32	208
#define __NR_getresuid32	209
#define __NR_setresgid32	210
#define __NR_getresgid32	211
#define __NR_lchown32		212
#define __NR_setuid32		213
#define __NR_setgid32		214
#define __NR_setfsuid32		215
#define __NR_setfsgid32		216
#define __NR_pivot_root		217
/* #define __NR_mincore		218 */
/* #define __NR_madvise		219 */
#define __NR_getdents64		220
#define __NR_fcntl64		221
/* 224 `reserved for TUX' */
/* #define __NR_security	223 */	/* syscall for security modules */
#define __NR_gettid		224
/* #define __NR_readahead	225 */


/* Microblaze syscall protocol:
   Syscall number in r12, args in r5-r10
   Return value in r3.

   System calls are achieved by branching to absolute address 0x8, 
   the exception handler.  Microblaze doesn't currently use exceptions
   so we take them over.  Contrary to the Microblaze ABI, we use r17
   as the exception/trap return link address.  This is due to the
   structure of entry.S, which uses common return code for traps and
   interrupts.  This probably reflects some architectural stuff about 
   the microblaze.  For now, lets leave it that way and worry about it in the future!
*/

#define SYSCALL_NUM	"r12"
#define SYSCALL_ARG0	"r5"
#define SYSCALL_ARG1	"r6"
#define SYSCALL_ARG2	"r7"
#define SYSCALL_ARG3	"r8"
#define SYSCALL_ARG4	"r9"
#define SYSCALL_ARG5	"r10"
#define SYSCALL_RET	"r3"

#define SYSCALL_ENTRY_POINT C_SYMBOL_NAME(syscall_entry)

/* Registers clobbered by a syscall.  */
#define SYSCALL_CLOBBERS	"r4", "r11", "r17", SYSCALL_NUM

#define __syscall_return(type, res)					      \
  do {									      \
	  /* user-visible error numbers are in the range -1 - -124:	      \
	     see <asm-microblaze/errno.h> */					      \
	  if ((unsigned long)(res) >= (unsigned long)(-125)) {		      \
		  errno = -(res);					      \
		  res = -1;						      \
	  }								      \
	  return (type) (res);						      \
  } while (0)

#define _syscall0(type, name)						      \
type name (void)							      \
{									      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop"			      \
			: "=r" (__ret)					      \
			: "r" (__syscall)				      \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}

#define _syscall1(type, name, atype, a)					      \
type name (atype a)							      \
{									      \
  register atype __a __asm__ (SYSCALL_ARG0) = a;			      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop;"			      \
			: "=r" (__ret)					      \
			: "r" (__syscall), "r" (__a)			      \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}

#define _syscall2(type, name, atype, a, btype, b)			      \
type name (atype a, btype b)						      \
{									      \
  register atype __a __asm__ (SYSCALL_ARG0) = a;			      \
  register btype __b __asm__ (SYSCALL_ARG1) = b;			      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop;"			      \
			: "=r" (__ret)					      \
			: "r" (__syscall), "r" (__a), "r" (__b)		      \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}

#define _syscall3(type, name, atype, a, btype, b, ctype, c)		      \
type name (atype a, btype b, ctype c)					      \
{									      \
  register atype __a __asm__ (SYSCALL_ARG0) = a;			      \
  register btype __b __asm__ (SYSCALL_ARG1) = b;			      \
  register ctype __c __asm__ (SYSCALL_ARG2) = c;			      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop;"			      \
			: "=r" (__ret)					      \
			: "r" (__syscall), "r" (__a), "r" (__b), "r" (__c)      \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}

#define _syscall4(type, name, atype, a, btype, b, ctype, c, dtype, d)	      \
type name (atype a, btype b, ctype c, dtype d)				      \
{									      \
  register atype __a __asm__ (SYSCALL_ARG0) = a;			      \
  register btype __b __asm__ (SYSCALL_ARG1) = b;			      \
  register ctype __c __asm__ (SYSCALL_ARG2) = c;			      \
  register dtype __d __asm__ (SYSCALL_ARG3) = d;			      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop;"			      \
			: "=r" (__ret)					      \
			: "r" (__syscall),				      \
			"r" (__a), "r" (__b), "r" (__c), "r" (__d)	      \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}

#define _syscall5(type, name, atype, a, btype, b, ctype, c, dtype, d, etype,e)\
type name (atype a, btype b, ctype c, dtype d, etype e)			      \
{									      \
  register atype __a __asm__ (SYSCALL_ARG0) = a;			      \
  register btype __b __asm__ (SYSCALL_ARG1) = b;			      \
  register ctype __c __asm__ (SYSCALL_ARG2) = c;			      \
  register dtype __d __asm__ (SYSCALL_ARG3) = d;			      \
  register etype __e __asm__ (SYSCALL_ARG4) = e;			      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop;"				      \
			: "=r" (__ret)					      \
			: "r" (__syscall),				      \
			"r" (__a), "r" (__b), "r" (__c), "r" (__d), "r" (__e) \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}

#define _syscall6(type, name, atype, a, btype, b, ctype, c, dtype, d, etype, e, ftype, f) \
type name (atype a, btype b, ctype c, dtype d, etype e, ftype f)	      \
{									      \
  register atype __a __asm__ (SYSCALL_ARG0) = a;			      \
  register btype __b __asm__ (SYSCALL_ARG1) = b;			      \
  register ctype __c __asm__ (SYSCALL_ARG2) = c;			      \
  register dtype __d __asm__ (SYSCALL_ARG3) = d;			      \
  register etype __e __asm__ (SYSCALL_ARG4) = e;			      \
  register etype __f __asm__ (SYSCALL_ARG5) = f;			      \
  register unsigned long __syscall __asm__ (SYSCALL_NUM) = __NR_##name;	      \
  register unsigned long __ret __asm__ (SYSCALL_RET);			      \
  __asm__ __volatile__ ("bralid r17, 0x8; nop;"			      \
			: "=r" (__ret)					      \
			: "r" (__syscall),				      \
			"r" (__a), "r" (__b), "r" (__c), "r" (__d),	      \
			"r" (__e), "r" (__f)				      \
			: SYSCALL_CLOBBERS);				      \
  __syscall_return (type, __ret);					      \
}
		

#ifdef __KERNEL_SYSCALLS__

/*
 * we need this inline - forking from kernel space will result
 * in NO COPY ON WRITE (!!!), until an execve is executed. This
 * is no problem, but for the stack. This is handled by not letting
 * main() use the stack at all after fork(). Thus, no function
 * calls - which means inline code for fork too, as otherwise we
 * would use the stack upon exit from 'fork()'.
 *
 * Actually only pause and fork are needed inline, so that there
 * won't be any messing with the stack from main(), but we define
 * some others too.
 */
#define __NR__exit __NR_exit
extern inline _syscall0(int,pause)
extern inline _syscall0(int,sync)
extern inline _syscall0(pid_t,setsid)
extern inline _syscall3(int,write,int,fd,const char *,buf,off_t,count)
extern inline _syscall3(int,read,int,fd,char *,buf,off_t,count)
extern inline _syscall3(off_t,lseek,int,fd,off_t,offset,int,count)
extern inline _syscall1(int,dup,int,fd)
extern inline _syscall3(int,execve,const char *,file,char **,argv,char **,envp)
extern inline _syscall3(int,open,const char *,file,int,flag,int,mode)
extern inline _syscall1(int,close,int,fd)
extern inline _syscall1(int,_exit,int,exitcode)
extern inline _syscall3(pid_t,waitpid,pid_t,pid,int *,wait_stat,int,options)
extern inline _syscall1(int,delete_module,const char *,name)

extern inline pid_t wait(int * wait_stat)
{
	return waitpid (-1, wait_stat, 0);
}

#endif

#endif /* __MICROBLAZE_UNISTD_H__ */
