dnl ** Additional Autoconf tests for BIRD configure script
dnl ** (c) 1999 Martin Mares <mj@ucw.cz>

AC_DEFUN(BIRD_CHECK_INTEGERS,
[AC_CHECK_SIZEOF(char, 0)
AC_CHECK_SIZEOF(short int, 0)
AC_CHECK_SIZEOF(int, 0)
AC_CHECK_SIZEOF(long int, 0)
for size in 1 2 4 ; do
	bits=`expr $size "*" 8`
	AC_MSG_CHECKING([for $bits-bit type])
	if test $ac_cv_sizeof_int = $size ; then
		res=int
	elif test $ac_cv_sizeof_char = $size ; then
		res=char
	elif test $ac_cv_sizeof_short_int = $size ; then
		res="short int"
	elif test $ac_cv_sizeof_long_int = $size ; then
		res="long int"
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([Cannot find $bits-bit integer type.])
	fi
	AC_MSG_RESULT($res)
	AC_DEFINE_UNQUOTED(INTEGER_$bits, $res)
	done
])

AC_DEFUN(BIRD_CHECK_ENDIAN,
[AC_CACHE_CHECK([CPU endianity], bird_cv_c_endian,[
AC_TRY_RUN([
#include <stdio.h>

unsigned int x = 0x12345678;
unsigned char *z = (unsigned char *) &x;

int main(void)
{
  FILE *f = fopen("conftestresult", "w");
  if (!f) return 10;
  fprintf(f, "%02x %02x %02x %02x", *z, *(z+1), *(z+2), *(z+3));
  fclose(f);
  exit(0);
}
],[
	endian=`cat conftestresult`
	if test "$endian" = "12 34 56 78" ; then
		bird_cv_c_endian=big-endian
	elif test "$endian" = "78 56 34 12" ; then
		bird_cv_c_endian=little-endian
	fi
],[endian="test program failed"],[endian="not available, we're cross compiling"])
if test -z "$bird_cv_c_endian" ; then
	AC_MSG_RESULT($endian)
	AC_MSG_ERROR([Cannot determine CPU endianity.])
	fi
])
case $bird_cv_c_endian in
	big-endian)	AC_DEFINE(CPU_BIG_ENDIAN) ;;
	little-endian)	AC_DEFINE(CPU_LITTLE_ENDIAN) ;;
	esac
])

AC_DEFUN(BIRD_CHECK_STRUCT_ALIGN,
[AC_CACHE_CHECK([usual alignment of structures],bird_cv_c_struct_align,[
AC_TRY_RUN([
#include <stdio.h>

struct { char x; long int y; } ary[2];

int main(void)
{
  FILE *f = fopen("conftestresult", "w");
  if (!f) return 10;
  fprintf(f, "%d", sizeof(ary)/2);
  fclose(f);
  exit(0);
}
],[
bird_cv_c_struct_align=`cat conftestresult`
],[
AC_MSG_RESULT([test program failed])
AC_MSG_ERROR([Cannot determine structure alignment])
],[bird_cv_c_struct_align=16])
])
AC_DEFINE_UNQUOTED(CPU_STRUCT_ALIGN, $bird_cv_c_struct_align)
])

AC_DEFUN(BIRD_CHECK_TIME_T,
[AC_CACHE_CHECK([characteristics of time_t], bird_cv_type_time_t, [
AC_TRY_RUN([
#include <stdio.h>
#include <sys/time.h>
#include <limits.h>

int main(void)
{
  FILE *f = fopen("conftestresult", "w");
  if (!f) return 10;
  fprintf(f, "%d-bit ", sizeof(time_t)*CHAR_BIT);
  if ((time_t) -1 > 0) fprintf(f, "un");
  fprintf(f, "signed");
  fclose(f);
  exit(0);
}
],[bird_cv_type_time_t=`cat conftestresult`
],[	AC_MSG_RESULT([test program failed])
	AC_MSG_ERROR([Cannot determine time_t size and signedness.])
],[bird_cv_type_time_t="32-bit signed"])
])
case "$bird_cv_type_time_t" in
	*64-bit*)	AC_DEFINE(TIME_T_IS_64BIT) ;;
	esac
case "$bird_cv_type_time_t" in
	*unsigned*)	;;
	*)		AC_DEFINE(TIME_T_IS_SIGNED) ;;
	esac
])

AC_DEFUN(BIRD_CHECK_STRUCT_IP_MREQN,
[AC_CACHE_CHECK([for struct ip_mreqn], bird_cv_struct_ip_mreqn,[
AC_TRY_COMPILE([#include <netinet/in.h>
],[struct ip_mreqn x;
],[bird_cv_struct_ip_mreqn=yes
],[bird_cv_struct_ip_mreqn=no
])])
if test "$bird_cv_struct_ip_mreqn" = yes ; then
	AC_DEFINE(HAVE_STRUCT_IP_MREQN)
fi
])

AC_DEFUN(BIRD_CHECK_LINUX_VERSION,
[AC_CACHE_CHECK([Linux kernel version], bird_cv_sys_linux_version, [
AC_REQUIRE_CPP()dnl
cat > conftest.$ac_ext <<EOF
[#]line __oline__ "configure"
#include "confdefs.h"
#include <linux/version.h>
VERSION: UTS_RELEASE
EOF
bird_cv_sys_linux_version=`eval "$ac_cpp conftest.$ac_ext" 2>&AC_FD_CC | sed '/^VERSION/!d;s/^VERSION: "//;s/".*//'`
rm -rf conftest*
if test -z "$bird_cv_sys_linux_version" ; then
	AC_MSG_RESULT([unknown])
	AC_MSG_ERROR([Cannot determine kernel version])
fi
])])
