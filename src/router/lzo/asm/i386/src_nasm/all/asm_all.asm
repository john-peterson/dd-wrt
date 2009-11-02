;  asm_all.asm --
;
;  This file is part of the LZO real-time data compression library.
;
;  Copyright (C) 2008 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2007 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2006 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2005 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2004 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2003 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2002 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2001 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 2000 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 1999 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
;  Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
;  All Rights Reserved.
;
;  The LZO library is free software; you can redistribute it and/or
;  modify it under the terms of the GNU General Public License as
;  published by the Free Software Foundation; either version 2 of
;  the License, or (at your option) any later version.
;
;  The LZO library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with the LZO library; see the file COPYING.
;  If not, write to the Free Software Foundation, Inc.,
;  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
;
;  Markus F.X.J. Oberhumer
;  <markus@oberhumer.com>
;  http://www.oberhumer.com/opensource/lzo/
;

; /***** DO NOT EDIT - GENERATED AUTOMATICALLY *****/

%define F(name) name
%ifidni __OUTPUT_FORMAT__,elf
%define globalf(x) global x:function
%else
%define globalf(x) global x
%endif
bits 32
%ifidni __OUTPUT_FORMAT__,elf
section .note.GNU-stack noalloc noexec nowrite progbits
%endif
%ifidni __OUTPUT_FORMAT__,obj
segment _TEXT class=CODE public use32 flat align=16
%else
section .text
%endif

globalf(_lzo1c_decompress_asm)
globalf(F(lzo1c_decompress_asm))
_lzo1c_decompress_asm:
F(lzo1c_decompress_asm):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,144,49,192,138,6,70,60,32,115
db 15,8,192,116,51,137,193,243,164,138,6,70,60,32,114,72
db 60,64,114,93,137,193,36,31,141,87,255,193,233,5,41,194
db 138,6,70,193,224,5,41,194,65,135,242,243,164,137,214,235
db 199,141,180,38,0,0,0,0,138,6,70,141,72,32,60,248
db 114,197,185,24,1,0,0,44,248,116,6,145,48,192,211,224
db 145,243,164,235,163,141,118,0,141,87,255,41,194,138,6,70
db 193,224,5,41,194,135,242,164,164,164,137,214,164,49,192,235
db 152,36,31,137,193,117,19,177,31,138,6,70,8,192,117,8
db 129,193,255,0,0,0,235,241,1,193,138,6,70,137,195,36
db 63,137,250,41,194,138,6,70,193,224,6,41,194,57,250,116
db 27,135,214,141,73,3,243,164,137,214,49,192,193,235,6,137
db 217,15,133,80,255,255,255,233,60,255,255,255,131,249,1,15
db 149,192,139,84,36,40,3,84,36,44,57,214,119,38,114,29
db 43,124,36,48,139,84,36,52,137,58,247,216,131,196,12,90
db 89,91,94,95,93,195,184,1,0,0,0,235,227,184,8,0
db 0,0,235,220,184,4,0,0,0,235,213,144,141,116,38,0

globalf(_lzo1c_decompress_asm_safe)
globalf(F(lzo1c_decompress_asm_safe))
_lzo1c_decompress_asm_safe:
F(lzo1c_decompress_asm_safe):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,141,70,253,3,68,36,44,137,68
db 36,4,137,248,139,84,36,52,3,2,137,4,36,141,118,0
db 49,192,138,6,70,60,32,115,40,8,192,116,99,137,193,141
db 28,15,57,28,36,15,130,107,1,0,0,141,28,14,57,92
db 36,4,15,130,87,1,0,0,243,164,138,6,70,60,32,114
db 127,60,64,15,130,169,0,0,0,137,193,36,31,141,87,255
db 193,233,5,41,194,138,6,70,193,224,5,41,194,65,135,242
db 59,116,36,48,15,130,51,1,0,0,141,28,15,57,28,36
db 15,130,32,1,0,0,243,164,137,214,235,148,141,116,38,0
db 138,6,70,141,72,32,60,248,114,149,185,24,1,0,0,44
db 248,116,6,145,48,192,211,224,145,141,28,15,57,28,36,15
db 130,241,0,0,0,141,28,14,57,92,36,4,15,130,221,0
db 0,0,243,164,233,87,255,255,255,141,180,38,0,0,0,0
db 141,87,255,41,194,138,6,70,193,224,5,41,194,135,242,59
db 116,36,48,15,130,196,0,0,0,141,95,4,57,28,36,15
db 130,177,0,0,0,164,164,164,137,214,164,49,192,233,72,255
db 255,255,36,31,137,193,117,26,177,31,138,6,70,8,192,117
db 15,129,193,255,0,0,0,235,241,141,180,38,0,0,0,0
db 1,193,138,6,70,137,195,36,63,137,250,41,194,138,6,70
db 193,224,6,41,194,57,250,116,41,135,214,141,73,3,59,116
db 36,48,114,105,141,4,15,57,4,36,114,90,243,164,137,214
db 49,192,193,235,6,137,217,15,133,210,254,255,255,233,190,254
db 255,255,131,249,1,15,149,192,59,60,36,119,57,139,84,36
db 40,3,84,36,44,57,214,119,38,114,29,43,124,36,48,139
db 84,36,52,137,58,247,216,131,196,12,90,89,91,94,95,93
db 195,184,1,0,0,0,235,227,184,8,0,0,0,235,220,184
db 4,0,0,0,235,213,184,5,0,0,0,235,206,184,6,0
db 0,0,235,199,141,182,0,0,0,0,141,191,0,0,0,0

globalf(_lzo1f_decompress_asm_fast)
globalf(F(lzo1f_decompress_asm_fast))
_lzo1f_decompress_asm_fast:
F(lzo1f_decompress_asm_fast):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,144,49,192,138,6,70,60,31,119
db 51,8,192,137,193,117,19,138,6,70,8,192,117,8,129,193
db 255,0,0,0,235,241,141,76,8,31,136,200,193,233,2,243
db 165,36,3,116,8,139,30,1,198,137,31,1,199,138,6,70
db 60,31,118,88,60,223,15,135,132,0,0,0,137,193,193,232
db 2,141,87,255,36,7,193,233,5,137,195,138,6,141,4,195
db 70,41,194,131,193,2,135,214,131,249,6,114,16,131,248,4
db 114,11,136,200,193,233,2,243,165,36,3,136,193,243,164,137
db 214,138,78,254,131,225,3,15,132,123,255,255,255,139,6,1
db 206,137,7,1,207,49,192,138,6,70,235,164,193,232,2,141
db 151,255,247,255,255,137,193,138,6,70,141,4,193,41,194,139
db 2,137,7,131,199,3,235,201,138,6,70,8,192,117,8,129
db 193,255,0,0,0,235,241,141,76,8,31,235,9,141,118,0
db 36,31,137,193,116,226,137,250,102,139,6,131,198,2,193,232
db 2,15,133,122,255,255,255,131,249,1,15,149,192,139,84,36
db 40,3,84,36,44,57,214,119,38,114,29,43,124,36,48,139
db 84,36,52,137,58,247,216,131,196,12,90,89,91,94,95,93
db 195,184,1,0,0,0,235,227,184,8,0,0,0,235,220,184
db 4,0,0,0,235,213,141,118,0,141,188,39,0,0,0,0

globalf(_lzo1f_decompress_asm_fast_safe)
globalf(F(lzo1f_decompress_asm_fast_safe))
_lzo1f_decompress_asm_fast_safe:
F(lzo1f_decompress_asm_fast_safe):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,141,70,253,3,68,36,44,137,68
db 36,4,137,248,139,84,36,52,3,2,137,4,36,141,118,0
db 49,192,138,6,70,60,31,119,76,8,192,137,193,117,19,138
db 6,70,8,192,117,8,129,193,255,0,0,0,235,241,141,76
db 8,31,141,28,15,57,28,36,15,130,61,1,0,0,141,28
db 14,57,92,36,4,15,130,41,1,0,0,136,200,193,233,2
db 243,165,36,3,116,8,139,30,1,198,137,31,1,199,138,6
db 70,60,31,118,110,60,223,15,135,179,0,0,0,137,193,193
db 232,2,141,87,255,36,7,193,233,5,137,195,138,6,141,4
db 195,70,41,194,131,193,2,135,214,59,116,36,48,15,130,239
db 0,0,0,141,28,15,57,28,36,15,130,220,0,0,0,131
db 249,6,114,16,131,248,4,114,11,136,200,193,233,2,243,165
db 36,3,136,193,243,164,137,214,138,78,254,131,225,3,15,132
db 76,255,255,255,139,6,1,206,137,7,1,207,49,192,138,6
db 70,235,142,141,87,3,57,20,36,15,130,156,0,0,0,193
db 232,2,141,151,255,247,255,255,137,193,138,6,70,141,4,193
db 41,194,59,84,36,48,15,130,134,0,0,0,139,2,137,7
db 131,199,3,235,179,138,6,70,8,192,117,8,129,193,255,0
db 0,0,235,241,141,76,8,31,235,12,141,182,0,0,0,0
db 36,31,137,193,116,223,137,250,102,139,6,131,198,2,193,232
db 2,15,133,75,255,255,255,131,249,1,15,149,192,59,60,36
db 119,57,139,84,36,40,3,84,36,44,57,214,119,38,114,29
db 43,124,36,48,139,84,36,52,137,58,247,216,131,196,12,90
db 89,91,94,95,93,195,184,1,0,0,0,235,227,184,8,0
db 0,0,235,220,184,4,0,0,0,235,213,184,5,0,0,0
db 235,206,184,6,0,0,0,235,199,141,180,38,0,0,0,0

globalf(_lzo1x_decompress_asm_fast)
globalf(F(lzo1x_decompress_asm_fast))
_lzo1x_decompress_asm_fast:
F(lzo1x_decompress_asm_fast):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,49,192,49,219,172,60,17,118,27
db 44,14,235,34,5,255,0,0,0,138,30,70,8,219,116,244
db 141,68,24,21,235,16,137,246,138,6,70,60,16,115,65,8
db 192,116,230,131,192,6,137,193,49,232,193,233,2,33,232,139
db 22,131,198,4,137,23,131,199,4,73,117,243,41,198,41,199
db 138,6,70,60,16,115,25,193,232,2,138,30,141,151,255,247
db 255,255,141,4,152,70,41,194,139,10,137,15,1,239,235,110
db 60,64,114,52,137,193,193,232,2,141,87,255,131,224,7,138
db 30,193,233,5,141,4,216,70,41,194,131,193,4,57,232,115
db 53,235,109,5,255,0,0,0,138,30,70,8,219,116,244,141
db 76,24,36,49,192,235,13,144,60,32,114,116,131,224,31,116
db 231,141,72,5,102,139,6,141,87,255,193,232,2,131,198,2
db 41,194,57,232,114,58,141,68,15,253,193,233,2,139,26,131
db 194,4,137,31,131,199,4,73,117,243,137,199,49,219,138,70
db 254,33,232,15,132,63,255,255,255,139,22,1,198,137,23,1
db 199,138,6,70,233,119,255,255,255,141,180,38,0,0,0,0
db 135,214,41,233,243,164,137,214,235,212,129,193,255,0,0,0
db 138,30,70,8,219,116,243,141,76,11,12,235,23,141,118,0
db 60,16,114,44,137,193,131,224,8,193,224,13,131,225,7,116
db 223,131,193,5,102,139,6,131,198,2,141,151,0,192,255,255
db 193,232,2,116,43,41,194,233,122,255,255,255,141,116,38,0
db 193,232,2,138,30,141,87,255,141,4,152,70,41,194,138,2
db 136,7,138,90,1,136,95,1,131,199,2,233,110,255,255,255
db 131,249,6,15,149,192,139,84,36,40,3,84,36,44,57,214
db 119,38,114,29,43,124,36,48,139,84,36,52,137,58,247,216
db 131,196,12,90,89,91,94,95,93,195,184,1,0,0,0,235
db 227,184,8,0,0,0,235,220,184,4,0,0,0,235,213,144

globalf(_lzo1x_decompress_asm_fast_safe)
globalf(F(lzo1x_decompress_asm_fast_safe))
_lzo1x_decompress_asm_fast_safe:
F(lzo1x_decompress_asm_fast_safe):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,141,70,253,3,68,36,44,137,68
db 36,4,137,248,139,84,36,52,3,2,137,4,36,49,192,49
db 219,172,60,17,118,55,44,14,235,62,5,255,0,0,0,141
db 84,6,18,57,84,36,4,15,130,78,2,0,0,138,30,70
db 8,219,116,230,141,68,24,21,235,30,141,182,0,0,0,0
db 57,116,36,4,15,130,49,2,0,0,138,6,70,60,16,115
db 119,8,192,116,216,131,192,6,141,84,7,253,57,20,36,15
db 130,29,2,0,0,141,84,6,253,57,84,36,4,15,130,8
db 2,0,0,137,193,49,232,193,233,2,33,232,139,22,131,198
db 4,137,23,131,199,4,73,117,243,41,198,41,199,138,6,70
db 60,16,115,52,141,87,3,57,20,36,15,130,226,1,0,0
db 193,232,2,138,30,141,151,255,247,255,255,141,4,152,70,41
db 194,59,84,36,48,15,130,206,1,0,0,139,10,137,15,1
db 239,233,151,0,0,0,137,246,60,64,114,68,137,193,193,232
db 2,141,87,255,131,224,7,138,30,193,233,5,141,4,216,70
db 41,194,131,193,4,57,232,115,73,233,170,0,0,0,5,255
db 0,0,0,141,86,3,57,84,36,4,15,130,123,1,0,0
db 138,30,70,8,219,116,231,141,76,24,36,49,192,235,17,144
db 60,32,15,130,200,0,0,0,131,224,31,116,227,141,72,5
db 102,139,6,141,87,255,193,232,2,131,198,2,41,194,57,232
db 114,102,59,84,36,48,15,130,77,1,0,0,141,68,15,253
db 193,233,2,57,4,36,15,130,54,1,0,0,139,26,131,194
db 4,137,31,131,199,4,73,117,243,137,199,49,219,138,70,254
db 33,232,15,132,216,254,255,255,141,20,7,57,20,36,15,130
db 14,1,0,0,141,20,6,57,84,36,4,15,130,250,0,0
db 0,139,22,1,198,137,23,1,199,138,6,70,233,55,255,255
db 255,141,180,38,0,0,0,0,59,84,36,48,15,130,231,0
db 0,0,141,68,15,253,57,4,36,15,130,211,0,0,0,135
db 214,41,233,243,164,137,214,235,164,129,193,255,0,0,0,141
db 86,3,57,84,36,4,15,130,175,0,0,0,138,30,70,8
db 219,116,230,141,76,11,12,235,27,141,180,38,0,0,0,0
db 60,16,114,44,137,193,131,224,8,193,224,13,131,225,7,116
db 219,131,193,5,102,139,6,131,198,2,141,151,0,192,255,255
db 193,232,2,116,57,41,194,233,38,255,255,255,141,116,38,0
db 141,87,2,57,20,36,114,106,193,232,2,138,30,141,87,255
db 141,4,152,70,41,194,59,84,36,48,114,93,138,2,136,7
db 138,90,1,136,95,1,131,199,2,233,31,255,255,255,131,249
db 6,15,149,192,59,60,36,119,57,139,84,36,40,3,84,36
db 44,57,214,119,38,114,29,43,124,36,48,139,84,36,52,137
db 58,247,216,131,196,12,90,89,91,94,95,93,195,184,1,0
db 0,0,235,227,184,8,0,0,0,235,220,184,4,0,0,0
db 235,213,184,5,0,0,0,235,206,184,6,0,0,0,235,199

globalf(_lzo1x_decompress_asm)
globalf(F(lzo1x_decompress_asm))
_lzo1x_decompress_asm:
F(lzo1x_decompress_asm):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,49,192,49,219,172,60,17,118,35
db 44,17,60,4,115,40,137,193,235,56,5,255,0,0,0,138
db 30,70,8,219,116,244,141,68,24,18,235,18,141,116,38,0
db 138,6,70,60,16,115,73,8,192,116,228,131,192,3,137,193
db 193,232,2,33,233,139,22,131,198,4,137,23,131,199,4,72
db 117,243,243,164,138,6,70,60,16,115,37,193,232,2,138,30
db 141,151,255,247,255,255,141,4,152,70,41,194,138,2,136,7
db 138,66,1,136,71,1,138,66,2,136,71,2,1,239,235,119
db 60,64,114,52,137,193,193,232,2,141,87,255,131,224,7,138
db 30,193,233,5,141,4,216,70,41,194,65,57,232,115,55,235
db 119,5,255,0,0,0,138,30,70,8,219,116,244,141,76,24
db 33,49,192,235,15,141,118,0,60,32,114,124,131,224,31,116
db 229,141,72,2,102,139,6,141,87,255,193,232,2,131,198,2
db 41,194,57,232,114,66,137,203,193,235,2,116,17,139,2,131
db 194,4,137,7,131,199,4,75,117,243,33,233,116,9,138,2
db 66,136,7,71,73,117,247,138,70,254,33,232,15,132,46,255
db 255,255,138,14,70,136,15,71,72,117,247,138,6,70,233,109
db 255,255,255,144,141,116,38,0,135,214,243,164,137,214,235,215
db 129,193,255,0,0,0,138,30,70,8,219,116,243,141,76,11
db 9,235,25,144,141,116,38,0,60,16,114,44,137,193,131,224
db 8,193,224,13,131,225,7,116,221,131,193,2,102,139,6,131
db 198,2,141,151,0,192,255,255,193,232,2,116,43,41,194,233
db 114,255,255,255,141,116,38,0,193,232,2,138,30,141,87,255
db 141,4,152,70,41,194,138,2,136,7,138,90,1,136,95,1
db 131,199,2,233,111,255,255,255,131,249,3,15,149,192,139,84
db 36,40,3,84,36,44,57,214,119,38,114,29,43,124,36,48
db 139,84,36,52,137,58,247,216,131,196,12,90,89,91,94,95
db 93,195,184,1,0,0,0,235,227,184,8,0,0,0,235,220
db 184,4,0,0,0,235,213,137,246,141,188,39,0,0,0,0

globalf(_lzo1x_decompress_asm_safe)
globalf(F(lzo1x_decompress_asm_safe))
_lzo1x_decompress_asm_safe:
F(lzo1x_decompress_asm_safe):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,141,70,253,3,68,36,44,137,68
db 36,4,137,248,139,84,36,52,3,2,137,4,36,49,192,49
db 219,172,60,17,118,87,44,17,60,4,115,92,141,20,7,57
db 20,36,15,130,130,2,0,0,141,20,6,57,84,36,4,15
db 130,110,2,0,0,137,193,235,110,5,255,0,0,0,141,84
db 6,18,57,84,36,4,15,130,87,2,0,0,138,30,70,8
db 219,116,230,141,68,24,18,235,31,141,180,38,0,0,0,0
db 57,116,36,4,15,130,57,2,0,0,138,6,70,60,16,115
db 127,8,192,116,215,131,192,3,141,84,7,0,57,20,36,15
db 130,37,2,0,0,141,84,6,0,57,84,36,4,15,130,16
db 2,0,0,137,193,193,232,2,33,233,139,22,131,198,4,137
db 23,131,199,4,72,117,243,243,164,138,6,70,60,16,115,64
db 141,87,3,57,20,36,15,130,238,1,0,0,193,232,2,138
db 30,141,151,255,247,255,255,141,4,152,70,41,194,59,84,36
db 48,15,130,218,1,0,0,138,2,136,7,138,66,1,136,71
db 1,138,66,2,136,71,2,1,239,233,163,0,0,0,137,246
db 60,64,114,68,137,193,193,232,2,141,87,255,131,224,7,138
db 30,193,233,5,141,4,216,70,41,194,65,57,232,115,75,233
db 180,0,0,0,5,255,0,0,0,141,86,3,57,84,36,4
db 15,130,125,1,0,0,138,30,70,8,219,116,231,141,76,24
db 33,49,192,235,19,141,118,0,60,32,15,130,200,0,0,0
db 131,224,31,116,225,141,72,2,102,139,6,141,87,255,193,232
db 2,131,198,2,41,194,57,232,114,110,59,84,36,48,15,130
db 77,1,0,0,141,4,15,57,4,36,15,130,58,1,0,0
db 137,203,193,235,2,116,17,139,2,131,194,4,137,7,131,199
db 4,75,117,243,33,233,116,9,138,2,66,136,7,71,73,117
db 247,138,70,254,33,232,15,132,196,254,255,255,141,20,7,57
db 20,36,15,130,2,1,0,0,141,20,6,57,84,36,4,15
db 130,238,0,0,0,138,14,70,136,15,71,72,117,247,138,6
db 70,233,42,255,255,255,137,246,59,84,36,48,15,130,223,0
db 0,0,141,68,15,0,57,4,36,15,130,203,0,0,0,135
db 214,243,164,137,214,235,170,129,193,255,0,0,0,141,86,3
db 57,84,36,4,15,130,169,0,0,0,138,30,70,8,219,116
db 230,141,76,11,9,235,21,144,60,16,114,44,137,193,131,224
db 8,193,224,13,131,225,7,116,225,131,193,2,102,139,6,131
db 198,2,141,151,0,192,255,255,193,232,2,116,57,41,194,233
db 38,255,255,255,141,116,38,0,141,87,2,57,20,36,114,106
db 193,232,2,138,30,141,87,255,141,4,152,70,41,194,59,84
db 36,48,114,93,138,2,136,7,138,90,1,136,95,1,131,199
db 2,233,43,255,255,255,131,249,3,15,149,192,59,60,36,119
db 57,139,84,36,40,3,84,36,44,57,214,119,38,114,29,43
db 124,36,48,139,84,36,52,137,58,247,216,131,196,12,90,89
db 91,94,95,93,195,184,1,0,0,0,235,227,184,8,0,0
db 0,235,220,184,4,0,0,0,235,213,184,5,0,0,0,235
db 206,184,6,0,0,0,235,199,144,141,180,38,0,0,0,0

globalf(_lzo1y_decompress_asm_fast)
globalf(F(lzo1y_decompress_asm_fast))
_lzo1y_decompress_asm_fast:
F(lzo1y_decompress_asm_fast):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,49,192,49,219,172,60,17,118,27
db 44,14,235,34,5,255,0,0,0,138,30,70,8,219,116,244
db 141,68,24,21,235,16,137,246,138,6,70,60,16,115,65,8
db 192,116,230,131,192,6,137,193,49,232,193,233,2,33,232,139
db 22,131,198,4,137,23,131,199,4,73,117,243,41,198,41,199
db 138,6,70,60,16,115,25,193,232,2,138,30,141,151,255,251
db 255,255,141,4,152,70,41,194,139,10,137,15,1,239,235,110
db 60,64,114,52,137,193,193,232,2,141,87,255,33,232,138,30
db 193,233,4,141,4,152,70,41,194,131,193,2,57,232,115,54
db 235,110,5,255,0,0,0,138,30,70,8,219,116,244,141,76
db 24,36,49,192,235,14,137,246,60,32,114,116,131,224,31,116
db 230,141,72,5,102,139,6,141,87,255,193,232,2,131,198,2
db 41,194,57,232,114,58,141,68,15,253,193,233,2,139,26,131
db 194,4,137,31,131,199,4,73,117,243,137,199,49,219,138,70
db 254,33,232,15,132,63,255,255,255,139,22,1,198,137,23,1
db 199,138,6,70,233,119,255,255,255,141,180,38,0,0,0,0
db 135,214,41,233,243,164,137,214,235,212,129,193,255,0,0,0
db 138,30,70,8,219,116,243,141,76,11,12,235,23,141,118,0
db 60,16,114,44,137,193,131,224,8,193,224,13,131,225,7,116
db 223,131,193,5,102,139,6,131,198,2,141,151,0,192,255,255
db 193,232,2,116,43,41,194,233,122,255,255,255,141,116,38,0
db 193,232,2,138,30,141,87,255,141,4,152,70,41,194,138,2
db 136,7,138,90,1,136,95,1,131,199,2,233,110,255,255,255
db 131,249,6,15,149,192,139,84,36,40,3,84,36,44,57,214
db 119,38,114,29,43,124,36,48,139,84,36,52,137,58,247,216
db 131,196,12,90,89,91,94,95,93,195,184,1,0,0,0,235
db 227,184,8,0,0,0,235,220,184,4,0,0,0,235,213,144

globalf(_lzo1y_decompress_asm_fast_safe)
globalf(F(lzo1y_decompress_asm_fast_safe))
_lzo1y_decompress_asm_fast_safe:
F(lzo1y_decompress_asm_fast_safe):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,141,70,253,3,68,36,44,137,68
db 36,4,137,248,139,84,36,52,3,2,137,4,36,49,192,49
db 219,172,60,17,118,55,44,14,235,62,5,255,0,0,0,141
db 84,6,18,57,84,36,4,15,130,78,2,0,0,138,30,70
db 8,219,116,230,141,68,24,21,235,30,141,182,0,0,0,0
db 57,116,36,4,15,130,49,2,0,0,138,6,70,60,16,115
db 119,8,192,116,216,131,192,6,141,84,7,253,57,20,36,15
db 130,29,2,0,0,141,84,6,253,57,84,36,4,15,130,8
db 2,0,0,137,193,49,232,193,233,2,33,232,139,22,131,198
db 4,137,23,131,199,4,73,117,243,41,198,41,199,138,6,70
db 60,16,115,52,141,87,3,57,20,36,15,130,226,1,0,0
db 193,232,2,138,30,141,151,255,251,255,255,141,4,152,70,41
db 194,59,84,36,48,15,130,206,1,0,0,139,10,137,15,1
db 239,233,151,0,0,0,137,246,60,64,114,68,137,193,193,232
db 2,141,87,255,33,232,138,30,193,233,4,141,4,152,70,41
db 194,131,193,2,57,232,115,74,233,171,0,0,0,5,255,0
db 0,0,141,86,3,57,84,36,4,15,130,124,1,0,0,138
db 30,70,8,219,116,231,141,76,24,36,49,192,235,18,137,246
db 60,32,15,130,200,0,0,0,131,224,31,116,226,141,72,5
db 102,139,6,141,87,255,193,232,2,131,198,2,41,194,57,232
db 114,102,59,84,36,48,15,130,77,1,0,0,141,68,15,253
db 193,233,2,57,4,36,15,130,54,1,0,0,139,26,131,194
db 4,137,31,131,199,4,73,117,243,137,199,49,219,138,70,254
db 33,232,15,132,216,254,255,255,141,20,7,57,20,36,15,130
db 14,1,0,0,141,20,6,57,84,36,4,15,130,250,0,0
db 0,139,22,1,198,137,23,1,199,138,6,70,233,55,255,255
db 255,141,180,38,0,0,0,0,59,84,36,48,15,130,231,0
db 0,0,141,68,15,253,57,4,36,15,130,211,0,0,0,135
db 214,41,233,243,164,137,214,235,164,129,193,255,0,0,0,141
db 86,3,57,84,36,4,15,130,175,0,0,0,138,30,70,8
db 219,116,230,141,76,11,12,235,27,141,180,38,0,0,0,0
db 60,16,114,44,137,193,131,224,8,193,224,13,131,225,7,116
db 219,131,193,5,102,139,6,131,198,2,141,151,0,192,255,255
db 193,232,2,116,57,41,194,233,38,255,255,255,141,116,38,0
db 141,87,2,57,20,36,114,106,193,232,2,138,30,141,87,255
db 141,4,152,70,41,194,59,84,36,48,114,93,138,2,136,7
db 138,90,1,136,95,1,131,199,2,233,31,255,255,255,131,249
db 6,15,149,192,59,60,36,119,57,139,84,36,40,3,84,36
db 44,57,214,119,38,114,29,43,124,36,48,139,84,36,52,137
db 58,247,216,131,196,12,90,89,91,94,95,93,195,184,1,0
db 0,0,235,227,184,8,0,0,0,235,220,184,4,0,0,0
db 235,213,184,5,0,0,0,235,206,184,6,0,0,0,235,199

globalf(_lzo1y_decompress_asm)
globalf(F(lzo1y_decompress_asm))
_lzo1y_decompress_asm:
F(lzo1y_decompress_asm):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,49,192,49,219,172,60,17,118,35
db 44,17,60,4,115,40,137,193,235,56,5,255,0,0,0,138
db 30,70,8,219,116,244,141,68,24,18,235,18,141,116,38,0
db 138,6,70,60,16,115,73,8,192,116,228,131,192,3,137,193
db 193,232,2,33,233,139,22,131,198,4,137,23,131,199,4,72
db 117,243,243,164,138,6,70,60,16,115,37,193,232,2,138,30
db 141,151,255,251,255,255,141,4,152,70,41,194,138,2,136,7
db 138,66,1,136,71,1,138,66,2,136,71,2,1,239,235,119
db 60,64,114,52,137,193,193,232,2,141,87,255,33,232,138,30
db 193,233,4,141,4,152,70,41,194,73,57,232,115,56,235,120
db 5,255,0,0,0,138,30,70,8,219,116,244,141,76,24,33
db 49,192,235,16,141,116,38,0,60,32,114,124,131,224,31,116
db 228,141,72,2,102,139,6,141,87,255,193,232,2,131,198,2
db 41,194,57,232,114,66,137,203,193,235,2,116,17,139,2,131
db 194,4,137,7,131,199,4,75,117,243,33,233,116,9,138,2
db 66,136,7,71,73,117,247,138,70,254,33,232,15,132,46,255
db 255,255,138,14,70,136,15,71,72,117,247,138,6,70,233,109
db 255,255,255,144,141,116,38,0,135,214,243,164,137,214,235,215
db 129,193,255,0,0,0,138,30,70,8,219,116,243,141,76,11
db 9,235,25,144,141,116,38,0,60,16,114,44,137,193,131,224
db 8,193,224,13,131,225,7,116,221,131,193,2,102,139,6,131
db 198,2,141,151,0,192,255,255,193,232,2,116,43,41,194,233
db 114,255,255,255,141,116,38,0,193,232,2,138,30,141,87,255
db 141,4,152,70,41,194,138,2,136,7,138,90,1,136,95,1
db 131,199,2,233,111,255,255,255,131,249,3,15,149,192,139,84
db 36,40,3,84,36,44,57,214,119,38,114,29,43,124,36,48
db 139,84,36,52,137,58,247,216,131,196,12,90,89,91,94,95
db 93,195,184,1,0,0,0,235,227,184,8,0,0,0,235,220
db 184,4,0,0,0,235,213,137,246,141,188,39,0,0,0,0

globalf(_lzo1y_decompress_asm_safe)
globalf(F(lzo1y_decompress_asm_safe))
_lzo1y_decompress_asm_safe:
F(lzo1y_decompress_asm_safe):
db 85,87,86,83,81,82,131,236,12,252,139,116,36,40,139,124
db 36,48,189,3,0,0,0,141,70,253,3,68,36,44,137,68
db 36,4,137,248,139,84,36,52,3,2,137,4,36,49,192,49
db 219,172,60,17,118,87,44,17,60,4,115,92,141,20,7,57
db 20,36,15,130,130,2,0,0,141,20,6,57,84,36,4,15
db 130,110,2,0,0,137,193,235,110,5,255,0,0,0,141,84
db 6,18,57,84,36,4,15,130,87,2,0,0,138,30,70,8
db 219,116,230,141,68,24,18,235,31,141,180,38,0,0,0,0
db 57,116,36,4,15,130,57,2,0,0,138,6,70,60,16,115
db 127,8,192,116,215,131,192,3,141,84,7,0,57,20,36,15
db 130,37,2,0,0,141,84,6,0,57,84,36,4,15,130,16
db 2,0,0,137,193,193,232,2,33,233,139,22,131,198,4,137
db 23,131,199,4,72,117,243,243,164,138,6,70,60,16,115,64
db 141,87,3,57,20,36,15,130,238,1,0,0,193,232,2,138
db 30,141,151,255,251,255,255,141,4,152,70,41,194,59,84,36
db 48,15,130,218,1,0,0,138,2,136,7,138,66,1,136,71
db 1,138,66,2,136,71,2,1,239,233,163,0,0,0,137,246
db 60,64,114,68,137,193,193,232,2,141,87,255,33,232,138,30
db 193,233,4,141,4,152,70,41,194,73,57,232,115,76,233,181
db 0,0,0,5,255,0,0,0,141,86,3,57,84,36,4,15
db 130,126,1,0,0,138,30,70,8,219,116,231,141,76,24,33
db 49,192,235,20,141,116,38,0,60,32,15,130,200,0,0,0
db 131,224,31,116,224,141,72,2,102,139,6,141,87,255,193,232
db 2,131,198,2,41,194,57,232,114,110,59,84,36,48,15,130
db 77,1,0,0,141,4,15,57,4,36,15,130,58,1,0,0
db 137,203,193,235,2,116,17,139,2,131,194,4,137,7,131,199
db 4,75,117,243,33,233,116,9,138,2,66,136,7,71,73,117
db 247,138,70,254,33,232,15,132,196,254,255,255,141,20,7,57
db 20,36,15,130,2,1,0,0,141,20,6,57,84,36,4,15
db 130,238,0,0,0,138,14,70,136,15,71,72,117,247,138,6
db 70,233,42,255,255,255,137,246,59,84,36,48,15,130,223,0
db 0,0,141,68,15,0,57,4,36,15,130,203,0,0,0,135
db 214,243,164,137,214,235,170,129,193,255,0,0,0,141,86,3
db 57,84,36,4,15,130,169,0,0,0,138,30,70,8,219,116
db 230,141,76,11,9,235,21,144,60,16,114,44,137,193,131,224
db 8,193,224,13,131,225,7,116,225,131,193,2,102,139,6,131
db 198,2,141,151,0,192,255,255,193,232,2,116,57,41,194,233
db 38,255,255,255,141,116,38,0,141,87,2,57,20,36,114,106
db 193,232,2,138,30,141,87,255,141,4,152,70,41,194,59,84
db 36,48,114,93,138,2,136,7,138,90,1,136,95,1,131,199
db 2,233,43,255,255,255,131,249,3,15,149,192,59,60,36,119
db 57,139,84,36,40,3,84,36,44,57,214,119,38,114,29,43
db 124,36,48,139,84,36,52,137,58,247,216,131,196,12,90,89
db 91,94,95,93,195,184,1,0,0,0,235,227,184,8,0,0
db 0,235,220,184,4,0,0,0,235,213,184,5,0,0,0,235
db 206,184,6,0,0,0,235,199,144,141,180,38,0,0,0,0

