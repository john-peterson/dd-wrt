/*
 * include/asm-arm/dec21285.h
 *
 * Copyright (C) 1998 Russell King
 *
 * DC21285 registers
 */
#define DC21285_PCI_IACK		0x79000000
#define DC21285_ARMCSR_BASE		0x42000000
#define DC21285_PCI_TYPE_0_CONFIG	0x7b000000
#define DC21285_PCI_TYPE_1_CONFIG	0x7a000000
#define DC21285_OUTBOUND_WRITE_FLUSH	0x78000000
#define DC21285_FLASH			0x41000000
#define DC21285_PCI_IO			0x7c000000
#define DC21285_PCI_MEM			0x80000000

#include <linux/config.h>
#ifndef __ASSEMBLY__
#include <asm/arch/hardware.h>
#define DC21285_IO(x)		((volatile unsigned long *)(ARMCSR_BASE+(x)))
#else
#define DC21285_IO(x)		(x)
#endif

#define CSR_PCICMD		DC21285_IO(0x0004)
#define CSR_CLASSREV		DC21285_IO(0x0008)
#define CSR_PCICACHELINESIZE	DC21285_IO(0x000c)
#define CSR_PCICSRBASE		DC21285_IO(0x0010)
#define CSR_PCICSRIOBASE	DC21285_IO(0x0014)
#define CSR_PCISDRAMBASE	DC21285_IO(0x0018)
#define CSR_PCIROMBASE		DC21285_IO(0x0030)
#define CSR_MBOX0		DC21285_IO(0x0050)
#define CSR_MBOX1		DC21285_IO(0x0054)
#define CSR_MBOX2		DC21285_IO(0x0058)
#define CSR_MBOX3		DC21285_IO(0x005c)
#define CSR_DOORBELL		DC21285_IO(0x0060)
#define CSR_DOORBELL_SETUP	DC21285_IO(0x0064)
#define CSR_ROMWRITEREG		DC21285_IO(0x0068)
#define CSR_CSRBASEMASK		DC21285_IO(0x00f8)
#define CSR_CSRBASEOFFSET	DC21285_IO(0x00fc)
#define CSR_SDRAMBASEMASK	DC21285_IO(0x0100)
#define CSR_SDRAMBASEOFFSET	DC21285_IO(0x0104)
#define CSR_ROMBASEMASK		DC21285_IO(0x0108)
#define CSR_SDRAMTIMING		DC21285_IO(0x010c)
#define CSR_SDRAMADDRSIZE0	DC21285_IO(0x0110)
#define CSR_SDRAMADDRSIZE1	DC21285_IO(0x0114)
#define CSR_SDRAMADDRSIZE2	DC21285_IO(0x0118)
#define CSR_SDRAMADDRSIZE3	DC21285_IO(0x011c)
#define CSR_I2O_INFREEHEAD	DC21285_IO(0x0120)
#define CSR_I2O_INPOSTTAIL	DC21285_IO(0x0124)
#define CSR_I2O_OUTPOSTHEAD	DC21285_IO(0x0128)
#define CSR_I2O_OUTFREETAIL	DC21285_IO(0x012c)
#define CSR_I2O_INFREECOUNT	DC21285_IO(0x0130)
#define CSR_I2O_OUTPOSTCOUNT	DC21285_IO(0x0134)
#define CSR_I2O_INPOSTCOUNT	DC21285_IO(0x0138)
#define CSR_SA110_CNTL		DC21285_IO(0x013c)
#define SA110_CNTL_INITCMPLETE		(1 << 0)
#define SA110_CNTL_ASSERTSERR		(1 << 1)
#define SA110_CNTL_RXSERR		(1 << 3)
#define SA110_CNTL_SA110DRAMPARITY	(1 << 4)
#define SA110_CNTL_PCISDRAMPARITY	(1 << 5)
#define SA110_CNTL_DMASDRAMPARITY	(1 << 6)
#define SA110_CNTL_DISCARDTIMER		(1 << 8)
#define SA110_CNTL_PCINRESET		(1 << 9)
#define SA110_CNTL_I2O_256		(0 << 10)
#define SA110_CNTL_I20_512		(1 << 10)
#define SA110_CNTL_I2O_1024		(2 << 10)
#define SA110_CNTL_I2O_2048		(3 << 10)
#define SA110_CNTL_I2O_4096		(4 << 10)
#define SA110_CNTL_I2O_8192		(5 << 10)
#define SA110_CNTL_I2O_16384		(6 << 10)
#define SA110_CNTL_I2O_32768		(7 << 10)
#define SA110_CNTL_WATCHDOG		(1 << 13)
#define SA110_CNTL_ROMWIDTH_UNDEF	(0 << 14)
#define SA110_CNTL_ROMWIDTH_16		(1 << 14)
#define SA110_CNTL_ROMWIDTH_32		(2 << 14)
#define SA110_CNTL_ROMWIDTH_8		(3 << 14)
#define SA110_CNTL_ROMACCESSTIME(x)	((x)<<16)
#define SA110_CNTL_ROMBURSTTIME(x)	((x)<<20)
#define SA110_CNTL_ROMTRISTATETIME(x)	((x)<<24)
#define SA110_CNTL_XCSDIR(x)		((x)<<28)
#define SA110_CNTL_PCICFN		(1 << 31)

/*
 * footbridge_cfn_mode() is used when we want
 * to check whether we are the central function
 */
#define __footbridge_cfn_mode() (*CSR_SA110_CNTL & SA110_CNTL_PCICFN)
#if defined(CONFIG_FOOTBRIDGE_HOST) && defined(CONFIG_FOOTBRIDGE_ADDIN)
#define footbridge_cfn_mode() __footbridge_cfn_mode()
#elif defined(CONFIG_FOOTBRIDGE_HOST)
#define footbridge_cfn_mode() (1)
#else
#define footbridge_cfn_mode() (0)
#endif

#define CSR_PCIADDR_EXTN	DC21285_IO(0x0140)
#define CSR_PREFETCHMEMRANGE	DC21285_IO(0x0144)
#define CSR_XBUS_CYCLE		DC21285_IO(0x0148)
#define CSR_XBUS_IOSTROBE	DC21285_IO(0x014c)
#define CSR_DOORBELL_PCI	DC21285_IO(0x0150)
#define CSR_DOORBELL_SA110	DC21285_IO(0x0154)
#define CSR_UARTDR		DC21285_IO(0x0160)
#define CSR_RXSTAT		DC21285_IO(0x0164)
#define CSR_H_UBRLCR		DC21285_IO(0x0168)
#define CSR_M_UBRLCR		DC21285_IO(0x016c)
#define CSR_L_UBRLCR		DC21285_IO(0x0170)
#define CSR_UARTCON		DC21285_IO(0x0174)
#define CSR_UARTFLG		DC21285_IO(0x0178)
#define CSR_IRQ_STATUS		DC21285_IO(0x0180)
#define CSR_IRQ_RAWSTATUS	DC21285_IO(0x0184)
#define CSR_IRQ_ENABLE		DC21285_IO(0x0188)
#define CSR_IRQ_DISABLE		DC21285_IO(0x018c)
#define CSR_IRQ_SOFT		DC21285_IO(0x0190)
#define CSR_FIQ_STATUS		DC21285_IO(0x0280)
#define CSR_FIQ_RAWSTATUS	DC21285_IO(0x0284)
#define CSR_FIQ_ENABLE		DC21285_IO(0x0288)
#define CSR_FIQ_DISABLE		DC21285_IO(0x028c)
#define CSR_FIQ_SOFT		DC21285_IO(0x0290)
#define CSR_TIMER1_LOAD		DC21285_IO(0x0300)
#define CSR_TIMER1_VALUE	DC21285_IO(0x0304)
#define CSR_TIMER1_CNTL		DC21285_IO(0x0308)
#define CSR_TIMER1_CLR		DC21285_IO(0x030c)
#define CSR_TIMER2_LOAD		DC21285_IO(0x0320)
#define CSR_TIMER2_VALUE	DC21285_IO(0x0324)
#define CSR_TIMER2_CNTL		DC21285_IO(0x0328)
#define CSR_TIMER2_CLR		DC21285_IO(0x032c)
#define CSR_TIMER3_LOAD		DC21285_IO(0x0340)
#define CSR_TIMER3_VALUE	DC21285_IO(0x0344)
#define CSR_TIMER3_CNTL		DC21285_IO(0x0348)
#define CSR_TIMER3_CLR		DC21285_IO(0x034c)
#define CSR_TIMER4_LOAD		DC21285_IO(0x0360)
#define CSR_TIMER4_VALUE	DC21285_IO(0x0364)
#define CSR_TIMER4_CNTL		DC21285_IO(0x0368)
#define CSR_TIMER4_CLR		DC21285_IO(0x036c)

#define TIMER_CNTL_ENABLE	(1 << 7)
#define TIMER_CNTL_AUTORELOAD	(1 << 6)
#define TIMER_CNTL_DIV1		(0)
#define TIMER_CNTL_DIV16	(1 << 2)
#define TIMER_CNTL_DIV256	(2 << 2)
#define TIMER_CNTL_CNTEXT	(3 << 2)


