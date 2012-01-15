/*
 *   Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */
#include <net/if.h>
//#include <linux/if_ether.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include <avl.h>
#include <list.h>
#include <linux/nl80211.h>
#include <unl.h>

#define DEFAULT_POLL_DELAY	1000
#define IFACE_DELAY		1000

#define LED_PREFIX "/sys/class/leds/"
#define LED_SUFFIX "/brightness"

static struct unl unl;
static LIST_HEAD(interfaces);
static LIST_HEAD(wdev);
static LIST_HEAD(leds);

static int poll_delay = DEFAULT_POLL_DELAY;
static int cur_signal_max, last_signal_max;
static bool _break = false;

struct interface {
	struct list_head list;
	bool partial;
	char name[];
};

struct led {
	struct list_head list;
	int signal_min;
	int last_val;
	char name[];
};

static bool match_ifname(const char *ifname)
{
	struct interface *iface;

	if (list_empty(&interfaces))
		return true;

	list_for_each_entry(iface, &interfaces, list) {
		if (!strncmp(ifname, iface->name, strlen(iface->name) +
				 !iface->partial))
			return true;
	}
	return false;
}


static void add_interface(const char *ifname)
{
	struct interface *iface;
	char *c;

	iface = calloc(1, sizeof(*iface) + strlen(ifname) + 1);
	strcpy(iface->name, ifname);

	c = strchr(iface->name, '*');
	if (c) {
		iface->partial = true;
		*c = 0;
	}
	list_add_tail(&iface->list, &interfaces);
}

static bool add_led(const char *name)
{
	struct led *led;
	const char *c;
	char *err, *led_name;
	int min, len;
	struct stat st;

	c = strrchr(name, ':');
	if (!c)
		return false;

	len = c - name;
	min = strtol(c + 1, &err, 0);
	if (err && *err)
		return false;

	led_name = alloca(len + 1);
	memcpy(led_name, name, len);
	led_name[len] = 0;

	led = calloc(1, sizeof(*led) + sizeof(LED_PREFIX) + len + sizeof(LED_SUFFIX) + 1);
	sprintf(led->name, LED_PREFIX "%s" LED_SUFFIX, led_name);
	if (stat(led->name, &st) < 0) {
		fprintf(stderr, "Could not find system LED %s\n", led_name);
		free(led);
		return false;
	}

	led->signal_min = min;
	led->last_val = -1;
	list_add_tail(&led->list, &leds);

	return true;
}

struct wdev_list {
	int count;
	int wdev[];
};

static int interface_cb(struct nl_msg *msg, void *arg)
{
	struct wdev_list **plist = arg;
	struct wdev_list *list = *plist;
	static struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		  genlmsg_attrlen(gnlh, 0), NULL);

	if (!tb[NL80211_ATTR_IFNAME] || !tb[NL80211_ATTR_IFINDEX])
		goto out;

	if (!match_ifname(nla_data(tb[NL80211_ATTR_IFNAME])))
		goto out;

	if (list->count++ > 0)
		list = realloc(list, sizeof(*list) + list->count * sizeof(int));

	list->wdev[list->count - 1] = nla_get_u32(tb[NL80211_ATTR_IFINDEX]);
	*plist = list;

out:
	return NL_SKIP;
}

static int station_cb(struct nl_msg *msg, void *arg)
{
	static struct nlattr *tb[NL80211_ATTR_MAX + 1];
	static struct nlattr *tb_sta[NL80211_STA_INFO_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *cur;
	int8_t signal;

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		  genlmsg_attrlen(gnlh, 0), NULL);

	cur = tb[NL80211_ATTR_STA_INFO];
	if (!cur)
		goto out;

	nla_parse(tb_sta, NL80211_STA_INFO_MAX, nla_data(cur), nla_len(cur), NULL);

	cur = tb_sta[NL80211_STA_INFO_SIGNAL_AVG];
	if (!cur)
		goto out;

	signal = nla_get_u8(cur);
	if (cur_signal_max < signal)
		cur_signal_max = signal;

out:
	return NL_SKIP;
}

static void led_set_val(struct led *led, bool val)
{
	char *v = val ? "1" : "0";
	int fd;

	fd = open(led->name, O_WRONLY);
	if (fd < 0)
		return;

	write(fd, v, 1);
	close(fd);
}

static void process_leds(void)
{
	struct led *led;
	bool val;

	list_for_each_entry(led, &leds, list) {
		val = (led->signal_min <= cur_signal_max);
		if (val == led->last_val)
			continue;

		led->last_val = val;
		led_set_val(led, val);
	}
}

static void run_loop(void)
{
	struct nl_msg *msg;
	struct wdev_list *list;
	int i;

	cur_signal_max = -127;
	list = calloc(1, sizeof(*list) + sizeof(int));
	while (!_break) {
		last_signal_max = cur_signal_max;
		cur_signal_max = -127;

		list->count = 0;
		msg = unl_genl_msg(&unl, NL80211_CMD_GET_INTERFACE, true);
		unl_genl_request(&unl, msg, interface_cb, &list);

		for (i = 0; i < list->count; i++) {
			msg = unl_genl_msg(&unl, NL80211_CMD_GET_STATION, true);
			NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, list->wdev[i]);
			unl_genl_request(&unl, msg, station_cb, NULL);
		}

		process_leds();
		usleep(poll_delay * 1000);
		continue;

nla_put_failure:
		nlmsg_free(msg);
	}
	free(list);
}

static int usage(const char *progname)
{
	fprintf(stderr, "Usage: %s [options]\n"
		"\n"
		"Options:\n"
		"  -i <pattern>			Pattern for interfaces (if not specified, use all interfaces)\n"
		"  -l <name>:<thresh>		Add a led with name <name> and minimum signal strength <thresh>\n"
		"  -d <delay>			Set polling delay (default: %d)\n"
		"\n", progname, DEFAULT_POLL_DELAY);
	return 1;
}

static void handle_sigint(int signo)
{
	_break = true;
}

static void setup_sigint(void)
{
	struct sigaction s;

	memset(&s, 0, sizeof(struct sigaction));
	s.sa_handler = handle_sigint;
	s.sa_flags = 0;
	sigaction(SIGINT, &s, NULL);
}

int main(int argc, char **argv)
{
	int ch;

	while ((ch = getopt(argc, argv, "i:l:")) != -1) {
		switch(ch) {
		case 'i':
			add_interface(optarg);
			break;
		case 'l':
			if (!add_led(optarg))
				return usage(argv[0]);
			break;
		case 'd':
			poll_delay = atoi(optarg);
			break;
		}
	}

	if (list_empty(&leds)) {
		fprintf(stderr, "Error: no LEDs specified\n");
		return usage(argv[0]);
	}

	if (unl_genl_init(&unl, "nl80211") < 0) {
		fprintf(stderr, "Failed to connect to nl80211\n");
		return 1;
	}

	setup_sigint();
	run_loop();
	unl_free(&unl);

	return 0;
}
