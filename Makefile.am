
# Automake file for netcdf-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

bin_PROGRAMS = dap_cdf_handler

if OPENDAPSERVER
    lib_LTLIBRARIES = libcdf_handler.la libcdf_module.la
    noinst_PROGRAMS = bes_cdf_handler
else
    lib_LTLIBRARIES = libcdf_handler.la
endif

CDFSRCS:= CDFArray.cc CDFByte.cc CDFFloat32.cc CDFFloat64.cc CDFGrid.cc \
	  CDFInt16.cc CDFInt32.cc CDFSequence.cc CDFStr.cc CDFStructure.cc \
	  CDFTypeFactory.cc CDFUInt16.cc CDFUInt32.cc CDFUrl.cc \
	  CDFreadAttributes.cc CDFreadDescriptors.cc CDFutilities.cc \
	  CDFDebug.cc

CDFHDRS:= CDFArray.h CDFByte.h CDFFloat32.h CDFFloat64.h CDFGrid.h \
	  CDFInt16.h CDFInt32.h CDFSequence.h CDFStr.h CDFStructure.h \
	  CDFTypeFactory.h CDFUInt16.h CDFUInt32.h CDFUrl.h CDFnamePath.h \
	  CDFreadAttributes.h CDFreadDescriptors.h CDFutilities.h \
	  CDFDebug.h config_cdf.h

libcdf_handler_la_SOURCES = $(CDFSRCS) $(CDFHDRS)

SERVER_SRC:=CDFRequestHandler.cc CDFModule.cc CDFRequestHandler.h \
		CDFModule.h CDFResponseNames.h

libcdf_module_la_SOURCES = $(SERVER_SRC)
libcdf_module_la_CPPFLAGS = $(BES_CFLAGS)
libcdf_module_la_LDFLAGS = libcdf_handler.la $(BES_LDFLAGS)

dap_cdf_handler_SOURCES = dap_cdf_handler.cc
dap_cdf_handler_LDADD = libcdf_handler.la

BES_HANDLER_SRC = bes_cdf_handler.cc CDFHandlerApp.cc CDFHandlerApp.h

bes_cdf_handler_SOURCES = $(BES_HANDLER_SRC)
bes_cdf_handler_CPPFLAGS = $(BES_CFLAGS)
bes_cdf_handler_LDFLAGS = libcdf_module.la libcdf_handler.la $(BES_LDFLAGS) 

EXTRA_PROGRAMS = bes_cdf_handler

EXTRA_DIST = COPYRIGHT COPYING netcdf_handler.spec

CLEANFILES = *~

###########################################################################

# Build linux RPMs

srpm:
	rpmbuild -ts cdf_handler-@PACKAGE_VERSION@.tar.gz

rpm:
	rpmbuild -tb cdf_handler-@PACKAGE_VERSION@.tar.gz

