#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utils.h>
#include <wlutils.h>
#include <errno.h>
#if !defined(HAVE_MICRO) || defined(HAVE_ADM5120) || defined(HAVE_WRK54G)

static void watchdog(void)
{
	int brand = getRouterBrand();
	int registered = -1;
	int radiostate = -1;
	int oldstate = -1;
	int counter = 0;
	int fd = open("/dev/misc/watchdog", O_WRONLY);
	if (fd == -1)
	    fd = open("/dev/watchdog", O_WRONLY);

	if (fd == -1) {
		return;
	}
	while (1) {
		write(fd, "\0", 1);
		fsync(fd);

#ifdef HAVE_REGISTER
		if (!nvram_match("flash_active", "1")) {
			if (registered == -1)
				registered = isregistered_real();
			if (!registered)
				isregistered();	//to poll
		}
#endif
		/* 
		 * software wlan led control 
		 */
#ifdef HAVE_MADWIFI
		if (!nvram_match("flash_active", "1")) {
			radiostate = get_radiostate("ath0");
		}
#else
		wl_ioctl(get_wdev(), WLC_GET_RADIO, &radiostate, sizeof(int));
#endif

		if (radiostate != oldstate) {
#ifdef HAVE_MADWIFI
			if (radiostate == 1)
#else
			if ((radiostate & WL_RADIO_SW_DISABLE) == 0)
#endif
				led_control(LED_WLAN, LED_ON);
			else {
				led_control(LED_WLAN, LED_OFF);
#ifndef HAVE_MADWIFI
				/* 
				 * Disable wireless will cause diag led blink, so we want to
				 * stop it. 
				 */
				if (brand == ROUTER_WRT54G)
					diag_led(DIAG, STOP_LED);
				/* 
				 * Disable wireless will cause power led off, so we want to
				 * turn it on. 
				 */
				if (brand == ROUTER_WRT54G_V8)
					led_control(LED_POWER, LED_ON);
#endif
			}

			oldstate = radiostate;
		}
		/* 
		 * end software wlan led control 
		 */

		sleep(10);
		if (nvram_match("warn_enabled", "1")) {
			counter++;
			if (!(counter % 60))
				system("notifier&");	// 
		}
	}
}

int watchdog_main(int argc, char *argv[])
{

	/* 
	 * Run it under background 
	 */
	switch (fork()) {
	case -1:
		perror("fork failed");
		exit(1);
		break;
	case 0:
		/* 
		 * child process 
		 */
		watchdog();
		exit(0);
		break;
	default:
		/* 
		 * parent process should just die 
		 */
		_exit(0);
	}
	return 0;
}
#endif
