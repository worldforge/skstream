INCLUDES = -I$(top_srcdir)

if USE_CPPUNIT
TESTPROGS = skstreamtestrunner
else
TESTPROGS = 
endif

TESTS = $(TESTPROGS)

check_PROGRAMS = $(TESTPROGS)

skstreamtestrunner_SOURCES = skstreamtestrunner.cpp \
        basicskstreamtest.h \
        childskstreamtest.h \
        skservertest.h \
        socketbuftest.h

skstreamtestrunner_LDADD= \
    @CPPUNIT_LIBS@ \
    $(top_builddir)/skstream/libskstream-0.3.la

