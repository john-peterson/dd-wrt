#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utils.h>
#include <bcmnvram.h>

#define HOME_DIR	"/tmp/root"
#define PASSWD_DIR	"/tmp/etc"
#define SSH_DIR		"/tmp/root/.ssh"
#define PASSWD_FILE	"/tmp/etc/passwd"
#define GROUP_FILE	"/tmp/etc/group"

#define NOCAT_CONF      "/tmp/etc/nocat.conf"

/* BPsmythe: Return the local network for the NOCAT conf file */
static char *
_get_network (char *ipaddr, char *snmask)
{
  u_long ipaddr2long (char *ipstr)
  {
    int ip[4];
    char *tmp = malloc (4 * sizeof (char));

    ip[0] = atoi (strncpy (tmp, ipstr, strcspn (ipstr, ".")));
    ipstr = strstr (ipstr, ".");
    ipstr++;
    strcpy (tmp, "    ");
    ip[1] = atoi (strncpy (tmp, ipstr, strcspn (ipstr, ".")));
    ipstr = strstr (ipstr, ".");
    ipstr++;
    strcpy (tmp, "    ");
    ip[2] = atoi (strncpy (tmp, ipstr, strcspn (ipstr, ".")));
    ipstr = strstr (ipstr, ".");
    ipstr++;
    strcpy (tmp, "    ");
    ip[3] = atoi (ipstr);

    free (tmp);
    return ((ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + ip[3]);
  }

  char *long2ipaddr (u_long addr)
  {
    static char buff[32];

    sprintf (buff, "%ld.%ld.%ld.%ld",
	     (addr >> 24 & 0xff),
	     (addr >> 16 & 0xff), (addr >> 8 & 0xff), (addr & 0xff));

    return buff;
  }


  static char network[32];

  strcpy (network, long2ipaddr (ipaddr2long (ipaddr) & ipaddr2long (snmask)));

  return network;
}

/* end BPsmythe */


void
to64 (char *s, long v, int n)
{

  unsigned char itoa64[] =
    "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  while (--n >= 0)
    {
      *s++ = itoa64[v & 0x3f];
      v >>= 6;
    }
}

char *
zencrypt (char *passwd)
{
  char salt[6];
  struct timeval tv;
  char *crypt (const char *, const char *);

  gettimeofday (&tv, 0);

  to64 (&salt[0], random (), 3);
  to64 (&salt[3], tv.tv_usec, 3);
  salt[5] = '\0';

  return crypt (passwd, salt);
}


int
mkfiles (void)
{
  FILE *fp;
  struct stat buf;
#ifdef HAVE_SKYTRON
  char *http_passwd = nvram_safe_get ("skyhttp_passwd");
#elif HAVE_NEWMEDIA
  char *http_passwd = nvram_safe_get ("newhttp_passwd");
#elif HAVE_34TELECOM
  char *http_passwd = nvram_safe_get ("newhttp_passwd");
#else
  char *http_passwd = nvram_safe_get ("http_passwd");
#endif
  char *cp;

  if (stat (HOME_DIR, &buf) != 0)
    {
      mkdir (HOME_DIR, 0700);
    }

#ifdef HAVE_SSHD
  if (stat (SSH_DIR, &buf) != 0)
    {
      mkdir (SSH_DIR, 0700);
    }
#endif

  /* Create password's and group's database directory */
  if (stat (PASSWD_DIR, &buf) != 0)
    {
      mkdir (PASSWD_DIR, 0700);
    }

//#ifdef HAVE_FREEBIRD
//      cp = "bJEt.IiWoP9G2";
//#else
  cp = (char *) zencrypt (http_passwd);	/* encrypt password */
//#endif
  /* Write password file with username root and password */
  if (!(fp = fopen (PASSWD_FILE, "w")))
    {
      perror (PASSWD_FILE);
      return errno;
    }
  fprintf (fp, "root:%s:0:0:Root User,,,:/tmp/root:/bin/sh\n", cp);
  fprintf (fp, "reboot:%s:0:0:Root User,,,:/tmp/root:/sbin/reboot\n", cp);
  fclose (fp);

  /* Write group file with group 'root' */
  if (!(fp = fopen (GROUP_FILE, "w")))
    {
      perror (GROUP_FILE);
      return errno;
    }
  fprintf (fp, "root:x:0:\n");
  fclose (fp);

  system ("/bin/mkdir /var/spool");
  system ("/bin/mkdir /var/spool/cron");
  system ("/bin/mkdir /var/spool/cron/crontabs");
  system ("/bin/touch /var/spool/cron/crontabs/root");

  system ("/bin/mkdir /var/log");
  system ("/bin/touch /var/log/messages");

#ifdef HAVE_SNMP
  system ("/bin/mkdir /var/snmp");
#endif
  system ("/bin/chmod 0777 /tmp");

  dns_to_resolv ();

  return 0;
}

#ifdef HAVE_NOCAT
int
mk_nocat_conf (void)
{
  FILE *fp;
  /* BPsmythe: Write out a nocat.conf file */
  if (!(fp = fopen (NOCAT_CONF, "w")))
    {
      perror (NOCAT_CONF);
      return errno;
    }

  fprintf (fp, "#\n");
  /* settings that need to be set based on router configurations */
  fflush (fp);
  fprintf (fp, "InternalDevice\t%s\n", nvram_safe_get ("lan_ifname"));
  fflush (fp);
  fprintf (fp, "ExternalDevice\t%s\n", nvram_safe_get ("wan_ifname"));
  fflush (fp);
  fprintf (fp, "LocalNetwork\t%s/%s\n",
	   _get_network (nvram_safe_get ("lan_ipaddr"),
			nvram_safe_get ("lan_netmask")),
	   nvram_safe_get ("lan_netmask"));
  fflush (fp);
  fprintf (fp, "InsideIP\t%s\n", nvram_safe_get ("lan_ipaddr"));
  fflush (fp);

  /* Irving - Rework getting DNS */
  struct dns_lists *dns_list = NULL;
  dns_list = get_dns_list ();
  if (!dns_list || dns_list->num_servers == 0)
    {
      fprintf (fp, "DNSAddr \t%s\n", nvram_safe_get ("lan_ipaddr"));
    }
  else
    {
      fprintf (fp, "DNSAddr \t%s %s %s\n", dns_list->dns_server[0],
	       dns_list->dns_server[1], dns_list->dns_server[2]);
    }
  if (dns_list)
    free (dns_list);
  fflush (fp);

  /* These are user defined, eventually via the web page */
  fprintf (fp, "ExcludePorts\t%s\n", nvram_safe_get ("NC_ExcludePorts"));
  fflush (fp);
  fprintf (fp, "Verbosity\t%s\n", nvram_safe_get ("NC_Verbosity"));
  fflush (fp);
  fprintf (fp, "GatewayName\t%s\n", nvram_safe_get ("NC_GatewayName"));
  fflush (fp);
  fprintf (fp, "HomePage\t%s\n", nvram_safe_get ("NC_HomePage"));
  fflush (fp);
  fprintf (fp, "AllowedWebHosts\t%s %s\n", nvram_safe_get ("lan_ipaddr"),
	   nvram_safe_get ("NC_AllowedWebHosts"));
  fflush (fp);
  fprintf (fp, "LoginTimeout\t%s\n", nvram_safe_get ("NC_LoginTimeout"));
  fflush (fp);
  fprintf (fp, "DocumentRoot\t%s\n", nvram_safe_get ("NC_DocumentRoot"));
  fflush (fp);
  fprintf (fp, "SplashURL\t%s\n", nvram_safe_get ("NC_SplashURL"));
  fprintf (fp, "SplashTimeout\t900\n");

  fflush (fp);
  fprintf (fp, "RouteOnly\t%s\n", nvram_safe_get ("NC_RouteOnly"));
  fflush (fp);

  /* These should be hardcoded as the defaults */
  fprintf (fp, "SplashForm\t%s\n", "splash.html");
  fflush (fp);
  fprintf (fp, "StatusForm\t%s\n", "status.html");
  fflush (fp);
  fprintf (fp, "GatewayMode\t%s\n", "Open");
  fflush (fp);
  fprintf (fp, "FirewallPath\t%s\n", "/usr/libexec/nocat");
  fflush (fp);

  fclose (fp);
  /* end BPsmythe */

  return 0;
}
#endif
