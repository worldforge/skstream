ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = skstream-0.3.pc @SKSTREAM_UNIX_PC@

if HAVE_PROG_ZIP
MSVC_SUPPORT = VisualC.zip
MSVC_INSTALL_DIST = cp -f
else
MSVC_SUPPORT = 
MSVC_INSTALL_DIST = /bin/true
endif

EXTRA_DIST = autogen.sh README.FreeSockets Doxyfile

dist-hook: $(MSVC_SUPPORT)
	$(MSVC_INSTALL_DIST) $(srcdir)/$(MSVC_SUPPORT) $(distdir)/$(MSVC_SUPPORT)   

VisualC.zip:
	@SK_ZIP@ -r $(srcdir)/VisualC.zip $(srcdir)/VisualC/ -x '*CVS/*'

SUBDIRS = skstream ping tools test

docs:
	@echo "running doxygen..."
	@doxygen Doxyfile
	@echo "documentation is in doc/."
