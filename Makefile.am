pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = skstream-0.3.pc @SKSTREAM_UNIX_PC@

if HAVE_PROG_ZIP
MSVC_SUPPORT = VisualC.zip
MSVC_INSTALL_DIST = cp -f
else
MSVC_SUPPORT = 
MSVC_INSTALL_DIST = /bin/true
endif

EXTRA_DIST = autogen.sh skstream-0.3.pc.in @SKSTREAM_UNIX_PC_IN@

dist-hook: $(MSVC_SUPPORT)
	$(MSVC_INSTALL_DIST) $(MSVC_SUPPORT) $(distdir)/$(MSVC_SUPPORT)   

VisualC.zip:
	@SK_ZIP@ -r VisualC.zip VisualC/ -x '*CVS/*'

SUBDIRS = skstream ping test

docdatadir = $(datadir)/doc/skstream-$(VERSION)
docdata_DATA = README COPYING AUTHORS NEWS

docs:
	@echo "running doxygen..."
	@doxygen Doxyfile
	@echo "documentation is in doc/."
