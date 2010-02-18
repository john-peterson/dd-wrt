dnl AC_VALIDATE_CACHE_SYSTEM_TYPE[(cmd)]
dnl if the cache file is inconsistent with the current host,
dnl target and build system types, execute CMD or print a default
dnl error message.
AC_DEFUN([AC_VALIDATE_CACHE_SYSTEM_TYPE], [
    AC_REQUIRE([AC_CANONICAL_SYSTEM])
    AC_MSG_CHECKING([config.cache system type])
    if { test x"${ac_cv_host_system_type+set}" = x"set" &&
         test x"$ac_cv_host_system_type" != x"$host"; } ||
       { test x"${ac_cv_build_system_type+set}" = x"set" &&
         test x"$ac_cv_build_system_type" != x"$build"; } ||
       { test x"${ac_cv_target_system_type+set}" = x"set" &&
         test x"$ac_cv_target_system_type" != x"$target"; }; then
	AC_MSG_RESULT([different])
	ifelse($#, 1, [$1],
		[AC_MSG_ERROR([you must remove config.cache and restart configure])])
    else
	AC_MSG_RESULT([same])
    fi
    ac_cv_host_system_type="$host"
    ac_cv_build_system_type="$build"
    ac_cv_target_system_type="$target"
])

dnl based on Automake's maintainer mode
AC_DEFUN([SAMBA_MAINTAINER_MODE],[
  AC_ARG_ENABLE(maintainer-mode,
	[  --enable-maintainer-mode enable some make rules for maintainers],
      maint_mode=$enableval, maint_mode=no)
  if test x"$maint_mode" = x"yes"; then MAINT=; else MAINT='#'; fi
  AC_SUBST(MAINT)
  AC_PATH_PROG(AUTOCONF, autoconf, autoconf)
  AC_SUBST(AUTOCONF)
  AC_PATH_PROG(AUTOHEADER, autoheader, autoheader)
  AC_SUBST(AUTOHEADER)
])


dnl AC_PROG_CC_FLAG(flag)
AC_DEFUN([AC_PROG_CC_FLAG],
[AC_CACHE_CHECK([whether ${CC-cc} accepts -$1], ac_cv_prog_cc_$1,
[echo 'void f(){}' > conftest.c
if test -z "`${CC-cc} -$1 -c conftest.c 2>&1`"; then
  ac_cv_prog_cc_$1=yes
else
  ac_cv_prog_cc_$1=no
fi
rm -f conftest*
])])

dnl check for a function in a library, but don't
dnl keep adding the same library to the LIBS variable.
dnl AC_LIBTESTFUNC(lib,func)
AC_DEFUN([AC_LIBTESTFUNC],
[case "$LIBS" in
  *-l$1*) AC_CHECK_FUNCS($2) ;;
  *) AC_CHECK_LIB($1, $2) 
     AC_CHECK_FUNCS($2)
  ;;
  esac
])
