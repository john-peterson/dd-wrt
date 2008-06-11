/*
 * OLSR Basic Multicast Forwarding (BMF) plugin.
 * Copyright (c) 2005 - 2007, Thales Communications, Huizen, The Netherlands.
 * Written by Erik Tromp.
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
 * * Neither the name of Thales, BMF nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* -------------------------------------------------------------------------
 * File       : Bmf.c
 * Description: Multicast forwarding functions
 * Created    : 29 Jun 2006
 *
 * ------------------------------------------------------------------------- */

#define _MULTI_THREADED

#include "Bmf.h"

/* System includes */
#include <stddef.h> /* NULL */
#include <sys/types.h> /* ssize_t */
#include <string.h> /* strerror() */
#include <stdarg.h> /* va_list, va_start, va_end */
#include <errno.h> /* errno */
#include <assert.h> /* assert() */
#include <linux/if_ether.h> /* ETH_P_IP */
#include <linux/if_packet.h> /* struct sockaddr_ll, PACKET_MULTICAST */
#include <pthread.h> /* pthread_t, pthread_create() */
#include <signal.h> /* sigset_t, sigfillset(), sigdelset(), SIGINT */
#include <netinet/ip.h> /* struct ip */
#include <netinet/udp.h> /* struct udphdr */

/* OLSRD includes */
#include "plugin_util.h" /* set_plugin_int */
#include "defs.h" /* olsr_cnf, OLSR_PRINTF */
#include "ipcalc.h"
#include "olsr.h" /* olsr_printf */
#include "scheduler.h" /* olsr_start_timer() */
#include "mid_set.h" /* mid_lookup_main_addr() */
#include "mpr_selector_set.h" /* olsr_lookup_mprs_set() */
#include "link_set.h" /* get_best_link_to_neighbor() */
#include "net_olsr.h" /* ipequal */

/* BMF includes */
#include "NetworkInterfaces.h" /* TBmfInterface, CreateBmfNetworkInterfaces(), CloseBmfNetworkInterfaces() */
#include "Address.h" /* IsMulticast() */
#include "Packet.h" /* ENCAP_HDR_LEN, BMF_ENCAP_TYPE, BMF_ENCAP_LEN etc. */
#include "PacketHistory.h" /* InitPacketHistory() */

static pthread_t BmfThread;
static int BmfThreadRunning = 0;

/* unicast/broadcast fan out limit */
int FanOutLimit = 2;

int BroadcastRetransmitCount = 1;

/* -------------------------------------------------------------------------
 * Function   : BmfPError
 * Description: Prints an error message at OLSR debug level 1.
 *              First the plug-in name is printed. Then (if format is not NULL
 *              and *format is not empty) the arguments are printed, followed
 *              by a colon and a blank. Then the message and a new-line.
 * Input      : format, arguments
 * Output     : none
 * Return     : none
 * Data Used  : none
 * ------------------------------------------------------------------------- */
void BmfPError(const char* format, ...)
{
#define MAX_STR_DESC 255
  char* strErr = strerror(errno);
  char strDesc[MAX_STR_DESC];

  /* Rely on short-circuit boolean evaluation */
  if (format == NULL || *format == '\0')
  {
    olsr_printf(1, "%s: %s\n", PLUGIN_NAME, strErr);
  }
  else
  {
    va_list arglist;

    olsr_printf(1, "%s: ", PLUGIN_NAME);

    va_start(arglist, format);
    vsnprintf(strDesc, MAX_STR_DESC, format, arglist);
    va_end(arglist);

    strDesc[MAX_STR_DESC - 1] = '\0'; /* Ensures null termination */

    olsr_printf(1, "%s: %s\n", strDesc, strErr);
  }
} /* BmfPError */

/* -------------------------------------------------------------------------
 * Function   : MainAddressOf
 * Description: Lookup the main address of a node
 * Input      : ip - IP address of the node
 * Output     : none
 * Return     : The main IP address of the node
 * Data Used  : none
 * ------------------------------------------------------------------------- */
union olsr_ip_addr* MainAddressOf(union olsr_ip_addr* ip)
{
  union olsr_ip_addr* result;

  /* TODO: mid_lookup_main_addr() is not thread-safe! */
  result = mid_lookup_main_addr(ip);
  if (result == NULL)
  {
    result = ip;
  }
  return result;
} /* MainAddressOf */

/* -------------------------------------------------------------------------
 * Function   : EncapsulateAndForwardPacket
 * Description: Encapsulate a captured raw IP packet and forward it
 * Input      : intf - the network interface on which to forward the packet
 *              encapsulationUdpData - The encapsulation header, followed by
 *                the encapsulated IP packet
 * Output     : none
 * Return     : none
 * Data Used  : none
 * ------------------------------------------------------------------------- */
static void EncapsulateAndForwardPacket(
  struct TBmfInterface* intf,
  unsigned char* encapsulationUdpData)
{
  /* The packet */
  u_int16_t udpDataLen = GetEncapsulationUdpDataLength(encapsulationUdpData);

  /* The next destination(s) */
  struct TBestNeighbors bestNeighborLinks;
  struct link_entry* bestNeighbor;

  int nPossibleNeighbors = 0;
  struct sockaddr_in forwardTo; /* Next destination of encapsulation packet */
  int nPacketsToSend;
  int sendUnicast; /* 0 = send broadcast; 1 = send unicast */

  int i;

  /* Find at most 'FanOutLimit' best neigbors to forward the packet to */
  FindNeighbors(&bestNeighborLinks, &bestNeighbor, intf, NULL, NULL, NULL, &nPossibleNeighbors);

  if (nPossibleNeighbors <= 0)
  {
    OLSR_PRINTF(
      8,
      "%s: --> not encap-forwarding on \"%s\": there is no neighbor that needs my retransmission\n",
      PLUGIN_NAME_SHORT,
      intf->ifName);
    return;
  }

  /* Compose destination of encapsulation packet */

  memset(&forwardTo, 0, sizeof(forwardTo));
  forwardTo.sin_family = AF_INET;
  forwardTo.sin_port = htons(BMF_ENCAP_PORT);

  /* Start by filling in the local broadcast address. This may be overwritten later. */
  forwardTo.sin_addr = intf->broadAddr.v4;

  /* - If the BMF mechanism is BM_UNICAST_PROMISCUOUS, always send just one
   *   unicast packet (to the best neighbor).
   * - But if the BMF mechanism is BM_BROADCAST,
   *   - send 'nPossibleNeighbors' unicast packets if there are up to
   *     'FanOutLimit' possible neighbors,
   *   - if there are more than 'FanOutLimit' possible neighbors, then
   *     send a (WLAN-air-expensive, less reliable) broadcast packet. */
  if (BmfMechanism == BM_UNICAST_PROMISCUOUS)
  {
    /* One unicast packet to the best neighbor */
    nPacketsToSend = 1;
    sendUnicast = 1;
    bestNeighborLinks.links[0] = bestNeighbor;
  }
  else /* BmfMechanism == BM_BROADCAST */
  {
    if (nPossibleNeighbors <= FanOutLimit)
    {
      /* 'nPossibleNeighbors' unicast packets */
      nPacketsToSend = nPossibleNeighbors;
      sendUnicast = 1;
    }
    else /* nPossibleNeighbors > FanOutLimit */
    {
      /* One broadcast packet, possibly retransmitted as specified in the
       * 'BroadcastRetransmitCount' plugin parameter */
      nPacketsToSend = BroadcastRetransmitCount;
      sendUnicast = 0;
    } /* if */
  } /* if */

  for (i = 0; i < nPacketsToSend; i++)
  {
    int nBytesWritten;

    if (sendUnicast == 1)
    {
      /* For unicast, overwrite the local broadcast address which was filled in above */
      forwardTo.sin_addr = bestNeighborLinks.links[i]->neighbor_iface_addr.v4;
    }

    /* Forward the BMF packet via the encapsulation socket */
    nBytesWritten = sendto(
      intf->encapsulatingSkfd,
      encapsulationUdpData,
      udpDataLen,
      MSG_DONTROUTE,
      (struct sockaddr*) &forwardTo,
      sizeof(forwardTo));                   

    /* Evaluate and display result */
    if (nBytesWritten != udpDataLen)
    {
      BmfPError("sendto() error forwarding pkt on \"%s\"", intf->ifName);
    }
    else
    {
      /* Increase counter */
      intf->nBmfPacketsTx++;

      OLSR_PRINTF(
        8,
        "%s: --> encapsulated and forwarded on \"%s\" to %s\n",
        PLUGIN_NAME_SHORT,
        intf->ifName,
        inet_ntoa(forwardTo.sin_addr));
    } /* if (nBytesWritten != udpDataLen) */
  } /* for */
} /* EncapsulateAndForwardPacket */

/* -------------------------------------------------------------------------
 * Function   : BmfPacketCaptured
 * Description: Handle a captured IP packet
 * Input      : intf - the network interface on which the packet was captured
 *              sllPkttype - the type of packet. Either PACKET_OUTGOING,
 *                PACKET_BROADCAST or PACKET_MULTICAST.
 *              encapsulationUdpData - space for the encapsulation header, followed by
 *                the captured IP packet
 * Output     : none
 * Return     : none
 * Data Used  : BmfInterfaces
 * Notes      : The IP packet is assumed to be captured on a socket of family
 *              PF_PACKET and type SOCK_DGRAM (cooked).
 * ------------------------------------------------------------------------- */
static void BmfPacketCaptured(
  struct TBmfInterface* intf,
  unsigned char sllPkttype,
  unsigned char* encapsulationUdpData)
{
  union olsr_ip_addr src; /* Source IP address in captured packet */
  union olsr_ip_addr dst; /* Destination IP address in captured packet */
  union olsr_ip_addr* origIp; /* Main OLSR address of source of captured packet */
  struct TBmfInterface* walker;
  int isFromOlsrIntf;
  int isFromOlsrNeighbor;
  int iAmMpr;
  unsigned char* ipPacket; /* The captured IP packet... */
  u_int16_t ipPacketLen; /* ...and its length */
  struct ip* ipHeader; /* The IP header inside the captured IP packet */
  u_int32_t crc32;
  struct TEncapHeader* encapHdr;
#ifndef NODEBUG
  struct ipaddr_str srcBuf, dstBuf;
#endif
  ipHeader = GetIpHeader(encapsulationUdpData);

  dst.v4 = ipHeader->ip_dst;

  /* Only forward multicast packets. If configured, also forward local broadcast packets */
  if (IsMulticast(&dst) ||
      (EnableLocalBroadcast != 0 && ipequal(&dst, &intf->broadAddr)))
  {
    /* continue */
  }
  else
  {
    return;
  }

  ipPacket = GetIpPacket(encapsulationUdpData);

  /* Don't forward fragments of IP packets: there is no way to distinguish fragments
   * of BMF encapsulation packets from other fragments.
   * Well yes, there is the IP-ID, which can be kept in a list to relate a fragment
   * to earlier sent BMF packets, but... sometimes the second fragment comes in earlier
   * than the first fragment, so that the list is not yet up to date and the second
   * fragment is not recognized as a BMF packet.
   * Also, don't forward OLSR packets (UDP port 698) and BMF encapsulated packets */
  if (IsIpFragment(ipPacket) || IsOlsrOrBmfPacket(ipPacket))
  {
    return;
  }

  /* Increase counter */
  intf->nBmfPacketsRx++;

  /* Check if the frame is captured on an OLSR-enabled interface */
  isFromOlsrIntf = (intf->olsrIntf != NULL);

  /* Retrieve the length of the captured packet */
  ipPacketLen = GetIpTotalLength(ipPacket);

  src.v4 = ipHeader->ip_src;

  OLSR_PRINTF(
    8,
    "%s: %s pkt of %ld bytes captured on %s interface \"%s\": %s->%s\n",
    PLUGIN_NAME_SHORT,
    sllPkttype == PACKET_OUTGOING ? "outgoing" : "incoming",
    (long)ipPacketLen,
    isFromOlsrIntf ? "OLSR" : "non-OLSR",
    intf->ifName,
    olsr_ip_to_string(&srcBuf, &src),
    olsr_ip_to_string(&dstBuf, &dst));

  /* Lookup main address of source in the MID table of OLSR */
  origIp = MainAddressOf(&src);

  /* Calculate packet fingerprint */
  crc32 = PacketCrc32(ipPacket, ipPacketLen);

  /* Check if this packet was seen recently */
  if (CheckAndMarkRecentPacket(crc32))
  {
    /* Increase counter */
    intf->nBmfPacketsRxDup++;

    OLSR_PRINTF(
      8,
      "%s: --> discarding: packet is duplicate\n",
      PLUGIN_NAME_SHORT);
    return;
  }

  /* Compose encapsulation header */
  encapHdr = (struct TEncapHeader*) encapsulationUdpData;
  memset (encapHdr, 0, ENCAP_HDR_LEN);
  encapHdr->type = BMF_ENCAP_TYPE;
  encapHdr->len = BMF_ENCAP_LEN;
  encapHdr->reserved = 0;
  encapHdr->crc32 = htonl(crc32);

  /* Check if the frame is captured on an OLSR interface from an OLSR neighbor.
   * TODO1: get_best_link_to_neighbor() is not thread-safe.
   * TODO2: get_best_link_to_neighbor() may be very CPU-expensive, a simpler call
   * would do here (something like 'get_any_link_to_neighbor()'). */
  isFromOlsrNeighbor =
    (isFromOlsrIntf /* The frame is captured on an OLSR interface... */
    && get_best_link_to_neighbor(origIp) != NULL); /* ...from an OLSR neighbor */ 

  /* Check with OLSR if I am MPR for that neighbor */
  /* TODO: olsr_lookup_mprs_set() is not thread-safe! */
  iAmMpr = olsr_lookup_mprs_set(origIp) != NULL;

  /* Check with each network interface what needs to be done on it */
  for (walker = BmfInterfaces; walker != NULL; walker = walker->next)
  {
    /* Is the forwarding interface OLSR-enabled? */
    int isToOlsrIntf = (walker->olsrIntf != NULL);

    /* Depending on certain conditions, we decide whether or not to forward
     * the packet, and if it is forwarded, in which form (encapsulated
     * or not, TTL decreased or not). These conditions are:
     * - is the packet is coming in on an OLSR interface or not? (isFromOlsrIntf)
     * - is the packet going out on an OLSR interface or not? (isToOlsrIntf)
     * - if the packet if coming in on an OLSR interface:
     *   - is the node that forwarded the packet my OLSR-neighbor? (isFromOlsrNeighbor)
     *   - has the node that forwarded the packet selected me as MPR? (iAmMpr)
     *
     * Based on these conditions, the following cases can be distinguished:
     *
     * - Case 1: Packet coming in on an OLSR interface. What to
     *   do with it on an OLSR interface?
     *   Answer:
     *   - Case 1.1: If the forwarding node is an OLSR neighbor that has *not*
     *     selected me as MPR: don't forward the packet.
     *   - Case 1.2: If the forwarding node is an OLSR neighbor that has selected
     *     me as MPR: encapsulate and forward the packet.
     *   - Case 1.3: If the forwarding node is not an OLSR neighbor: encapsulate
     *     and forward the packet.
     *     NOTE: Case 1.3 is a special case. In the perfect world, we expect to
     *     see only OLSR-neighbors on OLSR-enabled interfaces. Sometimes, however,
     *     ignorant users will connect a host not running OLSR, to a LAN in
     *     which there are hosts running OLSR. Of course these ignorant users,
     *     expecting magic, want to see their multicast packets being forwarded
     *     through the network.
     *
     * - Case 2: Packet coming in on an OLSR interface. What to do with it on a
     *   non-OLSR interface?
     *   Answer: Forward it.
     *
     * - Case 3: Packet coming in on a non-OLSR interface. What to
     *   do with it on an OLSR interface?
     *   Answer: Encapsulate and forward it.
     *
     * - Case 4: Packet coming in on non-OLSR interface. What to do with it on a
     *   non-OLSR interface?
     *   Answer 1: nothing. Multicast routing between non-OLSR interfaces
     *   is to be done by other protocols (e.g. PIM, DVMRP).
     *   Answer 2 (better): Forward it.
     */

    if (isFromOlsrIntf && isToOlsrIntf)
    {
      /* Case 1: Forward from an OLSR interface to an OLSR interface */

      if (isFromOlsrNeighbor && !iAmMpr)
      {
        /* Case 1.1 */
        {
#ifndef NODEBUG
          struct ipaddr_str buf;
#endif
          OLSR_PRINTF(
            8,
            "%s: --> not encap-forwarding on \"%s\": I am not selected as MPR by neighbor %s\n",
            PLUGIN_NAME_SHORT,
            walker->ifName,
            olsr_ip_to_string(&buf, &src));
        }    
      }
      else if (sllPkttype == PACKET_OUTGOING && intf == walker)
      {
        OLSR_PRINTF(
          8,
          "%s: --> not encap-forwarding on \"%s\": pkt was captured on that interface\n",
          PLUGIN_NAME_SHORT,
          walker->ifName);
      }
      else
      {
        /* Case 1.2 and 1.3 */
        EncapsulateAndForwardPacket(walker, encapsulationUdpData);
      }
    } /* if (isFromOlsrIntf && isToOlsrIntf) */

    else if (isFromOlsrIntf && !isToOlsrIntf)
    {
      /* Case 2: Forward from OLSR interface to non-OLSR interface */

      int nBytesWritten;
      struct sockaddr_ll dest;

      /* If the encapsulated IP packet is a local broadcast packet,
       * update its destination address to match the subnet of the network
       * interface on which the packet is being sent. */
      CheckAndUpdateLocalBroadcast(ipPacket, &walker->broadAddr);

      memset(&dest, 0, sizeof(dest));
      dest.sll_family = AF_PACKET;
      dest.sll_protocol = htons(ETH_P_IP);
      dest.sll_ifindex = if_nametoindex(walker->ifName);
      dest.sll_halen = IFHWADDRLEN;

      /* Use all-ones as destination MAC address. When the IP destination is
       * a multicast address, the destination MAC address should normally also
       * be a multicast address. E.g., when the destination IP is 224.0.0.1,
       * the destination MAC should be 01:00:5e:00:00:01. However, it does not
       * seem to matter when the destination MAC address is set to all-ones
       * in that case. */
      memset(dest.sll_addr, 0xFF, IFHWADDRLEN);

      nBytesWritten = sendto(
        walker->capturingSkfd,
        ipPacket,
        ipPacketLen,
        0,
        (struct sockaddr*) &dest,
        sizeof(dest));
      if (nBytesWritten != ipPacketLen)
      {
        BmfPError("sendto() error forwarding pkt on \"%s\"", walker->ifName);
      }
      else
      {
        /* Increase counter */
        walker->nBmfPacketsTx++;

        OLSR_PRINTF(8, "%s: --> forwarded on \"%s\"\n", PLUGIN_NAME_SHORT, walker->ifName);
      }
    } /* else if (isFromOlsrIntf && !isToOlsrIntf) */

    else if (!isFromOlsrIntf && isToOlsrIntf)
    {
      /* Case 3: Forward from a non-OLSR interface to an OLSR interface.
       * Encapsulate and forward packet. */

      EncapsulateAndForwardPacket(walker, encapsulationUdpData);
    } /* else if (!isFromOlsrIntf && isToOlsrIntf) */

    else
    {
      /* Case 4: Forward from non-OLSR interface to non-OLSR interface. */

      /* Don't forward on interface on which packet was received */
      if (intf == walker)
      {
        OLSR_PRINTF(
          8,
          "%s: --> not forwarding on \"%s\": pkt was captured on that interface\n",
          PLUGIN_NAME_SHORT,
          walker->ifName);
      }

      else
      {
        int nBytesWritten;
        struct sockaddr_ll dest;

        /* If the encapsulated IP packet is a local broadcast packet,
         * update its destination address to match the subnet of the network
         * interface on which the packet is being sent. */
        CheckAndUpdateLocalBroadcast(ipPacket, &walker->broadAddr);

        memset(&dest, 0, sizeof(dest));
        dest.sll_family = AF_PACKET;
        dest.sll_protocol = htons(ETH_P_IP);
        dest.sll_ifindex = if_nametoindex(walker->ifName);
        dest.sll_halen = IFHWADDRLEN;

        /* Use all-ones as destination MAC address. When the IP destination is
         * a multicast address, the destination MAC address should normally also
         * be a multicast address. E.g., when the destination IP is 224.0.0.1,
         * the destination MAC should be 01:00:5e:00:00:01. However, it does not
         * seem to matter when the destination MAC address is set to all-ones
         * in that case. */
        memset(dest.sll_addr, 0xFF, IFHWADDRLEN);

        nBytesWritten = sendto(
          walker->capturingSkfd,
          ipPacket,
          ipPacketLen,
          0,
          (struct sockaddr*) &dest,
          sizeof(dest));
        if (nBytesWritten != ipPacketLen)
        {
          BmfPError("sendto() error forwarding pkt on \"%s\"", walker->ifName);
        }
        else
        {
          /* Increase counter */
          walker->nBmfPacketsTx++;

          OLSR_PRINTF(
            8,
            "%s: --> forwarded from non-OLSR on non-OLSR \"%s\"\n",
            PLUGIN_NAME_SHORT,
            walker->ifName);
        }
      } /* if (intf == walker) */
    } /* if */
  } /* for */
} /* BmfPacketCaptured */

/* -------------------------------------------------------------------------
 * Function   : BmfEncapsulationPacketReceived
 * Description: Handle a received BMF-encapsulation packet
 * Input      : intf - the network interface on which the packet was received
 *              forwardedBy - the IP node that forwarded the packet to me
 *              forwardedTo - the destination IP address of the encapsulation
 *                packet, in case the packet was received promiscuously.
 *                Pass NULL if the packet is received normally (unicast or
 *                broadcast).
 *              encapsulationUdpData - the encapsulating IP UDP data, containting
 *                the BMF encapsulation header, followed by the encapsulated
 *                IP packet
 * Output     : none
 * Return     : none
 * Data Used  : BmfInterfaces
 * ------------------------------------------------------------------------- */
static void BmfEncapsulationPacketReceived(
  struct TBmfInterface* intf,
  union olsr_ip_addr* forwardedBy,
  union olsr_ip_addr* forwardedTo,
  unsigned char* encapsulationUdpData)
{
  int iAmMpr; /* True (1) if I am selected as MPR by 'forwardedBy' */
  struct sockaddr_in forwardTo; /* Next destination of encapsulation packet */
  unsigned char* ipPacket; /* The encapsulated IP packet */
  u_int16_t ipPacketLen; /* Length of the encapsulated IP packet */
  struct ip* ipHeader; /* IP header inside the encapsulated IP packet */
  union olsr_ip_addr mcSrc; /* Original source of the encapsulated multicast packet */
  union olsr_ip_addr mcDst; /* Multicast destination of the encapsulated packet */
  struct TEncapHeader* encapsulationHdr;
  u_int16_t encapsulationUdpDataLen;
  struct TBmfInterface* walker;
#ifndef NODEBUG
  struct ipaddr_str mcSrcBuf, mcDstBuf, forwardedByBuf, forwardedToBuf;
#endif
  /* Are we talking to ourselves? */
  if (if_ifwithaddr(forwardedBy) != NULL)
  {
    return;
  }

  /* Discard encapsulated packets received on a non-OLSR interface */
  if (intf->olsrIntf == NULL)
  {
    return;
  }

  /* Retrieve details about the encapsulated IP packet */
  ipPacket = GetIpPacket(encapsulationUdpData);
  ipPacketLen = GetIpTotalLength(ipPacket);
  ipHeader = GetIpHeader(encapsulationUdpData);

  mcSrc.v4 = ipHeader->ip_src;
  mcDst.v4 = ipHeader->ip_dst;

  /* Increase counter */
  intf->nBmfPacketsRx++;

  /* Beware: not possible to call olsr_ip_to_string more than 4 times in same printf */
  OLSR_PRINTF(
    8,
    "%s: encapsulated pkt of %ld bytes incoming on \"%s\": %s->%s, forwarded by %s to %s\n",
    PLUGIN_NAME_SHORT,
    (long)ipPacketLen,
    intf->ifName,
    olsr_ip_to_string(&mcSrcBuf, &mcSrc),
    olsr_ip_to_string(&mcDstBuf, &mcDst),
    olsr_ip_to_string(&forwardedByBuf, forwardedBy),
    forwardedTo != NULL ? olsr_ip_to_string(&forwardedToBuf, forwardedTo) : "me");

  /* Get encapsulation header */
  encapsulationHdr = (struct TEncapHeader*) encapsulationUdpData;

  /* Verify correct format of BMF encapsulation header */
  if (encapsulationHdr->type != BMF_ENCAP_TYPE ||
      encapsulationHdr->len != BMF_ENCAP_LEN ||
      ntohs(encapsulationHdr->reserved != 0))
  {
    OLSR_PRINTF(
      8,
      "%s: --> discarding: format of BMF encapsulation header not recognized\n",
      PLUGIN_NAME_SHORT);
    return;
  }

  /* Check if this packet was seen recently */
  if (CheckAndMarkRecentPacket(ntohl(encapsulationHdr->crc32)))
  {
    /* Increase counter */
    intf->nBmfPacketsRxDup++;

    OLSR_PRINTF(
      8,
      "%s: --> discarding: packet is duplicate\n",
      PLUGIN_NAME_SHORT);
    return;
  }

  if (EtherTunTapFd >= 0)
  {
    /* Unpack the encapsulated IP packet and deliver it locally, by sending
     * a copy into the local IP stack via the EtherTunTap interface */

    union olsr_ip_addr broadAddr;
    int nBytesToWrite, nBytesWritten;
    unsigned char* bufferToWrite;

    /* If the encapsulated IP packet is a local broadcast packet,
     * update its destination address to match the subnet of the EtherTunTap
     * interface */
    broadAddr.v4.s_addr = htonl(EtherTunTapIpBroadcast);
    CheckAndUpdateLocalBroadcast(ipPacket, &broadAddr);

    bufferToWrite = ipPacket;
    nBytesToWrite = ipPacketLen;

    /* Write the packet into the EtherTunTap interface for local delivery */
    nBytesWritten = write(EtherTunTapFd, bufferToWrite, nBytesToWrite);
    if (nBytesWritten != nBytesToWrite)
    {
      BmfPError("write() error forwarding encapsulated pkt on \"%s\"", EtherTunTapIfName);
    }
    else
    {
      OLSR_PRINTF(
        8,
        "%s: --> unpacked and delivered locally on \"%s\"\n",
        PLUGIN_NAME_SHORT,
        EtherTunTapIfName);
    }
  } /* if (EtherTunTapFd >= 0) */

  /* Check if I am MPR for the forwarder */
  /* TODO: olsr_lookup_mprs_set() is not thread-safe! */
  iAmMpr = (olsr_lookup_mprs_set(MainAddressOf(forwardedBy)) != NULL);

  /* Compose destination address for next hop */
  memset(&forwardTo, 0, sizeof(forwardTo));
  forwardTo.sin_family = AF_INET;
  forwardTo.sin_port = htons(BMF_ENCAP_PORT);

  /* Retrieve the number of bytes to be forwarded via the encapsulation socket */
  encapsulationUdpDataLen = GetEncapsulationUdpDataLength(encapsulationUdpData);

  /* Check with each network interface what needs to be done on it */
  for (walker = BmfInterfaces; walker != NULL; walker = walker->next)
  {
    /* What to do with the packet on a non-OLSR interface? Unpack
     * encapsulated packet, and forward it.
     *
     * What to do with the packet on an OLSR interface? Forward it only
     * if the forwarding node has selected us as MPR (iAmMpr).
     *
     * Note that the packet is always coming in on an OLSR interface, because
     * it is an encapsulated BMF packet. */

    /* To a non-OLSR interface: unpack the encapsulated IP packet and forward it */
    if (walker->olsrIntf == NULL)
    {
      int nBytesWritten;
      struct sockaddr_ll dest;

      /* If the encapsulated IP packet is a local broadcast packet,
       * update its destination address to match the subnet of the network
       * interface on which the packet is being sent. */
      CheckAndUpdateLocalBroadcast(ipPacket, &walker->broadAddr);

      memset(&dest, 0, sizeof(dest));
      dest.sll_family = AF_PACKET;
      dest.sll_protocol = htons(ETH_P_IP);
      dest.sll_ifindex = if_nametoindex(walker->ifName);
      dest.sll_halen = IFHWADDRLEN;

      /* Use all-ones as destination MAC address. When the IP destination is
       * a multicast address, the destination MAC address should normally also
       * be a multicast address. E.g., when the destination IP is 224.0.0.1,
       * the destination MAC should be 01:00:5e:00:00:01. However, it does not
       * seem to matter when the destination MAC address is set to all-ones
       * in that case. */
      memset(dest.sll_addr, 0xFF, IFHWADDRLEN);

      nBytesWritten = sendto(
        walker->capturingSkfd,
        ipPacket,
        ipPacketLen,
        0,
        (struct sockaddr*) &dest,
        sizeof(dest));
      if (nBytesWritten != ipPacketLen)
      {
        BmfPError("sendto() error forwarding unpacked encapsulated pkt on \"%s\"", walker->ifName);
      }
      else
      {
        /* Increase counter */
        walker->nBmfPacketsTx++;

        OLSR_PRINTF(
          8,
          "%s: --> unpacked and forwarded on \"%s\"\n",
          PLUGIN_NAME_SHORT,
          walker->ifName);
      }
    } /* if (walker->olsrIntf == NULL) */

    /* To an OLSR interface: forward the packet, but only if this node is
     * selected as MPR by the forwarding node */
    else if (iAmMpr)
    {
      struct TBestNeighbors bestNeighborLinks;
      struct link_entry* bestNeighbor;
      int nPossibleNeighbors;
      int nPacketsToSend;
      int sendUnicast; /* 0 = send broadcast; 1 = send unicast */
      int i;

      /* Retrieve at most two best neigbors to forward the packet to */
      FindNeighbors(
        &bestNeighborLinks,
        &bestNeighbor,
        walker,
        &mcSrc,
        forwardedBy,
        forwardedTo,
        &nPossibleNeighbors);

      if (nPossibleNeighbors <= 0)
      {
        OLSR_PRINTF(
          8,
          "%s: --> not forwarding on \"%s\": there is no neighbor that needs my retransmission\n",
          PLUGIN_NAME_SHORT,
          walker->ifName);

        continue; /* for */
      }

      /* Compose destination of encapsulation packet.
       * Start by filling in the local broadcast address. This may be overwritten later. */
      forwardTo.sin_addr = walker->broadAddr.v4;

      /* - If the BMF mechanism is BM_UNICAST_PROMISCUOUS, always send just one
       *   unicast packet (to the best neighbor).
       * - But if the BMF mechanism is BM_BROADCAST,
       *   - send 'nPossibleNeighbors' unicast packets if there are up to
       *     'FanOutLimit' possible neighbors,
       *   - if there are more than 'FanOutLimit' possible neighbors, then
       *     send a (WLAN-air-expensive, less reliable) broadcast packet. */
      if (BmfMechanism == BM_UNICAST_PROMISCUOUS)
      {
        /* One unicast packet to the best neighbor */
        nPacketsToSend = 1;
        sendUnicast = 1;
        bestNeighborLinks.links[0] = bestNeighbor;
      }
      else /* BmfMechanism == BM_BROADCAST */
      {
        if (nPossibleNeighbors <= FanOutLimit)
        {
          /* 'nPossibleNeighbors' unicast packets */
          nPacketsToSend = nPossibleNeighbors;
          sendUnicast = 1;
        }
        else /* nPossibleNeighbors > FanOutLimit */
        {
          /* One broadcast packet, possibly retransmitted as specified in the
           * 'BroadcastRetransmitCount' plugin parameter */
          nPacketsToSend = BroadcastRetransmitCount;
          sendUnicast = 0;
        } /* if */
      } /* if */

      for (i = 0; i < nPacketsToSend; i++)
      {
        int nBytesWritten;

        if (sendUnicast)
        {    
          /* For unicast, overwrite the local broadcast address which was filled in above */
          forwardTo.sin_addr = bestNeighborLinks.links[i]->neighbor_iface_addr.v4;
        }

        /* Forward the BMF packet via the encapsulation socket */
        nBytesWritten = sendto(
          walker->encapsulatingSkfd,
          encapsulationUdpData,
          encapsulationUdpDataLen,
          MSG_DONTROUTE,
          (struct sockaddr*) &forwardTo,
          sizeof(forwardTo));                   

        /* Evaluate and display result */
        if (nBytesWritten != encapsulationUdpDataLen)
        {
          BmfPError("sendto() error forwarding encapsulated pkt on \"%s\"", walker->ifName);
        }
        else
        {
          /* Increase counter */
          walker->nBmfPacketsTx++;

          OLSR_PRINTF(
            8,
            "%s: --> forwarded on \"%s\" to %s\n",
            PLUGIN_NAME_SHORT,
            walker->ifName,
            inet_ntoa(forwardTo.sin_addr));
        } /* if */
      } /* for */
    }  /* else if (iAmMpr) */

    else /* walker->olsrIntf != NULL && !iAmMpr */
    {
#ifndef NODEBUG
      struct ipaddr_str buf;
#endif
      /* 'walker' is an OLSR interface, but I am not selected as MPR. In that
       * case, don't forward. */
      OLSR_PRINTF(
        8,
        "%s: --> not forwarding on \"%s\": I am not selected as MPR by %s\n",
        PLUGIN_NAME_SHORT,
        walker->ifName,
        olsr_ip_to_string(&buf, forwardedBy));
    } /* else */
  } /* for */
} /* BmfEncapsulationPacketReceived */

/* -------------------------------------------------------------------------
 * Function   : BmfTunPacketCaptured
 * Description: Handle an IP packet, captured outgoing on the tuntap interface
 * Input      : encapsulationUdpData - space for the encapsulation header, followed by
 *                the captured outgoing IP packet
 * Output     : none
 * Return     : none
 * Data Used  : none
 * Notes      : The packet is assumed to be captured on a socket of family
 *              PF_PACKET and type SOCK_DGRAM (cooked).
 * ------------------------------------------------------------------------- */
static void BmfTunPacketCaptured(unsigned char* encapsulationUdpData)
{
  union olsr_ip_addr srcIp;
  union olsr_ip_addr dstIp;
  union olsr_ip_addr broadAddr;
  struct TBmfInterface* walker;
  unsigned char* ipPacket;
  u_int16_t ipPacketLen;
  struct ip* ipHeader;
  u_int32_t crc32;
  struct TEncapHeader* encapHdr;
#ifndef NODEBUG
  struct ipaddr_str srcIpBuf, dstIpBuf;
#endif
  ipPacket = GetIpPacket(encapsulationUdpData);
  ipPacketLen = GetIpTotalLength(ipPacket);
  ipHeader = GetIpHeader(encapsulationUdpData);

  dstIp.v4 = ipHeader->ip_dst;
  broadAddr.v4.s_addr = htonl(EtherTunTapIpBroadcast);

  /* Only forward multicast packets. If configured, also forward local broadcast packets */
  if (IsMulticast(&dstIp) ||
      (EnableLocalBroadcast != 0 && ipequal(&dstIp, &broadAddr)))
  {
    /* continue */
  }
  else
  {
    return;
  }

  srcIp.v4 = ipHeader->ip_src;

  OLSR_PRINTF(
    8,
    "%s: outgoing pkt of %ld bytes captured on tuntap interface \"%s\": %s->%s\n",
    PLUGIN_NAME_SHORT,
    (long)ipPacketLen,
    EtherTunTapIfName,
    olsr_ip_to_string(&srcIpBuf, &srcIp),
    olsr_ip_to_string(&dstIpBuf, &dstIp));

  /* Calculate packet fingerprint */
  crc32 = PacketCrc32(ipPacket, ipPacketLen);

  /* Check if this packet was seen recently */
  if (CheckAndMarkRecentPacket(crc32))
  {
    OLSR_PRINTF(
      8,
      "%s: --> discarding: packet is duplicate\n",
      PLUGIN_NAME_SHORT);
    return;
  }

  /* Compose encapsulation header */
  encapHdr = (struct TEncapHeader*) encapsulationUdpData;
  memset (encapHdr, 0, ENCAP_HDR_LEN);
  encapHdr->type = BMF_ENCAP_TYPE;
  encapHdr->len = BMF_ENCAP_LEN;
  encapHdr->reserved = 0;
  encapHdr->crc32 = htonl(crc32);

  /* Check with each network interface what needs to be done on it */
  for (walker = BmfInterfaces; walker != NULL; walker = walker->next)
  {
    /* Is the forwarding interface OLSR-enabled? */
    if (walker->olsrIntf != NULL)
    {
      /* On an OLSR interface: encapsulate and forward packet. */

      EncapsulateAndForwardPacket(walker, encapsulationUdpData);
    }
    else
    {
      /* On a non-OLSR interface: what to do?
       * Answer 1: nothing. Multicast routing between non-OLSR interfaces
       * is to be done by other protocols (e.g. PIM, DVMRP).
       * Answer 2 (better): Forward it. */

      int nBytesWritten;
      struct sockaddr_ll dest;

      /* If the encapsulated IP packet is a local broadcast packet,
       * update its destination address to match the subnet of the network
       * interface on which the packet is being sent. */
      CheckAndUpdateLocalBroadcast(ipPacket, &walker->broadAddr);

      memset(&dest, 0, sizeof(dest));
      dest.sll_family = AF_PACKET;
      dest.sll_protocol = htons(ETH_P_IP);
      dest.sll_ifindex = if_nametoindex(walker->ifName);
      dest.sll_halen = IFHWADDRLEN;

      /* Use all-ones as destination MAC address. When the IP destination is
       * a multicast address, the destination MAC address should normally also
       * be a multicast address. E.g., when the destination IP is 224.0.0.1,
       * the destination MAC should be 01:00:5e:00:00:01. However, it does not
       * seem to matter when the destination MAC address is set to all-ones
       * in that case. */
      memset(dest.sll_addr, 0xFF, IFHWADDRLEN);

      nBytesWritten = sendto(
        walker->capturingSkfd,
        ipPacket,
        ipPacketLen,
        0,
        (struct sockaddr*) &dest,
        sizeof(dest));
      if (nBytesWritten != ipPacketLen)
      {
        BmfPError("sendto() error forwarding pkt on \"%s\"", walker->ifName);
      }
      else
      {
        /* Increase counter */
        walker->nBmfPacketsTx++;

        OLSR_PRINTF(
          8,
          "%s: --> forwarded from non-OLSR to non-OLSR \"%s\"\n",
          PLUGIN_NAME_SHORT,
          walker->ifName);
      } /* if */
    } /* if */
  } /* for */
} /* BmfTunPacketCaptured */

/* -------------------------------------------------------------------------
 * Function   : DoBmf
 * Description: Wait (blocking) for IP packets, then call the handler for each
 *              received packet
 * Input      : none
 * Output     : none
 * Return     : none
 * Data Used  : BmfInterfaces
 * ------------------------------------------------------------------------- */
static void DoBmf(void)
{
  int nFdBitsSet;
  unsigned char rxBuffer[BMF_BUFFER_SIZE];
  fd_set rxFdSet;

  assert(HighestSkfd >= 0);

  /* Make a local copy of the set of file descriptors that select() can
   * modify to indicate which descriptors actually changed status */
  rxFdSet = InputSet;

  /* Wait (blocking) for packets received on any of the sockets.
   * NOTE: don't use a timeout (last parameter). It causes a high system CPU load! */
  nFdBitsSet = select(HighestSkfd + 1, &rxFdSet, NULL, NULL, NULL);
  if (nFdBitsSet < 0)
  {
    if (errno != EINTR)
    {
      BmfPError("select() error");
    }
    return;
  }

  while (nFdBitsSet > 0)
  {
    struct TBmfInterface* walker;

    /* Check if a packet was received on the capturing socket (if any)
     * of each network interface */
    for (walker = BmfInterfaces; walker != NULL; walker = walker->next)
    {
      int skfd = walker->capturingSkfd;
      if (skfd >= 0 && (FD_ISSET(skfd, &rxFdSet)))
      {
        struct sockaddr_ll pktAddr;
        socklen_t addrLen = sizeof(pktAddr);
        int nBytes;
        unsigned char* ipPacket;

        /* A packet was captured. */

        nFdBitsSet--;

        /* Receive the captured Ethernet frame, leaving space for the BMF
         * encapsulation header */
        ipPacket = GetIpPacket(rxBuffer);
        nBytes = recvfrom(
          skfd,
          ipPacket,
          BMF_BUFFER_SIZE - ENCAP_HDR_LEN,
          0,
          (struct sockaddr*)&pktAddr,
          &addrLen);
        if (nBytes < 0)
        {
          BmfPError("recvfrom() error on \"%s\"", walker->ifName);

          continue; /* for */
        } /* if (nBytes < 0) */

        /* Check if the number of received bytes is large enough for an IP
         * packet which contains at least a minimum-size IP header.
         * Note: There is an apparent bug in the packet socket implementation in
         * combination with VLAN interfaces. On a VLAN interface, the value returned
         * by 'recvfrom' may (but need not) be 4 (bytes) larger than the value
         * returned on a non-VLAN interface, for the same ethernet frame. */
        if (nBytes < (int)sizeof(struct ip))
        {
          olsr_printf(
            1,
            "%s: captured frame too short (%d bytes) on \"%s\"\n",
            PLUGIN_NAME,
            nBytes,
            walker->ifName);

          continue; /* for */
        }

        if (pktAddr.sll_pkttype == PACKET_OUTGOING ||
            pktAddr.sll_pkttype == PACKET_MULTICAST ||
            pktAddr.sll_pkttype == PACKET_BROADCAST)
        {
          /* A multicast or broadcast packet was captured */

          BmfPacketCaptured(walker, pktAddr.sll_pkttype, rxBuffer);

        } /* if (pktAddr.sll_pkttype == ...) */
      } /* if (skfd >= 0 && (FD_ISSET...)) */
    } /* for */
    
    /* Check if a BMF encapsulation packet was received on the listening
     * socket (if any) of each network interface */
    for (walker = BmfInterfaces; walker != NULL; walker = walker->next)
    {
      int skfd = walker->listeningSkfd;
      if (skfd >= 0 && (FD_ISSET(skfd, &rxFdSet)))
      {
        struct sockaddr_ll pktAddr;
        socklen_t addrLen = sizeof(pktAddr);
        int nBytes;
        int minimumLength;
        struct ip* ipHeader;
        struct udphdr* udpHeader;
        u_int16_t destPort;
        union olsr_ip_addr forwardedBy;
        union olsr_ip_addr forwardedTo;

        /* Heard a BMF packet */

        nFdBitsSet--;

        nBytes = recvfrom(
          skfd,
          rxBuffer,
          BMF_BUFFER_SIZE,
          0,
          (struct sockaddr*)&pktAddr,
          &addrLen);
        if (nBytes < 0)
        {
          BmfPError("recvfrom() error on \"%s\"", walker->ifName);

          continue; /* for */
        } /* if (nBytes < 0) */

        /* Check if the received packet is actually directed to another
         * node on the LAN */
        if (pktAddr.sll_pkttype != PACKET_OTHERHOST)
        {
          /* No, the packet is directed to this node. In that case it will
           * be, or will already have been received, via the encapsulating
           * socket. Discard it here. */
          continue; /* for */
        } /* if (pktAddr.sll_pkttype ...) */

        /* Check if the received packet is UDP - BMF port */
        ipHeader = (struct ip*)rxBuffer;
        if (ipHeader->ip_p != SOL_UDP)
        {
          /* Not UDP */
          continue; /* for */
        }

        udpHeader = (struct udphdr*)(rxBuffer + GetIpHeaderLength(rxBuffer));
        destPort = ntohs(udpHeader->dest);
        if (destPort != BMF_ENCAP_PORT)
        {
          /* Not BMF */
          continue; /* for */
        }

        /* Check if the number of received bytes is large enough for a minimal BMF
         * encapsulation packet, at least:
         * - the IP header of the encapsulation IP packet
         * - the UDP header of the encapsulation IP packet
         * - the encapsulation header
         * - a minimum IP header inside the encapsulated packet
         * Note: on a VLAN interface, the value returned by 'recvfrom' may (but need
         * not) be 4 (bytes) larger than the value returned on a non-VLAN interface, for
         * the same ethernet frame. */
        minimumLength =
          GetIpHeaderLength(rxBuffer) +
          sizeof(struct udphdr) +
          ENCAP_HDR_LEN +
          sizeof(struct ip);
        if (nBytes < minimumLength)
        {
          olsr_printf(
            1,
            "%s: captured a too short encapsulation packet (%d bytes) on \"%s\"\n",
            PLUGIN_NAME,
            nBytes,
            walker->ifName);

          continue; /* for */
        }

        forwardedBy.v4 = ipHeader->ip_src;
        forwardedTo.v4 = ipHeader->ip_dst;
        BmfEncapsulationPacketReceived(
          walker,
          &forwardedBy,
          &forwardedTo,
          rxBuffer + GetIpHeaderLength(rxBuffer) + sizeof(struct udphdr));

      } /* if (skfd >= 0 && (FD_ISSET...)) */
    } /* for */

    /* Check if a packet was received on the encapsulating socket (if any)
     * of each network interface */
    for (walker = BmfInterfaces; walker != NULL; walker = walker->next)
    {
      int skfd = walker->encapsulatingSkfd;
      if (skfd >= 0 && (FD_ISSET(skfd, &rxFdSet)))
      {
        struct sockaddr_in from;
        socklen_t fromLen = sizeof(from);
        int nBytes;
        int minimumLength;
        union olsr_ip_addr forwardedBy;

        /* An encapsulated packet was received */

        nFdBitsSet--;

        nBytes = recvfrom(
          skfd,
          rxBuffer,
          BMF_BUFFER_SIZE,
          0,
          (struct sockaddr*)&from,
          &fromLen);
        if (nBytes < 0)
        {
          BmfPError("recvfrom() error on \"%s\"", walker->ifName);

          continue; /* for */
        } /* if (nBytes < 0) */

        forwardedBy.v4 = from.sin_addr;

        /* Check if the number of received bytes is large enough for a minimal BMF
         * encapsulation packet, at least:
         * - the encapsulation header
         * - a minimum IP header inside the encapsulated packet */
        minimumLength =
          ENCAP_HDR_LEN +
          sizeof(struct ip);
        if (nBytes < minimumLength)
        {
          struct ipaddr_str buf;
          olsr_printf(
            1,
            "%s: received a too short encapsulation packet (%d bytes) from %s on \"%s\"\n",
            PLUGIN_NAME,
            nBytes,
            olsr_ip_to_string(&buf, &forwardedBy),
            walker->ifName);

          continue; /* for */
        }

        /* Unfortunately, the recvfrom call does not return the destination
         * of the encapsulation packet (the destination may be either the
         * my unicast or my local broadcast address). Therefore we fill in 'NULL'
         * for the 'forwardedTo' parameter. */
        BmfEncapsulationPacketReceived(walker, &forwardedBy, NULL, rxBuffer);

      } /* if (skfd >= 0 && (FD_ISSET...)) */
    } /* for */

    if (nFdBitsSet > 0 && FD_ISSET(EtherTunTapFd, &rxFdSet))
    {
      /* Check if an application has sent a packet out via the tuntap
       * network interface */

      int nBytes;
      unsigned char* ipPacket;
      unsigned char* bufferToRead;
      size_t nBytesToRead;

      nFdBitsSet--;

      /* Receive the packet, leaving space for the BMF encapsulation header */
      ipPacket = GetIpPacket(rxBuffer);
    
      bufferToRead = ipPacket;
      nBytesToRead = BMF_BUFFER_SIZE - ENCAP_HDR_LEN;

      nBytes = read(EtherTunTapFd, bufferToRead, nBytesToRead);

      if (nBytes < 0)
      {
        BmfPError("recvfrom() error on \"%s\"", EtherTunTapIfName);
      }
      else
      {
        /* Check if the number of received bytes is large enough for an IP
         * packet which contains at least a minimum-size IP header */
        if (nBytes < (int)sizeof(struct ip))
        {
          olsr_printf(
            1,
            "%s: captured packet too short (%d bytes) on \"%s\"\n",
            PLUGIN_NAME,
            nBytes,
            EtherTunTapIfName);
        }
        else
        {
          /* An outbound packet was captured */

          BmfTunPacketCaptured(rxBuffer);

        } /* if (nBytes < ... */
      } /* if (nBytes < 0) */
    } /* if (nFdBitsSet > 0 && ... */
  } /* while (nFdBitsSet > 0) */
} /* DoBmf */

/* -------------------------------------------------------------------------
 * Function   : BmfSignalHandler
 * Description: Signal handler function
 * Input      : signo - signal being handled
 * Output     : none
 * Return     : none
 * Data Used  : BmfThreadRunning
 * ------------------------------------------------------------------------- */
static void BmfSignalHandler(int signo __attribute__((unused)))
{
  BmfThreadRunning = 0;
} /* BmfSignalHandler */

/* -------------------------------------------------------------------------
 * Function   : BmfRun
 * Description: Receiver thread function
 * Input      : useless - not used
 * Output     : none
 * Return     : not used
 * Data Used  : BmfThreadRunning
 * Notes      : Another thread can gracefully stop this thread by sending
 *              a SIGALRM signal.
 * ------------------------------------------------------------------------- */
static void* BmfRun(void* useless __attribute__((unused)))
{
  /* Mask all signals except SIGALRM */
  sigset_t blockedSigs;
  sigfillset(&blockedSigs);
  sigdelset(&blockedSigs, SIGALRM);
  if (pthread_sigmask(SIG_BLOCK, &blockedSigs, NULL) != 0)
  {
    BmfPError("pthread_sigmask() error");
  }

  /* Set up the signal handler for the process: use SIGALRM to terminate
   * the BMF thread. Only if a signal handler is specified, does a blocking
   * system call return with errno set to EINTR; if a signal hander is not
   * specified, any system call in which the thread may be waiting will not
   * return. Note that the BMF thread is usually blocked in the select()
   * function (see DoBmf()). */
  if (signal(SIGALRM, BmfSignalHandler) == SIG_ERR)
  {
    BmfPError("signal() error");
  }

  /* Call the thread function until flagged to exit */
  while (BmfThreadRunning != 0)
  {
    DoBmf();
  }

  return NULL;
} /* BmfRun */

/* -------------------------------------------------------------------------
 * Function   : InterfaceChange
 * Description: Callback function passed to OLSRD for it to call whenever a
 *              network interface has been added, removed or updated
 * Input      : interf - the network interface to deal with
 *              action - indicates if the specified network interface was
 *                added, removed or updated.
 * Output     : none
 * Return     : always 0
 * Data Used  : none
 * ------------------------------------------------------------------------- */
int InterfaceChange(struct interface* interf, int action)
{
  switch (action)
  {
  case (IFCHG_IF_ADD):
    AddInterface(interf);
    olsr_printf(1, "%s: interface %s added\n", PLUGIN_NAME, interf->int_name);
    break;

  case (IFCHG_IF_REMOVE):
    /* We cannot just remove the interface, because the receive-thread is likely
     * to be waiting in select(...) for packets coming in via the interface.
     * Therefore we first close BMF (CloseBmf()), interrupting and kiling the
     * receive-thread so that it is safe to remove this (and all other)
     * interfaces. After that, BMF is re-started (InitBmf(interf)). */
    CloseBmf();
    InitBmf(interf);
    olsr_printf(1, "%s: interface %s removed\n", PLUGIN_NAME, interf->int_name);
    break;

  case (IFCHG_IF_UPDATE):
    olsr_printf(1, "%s: interface %s updated\n", PLUGIN_NAME, interf->int_name);
    break;
      
  default:
    olsr_printf(
      1,
      "%s: interface %s: error - unknown action (%d)\n",
      PLUGIN_NAME,
      interf->int_name, action);
    break;
  }

  return 0;
} /* InterfaceChange */


/* -------------------------------------------------------------------------
 * Function   : SetFanOutLimit
 * Description: Overrule the default fan out limit value (2)
 * Input      : value - fan out limit value (1...MAX_UNICAST_NEIGHBORS)
 *              data - not used
 *              addon - not used
 * Output     : none
 * Return     : success (0) or fail (1)
 * Data Used  : FanOutLimit
 * ------------------------------------------------------------------------- */
int SetFanOutLimit(
  const char* value,
  void* data __attribute__((unused)),
  set_plugin_parameter_addon addon __attribute__((unused)))
{
  if (set_plugin_int(value, &FanOutLimit, addon) == 0)
  {
    /* Extra check if within range */
    if (FanOutLimit >= 1 && FanOutLimit <= MAX_UNICAST_NEIGHBORS)
    {
      return 0;
    }
  }
  return 1;
}

/* -------------------------------------------------------------------------
 * Function   : InitBmf
 * Description: Initialize the BMF plugin
 * Input      : skipThisIntf - specifies which network interface should not
 *              be enabled for BMF. Pass NULL if not applicable.
 * Output     : none
 * Return     : fail (0) or success (1)
 * Data Used  : BmfThreadRunning, BmfThread
 * ------------------------------------------------------------------------- */
int InitBmf(struct interface* skipThisIntf)
{
  CreateBmfNetworkInterfaces(skipThisIntf);

  /* Start running the multicast packet processing thread */
  BmfThreadRunning = 1;
  if (pthread_create(&BmfThread, NULL, BmfRun, NULL) != 0)
  {
    BmfPError("pthread_create() error");
    return 0;
  }

  if (EtherTunTapFd >= 0)
  {
    /* Deactivate IP spoof filter for EtherTunTap interface */
    DeactivateSpoofFilter();

    /* If the BMF network interface has a sensible IP address, it is a good idea
     * to route all multicast traffic through that interface */
    if (EtherTunTapIp != ETHERTUNTAPDEFAULTIP)
    {
      AddMulticastRoute();
    }
  }

  return 1;
} /* InitBmf */

/* -------------------------------------------------------------------------
 * Function   : CloseBmf
 * Description: Close the BMF plugin and clean up
 * Input      : none
 * Output     : none
 * Return     : none
 * Data Used  : BmfThread
 * ------------------------------------------------------------------------- */
void CloseBmf(void)
{
  if (EtherTunTapFd >= 0)
  {
    /* If there is a multicast route, try to delete it first */
    DeleteMulticastRoute();

    /* Restore IP spoof filter for EtherTunTap interface */
    RestoreSpoofFilter();
  }

  if (BmfThreadRunning)
  {
    /* Signal BmfThread to exit */
    /* Strangely enough, all running threads receive the SIGALRM signal. But only the
     * BMF thread is affected by this signal, having specified a handler for this
     * signal in its thread entry function BmfRun(...). */
    if (pthread_kill(BmfThread, SIGALRM) != 0)
    {
      BmfPError("pthread_kill() error");
    }

    /* Wait for BmfThread to acknowledge */
    if (pthread_join(BmfThread, NULL) != 0)
    {
      BmfPError("pthread_join() error");
    }
  }

  /* Clean up after the BmfThread has been killed */
  CloseBmfNetworkInterfaces();
} /* CloseBmf */

