/*
 * ntp.c
 * Copyright (C) 2009-2010 by ipoque GmbH
 * 
 * This file is part of OpenDPI, an open source deep packet inspection
 * library based on the PACE technology by ipoque GmbH
 * 
 * OpenDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenDPI.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include "ipq_protocols.h"
#ifdef IPOQUE_PROTOCOL_NTP

/* detection also works asymmetrically */

static void ipoque_search_ntp_udp(struct ipoque_detection_module_struct *ipoque_struct)
{
	struct ipoque_packet_struct *packet = &ipoque_struct->packet;
	struct ipoque_flow_struct *flow = ipoque_struct->flow;

	if (!(packet->udp->dest == htons(123) || packet->udp->source == htons(123)))
		goto exclude_ntp;

	IPQ_LOG(IPOQUE_PROTOCOL_NTP, ipoque_struct, IPQ_LOG_DEBUG, "NTP port detected\n");

	if (packet->payload_packet_len != 48)
		goto exclude_ntp;

	IPQ_LOG(IPOQUE_PROTOCOL_NTP, ipoque_struct, IPQ_LOG_DEBUG, "NTP length detected\n");


	if ((((packet->payload[0] & 0x38) >> 3) <= 4)) {
		IPQ_LOG(IPOQUE_PROTOCOL_NTP, ipoque_struct, IPQ_LOG_DEBUG, "detected NTP.");
		ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_NTP);
		return;
	}



  exclude_ntp:
	IPQ_LOG(IPOQUE_PROTOCOL_NTP, ipoque_struct, IPQ_LOG_DEBUG, "NTP excluded.\n");
	IPOQUE_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, IPOQUE_PROTOCOL_NTP);
}

#endif
