/*
 * kontiki.c
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
#ifdef IPOQUE_PROTOCOL_KONTIKI

static void ipoque_int_kontiki_add_connection(struct ipoque_detection_module_struct
											  *ipoque_struct)
{

	struct ipoque_packet_struct *packet = &ipoque_struct->packet;
	struct ipoque_flow_struct *flow = ipoque_struct->flow;
	struct ipoque_id_struct *src = ipoque_struct->src;
	struct ipoque_id_struct *dst = ipoque_struct->dst;

	flow->detected_protocol = IPOQUE_PROTOCOL_KONTIKI;
	packet->detected_protocol = IPOQUE_PROTOCOL_KONTIKI;

	if (src != NULL) {
		IPOQUE_ADD_PROTOCOL_TO_BITMASK(src->detected_protocol_bitmask, IPOQUE_PROTOCOL_KONTIKI);
	}
	if (dst != NULL) {
		IPOQUE_ADD_PROTOCOL_TO_BITMASK(dst->detected_protocol_bitmask, IPOQUE_PROTOCOL_KONTIKI);
	}
}

void ipoque_search_kontiki(struct ipoque_detection_module_struct *ipoque_struct)
{
	struct ipoque_packet_struct *packet = &ipoque_struct->packet;
	struct ipoque_flow_struct *flow = ipoque_struct->flow;
//      struct ipoque_id_struct         *src=ipoque_struct->src;
//      struct ipoque_id_struct         *dst=ipoque_struct->dst;


	if (packet->payload_packet_len == 4 && (get_u32(packet->payload, 0) == htonl(0x02010100))) {
		IPQ_LOG(IPOQUE_PROTOCOL_KONTIKI, ipoque_struct, IPQ_LOG_DEBUG, "Kontiki UDP detected.\n");
		ipoque_int_kontiki_add_connection(ipoque_struct);
		return;
	}
	if (packet->payload_packet_len > 0 && packet->payload[0] == 0x02) {

		if (packet->payload_packet_len == 20 && (get_u32(packet->payload, 16) == htonl(0x02040100))) {
			IPQ_LOG(IPOQUE_PROTOCOL_KONTIKI, ipoque_struct, IPQ_LOG_DEBUG, "Kontiki UDP detected.\n");
			ipoque_int_kontiki_add_connection(ipoque_struct);
			return;
		}
		if (packet->payload_packet_len == 16 && (get_u32(packet->payload, 12) == htonl(0x000004e4))) {
			IPQ_LOG(IPOQUE_PROTOCOL_KONTIKI, ipoque_struct, IPQ_LOG_DEBUG, "Kontiki UDP detected.\n");
			ipoque_int_kontiki_add_connection(ipoque_struct);
			return;
		}
	}

	IPOQUE_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, IPOQUE_PROTOCOL_KONTIKI);
}

#endif
