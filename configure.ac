# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([cdf-handler], [1.0.0], [dods-tech@unidata.ucar.edu])
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
AC_CHECK_LIBDAP([3.5.3],
 [
  LIBS="$LIBS $DAP_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])
  
AC_ARG_WITH([bes],
            [AS_HELP_STRING([--with-bes=PATH],[bes install root directory])],
            [BES_PATH=${withval}], 
            [BES_PATH=""])

if test "${BES_PATH}" != "no"
then
    if test -x "${BES_PATH}/bin/bes-config"
    then
	AM_CONDITIONAL([OPENDAPSERVER], [true])
	BES_CPPFLAGS="`${BES_PATH}/bin/bes-config --cflags`"
	AC_SUBST([BES_CPPFLAGS])
	BES_LIBS="`${BES_PATH}/bin/bes-config --dap-libs`"
	AC_SUBST([BES_LIBS])
	BES_MODULE_DIR="`${BES_PATH}/bin/bes-config --modulesdir`"
	AC_SUBST([BES_MODULE_DIR])
    else
	AC_CHECK_BES([3.1.0],
	[
	    # I think the AC_SUBST([BES_MODULE_DIR]) in bes.m4 will put that 
	    # symbol in the output. Same for BES_CPPFLAGS. jhrg 5/1/06
	    AM_CONDITIONAL([OPENDAPSERVER], [true])
	],
	[ AM_CONDITIONAL([OPENDAPSERVER], [false])
	])

    fi
else
    AM_CONDITIONAL([OPENDAPSERVER], [false])
fi

dnl Check for the cdf library.
AC_DEFUN([AC_CHECK_CDF],
[
AC_ARG_WITH([cdf],
            [AS_HELP_STRING([--with-cdf=ARG],[cdf directory])],
            [CDF_PATH=${withval}], 
            [CDF_PATH="/usr/local/cdf"])

AC_ARG_WITH([cdf_inc],
            [AS_HELP_STRING([--with-cdf-include=ARG],[cdf include directory])],
            [CDF_PATH_INC=${withval}], 
            [CDF_PATH_INC=""])

AC_ARG_WITH([cdf_libdir],
            [AS_HELP_STRING([--with-cdf-libdir=ARG],[cdf library directory])],
            [CDF_PATH_LIBDIR=${withval}], 
            [CDF_PATH_LIBDIR=""])

AS_IF([test "z$CDF_PATH_LIBDIR" = "z"],[CDF_PATH_LIBDIR="$CDF_PATH/lib"])

AS_IF([test "z$CDF_PATH_INC" = "z"],[CDF_PATH_INC="$CDF_PATH/include"])

ac_cdf_ok='yes'
CDF_LIBS="-lcdf"
CDF_LDFLAGS="-L${CDF_PATH_LIBDIR}" 

AC_SUBST([CDF_LDFLAGS])
AC_SUBST([CDF_LIBS])

CDF_CPPFLAGS=
ac_cdf_h='no'
ac_cdf_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I$CDF_PATH_INC"
AC_CHECK_HEADERS([cdf.h],
  [ac_cdf_h='yes'],
  [ac_cdf_h='no']
)
CPPFLAGS=$ac_cdf_save_CPPFLAGS

AS_IF([test "$ac_cdf_h" = 'yes' ],
  [CDF_CPPFLAGS="-I$CDF_PATH_INC"]
)
AC_SUBST([CDF_CPPFLAGS])

AS_IF([test "$ac_cdf_ok" = 'no' -o "$ac_cdf_h" = 'no'],
  [m4_if([$2], [], [:], [$2])],
  [m4_if([$1], [], [:], [$1])]
)

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

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
