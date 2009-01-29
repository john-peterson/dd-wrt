

/*
 * $Id: MemBuf.h,v 1.9 2007/12/21 23:48:04 hno Exp $
 *
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#ifndef SQUID_MEMBUF_H
#define SQUID_MEMBUF_H

#include "cbdata.h"
#include "Packer.h"

/* auto-growing memory-resident buffer with printf interface */
/* note: when updating this struct, update MemBufNULL #define */

class MemBuf
{

public:
    _SQUID_INLINE_ MemBuf();
    _SQUID_INLINE_ ~MemBuf();

    /* use methods instead of deprecated buf and size members */

    char *content() { return buf; }             // start of the added data

    const char *content() const { return buf; } // start of the added data

    mb_size_t contentSize() const { return size; } // available data size

    bool hasContent() const { return size > 0; }

    // these space-related methods assume no growth and allow 0-termination
    char *space() { return buf + size; } // space to add data
    char *space(mb_size_t required) { if (size + required > capacity) grow(size + required); return buf + size; } // space to add data

    mb_size_t spaceSize() const;
    bool hasSpace() const { return size+1 < capacity; }

    mb_size_t potentialSpaceSize() const; // accounts for possible growth
    bool hasPotentialSpace() const { return potentialSpaceSize() > 0; }

    // there is currently no stretch() method to grow without appending

    void consume(mb_size_t sz);  // removes sz bytes, moving content left
    void append(const char *c, mb_size_t sz); // grows if needed and possible
    void appended(mb_size_t sz); // updates content size after external append

    // XXX: convert global memBuf*() functions into methods

    void terminate(); // zero-terminates the buffer w/o increasing contentSize

    bool wasStolen() const { return stolen; }

    /* init with specific sizes */
    void init(mb_size_t szInit, mb_size_t szMax);

    /* init with defaults */
    void init();

    /* cleans mb; last function to call if you do not give .buf away */
    void clean();

    /* resets mb preserving (or initializing if needed) memory buffer */
    void reset();

    /* unfirtunate hack to test if the buffer has been Init()ialized */
    int isNull();

    /* calls snprintf, extends buffer if needed */
    /* note we use Printf instead of printf so the compiler won't */
    /* think we're calling the libc printf() */
#if STDC_HEADERS

    void Printf(const char *fmt,...) PRINTF_FORMAT_ARG2;
#else

    void Printf();
#endif

    /* vPrintf for other printf()'s to use */
    void vPrintf(const char *fmt, va_list ap);

    /* returns free() function to be used, _freezes_ the object! */
    FREE *freeFunc();

private:
    /*
     * private copy constructor and assignment operator generates
     * compiler errors if someone tries to copy/assign a MemBuf
     */
    MemBuf(const MemBuf& m) {assert(false);};

    MemBuf& operator= (const MemBuf& m) {assert(false); return *this;};

    void grow(mb_size_t min_cap);

    CBDATA_CLASS2(MemBuf);

public:
    /* public, read-only, depricated in favor of space*() and content*() */
    // XXX: hide these members completely and remove 0-termination
    // so that consume() does not need to memmove all the time
    char *buf;          // available content
    mb_size_t size;     // used space, does not count 0-terminator

    /* private, stay away; use interface function instead */
    // XXX: make these private after converting memBuf*() functions to methods
    mb_size_t max_capacity;	/* when grows: assert(new_capacity <= max_capacity) */
    mb_size_t capacity;		/* allocated space */

unsigned stolen:
    1;		/* the buffer has been stolen for use by someone else */

#if 0

unsigned valid:
    1;		/* to be used for debugging only! */
#endif
};

#ifdef _USE_INLINE_
#include "MemBuf.cci"
#endif

/* returns free() function to be used, _freezes_ the object! */
SQUIDCEXTERN void memBufReport(MemBuf * mb);
/* pack content into a mem buf. */
SQUIDCEXTERN void packerToMemInit(Packer * p, MemBuf * mb);

#endif /* SQUID_MEM_H */
