//==========================================================================
//
//        font.h
//
//        LCD display font
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2003 Gary Thomas
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
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     gthomas
// Contributors:  gthomas
// Date:          2001-02-25
// Description:   Simple LCD font
//####DESCRIPTIONEND####

#ifndef _LCD_FONT_H_
#define _LCD_FONT_H_

// 8x8 Font - from Helios

#define FIRST_CHAR 0x20
#define LAST_CHAR  0x7F
#define FONT_HEIGHT 8
#define FONT_WIDTH  8
#define CURSOR_ON  0x7F
#define CURSOR_OFF 0x20
static char font_table[LAST_CHAR-FIRST_CHAR+1][8] =
{
        {        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /*   */
        {        0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00 }, /* ! */
        {        0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* " */
        {        0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00 }, /* # */
        {        0x30, 0xFC, 0x16, 0x7C, 0xD0, 0x7E, 0x18, 0x00 }, /* $ */
        {        0x06, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x60, 0x00 }, /* % */
        {        0x1C, 0x36, 0x36, 0x1C, 0xB6, 0x66, 0xDC, 0x00 }, /* & */
        {        0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ' */
        {        0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00 }, /* ( */
        {        0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00 }, /* ) */
        {        0x00, 0x18, 0x7E, 0x3C, 0x7E, 0x18, 0x00, 0x00 }, /* * */
        {        0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00 }, /* + */
        {        0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C }, /* , */
        {        0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00 }, /* - */
        {        0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 }, /* . */
        {        0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00 }, /* / */
        {        0x3C, 0x66, 0x76, 0x7E, 0x6E, 0x66, 0x3C, 0x00 }, /* 0 */
        {        0x18, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00 }, /* 1 */
        {        0x3C, 0x66, 0x60, 0x30, 0x18, 0x0C, 0x7E, 0x00 }, /* 2 */
        {        0x3C, 0x66, 0x60, 0x38, 0x60, 0x66, 0x3C, 0x00 }, /* 3 */
        {        0x30, 0x38, 0x3C, 0x36, 0x7E, 0x30, 0x30, 0x00 }, /* 4 */
        {        0x7E, 0x06, 0x3E, 0x60, 0x60, 0x66, 0x3C, 0x00 }, /* 5 */
        {        0x38, 0x0C, 0x06, 0x3E, 0x66, 0x66, 0x3C, 0x00 }, /* 6 */
        {        0x7E, 0x60, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00 }, /* 7 */
        {        0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00 }, /* 8 */
        {        0x3C, 0x66, 0x66, 0x7C, 0x60, 0x30, 0x1C, 0x00 }, /* 9 */
        {        0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00 }, /* : */
        {        0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x0C }, /* ; */
        {        0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00 }, /* < */
        {        0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00 }, /* = */
        {        0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00 }, /* > */
        {        0x3C, 0x66, 0x30, 0x18, 0x18, 0x00, 0x18, 0x00 }, /* ? */
        {        0x3C, 0x66, 0x76, 0x56, 0x76, 0x06, 0x3C, 0x00 }, /* @ */
        {        0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00 }, /* A */
        {        0x3E, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3E, 0x00 }, /* B */
        {        0x3C, 0x66, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00 }, /* C */
        {        0x1E, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1E, 0x00 }, /* D */
        {        0x7E, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x7E, 0x00 }, /* E */
        {        0x7E, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x00 }, /* F */
        {        0x3C, 0x66, 0x06, 0x76, 0x66, 0x66, 0x3C, 0x00 }, /* G */
        {        0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00 }, /* H */
        {        0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00 }, /* I */
        {        0x7C, 0x30, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00 }, /* J */
        {        0x66, 0x36, 0x1E, 0x0E, 0x1E, 0x36, 0x66, 0x00 }, /* K */
        {        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00 }, /* L */
        {        0xC6, 0xEE, 0xFE, 0xD6, 0xD6, 0xC6, 0xC6, 0x00 }, /* M */
        {        0x66, 0x66, 0x6E, 0x7E, 0x76, 0x66, 0x66, 0x00 }, /* N */
        {        0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00 }, /* O */
        {        0x3E, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x00 }, /* P */
        {        0x3C, 0x66, 0x66, 0x66, 0x56, 0x36, 0x6C, 0x00 }, /* Q */
        {        0x3E, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x66, 0x00 }, /* R */
        {        0x3C, 0x66, 0x06, 0x3C, 0x60, 0x66, 0x3C, 0x00 }, /* S */
        {        0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* T */
        {        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00 }, /* U */
        {        0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00 }, /* V */
        {        0xC6, 0xC6, 0xD6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00 }, /* W */
        {        0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00 }, /* X */
        {        0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00 }, /* Y */
        {        0x7E, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x7E, 0x00 }, /* Z */
        {        0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00 }, /* [ */
        {        0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00 }, /* \ */
        {        0x7C, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7C, 0x00 }, /* ] */ 
        {        0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ^ */
        {        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }, /* _ */
        {        0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ` */
        {        0x00, 0x00, 0x3C, 0x60, 0x7C, 0x66, 0x7C, 0x00 }, /* a */
        {        0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00 }, /* b */
        {        0x00, 0x00, 0x3C, 0x66, 0x06, 0x66, 0x3C, 0x00 }, /* c */
        {        0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x00 }, /* d */
        {        0x00, 0x00, 0x3C, 0x66, 0x7E, 0x06, 0x3C, 0x00 }, /* e */
        {        0x38, 0x0C, 0x0C, 0x3E, 0x0C, 0x0C, 0x0C, 0x00 }, /* f */
        {        0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x3C }, /* g */
        {        0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x00 }, /* h */
        {        0x18, 0x00, 0x1C, 0x18, 0x18, 0x18, 0x3C, 0x00 }, /* i */
        {        0x18, 0x00, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x0E }, /* j */
        {        0x06, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x00 }, /* k */
        {        0x1C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00 }, /* l */
        {        0x00, 0x00, 0x6C, 0xFE, 0xD6, 0xD6, 0xC6, 0x00 }, /* m */
        {        0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x00 }, /* n */
        {        0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00 }, /* o */
        {        0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x06 }, /* p */
        {        0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0xE0 }, /* q */
        {        0x00, 0x00, 0x36, 0x6E, 0x06, 0x06, 0x06, 0x00 }, /* r */
        {        0x00, 0x00, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x00 }, /* s */
        {        0x0C, 0x0C, 0x3E, 0x0C, 0x0C, 0x0C, 0x38, 0x00 }, /* t */
        {        0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00 }, /* u */
        {        0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00 }, /* v */
        {        0x00, 0x00, 0xC6, 0xD6, 0xD6, 0xFE, 0x6C, 0x00 }, /* w */
        {        0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00 }, /* x */
        {        0x00, 0x00, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x3C }, /* y */
        {        0x00, 0x00, 0x7E, 0x30, 0x18, 0x0C, 0x7E, 0x00 }, /* z */
        {        0x30, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x30, 0x00 }, /* { */
        {        0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 }, /* | */
        {        0x0C, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0C, 0x00 }, /* } */
        {        0x8C, 0xD6, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* ~ */
        {        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }  /* Block cursor */
};

#endif // _LCD_FONT_H_
