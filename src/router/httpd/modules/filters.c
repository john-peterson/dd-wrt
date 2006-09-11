
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <broadcom.h>

#ifdef FILTER_DEBUG
extern FILE *debout;
#define D(a) fprintf(debout,"%s\n",a); fflush(debout);
#else
#define D(a)
#endif

/* 
   Format:
     filter_rule{1...10}=$STAT:1$NAME:test1$
	(1=>disable 2=>enable)

   Format:
     filter_tod{1...10} = hr:min hr:min wday
     filter_tod_buf{1...10} = sun mon tue wed thu fri sat	 //only for web page read
   Example:
     Everyday and 24-hour
     filter_todXX = 0:0 23:59 0-0
     filter_tod_bufXX = 7	(for web)

     From 9:55 to 22:00 every sun, wed and thu
     filter_todXX = 9:55 22:00 0,3-4
     filter_tod_bufXX = 1 0 1 1 0 0 0	(for web)

   Format:
     filter_ip_grp{1...10} = ip1 ip2 ip3 ip4 ip5 ip6 ip_r1-ipr2 ip_r3-ip_r4
     filter_ip_mac{1...10} = 00:11:22:33:44:55 00:12:34:56:78:90
 
   Format:
     filter_port=udp:111-222 both:22-33 disable:22-333 tcp:11-22222

   Converting Between AM/PM and 24 Hour Clock:
    Converting from AM/PM to 24 hour clock:
     12:59 AM -> 0059 	 (between 12:00 AM and 12:59 AM, subtract 12 hours)
     10:00 AM -> 1000    (between 1:00 AM and 12:59 PM, a straight conversion)
     10:59 PM -> 2259    (between 1:00 PM and 11:59 PM, add 12 hours)
    Converting from 24 hour clock to AM/PM
     0059 -> 12:59 AM    (between 0000 and 0059, add 12 hours)
     0100 -> 1:00  AM    (between 0100 and 1159, straight converion to AM)
     1259 -> 12:59 PM    (between 1200 and 1259, straight converion to PM)
     1559 -> 3:59  PM    (between 1300 and 2359, subtract 12 hours)
     
*/

int filter_id = 1;
int day_all = 0, week0 = 0, week1 = 0, week2 = 0, week3 = 0, week4 =
  0, week5 = 0, week6 = 0;
int start_week = 0, end_week = 0;
int time_all = 0, start_hour = 0, start_min = 0, start_time = 0, end_hour =
  0, end_min = 0, end_time = 0;
int tod_data_null = 0;

void
validate_filter_ip_grp (webs_t wp, char *value, struct variable *v)
{
  D ("validate_filter_ip_grp");
  int i = 0;
  char buf[256] = "";
  char *ip0, *ip1, *ip2, *ip3, *ip4, *ip5, *ip_range0_0, *ip_range0_1,
    *ip_range1_0, *ip_range1_1;
  unsigned char ip[10] = { 0, 0, 0, 0, 0, 0, 0 };
  struct variable filter_ip_variables[] = {
  {longname: "TCP/UDP IP address", argv:ARGV ("0", "255")},
  }, *which;
  char _filter_ip[] = "filter_ip_grpXXX";
  //char _filter_rule[] = "filter_ruleXXX";
  //char _filter_tod[] = "filter_todXXX";

  which = &filter_ip_variables[0];

  ip0 = websGetVar (wp, "ip0", "0");
  ip1 = websGetVar (wp, "ip1", "0");
  ip2 = websGetVar (wp, "ip2", "0");
  ip3 = websGetVar (wp, "ip3", "0");
  ip4 = websGetVar (wp, "ip4", "0");
  ip5 = websGetVar (wp, "ip5", "0");
  ip_range0_0 = websGetVar (wp, "ip_range0_0", "0");
  ip_range0_1 = websGetVar (wp, "ip_range0_1", "0");
  ip_range1_0 = websGetVar (wp, "ip_range1_0", "0");
  ip_range1_1 = websGetVar (wp, "ip_range1_1", "0");


  if (!valid_range (wp, ip0, &which[0]) ||
      !valid_range (wp, ip1, &which[0]) ||
      !valid_range (wp, ip2, &which[0]) ||
      !valid_range (wp, ip3, &which[0]) ||
      !valid_range (wp, ip4, &which[0]) ||
      !valid_range (wp, ip5, &which[0]) ||
      !valid_range (wp, ip_range0_0, &which[0]) ||
      !valid_range (wp, ip_range0_1, &which[0]) ||
      !valid_range (wp, ip_range1_0, &which[0]) ||
      !valid_range (wp, ip_range1_0, &which[0]))
    {
      error_value = 1;
      D ("invalid range, return");
      return;
    }

  if (atoi (ip0))
    ip[i++] = atoi (ip0);
  if (atoi (ip1))
    ip[i++] = atoi (ip1);
  if (atoi (ip2))
    ip[i++] = atoi (ip2);
  if (atoi (ip3))
    ip[i++] = atoi (ip3);
  if (atoi (ip4))
    ip[i++] = atoi (ip4);
  if (atoi (ip5))
    ip[i++] = atoi (ip5);

  if (atoi (ip_range0_0) > atoi (ip_range0_1))
    SWAP (ip_range0_0, ip_range0_1);

  if (atoi (ip_range1_0) > atoi (ip_range1_1))
    SWAP (ip_range1_0, ip_range1_1);


  sprintf (buf, "%d %d %d %d %d %d %s-%s %s-%s", ip[0], ip[1], ip[2], ip[3],
	   ip[4], ip[5], ip_range0_0, ip_range0_1, ip_range1_0, ip_range1_1);


  snprintf (_filter_ip, sizeof (_filter_ip), "filter_ip_grp%s",
	    nvram_safe_get ("filter_id"));
  nvram_set (_filter_ip, buf);

  //snprintf(_filter_rule, sizeof(_filter_rule), "filter_rule%s", nvram_safe_get("filter_id"));
  //snprintf(_filter_tod, sizeof(_filter_tod), "filter_tod%s", nvram_safe_get("filter_id"));
  //if(nvram_match(_filter_rule, "")){
  //      nvram_set(_filter_rule, "$STAT:1$NAME:$$");
  //      nvram_set(_filter_tod, "0:0 23:59 0-6");
  //}
  D ("success return");

}

/* Example:
 * tcp:100-200 udp:210-220 both:250-260
 */

void
validate_filter_port (webs_t wp, char *value, struct variable *v)
{
  int i;
  char buf[1000] = "", *cur = buf;
  struct variable filter_port_variables[] = {
  {longname: "TCP/UDP Port Filter Starting LAN Port", argv:ARGV ("0",
	  "65535")},
  {longname: "TCP/UDP Port Filter Ending LAN Port", argv:ARGV ("0",
	  "65535")},
  }, *which;
  D ("validate_filter_port");
  which = &filter_port_variables[0];

  for (i = 0; i < FILTER_PORT_NUM; i++)
    {
      char filter_port[] = "protoXXX";
      char filter_port_start[] = "startXXX";
      char filter_port_end[] = "endXXX";
      char *port, *start, *end;
      char *temp;

      snprintf (filter_port, sizeof (filter_port), "proto%d", i);
      snprintf (filter_port_start, sizeof (filter_port_start), "start%d", i);
      snprintf (filter_port_end, sizeof (filter_port_end), "end%d", i);
      port = websGetVar (wp, filter_port, NULL);
      start = websGetVar (wp, filter_port_start, NULL);
      end = websGetVar (wp, filter_port_end, NULL);


      if (!port || !start || !end)
	continue;

      if (!*start && !*end)
	continue;

      if ((!strcmp (start, "0") || !strcmp (start, "")) &&
	  (!strcmp (end, "0") || !strcmp (end, "")))
	continue;

      if (!*start || !*end)
	{
	  //websWrite(wp, "Invalid <b>%s</b>: must specify a LAN Port Range<br>", v->longname);
	  continue;
	}
      if (!valid_range (wp, start, &which[0])
	  || !valid_range (wp, end, &which[1]))
	{
	  error_value = 1;
	  continue;
	}
      if (atoi (start) > atoi (end))
	{
	  temp = start;
	  start = end;
	  end = temp;
	}
      cur += snprintf (cur, buf + sizeof (buf) - cur, "%s%s:%d-%d",
		       cur == buf ? "" : " ", port, atoi (start), atoi (end));
    }


  nvram_set (v->name, buf);
  D ("success return");
}

void
validate_filter_dport_grp (webs_t wp, char *value, struct variable *v)
{
  int i;
  char buf[1000] = "", *cur = buf;
  struct variable filter_port_variables[] = {
  {longname: "TCP/UDP Port Filter Starting LAN Port", argv:ARGV ("0",
	  "65535")},
  {longname: "TCP/UDP Port Filter Ending LAN Port", argv:ARGV ("0",
	  "65535")},
  }, *which;
  char _filter_port[] = "filter_dport_grpXXX";
  //char _filter_rule[] = "filter_ruleXXX";
  //char _filter_tod[] = "filter_todXXX";
  D ("validate_filter-dport-grp");
  which = &filter_port_variables[0];

  for (i = 0; i < FILTER_PORT_NUM; i++)
    {
      char filter_port[] = "protoXXX";
      char filter_port_start[] = "startXXX";
      char filter_port_end[] = "endXXX";
      char *port, *start, *end;
      char *temp;

      snprintf (filter_port, sizeof (filter_port), "proto%d", i);
      snprintf (filter_port_start, sizeof (filter_port_start), "start%d", i);
      snprintf (filter_port_end, sizeof (filter_port_end), "end%d", i);
      port = websGetVar (wp, filter_port, NULL);
      start = websGetVar (wp, filter_port_start, NULL);
      end = websGetVar (wp, filter_port_end, NULL);


      if (!port || !start || !end)
	continue;

      if (!*start && !*end)
	continue;

      if ((!strcmp (start, "0") || !strcmp (start, "")) &&
	  (!strcmp (end, "0") || !strcmp (end, "")))
	continue;

      if (!*start || !*end)
	{
	  //websWrite(wp, "Invalid <b>%s</b>: must specify a LAN Port Range<br>", v->longname);
	  continue;
	}
      if (!valid_range (wp, start, &which[0])
	  || !valid_range (wp, end, &which[1]))
	{
	  error_value = 1;
	  continue;
	}
      if (atoi (start) > atoi (end))
	{
	  temp = start;
	  start = end;
	  end = temp;
	}
      cur += snprintf (cur, buf + sizeof (buf) - cur, "%s%s:%d-%d",
		       cur == buf ? "" : " ", port, atoi (start), atoi (end));
    }

  snprintf (_filter_port, sizeof (_filter_port), "filter_dport_grp%s",
	    nvram_safe_get ("filter_id"));
  nvram_set (_filter_port, buf);

  //snprintf(_filter_rule, sizeof(_filter_rule), "filter_rule%s", nvram_safe_get("filter_id"));
  //snprintf(_filter_tod, sizeof(_filter_tod), "filter_tod%s", nvram_safe_get("filter_id"));
  //if(nvram_match(_filter_rule, "")){
  //      nvram_set(_filter_rule, "$STAT:1$NAME:$$");
  //      nvram_set(_filter_tod, "0:0 23:59 0-6");
  //}
  D ("success return");
}

/* Example:
 * 2 00:11:22:33:44:55 00:11:22:33:44:56
 */

void
validate_filter_mac_grp (webs_t wp, char *value, struct variable *v)
{

  int i;
  char buf[1000] = "", *cur = buf;
  char _filter_mac[] = "filter_mac_grpXXX";
  //char _filter_rule[] = "filter_ruleXXX";
  //har _filter_tod[] = "filter_todXXX";
  D ("validate_filter__mac_grp");

  for (i = 0; i < FILTER_MAC_NUM; i++)
    {
      char filter_mac[] = "macXXX";
      char *mac, mac1[20] = "";


      snprintf (filter_mac, sizeof (filter_mac), "mac%d", i);

      mac = websGetVar (wp, filter_mac, NULL);
      if (!mac)
	continue;

      if (strcmp (mac, "") && strcmp (mac, "00:00:00:00:00:00")
	  && strcmp (mac, "000000000000"))
	{
	  if (strlen (mac) == 12)
	    {
	      char hex[] = "XX";
	      unsigned char h;
	      while (*mac)
		{
		  strncpy (hex, mac, 2);
		  h = (unsigned char) strtoul (hex, NULL, 16);
		  if (strlen (mac1))
		    sprintf (mac1 + strlen (mac1), ":");
		  sprintf (mac1 + strlen (mac1), "%02X", h);
		  mac += 2;
		}
	      mac1[17] = '\0';
	    }
	  else if (strlen (mac) == 17)
	    {
	      strcpy (mac1, mac);
	    }
	  if (!valid_hwaddr (wp, mac1, v))
	    {
	      error_value = 1;
	      continue;
	    }
	}
      else
	{
	  continue;
	}
      cur += snprintf (cur, buf + sizeof (buf) - cur, "%s%s",
		       cur == buf ? "" : " ", mac1);
    }


  snprintf (_filter_mac, sizeof (_filter_mac), "filter_mac_grp%s",
	    nvram_safe_get ("filter_id"));
  nvram_set (_filter_mac, buf);

  //snprintf(_filter_rule, sizeof(_filter_rule), "filter_rule%s", nvram_safe_get("filter_id"));
  //snprintf(_filter_tod, sizeof(_filter_tod), "filter_tod%s", nvram_safe_get("filter_id"));
  //if(nvram_match(_filter_rule, "")){
  //      nvram_set(_filter_rule, "$STAT:1$NAME:$$");
  //      nvram_set(_filter_tod, "0:0 23:59 0-6");
  //}
  D ("success return");
}


/* Example:
 * 100-200 250-260 (ie. 192.168.1.100-192.168.1.200 192.168.1.250-192.168.1.260)
 */

char *
filter_ip_get (char *type, int which)
{
  static char word[256];
  char *start, *end, *wordlist, *next;
  int temp;
  char filter_ip[] = "filter_ip_grpXXX";
  D ("filter_ip_get");
  snprintf (filter_ip, sizeof (filter_ip), "filter_ip_grp%s",
	    nvram_safe_get ("filter_id"));

  wordlist = nvram_safe_get (filter_ip);
  if (!wordlist)
    return "0";

  temp = which;

  foreach (word, wordlist, next)
  {
    if (which-- == 0)
      {
	if (temp == 6)
	  {
	    end = word;
	    start = strsep (&end, "-");
	    if (!strcmp (type, "ip_range0_0"))
	      return start;
	    else
	      return end;
	  }
	else if (temp == 7)
	  {
	    end = word;
	    start = strsep (&end, "-");
	    if (!strcmp (type, "ip_range1_0"))
	      return start;
	    else
	      return end;
	  }
	D ("return word");
	return word;
      }
  }
  D ("return zero");
  return "0";
}

/* Example:
 * tcp:100-200 udp:210-220 both:250-260
 */

char *
filter_dport_get (char *type, int which)
{
  static char word[256];
  char *wordlist, *next;
  char *start, *end, *proto;
  int temp;
  char name[] = "filter_dport_grpXXX";

  sprintf (name, "filter_dport_grp%s", nvram_safe_get ("filter_id"));
  wordlist = nvram_safe_get (name);
  temp = which;
  D ("filter dport get");
  foreach (word, wordlist, next)
  {
    if (which-- == 0)
      {
	start = word;
	proto = strsep (&start, ":");
	end = start;
	start = strsep (&end, "-");
	if (!strcmp (type, "disable"))
	  {
	    if (!strcmp (proto, "disable"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "both"))
	  {
	    if (!strcmp (proto, "both"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "tcp"))
	  {
	    if (!strcmp (proto, "tcp"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "udp"))
	  {
	    if (!strcmp (proto, "udp"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "l7"))
	  {
	    if (!strcmp (proto, "l7"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "start"))
	  return start;
	else if (!strcmp (type, "end"))
	  return end;
      }
  }
  D ("check type and return");
  if (!strcmp (type, "start") || !strcmp (type, "end"))
    return "0";
  else
    return "";
}

void
ej_filter_dport_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  int which;
  char *type;
  D ("ej filter dport get");
  if (ejArgs (argc, argv, "%s %d", &type, &which) < 2)
    {
      websError (wp, 400, "Insufficient args\n");
      D ("bad value");
      return;
    }

  websWrite (wp, "%s", filter_dport_get (type, which));
  D ("good value");

  return;

}

/* Example:
 * tcp:100-200 udp:210-220 both:250-260
 */

char *
filter_port_get (char *type, int which)
{
  static char word[256];
  char *wordlist, *next;
  char *start, *end, *proto;
  int temp;
  D ("filter port get");
  wordlist = nvram_safe_get ("filter_port");
  temp = which;

  foreach (word, wordlist, next)
  {
    if (which-- == 0)
      {
	start = word;
	proto = strsep (&start, ":");
	end = start;
	start = strsep (&end, "-");
	if (!strcmp (type, "disable"))
	  {
	    if (!strcmp (proto, "disable"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "both"))
	  {
	    if (!strcmp (proto, "both"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "tcp"))
	  {
	    if (!strcmp (proto, "tcp"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "udp"))
	  {
	    if (!strcmp (proto, "udp"))
	      return "selected";
	    else
	      return " ";
	  }
	else if (!strcmp (type, "start"))
	  return start;
	else if (!strcmp (type, "end"))
	  return end;
      }
  }
  D ("return type");
  if (!strcmp (type, "start") || !strcmp (type, "end"))
    return "0";
  else
    return "";
}

void
ej_filter_port_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  int which;
  char *type;
  D ("ej filter port get");
  if (ejArgs (argc, argv, "%s %d", &type, &which) < 2)
    {
      websError (wp, 400, "Insufficient args\n");
      D ("bad value");
      return;
    }


  websWrite (wp, "%s", filter_port_get (type, which));

  D ("good value");
  return;

}

/* Example:
 * 00:11:22:33:44:55 00:11:22:33:44:56
 */

char *
filter_mac_get (int which)
{
  static char word[256];
  char *wordlist, *next;
  char *mac;
  int temp;
  char filter_mac[] = "filter_mac_grpXXX";
  D ("filter mac get");
  snprintf (filter_mac, sizeof (filter_mac), "filter_mac_grp%s",
	    nvram_safe_get ("filter_id"));

  wordlist = nvram_safe_get (filter_mac);
  if (!wordlist)
    return "";

  temp = which;

  foreach (word, wordlist, next)
  {
    if (which-- == 0)
      {
	mac = word;
	D ("return mac");
	return mac;
      }
  }
  D ("return zero mac");
  return "00:00:00:00:00:00";
}


void
ej_filter_ip_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  int which;
  char *type;
  D ("ej-filter ip get");
  if (ejArgs (argc, argv, "%s %d", &type, &which) < 2)
    {

      websError (wp, 400, "Insufficient args\n");
      D ("BAD VALUE");
      return;
    }

  websWrite (wp, "%s", filter_ip_get (type, which));

  D ("good value");
  return;
}


void
ej_filter_mac_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  int which;
  D ("ej filter mac get");
  if (ejArgs (argc, argv, "%d", &which) < 1)
    {
      websError (wp, 400, "Insufficient args\n");
      D ("bad value");
      return;
    }

  websWrite (wp, "%s", filter_mac_get (which));
  D ("good value");
  return;
}

#if 0
void
validate_filter_ip_grp (webs_t wp, char *value, struct variable *v)
{
  char *filter_ip_grp_1, *filter_ip_grp_2;

  filter_ip_grp_1 = websGetVar (wp, "filter_ip_grp_1", NULL);
  filter_ip_grp_2 = websGetVar (wp, "filter_ip_grp_2", NULL);

  if (!filter_ip_grp_1)
    return;

  if (!strcmp (filter_ip_grp_1, "0"))
    nvram_set (v->name, "0");
  else if (!strcmp (filter_ip_grp_1, "other"))
    {
      if (!filter_ip_grp_2)
	return;
      nvram_set (v->name, filter_ip_grp_2);
    }
}

void
validate_filter_mac_grp (webs_t wp, char *value, struct variable *v)
{
  char *filter_mac_grp_1, *filter_mac_grp_2;

  filter_mac_grp_1 = websGetVar (wp, "filter_mac_grp_1", NULL);
  filter_mac_grp_2 = websGetVar (wp, "filter_mac_grp_2", NULL);


  if (!filter_mac_grp_1)
    return;

  if (!strcmp (filter_mac_grp_1, "0"))
    nvram_set (v->name, "0");
  else if (!strcmp (filter_mac_grp_1, "other"))
    {
      if (!filter_mac_grp_2)
	return;
      nvram_set (v->name, filter_mac_grp_2);
    }
}
#endif

/*  Format: url0=www.kimo.com.tw, ...
 *	    keywd0=sex, ...
 */
void
validate_filter_web (webs_t wp, char *value, struct variable *v)
{
  int i;
  char buf[1000] = "", *cur = buf;
  char buf1[1000] = "", *cur1 = buf1;
  char filter_host[] = "filter_web_hostXXX";
  char filter_url[] = "filter_web_urlXXX";
  D ("validate_filter_web");
  /* Handle Website Blocking by URL Address */
  for (i = 0; i < 6; i++)
    {
      char filter_host[] = "urlXXX";
      char *host;

      snprintf (filter_host, sizeof (filter_host), "host%d", i);
      host = websGetVar (wp, filter_host, "");

      if (!strcmp (host, ""))
	continue;

      cur += snprintf (cur, buf + sizeof (buf) - cur, "%s%s",
		       cur == buf ? "" : "<&nbsp;>", host);
    }

  if (strcmp (buf, ""))
    strcat (buf, "<&nbsp;>");

  snprintf (filter_host, sizeof (filter_host), "filter_web_host%s",
	    nvram_safe_get ("filter_id"));
  nvram_set (filter_host, buf);

  /* Handle Website Blocking by Keyword */
  for (i = 0; i < 8; i++)
    {
      char filter_url[] = "urlXXX";
      char *url;

      snprintf (filter_url, sizeof (filter_url), "url%d", i);
      url = websGetVar (wp, filter_url, "");

      if (!strcmp (url, ""))
	continue;

      cur1 += snprintf (cur1, buf1 + sizeof (buf1) - cur1, "%s%s",
			cur1 == buf1 ? "" : "<&nbsp;>", url);
    }
  if (strcmp (buf1, ""))
    strcat (buf1, "<&nbsp;>");

  snprintf (filter_url, sizeof (filter_url), "filter_web_url%s",
	    nvram_safe_get ("filter_id"));
  nvram_set (filter_url, buf1);
  D ("everything okay");
}

int
validate_filter_tod (webs_t wp)
{
  char buf[256] = "";
  char tod_buf[20];
  struct variable filter_tod_variables[] = {
  {longname: "Tod name", argv:ARGV ("20")},
  {longname: "Tod Status", argv:ARGV ("0", "1", "2")},

  }, *which;
  D ("validate filter tod");

  char *day_all, *week0, *week1, *week2, *week3, *week4, *week5, *week6;
  char *time_all, *start_hour, *start_min, *end_hour, *end_min;
  int _start_hour, _start_min, _end_hour, _end_min;
  char time[20];
  int week[7];
  int i, flag = -1, dash = 0;
  char filter_tod[] = "filter_todXXX";
  char filter_tod_buf[] = "filter_tod_bufXXX";

  which = &filter_tod_variables[0];

  day_all = websGetVar (wp, "day_all", "0");
  week0 = websGetVar (wp, "week0", "0");
  week1 = websGetVar (wp, "week1", "0");
  week2 = websGetVar (wp, "week2", "0");
  week3 = websGetVar (wp, "week3", "0");
  week4 = websGetVar (wp, "week4", "0");
  week5 = websGetVar (wp, "week5", "0");
  week6 = websGetVar (wp, "week6", "0");
  time_all = websGetVar (wp, "time_all", "0");
  start_hour = websGetVar (wp, "start_hour", "0");
  start_min = websGetVar (wp, "start_min", "0");
//  start_time = websGetVar (wp, "start_time", "0");
  end_hour = websGetVar (wp, "end_hour", "0");
  end_min = websGetVar (wp, "end_min", "0");
//  end_time = websGetVar (wp, "end_time", "0");

  //if(atoi(time_all) == 0)
  //      if(!start_hour || !start_min || !start_time || !end_hour || !end_min || !end_time)
  //              return 1;

  if (atoi (day_all) == 1)
    {
      strcpy (time, "0-6");
      strcpy (tod_buf, "7");
    }
  else
    {
      week[0] = atoi (week0);
      week[1] = atoi (week1);
      week[2] = atoi (week2);
      week[3] = atoi (week3);
      week[4] = atoi (week4);
      week[5] = atoi (week5);
      week[6] = atoi (week6);
      strcpy (time, "");

      for (i = 0; i < 7; i++)
	{
	  if (week[i] == 1)
	    {
	      if (i == 6)
		{
		  if (dash == 0 && flag == 1)
		    sprintf (time + strlen (time), "%c", '-');
		  sprintf (time + strlen (time), "%d", i);
		}
	      else if (flag == 1 && dash == 0)
		{
		  sprintf (time + strlen (time), "%c", '-');
		  dash = 1;
		}
	      else if (dash == 0)
		{
		  sprintf (time + strlen (time), "%d", i);
		  flag = 1;
		  dash = 0;
		}
	    }
	  else
	    {
	      if (!strcmp (time, ""))
		continue;
	      if (dash == 1)
		sprintf (time + strlen (time), "%d", i - 1);
	      if (flag != 0)
		sprintf (time + strlen (time), "%c", ',');
	      flag = 0;
	      dash = 0;
	    }
	}
      if (time[strlen (time) - 1] == ',')
	time[strlen (time) - 1] = '\0';

      snprintf (tod_buf, sizeof (tod_buf), "%s %s %s %s %s %s %s", week0,
		week1, week2, week3, week4, week5, week6);
    }
  if (atoi (time_all) == 1)
    {
      _start_hour = 0;
      _start_min = 0;
      _end_hour = 23;
      _end_min = 59;
    }
  else
    {
      _start_hour = atoi (start_hour);
      _start_min = atoi (start_min);
      _end_hour = atoi (end_hour);
      _end_min = atoi (end_min);
    }

  sprintf (buf, "%d:%d %d:%d %s", _start_hour, _start_min, _end_hour,
	   _end_min, time);
  snprintf (filter_tod, sizeof (filter_tod), "filter_tod%s",
	    nvram_safe_get ("filter_id"));
  snprintf (filter_tod_buf, sizeof (filter_tod_buf), "filter_tod_buf%s",
	    nvram_safe_get ("filter_id"));

  nvram_set (filter_tod, buf);
  nvram_set (filter_tod_buf, tod_buf);
  D ("everything okay");
  return 0;

}

int
delete_policy (webs_t wp, int which)
{
  char filter_rule[] = "filter_ruleXXX";
  char filter_tod[] = "filter_todXXX";
  char filter_tod_buf[] = "filter_tod_bufXXX";
  char filter_host[] = "filter_web_hostXXX";
  char filter_url[] = "filter_web_urlXXX";
  char filter_ip_grp[] = "filter_ip_grpXXX";
  char filter_mac_grp[] = "filter_mac_grpXXX";
  char filter_port_grp[] = "filter_port_grpXXX";
  char filter_dport_grp[] = "filter_dport_grpXXX";
  D ("delete policy");

  snprintf (filter_rule, sizeof (filter_rule), "filter_rule%d", which);
  snprintf (filter_tod, sizeof (filter_tod), "filter_tod%d", which);
  snprintf (filter_tod_buf, sizeof (filter_tod_buf), "filter_tod_buf%d",
	    which);
  snprintf (filter_host, sizeof (filter_host), "filter_web_host%d", which);
  snprintf (filter_url, sizeof (filter_url), "filter_web_url%d", which);
  snprintf (filter_ip_grp, sizeof (filter_ip_grp), "filter_ip_grp%d", which);
  snprintf (filter_mac_grp, sizeof (filter_mac_grp), "filter_mac_grp%d",
	    which);
  snprintf (filter_port_grp, sizeof (filter_port_grp), "filter_port_grp%d",
	    which);
  snprintf (filter_dport_grp, sizeof (filter_dport_grp), "filter_dport_grp%d",
	    which);

  nvram_set (filter_rule, "");
  nvram_set (filter_tod, "");
  nvram_set (filter_tod_buf, "");
  nvram_set (filter_host, "");
  nvram_set (filter_url, "");
  nvram_set (filter_ip_grp, "");
  nvram_set (filter_mac_grp, "");
  nvram_set (filter_port_grp, "");
  nvram_set (filter_dport_grp, "");
  D ("okay");
  return 1;
}

int
single_delete_policy (webs_t wp)
{
  int ret = 0;
  char *id = nvram_safe_get ("filter_id");
  D ("single delete policy");
  ret = delete_policy (wp, atoi (id));
  D ("okay");
  return ret;
}

int
summary_delete_policy (webs_t wp)
{
  int i, ret = 0;
  D ("summary delete policy");
  for (i = 1; i <= 10; i++)
    {
      char filter_sum[] = "sumXXX";
      char *sum;
      snprintf (filter_sum, sizeof (filter_sum), "sum%d", i);
      sum = websGetVar (wp, filter_sum, NULL);
      if (sum)
	ret += delete_policy (wp, i);
    }
  D ("okay");
  return ret;
}

int
save_policy (webs_t wp)
{
  char *f_id, *f_name, *f_status, *f_status2;
  char buf[256] = "";
  struct variable filter_variables[] = {
  {longname: "Filter ID", argv:ARGV ("1", "10")},
  {longname: "Filter Status", argv:ARGV ("0", "1", "2")},
  {longname: "Filter Status", argv:ARGV ("deny", "allow")},


  }, *which;
  char filter_buf[] = "filter_ruleXXX";
  D ("save policy");
  which = &filter_variables[0];
  f_id = websGetVar (wp, "f_id", NULL);
  f_name = websGetVar (wp, "f_name", NULL);
  f_status = websGetVar (wp, "f_status", NULL);	// 0=>Disable / 1,2=>Enable
  f_status2 = websGetVar (wp, "f_status2", NULL);	// deny=>Deny / allow=>Allow
  if (!f_id || !f_name || !f_status || !f_status2)
    {
      error_value = 1;
      D ("invalid");
      return -1;
    }
  if (!valid_range (wp, f_id, &which[0]))
    {
      error_value = 1;
      D ("invalid");
      return -1;
    }
  if (!valid_choice (wp, f_status, &which[1]))
    {
      error_value = 1;
      D ("invalid");
      return -1;
    }
  if (!valid_choice (wp, f_status2, &which[2]))
    {
      error_value = 1;
      D ("invalid");
      return -1;
    }

  validate_filter_tod (wp);

  snprintf (filter_buf, sizeof (filter_buf), "filter_rule%s",
	    nvram_safe_get ("filter_id"));

  // Add $DENY to decide that users select Allow or Deny, if status is Disable    // 2003/10/21
  snprintf (buf, sizeof (buf), "$STAT:%s$NAME:%s$DENY:%d$$", f_status, f_name,
	    !strcmp (f_status2, "deny") ? 1 : 0);

  nvram_set (filter_buf, buf);
  D ("okay");
  return 0;
}

void
validate_filter_policy (webs_t wp, char *value, struct variable *v)
{
  char *f_id = websGetVar (wp, "f_id", NULL);
  D ("validate filter policy");
  if (f_id)
    nvram_set ("filter_id", f_id);
  else
    nvram_set ("filter_id", "1");

  save_policy (wp);
  D ("okay");
}

/*   Format: 21:21:tcp:FTP(&nbsp;)500:1000:both:TEST1
 *
 */

int
validate_services_port (webs_t wp)
{
  char buf[8192] = "", services[8192] = "", *cur = buf, *svcs = NULL;
  char *services_array = websGetVar (wp, "services_array0", NULL);
//  char *services_length = websGetVar (wp, "services_length0", NULL);
  char word[1025], *next;
  char delim[] = "(&nbsp;)";
  char var[32] = "";
  int index = 0;
  D ("validate services port");
//printf("services_array: %s\n", services_array);

//printf("services_length: %s\n", services_length);

//      if(!services_array || !services_length)         return 0;

  do
    {
      snprintf (var, 31, "services_array%d", index++);
      svcs = websGetVar (wp, var, NULL);
      if (svcs)
	strcat (services, svcs);
//      printf ("services_array%d: %s\n", index, svcs);

    }
  while (svcs);

  services_array = services;

  split (word, services_array, next, delim)
  {
    int from, to, proto;
    char name[80];

    if (sscanf (word, "%d:%d:%d:%s", &from, &to, &proto, name) != 4)
      continue;

    cur +=
      snprintf (cur, buf + sizeof (buf) - cur,
		"%s$NAME:%03d:%s$PROT:%03d:%s$PORT:%03d:%d:%d",
		cur == buf ? "" : "<&nbsp;>", strlen (name), name,
		strlen (num_to_protocol (proto)), num_to_protocol (proto),
		(int) (get_int_len (from) + get_int_len (to) + strlen (":")),
		from, to);
  }

  /* segment filter_services into <= 1024 byte lengths */
  cur = buf;
  index = 0;
  while (strlen (cur) >= 1024)
    {
      snprintf (var, 31, "filter_services%d", index);
      memcpy (word, cur, 1024);
      word[1025] = 0;
      nvram_set (var, word);
      cur += 1024;
      index++;
    }

  if (strlen (cur) > 0)
    {
      snprintf (var, 31, "filter_services%d", index);
      nvram_set (var, cur);
    }
  D ("okay");
  return 1;
}

int
save_services_port (webs_t wp)
{
  D ("sdave services port");
  return validate_services_port (wp);
}

void
validate_blocked_service (webs_t wp, char *value, struct variable *v)
{
  int i;
  char buf[1000] = "", *cur = buf;
  char port_grp[] = "filter_port_grpXXX";
  D ("validate_blocked_service");
  for (i = 0; i < BLOCKED_SERVICE_NUM; i++)
    {
      char blocked_service[] = "blocked_serviceXXX";
      char *service;
      snprintf (blocked_service, sizeof (blocked_service),
		"blocked_service%d", i);
      service = websGetVar (wp, blocked_service, NULL);
      if (!service || !strcmp (service, "None"))
	continue;

      cur +=
	snprintf (cur, buf + sizeof (buf) - cur, "%s%s", service, "<&nbsp;>");
      //cur == buf ? "" : "<&nbsp;>", service);
    }

  snprintf (port_grp, sizeof (port_grp), "filter_port_grp%s",
	    nvram_safe_get ("filter_id"));
  nvram_set (port_grp, buf);
  D ("right");
}

/*
validates the p2p catchall filter
*/
void
validate_catchall (webs_t wp, char *value, struct variable *v)
{
  char *p2p;
  char port_grp[] = "filter_p2p_grpXXX";
  p2p = websGetVar (wp, "filter_p2p", NULL);
  if (p2p)
    {
      snprintf (port_grp, sizeof (port_grp), "filter_p2p_grp%s",
		nvram_safe_get ("filter_id"));
      nvram_set (port_grp, p2p);
    }

  return;
}


void
ej_filter_policy_select (int eid, webs_t wp, int argc, char_t ** argv)
{
  int i;
  D ("ej policy select");
  for (i = 1; i <= 10; i++)
    {
      char filter[] = "filter_ruleXXX";
      char *data = "";
      char name[50] = "";
      snprintf (filter, sizeof (filter), "filter_rule%d", i);
      data = nvram_safe_get (filter);

      if (data && strcmp (data, ""))
	{
	  find_match_pattern (name, sizeof (name), data, "$NAME:", "");	// get name value
	}
      websWrite (wp, "<option value=%d %s>%d ( %s ) </option>\n",
		 i,
		 (atoi (nvram_safe_get ("filter_id")) ==
		  i ? "selected=\"selected\" " : ""), i, name);
    }
  D ("okay");
  return;
}


void
ej_filter_policy_get (int eid, webs_t wp, int argc, char_t ** argv)
{

  char *type, *part;

  D ("ej filter policy get");
  if (ejArgs (argc, argv, "%s %s", &type, &part) < 2)
    {
      websError (wp, 400, "Insufficient args\n");
      return;
    }

  if (!strcmp (type, "f_id"))
    {
      websWrite (wp, "%s", nvram_safe_get ("filter_id"));
    }
  else if (!strcmp (type, "f_name"))
    {
      char name[50] = "";
      char filter[] = "filter_ruleXXX";
      char *data = "";
      snprintf (filter, sizeof (filter), "filter_rule%s",
		nvram_safe_get ("filter_id"));
      data = nvram_safe_get (filter);
      if (strcmp (data, ""))
	{
	  find_match_pattern (name, sizeof (name), data, "$NAME:", "");	// get name value
	  websWrite (wp, "%s", name);
	}
    }
  else if (!strcmp (type, "f_status"))
    {
      char status[50] = "", deny[50] = "";
      char filter[] = "filter_ruleXXX";
      char *data = "";
      snprintf (filter, sizeof (filter), "filter_rule%s",
		nvram_safe_get ("filter_id"));
      data = nvram_safe_get (filter);
      if (strcmp (data, ""))
	{			// have data
	  find_match_pattern (status, sizeof (status), data, "$STAT:", "1");	// get status value
	  find_match_pattern (deny, sizeof (deny), data, "$DENY:", "");	// get deny value
	  if (!strcmp (deny, ""))
	    {			// old format
	      if (!strcmp (status, "0") || !strcmp (status, "1"))
		strcpy (deny, "1");	// Deny
	      else
		strcpy (deny, "0");	// Allow
	    }
#if 0
	  if (!strcmp (part, "disable"))
	    {
	      if (!strcmp (status, "1"))
		websWrite (wp, "checked=\"checked\" ");
	    }
	  else if (!strcmp (part, "enable"))
	    {
	      if (!strcmp (status, "2"))
		websWrite (wp, "checked=\"checked\" ");
	    }
#endif
	  if (!strcmp (part, "disable"))
	    {
	      if (!strcmp (status, "0"))
		websWrite (wp, "checked=\"checked\" ");
	    }
	  else if (!strcmp (part, "enable"))
	    {
	      if (strcmp (status, "0"))
		websWrite (wp, "checked=\"checked\" ");
	    }
	  else if (!strcmp (part, "deny"))
	    {
	      if (!strcmp (deny, "1"))
		websWrite (wp, "checked=\"checked\" ");
	    }
	  else if (!strcmp (part, "allow"))
	    {
	      if (!strcmp (deny, "0"))
		websWrite (wp, "checked=\"checked\" ");
	    }
	  else if (!strcmp (part, "onload_status"))
	    {
	      if (!strcmp (deny, "1"))
		websWrite (wp, "deny");
	      else
		websWrite (wp, "allow");

	    }
	  else if (!strcmp (part, "init"))
	    {
	      if (!strcmp (status, "1"))
		websWrite (wp, "disable");
	      else if (!strcmp (status, "2"))
		websWrite (wp, "enable");
	      else
		websWrite (wp, "disable");
	    }
	}
      else
	{			// no data
	  if (!strcmp (part, "disable"))
	    websWrite (wp, "checked=\"checked\" ");
	  else if (!strcmp (part, "allow"))	// default policy is allow, 2003-10-21
	    websWrite (wp, "checked=\"checked\" ");
	  else if (!strcmp (part, "onload_status"))	// default policy is allow, 2003-10-21
	    websWrite (wp, "allow");
	  else if (!strcmp (part, "init"))
	    websWrite (wp, "disable");
	}
    }
  D ("okay");
  return;
}

int
filter_tod_init (int which)
{
  int ret;
  char *tod_data, *tod_buf_data;
  char filter_tod[] = "filter_todXXX";
  char filter_tod_buf[] = "filter_tod_bufXXX";
  char temp[3][20];

  tod_data_null = 0;
  day_all = week0 = week1 = week2 = week3 = week4 = week5 = week6 = 0;
  time_all = start_hour = start_min = start_time = end_hour = end_min =
    end_time = 0;
  start_week = end_week = 0;
  D ("filter tod init");
  snprintf (filter_tod, sizeof (filter_tod), "filter_tod%d", which);
  snprintf (filter_tod_buf, sizeof (filter_tod_buf), "filter_tod_buf%d",
	    which);

  /* Parse filter_tod{1...10} */
  tod_data = nvram_safe_get (filter_tod);
  if (!tod_data)
    return -1;			// no data
  if (strcmp (tod_data, ""))
    {
      sscanf (tod_data, "%s %s %s", temp[0], temp[1], temp[2]);
      sscanf (temp[0], "%d:%d", &start_hour, &start_min);
      sscanf (temp[1], "%d:%d", &end_hour, &end_min);
      ret = sscanf (temp[2], "%d-%d", &start_week, &end_week);
      if (ret == 1)
	end_week = start_week;

      if (start_hour == 0 && start_min == 0 && end_hour == 23
	  && end_min == 59)
	{			// 24 Hours
	  time_all = 1;
	  start_hour = end_hour = 0;
	  start_min = start_time = end_min = end_time = 0;
	}
/*      else
	{			// check AM or PM
	  time_all = 0;
	  if (start_hour > 11)
	    {
	      start_hour = start_hour - 12;
	      start_time = 1;
	    }
	  if (end_hour > 11)
	    {
	      end_hour = end_hour - 12;
	      end_time = 1;
	    }
	} */
    }
  else
    {				// default Everyday and 24 Hours
      tod_data_null = 1;
      day_all = 1;
      time_all = 1;
    }

  if (tod_data_null == 0)
    {
      /* Parse filter_tod_buf{1...10} */
      tod_buf_data = nvram_safe_get (filter_tod_buf);
      if (!strcmp (tod_buf_data, "7"))
	{
	  day_all = 1;
	}
      else if (strcmp (tod_buf_data, ""))
	{
	  sscanf (tod_buf_data, "%d %d %d %d %d %d %d", &week0, &week1,
		  &week2, &week3, &week4, &week5, &week6);
	  day_all = 0;
	}
    }
  D ("okay");
  return 0;
}

void
ej_filter_tod_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  char *type;
  int i;
  D ("ej-filter-tod_get");
  if (ejArgs (argc, argv, "%s", &type) < 1)
    {
      websError (wp, 400, "Insufficient args\n");
      return;
    }

  filter_tod_init (atoi (nvram_safe_get ("filter_id")));

  if (!strcmp (type, "day_all_init"))
    {
      if (day_all == 0)
	websWrite (wp, "1");
      else
	websWrite (wp, "0");
    }
  else if (!strcmp (type, "time_all_init"))
    {
      if (time_all == 0)
	websWrite (wp, "1");
      else
	websWrite (wp, "0");
    }
  else if (!strcmp (type, "day_all"))
    {
      websWrite (wp, "%s", day_all == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "start_week"))
    {
      websWrite (wp, "%d", start_week);
    }
  else if (!strcmp (type, "end_week"))
    {
      websWrite (wp, "%d", end_week);
    }
  else if (!strcmp (type, "week0"))
    {				// Sun
      websWrite (wp, "%s", week0 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "week1"))
    {				// Mon
      websWrite (wp, "%s", week1 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "week2"))
    {				// Tue
      websWrite (wp, "%s", week2 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "week3"))
    {				// Wed
      websWrite (wp, "%s", week3 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "week4"))
    {				// Thu
      websWrite (wp, "%s", week4 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "week5"))
    {				// Fri
      websWrite (wp, "%s", week5 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "week6"))
    {				// Sat
      websWrite (wp, "%s", week6 == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "time_all_en"))
    {				// for linksys
      websWrite (wp, "%s", time_all == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "time_all_dis"))
    {				// for linksys
      websWrite (wp, "%s", time_all == 0 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "time_all"))
    {
      websWrite (wp, "%s", time_all == 1 ? "checked=\"checked\" " : "");
    }
  else if (!strcmp (type, "start_hour_24"))
    {				// 00 -> 23
      for (i = 0; i < 24; i++)
	{

	  websWrite (wp, "<option value=%d %s>%d</option>\n", i,
		     i ==
		     start_hour ? "selected=\"selected\" " : "", i);
	}
    }
  else if (!strcmp (type, "start_min_1"))
    {				// 0 1 2 3 4 .... -> 58 59
      for (i = 0; i < 60; i++)
	{

	  websWrite (wp, "<option value=%02d %s>%02d</option>\n", i,
		     i == start_min ? "selected=\"selected\" " : "",
		     i);
	}
    }
  else if (!strcmp (type, "end_hour_24"))
    {				// 00 ->23
      for (i = 0; i < 24; i++)
	{

	  websWrite (wp, "<option value=%d %s>%d</option>\n", i,
		     i ==
		     end_hour ? "selected=\"selected\" " : "",
		     i);
	}
    }
  else if (!strcmp (type, "end_min_1"))
    {				// 0 1 2 3 4 .... -> 58 59
      for (i = 0; i < 60; i++)
	{

	  websWrite (wp, "<option value=%02d %s>%02d</option>\n", i,
		     i == end_min ? "selected=\"selected\" " : "",
		     i);
	}
    }
/*  else if (!strcmp (type, "start_hour_12"))
    {				// 1 -> 12
      for (i = 1; i <= 12; i++)
	{
	  int j;
	  if (i == 12)
	    j = 0;
	  else
	    j = i;

	  websWrite (wp, "<option value=%d %s>%d</option>\n", j,
		     j == start_hour ? "selected=\"selected\" " : "", i);
	}
    }
  else if (!strcmp (type, "start_min_5"))
    {				// 0 5 10 15 20 25 30 35 40 45 50 55
      for (i = 0; i < 12; i++)
	{

	  websWrite (wp, "<option value=%02d %s>%02d</option>\n", i * 5,
		     i * 5 == start_min ? "selected=\"selected\" " : "",
		     i * 5);
	}
    }
  else if (!strcmp (type, "start_time_am"))
    {
      websWrite (wp, "%s", start_time == 1 ? "" : "selected=\"selected\" ");
    }
  else if (!strcmp (type, "start_time_pm"))
    {
      websWrite (wp, "%s", start_time == 1 ? "selected=\"selected\" " : "");
    }
  else if (!strcmp (type, "end_hour_12"))
    {				// 1 -> 12
      for (i = 1; i <= 12; i++)
	{
	  int j;
	  if (i == 12)
	    j = 0;
	  else
	    j = i;

	  websWrite (wp, "<option value=%d %s>%d</option>\n", j,
		     j == end_hour ? "selected=\"selected\" " : "", i);
	}
    }
  else if (!strcmp (type, "end_min_5"))
    {				// 0 5 10 15 20 25 30 35 40 45 50 55
      for (i = 0; i < 12; i++)
	{

	  websWrite (wp, "<option value=%02d %s>%02d</option>\n", i * 5,
		     i * 5 == end_min ? "selected=\"selected\" " : "", i * 5);
	}
    }
  else if (!strcmp (type, "end_time_am"))
    {
      websWrite (wp, "%s", end_time == 1 ? "" : "selected=\"selected\" ");
    }
  else if (!strcmp (type, "end_time_pm"))
    {
      websWrite (wp, "%s", end_time == 1 ? "selected=\"selected\" " : "");
    } */
  D ("right");
  return;
}

/*  Format: url0, url1, url2, url3, ....
 *	    keywd0, keywd1, keywd2, keywd3, keywd4, keywd5, ....
 */
void
ej_filter_web_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  char *type;
  int which;
  char *token = "<&nbsp;>";
  D ("filter-web-get");
  if (ejArgs (argc, argv, "%s %d", &type, &which) < 1)
    {
      websError (wp, 400, "Insufficient args\n");
      return;
    }


  if (!strcmp (type, "host"))
    {
      char *host_data, filter_host[] = "filter_web_hostXXX";;
      char host[80];

      snprintf (filter_host, sizeof (filter_host), "filter_web_host%s",
		nvram_safe_get ("filter_id"));
      host_data = nvram_safe_get (filter_host);
      if (!strcmp (host_data, ""))
	return;			// no data
      find_each (host, sizeof (host), host_data, token, which, "");
      websWrite (wp, "%s", host);
    }
  else if (!strcmp (type, "url"))
    {
      char *url_data, filter_url[] = "filter_web_urlXXX";
      char url[80];

      snprintf (filter_url, sizeof (filter_url), "filter_web_url%s",
		nvram_safe_get ("filter_id"));
      url_data = nvram_safe_get (filter_url);
      if (!strcmp (url_data, ""))
	return;			// no data
      find_each (url, sizeof (url), url_data, token, which, "");
      websWrite (wp, "%s", url);
    }
  D ("okay");
  return;
}

void
ej_filter_summary_show (int eid, webs_t wp, int argc, char_t ** argv)
{
  int i;
#if LANGUAGE == JAPANESE
  char w[7][10] = { "��", "��", "��", "?�", "��", "��", "�y" };
  char am[] = "�ߑO";
  char pm[] = "�ߌ�";
  char _24h[] = "24 ����";
#else
  char w[7][15] =
    { "share.sun_s1", "share.mon_s1", "share.tue_s1", "share.wed_s1",
    "share.thu_s1", "share.fri_s1", "share.sat_s1"
  };
//  char am[] = "AM";
//  char pm[] = "PM";
  char _24h[] = "24 Hours.";
#endif
  D ("filter summary show");
  for (i = 0; i < 10; i++)
    {
      char name[50] = "---";
      char status[5] = "---";
      char filter[] = "filter_ruleXXX";
      char *data = "";
      char time_buf[50] = "---";
      snprintf (filter, sizeof (filter), "filter_rule%d", i + 1);
      data = nvram_safe_get (filter);
      if (data && strcmp (data, ""))
	{
	  find_match_pattern (name, sizeof (name), data, "$NAME:", "&nbsp;");	// get name value
	  find_match_pattern (status, sizeof (status), data, "$STAT:", "---");	// get name value
	}

      filter_tod_init (i + 1);

      websWrite (wp, " \
		<tr align=\"center\" bgcolor=\"#CCCCCC\" >\n\
			<td width=\"50\" ><font face=\"Arial\" size=\"2\" >%d.</font></td>\n\
			<td width=\"200\" ><font face=\"Arial\" size=\"2\" >%s</font></td>\n\
			<td height=\"30\" width=\"150\" >\n\
			<table width=\"150\" height=\"30\" border=\"1\" cellspacing=\"1\" bordercolor=\"#000000\" bgcolor=\"#FFFFFF\" style=\"border-collapse:collapse\" >\n\
				<tr>\n", i + 1, name);
      websWrite (wp, " \
			<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n\
			<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n\
			<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n\
			<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n", tod_data_null == 0 && (day_all == 1 || week0 == 1) ? "#C0C0C0" : "#FFFFFF", w[0], tod_data_null == 0 && (day_all == 1 || week1 == 1) ? "#C0C0C0" : "#FFFFFF", w[1], tod_data_null == 0 && (day_all == 1 || week2 == 1) ? "#C0C0C0" : "#FFFFFF", w[2], tod_data_null == 0 && (day_all == 1 || week3 == 1) ? "#C0C0C0" : "#FFFFFF", w[3]);
      websWrite (wp, " \
    		<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n\
			<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n\
			<td align=\"center\" width=\"17\" bgcolor=\"%s\" style=\"border-style: solid\"><script type=\"text/javascript\">Capture(%s)</script></td>\n\
		</tr>\n\
		</table>\n\
		</td>\n", tod_data_null == 0 && (day_all == 1 || week4 == 1) ? "#C0C0C0" : "#FFFFFF", w[4], tod_data_null == 0 && (day_all == 1 || week5 == 1) ? "#C0C0C0" : "#FFFFFF", w[5], tod_data_null == 0 && (day_all == 1 || week6 == 1) ? "#C0C0C0" : "#FFFFFF", w[6]);

      if (tod_data_null == 0)
	{
	  if (time_all == 1)
	    strcpy (time_buf, _24h);
	  else
	    {
	      snprintf (time_buf, sizeof (time_buf),
			"%02d:%02d - %02d:%02d",
			start_hour, start_min,
			end_hour, end_min);
		}
	}
      websWrite (wp, " \
        <td width=\"150\" ><font face=\"Arial\" size=\"2\" > %s </font></td>\n\
        <td width=\"70\" ><input type=\"checkbox\" name=\"sum%d\" value=\"1\" ></td>\n\
      </tr>\n", time_buf, i + 1);
    }
  D ("okay");
  return;

}

void
ej_filter_init (int eid, webs_t wp, int argc, char_t ** argv)
{
  char *f_id = websGetVar (wp, "f_id", NULL);
  D ("ej_filter-init");
  if (f_id)			// for first time enter this page and don't press apply.
    nvram_set ("filter_id", f_id);
  else
    nvram_set ("filter_id", "1");

  return;
}

void
ej_filter_port_services_get (int eid, webs_t wp, int argc, char_t ** argv)
{
  char *type;
  int which;
  char word[1024], *next, services[8192] = "", svcs_var[32] = "";
  char delim[] = "<&nbsp;>";
  int index = 0;
  D ("ej_filter_port_services get");
  if (ejArgs (argc, argv, "%s %d", &type, &which) < 2)
    {
      websError (wp, 400, "Insufficient args\n");
      return;
    }

  do
    {
      snprintf (svcs_var, 31, "filter_services%d", index++);
      strcat (services, nvram_safe_get (svcs_var));
    }
  while (strlen (nvram_safe_get (svcs_var)) > 0 && index < 10);

  if (!strcmp (type, "all_list"))
    {
      int count = 0;
//              services = nvram_safe_get("filter_services");
      split (word, services, next, delim)
      {
	int len = 0;
	char *name, *prot, *port;
	char protocol[100], ports[100];
	int from = 0, to = 0;
	//int proto;

	if ((name = strstr (word, "$NAME:")) == NULL ||
	    (prot = strstr (word, "$PROT:")) == NULL ||
	    (port = strstr (word, "$PORT:")) == NULL)
	  continue;

	/* $NAME */
	if (sscanf (name, "$NAME:%3d:", &len) != 1)
	  continue;
	strncpy (name, name + sizeof ("$NAME:nnn:") - 1, len);
	name[len] = '\0';

	/* $PROT */
	if (sscanf (prot, "$PROT:%3d:", &len) != 1)
	  continue;
	strncpy (protocol, prot + sizeof ("$PROT:nnn:") - 1, len);
	protocol[len] = '\0';

	/* $PORT */
	if (sscanf (port, "$PORT:%3d:", &len) != 1)
	  continue;
	strncpy (ports, port + sizeof ("$PORT:nnn:") - 1, len);
	ports[len] = '\0';
	if (sscanf (ports, "%d:%d", &from, &to) != 2)
	  continue;

	//cprintf("match:: name=%s, protocol=%s, ports=%s\n", 
	//      word, protocol, ports);


	websWrite (wp,
		   "services[%d]=new service(%d, \"%s\", %d, %d, %d);\n",
		   count, count, name, from, to, protocol_to_num (protocol));
	count++;

      }

      websWrite (wp, "services_length = %d;\n", count);
    }
  else if (!strcmp (type, "service"))
    {
      char *port_data, filter_port[] = "filter_port_grpXXX";
      char name[80];

      snprintf (filter_port, sizeof (filter_port), "filter_port_grp%s",
		nvram_safe_get ("filter_id"));
      port_data = nvram_safe_get (filter_port);
      if (!strcmp (port_data, ""))
	return;			// no data
      find_each (name, sizeof (name), port_data, "<&nbsp;>", which, "");
      websWrite (wp, "%s", name);

    }
  else if (!strcmp (type, "p2p"))
    {
      char *port_data, filter_port[] = "filter_p2p_grpXXX";

      snprintf (filter_port, sizeof (filter_port), "filter_p2p_grp%s",
		nvram_safe_get ("filter_id"));
      port_data = nvram_safe_get (filter_port);
      if (!strcmp (port_data, ""))
	return;			// no data
      websWrite (wp, "%s", port_data);

    }
  D ("okay");
  return;
}

int
filtersummary_onload (webs_t wp, char *arg)
{
  int ret = 0;
  D ("filter summary unload");
  if (!strcmp (nvram_safe_get ("filter_summary"), "1"))
    {
      websWrite (wp, arg);
    }
  D ("okay");
  return ret;
}
