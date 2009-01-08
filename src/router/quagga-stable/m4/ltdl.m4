## ltdl.m4 - Configure ltdl for the target system. -*-Autoconf-*-
## Copyright (C) 1999-2004 Free Software Foundation, Inc.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
##
## As a special exception to the GNU General Public License, if you
## distribute this file as part of a program that contains a
## configuration script generated by Autoconf, you may include it under
## the same distribution terms that you use for the rest of that program.

# serial 6 AC_LIB_LTDL

# AC_WITH_LTDL([DIRECTORY])
# -------------------------
# Clients of libltdl can use this macro to allow the installer to
# choose between a shipped copy of the ltdl sources or a preinstalled
# version of the library.
AC_DEFUN([AC_WITH_LTDL],
[AC_ARG_WITH([included_ltdl],
    [AC_HELP_STRING([--with-included-ltdl],
                    [use the GNU ltdl sources included here])])

if test "x$with_included_ltdl" != xyes; then
  # We are not being forced to use the included libltdl sources, so
  # decide whether there is a useful installed version we can use.
  AC_CHECK_HEADER([ltdl.h],
      [AC_CHECK_LIB([ltdl], [lt_dlcaller_register],
          [with_included_ltdl=no],
          [with_included_ltdl=yes])],

      [],
      [AC_INCLUDES_DEFAULT]
  )
fi

if test "x$enable_ltdl_install" != xyes; then
  # If the user did not specify an installable libltdl, then default
  # to a convenience lib.
  AC_LIBLTDL_CONVENIENCE
fi

if test "x$with_included_ltdl" = xno; then
  # If the included ltdl is not to be used. then Use the
  # preinstalled libltdl we found.
  AC_DEFINE([HAVE_LTDL], 1,
    [Define this if a modern libltdl is already installed])
  LIBLTDL=-lltdl
fi

# Report our decision...
AC_MSG_CHECKING([whether to use included libltdl])
AC_MSG_RESULT([$with_included_ltdl])

AC_CONFIG_SUBDIRS(m4_if($#, 1, [$1], [libltdl]))
])# AC_WITH_LTDL


# AC_LIBLTDL_CONVENIENCE([DIRECTORY])
# -----------------------------------
# sets LIBLTDL to the link flags for the libltdl convenience library and
# LTDLINCL to the include flags for the libltdl header and adds
# --enable-ltdl-convenience to the configure arguments.  Note that LIBLTDL
# and LTDLINCL are not AC_SUBSTed, nor is AC_CONFIG_SUBDIRS called.  If
# DIRECTORY is not provided, it is assumed to be `libltdl'.  LIBLTDL will
# be prefixed with '${top_builddir}/' and LTDLINCL will be prefixed with
# '${top_srcdir}/' (note the single quotes!).  If your package is not
# flat and you're not using automake, define top_builddir and
# top_srcdir appropriately in the Makefiles.
AC_DEFUN([AC_LIBLTDL_CONVENIENCE],
[case $enable_ltdl_convenience in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
LIBLTDL='${top_builddir}/'m4_if($#, 1, [$1], ['libltdl'])/libltdlc.la
LTDLINCL='-I${top_srcdir}/'m4_if($#, 1, [$1], ['libltdl'])

AC_SUBST([LIBLTDL])
AC_SUBST([LTDLINCL])

# For backwards non-gettext consistent compatibility...
INCLTDL="$LTDLINCL"
AC_SUBST([INCLTDL])
])# AC_LIBLTDL_CONVENIENCE


# AC_LIBLTDL_INSTALLABLE([DIRECTORY])
# -----------------------------------
# sets LIBLTDL to the link flags for the libltdl installable library and
# LTDLINCL to the include flags for the libltdl header and adds
# --enable-ltdl-install to the configure arguments.  Note that LIBLTDL
# and LTDLINCL are not AC_SUBSTed, nor is AC_CONFIG_SUBDIRS called.  If
# DIRECTORY is not provided and an installed libltdl is not found, it is
# assumed to be `libltdl'.  LIBLTDL will be prefixed with '${top_builddir}/'
# and LTDLINCL will be prefixed with '${top_srcdir}/' (note the single
# quotes!).  If your package is not flat and you're not using automake,
# define top_builddir and top_srcdir appropriately in the Makefiles.
# In the future, this macro may have to be called after LT_INIT.
AC_DEFUN([AC_LIBLTDL_INSTALLABLE],
[AC_CHECK_LIB(ltdl, lt_dlinit,
  [test x"$enable_ltdl_install" != xyes && enable_ltdl_install=no],
  [if test x"$enable_ltdl_install" = xno; then
     AC_MSG_WARN([libltdl not installed, but installation disabled])
   else
     enable_ltdl_install=yes
   fi
  ])
if test x"$enable_ltdl_install" = x"yes"; then
  ac_configure_args="$ac_configure_args --enable-ltdl-install"
  LIBLTDL='${top_builddir}/'m4_if($#, 1, [$1], ['libltdl'])/libltdl.la
  LTDLINCL='-I${top_srcdir}/'m4_if($#, 1, [$1], ['libltdl'])
else
  ac_configure_args="$ac_configure_args --enable-ltdl-install=no"
  LIBLTDL="-lltdl"
  LTDLINCL=
fi

AC_SUBST([LIBLTDL])
AC_SUBST([LTDLINCL])

# For backwards non-gettext consistent compatibility...
INCLTDL="$LTDLINCL"
AC_SUBST([INCLTDL])
])# AC_LIBLTDL_INSTALLABLE


# AC_LIB_LTDL
# -----------
# Perform all the checks necessary for compilation of the ltdl objects
#  -- including compiler checks and header checks.
AC_DEFUN([AC_LIB_LTDL],
[AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([AC_C_CONST])
AC_REQUIRE([AC_HEADER_STDC])
AC_REQUIRE([AC_HEADER_DIRENT])
AC_REQUIRE([_LT_AC_CHECK_DLFCN])
AC_REQUIRE([AC_LTDL_ENABLE_INSTALL])
AC_REQUIRE([AC_LTDL_SHLIBEXT])
AC_REQUIRE([AC_LTDL_SHLIBPATH])
AC_REQUIRE([AC_LTDL_SYSSEARCHPATH])
AC_REQUIRE([AC_LTDL_OBJDIR])
AC_REQUIRE([AC_LTDL_DLPREOPEN])
AC_REQUIRE([AC_LTDL_DLLIB])
AC_REQUIRE([AC_LTDL_SYMBOL_USCORE])
AC_REQUIRE([AC_LTDL_DLSYM_USCORE])
AC_REQUIRE([AC_LTDL_SYS_DLOPEN_DEPLIBS])
AC_REQUIRE([gl_FUNC_ARGZ])

# In order that ltdl.c can compile, run AC_CONFIG_HEADERS for the user
# if they did not call it themself.  This is so that ltdl.h can pick up
# the parent projects config.h file, The first file in AC_CONFIG_HEADERS
# must contain the definitions required by ltdl.c.
m4_ifset([AC_LIST_HEADERS],
    [CONFIG_H=`echo "AC_LIST_HEADERS" | $SED 's,^[[ 	]]*,,;s,[[ :]].*$,,'`],
  [CONFIG_H=config.h;AC_CONFIG_HEADERS([config.h])])
AC_SUBST([CONFIG_H])

AC_CHECK_HEADERS([memory.h unistd.h dl.h sys/dl.h dld.h mach-o/dyld.h],
	[], [], [AC_INCLUDES_DEFAULT])
AC_CHECK_HEADERS([string.h strings.h], [break], [], [AC_INCLUDES_DEFAULT])

AC_CHECK_FUNCS([closedir opendir readdir], [], [AC_LIBOBJ([lt__dirent])])
])# AC_LIB_LTDL


# AC_LTDL_ENABLE_INSTALL
# ----------------------
AC_DEFUN([AC_LTDL_ENABLE_INSTALL],
[AC_ARG_ENABLE([ltdl-install],
    [AC_HELP_STRING([--enable-ltdl-install], [install libltdl])])

AM_CONDITIONAL(INSTALL_LTDL, test x"${enable_ltdl_install-no}" != xno)
AM_CONDITIONAL(CONVENIENCE_LTDL, test x"${enable_ltdl_convenience-no}" != xno)
])])# AC_LTDL_ENABLE_INSTALL


# AC_LTDL_SYS_DLOPEN_DEPLIBS
# --------------------------
AC_DEFUN([AC_LTDL_SYS_DLOPEN_DEPLIBS],
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_CACHE_CHECK([whether deplibs are loaded by dlopen],
  [libltdl_cv_sys_dlopen_deplibs],
  [# PORTME does your system automatically load deplibs for dlopen?
  # or its logical equivalent (e.g. shl_load for HP-UX < 11)
  # For now, we just catch OSes we know something about -- in the
  # future, we'll try test this programmatically.
  libltdl_cv_sys_dlopen_deplibs=unknown
  case "$host_os" in
  aix3*|aix4.1.*|aix4.2.*)
    # Unknown whether this is true for these versions of AIX, but
    # we want this `case' here to explicitly catch those versions.
    libltdl_cv_sys_dlopen_deplibs=unknown
    ;;
  aix[[45]]*)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  darwin*)
    # Assuming the user has installed a libdl from somewhere, this is true
    # If you are looking for one http://www.opendarwin.org/projects/dlcompat
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  gnu* | linux* | kfreebsd*-gnu | knetbsd*-gnu)
    # GNU and its variants, using gnu ld.so (Glibc)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  hpux10*|hpux11*)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  irix[[12345]]*|irix6.[[01]]*)
    # Catch all versions of IRIX before 6.2, and indicate that we don't
    # know how it worked for any of those versions.
    libltdl_cv_sys_dlopen_deplibs=unknown
    ;;
  irix*)
    # The case above catches anything before 6.2, and it's known that
    # at 6.2 and later dlopen does load deplibs.
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  netbsd*)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  openbsd*)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  osf[[1234]]*)
    # dlopen did load deplibs (at least at 4.x), but until the 5.x series,
    # it did *not* use an RPATH in a shared library to find objects the
    # library depends on, so we explictly say `no'.
    libltdl_cv_sys_dlopen_deplibs=no
    ;;
  osf5.0|osf5.0a|osf5.1)
    # dlopen *does* load deplibs and with the right loader patch applied
    # it even uses RPATH in a shared library to search for shared objects
    # that the library depends on, but there's no easy way to know if that
    # patch is installed.  Since this is the case, all we can really
    # say is unknown -- it depends on the patch being installed.  If
    # it is, this changes to `yes'.  Without it, it would be `no'.
    libltdl_cv_sys_dlopen_deplibs=unknown
    ;;
  osf*)
    # the two cases above should catch all versions of osf <= 5.1.  Read
    # the comments above for what we know about them.
    # At > 5.1, deplibs are loaded *and* any RPATH in a shared library
    # is used to find them so we can finally say `yes'.
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  qnx*)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  solaris*)
    libltdl_cv_sys_dlopen_deplibs=yes
    ;;
  esac
  ])
if test "$libltdl_cv_sys_dlopen_deplibs" != yes; then
 AC_DEFINE([LTDL_DLOPEN_DEPLIBS], [1],
    [Define if the OS needs help to load dependent libraries for dlopen().])
fi
])# AC_LTDL_SYS_DLOPEN_DEPLIBS


# AC_LTDL_SHLIBEXT
# ----------------
AC_DEFUN([AC_LTDL_SHLIBEXT],
[AC_REQUIRE([AC_LIBTOOL_SYS_DYNAMIC_LINKER])
AC_CACHE_CHECK([which extension is used for loadable modules],
  [libltdl_cv_shlibext],
[
module=yes
eval libltdl_cv_shlibext=$shrext_cmds
  ])
if test -n "$libltdl_cv_shlibext"; then
  AC_DEFINE_UNQUOTED(LTDL_SHLIB_EXT, "$libltdl_cv_shlibext",
    [Define to the extension used for shared libraries, say, ".so".])
fi
])# AC_LTDL_SHLIBEXT


# AC_LTDL_SHLIBPATH
# -----------------
AC_DEFUN([AC_LTDL_SHLIBPATH],
[AC_REQUIRE([AC_LIBTOOL_SYS_DYNAMIC_LINKER])
AC_CACHE_CHECK([which variable specifies run-time library path],
  [libltdl_cv_shlibpath_var], [libltdl_cv_shlibpath_var="$shlibpath_var"])
if test -n "$libltdl_cv_shlibpath_var"; then
  AC_DEFINE_UNQUOTED(LTDL_SHLIBPATH_VAR, "$libltdl_cv_shlibpath_var",
    [Define to the name of the environment variable that determines the dynamic library search path.])
fi
])# AC_LTDL_SHLIBPATH


# AC_LTDL_SYSSEARCHPATH
# ---------------------
AC_DEFUN([AC_LTDL_SYSSEARCHPATH],
[AC_REQUIRE([AC_LIBTOOL_SYS_DYNAMIC_LINKER])
AC_CACHE_CHECK([for the default library search path],
  [libltdl_cv_sys_search_path],
  [libltdl_cv_sys_search_path="$sys_lib_dlsearch_path_spec"])
if test -n "$libltdl_cv_sys_search_path"; then
  sys_search_path=
  for dir in $libltdl_cv_sys_search_path; do
    if test -z "$sys_search_path"; then
      sys_search_path="$dir"
    else
      sys_search_path="$sys_search_path$PATH_SEPARATOR$dir"
    fi
  done
  AC_DEFINE_UNQUOTED(LTDL_SYSSEARCHPATH, "$sys_search_path",
    [Define to the system default library search path.])
fi
])# AC_LTDL_SYSSEARCHPATH


# AC_LTDL_OBJDIR
# --------------
AC_DEFUN([AC_LTDL_OBJDIR],
[AC_CACHE_CHECK([for objdir],
  [libltdl_cv_objdir],
  [libltdl_cv_objdir="$objdir"
  if test -n "$objdir"; then
    :
  else
    rm -f .libs 2>/dev/null
    mkdir .libs 2>/dev/null
    if test -d .libs; then
      libltdl_cv_objdir=.libs
    else
      # MS-DOS does not allow filenames that begin with a dot.
      libltdl_cv_objdir=_libs
    fi
  rmdir .libs 2>/dev/null
  fi
  ])
AC_DEFINE_UNQUOTED(LTDL_OBJDIR, "$libltdl_cv_objdir/",
  [Define to the sub-directory in which libtool stores uninstalled libraries.])
])# AC_LTDL_OBJDIR


# AC_LTDL_DLPREOPEN
# -----------------
AC_DEFUN([AC_LTDL_DLPREOPEN],
[AC_REQUIRE([AC_LIBTOOL_SYS_GLOBAL_SYMBOL_PIPE])
AC_CACHE_CHECK([whether libtool supports -dlopen/-dlpreopen],
  [libltdl_cv_preloaded_symbols],
  [if test -n "$lt_cv_sys_global_symbol_pipe"; then
    libltdl_cv_preloaded_symbols=yes
  else
    libltdl_cv_preloaded_symbols=no
  fi
  ])
if test x"$libltdl_cv_preloaded_symbols" = xyes; then
  AC_DEFINE(HAVE_PRELOADED_SYMBOLS, 1,
    [Define if libtool can extract symbol lists from object files.])
fi
])# AC_LTDL_DLPREOPEN


# AC_LTDL_DLLIB
# -------------
AC_DEFUN([AC_LTDL_DLLIB],
[m4_pattern_allow([^LT_DLLOADERS$])
LT_DLLOADERS=
AC_SUBST([LT_DLLOADERS])

AC_LANG_PUSH([C])

LIBADD_DLOPEN=
AC_CHECK_LIB([dl], [dlopen],
	[AC_DEFINE([HAVE_LIBDL], [1],
		   [Define if you have the libdl library or equivalent.])
	LIBADD_DLOPEN="-ldl" libltdl_cv_lib_dl_dlopen="yes"
	LT_DLLOADERS="$LT_DLLOADERS dlopen.la"],
    [AC_TRY_LINK([#if HAVE_DLFCN_H
#  include <dlfcn.h>
#endif
    ], [dlopen(0, 0);],
	    [AC_DEFINE([HAVE_LIBDL], [1],
		       [Define if you have the libdl library or equivalent.])
	    libltdl_cv_func_dlopen="yes"
	    LT_DLLOADERS="$LT_DLLOADERS dlopen.la"],
	[AC_CHECK_LIB([svld], [dlopen],
		[AC_DEFINE([HAVE_LIBDL], [1],
			 [Define if you have the libdl library or equivalent.])
	        LIBADD_DLOPEN="-lsvld" libltdl_cv_func_dlopen="yes"
		LT_DLLOADERS="$LT_DLLOADERS dlopen.la"])])])
if test x"$libltdl_cv_func_dlopen" = xyes || test x"$libltdl_cv_lib_dl_dlopen" = xyes
then
  lt_save_LIBS="$LIBS"
  LIBS="$LIBS $LIBADD_DLOPEN"
  AC_CHECK_FUNCS([dlerror])
  LIBS="$lt_save_LIBS"
fi
AC_SUBST([LIBADD_DLOPEN])

LIBADD_SHL_LOAD=
AC_CHECK_FUNC([shl_load],
	[AC_DEFINE([HAVE_SHL_LOAD], [1],
		   [Define if you have the shl_load function.])
	LT_DLLOADERS="$LT_DLLOADERS shl_load.la"],
    [AC_CHECK_LIB([dld], [shl_load],
	    [AC_DEFINE([HAVE_SHL_LOAD], [1],
		       [Define if you have the shl_load function.])
	    LT_DLLOADERS="$LT_DLLOADERS shl_load.la"
	    LIBADD_SHL_LOAD="-ldld"])])
AC_SUBST([LIBADD_SHL_LOAD])

AC_CHECK_FUNC([_dyld_func_lookup],
	[AC_DEFINE([HAVE_DYLD], [1],
		   [Define if you have the _dyld_func_lookup function.])
	LT_DLLOADERS="$LT_DLLOADERS dyld.la"])

case $host_os in
beos*)
  LT_DLLOADERS="$LT_DLLOADERS load_add_on.la"
  ;;
cygwin* | mingw* | os2* | pw32*)
  LT_DLLOADERS="$LT_DLLOADERS loadlibrary.la"
  ;;
esac

AC_CHECK_LIB([dld], [dld_link],
	[AC_DEFINE([HAVE_DLD], [1],
		   [Define if you have the GNU dld library.])
		LT_DLLOADERS="$LT_DLLOADERS dld_link.la"])
AC_SUBST([LIBADD_DLD_LINK])

m4_pattern_allow([^LT_DLPREOPEN$])
LT_DLPREOPEN=
for lt_loader in $LT_DLLOADERS; do
  LT_DLPREOPEN="$LT_DLPREOPEN-dlpreopen loaders/$lt_loader "
done
AC_SUBST([LT_DLPREOPEN])

dnl This isn't used anymore, but set it for backwards compatibility
LIBADD_DL="$LIBADD_DLOPEN $LIBADD_SHL_LOAD"
AC_SUBST([LIBADD_DL])

AC_LANG_POP
])# AC_LTDL_DLLIB


# AC_LTDL_SYMBOL_USCORE
# ---------------------
# does the compiler prefix global symbols with an underscore?
AC_DEFUN([AC_LTDL_SYMBOL_USCORE],
[AC_REQUIRE([AC_LIBTOOL_SYS_GLOBAL_SYMBOL_PIPE])
AC_CACHE_CHECK([for _ prefix in compiled symbols],
  [ac_cv_sys_symbol_underscore],
  [ac_cv_sys_symbol_underscore=no
  cat > conftest.$ac_ext <<EOF
void nm_test_func(){}
int main(){nm_test_func;return 0;}
EOF
  if AC_TRY_EVAL(ac_compile); then
    # Now try to grab the symbols.
    ac_nlist=conftest.nm
    if AC_TRY_EVAL(NM conftest.$ac_objext \| $lt_cv_sys_global_symbol_pipe \> $ac_nlist) && test -s "$ac_nlist"; then
      # See whether the symbols have a leading underscore.
      if grep '^. _nm_test_func' "$ac_nlist" >/dev/null; then
        ac_cv_sys_symbol_underscore=yes
      else
        if grep '^. nm_test_func ' "$ac_nlist" >/dev/null; then
	  :
        else
	  echo "configure: cannot find nm_test_func in $ac_nlist" >&AS_MESSAGE_LOG_FD
        fi
      fi
    else
      echo "configure: cannot run $lt_cv_sys_global_symbol_pipe" >&AS_MESSAGE_LOG_FD
    fi
  else
    echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
    cat conftest.c >&AS_MESSAGE_LOG_FD
  fi
  rm -rf conftest*
  ])
])# AC_LTDL_SYMBOL_USCORE


# AC_LTDL_DLSYM_USCORE
# --------------------
AC_DEFUN([AC_LTDL_DLSYM_USCORE],
[AC_REQUIRE([AC_LTDL_SYMBOL_USCORE])
if test x"$ac_cv_sys_symbol_underscore" = xyes; then
  if test x"$libltdl_cv_func_dlopen" = xyes ||
     test x"$libltdl_cv_lib_dl_dlopen" = xyes ; then
	AC_CACHE_CHECK([whether we have to add an underscore for dlsym],
	  [libltdl_cv_need_uscore],
	  [libltdl_cv_need_uscore=unknown
          save_LIBS="$LIBS"
          LIBS="$LIBS $LIBADD_DLOPEN"
	  _LT_AC_TRY_DLOPEN_SELF(
	    [libltdl_cv_need_uscore=no], [libltdl_cv_need_uscore=yes],
	    [],				 [libltdl_cv_need_uscore=cross])
	  LIBS="$save_LIBS"
	])
  fi
fi

if test x"$libltdl_cv_need_uscore" = xyes; then
  AC_DEFINE(NEED_USCORE, 1,
    [Define if dlsym() requires a leading underscore in symbol names.])
fi
])# AC_LTDL_DLSYM_USCORE
