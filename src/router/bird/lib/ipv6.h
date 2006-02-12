/*
 *	BIRD -- IP Addresses et Cetera for IPv6
 *
 *	(c) 1999--2000 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#ifndef _BIRD_IPV6_H_
#define _BIRD_IPV6_H_

#include <sys/types.h>
#include <netinet/in.h>
#include "lib/string.h"

typedef struct ipv6_addr {
  u32 addr[4];
} ip_addr;

#define _MI(a,b,c,d) ((struct ipv6_addr) {{ a, b, c, d }})
#define _I0(a) ((a).addr[0])
#define _I1(a) ((a).addr[1])
#define _I2(a) ((a).addr[2])
#define _I3(a) ((a).addr[3])

#define BITS_PER_IP_ADDRESS 128
#define STD_ADDRESS_P_LENGTH 39
#define SIZE_OF_IP_HEADER 40

#define IPA_NONE _MI(0,0,0,0)

#define ipa_equal(x,y) (!memcmp(&(x),&(y),sizeof(ip_addr)))
#define ipa_nonzero(x) ({ ip_addr _a=(x); (_I0(_a) || _I1(_a) || _I2(_a) || _I3(_a)); })
#define ipa_and(x,y) ({ ip_addr _a=(x), _b=(y); \
		     _MI(_I0(_a) & _I0(_b), \
			 _I1(_a) & _I1(_b), \
			 _I2(_a) & _I2(_b), \
			 _I3(_a) & _I3(_b)); })
#define ipa_or(x,y)  ({ ip_addr _a=(x), _b=(y); \
		     _MI(_I0(_a) | _I0(_b), \
			 _I1(_a) | _I1(_b), \
			 _I2(_a) | _I2(_b), \
			 _I3(_a) | _I3(_b)); })
#define ipa_xor(x,y) ({ ip_addr _a=(x), _b=(y); \
		     _MI(_I0(_a) ^ _I0(_b), \
			 _I1(_a) ^ _I1(_b), \
			 _I2(_a) ^ _I2(_b), \
			 _I3(_a) ^ _I3(_b)); })
#define ipa_not(x) ({ ip_addr _a=(x); _MI(~_I0(_a),~_I1(_a),~_I2(_a),~_I3(_a)); })
#define ipa_mkmask(x) ipv6_mkmask(x)
#define ipa_mklen(x) ipv6_mklen(&(x))
#define ipa_hash(x) ipv6_hash(&(x))
#define ipa_hton(x) ipv6_hton(&(x))
#define ipa_ntoh(x) ipv6_ntoh(&(x))
#define ipa_classify(x) ipv6_classify(&(x))
/* ipa_opposite and ipa_class_mask don't make sense with IPv6 */
/* ipa_from_u32 and ipa_to_u32 replaced by ipa_build */
#define ipa_build(a,b,c,d) _MI(a,b,c,d)
#define ipa_compare(x,y) ipv6_compare(x,y)
#define ipa_absolutize(x,y) ipv6_absolutize(x,y)

ip_addr ipv6_mkmask(unsigned);
unsigned ipv6_mklen(ip_addr *);
int ipv6_classify(ip_addr *);
void ipv6_hton(ip_addr *);
void ipv6_ntoh(ip_addr *);
int ipv6_compare(ip_addr, ip_addr);
int ipv4_pton_u32(char *, u32 *);
void ipv6_absolutize(ip_addr *, ip_addr *);

/*
 *  This hash function looks well, but once IPv6 enters
 *  mainstream use, we need to check that it has good
 *  distribution properties on real routing tables.
 */

static inline unsigned ipv6_hash(ip_addr *a)
{
  /* Returns a 16-bit hash key */
  u32 x = _I0(*a) ^ _I1(*a) ^ _I2(*a) ^ _I3(*a);
  return (x ^ (x >> 16) ^ (x >> 8)) & 0xffff;
}

/*
 *  RFC 1883 defines packet precendece, but RFC 2460 replaces it
 *  by generic Traffic Class ID with no defined semantics. Better
 *  not use it yet.
 */
#define IP_PREC_INTERNET_CONTROL -1

#endif
