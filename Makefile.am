lib_LTLIBRARIES = libskstream.la
libskstream_la_LDFLAGS = -version-info @SKSTREAM_VERSION_INFO@

libskstream_la_SOURCES = skstream.cpp skserver.cpp

libskstreamincludedir = $(includedir)/skstream
libskstreaminclude_HEADERS = skstream.h skserver.h sasproto.h

m4datadir = $(datadir)/aclocal
m4data_DATA = skstream.m4

bin_SCRIPTS = skstream-config

if HAVE_PROG_ZIP
MSVC_SUPPORT = VisualC.zip
MSVC_INSTALL_DIST = cp -f
else
MSVC_SUPPORT = 
MSVC_INSTALL_DIST = /bin/true
endif

EXTRA_DIST = skstream.m4 skstream-config.in autogen.sh 

dist-hook: $(MSVC_SUPPORT)
	$(MSVC_INSTALL_DIST) $(MSVC_SUPPORT) $(distdir)/$(MSVC_SUPPORT)   

VisualC.zip:
	@SK_ZIP@ -r VisualC.zip VisualC/ -x '*CVS/*'

SUBDIRS = . test

docdatadir = $(datadir)/doc/skstream-$(VERSION)
docdata_DATA = README COPYING AUTHORS NEWS
