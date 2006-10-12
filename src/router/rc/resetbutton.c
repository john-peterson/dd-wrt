/*
 * Reset button 
 *
 * Copyright 2002, Cybertan Corporation
 * All Rights Reserved.
 *
 * Description:
 *   This program checks the Reset-Button status periodically.
 *   There is two senaria, whenever the button is pushed
 *     o  Less than 3 seconds : reboot.
 *     o  Greater than 3 seconds : factory default restore, and reboot. 
 *
 *   The reset-button is connected to the GPIO pin, it has character
 *   device driver to manage it by file operation read/write.
 *
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Cybertan Corporation;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Cybertan Corporation.
 *
 */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include <rc.h>
#include <stdarg.h>
#include <dirent.h>

// #define BCM47XX_SOFTWARE_RESET  0x40 /* GPIO 6 */
// #define BCM47XX_SW_PUSH         0x10	/* GPIO 4 */

// #define WHR_SOFTWARE_RESET 0x10      //GPIO 4  , should work with Buffalo WBR-G54 too, and WZRRSG54
// #define WHR_SW_PUSH 0		//GPIO 0, code unknown

// #define WBR2_SOFTWARE_RESET 0x80     //GPIO 7
// #define WBR2_SW_PUSH 0		//GPIO 0, code unknown

#define	SES_LED_CHECK_TIMES	"9999"	/* How many times to check? */
#define	SES_LED_CHECK_INTERVAL	"1"	/* Wait interval seconds */
#define RESET_WAIT		3	/* seconds */
#define RESET_WAIT_COUNT	RESET_WAIT * 10	/* 10 times a second */

#define NORMAL_INTERVAL		1	/* second */
#define URGENT_INTERVAL		100 * 1000	/* microsecond */
						/* 1/10 second */
#ifdef HAVE_XSCALE
#define GPIO_FILE		"/proc/sys/reset"
#else
#define GPIO_FILE		"/dev/gpio/in"
#endif
#if 0
#define DEBUG printf
#else
#define DEBUG(format, args...)
#endif



#ifdef HAVE_MAGICBOX
#include <sys/mman.h>

#define GPIO0_OR   0x0700 /* rw, output */
#define GPIO0_TCR  0x0704 /* rw, three-state control */
#define GPIO0_ODR  0x0718 /* rw, open drain */
#define GPIO0_IR   0x071c /* ro, input */
#define GPIO0_BASE 0xef600000 /* page */

#define GPIO_LED    0x20000000  /* GPIO1 */
#define GPIO_BUTTON 0x40000000  /* GPIO2 */

#define REG(buf, offset) ((unsigned int *)((void *)buf + offset))

static unsigned int *page;
static int fd;

void init_gpio()
{
     void *start = 0;

     fd = open("/dev/mem", O_RDWR);
     if (fd < 0) {
//	  syslog(LOG_ERR, "Can't open /dev/mem: %s", strerror(errno));
	  exit(1);
     }

     page = mmap(start, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd,
		 (off_t)GPIO0_BASE);
     if (page == MAP_FAILED) {
//	  syslog(LOG_ERR, "Can't mmap GPIO memory space: %s", strerror(errno));
	  exit(1);
     }

     /* disable */
     *REG(page, GPIO0_TCR) &= ~(GPIO_LED | GPIO_BUTTON);
     /* enable led */
     *REG(page, GPIO0_TCR) |= GPIO_LED | GPIO_BUTTON;
     /* enable/disable(?) button */
     *REG(page, GPIO0_TCR) &= ~GPIO_BUTTON;

     *REG(page, GPIO0_IR) & GPIO_BUTTON;
     *REG(page, GPIO0_IR) & GPIO_BUTTON;

}
int getbuttonstate()
{
return (*REG(page, GPIO0_IR) & GPIO_BUTTON)==0;

}


#endif

static int mode = 0;		/* mode 1 : pushed */
static int ses_mode = 0;	/* mode 1 : pushed */
static int count = 0;

static int brand;

static void
alarmtimer (unsigned long sec, unsigned long usec)
{
  struct itimerval itv;

  itv.it_value.tv_sec = sec;
  itv.it_value.tv_usec = usec;

  itv.it_interval = itv.it_value;
  setitimer (ITIMER_REAL, &itv, NULL);
}

int
endswith (char *str, char *cmp)
{
  int cmp_len, str_len, i;
  cmp_len = strlen (cmp);
  str_len = strlen (str);
  if (cmp_len > str_len)
    return (0);
  for (i = 0; i < cmp_len; i++)
    {
      if (str[(str_len - 1) - i] != cmp[(cmp_len - 1) - i])
	return (0);
    }
  return (1);
}

void
runStartup (char *folder, char *extension)
{
  struct dirent *entry;
  DIR *directory;
  unsigned char *buf;
  buf = malloc (1024);
  directory = opendir (folder);
  if (directory == NULL)
    return;
//list all files in this directory 
  while ((entry = readdir (directory)) != NULL)
    {
      if (endswith (entry->d_name, extension))
	{
	  sprintf (buf, "%s/%s&\n", folder, entry->d_name);
	  //execute script     
	  system (buf);
	}
    }
  free (buf);
  closedir (directory);
}

/*
void system_reboot(void)
{
	DEBUG("resetbutton: reboot\n");
	alarmtimer(0, 0);
	eval("reboot");
}
*/

void
service_restart (void)
{
  DEBUG ("resetbutton: restart\n");
  /* Stop the timer alarm */
  alarmtimer (0, 0);
  /* Reset the Diagnostic LED */
  diag_led (DIAG, START_LED);	/* call from service.c */
  /* Restart all of services */
  eval ("rc", "restart");
}

int
check_ses_led_main (int argc, char **argv)
{
  int i;
  int times;
  int interval;

  times = atoi (argv[1]);
  interval = atoi (argv[2]);

  for (i = 0; i < times; i++)
    {

      /* White led */
      diag_led (SES_LED1, START_LED);
      usleep (1000000 * interval);
      diag_led (SES_LED1, STOP_LED);

      /* Orange led */
      diag_led (SES_LED2, START_LED);
      usleep (1000000 * interval);
      diag_led (SES_LED2, STOP_LED);
    }
  return 0;
}

void
period_check (int sig)
{
  FILE *fp;
  unsigned int val = 0;
//      time_t t;

//      time(&t);
//      DEBUG("resetbutton: now time=%d\n", t);

#ifdef HAVE_MAGICBOX
val = getbuttonstate();
#else

  if ((fp = fopen (GPIO_FILE, "r")))
    {
#ifdef HAVE_XSCALE
      fscanf (fp, "%d", &val);
#else
      fread (&val, 4, 1, fp);
#endif
      fclose (fp);
    }
  else
    perror (GPIO_FILE);
#endif
  DEBUG ("resetbutton: GPIO = 0x%x\n", val);

  int gpio = 0;

  int state = 0;
#ifdef HAVE_XSCALE
  state = val;
#elif HAVE_MAGICBOX
  state = val;
#else
  if ((brand & 0x000f) != 0x000f)
    gpio = 1 << (brand & 0x000f);	//calculate gpio pin no.

  if ((brand & 0x0010) == 0)	//check reset button polarity: 0 normal, 1 inversed
    state = (val & gpio);
  else
    state = !(val & gpio);
    
  int push;
  switch (brand)
  	{
  	case ROUTER_BUFFALO_WHRG54S: // ROUTER_BUFFALO_HP_WHRG54S too
		push = 0x01;  //gpio 0
  		break;
  	case ROUTER_ASUS_WL500G_PRE:
	case ROUTER_WRT54G:
  		push = 0x10;  //gpio 4
  		break;
  	default:
  		push = 0x00;  //code unknown = disabled
	}
  
#endif
  /*  The value is zero during button-pushed. */
  if (state)
    {
      DEBUG ("resetbutton: mode=%d, count=%d\n", mode, count);

      if (mode == 0)
	{
	  /* We detect button pushed first time */
	  alarmtimer (0, URGENT_INTERVAL);
	  mode = 1;
	}
      else
	{			/* Whenever it is pushed steady */
	  if (++count > RESET_WAIT_COUNT)
	    {
	      if (check_action () != ACT_IDLE)
		{		// Don't execute during upgrading
		  fprintf (stderr, "resetbutton: nothing to do...\n");
		  alarmtimer (0, 0);	/* Stop the timer alarm */
		  return;
		}
	      if ((brand & 0x000f) != 0x000f)
		{
		  printf ("resetbutton: factory default.\n");
#ifndef HAVE_XSCALE
#ifndef HAVE_MAGICBOX
		  switch (brand)
		    {
		    case ROUTER_BUFFALO_WBR54G:
		    case ROUTER_BUFFALO_WZRRSG54:
		      eval ("gpio", "disable", "7");	//turn on DIAG led on some Buffalos
		      break;
		    case ROUTER_BUFFALO_WBR2G54S:
		      eval ("gpio", "enable", "1");	//turn on DIAG led on WBR2-G54
		      break;
		    case ROUTER_BUFFALO_WLA2G54C:
		      eval ("gpio", "disable", "3");	//turn on red DIAG led on WLA2G54C / WLI3-TX1-G54
		      eval ("gpio", "disable", "4");
		      break;
		    }
#endif
#endif
		  ACTION ("ACT_HW_RESTORE");
		  alarmtimer (0, 0);	/* Stop the timer alarm */
		  nvram_set ("sv_restore_defaults", "1");
		  nvram_commit ();
		  kill (1, SIGTERM);
		}
	    }
	}
    }
#ifndef HAVE_XSCALE
#ifndef HAVE_MAGICBOX

  else if (!(val & push) && push != 0x00)
    {
      runStartup ("/etc/config", ".sesbutton");
      runStartup ("/jffs/etc/config", ".sesbutton");	//if available
      runStartup ("/mmc/etc/config", ".sesbutton");	//if available
      runStartup ("/tmp/etc/config", ".sesbutton");	//if available
      if (ses_mode == 1)
	{
	  //enable orange led
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    eval ("wl", "radio", "on");
#endif
	  if (brand == ROUTER_WRT54G)
	    {
	      eval ("gpio", "enable", "2");
	      eval ("gpio", "disable", "3");
	    }
	  if (brand == ROUTER_WRTSL54GS)
	    {
	      eval ("gpio", "enable", "5");
	      eval ("gpio", "disable", "7");
	    }
	  ses_mode = 2;
	}
      else if (ses_mode == 0)
	{
	  //enable white led
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    eval ("wl", "radio", "on");
#endif
	  if (brand == ROUTER_WRT54G)
	    {
	      eval ("gpio", "enable", "3");
	      eval ("gpio", "disable", "2");
	    }
	  if (brand == ROUTER_WRTSL54GS)
	    {
	      eval ("gpio", "enable", "7");
	      eval ("gpio", "disable", "5");
	    }
	  ses_mode = 1;
	}
      else if (ses_mode == 2)
	{
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    eval ("wl", "radio", "on");
#endif
	  if (brand == ROUTER_WRT54G)
	    {
	      eval ("gpio", "disable", "3");
	      eval ("gpio", "disable", "2");
	    }
	  if (brand == ROUTER_WRTSL54GS)
	    {
	      eval ("gpio", "disable", "5");
	      eval ("gpio", "disable", "7");
	    }
	  ses_mode = 3;
	}
      else if (ses_mode == 3)
	{
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    eval ("wl", "radio", "off");
#endif
	  if (brand == ROUTER_WRT54G)
	    {
	      eval ("gpio", "enable", "3");
	      eval ("gpio", "enable", "2");
	    }
	  if (brand == ROUTER_WRTSL54GS)
	    {
	      eval ("gpio", "enable", "5");
	      eval ("gpio", "enable", "7");
	    }
	  ses_mode = 0;
	}

      /*
         char *led_argv[] = { "check_ses_led",
         SES_LED_CHECK_TIMES,
         SES_LED_CHECK_INTERVAL,
         NULL
         };
         pid_t pid;

         if(!is_exist("/tmp/EnablePushButton"))       
         return;

         ses_mode = 1;
         eval("killall", "check_ses_led");
         _eval(led_argv, NULL, 0, &pid); */

    }
#endif
#endif
  else
    {

      /* Although it's unpushed now, it had ever been pushed */
      if (mode == 1)
	{
	  if (check_action () != ACT_IDLE)
	    {			// Don't execute during upgrading
	      fprintf (stderr, "resetbutton: nothing to do...\n");
	      alarmtimer (0, 0);	/* Stop the timer alarm */
	      return;
	    }
	  service_restart ();
	}
      /*
         if( ses_mode == 1 ){
         cprintf("Release SES push button\n");
         eval("sendudp", "-i", nvram_safe_get("lan_ifname"),
         "-s", nvram_safe_get("lan_ipaddr"),
         "-d", nvram_safe_get("http_client_ip"),
         "-m", nvram_safe_get("lan_hwaddr"),
         "-p", "9999",
         "LED TEST FINISH");
         ses_mode = 0;
         } */
    }
}

int
resetbutton_main (int argc, char *argv[])
{

  brand = getRouterBrand ();

  if ((brand & 0x000f) == 0x000f)

/*  	if ((brand == ROUTER_SIEMENS) || 
  		(brand == ROUTER_BELKIN) || 
  		(brand == ROUTER_RT210W) ||
  		(brand == ROUTER_MOTOROLA) || 
  		(brand == ROUTER_BELKIN_F5D7230) ||
  		(brand == ROUTER_MICROSOFT_MN700) ||
  		(brand == ROUTER_BUFFALO_WLAG54C) ||
  		(brand == ROUTER_BUFFALO_WLA2G54C) ||
  		(brand == ROUTER_ASUS_WL500G_PRE))
*/
    {
      puts ("sorry, your unit does not support resetbutton feature\n");
      nvram_set ("resetbutton_enable", "0");
      return 0;
    }
#ifdef HAVE_MAGICBOX
   init_gpio();
#endif
  /* Run it under background */
  switch (fork ())
    {
    case -1:
      DEBUG ("can't fork\n");
      exit (0);
      break;
    case 0:
      /* child process */
      DEBUG ("fork ok\n");
      (void) setsid ();
      break;
    default:
      /* parent process should just die */
      _exit (0);
    }

  /* set the signal handler */
  signal (SIGALRM, period_check);

  /* set timer */
  alarmtimer (NORMAL_INTERVAL, 0);

  /* Most of time it goes to sleep */
  while (1)
    pause ();

  return 0;
}
