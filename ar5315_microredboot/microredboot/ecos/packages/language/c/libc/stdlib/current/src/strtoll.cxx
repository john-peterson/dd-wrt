//===========================================================================
//
//      strtoll.cxx
//
//      String to long long int conversion function
//
//===========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//===========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    jlarmour
// Contributors: Fredrik Hederstierna, converted file from long to long long.
// Date:         2000-04-30, 2004-02-19
// Purpose:     
// Description: 
// Usage:       
//
//####DESCRIPTIONEND####
//
//===========================================================================
//
// This code is based on original code with the following copyright:
//
/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


// CONFIGURATION

#include <pkgconf/libc_stdlib.h>   // Configuration header

// INCLUDES

#include <cyg/infra/cyg_type.h>    // Common type definitions and support
#include <cyg/infra/cyg_trac.h>    // Tracing support
#include <cyg/infra/cyg_ass.h>     // Assertion support
#include <limits.h>                // Definition of LONG_LONG_MIN and LONG_LONG_MAX
#include <ctype.h>                 // Definition of many ctype functions
#include <errno.h>                 // Error code definitions
#include <stdlib.h>                // Header for all stdlib functions
                                   // (like this one)


// FUNCTIONS

//
// Convert a string to a long long integer.
//
// Ignores `locale' stuff.  Assumes that the upper and lower case
// alphabets and digits are each contiguous.
//

long long
strtoll( const char *nptr, char **endptr, int base )
{
    const char *s = nptr;
    unsigned long long acc;
    int c;
    unsigned long long cutoff;
    int neg = 0, any, cutlim;

    CYG_REPORT_FUNCNAMETYPE( "strtoll", "returning long long %lld" );
    CYG_REPORT_FUNCARG3( "nptr=%08x, endptr=%08x, base=%d",
                         nptr, endptr, base );
    CYG_CHECK_DATA_PTR( nptr, "nptr is not a valid pointer!" );

    if (endptr != NULL)
        CYG_CHECK_DATA_PTR( endptr, "endptr is not a valid pointer!" );
    
    //
    // Skip white space and pick up leading +/- sign if any.
    // If base is 0, allow 0x for hex and 0 for octal, else
    // assume decimal; if base is already 16, allow 0x.
    //
    
    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    
    //
    // Compute the cutoff value between legal numbers and illegal
    // numbers.  That is the largest legal value, divided by the
    // base.  An input number that is greater than this value, if
    // followed by a legal input character, is too big.  One that
    // is equal to this value may be valid or not; the limit
    // between valid and invalid numbers is then based on the last
    // digit.  For instance, if the range for long longs is
    // [-2147483648..2147483647] and the input base is 10,
    // cutoff will be set to 214748364 and cutlim to either
    // 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
    // a value > 214748364, or equal but the next digit is > 7 (or 8),
    // the number is too big, and we will return a range error.
    //
    // Set any if any `digits' consumed; make it negative to indicate
    // overflow.
    //
    
    cutoff = neg ? -(unsigned long long)LONG_LONG_MIN : LONG_LONG_MAX;
    cutlim = cutoff % (unsigned long long)base;
    cutoff /= (unsigned long long)base;
    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LONG_LONG_MIN : LONG_LONG_MAX;
        errno = ERANGE;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (char *) (any ? s - 1 : nptr);

    CYG_REPORT_RETVAL ( acc );

    return acc;
} // strtoll()

// EOF strtoll.cxx
