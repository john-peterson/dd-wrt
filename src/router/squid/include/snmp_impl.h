/*
 * $Id: snmp_impl.h,v 1.15 2003/01/23 00:36:48 robertc Exp $
 */

#ifndef SQUID_SNMP_IMPL_H
#define SQUID_SNMP_IMPL_H

/*
 * Definitions for SNMP (RFC 1067) implementation.
 *
 *
 */
/***********************************************************
	Copyright 1988, 1989 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of CMU not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/

#if 0
#if (defined vax) || (defined (mips))
/*
 * This is a fairly bogus thing to do, but there seems to be no better way for
 * compilers that don't understand void pointers.
 */
#ifndef void
#define void char
#endif
#endif
#endif

#define SID_MAX_LEN	64

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#define READ	    1
#define WRITE	    0

#define SNMP_RESERVE1    0
#define SNMP_RESERVE2    1
#define SNMP_COMMIT      2
#define SNMP_FREE        3

#define RONLY	0xAAAA		/* read access for everyone */
#define RWRITE	0xAABA		/* add write access for community private */
#define NOACCESS 0x0000		/* no access for anybody */

struct trapVar {
    oid *varName;
    int varNameLen;
    u_char varType;
    int varLen;
    u_char *varVal;
    struct trapVar *next;
};

/* from snmp.c */
extern u_char sid[];		/* size SID_MAX_LEN */

#endif /* SQUID_SNMP_IMPL_H */
