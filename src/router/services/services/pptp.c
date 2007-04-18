#ifdef HAVE_PPTPD
#include <stdlib.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
int
start_pptpd (void)
{
  int ret = 0, mss = 0;
  char *lpTemp;
  FILE *fp;

  if (!nvram_invmatch ("pptpd_enable", "0"))
    {
      stop_pptpd ();
      return 0;
    }
//cprintf("stop vpn modules\n");
//  stop_vpn_modules ();


  // Create directory for use by pptpd daemon and its supporting files
  mkdir ("/tmp/pptpd", 0744);
  cprintf ("open options file\n");
  // Create options file that will be unique to pptpd to avoid interference with pppoe and pptp
  fp = fopen ("/tmp/pptpd/options.pptpd", "w");
  cprintf ("adding radius plugin\n");
  if (nvram_match ("pptpd_radius", "1"))
    fprintf (fp, "plugin /usr/lib/pppd/radius.so\n"
	     "radius-config-file /tmp/pptpd/radius/radiusclient.conf\n"
	     "%s%s\n", nvram_get ("pptpd_radavpair") ? "avpair " : "",
	     nvram_get ("pptpd_radavpair") ? nvram_get ("pptpd_radavpair") :
	     "");
  cprintf ("check if wan_wins = zero\n");
  int nowins = 0;
  if (nvram_match ("wan_wins", "0.0.0.0"))
    {
      nvram_set ("wan_wins", "");
      nowins = 1;
    }
  if (strlen (nvram_safe_get ("wan_wins")) == 0)
    nowins = 1;

  cprintf ("write config\n");
  fprintf (fp, "lock\n"
	   "name *\n"
	   "proxyarp\n"
	   "ipcp-accept-local\n"
	   "ipcp-accept-remote\n"
	   "lcp-echo-failure 10\n"
	   "lcp-echo-interval 5\n"
	   "deflate 0\n"
	   "auth\n"
	   "-chap\n"
	   "-mschap\n"
	   "+mschap-v2\n"
	   "mppe stateless\n"
	   "mppc\n"
	   "ms-ignore-domain\n"
	   "chap-secrets /tmp/pptpd/chap-secrets\n"
	   "ip-up-script /tmp/pptpd/ip-up\n"
	   "ip-down-script /tmp/pptpd/ip-down\n"
	   "ms-dns %s\n" "%s%s%s" "%s%s%s" "mtu %s\n" "mru %s\n",
	   // Crude but very effective one-liners. Speed is not an issue as this is only run at startup.
	   // Since we need NULL's returned by nvram_get's we cant use nvram_safe_get
	   nvram_get ("pptpd_dns1") ? nvram_get ("pptpd_dns1") :
	   nvram_safe_get ("lan_ipaddr"),
	   nvram_get ("pptpd_dns2") ? "ms-dns " : "",
	   nvram_get ("pptpd_dns2") ? nvram_get ("pptpd_dns2") : "",
	   nvram_get ("pptpd_dns2") ? "\n" : "", !nowins ? "ms-wins " : "",
	   !nowins ? nvram_get ("wan_wins") : "", !nowins ? "\n" : "",
//         nvram_get ("pptpd_wins2") ? "ms-wins " : "",
//         nvram_get ("pptpd_wins2") ? nvram_get ("pptpd_wins2") : "",
//         nvram_get ("pptpd_wins2") ? "\n" : "",
	   nvram_get ("pptpd_mtu") ? nvram_get ("pptpd_mtu") : "1450",
	   nvram_get ("pptpd_mru") ? nvram_get ("pptpd_mru") : "1450");


  // Following is all crude and need to be revisited once testing confirms that it does work
  // Should be enough for testing..
  if (nvram_match ("pptpd_radius", "1"))
    {
      if (nvram_get ("pptpd_radserver") != NULL
	  && nvram_get ("pptpd_radpass") != NULL)
	{


	  fclose (fp);

	  mkdir ("/tmp/pptpd/radius", 0744);

	  fp = fopen ("/tmp/pptpd/radius/radiusclient.conf", "w");
	  fprintf (fp, "auth_order radius\n"
		   "login_tries 4\n"
		   "login_timeout 60\n"
		   "radius_timeout 10\n"
		   "nologin /etc/nologin\n"
		   "servers /tmp/pptpd/radius/servers\n"
		   "dictionary /etc/radiusclient/dictionary\n"
		   "seqfile /var/run/radius.seq\n"
		   "mapfile /etc/radiusclient/port-id-map\n"
		   "radius_retries 3\n"
		   "authserver %s:%s\n", nvram_get ("pptpd_radserver"),
		   nvram_get ("pptpd_radport") ? nvram_get ("pptpd_radport") :
		   "radius");

	  if (nvram_get ("pptpd_acctserver") != NULL
	      && nvram_get ("pptpd_acctpass") != NULL)
	    fprintf (fp, "acctserver %s:%s\n", nvram_get ("pptpd_acctserver"),
		     nvram_get ("pptpd_acctport") ?
		     nvram_get ("pptpd_acctport") : "radacct");
	  fclose (fp);

	  fp = fopen ("/tmp/pptpd/radius/servers", "w");
	  fprintf (fp, "%s\t%s\n", nvram_get ("pptpd_acctserver"),
		   nvram_get ("pptpd_acctpass"));
	  fprintf (fp, "%s\t%s\n", nvram_get ("pptpd_radserver"),
		   nvram_get ("pptpd_radpass"));
	  fclose (fp);


	}
      else
	fclose (fp);
    }
  else
    fclose (fp);


  // Create pptpd.conf options file for pptpd daemon
  fp = fopen ("/tmp/pptpd/pptpd.conf", "w");
  fprintf (fp, "bcrelay %s\n"
	   "localip %s\n"
	   "remoteip %s\n", nvram_safe_get ("lan_ifname"),
	   nvram_safe_get ("pptpd_lip"), nvram_safe_get ("pptpd_rip"));
  fclose (fp);


  // Create ip-up and ip-down scripts that are unique to pptpd to avoid interference with pppoe and pptp
  /* adjust for tunneling overhead (mtu - 40 byte IP - 108 byte tunnel overhead) */
  if (nvram_match ("mtu_enable", "1"))
    mss = atoi (nvram_safe_get ("wan_mtu")) - 40 - 108;
  else
    mss = 1500 - 40 - 108;

  fp = fopen ("/tmp/pptpd/ip-up", "w");
  fprintf (fp, "#!/bin/sh\n"
	   "/usr/sbin/iptables -I FORWARD -i $1 -p tcp --tcp-flags SYN,RST SYN -m tcpmss --mss %d: -j TCPMSS --set-mss %d\n"
	   "/usr/sbin/iptables -I INPUT -i $1 -j ACCEPT\n"
	   "/usr/sbin/iptables -I FORWARD -i $1 -j ACCEPT\n"
	   "%s\n", mss + 1, mss,
	   nvram_get ("pptpd_ipup_script") ? nvram_get ("pptpd_ipup_script") :
	   "");
  fclose (fp);
  fp = fopen ("/tmp/pptpd/ip-down", "w");
  fprintf (fp, "#!/bin/sh\n"
	   "/usr/sbin/iptables -D FORWARD -i $1 -p tcp --tcp-flags SYN,RST SYN -m tcpmss --mss %d: -j TCPMSS --set-mss %d\n"
	   "/usr/sbin/iptables -D INPUT -i $1 -j ACCEPT\n"
	   "/usr/sbin/iptables -D FORWARD -i $1 -j ACCEPT\n"
	   "%s\n", mss + 1, mss,
	   nvram_get ("pptpd_ipdown_script") ?
	   nvram_get ("pptpd_ipdown_script") : "");
  fclose (fp);
  chmod ("/tmp/pptpd/ip-up", 0744);
  chmod ("/tmp/pptpd/ip-down", 0744);

  // Exctract chap-secrets from nvram and add the default account with routers password
  lpTemp = nvram_safe_get ("pptpd_auth");
  fp = fopen ("/tmp/pptpd/chap-secrets", "w");
//  fprintf (fp, "root\t*\t%s\t*\n", nvram_safe_get ("http_passwd"));
  if (strlen (lpTemp) != 0)
    fprintf (fp, "%s\n", lpTemp);
  fclose (fp);

  chmod ("/tmp/pptpd/chap-secrets", 0600);

  //  Execute pptpd daemon
  ret =
    eval ("/usr/sbin/pptpd", "-c", "/tmp/pptpd/pptpd.conf", "-o",
	  "/tmp/pptpd/options.pptpd");

  syslog (LOG_INFO, "pptpd : pptp daemon successfully started\n");
  return ret;
}

int
stop_pptpd (void)
{
  int ret = 0;
  if (pidof ("pptpd") > 0)
    syslog (LOG_INFO, "pptpd : pptp daemon successfully stopped\n");
  ret = killall ("pptpd", SIGKILL);
  killall ("bcrelay", SIGKILL);
  return ret;
}
#endif

