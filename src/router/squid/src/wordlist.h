
/*
 * $Id: wordlist.h,v 1.1 2006/04/23 11:10:32 robertc Exp $
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

#ifndef SQUID_WORDLIST_H
#define SQUID_WORDLIST_H

#include "squid.h"
#include "MemPool.h"

class wordlist
{

public:
    MEMPROXY_CLASS(wordlist);
    char *key;
    wordlist *next;
};

MEMPROXY_CLASS_INLINE(wordlist);

SQUIDCEXTERN const char *wordlistAdd(wordlist **, const char *);
SQUIDCEXTERN void wordlistAddWl(wordlist **, wordlist *);
SQUIDCEXTERN void wordlistJoin(wordlist **, wordlist **);
SQUIDCEXTERN wordlist *wordlistDup(const wordlist *);
SQUIDCEXTERN void wordlistDestroy(wordlist **);

#endif /* SQUID_WORDLIST_H */
