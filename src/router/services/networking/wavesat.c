/*
 * wavesat.c
 *
 * Copyright (C) 2008 Sebastian Gottschall <gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id:
 */


#ifdef HAVE_WAVESAT
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>




#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <getopt.h>
#include <err.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <utils.h>
#include <unistd.h>
#include <linux/if.h>

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

extern int br_add_interface (const char *br, const char *dev);

void
configure_wimax (void)
{
  char *mode = "0";
  char *dev = "ofdm";
  if (nvram_match ("ofdm_duplex", "TDD"))
    mode = "0";
  if (nvram_match ("ofdm_duplex", "H-FDD"))
    mode = "1";
  if (nvram_match ("ofdm_mode", "disabled"))
    return;
  eval ("/sub/lm_scripts/go_ss", nvram_safe_get ("ofdm_width"), mode);
  if (!nvram_match ("ofdm_mode", "sta"))
    {
      char bridged[32];
      sprintf (bridged, "%s_bridged", dev);
      if (nvram_default_match (bridged, "1", "1"))
	{
	  ifconfig (dev, IFUP, NULL, NULL);
	  br_add_interface (getBridge (dev), dev);
	  eval ("ifconfig", dev, "0.0.0.0", "up");
	}
      else
	{
	  char ip[32];
	  char mask[32];
	  sprintf (ip, "%s_ipaddr", dev);
	  sprintf (mask, "%s_netmask", dev);
//        eval ("ifconfig", dev, "mtu", "1500");
	  eval ("ifconfig", dev, nvram_safe_get (ip), "netmask",
		nvram_safe_get (mask), "up");
	}
    }
  else
    {
      char bridged[32];
      sprintf (bridged, "%s_bridged", dev);
      if (nvram_default_match (bridged, "0", "1"))
	{
	  char ip[32];
	  char mask[32];
	  sprintf (ip, "%s_ipaddr", dev);
	  sprintf (mask, "%s_netmask", dev);
	  eval ("ifconfig", dev, "mtu", "1500");
	  eval ("ifconfig", dev, nvram_safe_get (ip), "netmask",
		nvram_safe_get (mask), "up");
	}
    }
}
void
deconfigure_wimax (void)
{
  char *dev = "ofdm";

  if (ifexists (dev))
    {
      br_del_interface ("br0", dev);
      eval ("ifconfig", dev, "down");
    }
  eval ("/sub/common/ssmodunload");
}
#endif
