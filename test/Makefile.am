TESTS = skstreamtestrunner

INCLUDES = -I$(top_srcdir)

check_PROGRAMS = $(TESTS)

skstreamtestrunner_SOURCES = skstreamtestrunner.cpp \
        basicskstreamtest.h \
        childskstreamtest.h \
        skservertest.h \
        socketbuftest.h

skstreamtestrunner_LDADD= \
    @CPPUNIT_LIBS@ \
    -L$(top_builddir)/skstream -lskstream-0.2

