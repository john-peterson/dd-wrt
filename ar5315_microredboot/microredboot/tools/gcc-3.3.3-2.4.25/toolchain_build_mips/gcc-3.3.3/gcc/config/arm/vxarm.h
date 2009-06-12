/* Definitions of target machine for GNU compiler,
   for ARM with targetting the VXWorks run time environment. 
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.

   Contributed by: Mike Stump <mrs@wrs.com>
   
This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#define SUBTARGET_CPU_DEFAULT TARGET_CPU_arm710

#undef  SUBTARGET_CPP_SPEC
#define SUBTARGET_CPP_SPEC		\
"%{march=arm710:-DCPU=ARM710A}		\
 %{march=arm7tdmi:-DCPU=ARM7TDMI}	\
 %{march=arm810:-DCPU=ARM810}		\
 %{march=strongarm110:-DCPU=ARMSA110}	\
 %{!march=*:				\
  %{mcpu=arm710:-DCPU=ARM710A}		\
  %{mcpu=arm7tdmi:-DCPU=ARM7TDMI}	\
  %{mcpu=arm810:-DCPU=ARM810}		\
  %{mcpu=strongarm110:-DCPU=ARMSA110}}	\
  %{!mcpu*:%{!march=*:-DCPU=ARM710A}}	\
"

#define TARGET_OS_CPP_BUILTINS()		\
    do {					\
	builtin_define ("__vxworks");		\
    } while (0)

/* VxWorks does all the library stuff itself.  */
#undef  LIB_SPEC
#define LIB_SPEC 	""

/* VxWorks uses object files, not loadable images.  make linker just
   combine objects. */
#undef  LINK_SPEC
#define LINK_SPEC 	"-r"

/* VxWorks provides the functionality of crt0.o and friends itself.  */
#undef  STARTFILE_SPEC
#define STARTFILE_SPEC 	""

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC 	""

#undef  TARGET_VERSION
#define TARGET_VERSION	fputs (" (ARM/VxWorks)", stderr);

#undef  ASM_FILE_START
#define ASM_FILE_START(STREAM) 						\
  do 									\
    {									\
      fprintf (STREAM, "%s Generated by gcc %s for ARM/VxWorks\n",	\
	       ASM_COMMENT_START, version_string);			\
    }									\
  while (0)
