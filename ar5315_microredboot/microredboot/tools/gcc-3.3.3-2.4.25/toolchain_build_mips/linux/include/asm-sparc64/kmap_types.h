#ifndef _ASM_KMAP_TYPES_H
#define _ASM_KMAP_TYPES_H

/* Dummy header just to define km_type.  None of this
 * is actually used on sparc64.  -DaveM
 */

enum km_type {
	KM_BOUNCE_READ,
	KM_SKB_SUNRPC_DATA,
	KM_SKB_DATA_SOFTIRQ,
	KM_USER0,
	KM_USER1,
	KM_BH_IRQ,
	KM_SOFTIRQ0,
	KM_SOFTIRQ1,
	KM_TYPE_NR
};

#endif
