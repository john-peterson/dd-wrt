/* For terms of usage/redistribution/modification see the LICENSE file */
/* For authors and contributors see the AUTHORS file */

/***

ipfrag.c - module that handles fragmented IP packets.

This module is necessary to maintain accurate counts in case
fragmented IP packets are received.  TCP and UDP headers are not copied
in fragments.

This module is based on RFC 815, but does not really reassemble packets.
The routines here merely accumulate packet sizes and pass them off to
the IP traffic monitor routine.

***/

#include "iptraf-ng-compat.h"

#include "ipfrag.h"

static struct fragent *fraglist = NULL;
static struct fragent *fragtail = NULL;

static struct fragent *addnewdgram(struct iphdr *packet)
{
	struct fragent *ptmp;

	ptmp = xmallocz(sizeof(struct fragent));
	if (fraglist == NULL) {
		fraglist = ptmp;
		ptmp->prev_entry = NULL;
	}
	if (fragtail != NULL) {
		fragtail->next_entry = ptmp;
		ptmp->prev_entry = fragtail;
	}
	ptmp->fragdesclist = xmalloc(sizeof(struct fragdescent));
	ptmp->fragdesclist->min = 0;
	ptmp->fragdesclist->max = 65535;
	ptmp->fragdesclist->next_entry = NULL;
	ptmp->fragdesclist->prev_entry = NULL;
	ptmp->fragdesctail = ptmp->fragdesclist;

	fragtail = ptmp;
	ptmp->next_entry = NULL;

	ptmp->s_addr = packet->saddr;
	ptmp->d_addr = packet->daddr;
	ptmp->protocol = packet->protocol;
	ptmp->id = packet->id;

	return ptmp;
}

static struct fragdescent *addnewhole(struct fragent *frag)
{
	struct fragdescent *ptmp;

	ptmp = xmalloc(sizeof(struct fragdescent));

	if (frag->fragdesclist == NULL) {
		frag->fragdesclist = ptmp;
		ptmp->prev_entry = NULL;
	}
	if (frag->fragdesctail != NULL) {
		frag->fragdesctail->next_entry = ptmp;
		ptmp->prev_entry = frag->fragdesctail;
	}
	ptmp->next_entry = NULL;
	frag->fragdesctail = ptmp;

	return ptmp;
}

static struct fragent *searchfrags(unsigned long saddr, unsigned long daddr,
				   unsigned int protocol, unsigned int id)
{
	struct fragent *ftmp = fraglist;

	while (ftmp != NULL) {
		if ((saddr == ftmp->s_addr) && (daddr == ftmp->d_addr)
		    && (protocol == ftmp->protocol) && (id == ftmp->id))
			return ftmp;

		ftmp = ftmp->next_entry;
	}

	return NULL;
}

static void deldgram(struct fragent *ftmp)
{
	if (ftmp->prev_entry != NULL)
		ftmp->prev_entry->next_entry = ftmp->next_entry;
	else
		fraglist = ftmp->next_entry;

	if (ftmp->next_entry != NULL)
		ftmp->next_entry->prev_entry = ftmp->prev_entry;
	else
		fragtail = ftmp->prev_entry;

	free(ftmp);
}


/*
 * Destroy hole descriptor list
 */

static void destroyholes(struct fragent *ftmp)
{
	struct fragdescent *dtmp = ftmp->fragdesclist;
	struct fragdescent *ntmp = NULL;

	if (ftmp->fragdesclist != NULL) {
		ntmp = dtmp->next_entry;

		while (dtmp != NULL) {
			free(dtmp);
			dtmp = ntmp;

			if (ntmp != NULL)
				ntmp = ntmp->next_entry;
		}
	}
}

void destroyfraglist(void)
{
	struct fragent *ptmp = fraglist;
	struct fragent *ctmp = NULL;

	if (fraglist != NULL) {
		ctmp = ptmp->next_entry;

		while (ptmp != NULL) {
			destroyholes(ptmp);
			free(ptmp);
			ptmp = ctmp;

			if (ctmp != NULL)
				ctmp = ctmp->next_entry;
		}
	}
	fraglist = NULL;
	fragtail = NULL;
}

/*
 * Process IP fragment.  Returns number of bytes to report to the traffic
 * monitor or 0 for an error condition.
 */

unsigned int processfragment(struct iphdr *packet, unsigned int *sport,
			     unsigned int *dport, int *firstin)
{
	struct fragent *ftmp;
	struct fragdescent *dtmp;
	struct fragdescent *ntmp;
	char *tpacket;

	unsigned int offset;
	unsigned int lastbyte;
	unsigned int retval;

	/* Determine appropriate hole descriptor list */

	ftmp =
	    searchfrags(packet->saddr, packet->daddr, packet->protocol,
			packet->id);

	if (ftmp == NULL)	/* No such datagram for this frag yet */
		ftmp = addnewdgram(packet);

	if (ftmp == NULL)
		return 0;

	/* 
	 * At this point, ftmp should contain the address of the appropriate
	 * descriptor list.
	 */

	dtmp = ftmp->fragdesclist;	/* Point to hole descriptors */
	offset = (ntohs(packet->frag_off) & 0x1fff) * 8;
	lastbyte = (offset + (ntohs(packet->tot_len) - (packet->ihl) * 4)) - 1;

	if ((ntohs(packet->frag_off) & 0x1fff) == 0) {	/* first fragment? */
		ftmp->firstin = 1;
		tpacket = ((char *) (packet)) + (packet->ihl * 4);
		if (packet->protocol == IPPROTO_TCP) {
			ftmp->s_port = ((struct tcphdr *) tpacket)->source;
			ftmp->d_port = ((struct tcphdr *) tpacket)->dest;
		} else if (packet->protocol == IPPROTO_UDP) {
			ftmp->s_port = ((struct udphdr *) tpacket)->source;
			ftmp->d_port = ((struct udphdr *) tpacket)->dest;
		}
	}
	while (dtmp != NULL) {
		if ((offset <= dtmp->max) && (lastbyte >= dtmp->min))
			break;

		dtmp = dtmp->next_entry;
	}

	if (dtmp != NULL) {	/* Duplicate/overlap or something out of the
				   loopback interface */
		/* 
		 * Delete current entry from hole descriptor list
		 */

		if (dtmp->prev_entry != NULL)
			dtmp->prev_entry->next_entry = dtmp->next_entry;
		else
			ftmp->fragdesclist = dtmp->next_entry;

		if (dtmp->next_entry != NULL)
			dtmp->next_entry->prev_entry = dtmp->prev_entry;
		else
			ftmp->fragdesctail = dtmp->prev_entry;

		/*
		 * Memory for the hole descriptor will not be released yet.
		 */

		if (offset > dtmp->min) {
			/*
			 * If offset in fragment is greater than offset in the descriptor,
			 * create a new hole descriptor.
			 */

			ntmp = addnewhole(ftmp);
			ntmp->min = dtmp->min;
			ntmp->max = offset - 1;
		}
		if ((lastbyte < dtmp->max)
		    && (ntohs(packet->frag_off) & 0x2000)) {
			/*
			 * If last byte in fragment is less than the last byte of the
			 * hole descriptor, and more fragments, create a new hole
			 * descriptor.
			 */

			ntmp = addnewhole(ftmp);
			ntmp->min = lastbyte + 1;
			ntmp->max = dtmp->max;
		}
		free(dtmp);

	}
	*firstin = ftmp->firstin;

	ftmp->bcount += ntohs(packet->tot_len);

	if (ftmp->firstin) {
		*sport = ftmp->s_port;
		*dport = ftmp->d_port;
		retval = ftmp->bcount;
		ftmp->bcount = 0;

		if (ftmp->fragdesclist == NULL)
			deldgram(ftmp);

		return retval;
	} else
		return 0;
}
