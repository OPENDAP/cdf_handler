# check for cedar libraries, not in usual location
AC_DEFUN(DODS_CDF_PACKAGE, [dnl
    AC_ARG_WITH(cdf,
        [  --with-cdf=ARG          Where is the local cdf library sources (directory)],
        CDF_PATH=${withval}, CDF_PATH="$CDF_PATH")
    if test ! -d "$CDF_PATH"
    then
	AC_MSG_ERROR([Could not find CDF library])
    else
	INCS="$INCS -I${CDF_PATH}/include"
	LIBS="$LIBS -L${CDF_PATH}/lib -lcdf"
	AC_SUBST(INCS)
    fi])

