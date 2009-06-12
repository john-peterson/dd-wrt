/*
 * Copyright (c) 1999, 2000 Greg Haerr <greg@censoft.com>
 *
 * PC Bios ROM font extractor
 * Note: ascent field of produced C file must be hand-editted
 */
#include "../../device.h"
#include "../vga.h"

/* rom font constants*/
int	ROM_CHAR_HEIGHT = 16;	/* number of scan lines in fonts in ROM */
#define	ROM_CHAR_WIDTH	8	/* number of pixels for character width */
#define MAX_ROM_HEIGHT	16	/* max rom character height*/
#define	FONT_CHARS	256	/* number of characters in font tables */

/* int10 functions*/
#define FNGR640x480	0x0012	/* function for graphics mode 640x480x16*/
#define FNGR640x350	0x0010	/* function for graphics mode 640x350x16*/
#define FNTEXT		0x0003	/* function for 80x25 text mode*/
#define FNGETROMADDR	0x1130	/* function for address of rom character table*/
#define GETROM8x14	0x0200	/* want address of ROM 8x14 char table*/
#define GETROM8x8	0x0300	/* want address of ROM 8x8 char table*/
#define GETROM8x16	0x0600	/* want address of ROM 8x16 char table*/


#define GRMODE		FNGR640x350

/* local data*/
FARADDR 		rom_char_addr;

void print_rom_table(void);
void print_char(int ch,MWIMAGEBITS *b, int w, int h);
void print_bits(MWIMAGEBITS *bits, int width, int height);

main()
{
	/* init bios graphics mode*/
	int10(GRMODE, 0);

	/* get address of rom character table*/
	rom_char_addr = int10(FNGETROMADDR, GETROM8x14);

	/* check bios data area for actual character height,
	 * as the returned font isn't always 14 high
	 */
#if 0
	ROM_CHAR_HEIGHT = GETBYTE_FP(MK_FP(0x0040, 0x0085));
#else
	ROM_CHAR_HEIGHT = 16;
#endif

	printf("/* Generated by convrom.exe*/\n");
	printf("#include \"device.h\"\n\n");
	printf("/* ROM %dx%d Font bios mode %x */\n\n",
		ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT, GRMODE);
	printf("static MWIMAGEBITS rom%dx%d_bits[] = {\n\n",
		ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT);

	print_rom_table();

	printf("};\n\n");
	printf("/* Exported structure definition. */\n"
		"MWCFONT font_rom%dx%d = {\n",
		ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT);
	printf("\t\"rom%dx%d\",\n", ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT);
	printf("\t%d,\n", ROM_CHAR_WIDTH);
	printf("\t%d,\n", ROM_CHAR_HEIGHT);
	printf("\t%d,\n", ROM_CHAR_HEIGHT);	/* ascent*/
	printf("\t0,\n\t256,\n");
	printf("\trom%dx%d_bits,\n", ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT);
	printf("\t0,\n\t0\n");
	printf("};\n");

	/* init bios 80x25 text mode*/
	int10(FNTEXT, 0);
}

void
print_rom_table(void)
{
	FARADDR		bits;
	int		n;
	int		ch;
	MWIMAGEBITS *	p;
	MWIMAGEBITS	image[MAX_ROM_HEIGHT];

	for(ch=0; ch < 256; ++ch) {
		bits = rom_char_addr + ch * ROM_CHAR_HEIGHT;
		p = image;
		for(n=0; n<ROM_CHAR_HEIGHT; ++n)
		    *p++ = GETBYTE_FP(bits++) << 8;
		print_char(ch, image, ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT);
		print_bits(image, ROM_CHAR_WIDTH, ROM_CHAR_HEIGHT);
		printf("\n");
	}
}

/* Character ! (0x21):
   ht=16, width=8
   +----------------+
   |                |
   |                |
   | *              |
   | *              |
   | *              |
   | *              |
   | *              |
   | *              |
   |                |
   | *              |
   |                |
   |                |
   +----------------+ */

void
print_char(int ch,MWIMAGEBITS *bits, int width, int height)
{
	COORD 		x;
	int 		bitcount;	/* number of bits left in bitmap word */
	MWIMAGEBITS	bitvalue;	/* bitmap word value */

	printf("/* Character %c (0x%02x):\n", ch? ch: ' ', ch);
	printf("   ht=%d, width=%d\n", height, width);
	printf("   +");
	for(x=0; x<width; ++x)
		printf("-");
	printf("+\n");
	x = 0;
	bitcount = 0;
	while (height > 0) {
	    if (bitcount <= 0) {
		    printf("   |");
		    bitcount = MWIMAGE_BITSPERIMAGE;
		    bitvalue = *bits++;
	    }
		if (MWIMAGE_TESTBIT(bitvalue))
			    printf("*");
		else printf(" ");
	    bitvalue = MWIMAGE_SHIFTBIT(bitvalue);
	    --bitcount;
	    if (x++ == width-1) {
		    x = 0;
		    --height;
		    bitcount = 0;
		    printf("|\n");
	    }
	}
	printf("   +");
	for(x=0; x<width; ++x)
		printf("-");
	printf("+ */\n");
}

#define	MWIMAGE_GETBIT4(m)	(((m) & 0xf000) >> 12)
#define	MWIMAGE_SHIFTBIT4(m)	((IMAGEBITS) ((m) << 4))

void
print_bits(MWIMAGEBITS *bits, int width, int height)
{
	COORD 		x;
	int 		bitcount;	/* number of bits left in bitmap word */
	MWIMAGEBITS	bitvalue;	/* bitmap word value */

	x = 0;
	bitcount = 0;
	while (height > 0) {
	    if (bitcount <= 0) {
		    printf("0x");
		    bitcount = MWIMAGE_BITSPERIMAGE;
		    bitvalue = *bits++;
	    }
		printf("%x", MWIMAGE_GETBIT4(bitvalue));
	    bitvalue = MWIMAGE_SHIFTBIT4(bitvalue);
	    bitcount -= 4;
		x += 4;
	    if (x >= width-1) {
			if(MWIMAGE_BITSPERIMAGE > width)
				for(x=MWIMAGE_BITSPERIMAGE-width; x>0; ) {
					printf("0");
					x -= 4;
				}
		    x = 0;
		    --height;
		    bitcount = 0;
		    printf(",\n");
	    }
	}
}
