
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wait.h>
#include <dlfcn.h>

#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <utils.h>
#include <cy_conf.h>
#include <code_pattern.h>
#include <rc.h>

struct mon
{
  char *name;			// Process name
  int count;			// Process count, 0 means don't check
  int type;			// LAN or WAN
//  int (*stop) (void);         // stop function
//  int (*start) (void);                // start function
};

enum
{ M_LAN, M_WAN };

struct mon mons[] = {
//  {"tftpd", 1, M_LAN, stop_tftpd, start_tftpd},
  {"upnp", 1, M_LAN},
  {"process_monitor", 1, M_LAN},
  {"httpd", 2, M_LAN},
  {"udhcpd", 1, M_LAN},
  {"dnsmasq", 1, M_LAN},
  {"dhcpfwd", 1, M_LAN},
#ifndef HAVE_MSSID
  {"nas", 1, M_LAN},
#endif
#ifdef HAVE_NOCAT
  {"splashd", 1, M_LAN},
#endif
#ifdef HAVE_CHILLI
  {"chilli", 1, M_WAN},
#endif
#ifdef HAVE_WIFIDOG
  {"wifidog", 1, M_WAN},
#endif
#ifdef HAVE_OLSRD
  {"olsrd", 1, M_LAN},
#endif
#ifdef HAVE_SPUTNIK_APD
  {"sputnik", 1, M_WAN},
#endif
  {NULL, 0, 0}
};

int
search_process (char *name, int count)
{
  int c = 0;


  c = count_processes (name);
  if (!c)
    {
      printf ("Can't find %s\n", name);
      return 0;
    }
  else
    {
      printf ("Find %s which count is %d\n", name, c);
      //if(count && c != count){
      //      cprintf("%s count is not match\n", name);
      //      return 0;
      //}
      //else
      return 1;
    }
}
void checkupgrade(void)
{
#ifndef HAVE_X86
FILE *in=fopen("/tmp/firmware.bin","rb");
if (in!=NULL)
    {
    fclose(in);
    eval("rm","/tmp/cron.d/check_ps"); // deleting cron file to prevent double call of this
    fprintf(stderr,"found firmware upgrade, flashing now, but we will wait for another 30 seconds\n");
    sleep(30);
#if defined(HAVE_FONERA) || defined(HAVE_WHRAG108) || defined(HAVE_MERAKI) || defined(HAVE_LS2)
    eval("write","/tmp/firmware.bin","rootfs");
#else
    eval("write","/tmp/firmware.bin","linux");
#endif
    fprintf(stderr,"done. rebooting now\n");
    eval("killall","-3","init");
    }
#endif
}
int
do_mon (void)
{
  struct mon *v;
  checkupgrade();
  void *handle = load_service (NULL);
  if (!handle)
    return 1;
  char service[64];
  void (*fptr) (void);
  
  for (v = mons; v < &mons[sizeof (mons) / sizeof (*v)]; v++)
    {
      if (v->name == NULL)
	break;
      cprintf ("checking %s\n", v->name);
      if (v->type == M_WAN)
	if (!check_wan_link (0))
	  {
	    cprintf ("process is wan, but wan is not up\n");
	    continue;
	  }
      if (!search_process (v->name, v->count))
	{

	  cprintf ("Maybe %s had died, we need to re-exec it\n", v->name);
	  sprintf (service, "stop_%s", v->name);
	  fptr = (void (*)(void)) dlsym (handle, service);
	  if (fptr)
	    fptr ();
	  killall (v->name, SIGKILL);
	  sprintf (service, "start_%s", v->name);
	  fptr = (void (*)(void)) dlsym (handle, service);
	  if (fptr)
	    fptr ();
	}
      cprintf ("checking for %s done\n", v->name);
    }
  if (handle);
  dlclose (handle);

  return 1;
}

int
check_ps_main (int argc, char **argv)
{
  pid_t pid;

  if (check_action () != ACT_IDLE)
    {				// Don't execute during upgrading
      cprintf ("check_ps: nothing to do...\n");
      return 1;
    }

  pid = fork ();
  switch (pid)
    {
    case -1:
      perror ("fork failed");
      exit (1);
      break;
    case 0:
      do_mon ();
      exit (0);
      break;
    default:
      _exit (0);
      break;
    }
}
