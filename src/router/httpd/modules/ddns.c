
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include <broadcom.h>

void
ej_show_ddns_status (webs_t wp, int argc, char_t ** argv)
{
  char buff[512];
  FILE *fp;
  char *enable = websGetVar (wp, "ddns_enable", NULL);

  if (!enable)
    enable = nvram_safe_get ("ddns_enable");	// for first time

  if (strcmp (nvram_safe_get ("ddns_enable"), enable))	// change service
    websWrite (wp, " ");

  if (nvram_match ("ddns_enable", "0"))	// only for no hidden page
    {
      websWrite (wp, "%s", live_translate ("ddnsm.all_disabled"));
      return;
    }

  /*if (!check_wan_link (0))
     {
     websWrite (wp,
     "<script type=\"text/javascript\">Capture(ddnsm.all_noip)</script>");
     return;
     } */

  if ((fp = fopen ("/tmp/ddns/ddns.log", "r")))
    {
      /* Just dump the log file onto the web page */
      while (fgets (buff, sizeof (buff), fp))
	{
	  websWrite (wp, "%s <br />", buff);
	}
      fclose (fp);
    }
  else
    {
      websWrite (wp, "%s", live_translate ("ddnsm.all_connecting"));
      return;
    }

  return;
}

int
ddns_save_value (webs_t wp)
{
  char *enable, *username, *passwd, *hostname, *dyndnstype, *wildcard,
    *custom, *conf, *url, *force;
  struct variable ddns_variables[] = {
  {argv:ARGV ("0", "1", "2", "3", "4", "5", "6",
	  "7",
	  "8")},
  {argv:ARGV ("30")},
  }, *which;
  int ret = -1;
  char _username[] = "ddns_username_X";
  char _passwd[] = "ddns_passwd_X";
  char _hostname[] = "ddns_hostname_X";
  char _dyndnstype[] = "ddns_dyndnstype_X";
  char _wildcard[] = "ddns_wildcard_X";
  char _custom[] = "ddns_custom_X";
  char _conf[] = "ddns_conf";
  char _url[] = "ddns_url";
  char _force[] = "ddns_force";

  which = &ddns_variables[0];

  enable = websGetVar (wp, "ddns_enable", NULL);
  if (!enable && !valid_choice (wp, enable, &which[0]))
    {
      error_value = 1;
      return 1;
    }

  if (atoi (enable) == 0)
    {
      // Disable
      nvram_set ("ddns_enable", enable);
      return 1;
    }
  else if (atoi (enable) == 1)
    {
      // dyndns
      snprintf (_username, sizeof (_username), "%s", "ddns_username");
      snprintf (_passwd, sizeof (_passwd), "%s", "ddns_passwd");
      snprintf (_hostname, sizeof (_hostname), "%s", "ddns_hostname");
      snprintf (_dyndnstype, sizeof (_dyndnstype), "%s", "ddns_dyndnstype");
      snprintf (_wildcard, sizeof (_wildcard), "%s", "ddns_wildcard");
    }
  else if (atoi (enable) == 2)
    {
      // afraid
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
    }
  else if (atoi (enable) == 3)
    {
      // zoneedit
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
    }
  else if (atoi (enable) == 4)
    {
      // no-ip
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
    }
  else if (atoi (enable) == 5)
    {
      // custom
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
      snprintf (_custom, sizeof (_custom), "ddns_custom_%s", enable);
      snprintf (_conf, sizeof (_conf), "%s", "ddns_conf");
      snprintf (_url, sizeof (_url), "%s", "ddns_url");
    }
  else if (atoi (enable) == 6)
    {
      // 3322 dynamic : added botho 30/07/06
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
      snprintf (_dyndnstype, sizeof (_dyndnstype), "ddns_dyndnstype_%s",
		enable);
      snprintf (_wildcard, sizeof (_wildcard), "ddns_wildcard_%s", enable);
    }
  else if (atoi (enable) == 7)
    {
      // easydns
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
      snprintf (_wildcard, sizeof (_wildcard), "ddns_wildcard_%s", enable);
    }
  else if (atoi (enable) == 8)
    {
      // tzo
      snprintf (_username, sizeof (_username), "ddns_username_%s", enable);
      snprintf (_passwd, sizeof (_passwd), "ddns_passwd_%s", enable);
      snprintf (_hostname, sizeof (_hostname), "ddns_hostname_%s", enable);
    }

  username = websGetVar (wp, _username, NULL);
  passwd = websGetVar (wp, _passwd, NULL);
  hostname = websGetVar (wp, _hostname, NULL);
  dyndnstype = websGetVar (wp, _dyndnstype, NULL);
  wildcard = websGetVar (wp, _wildcard, NULL);
  custom = websGetVar (wp, _custom, NULL);
  conf = websGetVar (wp, _conf, NULL);
  url = websGetVar (wp, _url, NULL);
  force = websGetVar (wp, _force, NULL);

  if (!username || !passwd || !hostname || !force)
    {
      error_value = 1;
      return 1;
    }

  nvram_set ("ddns_enable", enable);
  nvram_set (_username, username);
  if (strcmp (passwd, TMP_PASSWD))
    {
      nvram_set (_passwd, passwd);
    }
  nvram_set (_hostname, hostname);
  nvram_set (_dyndnstype, dyndnstype);
  nvram_set (_wildcard, wildcard);
  nvram_set (_custom, custom);
  nvram_set (_conf, conf);
  nvram_set (_url, url);
  nvram_set (_force, force);

  return ret;
}

int
ddns_update_value (webs_t wp)
{
  return 1;
}
