lib_LTLIBRARIES = libskstream-0.3.la
libskstream_0_3_la_LDFLAGS = -version-info @SKSTREAM_VERSION_INFO@

libskstream_0_3_la_SOURCES = skstream.cpp skserver.cpp skpoll.cpp

libskstreamincludedir = $(includedir)/skstream-0.3/skstream
libskstreaminclude_HEADERS = skstream.h skserver.h sksystem.h skpoll.h sasproto.h

m4datadir = $(datadir)/aclocal
m4data_DATA = skstream.m4

manpagedir = $(mandir)/man1
manpage_DATA = skstream-config.1

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = skstream-0.3.pc

bin_SCRIPTS = skstream-config

if HAVE_PROG_ZIP
MSVC_SUPPORT = VisualC.zip
MSVC_INSTALL_DIST = cp -f
else
MSVC_SUPPORT = 
MSVC_INSTALL_DIST = /bin/true
endif

EXTRA_DIST = skstream.m4 skstream-config.in autogen.sh sksystem.h.in skstream-config.1

dist-hook: $(MSVC_SUPPORT)
	$(MSVC_INSTALL_DIST) $(MSVC_SUPPORT) $(distdir)/$(MSVC_SUPPORT)   

VisualC.zip:
	@SK_ZIP@ -r VisualC.zip VisualC/ -x '*CVS/*'

SUBDIRS = . test

docdatadir = $(datadir)/doc/skstream-$(VERSION)
docdata_DATA = README COPYING AUTHORS NEWS

docs:
	@echo "running doxygen..."
	@doxygen Doxyfile
	@echo "documentation is in doc/."
