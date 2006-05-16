/*********************************************************************
 *                
 * Filename:      ircomm_ttp.c
 * Version:       1.0
 * Description:   Interface between IrCOMM and IrTTP
 * Status:        Stable
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sun Jun  6 20:48:27 1999
 * Modified at:   Mon Dec 13 11:35:13 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999 Dag Brattli, All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *     MA 02111-1307 USA
 *     
 ********************************************************************/

#include <linux/sched.h>
#include <linux/init.h>

#include <net/irda/irda.h>
#include <net/irda/irlmp.h>
#include <net/irda/iriap.h>
#include <net/irda/irttp.h>

#include <net/irda/ircomm_event.h>
#include <net/irda/ircomm_ttp.h>

/*
 * Function ircomm_open_tsap (self)
 *
 *    
 *
 */
int ircomm_open_tsap(struct ircomm_cb *self)
{
	notify_t notify;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);

	/* Register callbacks */
	irda_notify_init(&notify);
	notify.data_indication       = ircomm_ttp_data_indication;
	notify.connect_confirm       = ircomm_ttp_connect_confirm;
	notify.connect_indication    = ircomm_ttp_connect_indication;
	notify.flow_indication       = ircomm_ttp_flow_indication;
	notify.disconnect_indication = ircomm_ttp_disconnect_indication;
	notify.instance = self;
	strncpy(notify.name, "IrCOMM", NOTIFY_MAX_NAME);

	self->tsap = irttp_open_tsap(LSAP_ANY, DEFAULT_INITIAL_CREDIT,
				     &notify);
	if (!self->tsap) {
		IRDA_DEBUG(0, "%s failed to allocate tsap\n", __FUNCTION__);
		return -1;
	}
	self->slsap_sel = self->tsap->stsap_sel;

	/*
	 *  Initialize the call-table for issuing commands
	 */
	self->issue.data_request       = ircomm_ttp_data_request;
	self->issue.connect_request    = ircomm_ttp_connect_request;
	self->issue.connect_response   = ircomm_ttp_connect_response;
	self->issue.disconnect_request = ircomm_ttp_disconnect_request;

	return 0;
}

/*
 * Function ircomm_ttp_connect_request (self, userdata)
 *
 *    
 *
 */
int ircomm_ttp_connect_request(struct ircomm_cb *self, 
			       struct sk_buff *userdata, 
			       struct ircomm_info *info)
{
	int ret = 0;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);

	ret = irttp_connect_request(self->tsap, info->dlsap_sel,
				    info->saddr, info->daddr, NULL, 
				    TTP_SAR_DISABLE, userdata); 
	return ret;
}	

/*
 * Function ircomm_ttp_connect_response (self, skb)
 *
 *    
 *
 */
int ircomm_ttp_connect_response(struct ircomm_cb *self, struct sk_buff *skb)
{
	int ret;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);
	
	ret = irttp_connect_response(self->tsap, TTP_SAR_DISABLE, skb);

	return ret;
}

/*
 * Function ircomm_ttp_data_request (self, userdata)
 *
 *    Send IrCOMM data to IrTTP layer. Currently we do not try to combine 
 *    control data with pure data, so they will be sent as separate frames. 
 *    Should not be a big problem though, since control frames are rare. But
 *    some of them are sent after connection establishment, so this can 
 *    increase the latency a bit.
 */
int ircomm_ttp_data_request(struct ircomm_cb *self, struct sk_buff *skb, 
			    int clen)
{
	int ret;

	ASSERT(skb != NULL, return -1;);

	IRDA_DEBUG(2, "%s(), clen=%d\n", __FUNCTION__, clen);

	/* 
	 * Insert clen field, currently we either send data only, or control
	 * only frames, to make things easier and avoid queueing
	 */
	ASSERT(skb_headroom(skb) >= IRCOMM_HEADER_SIZE, return -1;);
	skb_push(skb, IRCOMM_HEADER_SIZE);

	skb->data[0] = clen;

	ret = irttp_data_request(self->tsap, skb);
	if (ret) {
		ERROR("%s(), failed\n", __FUNCTION__);
		dev_kfree_skb(skb);
	}

	return ret;
}

/*
 * Function ircomm_ttp_data_indication (instance, sap, skb)
 *
 *    Incoming data
 *
 */
int ircomm_ttp_data_indication(void *instance, void *sap,
			       struct sk_buff *skb)
{
	struct ircomm_cb *self = (struct ircomm_cb *) instance;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);
	
	ASSERT(self != NULL, return -1;);
	ASSERT(self->magic == IRCOMM_MAGIC, return -1;);
	ASSERT(skb != NULL, return -1;);

	ircomm_do_event(self, IRCOMM_TTP_DATA_INDICATION, skb, NULL);

	return 0;
}

void ircomm_ttp_connect_confirm(void *instance, void *sap,
				struct qos_info *qos, 
				__u32 max_sdu_size, 
				__u8 max_header_size,
				struct sk_buff *skb)
{
	struct ircomm_cb *self = (struct ircomm_cb *) instance;
	struct ircomm_info info;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);

	ASSERT(self != NULL, return;);
	ASSERT(self->magic == IRCOMM_MAGIC, return;);
	ASSERT(skb != NULL, return;);
	ASSERT(qos != NULL, return;);

	if (max_sdu_size != TTP_SAR_DISABLE) {
		ERROR("%s(), SAR not allowed for IrCOMM!\n", __FUNCTION__);
		dev_kfree_skb(skb);
		return;
	}

	info.max_data_size = irttp_get_max_seg_size(self->tsap)
		- IRCOMM_HEADER_SIZE;
	info.max_header_size = max_header_size + IRCOMM_HEADER_SIZE;
	info.qos = qos;

	ircomm_do_event(self, IRCOMM_TTP_CONNECT_CONFIRM, skb, &info);
}

/*
 * Function ircomm_ttp_connect_indication (instance, sap, qos, max_sdu_size,
 *                                         max_header_size, skb)
 *
 *    
 *
 */
void ircomm_ttp_connect_indication(void *instance, void *sap,
				   struct qos_info *qos,
				   __u32 max_sdu_size,
				   __u8 max_header_size,
				   struct sk_buff *skb)
{
	struct ircomm_cb *self = (struct ircomm_cb *)instance;
	struct ircomm_info info;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);

	ASSERT(self != NULL, return;);
	ASSERT(self->magic == IRCOMM_MAGIC, return;);
	ASSERT(skb != NULL, return;);
	ASSERT(qos != NULL, return;);

	if (max_sdu_size != TTP_SAR_DISABLE) {
		ERROR("%s(), SAR not allowed for IrCOMM!\n", __FUNCTION__);
		dev_kfree_skb(skb);
		return;
	}

	info.max_data_size = irttp_get_max_seg_size(self->tsap)
		- IRCOMM_HEADER_SIZE;
	info.max_header_size = max_header_size + IRCOMM_HEADER_SIZE;
	info.qos = qos;

	ircomm_do_event(self, IRCOMM_TTP_CONNECT_INDICATION, skb, &info);
}

/*
 * Function ircomm_ttp_disconnect_request (self, userdata, info)
 *
 *    
 *
 */
int ircomm_ttp_disconnect_request(struct ircomm_cb *self, 
				  struct sk_buff *userdata, 
				  struct ircomm_info *info)
{
	int ret;

	ret = irttp_disconnect_request(self->tsap, userdata, P_NORMAL);

	return ret;
}

/*
 * Function ircomm_ttp_disconnect_indication (instance, sap, reason, skb)
 *
 *    
 *
 */
void ircomm_ttp_disconnect_indication(void *instance, void *sap, 
				      LM_REASON reason,
				      struct sk_buff *skb)
{
	struct ircomm_cb *self = (struct ircomm_cb *) instance;
	struct ircomm_info info;

	IRDA_DEBUG(2, "%s()\n", __FUNCTION__);

	ASSERT(self != NULL, return;);
	ASSERT(self->magic == IRCOMM_MAGIC, return;);

	info.reason = reason;

	ircomm_do_event(self, IRCOMM_TTP_DISCONNECT_INDICATION, skb, &info);
}

/*
 * Function ircomm_ttp_flow_indication (instance, sap, cmd)
 *
 *    Layer below is telling us to start or stop the flow of data
 *
 */
void ircomm_ttp_flow_indication(void *instance, void *sap, LOCAL_FLOW cmd)
{
	struct ircomm_cb *self = (struct ircomm_cb *) instance;

	IRDA_DEBUG(4, "%s()\n", __FUNCTION__);

	ASSERT(self != NULL, return;);
	ASSERT(self->magic == IRCOMM_MAGIC, return;);
	
	if (self->notify.flow_indication)
		self->notify.flow_indication(self->notify.instance, self, cmd);
}


