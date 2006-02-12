/*
 *	wol - wake on lan client
 *
 *	$Id: net.c,v 1.1.2.1 2004/06/20 21:55:18 nikki Exp $
 *
 *	Copyright (C) 2000-2003 Thomas Krennwallner <krennwallner@aon.at>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */


#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "net.h"
#include "wol.h"



static int
net_resolv (const char *hostname, struct in_addr *sin_addr)
{
	struct hostent *hent;

	hent = gethostbyname (hostname);
	if (hent == NULL)
		{
			errno = EINVAL;
			return -1;
		}

	memcpy ((void *) sin_addr, (const void *) hent->h_addr,
					(size_t) hent->h_length);

	return 0;
}


int
net_close (int socket)
{
	if (close (socket))
		{
			perror ("Couldn't close socket");
			return -1;
		}

	return 0;
}




int
net_open (void)
{
	int optval;
	int sockfd;

	sockfd = socket (PF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		{
			perror ("socket() failed");
			return -1;
		}

	optval = 1;

	if (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof (optval)))
		{
			perror ("setsockopt() failed");
			close (sockfd);
			return -1;
		}

	return sockfd;
}



int 
net_send (int socket, const char *ip_str, unsigned short int port,
					const void *buf, size_t len)
{
	struct sockaddr_in toaddr;
	int sendret;


	if (ip_str == NULL || buf == NULL) return -1;
	
	memset (&toaddr, 0, sizeof (struct sockaddr_in));

	if (net_resolv (ip_str, &toaddr.sin_addr))
		{
			error (0, 0, _("Invalid IP address given: %s"), strerror (errno));
			return -1;
		}

	toaddr.sin_family = AF_INET;
	toaddr.sin_port = htons (port);

	/* keep on sending and check for possible errors */
	sendret = sendto (socket, buf, len, 0, (struct sockaddr *) &toaddr,
										sizeof (struct sockaddr_in));

	return sendret == -1 ? sendret : 0;
}
