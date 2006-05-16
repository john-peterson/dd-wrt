#ifndef _IPT_SAME_H
#define _IPT_SAME_H

#define IPT_SAME_MAX_RANGE	10

#define IPT_SAME_NODST		0x01

struct ipt_same_info
{
	unsigned char info;
	u_int32_t rangesize;
	u_int32_t ipnum;
#ifdef KERNEL_64_USERSPACE_32
	u_int64_t placeholder;
#else
	u_int32_t *iparray;
#endif

	/* hangs off end. */
	struct ip_nat_range range[IPT_SAME_MAX_RANGE];
};

#endif /*_IPT_SAME_H*/
