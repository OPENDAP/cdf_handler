
# Automake file for cdf_handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

bin_PROGRAMS = dap_cdf_handler

lib_LTLIBRARIES = libcdf_handler.la
if OPENDAPSERVER
    lib_besdir=$(libdir)/bes
    lib_bes_LTLIBRARIES = libcdf_module.la
endif

CDFSRCS:= CDFArray.cc CDFByte.cc CDFFloat32.cc CDFFloat64.cc CDFGrid.cc \
	  CDFInt16.cc CDFInt32.cc CDFSequence.cc CDFStr.cc CDFStructure.cc \
	  CDFTypeFactory.cc CDFUInt16.cc CDFUInt32.cc CDFUrl.cc \
	  CDFreadAttributes.cc CDFreadDescriptors.cc CDFutilities.cc

CDFHDRS:= CDFArray.h CDFByte.h CDFFloat32.h CDFFloat64.h CDFGrid.h \
	  CDFInt16.h CDFInt32.h CDFSequence.h CDFStr.h CDFStructure.h \
	  CDFTypeFactory.h CDFUInt16.h CDFUInt32.h CDFUrl.h CDFnamePath.h \
	  CDFreadAttributes.h CDFreadDescriptors.h CDFutilities.h \
	  config_cdf.h

libcdf_handler_la_SOURCES = $(CDFSRCS) $(CDFHDRS)

SERVER_SRC:=CDFRequestHandler.cc CDFModule.cc CDFRequestHandler.h \
		CDFModule.h CDFResponseNames.h

libcdf_module_la_SOURCES = $(SERVER_SRC) $(CDFSRCS) $(CDFHDRS)
libcdf_module_la_CPPFLAGS = -DHAVE_BES=1 $(BES_CPPFLAGS)
libcdf_module_la_LDFLAGS = -avoid-version -module 
libcdf_module_la_LIBADD = $(BES_LIBS)

dap_cdf_handler_SOURCES = dap_cdf_handler.cc
dap_cdf_handler_LDADD = libcdf_handler.la

EXTRA_DIST = COPYRIGHT COPYING

CLEANFILES = *~

###########################################################################

# Build linux RPMs

srpm:
	rpmbuild -ts cdf_handler-@PACKAGE_VERSION@.tar.gz

rpm:
	rpmbuild -tb cdf_handler-@PACKAGE_VERSION@.tar.gz

