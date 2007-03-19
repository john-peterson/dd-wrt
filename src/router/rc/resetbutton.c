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
#include <syslog.h>



#define	SES_LED_CHECK_TIMES	"9999"	/* How many times to check? */
#define	SES_LED_CHECK_INTERVAL	"1"	/* Wait interval seconds */
#define RESET_WAIT		3	/* seconds */
#define RESET_WAIT_COUNT	RESET_WAIT * 10	/* 10 times a second */

#define NORMAL_INTERVAL		1	/* second */
#define URGENT_INTERVAL		100 * 1000	/* microsecond */

#ifndef HAVE_GATEWORX		/* 1/10 second */
#define GPIO_FILE		"/dev/gpio/in"
#endif
#if 0
#define DEBUG printf
#else
#define DEBUG(format, args...)
#endif



#ifdef HAVE_MAGICBOX
#include <sys/mman.h>

#define GPIO0_OR   0x0700	/* rw, output */
#define GPIO0_TCR  0x0704	/* rw, three-state control */
#define GPIO0_ODR  0x0718	/* rw, open drain */
#define GPIO0_IR   0x071c	/* ro, input */
#define GPIO0_BASE 0xef600000	/* page */

#define GPIO_LED    0x20000000	/* GPIO1 */
#define GPIO_BUTTON 0x40000000	/* GPIO2 */

#define REG(buf, offset) ((unsigned int *)((void *)buf + offset))

static unsigned int *page;
static int fd;

void
init_gpio ()
{
  void *start = 0;

  fd = open ("/dev/mem", O_RDWR);
  if (fd < 0)
    {
//        syslog(LOG_ERR, "Can't open /dev/mem: %s", strerror(errno));
      exit (1);
    }

  page = mmap (start, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
	       (off_t) GPIO0_BASE);
  if (page == MAP_FAILED)
    {
//        syslog(LOG_ERR, "Can't mmap GPIO memory space: %s", strerror(errno));
      exit (1);
    }

  /* disable */
  *REG (page, GPIO0_TCR) &= ~(GPIO_LED | GPIO_BUTTON);
  /* enable led */
  *REG (page, GPIO0_TCR) |= GPIO_LED | GPIO_BUTTON;
  /* enable/disable(?) button */
  *REG (page, GPIO0_TCR) &= ~GPIO_BUTTON;

  *REG (page, GPIO0_IR) & GPIO_BUTTON;
  *REG (page, GPIO0_IR) & GPIO_BUTTON;

}

int
getbuttonstate ()
{
  return (*REG (page, GPIO0_IR) & GPIO_BUTTON) == 0;

}
#endif

#if defined(HAVE_FONERA) || defined(HAVE_WHRAG108)
int
getbuttonstate ()
{
  FILE *in;
  int ret;
  in = fopen ("/proc/gpio/6_in", "rb");
  if (in == NULL)
    return 0;
  fscanf (in, "%d", &ret);
  fclose (in);
  return ret;
}
#endif
#if defined(HAVE_GATEWORX)

#define u8 unsigned char
#define u32 unsigned long

//#include <linux/ixp425-gpio.h>

#include <include/asm/hardware.h>
#include <include/asm-arm/arch-ixp4xx/ixp4xx-regs.h>

#define IXP4XX_GPIO_OUT 		0x1
#define IXP4XX_GPIO_IN  		0x2

struct gpio_bit
{
  unsigned char bit;
  unsigned char state;
};



char *filename = "/dev/gpio";


int
read_bit (int bit)
{
  int file;
  struct gpio_bit _bit;

  /* open device */
  if ((file = open (filename, O_RDONLY)) == -1)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return 1;
    }

  /* Config pin as input */
  _bit.bit = bit;
  _bit.state = IXP4XX_GPIO_IN;
  if (ioctl (file, GPIO_SET_CONFIG, (long) &_bit) < 0)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return 1;
    }

  /* Read data */
  _bit.bit = bit;
  if (ioctl (file, GPIO_GET_BIT, (long) &_bit) < 0)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return 1;
    }

  close (file);
  return _bit.state;
}

int
getbuttonstate ()
{
  FILE *in;
  int ret = read_bit (4);
  return ret == 0 ? 1 : 0;
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
  unsigned char buf[128];
  directory = opendir (folder);
  if (directory == NULL)
    {
      return;
    }
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
      //diag_led (SES_LED1, START_LED);
      led_control (LED_SES, LED_ON);
      usleep (1000000 * interval);
      //diag_led (SES_LED1, STOP_LED);
      led_control (LED_SES, LED_OFF);

      /* Orange led */
      //diag_led (SES_LED2, START_LED);
      led_control (LED_SES2, LED_ON);
      usleep (1000000 * interval);
      //diag_led (SES_LED2, STOP_LED);
      led_control (LED_SES2, LED_OFF);
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

#if defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_WHRAG108) || defined(HAVE_GATEWORX)
  val = getbuttonstate ();
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
#if defined(HAVE_XSCALE) || defined(HAVE_MAGICBOX) || defined(HAVE_FONERA) || defined(HAVE_WHRAG108) || defined(HAVE_GATEWORX)
  state = val;
#else
  if ((brand & 0x000f) != 0x000f)
    gpio = 1 << (brand & 0x000f);	//calculate gpio value.

  if ((brand & 0x0010) == 0)	//check reset button polarity: 0 normal, 1 inversed
    state = (val & gpio);
  else
    state = !(val & gpio);

/* 1 byte router's SES or AOSS button gpio number and polarity; Eko 25.nov.06

  R R R P N N N N   = 0xXX
  ----- - -------
    |   | gpio num (1111 = f = disable SES - AOSS button)
    |   |
    |   |--- SES - AOSS button polarity (0: normal, 1 inversed)
    |
    |-------- reserved for future use 
*/
  int push;
  int sesgpio;
  switch (brand)
    {
    case ROUTER_BUFFALO_WHRG54S:
    case ROUTER_BUFFALO_WZRRSG54:
    case ROUTER_BUFFALO_WLI_TX4_G54HP:
      sesgpio = 0x10;		//gpio 0, inversed
      break;
    case ROUTER_WRT54G:
      sesgpio = 0x14;		//gpio 4, inversed
      break;
    case ROUTER_BUFFALO_WLA2G54C:
      sesgpio = 0x12;		//gpio 2, inversed
      break;
    case ROUTER_BUFFALO_WBR2G54S:
    case ROUTER_ASUS_WL500G_PRE:
      sesgpio = 0x04;		//gpio 4, normal
      break;
    default:
      sesgpio = 0x0f;		//gpio unknown, disabled
    }

  push = 1 << (sesgpio & 0x0f);	//calculate push value from ses gpio pin no.

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
		  syslog (LOG_DEBUG,
			  "Reset button: restoring factory defaults now!\n");
#if !defined(HAVE_XSCALE) && !defined(HAVE_MAGICBOX) && !defined(HAVE_FONERA) && !defined(HAVE_WHRAG108) && !defined(HAVE_GATEWORX)
		  led_control (LED_DIAG, LED_ON);
#endif
		  ACTION ("ACT_HW_RESTORE");
		  alarmtimer (0, 0);	/* Stop the timer alarm */
#ifdef HAVE_X86
		  eval ("mount", "/usr/local", "-o", "remount,rw");
		  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
		  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
		  eval ("rm", "-f", "/usr/local/nvram/*");	// delete nvram database
		  eval ("mount", "/usr/local", "-o", "remount,ro");
#elif HAVE_RB500
		  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
		  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
		  eval ("rm", "-f", "/etc/nvram/*");	// delete nvram database
#elif HAVE_MAGICBOX
		  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
		  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
		  eval ("erase", "nvram");
#elif HAVE_FONERA
		  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
		  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
		  eval ("erase", "nvram");
#elif HAVE_WHRAG108
		  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
		  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
		  eval ("erase", "nvram");
#elif HAVE_XSCALE
		  eval ("rm", "-f", "/tmp/nvram/*");	// delete nvram database
		  eval ("rm", "-f", "/tmp/nvram/.lock");	// delete nvram database
		  eval ("erase", "nvram");
#else
          nvram_set ("sv_restore_defaults", "1");
          nvram_commit ();
		  eval ("erase", "nvram");
#endif


//                nvram_set ("sv_restore_defaults", "1");
//                nvram_commit ();



		  kill (1, SIGTERM);
		}
	    }
	}
    }
#if !defined(HAVE_XSCALE) && !defined(HAVE_MAGICBOX) && !defined(HAVE_FONERA) && !defined(HAVE_WHRAG108) && !defined(HAVE_GATEWORX)

  else if ((sesgpio != 0x0f)
	   && (((sesgpio & 0x10) == 0 && (val & push))
	       || ((sesgpio & 0x10) == 0x10 && !(val & push))))
    {
      runStartup ("/etc/config", ".sesbutton");
      runStartup ("/jffs/etc/config", ".sesbutton");	//if available
      runStartup ("/mmc/etc/config", ".sesbutton");	//if available
      runStartup ("/tmp/etc/config", ".sesbutton");	//if available

      if (ses_mode == 1)
	{
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    {
	      eval ("wl", "radio", "on");
	      syslog (LOG_DEBUG,
		      "SES /AOSS /EZ-setup button: turning radio on\n");
	    }
#endif

//              led_control (LED_SES, LED_OFF);         
//              led_control (LED_SES2, LED_ON);         //enable orange led
	  led_control (LED_AOSS, LED_FLASH);	//blink AOSS led
	  led_control (LED_SES, LED_FLASH);	//when pressed, blink white SES led

//      switch (brand)
//              {
//              case ROUTER_WRT54G:
//              case ROUTER_WRTSL54GS: 
//                      ses_mode = 2;
//              break;
//              default:
	  ses_mode = 0;
//              }

	}
      else if (ses_mode == 0)
	{
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    {
	      eval ("wl", "radio", "off");
	      syslog (LOG_DEBUG,
		      "SES /AOSS /EZ-setup button: turning radio off\n");
	    }
#endif
//              led_control (LED_SES, LED_ON);          //enable white led
//              led_control (LED_SES2, LED_OFF);
	  led_control (LED_AOSS, LED_FLASH);	//blink AOSS led
	  led_control (LED_SES, LED_FLASH);	//when pressed, blink white SES led

	  ses_mode = 1;

	}
/*      else if (ses_mode == 2)
	{
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    eval ("wl", "radio", "on");
#endif

		led_control (LED_SES, LED_ON);		//both leds on
		led_control (LED_SES2, LED_ON);

		ses_mode = 3;
		
	}
      else if (ses_mode == 3)
	{
#ifdef HAVE_RADIOOFF
	  if (nvram_match ("radiooff_button", "1"))
	    eval ("wl", "radio", "off");
#endif
		led_control (LED_SES, LED_OFF);		//both leds off
		led_control (LED_SES2, LED_OFF);

		ses_mode = 0;
		
	}
*/

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

    {
      puts ("sorry, your unit does not support resetbutton feature\n");
      return 0;
    }
#ifdef HAVE_MAGICBOX
  init_gpio ();
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
