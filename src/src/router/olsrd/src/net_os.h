/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas T�nnesen(andreto@olsr.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 * $Id: net_os.h,v 1.12 2005/08/28 19:30:29 kattemat Exp $
 */


/*
 * This file defines the OS dependent network related functions
 * that MUST be available to olsrd.
 * The implementations of the functions should be found in
 * <OS>/net.c (e.g. linux/net.c)
 */


#ifndef _OLSR_NET_OS_H
#define _OLSR_NET_OS_H

/* OS dependent functions */

ssize_t
olsr_sendto(int, 
	    const void *, 
	    size_t, 
	    int, 
	    const struct sockaddr *, 
	    socklen_t);

ssize_t  
olsr_recvfrom(int, 
	      void *, 
	      size_t, 
	      int, 
	      struct sockaddr *,
	      socklen_t *);

int
olsr_select(int, 
	    fd_set *, 
	    fd_set *, 
	    fd_set *, 
	    struct timeval *);

int
bind_socket_to_device(int, char *);

int
convert_ip_to_mac(union olsr_ip_addr *, struct sockaddr *, char *);

int
disable_redirects(char *, int, int);

int
disable_redirects_global(int);

int
deactivate_spoof(char *, int, int);

int
restore_settings(int);

int
enable_ip_forwarding(int);

int
gethemusocket(struct sockaddr_in *);

int  
getsocket(struct sockaddr *, int, char *);

int  
getsocket6(struct sockaddr_in6 *, int, char *);

int
get_ipv6_address(char *, struct sockaddr_in6 *, int);

int
calculate_if_metric(char *);

int
check_wireless_interface(char *);

olsr_bool
is_if_link_up(char *);

int
join_mcast(struct interface *, int);

#endif
