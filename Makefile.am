m4datadir = $(datadir)/aclocal
m4data_DATA = skstream.m4

manpagedir = $(mandir)/man1
manpage_DATA = skstream-config.1

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = skstream-0.2.pc

bin_SCRIPTS = skstream-config

if HAVE_PROG_ZIP
MSVC_SUPPORT = VisualC.zip
MSVC_INSTALL_DIST = cp -f
else
MSVC_SUPPORT = 
MSVC_INSTALL_DIST = /bin/true
endif

EXTRA_DIST = skstream.m4 skstream-config.in autogen.sh skstream-config.1 Skstream.pbproj/jmt.pbxuser Skstream.pbproj/project.pbxproj

dist-hook: $(MSVC_SUPPORT)
	$(MSVC_INSTALL_DIST) $(MSVC_SUPPORT) $(distdir)/$(MSVC_SUPPORT)   

VisualC.zip:
	@SK_ZIP@ -r VisualC.zip VisualC/ -x '*CVS/*'

SUBDIRS = skstream test

docdatadir = $(datadir)/doc/skstream-$(VERSION)
docdata_DATA = README COPYING AUTHORS NEWS

docs:
	@echo "running doxygen..."
	@doxygen Doxyfile
	@echo "documentation is in doc/."
