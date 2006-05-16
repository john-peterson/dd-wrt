// Routine to trigger Finisar GTA analyzer. Runs of GPIO2
// NOTE: DEBUG ONLY! Could interfere with FCMNGR/Miniport operation
// since it writes directly to the Tachyon board.  This function
// developed for Compaq HBA Tachyon TS v1.2 (Rev X5 PCB)

#include "cpqfcTStrigger.h"
#if TRIGGERABLE_HBA

#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <asm/io.h>

void TriggerHBA(void *io_upper, int print)
{
	__u32 long value;

	// get initial value in hopes of not modifying any other GPIO line
	io_upper += 0x188;	// TachTL/TS Control reg

	value = readl(io_upper);
	// set HIGH to trigger external analyzer (tested on Dolche Finisar 1Gb GTA)
	// The Finisar anaylzer triggers on low-to-high TTL transition
	value |= 0x01;		// set bit 0

	writel(value, io_upper);

	if (print)
		printk(" -GPIO0 set- ");
}

#endif
