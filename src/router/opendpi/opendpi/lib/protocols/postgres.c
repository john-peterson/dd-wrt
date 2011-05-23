/*
 * postgres.c
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
#ifdef IPOQUE_PROTOCOL_POSTGRES

static void ipoque_search_postgres_tcp(struct ipoque_detection_module_struct
								*ipoque_struct)
{
	struct ipoque_packet_struct *packet = &ipoque_struct->packet;
	struct ipoque_flow_struct *flow = ipoque_struct->flow;
	u16 size;

	if (flow->postgres_stage == 0) {
		//SSL
		if (packet->payload_packet_len > 7 &&
			packet->payload[4] == 0x04 &&
			packet->payload[5] == 0xd2 &&
			packet->payload[6] == 0x16 &&
			packet->payload[7] == 0x2f && ntohl(get_u32(packet->payload, 0)) == packet->payload_packet_len) {
			flow->postgres_stage = 1 + packet->packet_direction;
			return;
		}
		//no SSL
		if (packet->payload_packet_len > 7 &&
			//protocol version number - to be updated
			ntohl(get_u32(packet->payload, 4)) < 0x00040000 &&
			ntohl(get_u32(packet->payload, 0)) == packet->payload_packet_len) {
			flow->postgres_stage = 3 + packet->packet_direction;
			return;
		}
	} else {
		if (flow->postgres_stage == 2 - packet->packet_direction) {
			//SSL accepted
			if (packet->payload_packet_len == 1 && packet->payload[0] == 'S') {
				IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "PostgreSQL detected, SSL accepted.\n");
				ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
				return;
			}
			//SSL denied
			if (packet->payload_packet_len == 1 && packet->payload[0] == 'N') {
				IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "PostgreSQL detected, SSL denied.\n");
				ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
				return;
			}
		}
		//no SSL
		if (flow->postgres_stage == 4 - packet->packet_direction)
			if (packet->payload_packet_len > 8 &&
				ntohl(get_u32(packet->payload, 5)) < 10 &&
				ntohl(get_u32(packet->payload, 1)) == packet->payload_packet_len - 1 && packet->payload[0] == 0x52) {
				IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "PostgreSQL detected, no SSL.\n");
				ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
				return;
			}
		if (flow->postgres_stage == 6
			&& ntohl(get_u32(packet->payload, 1)) == packet->payload_packet_len - 1 && packet->payload[0] == 'p') {
			IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "found postgres asymmetrically.\n");
			ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
			return;
		}
		if (flow->postgres_stage == 5 && packet->payload[0] == 'R') {
			if (ntohl(get_u32(packet->payload, 1)) == packet->payload_packet_len - 1) {
				IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "found postgres asymmetrically.\n");
				ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
				return;
			}
			size = ntohl(get_u32(packet->payload, 1)) + 1;
			if (packet->payload[size - 1] == 'S') {
				if ((size + get_u32(packet->payload, (size + 1))) == packet->payload_packet_len) {
					IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "found postgres asymmetrically.\n");
					ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
					return;
				}
			}
			size += get_u32(packet->payload, (size + 1)) + 1;
			if (packet->payload[size - 1] == 'S') {
				IPQ_LOG(IPOQUE_PROTOCOL_POSTGRES, ipoque_struct, IPQ_LOG_DEBUG, "found postgres asymmetrically.\n");
				ipq_connection_detected(ipoque_struct, IPOQUE_PROTOCOL_POSTGRES);
				return;
			}
		}
	}

	IPOQUE_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, IPOQUE_PROTOCOL_POSTGRES);
}

#endif
