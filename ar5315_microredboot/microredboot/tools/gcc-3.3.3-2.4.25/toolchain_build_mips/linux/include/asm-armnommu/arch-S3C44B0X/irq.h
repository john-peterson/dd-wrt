/*
 * uclinux/include/asm-armnommu/arch-S3C44B0X/irq.h
 *
 * Copyright (C) 2003 Thomas Eschenbacher <eschenbacher@sympat.de>
 *
 */

#ifndef __ASM_ARCH_IRQ_H__
#define __ASM_ARCH_IRQ_H__

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/mach/irq.h>
#include <asm/arch/irqs.h>

extern void s3c44b0x_mask_irq(unsigned int irq);
extern void s3c44b0x_unmask_irq(unsigned int irq);
extern void s3c44b0x_mask_ack_irq(unsigned int irq);
extern void s3c44b0x_clear_pb(unsigned int irq);

static __inline__ unsigned int fixup_irq (unsigned int irq)
{
	s3c44b0x_clear_pb(irq);
	return (irq);
}

static __inline__ void irq_init_irq(void)
{
	int irq;
	for (irq = 0; irq < NR_IRQS; irq++) {
		irq_desc[irq].valid	= 1;
		irq_desc[irq].probe_ok	= 1;
		irq_desc[irq].mask_ack	= s3c44b0x_mask_ack_irq;
		irq_desc[irq].mask	= s3c44b0x_mask_irq;
		irq_desc[irq].unmask	= s3c44b0x_unmask_irq;

		/* @TODO initialize interrupt priorities */
	}


	/* mask and disable all further interrupts */
	/* but globaly enable the INT-controller */
	outl(0x03FFFFFF, S3C44B0X_INTMSK);

	/* set all to IRQ mode, not FIQ */
	outl(0x00000000, S3C44B0X_INTMOD);

	/* clear all pending bits */
	outl(0x07FFFFFF, S3C44B0X_I_ISPC);
}


#endif /* __ASM_ARCH_IRQ_H__ */
