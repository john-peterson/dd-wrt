/* 
 *$Id: receive.c,v 1.5 2005/09/01 08:13:56 bogdan_iancu Exp $
 *
 * Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of openser, a free SIP server.
 *
 * openser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * openser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 * ---------
 * 2003-02-28 scratchpad compatibility abandoned (jiri)
 * 2003-01-29 transport-independent message zero-termination in
 *            receive_msg (jiri)
 * 2003-02-07 undoed jiri's zero term. changes (they break tcp) (andrei)
 * 2003-02-10 moved zero-term in the calling functions (udp_receive &
 *            tcp_read_req)
 * 2003-08-13 fixed exec_pre_cb returning 0 (backported from stable) (andrei)
 * 2004-02-06 added user preferences support - destroy_avps() (bogdan)
 * 2004-04-30 exec_pre_cb is called after basic sanity checks (at least one
 *            via present & parsed ok)  (andrei)
 * 2004-08-23 avp core changed - destroy_avp-> reset_avps (bogdan)
 * 2005-07-26 default onreply route added (andrei)
 */


#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "receive.h"
#include "globals.h"
#include "dprint.h"
#include "route.h"
#include "parser/msg_parser.h"
#include "forward.h"
#include "action.h"
#include "mem/mem.h"
#include "stats.h"
#include "ip_addr.h"
#include "script_cb.h"
#include "dset.h"
#include "usr_avp.h"


#include "tcp_server.h" /* for tcpconn_add_alias */


#ifdef DEBUG_DMALLOC
#include <mem/dmalloc.h>
#endif

unsigned int msg_no=0;
/* address preset vars */
str default_global_address={0,0};
str default_global_port={0,0};
str default_via_address={0,0};
str default_via_port={0,0};


/* WARNING: buf must be 0 terminated (buf[len]=0) or some things might 
 * break (e.g.: modules/textops)
 */
int receive_msg(char* buf, unsigned int len, struct receive_info* rcv_info) 
{
	struct sip_msg* msg;
#ifdef STATS
	int skipped = 1;
	struct timeval tvb, tve;
	struct timezone tz;
	unsigned int diff;
#endif

	msg=pkg_malloc(sizeof(struct sip_msg));
	if (msg==0) {
		LOG(L_ERR, "ERROR: receive_msg: no mem for sip_msg\n");
		goto error00;
	}
	msg_no++;
	/* number of vias parsed -- good for diagnostic info in replies */
	via_cnt=0;

	memset(msg,0, sizeof(struct sip_msg)); /* init everything to 0 */
	/* fill in msg */
	msg->buf=buf;
	msg->len=len;
	/* zero termination (termination of orig message bellow not that
	   useful as most of the work is done with scratch-pad; -jiri  */
	/* buf[len]=0; */ /* WARNING: zero term removed! */
	msg->rcv=*rcv_info;
	msg->id=msg_no;
	msg->set_global_address=default_global_address;
	msg->set_global_port=default_global_port;
	
	if (parse_msg(buf,len, msg)!=0){
		LOG(L_ERR, "ERROR: receive_msg: parse_msg failed\n");
		goto error02;
	}
	DBG("After parse_msg...\n");


	/* ... clear branches from previous message */
	clear_branches();

	if (msg->first_line.type==SIP_REQUEST){
		/* sanity checks */
		if ((msg->via1==0) || (msg->via1->error!=PARSE_OK)){
			/* no via, send back error ? */
			LOG(L_ERR, "ERROR: receive_msg: no via found in request\n");
			goto error02;
		}
		/* check if necessary to add receive?->moved to forward_req */
		/* check for the alias stuff */
#ifdef USE_TCP
		if (msg->via1->alias && tcp_accept_aliases && 
				(((rcv_info->proto==PROTO_TCP) && !tcp_disable)
#ifdef USE_TLS
					|| ((rcv_info->proto==PROTO_TLS) && !tls_disable)
#endif
				)
			){
			if (tcpconn_add_alias(rcv_info->proto_reserved1, msg->via1->port,
									rcv_info->proto)!=0){
				LOG(L_ERR, " ERROR: receive_msg: tcp alias failed\n");
				/* continue */
			}
		}
#endif

		DBG("preparing to run routing scripts...\n");
#ifdef  STATS
		gettimeofday( & tvb, &tz );
#endif
		/* set request route type --bogdan*/
		set_route_type( REQUEST_ROUTE );

		/* execute pre-script callbacks, if any;
		 * if some of the callbacks said not to continue with
		 * script processing, don't do so;
		 * if we are here basic sanity checks are already done
		 * (like presence of at least one via), so you can count
		 * on via1 being parsed in a pre-script callback --andrei
		 */
		if (exec_pre_req_cb(msg)==0 )
			goto end; /* drop the message */

		/* exec the routing script */
		if (run_actions(rlist[DEFAULT_RT], msg)<0) {
			LOG(L_WARN, "WARNING: receive_msg: "
					"error while trying script\n");
		} else {
#ifdef STATS
			gettimeofday( & tve, &tz );
			diff = (tve.tv_sec-tvb.tv_sec)*1000000+(tve.tv_usec-tvb.tv_usec);
			stats->processed_requests++;
			stats->acc_req_time += diff;
			DBG("successfully ran routing scripts...(%d usec)\n", diff);
			STATS_RX_REQUEST( msg->first_line.u.request.method_value );
#endif
		}

		/* execute post request-script callbacks */
		exec_post_req_cb(msg);
	}else if (msg->first_line.type==SIP_REPLY){
		/* sanity checks */
		if ((msg->via1==0) || (msg->via1->error!=PARSE_OK)){
			/* no via, send back error ? */
			LOG(L_ERR, "ERROR: receive_msg: no via found in reply\n");
			goto error02;
		}

#ifdef STATS
		gettimeofday( & tvb, &tz );
		STATS_RX_RESPONSE ( msg->first_line.u.reply.statuscode / 100 );
#endif
		/* set reply route type --bogdan*/
		set_route_type( ONREPLY_ROUTE );

		/* execute pre-script callbacks, if any ;
		 * if some of the callbacks said not to continue with
		 * script processing, don't do so ;
		 * if we are here, basic sanity checks are already done
		 * (like presence of at least one via), so you can count
		 * on via1 being parsed in a pre-script callback --andrei
		 */
		if (exec_pre_rpl_cb(msg)==0 )
			goto end; /* drop the request */

		/* exec the onreply routing script */
		if ( onreply_rlist[DEFAULT_RT]!=0 &&
		run_actions(onreply_rlist[DEFAULT_RT],msg)==0
		&& msg->REPLY_STATUS<200 && (action_flags&ACT_FL_DROP)) {
			DBG("DEBUG:received: dropping provisional reply %d\n",
				msg->REPLY_STATUS);
			goto end; /* drop the message */
		} else {
			/* send the msg */
			forward_reply(msg);
#ifdef STATS
			gettimeofday( & tve, &tz );
			diff = (tve.tv_sec-tvb.tv_sec)*1000000+(tve.tv_usec-tvb.tv_usec);
			stats->processed_responses++;
			stats->acc_res_time+=diff;
			DBG("successfully ran reply processing...(%d usec)\n", diff);
#endif
		}

		/* execute post reply-script callbacks */
		exec_post_rpl_cb(msg);
	}

#ifdef STATS
	skipped = 0;
#endif
end:
	/* free possible loaded avps -bogdan */
	reset_avps();
	DBG("receive_msg: cleaning up\n");
	free_sip_msg(msg);
	pkg_free(msg);
#ifdef STATS
	if (skipped) STATS_RX_DROPS;
#endif
	return 0;
error02:
	free_sip_msg(msg);
	pkg_free(msg);
error00:
#ifdef STATS
	STATS_RX_DROPS;
#endif
	return -1;
}

