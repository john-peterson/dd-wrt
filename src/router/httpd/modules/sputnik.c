/*
 * $Id:$
 *
 * Copyright (c) 2005 Sputnik, Inc.
 * All rights reserved
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include <broadcom.h>
#include <support.h>
/* DD-WRT common web support by BrainSlayer */
void
ej_show_sputnik (int eid, webs_t wp, int argc, char_t ** argv)
{
  if (nvram_match ("apd_enable", "1"))
    {
      websWrite (wp,
		 "<li><a href=\"Status_SputnikAPD.asp\">Sputnik Agent</a></li>\n");
    }
  return;
}

/* end */
void
ej_sputnik_apd_status (int eid, webs_t wp, int argc, char_t ** argv)
{
  char *key;
  FILE *fh;

  if (ejArgs (argc, argv, "%s", &key) < 1)
    {
      websError (wp, 400, "Insufficient args\n");
    }

  if ((fh = fopen ("/var/run/apd.status", "r")))
    {
      char s[128];

      /*
       * The first line is always the PPID.  If it doesn't exist, apd
       *   isn't running.
       */
      if (fgets (s, sizeof (s), fh))
	{
	  int apd_pid = (pid_t) atol (s);

	  if (!kill (apd_pid, 0))
	    {
	      if (!strcmp (key, "pid"))
		{
		  websWrite (wp, "%d", apd_pid);
		}
	      else
		{
		  /*
		   * We're good, read the rest.
		   */
		  while (fgets (s, sizeof (s), fh))
		    {
		      int len = strlen (s);
		      char *eqloc;

		      if (len > 0 && s[len - 1] == '\n')
			{
			  s[len - 1] = '\0';
			}

		      if ((eqloc = index (s, '=')))
			{
			  char *v;

			  *eqloc = '\0';
			  v = eqloc + 1;

			  if (!strcmp (key, s))
			    {
			      websWrite (wp, "%s", v);
			      break;
			    }
			}
		    }		/* End while */
		}
	    }
	}

      fclose (fh);

    }

  return;
}				/* End ej_sputnik_apd_status() */
