
# Automake file for cdf-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

bin_PROGRAMS = dap_cdf_handler

CDFSRCS:=CDFArray.cc CDFFloat32.cc CDFFloat64.cc CDFInt32.cc CDFUInt32.cc \
	CDFByte.cc CDFStr.cc CDFreadAttributes.cc CDFreadDescriptors.cc \
	CDFutilities.cc CDFTypeFactory.cc

CDFHDRS:=CDFArray.h CDFFloat32.h CDFFloat64.h CDFInt32.h CDFUInt32.h \
	CDFByte.h CDFStr.h CDFreadAttributes.h CDFreadDescriptors.h \
	CDFutilities.h CDFTypeFactory.h

HANDLER_SRC = cdf_handler.cc cdf_module.cc CDFHandlerApp.cc \
	CDFRequestHandler.cc CDFHandlerApp.h CDFRequestHandler.h \
	CDFResponseNames.h config_cdf.h

dap_cdf_handler_SOURCES = $(CDFSRCS) $(HANDLER_SRC) $(CDFHDRS)
dap_cdf_handler_LDADD = $(BES_LDADD)

EXTRA_DIST = COPYRIGHT COPYING cdf_handler.spec

CLEANFILES = *~

###########################################################################

# Build linux RPMs

srpm:
	rpmbuild -ts cdf_handler-@PACKAGE_VERSION@.tar.gz

rpm:
	rpmbuild -tb cdf_handler-@PACKAGE_VERSION@.tar.gz
