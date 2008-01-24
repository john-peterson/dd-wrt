/*
 *  An implementation of the ARCFOUR algorithm
 *
 *  Copyright (C) 2006-2007  Christophe Devine
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License, version 2.1 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */
/*
 *  The ARCFOUR algorithm was publicly disclosed on 94/09.
 *
 *  http://groups.google.com/group/sci.crypt/msg/10a300c9d21afca0
 */

#include "xyssl/config.h"

#if defined(XYSSL_ARC4_C)

#include "xyssl/arc4.h"

/*
 * ARC4 key schedule
 */
void arc4_setup( arc4_context *ctx, unsigned char *key, int keylen )
{
    int i, j, k, a;
    unsigned char *m;

    ctx->x = 0;
    ctx->y = 0;
    m = ctx->m;

    for( i = 0; i < 256; i++ )
        m[i] = (unsigned char) i;

    j = k = 0;

    for( i = 0; i < 256; i++, k++ )
    {
        if( k >= keylen ) k = 0;

        a = m[i];
        j = ( j + a + key[k] ) & 0xFF;
        m[i] = m[j];
        m[j] = (unsigned char) a;
    }
}

/*
 * ARC4 cipher function
 */
void arc4_crypt( arc4_context *ctx, unsigned char *buf, int buflen )
{
    int i, x, y, a, b;
    unsigned char *m;

    x = ctx->x;
    y = ctx->y;
    m = ctx->m;

    for( i = 0; i < buflen; i++ )
    {
        x = ( x + 1 ) & 0xFF; a = m[x];
        y = ( y + a ) & 0xFF; b = m[y];

        m[x] = (unsigned char) b;
        m[y] = (unsigned char) a;

        buf[i] = (unsigned char)
            ( buf[i] ^ m[(unsigned char)( a + b )] );
    }

    ctx->x = x;
    ctx->y = y;
}

#if defined(XYSSL_SELF_TEST)

#include <string.h>
#include <stdio.h>

/*
 * ARC4 tests vectors as posted by Eric Rescorla in sep. 1994:
 *
 * http://groups.google.com/group/comp.security.misc/msg/10a300c9d21afca0
 */
static const unsigned char arc4_test_key[3][8] =
{
    { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
    { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

static const unsigned char arc4_test_pt[3][8] =
{
    { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

static const unsigned char arc4_test_ct[3][8] =
{
    { 0x75, 0xB7, 0x87, 0x80, 0x99, 0xE0, 0xC5, 0x96 },
    { 0x74, 0x94, 0xC2, 0xE7, 0x10, 0x4B, 0x08, 0x79 },
    { 0xDE, 0x18, 0x89, 0x41, 0xA3, 0x37, 0x5D, 0x3A }
};

/*
 * Checkup routine
 */
int arc4_self_test( int verbose )
{
    int i;
    unsigned char buf[8];
    arc4_context ctx;

    for( i = 0; i < 3; i++ )
    {
        if( verbose != 0 )
            printf( "  ARC4 test #%d: ", i + 1 );

        memcpy( buf, arc4_test_pt[i], 8 );

        arc4_setup( &ctx, (unsigned char *) arc4_test_key[i], 8 );
        arc4_crypt( &ctx, buf, 8 );

        if( memcmp( buf, arc4_test_ct[i], 8 ) != 0 )
        {
            if( verbose != 0 )
                printf( "failed\n" );

            return( 1 );
        }

        if( verbose != 0 )
            printf( "passed\n" );
    }

    if( verbose != 0 )
        printf( "\n" );

    return( 0 );
}

#endif

#endif
