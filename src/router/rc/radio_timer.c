/* Radio timer by Eko: 19.jul.2006

   hours are represented as bits in 24 bit = xxxxxxxxxxxxxxxxxxxxxxxx from GUI
   code scans for changes: 10 = radio off, 01 = radio on
   firsttime change: 00 and 10 = radio off, 11 and 01 = radio on
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcmnvram.h>
#include <cy_conf.h>
#include <rc.h>
#include <shutils.h>
#include <syslog.h>
#include <utils.h>

int
radio_timer_main (void)
{

  long radiotime;		//4 byte int number (24 bits from gui + 1 bit for midnight)
  int firsttime, needchange;
  int gentime;			//general-time holds year or hour or minute
  char *end_ptr;

  needchange = 1;
  firsttime = 1;

  struct tm *currtime;
  long tloc;


  do
    {
      time (&tloc);		// get time in seconds since epoch
      currtime = localtime (&tloc);	// convert seconds to date structure

      gentime = currtime->tm_year;	//gentime = year

#ifdef HAVE_MADWIFI
      if ((gentime > 100) && nvram_invmatch ("ath0_net_mode", "disabled"))	//ntp time must be set  && radio must be on
#elif HAVE_MSSID
      if ((gentime > 100) && nvram_invmatch ("wl0_net_mode", "disabled"))
#else
      if ((gentime > 100) && nvram_invmatch ("wl_net_mode", "disabled"))
#endif
	{

	  gentime = currtime->tm_hour;	//gentime = hour

	  radiotime = strtol (nvram_get ("radio0_on_time"), &end_ptr, 2);	//convert binary string to long int
	  radiotime += ((radiotime & 1) << 24);	//duplicate 23-24h bit to the start to take care of midnight
	  radiotime = (radiotime >> (24 - gentime - 1)) & 3;	//get pattern only (last two bits)

	  gentime = currtime->tm_min;	//gentime = min

	  if (gentime != 0)
	    needchange = 1;	//prevet to be executed more than once when min == 0

	  if (firsttime)	//first time change
	    {
	      switch (radiotime)
		{
		case 3:	//11
		  radiotime = 1;	//01
		  break;
		case 0:	//00
		  radiotime = 2;	//10
		  break;
		}
	    }

	  if (((gentime == 0) && (needchange)) || (firsttime))	//change when min = 0 or firstime
	    {
	      switch (radiotime)
		{
		case 1:	//01 - turn radio on
		  syslog (LOG_DEBUG, "Turning radio on\n");
    	  led_control (LED_WLAN, LED_ON);
#ifdef HAVE_MADWIFI
		  eval ("ifconfig", "ath0", "up");
#elif HAVE_MSSID
		  eval ("wl", "radio", "on");
#else
		  eval ("wl", "radio", "on");
#endif
		  break;

		case 2:	//10 - turn radio off

		  syslog (LOG_DEBUG, "Turning radio off\n");
		  led_control (LED_WLAN, LED_OFF);
#ifdef HAVE_MADWIFI
		  eval ("ifconfig", "ath0", "down");
#elif HAVE_MSSID
		  eval ("wl", "radio", "off");
			if (check_hw_type () == BCM4712_CHIP) /* Disable wireless will cause diag led blink, so we want to stop it. */
				diag_led (DIAG, STOP_LED);
#else
		  eval ("wl", "radio", "off");
			if (check_hw_type () == BCM4712_CHIP) /* Disable wireless will cause diag led blink, so we want to stop it. */
				diag_led (DIAG, STOP_LED);
#endif
		  break;
		}
	      needchange = 0;
	      firsttime = 0;
	    }

	}
      else			//if yr < 100 (=2000) wait 5 min and try again (if ntp time is maybe set now)
	{
	  sleep (242);
	}

      sleep (58);		// loop every 58 s to be sure to catch min == 0

    }
  while (1);

  return 0;

}
