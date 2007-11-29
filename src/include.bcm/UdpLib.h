/* UdpLib helper header
 * Copyright 2007, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id$
*/
#ifndef _UDPLIB_
#define _UDPLIB_

int udp_open();
int udp_bind(int fd, int portno);
void udp_close(int fd);
int udp_write(int fd, char * buf, int len, struct sockaddr_in * to);
int udp_read(int fd, char * buf, int len, struct sockaddr_in * from);
int udp_read_timed(int fd, char * buf, int len,
                   struct sockaddr_in * from, int timeout);
#endif /* _UDPLIB_ */
