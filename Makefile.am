lib_LTLIBRARIES = libskstream.la
libskstream_la_LDFLAGS = -version-info @SKSTREAM_VERSION_INFO@

libskstream_la_SOURCES = skstream.cpp skserver.cpp

libskstreamincludedir = $(includedir)
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

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/doc/skstream-$(VERSION)
	$(INSTALL_DATA) README $(DESTDIR)$(datadir)/doc/skstream-$(VERSION)
	$(INSTALL_DATA) COPYING $(DESTDIR)$(datadir)/doc/skstream-$(VERSION)
	$(INSTALL_DATA) AUTHORS $(DESTDIR)$(datadir)/doc/skstream-$(VERSION)
	$(INSTALL_DATA) NEWS $(DESTDIR)$(datadir)/doc/skstream-$(VERSION)
