#ifndef _FRIONOMMU_BYTEORDER_H
#define _FRIONOMMU_BYTEORDER_H

#include <asm/types.h>

#if defined(__GNUC__) && !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#include <linux/byteorder/little_endian.h>

#endif /* _FRIONOMMU_BYTEORDER_H */

