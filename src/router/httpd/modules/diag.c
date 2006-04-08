
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <broadcom.h>


int
diag_ping_start (webs_t wp)
{
  int ret = 0;
  char *ip = websGetVar (wp, "ping_ip", NULL);
  char *times = websGetVar (wp, "ping_times", NULL);

  if (!ip || !times || !strcmp (ip, ""))
    return ret;
  int i;
  int add=0;
  for (i=0;i<strlen(ip);i++)
    {
    if (ip[i]=='"')add++;
    }
  char *n = (char*)malloc(strlen(ip)+add);
  add=0;
  for (i=0;i<strlen(ip);i++)
  {
  if (ip[i]=='"')
    {
    n[add++]='\\';
    n[add++]='"';
    }else{
    n[add++]=ip[i];
    }
  }
  unlink (PING_TMP);
  nvram_set ("ping_ip", n);
  nvram_set ("ping_times", times);
  free(n);

  // The web will hold, so i move to service.c
  //snprintf(cmd, sizeof(cmd), "ping -c %s %s &", times, ip);
  //cprintf("cmd=[%s]\n",cmd);
  //system(cmd);

  return ret;
}

void
removeLineBreak (char *startup)
{
  int i = 0;
  int c = 0;
  for (i = 0; i < strlen (startup); i++)
    {
      if (startup[i] == '\r')
	continue;
      startup[c++] = startup[i];
    }
  startup[c++] = 0;

}

int
ping_startup (webs_t wp)
{
  char *startup = websGetVar (wp, "ping_ip", NULL);
  char *p = startup;

  // filter Windows <cr>ud
  removeLineBreak (startup);

  nvram_set ("rc_startup", startup);
  nvram2file ("rc_startup", "/tmp/.rc_startup");
  chmod ("/tmp/.rc_startup", 0700);
//  diag_ping_start(wp);

  return 0;

}

int
ping_firewall (webs_t wp)
{
  char *firewall = websGetVar (wp, "ping_ip", NULL);
  char *p = firewall;

  // filter Windows <cr>ud
  removeLineBreak (firewall);
  nvram_set ("rc_firewall", firewall);
  nvram2file ("rc_firewall", "/tmp/.rc_firewall");
  chmod ("/tmp/.rc_firewall", 0700);
  //diag_ping_start(wp);

  return 0;
}

int
diag_ping_stop (webs_t wp)
{
  return eval ("killall", "-9", "ping");
}

int
diag_ping_clear (webs_t wp)
{
  return unlink (PING_TMP);
}

int
ping_onload (webs_t wp, char *arg)
{
  int ret = 0;
  int pid;
  char *type = websGetVar (wp, "submit_type", "");

  pid = find_pid_by_ps ("ping");

  if (pid > 0 && strncmp (type, "stop", 4))
    {				// pinging
      ret += websWrite (wp, arg);
    }

  return ret;
}

int
diag_traceroute_start (webs_t wp)
{
  int ret = 0;
  char *ip = websGetVar (wp, "traceroute_ip", NULL);

  if (!ip || !strcmp (ip, ""))
    return ret;

  unlink (TRACEROUTE_TMP);
  nvram_set ("traceroute_ip", ip);

  return ret;
}

int
diag_traceroute_stop (webs_t wp)
{
  return eval ("killall", "-9", "traceroute");
}

int
diag_traceroute_clear (webs_t wp)
{
  return unlink (TRACEROUTE_TMP);
}

int
traceroute_onload (webs_t wp, char *arg)
{
  int ret = 0;
  int pid;
  char *type = websGetVar (wp, "submit_type", "");

  pid = find_pid_by_ps ("traceroute");

  if (pid > 0 && strncmp (type, "stop", 4))
    {				// tracerouting
      ret += websWrite (wp, arg);
    }

  return ret;
}

int
ej_dump_ping_log (int eid, webs_t wp, int argc, char_t ** argv)
{
  int ret = 0, count = 0;
  FILE *fp;
  char line[254];

  if ((fp = fopen (PING_TMP, "r")) != NULL)
    {				// show result
      while (fgets (line, sizeof (line), fp) != NULL)
	{
	  line[strlen (line) - 1] = '\0';
	  if (!strcmp (line, ""))
	    continue;
	  ret += websWrite (wp, "%c\"%s\"\n", count ? ',' : ' ', line);
	  count++;
	}
      fclose (fp);
    }

  return ret;
}

int
ej_dump_traceroute_log (int eid, webs_t wp, int argc, char_t ** argv)
{
  int ret = 0, count = 0;
  FILE *fp;
  char line[254];

  if ((fp = fopen (TRACEROUTE_TMP, "r")) != NULL)
    {				// show result
      while (fgets (line, sizeof (line), fp) != NULL)
	{
	  line[strlen (line) - 1] = '\0';
	  if (!strcmp (line, ""))
	    continue;
	  ret += websWrite (wp, "%c\"%s\"\n", count ? ',' : ' ', line);
	  count++;
	}
      fclose (fp);
    }

  return ret;
}
