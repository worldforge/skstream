AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)

lib_LTLIBRARIES = libskstream-0.3.la
libskstream_0_3_la_LDFLAGS = -version-info @SKSTREAM_VERSION_INFO@

libskstream_0_3_la_SOURCES = sksocket.cpp skstream.cpp skserver.cpp skpoll.cpp

libskstreamincludedir = $(includedir)/skstream-0.3/skstream
libskstreaminclude_HEADERS = sksocket.h \
                             skstream.h skstream_unix.h \
                             skserver.h skserver_unix.h \
                             skpoll.h sasproto.h

libskstreamconfigincludedir = $(libdir)/skstream-0.3/include/skstream
libskstreamconfiginclude_HEADERS = skstreamconfig.h

EXTRA_DIST = skstreamconfig.h.in
