lib_LTLIBRARIES = libskstream-0.3.la
libskstream_0_3_la_LDFLAGS = -version-info @SKSTREAM_VERSION_INFO@

libskstream_0_3_la_SOURCES = skstream.cpp skserver.cpp skpoll.cpp

libskstreamincludedir = $(includedir)/skstream-0.3/skstream
libskstreaminclude_HEADERS = skstream.h skstream_unix.h skserver.h \
                             skstreamconfig.h skpoll.h sasproto.h

libskstreamconfigincludedir = $(libdir)/skstream-0.3/include/skstream
libskstreamconfiginclude_HEADERS = skstreamconfig.h

EXTRA_DIST = skstreamconfig.h.in
