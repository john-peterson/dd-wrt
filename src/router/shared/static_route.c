
/*
 *********************************************************
 *   Copyright 2003, CyberTAN  Inc.  All Rights Reserved *
 *********************************************************

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of CyberTAN Inc.
 the contents of this file may not be disclosed to third parties,
 copied or duplicated in any form without the prior written
 permission of CyberTAN Inc.

 This software should be used as a reference only, and it not
 intended for production use!


 THIS SOFTWARE IS OFFERED "AS IS", AND CYBERTAN GRANTS NO WARRANTIES OF ANY
 KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE.  CYBERTAN
 SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/route.h>

#include <broadcom.h>

/*
 * Example: 
 * static_route=192.168.2.0:255.255.255.0:192.168.1.2:1:br0
 * <% static_route("ipaddr", 0); %> produces "192.168.2.0"
 * <% static_route("lan", 0); %> produces "selected" if nvram_match("lan_ifname", "br0")
 */
int
ej_static_route_setting (int eid, webs_t wp, int argc, char_t ** argv)
{
  char *arg;
  int which, count;
  char word[256], *next, *page;
  char name[50] = "", *ipaddr, *netmask, *gateway, *metric, *ifname;
  int ret = 0, temp;
  char new_name[200];

  if (ejArgs (argc, argv, "%s %d", &arg, &count) < 2)
    {
      websError (wp, 400, "Insufficient args\n");
      return -1;
    }

  page = websGetVar (wp, "route_page", NULL);
  if (!page)
    page = "0";

  which = atoi (page);

  temp = which;

  if (!strcmp (arg, "name"))
    {
      foreach (word, nvram_safe_get ("static_route_name"), next)
      {
	if (which-- == 0)
	  {
	    find_match_pattern (name, sizeof (name), word, "$NAME:", "");
	    httpd_filter_name (name, new_name, sizeof (new_name), GET);
	    return websWrite (wp, new_name);
	  }

      }
    }

  foreach (word, nvram_safe_get ("static_route"), next)
  {
    if (which-- == 0)
      {
	netmask = word;
	ipaddr = strsep (&netmask, ":");
	if (!ipaddr || !netmask)
	  continue;
	gateway = netmask;
	netmask = strsep (&gateway, ":");
	if (!netmask || !gateway)
	  continue;
	metric = gateway;
	gateway = strsep (&metric, ":");
	if (!gateway || !metric)
	  continue;
	ifname = metric;
	metric = strsep (&ifname, ":");
	if (!metric || !ifname)
	  continue;
	if (!strcmp (arg, "ipaddr"))
	  {
	    return websWrite (wp, "%d", get_single_ip (ipaddr, count));
	  }
	else if (!strcmp (arg, "netmask"))
	  {
	    return websWrite (wp, "%d", get_single_ip (netmask, count));
	  }
	else if (!strcmp (arg, "gateway"))
	  {
	    return websWrite (wp, "%d", get_single_ip (gateway, count));
	  }
	else if (!strcmp (arg, "metric"))
	  return websWrite (wp, metric);
	else if (!strcmp (arg, "lan") && nvram_match ("lan_ifname", ifname))
	  return websWrite (wp, "selected");
	else if (!strcmp (arg, "wan") && nvram_match ("wan_ifname", ifname))
	  return websWrite (wp, "selected");
      }
  }

  if (!strcmp (arg, "ipaddr") || !strcmp (arg, "netmask")
      || !strcmp (arg, "gateway"))
    ret += websWrite (wp, "0");
  else if (!strcmp (arg, "metric"))
    ret += websWrite (wp, "0");

  return ret;
}

void
validate_static_route (webs_t wp, char *value, struct variable *v)
{
  int i, tmp = 1;
  char word[256], *next;
  char buf[1000] = "", *cur = buf;
  char buf_name[1000] = "", *cur_name = buf_name;
  char old[STATIC_ROUTE_PAGE][60];
  char old_name[STATIC_ROUTE_PAGE][30];
  struct variable static_route_variables[] = {
  {longname: "Route IP Address", argv:NULL},
  {longname: "Route Subnet Mask", argv:NULL},
  {longname: "Route Gateway", argv:NULL},
  {longname: "Route Interface", argv:ARGV ("lan", "wan")},
  };

  char *name, ipaddr[20], netmask[20], gateway[20], *metric =
    "0", *ifname, *page;
  char new_name[80];
  char temp[30], *val = NULL;

  name = websGetVar (wp, "route_name", "");	// default empty if no find route_name

  /* validate ip address */
  strcpy (ipaddr, "");
  for (i = 0; i < 4; i++)
    {
      snprintf (temp, sizeof (temp), "%s_%d", "route_ipaddr", i);
      val = websGetVar (wp, temp, NULL);
      if (val)
	{
	  strcat (ipaddr, val);
	  if (i < 3)
	    strcat (ipaddr, ".");
	}
      else
	{
//        free (ipaddr);
	  return;
	}
    }

  /* validate netmask */
  strcpy (netmask, "");
  for (i = 0; i < 4; i++)
    {
      snprintf (temp, sizeof (temp), "%s_%d", "route_netmask", i);
      val = websGetVar (wp, temp, NULL);
      if (val)
	{
	  strcat (netmask, val);
	  if (i < 3)
	    strcat (netmask, ".");
	}
      else
	{
//        free (netmask);
//        free (ipaddr);
	  return;
	}
    }

  /* validate gateway */
  strcpy (gateway, "");
  for (i = 0; i < 4; i++)
    {
      snprintf (temp, sizeof (temp), "%s_%d", "route_gateway", i);
      val = websGetVar (wp, temp, NULL);
      if (val)
	{
	  strcat (gateway, val);
	  if (i < 3)
	    strcat (gateway, ".");
	}
      else
	{
//        free (gateway);
//        free (netmask);
//        free (ipaddr);
	  return;
	}
    }

  page = websGetVar (wp, "route_page", NULL);
  ifname = websGetVar (wp, "route_ifname", NULL);


  if (!page || !ipaddr || !netmask || !gateway || !metric || !ifname)
    return;

  if ((!strcmp (ipaddr, "0.0.0.0") || !strcmp (ipaddr, "")) &&
      (!strcmp (netmask, "0.0.0.0") || !strcmp (netmask, "")) &&
      (!strcmp (gateway, "0.0.0.0") || !strcmp (gateway, "")))
    {
      tmp = 0;
      goto write_nvram;
    }

  if (!valid_choice (wp, ifname, &static_route_variables[3]))
    {
//      free (gateway);
//      free (netmask);
//      free (ipaddr);

      return;
    }

  if (!*ipaddr)
    {
      websDebugWrite (wp, "Invalid <b>%s</b>: must specify an IP Address<br>",
		      v->longname);
//      free (gateway);
//      free (netmask);
//      free (ipaddr);

      return;
    }
  if (!*netmask)
    {
      websDebugWrite (wp, "Invalid <b>%s</b>: must specify a Subnet Mask<br>",
		      v->longname);
//      free (gateway);
//      free (netmask);
//      free (ipaddr);

      return;
    }
  if (!strcmp (ifname, "lan"))
    {
      ifname = nvram_safe_get ("lan_ifname");
      //if(!strcmp(gateway,"0.0.0.0") || !strcmp(gateway,""))
      static_route_variables[2].argv = NULL;
      //else
      //      static_route_variables[2].argv = ARGV("lan_ipaddr", "lan_netmask");
    }
  else
    {
      ifname = get_wan_face ();
      static_route_variables[2].argv = NULL;
    }
  if (!valid_ipaddr (wp, ipaddr, &static_route_variables[0]) ||
      !valid_netmask (wp, netmask, &static_route_variables[1]) ||
      !valid_ipaddr (wp, gateway, &static_route_variables[2]))
    {
//      free (gateway);
//      free (netmask);
//      free (ipaddr);

      return;
    }

  /* save old value in nvram */

write_nvram:

  for (i = 0; i < STATIC_ROUTE_PAGE; i++)
    {
      strcpy (old[i], "");
      strcpy (old_name[i], "");
    }
  i = 0;
  foreach (word, nvram_safe_get ("static_route"), next)
  {
    strcpy (old[i], word);
    i++;
  }
  i = 0;
  foreach (word, nvram_safe_get ("static_route_name"), next)
  {
    strcpy (old_name[i], word);
    i++;
  }

  if (!tmp)
    {
      snprintf (old[atoi (page)], sizeof (old[0]), "%s", "");
      snprintf (old_name[atoi (page)], sizeof (old_name[0]), "%s", "");
    }
  else
    {
      snprintf (old[atoi (page)], sizeof (old[0]), "%s:%s:%s:%s:%s", ipaddr,
		netmask, gateway, metric, ifname);
      httpd_filter_name (name, new_name, sizeof (new_name), SET);
      snprintf (old_name[atoi (page)], sizeof (old_name[0]), "$NAME:%s$$",
		new_name);
    }


  for (i = 0; i < STATIC_ROUTE_PAGE; i++)
    {
      if (strcmp (old[i], ""))
	cur += snprintf (cur, buf + sizeof (buf) - cur, "%s%s",
			 cur == buf ? "" : " ", old[i]);
      if (strcmp (old_name[i], ""))
	cur_name +=
	  snprintf (cur_name, buf_name + sizeof (buf_name) - cur_name, "%s%s",
		    cur_name == buf_name ? "" : " ", old_name[i]);
    }

  nvram_set (v->name, buf);
  nvram_set ("static_route_name", buf_name);

//  if (ipaddr)
//    free (ipaddr);
//  if (netmask)
//    free (netmask);
//  if (gateway)
//    free (gateway);
}

int
ej_static_route_table (int eid, webs_t wp, int argc, char_t ** argv)
{
  int i, ret = 0, page;
  int which;
  char *type;
  char word[256], *next;

  if (ejArgs (argc, argv, "%s", &type) < 1)
    {
      websError (wp, 400, "Insufficient args\n");
      return -1;
    }

  page = atoi (websGetVar (wp, "route_page", "0"));	// default to 0

  if (!strcmp (type, "select"))
    {
      for (i = 0; i < STATIC_ROUTE_PAGE; i++)
	{
	  char name[50] = " ";
	  char new_name[80] = " ";
	  char buf[80] = "";
	  which = i;
	  foreach (word, nvram_safe_get ("static_route_name"), next)
	  {
	    if (which-- == 0)
	      {
		find_match_pattern (name, sizeof (name), word, "$NAME:", " ");
		httpd_filter_name (name, new_name, sizeof (new_name), GET);
	      }
	  }
	  snprintf (buf, sizeof (buf), "(%s)", new_name);

	  ret =
	    websWrite (wp, "\t\t<option value=\"%d\" %s> %d %s</option>\n", i,
		       (i == page) ? "selected" : "", i + 1, buf);
	}
    }

  return ret;
}

int
delete_static_route (webs_t wp)
{
  char buf[1000] = "", *cur = buf;
  char buf_name[1000] = "", *cur_name = buf_name;
  static char word[256], *next;
  static char word_name[256], *next_name;
  char *page = websGetVar (wp, "route_page", NULL);
  int i = 0;

  foreach (word, nvram_safe_get ("static_route"), next)
  {
    if (i == atoi (page))
      {
	nvram_set ("action_service_arg1", word);
	i++;
	continue;
      }

    cur += snprintf (cur, buf + sizeof (buf) - cur, "%s%s",
		     cur == buf ? "" : " ", word);

    i++;
  }

  i = 0;
  foreach (word_name, nvram_safe_get ("static_route_name"), next_name)
  {
    if (i == atoi (page))
      {
	i++;
	continue;
      }
    cur_name +=
      snprintf (cur_name, buf_name + sizeof (buf_name) - cur_name, "%s%s",
		cur_name == buf_name ? "" : " ", word_name);

    i++;
  }

  nvram_set ("static_route", buf);
  nvram_set ("static_route_name", buf_name);

  return 1;
}
