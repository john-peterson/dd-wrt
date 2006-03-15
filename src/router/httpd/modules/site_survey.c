
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <cyutils.h>
#include <code_pattern.h>
#include <broadcom.h>

#define SITE_SURVEY_DB  "/tmp/site_survey"
#define SITE_SURVEY_NUM 50


struct site_survey_list
{
  uint8 SSID[32];
  unsigned char BSSID[18];
  uint8 channel;		/* Channel no. */
  int16 RSSI;			/* receive signal strength (in dBm) */
  int8 phy_noise;		/* noise (in dBm) */
  uint16 beacon_period;		/* units are Kusec */
  uint16 capability;		/* Capability information */
  uint rate_count;		/* # rates in this set */
  uint8 dtim_period;		/* DTIM period */
} site_survey_lists[SITE_SURVEY_NUM];

static int
open_site_survey (void)
{
  FILE *fp;

  bzero (site_survey_lists, sizeof (site_survey_lists));

  if ((fp = fopen (SITE_SURVEY_DB, "r")))
    {
      fread (&site_survey_lists[0], sizeof (site_survey_lists), 1, fp);
      fclose (fp);
      return TRUE;
    }
  return FALSE;
}


#ifdef FBNFW

int
ej_list_fbn (int eid, webs_t wp, int argc, char_t ** argv)
{
  int i;
  system ("site_survey");

  open_site_survey ();
  for (i = 0; i < SITE_SURVEY_NUM; i++)
    {


      if (site_survey_lists[i].SSID[0] == 0 ||
	  site_survey_lists[i].BSSID[0] == 0 ||
	  site_survey_lists[i].channel == 0 ||
	  site_survey_lists[i].channel > 14)
	break;

      if (startswith (site_survey_lists[i].SSID, "www.fbn-dd.de"))
	{
	  websWrite (wp, "<option value=\"");
	  websWrite (wp, site_survey_lists[i].SSID);
	  websWrite (wp, "\">");
	  websWrite (wp, site_survey_lists[i].SSID);
	  websWrite (wp, "</option>\n");
	}

    }
}

#endif
int
ej_dump_site_survey (int eid, webs_t wp, int argc, char_t ** argv)
{
  int i;
  int ret;
  char buf[10] = { 0 };
  char *rates = NULL;
  char cmd[256];
  char *name;
  name = websGetVar (wp, "hidden_scan", NULL);
  if (name == NULL || strlen (name) == 0)
    system ("site_survey");
  else
    {
      sprintf (buf, "site_survey \"%s\"", name);
      system (buf);
    }

  open_site_survey ();

  for (i = 0; i < SITE_SURVEY_NUM; i++)
    {


      if (site_survey_lists[i].SSID[0] == 0 ||
	  site_survey_lists[i].BSSID[0] == 0 ||
	  site_survey_lists[i].channel == 0 ||
	  site_survey_lists[i].channel > 14)
	break;

      if (site_survey_lists[i].rate_count == 4)
	rates = "4(b)";
      else if (site_survey_lists[i].rate_count == 12)
	rates = "12(g)";
      else
	{
	  rates = buf;
	  snprintf (rates, 9, "%d", site_survey_lists[i].rate_count);
	}

      ret +=
	websWrite (wp,
		   "%c\"%s\",\"%s\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%s\"\n",
		   i ? ',' : ' ', site_survey_lists[i].SSID,
		   site_survey_lists[i].BSSID, site_survey_lists[i].channel,
		   site_survey_lists[i].RSSI, site_survey_lists[i].phy_noise,
		   site_survey_lists[i].beacon_period,
		   site_survey_lists[i].capability,
		   site_survey_lists[i].dtim_period, rates);
    }

  return ret;
}
