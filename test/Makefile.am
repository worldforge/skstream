TESTS = skstreamtestrunner
check_PROGRAMS = $(TESTS)

INCLUDES = -I$(top_srcdir) 

skstreamtestrunner_SOURCES = skstreamtestrunner.cpp

skstreamtestrunner_LDADD= \
    @CPPUNIT_LIBS@ \
    $(top_srcdir)/libskstream.la

