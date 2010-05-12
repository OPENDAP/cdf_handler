# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(cdf_handler, 1.0.4, opendap-tech@opendap.org)
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADER([config_cdf.h])
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])

AM_INIT_AUTOMAKE(1.10)
AC_CONFIG_TESTDIR(bes-testsuite, [.])

dnl flags for the compilers and linkers - set these before locating the
dnl actual tools since some of the AC_PROG macros set these `flag variables'
dnl to default values otherwise.

AC_CANONICAL_HOST
AC_SUBST(host)

dnl library visioning: Update these when the interface changes. Generally,
dnl assume that the interface tracks the major and minor release numbers.
LIB_CDF_HANDLER_CURRENT=1
LIB_CDF_HANDLER_AGE=0
LIB_CDF_HANDLER_REVISION=0
AC_SUBST(LIB_CDF_HANDLER_CURRENT)
AC_SUBST(LIB_CDF_HANDLER_AGE)
AC_SUBST(LIB_CDF_HANDLER_REVISION)
LIBCDF_HANDLER_VERSION="$LIB_CDF_HANDLER_CURRENT:$LIB_CDF_HANDLER_REVISION:$LIB_CDF_HANDLER_AGE"
AC_SUBST(LIBCDF_HANDLER_VERSION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.10.2],
 [
  LIBS="$LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])
  
AC_CHECK_BES([3.8.3],
[
],
[ AC_MSG_ERROR([Cannot find bes])
])

AC_CHECK_CDF(
   [
       LIBS="$LIBS $CDF_LIBS"
       LDFLAGS="$LDFLAGS $CDF_LDFLAGS"
       CPPFLAGS="$CPPFLAGS $CDF_CPPFLAGS"
   ],
   [
       echo "*** Use --with-cdf for the root cdf directory."
       echo "*** Otherwise use --with-cdf-include switch for includes directory"
       echo "*** and --with-cdf-libdir switch for libraries directory."
       AC_MSG_ERROR([cdf library and cdf headers are required.])
   ]
)

dnl if test "$CDF_PATH"
dnl then
dnl     AC_MSG_RESULT([$CDF_PATH])
dnl     LDFLAGS="$LDFLAGS -L${CDF_PATH}/lib"
dnl     LIBS="$LIBS -lcdf"
dnl     CPPFLAGS="$CPPFLAGS -I${CDF_PATH}/include"
dnl fi

OPENDAP_DEBUG_OPTION

AH_TOP([#ifndef _config_cdf_h
#define _config_cdf_h])

AH_BOTTOM([/* Shorthand for gcc's unused attribute feature */
#ifdef HAVE_BES
#include "BESDebug.h"
#else
#define BESDEBUG( x, y ) ;
#define BESISDEBUG( x ) false
#endif /* HAVE_BES */

#endif /* _config_cdf_h */])

AC_CONFIG_FILES([Makefile
		 bes-testsuite/atlocal
		 bes-testsuite/Makefile])
AC_OUTPUT
