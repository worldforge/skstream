TESTS = skstreamtestrunner
check_PROGRAMS = $(TESTS)

INCLUDES = -I$(top_srcdir) 

skstreamtestrunner_SOURCES = skstreamtestrunner.cpp \
        basicskstreamtest.h \
        childskstreamtest.h \
        skservertest.h \
        socketbuftest.h

skstreamtestrunner_LDADD= \
    @CPPUNIT_LIBS@ \
    $(top_builddir)/libskstream-0.2.la

