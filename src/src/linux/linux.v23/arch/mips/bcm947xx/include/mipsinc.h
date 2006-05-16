/*
 * HND Run Time Environment for standalone MIPS programs.
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id$
 */

#ifndef	_MISPINC_H
#define	_MISPINC_H


/* MIPS defines */

#ifdef	_LANGUAGE_ASSEMBLY

/*
 * Symbolic register names for 32 bit ABI
 */
#define zero	$0	/* wired zero */
#define AT	$1	/* assembler temp - uppercase because of ".set at" */
#define v0	$2	/* return value */
#define v1	$3
#define a0	$4	/* argument registers */
#define a1	$5
#define a2	$6
#define a3	$7
#define t0	$8	/* caller saved */
#define t1	$9
#define t2	$10
#define t3	$11
#define t4	$12
#define t5	$13
#define t6	$14
#define t7	$15
#define s0	$16	/* callee saved */
#define s1	$17
#define s2	$18
#define s3	$19
#define s4	$20
#define s5	$21
#define s6	$22
#define s7	$23
#define t8	$24	/* caller saved */
#define t9	$25
#define jp	$25	/* PIC jump register */
#define k0	$26	/* kernel scratch */
#define k1	$27
#define gp	$28	/* global pointer */
#define sp	$29	/* stack pointer */
#define fp	$30	/* frame pointer */
#define s8	$30	/* same like fp! */
#define ra	$31	/* return address */


/*  *********************************************************************
    *  CP0 Registers 
    ********************************************************************* */

#define C0_INX		$0
#define C0_RAND		$1
#define C0_TLBLO0	$2
#define C0_TLBLO	C0_TLBLO0
#define C0_TLBLO1	$3
#define C0_CTEXT	$4
#define C0_PGMASK	$5
#define C0_WIRED	$6
#define C0_BADVADDR	$8
#define C0_COUNT 	$9
#define C0_TLBHI	$10
#define C0_COMPARE	$11
#define C0_SR		$12
#define C0_STATUS	C0_SR
#define C0_CAUSE	$13
#define C0_EPC		$14
#define C0_PRID		$15
#define C0_CONFIG	$16
#define C0_LLADDR	$17
#define C0_WATCHLO	$18
#define C0_WATCHHI	$19
#define C0_XCTEXT	$20
#define C0_DIAGNOSTIC	$22
#define C0_BROADCOM	C0_DIAGNOSTIC
#define C0_ECC		$26
#define C0_CACHEERR	$27
#define C0_TAGLO	$28
#define C0_TAGHI	$29
#define C0_ERREPC	$30
#define C0_DESAVE	$31

/*
 * LEAF - declare leaf routine
 */
#define	LEAF(symbol)				\
		.globl	symbol;			\
		.align	2;			\
		.type	symbol,@function;	\
		.ent	symbol,0;		\
symbol:		.frame	sp,0,ra

/*
 * END - mark end of function
 */
#define	END(function)				\
		.end	function;		\
		.size	function,.-function

#define	_ULCAST_

#else

/*
 * The following macros are especially useful for __asm__
 * inline assembler.
 */
#ifndef __STR
#define __STR(x) #x
#endif
#ifndef STR
#define STR(x) __STR(x)
#endif

#define	_ULCAST_ (unsigned long)


/*  *********************************************************************
    *  CP0 Registers 
    ********************************************************************* */

#define C0_INX		0		/* CP0: TLB Index */
#define C0_RAND		1		/* CP0: TLB Random */
#define C0_TLBLO0	2		/* CP0: TLB EntryLo0 */
#define C0_TLBLO	C0_TLBLO0	/* CP0: TLB EntryLo0 */
#define C0_TLBLO1	3		/* CP0: TLB EntryLo1 */
#define C0_CTEXT	4		/* CP0: Context */
#define C0_PGMASK	5		/* CP0: TLB PageMask */
#define C0_WIRED	6		/* CP0: TLB Wired */
#define C0_BADVADDR	8		/* CP0: Bad Virtual Address */
#define C0_COUNT 	9		/* CP0: Count */
#define C0_TLBHI	10		/* CP0: TLB EntryHi */
#define C0_COMPARE	11		/* CP0: Compare */
#define C0_SR		12		/* CP0: Processor Status */
#define C0_STATUS	C0_SR		/* CP0: Processor Status */
#define C0_CAUSE	13		/* CP0: Exception Cause */
#define C0_EPC		14		/* CP0: Exception PC */
#define C0_PRID		15		/* CP0: Processor Revision Indentifier */
#define C0_CONFIG	16		/* CP0: Config */
#define C0_LLADDR	17		/* CP0: LLAddr */
#define C0_WATCHLO	18		/* CP0: WatchpointLo */
#define C0_WATCHHI	19		/* CP0: WatchpointHi */
#define C0_XCTEXT	20		/* CP0: XContext */
#define C0_DIAGNOSTIC	22		/* CP0: Diagnostic */
#define C0_BROADCOM	C0_DIAGNOSTIC	/* CP0: Broadcom Register */
#define C0_ECC		26		/* CP0: ECC */
#define C0_CACHEERR	27		/* CP0: CacheErr */
#define C0_TAGLO	28		/* CP0: TagLo */
#define C0_TAGHI	29		/* CP0: TagHi */
#define C0_ERREPC	30		/* CP0: ErrorEPC */
#define C0_DESAVE	31		/* CP0: DebugSave */

#endif	/* _LANGUAGE_ASSEMBLY */

/*
 * Memory segments (32bit kernel mode addresses)
 */
#undef KUSEG
#undef KSEG0
#undef KSEG1
#undef KSEG2
#undef KSEG3
#define KUSEG		0x00000000
#define KSEG0		0x80000000
#define KSEG1		0xa0000000
#define KSEG2		0xc0000000
#define KSEG3		0xe0000000
#define PHYSADDR_MASK	0x1fffffff

/*
 * Map an address to a certain kernel segment
 */
#undef PHYSADDR
#undef KSEG0ADDR
#undef KSEG1ADDR
#undef KSEG2ADDR
#undef KSEG3ADDR

#define PHYSADDR(a)	(_ULCAST_(a) & PHYSADDR_MASK)
#define KSEG0ADDR(a)	((_ULCAST_(a) & PHYSADDR_MASK) | KSEG0)
#define KSEG1ADDR(a)	((_ULCAST_(a) & PHYSADDR_MASK) | KSEG1)
#define KSEG2ADDR(a)	((_ULCAST_(a) & PHYSADDR_MASK) | KSEG2)
#define KSEG3ADDR(a)	((_ULCAST_(a) & PHYSADDR_MASK) | KSEG3)


#ifndef	Index_Invalidate_I
/*
 * Cache Operations
 */
#define Index_Invalidate_I      0x00
#define Index_Writeback_Inv_D   0x01
#define Index_Invalidate_SI     0x02
#define Index_Writeback_Inv_SD  0x03
#define Index_Load_Tag_I	0x04
#define Index_Load_Tag_D	0x05
#define Index_Load_Tag_SI	0x06
#define Index_Load_Tag_SD	0x07
#define Index_Store_Tag_I	0x08
#define Index_Store_Tag_D	0x09
#define Index_Store_Tag_SI	0x0A
#define Index_Store_Tag_SD	0x0B
#define Create_Dirty_Excl_D	0x0d
#define Create_Dirty_Excl_SD	0x0f
#define Hit_Invalidate_I	0x10
#define Hit_Invalidate_D	0x11
#define Hit_Invalidate_SI	0x12
#define Hit_Invalidate_SD	0x13
#define Fill_I			0x14
#define Hit_Writeback_Inv_D	0x15
					/* 0x16 is unused */
#define Hit_Writeback_Inv_SD	0x17
#define R5K_Page_Invalidate_S	0x17
#define Hit_Writeback_I		0x18
#define Hit_Writeback_D		0x19
					/* 0x1a is unused */
#define Hit_Writeback_SD	0x1b
					/* 0x1c is unused */
					/* 0x1e is unused */
#define Hit_Set_Virtual_SI	0x1e
#define Hit_Set_Virtual_SD	0x1f
#endif

#ifndef	_LANGUAGE_ASSEMBLY

/*
 * Macros to access the system control coprocessor
 */

#define MFC0(source, sel)					\
({								\
	int __res;						\
	__asm__ __volatile__(					\
	".set\tnoreorder\n\t"					\
	".set\tnoat\n\t"					\
	".word\t"STR(0x40010000 | ((source)<<11) | (sel))"\n\t"	\
	"move\t%0,$1\n\t"					\
	".set\tat\n\t"						\
	".set\treorder"						\
	:"=r" (__res)						\
	:							\
	:"$1");							\
	__res;							\
})

#define MTC0(source, sel, value)				\
do {								\
	__asm__ __volatile__(					\
	".set\tnoreorder\n\t"					\
	".set\tnoat\n\t"					\
	"move\t$1,%z0\n\t"					\
	".word\t"STR(0x40810000 | ((source)<<11) | (sel))"\n\t"	\
	".set\tat\n\t"						\
	".set\treorder"						\
	:							\
	:"jr" (value)						\
	:"$1");							\
} while (0)

#define get_c0_count()						\
({								\
	int __res;						\
	__asm__ __volatile__(					\
	".set\tnoreorder\n\t"					\
	".set\tnoat\n\t"					\
	"mfc0\t%0,$9\n\t"					\
	".set\tat\n\t"						\
	".set\treorder"						\
	:"=r" (__res));						\
	__res;							\
})

static INLINE void icache_probe(uint32 config1, uint *size, uint *lsize)
{
	uint lsz, sets, ways;

	/* Instruction Cache Size = Associativity * Line Size * Sets Per Way */
	if ((lsz = ((config1 >> 19) & 7)))
		lsz = 2 << lsz;
	sets = 64 << ((config1 >> 22) & 7);
	ways = 1 + ((config1 >> 16) & 7);
	*size = lsz * sets * ways;
	*lsize = lsz;
}

static INLINE void dcache_probe(uint32 config1, uint *size, uint *lsize)
{
	uint lsz, sets, ways;

	/* Data Cache Size = Associativity * Line Size * Sets Per Way */
	if ((lsz = ((config1 >> 10) & 7)))
		lsz = 2 << lsz;
	sets = 64 << ((config1 >> 13) & 7);
	ways = 1 + ((config1 >> 7) & 7);
	*size = lsz * sets * ways;
	*lsize = lsz;
}

#define cache_unroll(base,op)	        	\
	__asm__ __volatile__("	         	\
		.set noreorder;		        \
		.set mips3;		        \
                cache %1, (%0);	                \
		.set mips0;			\
		.set reorder"			\
		:				\
		: "r" (base),			\
		  "i" (op));

#endif /* !_LANGUAGE_ASSEMBLY */


/*
 * R4x00 interrupt enable / cause bits
 */
#undef IE_SW0
#undef IE_SW1
#undef IE_IRQ0
#undef IE_IRQ1
#undef IE_IRQ2
#undef IE_IRQ3
#undef IE_IRQ4
#undef IE_IRQ5
#define IE_SW0		(1<< 8)
#define IE_SW1		(1<< 9)
#define IE_IRQ0		(1<<10)
#define IE_IRQ1		(1<<11)
#define IE_IRQ2		(1<<12)
#define IE_IRQ3		(1<<13)
#define IE_IRQ4		(1<<14)
#define IE_IRQ5		(1<<15)

/*
 * Bitfields in the mips32 cp0 status register
 */
#define ST0_IE			0x00000001
#define ST0_EXL			0x00000002
#define ST0_ERL			0x00000004
/* already defined
#define ST0_UM			0x00000010
#define ST0_SWINT0		0x00000100
#define ST0_SWINT1		0x00000200
*/
#define ST0_HWINT0		0x00000400
#define ST0_HWINT1		0x00000800
#define ST0_HWINT2		0x00001000
#define ST0_HWINT3		0x00002000
#define ST0_HWINT4		0x00004000
#define ST0_HWINT5		0x00008000
#define ST0_IM			0x0000ff00
#define ST0_NMI			0x00080000
#define ST0_SR			0x00100000
#define ST0_TS			0x00200000
#define ST0_BEV			0x00400000
#define ST0_RE			0x02000000
#define ST0_RP			0x08000000
#define ST0_CU			0xf0000000
#define ST0_CU0			0x10000000
#define ST0_CU1			0x20000000
#define ST0_CU2			0x40000000
#define ST0_CU3			0x80000000


/*
 * Bitfields in the mips32 cp0 cause register
 */
#define C_EXC			0x0000007c
#define C_EXC_SHIFT		2
#define C_INT			0x0000ff00
#define C_INT_SHIFT		8
/* already defined
#define C_SW0			0x00000100
#define C_SW1			0x00000200
#define C_IRQ0			0x00000400
#define C_IRQ1			0x00000800
#define C_IRQ2			0x00001000
#define C_IRQ3			0x00002000
#define C_IRQ4			0x00004000
#define C_IRQ5			0x00008000
*/
#define C_WP			0x00400000
#define C_IV			0x00800000
#define C_CE			0x30000000
#define C_CE_SHIFT		28
#define C_BD			0x80000000

/* Values in C_EXC */
#define EXC_INT			0
#define EXC_TLBM		1
#define EXC_TLBL		2
#define EXC_TLBS		3
#define EXC_AEL			4
#define EXC_AES			5
#define EXC_IBE			6
#define EXC_DBE			7
#define EXC_SYS			8
#define EXC_BPT			9
#define EXC_RI			10
#define EXC_CU			11
#define EXC_OV			12
#define EXC_TR			13
#define EXC_WATCH		23
#define EXC_MCHK		24


/*
 * Bits in the cp0 config register.
 */
#define CONF_CM_CACHABLE_NO_WA		0
#define CONF_CM_CACHABLE_WA		1
#define CONF_CM_UNCACHED		2
#define CONF_CM_CACHABLE_NONCOHERENT	3
#define CONF_CM_CACHABLE_CE		4
#define CONF_CM_CACHABLE_COW		5
#define CONF_CM_CACHABLE_CUW		6
#define CONF_CM_CACHABLE_ACCELERATED	7
#define CONF_CM_CMASK			7
#define CONF_CU				(_ULCAST_(1) <<  3)
#define CONF_DB				(_ULCAST_(1) <<  4)
#define CONF_IB				(_ULCAST_(1) <<  5)
#define CONF_SE				(_ULCAST_(1) << 12)
#define CONF_SC				(_ULCAST_(1) << 17)
#define CONF_AC				(_ULCAST_(1) << 23)
#define CONF_HALT			(_ULCAST_(1) << 25)


/*
 * Bits in the cp0 config register select 1.
 */
#define CONF1_FP			0x00000001		/* FPU present */
#define CONF1_EP			0x00000002		/* EJTAG present */
#define CONF1_CA			0x00000004		/* mips16 implemented */
#define CONF1_WR			0x00000008		/* Watch registers present */
#define CONF1_PC			0x00000010		/* Performance counters present */
#define	CONF1_DA_SHIFT			7			/* D$ associativity */
#define CONF1_DA_MASK			0x00000380
#define CONF1_DA_BASE			1
#define CONF1_DL_SHIFT			10			/* D$ line size */
#define CONF1_DL_MASK			0x00001c00
#define CONF1_DL_BASE			2
#define CONF1_DS_SHIFT			13			/* D$ sets/way */
#define CONF1_DS_MASK			0x0000e000
#define CONF1_DS_BASE			64
#define CONF1_IA_SHIFT			16			/* I$ associativity */
#define CONF1_IA_MASK			0x00070000
#define CONF1_IA_BASE			1
#define CONF1_IL_SHIFT			19			/* I$ line size */
#define CONF1_IL_MASK			0x00380000
#define CONF1_IL_BASE			2
#define CONF1_IS_SHIFT			22			/* Instruction cache sets/way */
#define CONF1_IS_MASK			0x01c00000
#define CONF1_IS_BASE			64
#define CONF1_MS_MASK			0x7e000000		/* Number of tlb entries */
#define CONF1_MS_SHIFT			25

/* PRID register */
#define PRID_COPT_MASK		0xff000000
#define PRID_COMP_MASK		0x00ff0000
#define PRID_IMP_MASK		0x0000ff00
#define PRID_REV_MASK		0x000000ff

#define PRID_COMP_LEGACY	0x000000
#define PRID_COMP_MIPS		0x010000
#define PRID_COMP_BROADCOM	0x020000
#define PRID_COMP_ALCHEMY	0x030000
#define PRID_COMP_SIBYTE	0x040000
#define PRID_IMP_BCM4710	0x4000
#define PRID_IMP_BCM3302	0x9000
#define PRID_IMP_BCM3303	0x9100
#define	PRID_IMP_BCM3303	0x9100

#define PRID_IMP_UNKNOWN	0xff00

#define       BCM330X(id) \
	(((id & (PRID_COMP_MASK | PRID_IMP_MASK)) == (PRID_COMP_BROADCOM | PRID_IMP_BCM3302)) \
	|| ((id & (PRID_COMP_MASK | PRID_IMP_MASK)) == (PRID_COMP_BROADCOM | PRID_IMP_BCM3303)))

/* Bits in C0_BROADCOM */
#define	BRCM_PFC_AVAIL		0x20000000	/* PFC is available */
#define	BRCM_DC_ENABLE		0x40000000	/* Enable Data $ */
#define	BRCM_IC_ENABLE		0x80000000	/* Enable Instruction $ */
#define	BRCM_PFC_ENABLE		0x00400000	/* Obsolete? Enable PFC (at least on 4310) */

/* PreFetch Cache aka Read Ahead Cache */

#define	PFC_CR0		0xff400000	/* control reg 0 */
#define	PFC_CR1		0xff400004	/* control reg 1 */

/* 
 * These are the UART port assignments, expressed as offsets from the base
 * register.  These assignments should hold for any serial port based on
 * a 8250, 16450, or 16550(A).
 */

#define UART_RX		0	/* In:  Receive buffer (DLAB=0) */
#define UART_TX		0	/* Out: Transmit buffer (DLAB=0) */
#define UART_DLL	0	/* Out: Divisor Latch Low (DLAB=1) */
#define UART_DLM	1	/* Out: Divisor Latch High (DLAB=1) */
#define UART_LCR	3	/* Out: Line Control Register */
#define UART_MCR	4	/* Out: Modem Control Register */
#define UART_LSR	5	/* In:  Line Status Register */
#define UART_MSR	6	/* In:  Modem Status Register */
#define UART_SCR	7	/* I/O: Scratch Register */
#define UART_LCR_DLAB	0x80	/* Divisor latch access bit */
#define UART_LCR_WLEN8  0x03	/* Wordlength: 8 bits */
#define UART_MCR_LOOP	0x10	/* Enable loopback test mode */
#define UART_LSR_THRE	0x20	/* Transmit-hold-register empty */
#define UART_LSR_RXRDY	0x01	/* Receiver ready */


#endif	/* _MISPINC_H */
