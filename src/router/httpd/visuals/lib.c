#define VISUALSOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <net/if_arp.h>
#include <stdarg.h>

#include <broadcom.h>
#include <cyutils.h>
#include <code_pattern.h>
#include <cy_conf.h>

void ej_compile_date(webs_t wp, int argc, char_t ** argv)
{
	char year[8], mon[4], day[4];
	char string[20];

	sscanf(__DATE__, "%s %s %s", mon, day, year);
	snprintf(string, sizeof(string), "%s. %s, %s", mon, day, year);

	websWrite(wp, "%s", string);
}

void ej_compile_time(webs_t wp, int argc, char_t ** argv)
{
	websWrite(wp, "%s", __TIME__);
}

#ifndef HAVE_SPECIALEDITION
#include <revision.h>

void ej_get_firmware_version(webs_t wp, int argc, char_t ** argv)
{
#ifdef HAVE_BUFFALO
	websWrite(wp, "%s%s %s%s", CYBERTAN_VERSION, MINOR_VERSION,
		  nvram_safe_get("dist_type"), DIST_OPT);
#else

#ifdef HAVE_REGISTER
	if (isregistered() && !isregistered_real()) {
		websWrite(wp, "Click here to ACTIVATE %d Hour Trial",
			  getTrialCount());
	} else {
#ifdef HAVE_WIKINGS
#ifdef HAVE_SUB3
#define V "ExcelMed"
#elif HAVE_SUB6
#define V "ExcelMin"
#else
#define V "Excellent"
#endif
		websWrite(wp, "Excel Networks (%s series) V 2.10", V);
#undef V
#elif HAVE_CARLSONWIRELESS
		websWrite(wp, "Carlson Wireless v5.3 (%s)", SVN_REVISION);
#else

		websWrite(wp, "%s%s %s%s", CYBERTAN_VERSION, MINOR_VERSION,
			  nvram_safe_get("dist_type"), DIST_OPT);
#endif
	}

#else
#ifdef HAVE_WIKINGS
#ifdef HAVE_SUB3
#define V "ExcelMed"
#elif HAVE_SUB6
#define V "ExcelMin"
#else
#define V "Excellent"
#endif
	websWrite(wp, "Excel Networks (%s series) V 2.10", V);
#undef V
#elif HAVE_CARLSONWIRELESS
	websWrite(wp, "Carlson Wireless v1.0 (%s)", SVN_REVISION);
#else

	websWrite(wp, "%s%s %s%s", CYBERTAN_VERSION, MINOR_VERSION,
		  nvram_safe_get("dist_type"), DIST_OPT);
#endif
#endif
#endif
}
#endif

void ej_get_firmware_title(webs_t wp, int argc, char_t ** argv)
{
	websWrite(wp, "Wireless-G Broadband Router");
}

#include <revision.h>
void ej_get_firmware_svnrev(webs_t wp, int argc, char_t ** argv)
{
	websWrite(wp, "%s", SVN_REVISION);
}

void ej_get_web_page_name(webs_t wp, int argc, char_t ** argv)
{
	websWrite(wp, "%s.asp", websGetVar(wp, "submit_button", "index"));
}

void ej_get_model_name(webs_t wp, int argc, char_t ** argv)
{
	// return websWrite(wp,"%s",MODEL_NAME);
	websWrite(wp, "%s", nvram_safe_get("router_name"));
}

void ej_get_url(webs_t wp, int argc, char_t ** argv)
{
	char *type;

	if (ejArgs(argc, argv, "%s", &type) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return;
	}

	websWrite(wp, "%s", "Invalid argument\n");
}

void ej_show_logo(webs_t wp, int argc, char_t ** argv)
{
	return;
}

int protocol_to_num(char *proto)
{
	if (!strcmp(proto, "icmp"))
		return 1;
	else if (!strcmp(proto, "tcp"))
		return 6;
	else if (!strcmp(proto, "udp"))
		return 17;
	else if (!strcmp(proto, "both"))
		return 23;
	else if (!strcmp(proto, "l7"))
		return 99;
	else if (!strcmp(proto, "p2p"))
		return 100;
#ifdef HAVE_OPENDPI
	else if (!strcmp(proto, "dpi"))
		return 101;
#endif
	else
		return 0;
}
