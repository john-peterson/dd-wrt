#ifndef __ASM_SH_BYTEORDER_H
#define __ASM_SH_BYTEORDER_H

/*
 * Copyright (C) 1999  Niibe Yutaka
 */

#include <asm/types.h>

static __inline__ __const__ __u32 ___arch__swab32(__u32 x)
{
	__asm__("swap.b	%0, %0\n\t"
		"swap.w %0, %0\n\t"
		"swap.b %0, %0"
		: "=r" (x)
		: "0" (x));
	return x;
}

static __inline__ __const__ __u16 ___arch__swab16(__u16 x)
{
	__asm__("swap.b %0, %0"
		: "=r" (x)
		:  "0" (x));
	return x;
}

#define __arch__swab32(x) ___arch__swab32(x)
#define __arch__swab16(x) ___arch__swab16(x)

#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#ifdef __LITTLE_ENDIAN__
#include <linux/byteorder/little_endian.h>
#else
#include <linux/byteorder/big_endian.h>
#endif

#endif /* __ASM_SH_BYTEORDER_H */
