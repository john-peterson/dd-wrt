#include <stdio.h>
#include <stdlib.h>
#include <shutils.h>
#include <bcmnvram.h>

static int scanFor(int Vendor, int Product)
{
#ifdef ARCH_broadcom
	char grepstr[128];
	sprintf(grepstr, "grep Vendor=%x ProdID=%x /tmp/usb/devices|wc -l",
		Vendor, Product);
	FILE *check = popen(grepstr, "rb");
	if (check) {
		int count = 0;
		fscanf(check, "%d", &count);
		fclose(check);
		if (count > 0) {
			eval("umount /tmp/usb");
			return 1;
		}
	}
	return 0;
#else
	int count = 1;
	while (1) {
		char sysfs[64];
		sprintf(sysfs, "/sys/bus/usb/devices/%d-0:1.0/bInterfaceNumber",
			count);
		FILE *probe = fopen(sysfs, "rb");
		if (!probe)
			return 0;
		fclose(probe);
		sprintf(sysfs, "/sys/bus/usb/devices/%d-1/idProduct", count);
		FILE *modem = fopen(sysfs, "rb");
		if (!modem) {
			count++;
			continue;
		}
		int idProduct;
		int idVendor;
		fscanf(modem, "%X", &idProduct);
		fclose(modem);
		sprintf(sysfs, "/sys/bus/usb/devices/%d-1/idVendor", count);
		modem = fopen(sysfs, "rb");
		if (!modem) {
			count++;
			continue;
		}
		fscanf(modem, "%X", &idVendor);
		fclose(modem);
		if (idVendor == Vendor && idProduct == Product)
			return 1;
		count++;
	}
#endif
}

void checkreset(char *tty)
{
#ifdef HAVE_CAMBRIA
	eval("comgt", "-d", tty, "-s", "/etc/comgt/reset.comgt");
	FILE *check = NULL;
	int count = 0;
	sleep(1);
	while (!(check = fopen(tty, "rb")) && count < 10) {
		sleep(1);
		count++;
	}
	if (check)
		fclose(check);
	else
		fprintf(stderr, "reset error\n");
	fprintf(stderr, "wakeup card\n");
	eval("comgt", "-d", tty, "-s", "/etc/comgt/wakeup.comgt");
	sleep(5);		//give extra delay for registering
#endif
}

char *get3GControlDevice(void)
{
#ifdef ARCH_broadcom
	mkdir("/tmp/usb");
	eval("mount", "-t", "usbfs", "usb", "/tmp/usb");
//insmod("sierra");  //further investigation required (compass problem)
#endif

	char *ttsdevice = "/dev/usb/tts/0";
#ifdef HAVE_CAMBRIA
	eval("gpio", "enable", "26");
	eval("gpio", "disable", "27");
	if (nvram_match("wan_select", "1")) {
		eval("gpio", "enable", "26");
		eval("gpio", "disable", "27");
	}
	if (nvram_match("wan_select", "2")) {
		eval("gpio", "disable", "26");
		eval("gpio", "enable", "27");
	}
	if (nvram_match("wan_select", "3")) {
		eval("gpio", "enable", "26");
		eval("gpio", "enable", "27");
	}
#endif
	nvram_set("3gdata", "/dev/usb/tts/0");
	if (scanFor(0x1199, 0x6880)) {
		//sierra wireless 
		fprintf(stderr, "Sierra Wireless Compass 885 deteted\n");
		insmod("usbserial");
		insmod("sierra");
		nvram_set("3gdata", "/dev/usb/tts/4");
		return "/dev/usb/tts/3";
	}
	if (scanFor(0x1199, 0x6890)) {
		//sierra wireless 
		fprintf(stderr, "Sierra Wireless Compass 888 deteted\n");
		insmod("usbserial");
		insmod("sierra");
		nvram_set("3gdata", "/dev/usb/tts/4");
		return "/dev/usb/tts/3";
	}
	if (scanFor(0x1199, 0x683C)) {
		//sierra wireless 
		fprintf(stderr, "Sierra Wireless MC8790\n");
		nvram_set("3gdata", "/dev/usb/tts/4");
		insmod("usbserial");
		insmod("sierra");
		checkreset("/dev/usb/tts/3");
		return "/dev/usb/tts/3";
	}
	if (scanFor(0x1199, 0x683D)) {
		//sierra wireless 
		fprintf(stderr, "Sierra Wireless MC8790\n");
		nvram_set("3gdata", "/dev/usb/tts/4");
		insmod("usbserial");
		insmod("sierra");
		checkreset("/dev/usb/tts/3");
		return "/dev/usb/tts/3";
	}
	if (scanFor(0x1199, 0x683E)) {
		//sierra wireless 
		fprintf(stderr, "Sierra Wireless MC8790\n");
		nvram_set("3gdata", "/dev/usb/tts/4");
		insmod("usbserial");
		insmod("sierra");
		checkreset("/dev/usb/tts/3");
		return "/dev/usb/tts/3";
	}
	if (scanFor(0x12d1, 0x1003)) {
		//huawei
		fprintf(stderr, "HUAWEI/Option E172 detected\n");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/0";
	}
	if (scanFor(0x0af0, 0x7011)) {
		//huawei
		fprintf(stderr, "HUAWEI/Option E301 HSUPA detected\n");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/0";
	}
	if (scanFor(0x12d1, 0x1001)) {
		//huawei
		fprintf(stderr, "HUAWEI/Option E600 detected\n");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/0";
	}
	if (scanFor(0x12d1, 0x1003)) {
		//huawei
		fprintf(stderr, "HUAWEI/Option EC270 detected\n");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/0";
	}
	if (scanFor(0x12d1, 0x1412)) {
		//huawei
		fprintf(stderr, "HUAWEI/Option EC168 detected\n");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/0";
	}
	if (scanFor(0x12d1, 0x1412)) {
		//huawei
		fprintf(stderr, "HUAWEI/Option EC168 detected\n");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/0";
	}

	if (scanFor(0x1e0e, 0x9000)) {
		//huawei
		fprintf(stderr, "QUALCOMM ICON 210 detected\n");
		nvram_set("3gdata", "/dev/usb/tts/2");
		insmod("usbserial");
		insmod("option");
		return "/dev/usb/tts/2";
	}

	if (scanFor(0x1e0e, 0xf000)) {
		//huawei
		fprintf(stderr, "QUALCOMM ICON 210 detected\n");
		FILE *out = fopen("/tmp/usb_modeswitch.conf", "wb");
		fprintf(out, "DefaultVendor=0x1e0e\n");
		fprintf(out, "DefaultProduct=0xf000\n");
		fprintf(out, "TargetVendor=0x1e0e\n");
		fprintf(out, "TargetProduct=0x9000\n");
		fprintf(out,
			"MessageContent=\"555342431234567800000000000006bd000000020000000000000000000000\"\n");
		fprintf(out, "ResponseEndpoint=0x01\n");
		fclose(out);
		system("usb_modeswitch -c /tmp/usb_modeswitch.conf");
		sleep(2);
		insmod("usbserial");
		insmod("option");
		nvram_set("3gdata", "/dev/usb/tts/2");
		return "/dev/usb/tts/2";
	}

	if (scanFor(0x0af0, 0x6971)) {
		//huawei
		fprintf(stderr, "QUALCOMM ICON 225 detected\n");
		system("ozerocdoff -wi 0x6971");
		sleep(10);
		system("insmod hso");
		FILE *out = fopen("/tmp/conninfo.ini", "wb");
		fprintf(out, "APN=%s\n", nvram_safe_get("wan_apn"));
		fprintf(out, "USER=%s\n", nvram_safe_get("ppp_username"));
		fprintf(out, "PASS=%s\n", nvram_safe_get("ppp_passwd"));
		fprintf(out, "PIN=%s\n", nvram_safe_get("wan_pin"));
		fclose(out);
		nvram_set("3gdata", "hso");
		system("/etc/hso/hso_connect.sh restart");
		return "hso";
	}

	if (scanFor(0x1199, 0x6832)) {
		//sierra wireless mc 8780
		fprintf(stderr,
			"Sierra Wireless MC 8780 detected\nreset card\n");
		checkreset("/dev/usb/tts/2");
		return "/dev/usb/tts/2";
	}
	insmod("usbserial");
	insmod("sierra");
	insmod("option");
	return ttsdevice;
}
