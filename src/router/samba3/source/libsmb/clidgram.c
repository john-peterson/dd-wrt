/* 
   Unix SMB/CIFS implementation.
   client dgram calls
   Copyright (C) Andrew Tridgell 1994-1998
   Copyright (C) Richard Sharpe 2001
   Copyright (C) John Terpstra 2001

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "includes.h"

/*
 * cli_send_mailslot, send a mailslot for client code ...
 */

bool cli_send_mailslot(struct messaging_context *msg_ctx,
		       bool unique, const char *mailslot,
		       uint16 priority,
		       char *buf, int len,
		       const char *srcname, int src_type,
		       const char *dstname, int dest_type,
		       const struct sockaddr_storage *dest_ss)
{
	struct packet_struct p;
	struct dgram_packet *dgram = &p.packet.dgram;
	char *ptr, *p2;
	char tmp[4];
	pid_t nmbd_pid;
	char addr[INET6_ADDRSTRLEN];

	if ((nmbd_pid = pidfile_pid("nmbd")) == 0) {
		DEBUG(3, ("No nmbd found\n"));
		return False;
	}

	if (dest_ss->ss_family != AF_INET) {
		DEBUG(3, ("cli_send_mailslot: can't send to IPv6 address.\n"));
		return false;
	}

	memset((char *)&p, '\0', sizeof(p));

	/*
	 * Next, build the DGRAM ...
	 */

	/* DIRECT GROUP or UNIQUE datagram. */
	dgram->header.msg_type = unique ? 0x10 : 0x11;
	dgram->header.flags.node_type = M_NODE;
	dgram->header.flags.first = True;
	dgram->header.flags.more = False;
	dgram->header.dgm_id = ((unsigned)time(NULL)%(unsigned)0x7FFF) +
		((unsigned)sys_getpid()%(unsigned)100);
	/* source ip is filled by nmbd */
	dgram->header.dgm_length = 0; /* Let build_dgram() handle this. */
	dgram->header.packet_offset = 0;

	make_nmb_name(&dgram->source_name,srcname,src_type);
	make_nmb_name(&dgram->dest_name,dstname,dest_type);

	ptr = &dgram->data[0];

	/* Setup the smb part. */
	ptr -= 4; /* XXX Ugliness because of handling of tcp SMB length. */
	memcpy(tmp,ptr,4);

	if (smb_size + 17*2 + strlen(mailslot) + 1 + len > MAX_DGRAM_SIZE) {
		DEBUG(0, ("cli_send_mailslot: Cannot write beyond end of packet\n"));
		return False;
	}

	cli_set_message(ptr,17,strlen(mailslot) + 1 + len,True);
	memcpy(ptr,tmp,4);

	SCVAL(ptr,smb_com,SMBtrans);
	SSVAL(ptr,smb_vwv1,len);
	SSVAL(ptr,smb_vwv11,len);
	SSVAL(ptr,smb_vwv12,70 + strlen(mailslot));
	SSVAL(ptr,smb_vwv13,3);
	SSVAL(ptr,smb_vwv14,1);
	SSVAL(ptr,smb_vwv15,priority);
	SSVAL(ptr,smb_vwv16,2);
	p2 = smb_buf(ptr);
	fstrcpy(p2,mailslot);
	p2 = skip_string(ptr,MAX_DGRAM_SIZE,p2);
	if (!p2) {
		return False;
	}

	memcpy(p2,buf,len);
	p2 += len;

	dgram->datasize = PTR_DIFF(p2,ptr+4); /* +4 for tcp length. */

	p.packet_type = DGRAM_PACKET;
	p.ip = ((const struct sockaddr_in *)dest_ss)->sin_addr;
	p.timestamp = time(NULL);

	DEBUG(4,("send_mailslot: Sending to mailslot %s from %s ",
		 mailslot, nmb_namestr(&dgram->source_name)));
	print_sockaddr(addr, sizeof(addr), dest_ss);

	DEBUGADD(4,("to %s IP %s\n", nmb_namestr(&dgram->dest_name), addr));

	return NT_STATUS_IS_OK(messaging_send_buf(msg_ctx,
						  pid_to_procid(nmbd_pid),
						  MSG_SEND_PACKET,
						  (uint8 *)&p, sizeof(p)));
}

static const char *mailslot_name(TALLOC_CTX *mem_ctx, struct in_addr dc_ip)
{
	return talloc_asprintf(mem_ctx, "%s%X",
			       NBT_MAILSLOT_GETDC, dc_ip.s_addr);
}

bool send_getdc_request(TALLOC_CTX *mem_ctx,
			struct messaging_context *msg_ctx,
			struct sockaddr_storage *dc_ss,
			const char *domain_name,
			const DOM_SID *sid,
			uint32_t nt_version)
{
	struct in_addr dc_ip;
	const char *my_acct_name = NULL;
	const char *my_mailslot = NULL;
	struct nbt_netlogon_packet packet;
	struct NETLOGON_SAM_LOGON_REQUEST *s;
	enum ndr_err_code ndr_err;
	DATA_BLOB blob;
	struct dom_sid my_sid;

	ZERO_STRUCT(packet);
	ZERO_STRUCT(my_sid);

	if (dc_ss->ss_family != AF_INET) {
		return false;
	}

	if (sid) {
		my_sid = *sid;
	}

	dc_ip = ((struct sockaddr_in *)dc_ss)->sin_addr;
	my_mailslot = mailslot_name(mem_ctx, dc_ip);
	if (!my_mailslot) {
		return false;
	}

	my_acct_name = talloc_asprintf(mem_ctx, "%s$", global_myname());
	if (!my_acct_name) {
		return false;
	}

	packet.command	= LOGON_SAM_LOGON_REQUEST;
	s		= &packet.req.logon;

	s->request_count	= 0;
	s->computer_name	= global_myname();
	s->user_name		= my_acct_name;
	s->mailslot_name	= my_mailslot;
	s->acct_control		= ACB_WSTRUST;
	s->sid			= my_sid;
	s->nt_version		= nt_version;
	s->lmnt_token		= 0xffff;
	s->lm20_token		= 0xffff;

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_DEBUG(nbt_netlogon_packet, &packet);
	}

	ndr_err = ndr_push_struct_blob(&blob, mem_ctx, &packet,
		       (ndr_push_flags_fn_t)ndr_push_nbt_netlogon_packet);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		return false;
	}

	return cli_send_mailslot(msg_ctx,
				 false, NBT_MAILSLOT_NTLOGON, 0,
				 (char *)blob.data, blob.length,
				 global_myname(), 0, domain_name, 0x1c,
				 dc_ss);
}

bool receive_getdc_response(TALLOC_CTX *mem_ctx,
			    struct sockaddr_storage *dc_ss,
			    const char *domain_name,
			    uint32_t *nt_version,
			    const char **dc_name,
			    struct netlogon_samlogon_response **_r)
{
	struct packet_struct *packet;
	const char *my_mailslot = NULL;
	struct in_addr dc_ip;
	DATA_BLOB blob;
	struct netlogon_samlogon_response r;
	union dgram_message_body p;
	enum ndr_err_code ndr_err;
	NTSTATUS status;

	const char *returned_dc = NULL;
	const char *returned_domain = NULL;

	if (dc_ss->ss_family != AF_INET) {
		return false;
	}

	dc_ip = ((struct sockaddr_in *)dc_ss)->sin_addr;

	my_mailslot = mailslot_name(mem_ctx, dc_ip);
	if (!my_mailslot) {
		return false;
	}

	packet = receive_unexpected(DGRAM_PACKET, 0, my_mailslot);

	if (packet == NULL) {
		DEBUG(5, ("Did not receive packet for %s\n", my_mailslot));
		return False;
	}

	DEBUG(5, ("Received packet for %s\n", my_mailslot));

	blob = data_blob_const(packet->packet.dgram.data,
			       packet->packet.dgram.datasize);

	if (blob.length < 4) {
		DEBUG(0,("invalid length: %d\n", (int)blob.length));
		return false;
	}

	if (RIVAL(blob.data,0) != DGRAM_SMB) {
		DEBUG(0,("invalid packet\n"));
		return false;
	}

	blob.data += 4;
	blob.length -= 4;

	ndr_err = ndr_pull_union_blob_all(&blob, mem_ctx, &p, DGRAM_SMB,
		       (ndr_pull_flags_fn_t)ndr_pull_dgram_smb_packet);
	if (!NDR_ERR_CODE_IS_SUCCESS(ndr_err)) {
		DEBUG(0,("failed to parse packet\n"));
		return false;
	}

	if (p.smb.smb_command != SMB_TRANSACTION) {
		DEBUG(0,("invalid smb_command: %d\n", p.smb.smb_command));
		return false;
	}

	if (DEBUGLEVEL >= 10) {
		NDR_PRINT_DEBUG(dgram_smb_packet, &p);
	}

	blob = p.smb.body.trans.data;

	ZERO_STRUCT(r);

	status = pull_netlogon_samlogon_response(&blob, mem_ctx, &r);
	if (!NT_STATUS_IS_OK(status)) {
		return false;
	}

	map_netlogon_samlogon_response(&r);

	/* do we still need this ? */
	*nt_version = r.ntver;

	returned_domain = r.data.nt5_ex.domain;
	returned_dc = r.data.nt5_ex.pdc_name;

	if (!strequal(returned_domain, domain_name)) {
		DEBUG(3, ("GetDC: Expected domain %s, got %s\n",
			  domain_name, returned_domain));
		return false;
	}

	*dc_name = talloc_strdup(mem_ctx, returned_dc);
	if (!*dc_name) {
		return false;
	}

	if (**dc_name == '\\')	*dc_name += 1;
	if (**dc_name == '\\')	*dc_name += 1;

	if (_r) {
		*_r = (struct netlogon_samlogon_response *)talloc_memdup(
			mem_ctx, &r, sizeof(struct netlogon_samlogon_response));
		if (!*_r) {
			return false;
		}
	}

	DEBUG(10, ("GetDC gave name %s for domain %s\n",
		   *dc_name, returned_domain));

	return True;
}

