# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([cdf_handler], [1.0.2], [dods-tech@unidata.ucar.edu])
AC_CONFIG_AUX_DIR([conf])

AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([CDFStr.h])
AC_CONFIG_HEADER([config_cdf.h])

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
AC_CHECK_LIBDAP([3.7.10],
 [
  LIBS="$LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])
  
AC_CHECK_BES([3.5.3],
[
    # I think the AC_SUBST([BES_MODULE_DIR]) in bes.m4 will put that 
    # symbol in the output. Same for BES_CPPFLAGS. jhrg 5/1/06
    AM_CONDITIONAL([OPENDAPSERVER], [true])
],
[ AM_CONDITIONAL([OPENDAPSERVER], [false])
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

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
