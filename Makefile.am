
# Automake file for cdf_handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

SUBDIRS = bes-testsuite

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libcdf_module.la

CDFSRCS:= CDFArray.cc CDFByte.cc CDFFloat32.cc CDFFloat64.cc CDFGrid.cc \
	  CDFInt16.cc CDFInt32.cc CDFSequence.cc CDFStr.cc CDFStructure.cc \
	  CDFUInt16.cc CDFUInt32.cc CDFUrl.cc \
	  CDFreadAttributes.cc CDFreadDescriptors.cc CDFutilities.cc

CDFHDRS:= CDFArray.h CDFByte.h CDFFloat32.h CDFFloat64.h CDFGrid.h \
	  CDFInt16.h CDFInt32.h CDFSequence.h CDFStr.h CDFStructure.h \
	  CDFUInt16.h CDFUInt32.h CDFUrl.h CDFnamePath.h \
	  CDFreadAttributes.h CDFreadDescriptors.h CDFutilities.h \
	  config_cdf.h

SERVER_SRC:=CDFRequestHandler.cc CDFModule.cc CDFRequestHandler.h \
		CDFModule.h CDFResponseNames.h

libcdf_module_la_SOURCES = $(SERVER_SRC) $(CDFSRCS) $(CDFHDRS)
libcdf_module_la_CPPFLAGS = -DHAVE_BES=1 $(BES_CPPFLAGS)
libcdf_module_la_LDFLAGS = -avoid-version -module 
libcdf_module_la_LIBADD = $(BES_DAP_LIBS)

EXTRA_DIST = data COPYRIGHT COPYING cdf_handler.spec \
	cdf.conf.in OSX_Resources

CLEANFILES = *~ cdf.conf

sample_datadir = $(datadir)/hyrax/data/cdf
sample_data_DATA = data/example.cdf data/example1.cdf data/geocpi0.cdf data/wind.cdf

install-data-local: cdf.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/cdf.conf && mv -f $(DESTDIR)$(sysconfdir)/bes/modules/cdf.conf $(DESTDIR)$(sysconfdir)/bes/modules/cdf.conf.$$currdate; \
	$(INSTALL_DATA) cdf.conf $(DESTDIR)$(sysconfdir)/bes/modules/cdf.conf

uninstall-local:
	rm -f $(DESTDIR)$(sysconfdir)/bes/modules/cdf.conf

cdf.conf: cdf.conf.in config.status
	sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > cdf.conf

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
	rpmbuild -ts --clean @PACKAGE@-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb --clean @PACKAGE@-@PACKAGE_VERSION@.tar.gz

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

