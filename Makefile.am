lib_LTLIBRARIES = libskstream.la

libskstream_la_SOURCES = skstream.cpp skserver.cpp

libskstreamincludedir = $(includedir)
libskstreaminclude_HEADERS = skstream.h skserver.h sasproto.h

m4datadir = $(datadir)/aclocal
m4data_DATA = skstream.m4

bin_SCRIPTS = skstream-config

EXTRA_DIST = skstream.m4 skstream-config.in autogen.sh

SUBDIRS = . test
