/*
 * idevicedate.c
 * Simple utility to get and set the clock on a device
 *
 * Copyright (c) 2011 Martin Szulecki All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>

#ifdef _DATE_FMT
# define DATE_FMT_LANGINFO() nl_langinfo (_DATE_FMT)
#else
# define DATE_FMT_LANGINFO() ""
#endif

static void print_usage(int argc, char **argv)
{
	char *name = NULL;
	
	name = strrchr(argv[0], '/');
	printf("Usage: %s [OPTIONS]\n", (name ? name + 1: argv[0]));
	printf("Display the current date or set it on a device.\n\n");
	printf("  -d, --debug\t\tenable communication debugging\n");
	printf("  -u, --udid UDID\ttarget specific device by its 40-digit device UDID\n");
	printf("  -s, --set TIMESTAMP\tset UTC time described by TIMESTAMP\n");
	printf("  -c, --sync\t\tset time of device to current system time\n");
	printf("  -h, --help\t\tprints usage information\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	lockdownd_client_t client = NULL;
	idevice_t phone = NULL;
	idevice_error_t ret = IDEVICE_E_UNKNOWN_ERROR;
	int i;
	char udid[41];
	time_t setdate = 0;
	plist_t node = NULL;
	udid[0] = 0;
	uint64_t datetime = 0;
	time_t rawtime;
	struct tm * tmp;
	char const *format = NULL;
	char buffer[80];

	/* parse cmdline args */
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
			idevice_set_debug_level(1);
			continue;
		}
		else if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--udid")) {
			i++;
			if (!argv[i] || (strlen(argv[i]) != 40)) {
				print_usage(argc, argv);
				return 0;
			}
			strcpy(udid, argv[i]);
			continue;
		}
		else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--set")) {
			i++;
			if (!argv[i] || (strlen(argv[i]) <= 1)) {
				print_usage(argc, argv);
				return 0;
			}
			setdate = atoi(argv[i]);
			if (setdate == 0) {
				printf("ERROR: Invalid timestamp value.\n");
				print_usage(argc, argv);
				return 0;
			}
			continue;
		}
		else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--sync")) {
			i++;
			/* get current time */
			setdate = time(NULL);
			/* convert it to local time which sets timezone/daylight variables */
			tmp = localtime(&setdate);
			/* recalculate to make it UTC */
			setdate = mktime(tmp);
			continue;
		}
		else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			print_usage(argc, argv);
			return 0;
		}
		else {
			print_usage(argc, argv);
			return 0;
		}
	}

	/* determine a date format */
	if (!format) {
		format = DATE_FMT_LANGINFO ();
		if (!*format) {
			format = "%a %b %e %H:%M:%S %Z %Y";
		}
	}

	if (udid[0] != 0) {
		ret = idevice_new(&phone, udid);
		if (ret != IDEVICE_E_SUCCESS) {
			printf("No device found with udid %s, is it plugged in?\n", udid);
			return -1;
		}
	}
	else
	{
		ret = idevice_new(&phone, NULL);
		if (ret != IDEVICE_E_SUCCESS) {
			printf("No device found, is it plugged in?\n");
			return -1;
		}
	}

	if (LOCKDOWN_E_SUCCESS != lockdownd_client_new_with_handshake(phone, &client, "idevicedate")) {
		idevice_free(phone);
		return -1;
	}

	/* get or set? */
	if (setdate == 0) {
		/* get time value from device */
		if(lockdownd_get_value(client, NULL, "TimeIntervalSince1970", &node) == LOCKDOWN_E_SUCCESS) {
			if (node) {
				plist_get_uint_val(node, &datetime);
				plist_free(node);
				node = NULL;

				/* date/time calculations */
				rawtime = (time_t)datetime;
				tmp = localtime(&rawtime);

				/* finally we format and print the current date */
				strftime(buffer, 80, format, tmp);
				puts(buffer);
			}
		}
	} else {
		datetime = setdate;

		if(lockdownd_set_value(client, NULL, "TimeIntervalSince1970", plist_new_uint(datetime)) == LOCKDOWN_E_SUCCESS) {
			tmp = localtime(&setdate);
			strftime(buffer, 80, format, tmp);
			puts(buffer);
		} else {
			printf("ERROR: Failed to set date on device.\n");
		}
	}

	lockdownd_client_free(client);
	idevice_free(phone);

	return 0;
}

