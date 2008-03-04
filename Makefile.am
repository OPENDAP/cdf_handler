
# Automake file for cdf_handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

SUBDIRS = testsuite

bin_PROGRAMS = dap_cdf_handler

lib_LTLIBRARIES = libcdf_handler.la

if OPENDAPSERVER

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libcdf_module.la
dist_bin_SCRIPTS = bes-cdf-data.sh

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
libcdf_handler_la_LDFLAGS = -version-info $(LIBCDF_HANDLER_VERSION)

SERVER_SRC:=CDFRequestHandler.cc CDFModule.cc CDFRequestHandler.h \
		CDFModule.h CDFResponseNames.h

libcdf_module_la_SOURCES = $(SERVER_SRC) $(CDFSRCS) $(CDFHDRS)
libcdf_module_la_CPPFLAGS = -DHAVE_BES=1 $(BES_CPPFLAGS)
libcdf_module_la_LDFLAGS = -avoid-version -module 
libcdf_module_la_LIBADD = $(BES_DAP_LIBS)

dap_cdf_handler_SOURCES = dap_cdf_handler.cc
dap_cdf_handler_LDADD = libcdf_handler.la

EXTRA_DIST = data COPYRIGHT COPYING cdf_handler.spec \
	bes-cdf-data.sh.in OSX_Resources

CLEANFILES = *~

if OPENDAPSERVER

CLEANFILES += bes-cdf-data.sh

sample_datadir = $(datadir)/hyrax/data/cdf
sample_data_DATA = data/example.cdf data/example1.cdf data/geocpi0.cdf data/wind.cdf


do_subst = sed -e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		   	   -e 's,[@]libdir[@],$(libdir),g' \
		   	   -e 's,[@]datadir[@],$(datadir),g'

bes-cdf-data.sh: bes-cdf-data.sh.in config.status
	$(do_subst) < $(srcdir)/bes-cdf-data.sh.in > bes-cdf-data.sh
	chmod +x bes-cdf-data.sh

# This makes sure that the handler modifies the bes which configure found
# even when distcheck is run.
bes-conf: bes-cdf-data.sh
	(bes_prefix=`bes-config --prefix` ; \
	if test "$$bes_prefix" = "/usr" ; \
	 then \
	    ./bes-cdf-data.sh /etc/bes/bes.conf /usr/lib/bes ;\
	else \
	    ./bes-cdf-data.sh $$bes_prefix/etc/bes/bes.conf $$bes_prefix/lib/bes ; \
	fi)

endif

###########################################################################

# Fortify targets.

.PHONY: fortify
fortify:
	sourceanalyzer -b @PACKAGE@ $(MAKE)
	sourceanalyzer -b @PACKAGE@ -scan -f @PACKAGE@-@PACKAGE_VERSION@.fpr

# Use this to clean the fortify project.
.PHONY: fortifyclean
fortifyclean:
	sourceanalyzer -b @PACKAGE@ -clean

###########################################################################
#
# Build linux RPMs

srpm: dist
	rpmbuild -ts cdf_handler-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb cdf_handler-@PACKAGE_VERSION@.tar.gz

###########################################################################

# Build OS/X Packages.

PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker*

UNIV_ARCH=-arch ppc -arch i386
MAC_SYSROOT=Developer/SDKs/MacOSX10.4u.sdk

PKG_CONF_FLAGS=

clean-pkg:
	-rm -rf mac_osx @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@

pkg-build:
	./configure --prefix=/usr --disable-dependency-tracking $(PKG_CONF_FLAGS)
	make clean all
	DESTDIR=${PWD}/mac_osx make install

pkg-main:
	./OSX_Resources/update_mac_package_contents.pl README "configure.ac" "mac_osx/usr"
	${PACKAGEMAKER} -build -p @PACKAGE@-@PACKAGE_VERSION@.pkg \
		-f mac_osx/usr \
		-ds -r OSX_Resources \
		-i OSX_Resources/Info.plist \
		-d OSX_Resources/Description.plist
	-rm -rf mac_osx

pkg-dmg:
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@.dmg
	mkdir @PACKAGE@-@PACKAGE_VERSION@
	mv @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@
	cp README NEWS @PACKAGE@-@PACKAGE_VERSION@
	dropdmg -i --sanitize-for-servers --format zlib @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@

pkg: clean-pkg pkg-build pkg-main pkg-dmg

