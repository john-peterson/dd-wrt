/*
 *	md5.c
 *	Release $Name: MATRIXSSL_1_8_2_OPEN $
 *
 *	MD5 hash implementation
 */
/*
 *	Copyright (c) PeerSec Networks, 2002-2006. All Rights Reserved.
 *	The latest version of this code is available at http://www.matrixssl.org
 *
 *	This software is open source; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This General Public License does NOT permit incorporating this software 
 *	into proprietary programs.  If you are unable to comply with the GPL, a 
 *	commercial license for this software may be purchased from PeerSec Networks
 *	at http://www.peersec.com
 *	
 *	This program is distributed in WITHOUT ANY WARRANTY; without even the 
 *	implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *	See the GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *	http://www.gnu.org/copyleft/gpl.html
 */
/******************************************************************************/

#include "../cryptoLayer.h"

#define F(x,y,z)	(z ^ (x & (y ^ z)))
#define G(x,y,z)	(y ^ (z & (y ^ x)))
#define H(x,y,z)	(x^y^z)
#define I(x,y,z)	(y^(x|(~z)))

#ifdef SMALL_CODE

#define FF(a,b,c,d,M,s,t) \
	a = (a + F(b,c,d) + M + t); a = ROL(a, s) + b;

#define GG(a,b,c,d,M,s,t) \
	a = (a + G(b,c,d) + M + t); a = ROL(a, s) + b;

#define HH(a,b,c,d,M,s,t) \
	a = (a + H(b,c,d) + M + t); a = ROL(a, s) + b;

#define II(a,b,c,d,M,s,t) \
	a = (a + I(b,c,d) + M + t); a = ROL(a, s) + b;

static const unsigned char Worder[64] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
	5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
	0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9
};

static const unsigned char Rorder[64] = {
	7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
	5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
	4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
	6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};

static const ulong32 Korder[] = {
	0xd76aa478UL, 0xe8c7b756UL, 0x242070dbUL, 0xc1bdceeeUL,
	0xf57c0fafUL, 0x4787c62aUL, 0xa8304613UL, 0xfd469501UL,
	0x698098d8UL, 0x8b44f7afUL, 0xffff5bb1UL, 0x895cd7beUL,
	0x6b901122UL, 0xfd987193UL, 0xa679438eUL, 0x49b40821UL,
	0xf61e2562UL, 0xc040b340UL, 0x265e5a51UL, 0xe9b6c7aaUL,
	0xd62f105dUL, 0x02441453UL, 0xd8a1e681UL, 0xe7d3fbc8UL,
	0x21e1cde6UL, 0xc33707d6UL, 0xf4d50d87UL, 0x455a14edUL,
	0xa9e3e905UL, 0xfcefa3f8UL, 0x676f02d9UL, 0x8d2a4c8aUL,
	0xfffa3942UL, 0x8771f681UL, 0x6d9d6122UL, 0xfde5380cUL,
	0xa4beea44UL, 0x4bdecfa9UL, 0xf6bb4b60UL, 0xbebfbc70UL,
	0x289b7ec6UL, 0xeaa127faUL, 0xd4ef3085UL, 0x04881d05UL,
	0xd9d4d039UL, 0xe6db99e5UL, 0x1fa27cf8UL, 0xc4ac5665UL,
	0xf4292244UL, 0x432aff97UL, 0xab9423a7UL, 0xfc93a039UL,
	0x655b59c3UL, 0x8f0ccc92UL, 0xffeff47dUL, 0x85845dd1UL,
	0x6fa87e4fUL, 0xfe2ce6e0UL, 0xa3014314UL, 0x4e0811a1UL,
	0xf7537e82UL, 0xbd3af235UL, 0x2ad7d2bbUL, 0xeb86d391UL,
	0xe1f27f3aUL, 0xf5710fb0UL, 0xada0e5c4UL, 0x98e4c919UL
 };
#else /* SMALL_CODE */

#define FF(a,b,c,d,M,s,t) \
	a = (a + F(b,c,d) + M + t); a = ROLc(a, s) + b;

#define GG(a,b,c,d,M,s,t) \
	a = (a + G(b,c,d) + M + t); a = ROLc(a, s) + b;

#define HH(a,b,c,d,M,s,t) \
	a = (a + H(b,c,d) + M + t); a = ROLc(a, s) + b;

#define II(a,b,c,d,M,s,t) \
    a = (a + I(b,c,d) + M + t); a = ROLc(a, s) + b;

#endif /* SMALL_CODE */

#ifdef CLEAN_STACK
static void _md5_compress(hash_state *md)
#else
static void md5_compress(hash_state *md)
#endif /* CLEAN_STACK */
{
	unsigned long	i, W[16], a, b, c, d;
#ifdef SMALL_CODE
	ulong32			t;
#endif

	sslAssert(md != NULL);

/*
	copy the state into 512-bits into W[0..15]
 */
	for (i = 0; i < 16; i++) {
		LOAD32L(W[i], md->md5.buf + (4*i));
	}

/*
	copy state
 */
	a = md->md5.state[0];
	b = md->md5.state[1];
	c = md->md5.state[2];
	d = md->md5.state[3];

#ifdef SMALL_CODE
	for (i = 0; i < 16; ++i) {
		FF(a,b,c,d,W[Worder[i]],Rorder[i],Korder[i]);
		t = d; d = c; c = b; b = a; a = t;
	}

	for (; i < 32; ++i) {
		GG(a,b,c,d,W[Worder[i]],Rorder[i],Korder[i]);
		t = d; d = c; c = b; b = a; a = t;
	}

	for (; i < 48; ++i) {
		HH(a,b,c,d,W[Worder[i]],Rorder[i],Korder[i]);
		t = d; d = c; c = b; b = a; a = t;
	}

	for (; i < 64; ++i) {
		II(a,b,c,d,W[Worder[i]],Rorder[i],Korder[i]);
		t = d; d = c; c = b; b = a; a = t;
	}

#else /* SMALL_CODE */

	FF(a,b,c,d,W[0],7,0xd76aa478UL)
	FF(d,a,b,c,W[1],12,0xe8c7b756UL)
	FF(c,d,a,b,W[2],17,0x242070dbUL)
	FF(b,c,d,a,W[3],22,0xc1bdceeeUL)
	FF(a,b,c,d,W[4],7,0xf57c0fafUL)
	FF(d,a,b,c,W[5],12,0x4787c62aUL)
	FF(c,d,a,b,W[6],17,0xa8304613UL)
	FF(b,c,d,a,W[7],22,0xfd469501UL)
	FF(a,b,c,d,W[8],7,0x698098d8UL)
	FF(d,a,b,c,W[9],12,0x8b44f7afUL)
	FF(c,d,a,b,W[10],17,0xffff5bb1UL)
	FF(b,c,d,a,W[11],22,0x895cd7beUL)
	FF(a,b,c,d,W[12],7,0x6b901122UL)
	FF(d,a,b,c,W[13],12,0xfd987193UL)
	FF(c,d,a,b,W[14],17,0xa679438eUL)
	FF(b,c,d,a,W[15],22,0x49b40821UL)
	GG(a,b,c,d,W[1],5,0xf61e2562UL)
	GG(d,a,b,c,W[6],9,0xc040b340UL)
	GG(c,d,a,b,W[11],14,0x265e5a51UL)
	GG(b,c,d,a,W[0],20,0xe9b6c7aaUL)
	GG(a,b,c,d,W[5],5,0xd62f105dUL)
	GG(d,a,b,c,W[10],9,0x02441453UL)
	GG(c,d,a,b,W[15],14,0xd8a1e681UL)
	GG(b,c,d,a,W[4],20,0xe7d3fbc8UL)
	GG(a,b,c,d,W[9],5,0x21e1cde6UL)
	GG(d,a,b,c,W[14],9,0xc33707d6UL)
	GG(c,d,a,b,W[3],14,0xf4d50d87UL)
	GG(b,c,d,a,W[8],20,0x455a14edUL)
	GG(a,b,c,d,W[13],5,0xa9e3e905UL)
	GG(d,a,b,c,W[2],9,0xfcefa3f8UL)
	GG(c,d,a,b,W[7],14,0x676f02d9UL)
	GG(b,c,d,a,W[12],20,0x8d2a4c8aUL)
	HH(a,b,c,d,W[5],4,0xfffa3942UL)
	HH(d,a,b,c,W[8],11,0x8771f681UL)
	HH(c,d,a,b,W[11],16,0x6d9d6122UL)
	HH(b,c,d,a,W[14],23,0xfde5380cUL)
	HH(a,b,c,d,W[1],4,0xa4beea44UL)
	HH(d,a,b,c,W[4],11,0x4bdecfa9UL)
	HH(c,d,a,b,W[7],16,0xf6bb4b60UL)
	HH(b,c,d,a,W[10],23,0xbebfbc70UL)
	HH(a,b,c,d,W[13],4,0x289b7ec6UL)
	HH(d,a,b,c,W[0],11,0xeaa127faUL)
	HH(c,d,a,b,W[3],16,0xd4ef3085UL)
	HH(b,c,d,a,W[6],23,0x04881d05UL)
	HH(a,b,c,d,W[9],4,0xd9d4d039UL)
	HH(d,a,b,c,W[12],11,0xe6db99e5UL)
	HH(c,d,a,b,W[15],16,0x1fa27cf8UL)
	HH(b,c,d,a,W[2],23,0xc4ac5665UL)
	II(a,b,c,d,W[0],6,0xf4292244UL)
	II(d,a,b,c,W[7],10,0x432aff97UL)
	II(c,d,a,b,W[14],15,0xab9423a7UL)
	II(b,c,d,a,W[5],21,0xfc93a039UL)
	II(a,b,c,d,W[12],6,0x655b59c3UL)
	II(d,a,b,c,W[3],10,0x8f0ccc92UL)
	II(c,d,a,b,W[10],15,0xffeff47dUL)
	II(b,c,d,a,W[1],21,0x85845dd1UL)
	II(a,b,c,d,W[8],6,0x6fa87e4fUL)
	II(d,a,b,c,W[15],10,0xfe2ce6e0UL)
	II(c,d,a,b,W[6],15,0xa3014314UL)
	II(b,c,d,a,W[13],21,0x4e0811a1UL)
	II(a,b,c,d,W[4],6,0xf7537e82UL)
	II(d,a,b,c,W[11],10,0xbd3af235UL)
	II(c,d,a,b,W[2],15,0x2ad7d2bbUL)
	II(b,c,d,a,W[9],21,0xeb86d391UL)
#endif /* SMALL_CODE */

	md->md5.state[0] = md->md5.state[0] + a;
	md->md5.state[1] = md->md5.state[1] + b;
	md->md5.state[2] = md->md5.state[2] + c;
	md->md5.state[3] = md->md5.state[3] + d;
}

#ifdef CLEAN_STACK
static void md5_compress(hash_state *md)
{
	_md5_compress(md);
	psBurnStack(sizeof(unsigned long) * 21);
}
#endif /* CLEAN_STACK */

void matrixMd5Init(hash_state * md)
{
	sslAssert(md != NULL);
	md->md5.state[0] = 0x67452301UL;
	md->md5.state[1] = 0xefcdab89UL;
	md->md5.state[2] = 0x98badcfeUL;
	md->md5.state[3] = 0x10325476UL;
	md->md5.curlen = 0;
#ifdef USE_INT64
	md->md5.length = 0;
#else
	md->md5.lengthHi = 0;
	md->md5.lengthLo = 0;
#endif /* USE_INT64 */
}

void matrixMd5Update(hash_state * md, const unsigned char *buf, unsigned long len)
{
	unsigned long n;

	sslAssert(md != NULL);
	sslAssert(buf != NULL);
	while (len > 0) {
		n = MIN(len, (64 - md->md5.curlen));
		memcpy(md->md5.buf + md->md5.curlen, buf, (size_t)n);
		md->md5.curlen	+= n;
		buf				+= n;
		len				-= n;

/*
		is 64 bytes full?
 */
		if (md->md5.curlen == 64) {
			md5_compress(md);
#ifdef USE_INT64
			md->md5.length += 512;
#else
			n = (md->md5.lengthLo + 512) & 0xFFFFFFFFL;
			if (n < md->md5.lengthLo) {
				md->md5.lengthHi++;
			}
			md->md5.lengthLo = n;
#endif /* USE_INT64 */
			md->md5.curlen = 0;
		}
	}
}

int32 matrixMd5Final(hash_state * md, unsigned char *hash)
{
	int32 i;
#ifndef USE_INT64
	unsigned long	n;
#endif

	sslAssert(md != NULL);
	if (hash == NULL) {
		return -1;
	}

/*
	increase the length of the message
 */
#ifdef USE_INT64
	md->md5.length += md->md5.curlen << 3;
#else
	n = (md->md5.lengthLo + (md->md5.curlen << 3)) & 0xFFFFFFFFL;
	if (n < md->md5.lengthLo) {
		md->md5.lengthHi++;
	}
	md->md5.lengthHi += (md->md5.curlen >> 29);
	md->md5.lengthLo = n;
#endif /* USE_INT64 */

/*
	append the '1' bit
 */
	md->md5.buf[md->md5.curlen++] = (unsigned char)0x80;

/*
	if the length is currently above 56 bytes we append zeros then compress.
	Then we can fall back to padding zeros and length encoding like normal.
 */
	if (md->md5.curlen > 56) {
		while (md->md5.curlen < 64) {
			md->md5.buf[md->md5.curlen++] = (unsigned char)0;
		}
		md5_compress(md);
		md->md5.curlen = 0;
	}

/*
	pad upto 56 bytes of zeroes
 */
	while (md->md5.curlen < 56) {
		md->md5.buf[md->md5.curlen++] = (unsigned char)0;
	}
/*
	store length
 */
#ifdef USE_INT64
	STORE64L(md->md5.length, md->md5.buf+56);
#else
	STORE32L(md->md5.lengthLo, md->md5.buf+56);
	STORE32L(md->md5.lengthHi, md->md5.buf+60);
#endif /* USE_INT64 */
	md5_compress(md);

/*
	copy output
 */
	for (i = 0; i < 4; i++) {
		STORE32L(md->md5.state[i], hash+(4*i));
	}
#ifdef CLEAN_STACK
	psZeromem(md, sizeof(hash_state));
#endif /* CLEAN_STACK */
	return 16;
}

#ifdef PEERSEC_TEST

int32 matrixMd5Test()
{
	static const struct {
		char *msg;
		unsigned char hash[16];
	} tests[] = {
		{ "",
		{ 0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 
			0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e } },
		{ "a",
		{0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 
			0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61 } },
		{ "abc",
		{ 0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 
			0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72 } },
		{ "message digest", 
		{ 0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d, 
			0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0 } }, 
		{ "abcdefghijklmnopqrstuvwxyz",
		{ 0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00, 
			0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b } },
		{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		{ 0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5, 
			0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f } },
		{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		{ 0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55, 
			0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a } }, 
		{ NULL, { 0 } }
	};

	int32 i;
	unsigned char tmp[16];
	hash_state md;

	for (i = 0; tests[i].msg != NULL; i++) {
		matrixMd5Init(&md);
		matrixMd5Update(&md, (unsigned char *)tests[i].msg, (unsigned long)strlen(tests[i].msg));
		matrixMd5Final(&md, tmp);
		if (memcmp(tmp, tests[i].hash, 16) != 0) {
			return CRYPT_FAIL_TESTVECTOR;
		}
	}
	return CRYPT_OK;
}
#endif /* PEERSEC_TEST */

/******************************************************************************/
