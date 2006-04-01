dnl Initialise autoconf

AC_INIT(skstream, 0.3.6, alriddoch@zepler.org)
AC_CONFIG_SRCDIR(skstream/skstream.cpp)
AC_CONFIG_HEADERS([config.h])

dnl Detect the canonical host environment

AC_CANONICAL_HOST

dnl Initialise automake

AM_INIT_AUTOMAKE([dist-bzip2 check-news])

SKSTREAM_CURRENT=4
SKSTREAM_REVISION=0
SKSTREAM_AGE=0
SKSTREAM_VERSION_INFO=$SKSTREAM_CURRENT:$SKSTREAM_REVISION:$SKSTREAM_AGE
SKSTREAM_VERSION_NO=$SKSTREAM_CURRENT.$SKSTREAM_AGE.$SKSTREAM_REVISION

SKSTREAM_UNIX_PC=
SKSTREAM_UNIX_PC_IN=
SKSTREAM_LIB_SUFFIX="-0.3"
SKSTREAM_LIBS="-lskstream$SKSTREAM_LIB_SUFFIX"
SKSTREAM_EXTRA_LIBS=

AC_SUBST(SKSTREAM_CURRENT)
AC_SUBST(SKSTREAM_VERSION_INFO)
AC_SUBST(SKSTREAM_VERSION_NO)

dnl Test for GNU libtool

AC_DISABLE_STATIC
AC_PROG_LIBTOOL

dnl Test for C++ compiler

AC_PROG_CXX

AC_ARG_ENABLE(debug,
    [  --enable-debug          enable debug information [default=no]],
    [
        if test "$enableval" = "yes"; then
            CXXFLAGS="$CXXFLAGS -Wall -DDEBUG"
        else
            CXXFLAGS="$CXXFLAGS -Wall -DNDEBUG"
        fi
    ],[
        CXXFLAGS="$CXXFLAGS -Wall -DNDEBUG"
    ]
)

AC_ARG_ENABLE(getaddrinfo,
    [  --enable-getaddrinfo    enable getaddrinfo for hostname lookups [default=yes]],
    [
        if test "$enableval" = "yes"; then
            ac_enable_getaddrinfo="yes"
        else
            ac_enable_getaddrinfo="no"
        fi
    ],[
        ac_enable_getaddrinfo="yes"
    ]
)

dnl Test for ANSI standard language features

dnl Test for C++ bool keyword

AC_LANG(C++)

dnl Test for C++ Standard Library

AC_CHECK_HEADERS(cstdio iostream string)

dnl Test for mingw32

# There might be a better way to do this.
AC_MSG_CHECKING(for mingw32)
AC_TRY_COMPILE(,[
#ifdef __MINGW32__
#error MingW32 detected, adding -lws2_32 and -lwsock32 to LIBS
#endif
],
AC_MSG_RESULT(no),[
AC_MSG_RESULT(yes)
LIBS="$LIBS -lws2_32 -lwsock32"
SKSTREAM_EXTRA_LIBS="$SKSTREAM_EXTRA_LIBS -lws2_32 -lwsock32"
])

dnl Test for types

AC_MSG_CHECKING([for socket type])
SKSTREAM_SOCKET_TYPE=int;
SKSTREAM_SOCKET_HEADER="#include <unistd.h>"
AC_CACHE_VAL(ac_cv_socket_type,
[
    AC_TRY_COMPILE([#include <ws2tcpip.h>],
    [
        SOCKET foo;
        return foo; /* Avoids warning */
    ],
    [
        ac_cv_socket_type=SOCKET
        SKSTREAM_SOCKET_TYPE=$ac_cv_socket_type
        SKSTREAM_SOCKET_HEADER="#include <ws2tcpip.h>"
    ],
    [
        ac_cv_socket_type=int
        SKSTREAM_SOCKET_TYPE=$ac_cv_socket_type
        SKSTREAM_SOCKET_HEADER="#include <unistd.h>"
    ])
])
AC_MSG_RESULT($ac_cv_socket_type)
AC_SUBST(SKSTREAM_SOCKET_TYPE)
AC_SUBST(SKSTREAM_SOCKET_HEADER)

AC_MSG_CHECKING([for socket address length type])
SKSTREAM_SOCKLEN_TYPE=int;
AC_CACHE_VAL(ac_cv_socklen_type,
[
    AC_TRY_COMPILE([#include <unistd.h>
                    #include <sys/socket.h>],
    [
        socklen_t foo = 0;
        return foo != 0; /* Avoids warning */
    ],
    [
        ac_cv_socklen_type=socklen_t
        SKSTREAM_SOCKLEN_TYPE=$ac_cv_socklen_type
    ],
    [
        ac_cv_socklen_type=int
        SKSTREAM_SOCKLEN_TYPE=$ac_cv_socklen_type
    ])
])
AC_MSG_RESULT($ac_cv_socklen_type)
AC_SUBST(SKSTREAM_SOCKLEN_TYPE)

AC_MSG_CHECKING([for unix domain sockets])
AC_TRY_COMPILE([#include <sys/types.h>
                #include <sys/socket.h>
                #include <sys/un.h>],
[
    sockaddr_un sa;
    sa.sun_family = AF_UNIX;
],
[
    AC_MSG_RESULT(yes)
    AC_DEFINE(SKSTREAM_UNIX_SOCKETS, 1,
              [Define if unix domain sockets are supported])
    SKSTREAM_UNIX_PC=skstream-unix-0.3.pc
    SKSTREAM_UNIX_PC_IN=skstream-unix-0.3.pc.in
],
[
    AC_MSG_RESULT(no)
])

AC_SUBST(SKSTREAM_UNIX_PC)
AC_SUBST(SKSTREAM_UNIX_PC_IN)

AC_MSG_CHECKING([for timeval header])
SKSTREAM_TIMEVAL_HEADER="#include <sys/time.h>"
SKSTREAM_UNIX_HEADER_FLAG=0
SKSTREAM_BEOS_HEADER_FLAG=0
SKSTREAM_WINDOWS_HEADER_FLAG=0
AC_CACHE_VAL(ac_cv_timeval_header,
[
    AC_TRY_COMPILE([#include <winsock2.h>],
    [
        timeval foo;
        return &foo == 0; /* Avoids warning */
    ],
    [
        ac_cv_timeval_header="#include <winsock2.h>"
        SKSTREAM_TIMEVAL_HEADER=$ac_cv_timeval_header
        SKSTREAM_WINDOWS_HEADER_FLAG=1
    ],
    [
        AC_TRY_COMPILE([#include <sys/time.h>],
        [
            timeval foo;
            return &foo == 0; /* Avoids warning */
        ],
        [
            ac_cv_timeval_header="#include <sys/time.h>"
            SKSTREAM_TIMEVAL_HEADER=$ac_cv_timeval_header
            SKSTREAM_UNIX_HEADER_FLAG=1
        ],
        [
            AC_TRY_COMPILE([#include <net/socket.h>],
            [
                timeval foo;
                return &foo == 0; /* Avoids warning */
            ],
            [
                ac_cv_timeval_header="#include <net/socket.h>"
                SKSTREAM_TIMEVAL_HEADER=$ac_cv_timeval_header
                SKSTREAM_BEOS_HEADER_FLAG=1
            ],
            [
                AC_MSG_ERROR(Unable to find header for timeval type)
            ])
    
        ])
    ])
])
AC_MSG_RESULT($ac_cv_timeval_header)
AC_SUBST(SKSTREAM_TIMEVAL_HEADER)
AC_SUBST(SKSTREAM_UNIX_HEADER_FLAG)
AC_SUBST(SKSTREAM_BEOS_HEADER_FLAG)
AC_SUBST(SKSTREAM_WINDOWS_HEADER_FLAG)

dnl Test for socket block error value

AC_MSG_CHECKING([for socket block error value])
AC_TRY_COMPILE([#include <errno.h>],
[
    int i = EINPROGRESS;
    return i == 0; /* Avoids warning */
],
[
    SKSTREAM_BLOCK_ERROR_VAL=EINPROGRESS
],
[
    AC_TRY_COMPILE([#include <windows.h>],
    [
        int i = WSAEWOULDBLOCK;
        return i == 0; /* Avoids warning */
    ],
    [
        SKSTREAM_BLOCK_ERROR_VAL=WSAEWOULDBLOCK
    ],
    [
        AC_MSG_RESULT(failed)
        AC_MSG_ERROR(Unable to determine error message for socket blocking)
    ])
])
AC_MSG_RESULT($SKSTREAM_BLOCK_ERROR_VAL)
AC_SUBST(SKSTREAM_BLOCK_ERROR_VAL)

dnl Test for IPv6 socket addresses

AC_MSG_CHECKING([for IPv6 socket address])
AC_TRY_COMPILE([#include <sys/types.h>
                #include <sys/socket.h>
                #include <netinet/in.h>],
[
    sockaddr_in6 sin6;
    sin6.sin6_family = AF_INET6;
],
[
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_IPV6, 1, true if we have sockaddr_in6 and AF_INET6)
],
[
    AC_TRY_COMPILE([#include <ws2tcpip.h>],
    [
        sockaddr_in6 sin6;
        sin6.sin6_family = AF_INET6;
    ],
    [
        AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_IPV6, 1, true if we have sockaddr_in6 and AF_INET6)
    ],
    [
        AC_MSG_RESULT(no)
    ])
])

dnl Test for in_addr_t

AC_MSG_CHECKING([for in_addr_t])
AC_TRY_COMPILE([#include <sys/types.h>
                #include <sys/socket.h>
                #include <netinet/in.h>],
[
    in_addr_t ia;
    return &ia == 0; /* Avoids warnings */
],
[
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_IN_ADDR_T, 1, true if we have in_addr_t)
],
[
    AC_TRY_COMPILE([#include <ws2tcpip.h>],
    [
        in_addr_t ia;
        return &ia == 0; /* Avoids warnings */
    ],
    [
        AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_IN_ADDR_T, 1, true if we have in_addr_t)
    ],
    [
        AC_MSG_RESULT(no)
    ])
])

dnl Test for nsl library

AC_CHECK_FUNC(gethostbyname,,
[
    AC_MSG_CHECKING([for gethostbyname in ws2tcpip.h])
    AC_TRY_COMPILE(
    [
        #include <ws2tcpip.h>
    ],
    [
        ::gethostbyname("foo");
    ],
    [
        AC_MSG_RESULT(yes)
    ],
    [
        AC_MSG_RESULT(no)
        AC_CHECK_LIB(nsl, gethostbyname,
        [
            LIBS="$LIBS -lnsl"
            SKSTREAM_EXTRA_LIBS="$SKSTREAM_EXTRA_LIBS -lnsl"
        ],
        [
            AC_MSG_ERROR(Unable to find gethostbyname() socket call.)
        ])
    ])
])

dnl Test for socket library

AC_CHECK_FUNC(connect,,
[
    AC_MSG_CHECKING([for connect in ws2tcpip.h])
    AC_TRY_COMPILE(
    [
        #include <ws2tcpip.h>
    ],
    [
        ::connect(0, 0, 0);
    ],
    [
        AC_MSG_RESULT(yes)
    ],
    [
        AC_MSG_RESULT(no)
        AC_CHECK_LIB(socket, connect,
        [
            LIBS="$LIBS -lsocket"
            SKSTREAM_EXTRA_LIBS="$SKSTREAM_EXTRA_LIBS -lsocket"
        ],
        [
            AC_MSG_ERROR(Unable to find connect() socket call.)
        ])
    ])
])

dnl Test for closesocket

AC_MSG_CHECKING([for closesocket])
AC_TRY_COMPILE([#include <ws2tcpip.h>],
[
    closesocket(0);
],
[
    AC_DEFINE(HAVE_CLOSESOCKET, 1, "Define to 1 if you have the `closesocket' function.")
    AC_MSG_RESULT(yes)
],
[
    AC_MSG_RESULT(no)
])

dnl Test for getaddrinfo

if test "$ac_enable_getaddrinfo" = "yes"; then
    AC_CHECK_FUNC(getaddrinfo,
    [
        AC_DEFINE(HAVE_GETADDRINFO, 1, "Define to 1 if you have the `getaddrinfo' function.")
    ],
    [
        AC_MSG_CHECKING([for getaddrinfo in ws2tcpip.h])
        AC_TRY_COMPILE(
        [
            #include <ws2tcpip.h>
        ],
        [
            ::getaddrinfo(0, 0, 0, 0);
        ],
        [
            AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_GETADDRINFO, 1, "Define to 1 if you have the `getaddrinfo' function.")
        ],
        [
            AC_MSG_RESULT(no)
        ])
    ])
fi

dnl Test for C library functions

AC_CHECK_FUNCS([inet_aton])

dnl Test for Libraries

found_cppunit=yes
AM_PATH_CPPUNIT(1.6.1,, 
    AC_MSG_WARN(Couldn't Find CppUnit - Tests will not work)
    found_cppunit=no
)

AM_CONDITIONAL(USE_CPPUNIT, test "$found_cppunit" = "yes")

dnl test for support programs

AC_CHECK_PROG(SK_ZIP, zip, zip, no)
AM_CONDITIONAL(HAVE_PROG_ZIP, test x$ac_cv_prog_SK_ZIP = xzip)

AC_SUBST(SKSTREAM_LIB_SUFFIX)
AC_SUBST(SKSTREAM_LIBS)
AC_SUBST(SKSTREAM_EXTRA_LIBS)

AC_DEFINE(CONFIG_H_INCLUDED, 1, true if config.h was included)

dnl Generate files

AC_CONFIG_FILES([
	Makefile
	skstream/Makefile
	ping/Makefile
	test/Makefile
	skstream.spec
	skstream-0.3.pc
	skstream-unix-0.3.pc
	skstream/skstreamconfig.h
])
AC_OUTPUT
