/* dnsmasq is Copyright (c) 2000-2006 Simon Kelley

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
*/

#include "dnsmasq.h"

#ifndef HAVE_LINUX_NETWORK

#include <net/bpf.h>

static struct iovec ifconf = {
  .iov_base = NULL,
  .iov_len = 0
};

static struct iovec ifreq = {
  .iov_base = NULL,
  .iov_len = 0
};

struct header {
  struct ether_header ether; 
  struct ip ip;
  struct udphdr {
    u16 uh_sport;               /* source port */
    u16 uh_dport;               /* destination port */
    u16 uh_ulen;                /* udp length */
    u16 uh_sum;                 /* udp checksum */
  } udp;
};

void init_bpf(struct daemon *daemon)
{
  int i = 0;

  while (1) 
    {
      /* useful size which happens to be sufficient */
      if (expand_buf(&ifreq, sizeof(struct ifreq)))
	{
	  sprintf(ifreq.iov_base, "/dev/bpf%d", i++);
	  if ((daemon->dhcp_raw_fd = open(ifreq.iov_base, O_RDWR, 0)) != -1)
	    {
	      int flags = fcntl(daemon->dhcp_raw_fd, F_GETFD);
	      if (flags != -1)
		fcntl(daemon->dhcp_raw_fd, F_SETFD, flags | FD_CLOEXEC); 
	      return;
	    }
	}
      if (errno != EBUSY)
	die(_("cannot create DHCP BPF socket: %s"), NULL);
    }	     
}

void send_via_bpf(struct daemon *daemon, struct dhcp_packet *mess, size_t len,
		  struct in_addr iface_addr, struct ifreq *ifr)
{
   /* Hairy stuff, packet either has to go to the
      net broadcast or the destination can't reply to ARP yet,
      but we do know the physical address. 
      Build the packet by steam, and send directly, bypassing
      the kernel IP stack */
  
  struct header header;
  u32 i, sum;
  struct iovec iov[2];

  /* Only know how to do ethernet on *BSD */
  if (mess->htype != ARPHRD_ETHER || mess->hlen != ETHER_ADDR_LEN)
    {
      syslog(LOG_WARNING, _("DHCP request for unsupported hardware type (%d) received on %s"), 
	     mess->htype, ifr->ifr_name);
      return;
    }
   
  ifr->ifr_addr.sa_family = AF_LINK;
  if (ioctl(daemon->dhcpfd, SIOCGIFADDR, ifr) < 0)
    return;
  
  memcpy(header.ether.ether_shost, LLADDR((struct sockaddr_dl *)&ifr->ifr_addr), ETHER_ADDR_LEN);
  header.ether.ether_type = htons(ETHERTYPE_IP);
  
  if (ntohs(mess->flags) & 0x8000)
    {
      memset(header.ether.ether_dhost, 255,  ETHER_ADDR_LEN);
      header.ip.ip_dst.s_addr = INADDR_BROADCAST;
    }
  else
    {
      memcpy(header.ether.ether_dhost, mess->chaddr, ETHER_ADDR_LEN); 
      header.ip.ip_dst.s_addr = mess->yiaddr.s_addr;
    }
  
  header.ip.ip_p = IPPROTO_UDP;
  header.ip.ip_src.s_addr = iface_addr.s_addr;
  header.ip.ip_len = htons(sizeof(struct ip) + 
			   sizeof(struct udphdr) +
			   len) ;
  header.ip.ip_hl = sizeof(struct ip) / 4;
  header.ip.ip_v = IPVERSION;
  header.ip.ip_tos = 0;
  header.ip.ip_id = htons(0);
  header.ip.ip_off = htons(0x4000); /* don't fragment */
  header.ip.ip_ttl = IPDEFTTL;
  header.ip.ip_sum = 0;
  for (sum = 0, i = 0; i < sizeof(struct ip) / 2; i++)
    sum += ((u16 *)&header.ip)[i];
  while (sum>>16)
    sum = (sum & 0xffff) + (sum >> 16);  
  header.ip.ip_sum = (sum == 0xffff) ? sum : ~sum;
  
  header.udp.uh_sport = htons(DHCP_SERVER_PORT);
  header.udp.uh_dport = htons(DHCP_CLIENT_PORT);
  if (len & 1)
    ((char *)mess)[len] = 0; /* for checksum, in case length is odd. */
  header.udp.uh_sum = 0;
  header.udp.uh_ulen = sum = htons(sizeof(struct udphdr) + len);
  sum += htons(IPPROTO_UDP);
  for (i = 0; i < 4; i++)
    sum += ((u16 *)&header.ip.ip_src)[i];
  for (i = 0; i < sizeof(struct udphdr)/2; i++)
    sum += ((u16 *)&header.udp)[i];
  for (i = 0; i < (len + 1) / 2; i++)
    sum += ((u16 *)mess)[i];
  while (sum>>16)
    sum = (sum & 0xffff) + (sum >> 16);
  header.udp.uh_sum = (sum == 0xffff) ? sum : ~sum;
  
  ioctl(daemon->dhcp_raw_fd, BIOCSETIF, ifr);
  
  iov[0].iov_base = &header;
  iov[0].iov_len = sizeof(struct header);
  iov[1].iov_base = mess;
  iov[1].iov_len = len;
  while (writev(daemon->dhcp_raw_fd, iov, 2) == -1 && retry_send());
}

int iface_enumerate(struct daemon *daemon, void *parm, int (*ipv4_callback)(), int (*ipv6_callback)())
{
  char *ptr;
  struct ifreq *ifr;
  struct ifconf ifc;
  int fd, errsav, ret = 0;
  int lastlen = 0;
  size_t len;
  
  if ((fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    return 0;
        
  for (len = 0; ; len += 10*sizeof(struct ifreq))
    {
       if (!expand_buf(&ifconf, len))
	 goto err;

       ifc.ifc_len = len;
       ifc.ifc_buf = ifconf.iov_base;

       if (ioctl(fd, SIOCGIFCONF, &ifc) == -1)
	 {
	   if (errno != EINVAL || lastlen != 0)
	     goto err;
	 }
       else
	 {
	   if (ifc.ifc_len == lastlen)
	     break; /* got a big enough buffer now */
	   lastlen = ifc.ifc_len;
	 }
     }
  
  for (ptr = ifc.ifc_buf; ptr < ifc.ifc_buf + ifc.ifc_len; ptr += len )
    {
      /* subsequent entries may not be aligned, so copy into
	 an aligned buffer to avoid nasty complaints about 
	 unaligned accesses. */
#ifdef HAVE_SOCKADDR_SA_LEN
      len = ((struct ifreq *)ptr)->ifr_addr.sa_len + IF_NAMESIZE;
#else
      len = sizeof(struct ifreq);
#endif
      if (!expand_buf(&ifreq, len))
	goto err;

      ifr = ifreq.iov_base;
      memcpy(ifr, ptr, len);
           
      if (ifr->ifr_addr.sa_family == AF_INET && ipv4_callback)
	{
	  struct in_addr addr, netmask, broadcast;
	  broadcast.s_addr = 0;
	  addr = ((struct sockaddr_in *) &ifr->ifr_addr)->sin_addr;
	  if (ioctl(fd, SIOCGIFNETMASK, ifr) == -1)
	    continue;
	  netmask = ((struct sockaddr_in *) &ifr->ifr_addr)->sin_addr;
	  if (ioctl(fd, SIOCGIFBRDADDR, ifr) != -1)
	    broadcast = ((struct sockaddr_in *) &ifr->ifr_addr)->sin_addr; 
	  if (!((*ipv4_callback)(daemon, addr, 
				 (int)if_nametoindex(ifr->ifr_name),
				 netmask, broadcast, 
				 parm)))
	    goto err;
	}
#ifdef HAVE_IPV6
      else if (ifr->ifr_addr.sa_family == AF_INET6 && ipv6_callback)
	{
	  struct in6_addr *addr = &((struct sockaddr_in6 *)&ifr->ifr_addr)->sin6_addr;
	  /* voodoo to clear interface field in address */
	  if (!(daemon->options & OPT_NOWILD) && IN6_IS_ADDR_LINKLOCAL(addr))
	    {
	      addr->s6_addr[2] = 0;
	      addr->s6_addr[3] = 0;
	    }
	  if (!((*ipv6_callback)(daemon, addr,
				 (int)((struct sockaddr_in6 *)&ifr->ifr_addr)->sin6_scope_id,
				 (int)if_nametoindex(ifr->ifr_name),
				 parm)))
	    goto err;
	}
#endif
    }
  
  ret = 1;

 err:
  errsav = errno;
  close(fd);  
  errno = errsav;

  return ret;
}

#endif
