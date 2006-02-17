/*
 * System V init functionality
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: init.c,v 1.4 2005/11/21 15:03:05 seg Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <shutils.h>
#include <utils.h>

#define loop_forever() do { sleep(1); } while (1)
#define SHELL "/bin/sh"


pid_t
ddrun_shell (int timeout, int nowait)
{
  pid_t pid;
  char tz[1000];
  char *envp[] = {
    "TERM=vt100",
    "HOME=/",
    "PATH=/usr/bin:/bin:/usr/sbin:/sbin:/jffs/usr/bin:/jffs/bin:/jffs/usr/sbin:/jffs/sbin",
    "LD_LIBRARY_PATH=/usr/lib:/lib:/jffs/usr/lib:/jffs/lib",
    "SHELL=" SHELL,
    "USER=root",
    tz,
    NULL
  };
  int sig;

  /* Wait for user input */
  // cprintf("Hit enter to continue...");
  if (waitfor (STDIN_FILENO, timeout) <= 0)
    return 0;

  switch ((pid = fork ()))
    {
    case -1:
      perror ("fork");
      return 0;
    case 0:
      /* Reset signal handlers set for parent process */
      for (sig = 0; sig < (_NSIG - 1); sig++)
	signal (sig, SIG_DFL);

      /* Reopen console */
      start_service("console_init");
      
      /* Pass on TZ */
      snprintf (tz, sizeof (tz), "TZ=%s", getenv ("TZ"));

      /* Now run it.  The new program will take over this PID, 
       * so nothing further in init.c should be run. */
      execve (SHELL, (char *[])
	      {
	      "/bin/sh", NULL}, envp);

      /* We're still here?  Some error happened. */
      perror (SHELL);
      exit (errno);
    default:
      if (nowait)
	return pid;
      else
	{
	  waitpid (pid, NULL, 0);
	  return 0;
	}
    }
}

void
shutdown_system (void)
{
  int sig;

  /* Disable signal handlers */
  for (sig = 0; sig < (_NSIG - 1); sig++)
    signal (sig, SIG_DFL);

  /* Blink led before reboot */
  diag_led (DIAG, START_LED);

  cprintf ("Sending SIGTERM to all processes\n");
  kill (-1, SIGTERM);
  sleep (1);

  cprintf ("Sending SIGKILL to all processes\n");
  kill (-1, SIGKILL);
  sleep (1);

  sync ();
#ifdef HAVE_RB500
  eval ("umount", "/");
#endif
}

static int fatal_signals[] = {
  SIGQUIT,
  SIGILL,
#ifndef HAVE_RB500
  SIGABRT,
#endif
  SIGFPE,
  SIGPIPE,
  SIGBUS,
  // SIGSEGV,      // Don't shutdown, when Segmentation fault.
  SIGSYS,
  SIGTRAP,
  SIGPWR,
  SIGTERM,			/* reboot */
  //SIGUSR1,      /* halt */      // We use the for some purpose
};

void
fatal_signal (int sig)
{
  char *message = NULL;

  switch (sig)
    {
    case SIGQUIT:
      message = "Quit";
      break;
    case SIGILL:
      message = "Illegal instruction";
      break;
    case SIGABRT:
      message = "Abort";
      break;
    case SIGFPE:
      message = "Floating exception";
      break;
    case SIGPIPE:
      message = "Broken pipe";
      break;
    case SIGBUS:
      message = "Bus error";
      break;
    case SIGSEGV:
      message = "Segmentation fault";
      break;
    case SIGSYS:
      message = "Bad system call";
      break;
    case SIGTRAP:
      message = "Trace trap";
      break;
    case SIGPWR:
      message = "Power failure";
      break;
    case SIGTERM:
      message = "Terminated";
      break;
      //case SIGUSR1: message = "User-defined signal 1"; break;
    }

  if (message)
    cprintf ("%s....................................\n", message);
  else
    cprintf ("Caught signal %d.......................................\n",
	     sig);

  shutdown_system ();
  sleep (2);

  /* Halt on SIGUSR1 */
  reboot (sig == SIGUSR1 ? RB_HALT_SYSTEM : RB_AUTOBOOT);
  loop_forever ();
}

static void
reap (int sig)
{
  pid_t pid;

  while ((pid = waitpid (-1, NULL, WNOHANG)) > 0)
    cprintf ("Reaped %d\n", pid);
}


void
signal_init (void)
{
  int i;

  for (i = 0; i < sizeof (fatal_signals) / sizeof (fatal_signals[0]); i++)
    signal (fatal_signals[i], fatal_signal);

  signal (SIGCHLD, reap);
}
