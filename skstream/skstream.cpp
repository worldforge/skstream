/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2000-2001 Rafael Guterres Jeffman

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

**************************************************************************/

/*
 * This software package has been modified by the Worldforge Project 
 * in the following ways:
 *
 * $Log$
 * Revision 1.47  2004-11-29 22:47:30  alriddoch
 * 2004-11-29  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.h, skstream/skstream.cpp: Inline operator!
 * 	  and timeout as they don't need to be virtual. Don't inline
 * 	  open() as its too big. Make setOutpeer return void, as the returned
 * 	  bool value was useless and ignored.
 *
 * Revision 1.46  2004/11/24 00:50:36  alriddoch
 * 2004-11-24  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.cpp, skstream/skstream.h, skstream/skstream_unix.h:
 * 	  Re-work the way stream buffer objects are created so they are
 * 	  allocated in the super class using new, and passed to the base
 * 	  where they are deleted in the destructor. This ensures that the
 * 	  stream buffer is never used un-initialised, or after destruction.
 * 	  Move some lengthy constructors into the .cpp file.
 *
 * 	* configure.ac: Increment version and interface so applications can
 * 	  ensure they get the updated API.
 *
 * Revision 1.45  2004/11/23 01:22:24  alriddoch
 * 2004-11-23  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skserver.cpp, skstream/skserver.h,
 * 	  skstream/skstream.cpp, skstream/skstream.h:
 * 	  Re-purpose the shutdown() method of various classes
 * 	  so it no longer closes the socket. This makes it more consistent
 * 	  and sorts out some issues with using epoll() with skstream.
 * 	  Make sure close() is called in the right places in destructors.
 * 	  Fix a bug in the Win32 build where WSACleanup() was getting
 * 	  called at utterly the wrong time.
 *
 * Revision 1.44  2003/09/26 22:26:44  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - Add option to get streams remote details as reverse lookup, rather
 *       than just presentation form.
 *
 * Revision 1.43  2003/09/26 14:38:43  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - Write some tests to pick up the socket and name resolver libs on
 *       System V.
 *     - Clean up handling of libs required to get socket calls in general.
 *     - ping/ping.cpp, skstream/skserver.cpp: Add missing stdio include.
 *     - skstream/skserver.h: Remove non-required string.h include.
 *     - skstream/skstream.cpp: Add mising stdio include, and clean up sprintf()
 *       usage.
 *
 * Revision 1.42  2003/09/26 10:49:03  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp: Add code that handles converting service to
 *       presentation format in a protocol independant way.
 *
 * Revision 1.41  2003/09/26 01:29:41  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h: Add accessors for address size members.
 *     - ping/ping.cpp: Clean up use of libc and system calls.
 *     - skstream/skstream.cpp: Clean up handling socket address sizes
 *       when setting socket by descriptor in the buffer objects.
 *       Add code that handles converting address to presentation format
 *       in a protocol independant way.
 *
 * Revision 1.40  2003/09/25 17:05:49  alriddoch
 *  2003-09-25 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skpoll.cpp, skstream/skserver.cpp, skstream/skstream.cpp:
 *       Finish off fully qualifying all libc and system calls as being
 *       explicitly in the global namespace.
 *
 * Revision 1.39  2003/09/25 16:39:50  alriddoch
 *  2003-09-25 Al Riddoch <alriddoch@zepler.org>
 *     - Remove messy attempt to handle IPv6 addresses without getaddrinfo
 *
 * Revision 1.38  2003/09/25 01:17:10  alriddoch
 *  2003-09-25 James Turner <james@worldforge.org>
 *     - skstream/skstream.cpp: Fully qualify all libc and system calls as
 *       being explicitly in the global namespace.
 *
 * Revision 1.37  2003/09/24 21:02:11  alriddoch
 *  2003-09-24 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp: Implement non-blocking connect with addrinfo list.
 *
 * Revision 1.36  2003/09/24 16:20:03  alriddoch
 *  2003-09-24 Al Riddoch <alriddoch@zepler.org>
 *     - Typo in in_addr_t check fixed.
 *     - skstream/skstream.cpp: Missed out typedef keyword fix.
 *
 * Revision 1.35  2003/09/24 14:47:06  alriddoch
 *  2003-09-24 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp: Add check, and fallback definition for
 *       in_addr_t.
 *
 * Revision 1.34  2003/09/24 01:05:46  alriddoch
 *  2003-09-24 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp: Re-write connecting
 *       code for IP datagram and stream sockets so they try each of the
 *       results from getaddrinfo in turn. Needs work on non-blocking
 *       sockets.
 *
 * Revision 1.33  2003/08/25 17:18:27  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp: Add extra argument
 *       to dgram_streambuf::setTarget() so protocol can be passed in.
 *       Add #warnings if the old non getaddrinfo code is being used.
 *
 * Revision 1.32  2003/08/23 22:13:55  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstreamconfig.h.in, skstream/skstream_unix.h,
 *       skstream/skstream.h, skstream/skstream.cpp,
 *       skstream/skserver_unix.h, skstream/skserver.h, skstream/skserver.cpp:
 *       Remove as much platform sensitive code from the headers as possible,
 *       and simplify the generated header. Improve formatting and header
 *       guards for readability.
 *     - skstream/skserver_unix.h: Add a new constructor for unix_socket_server
 *       so it can be instanced without already knowing the details of the
 *       service.
 *
 * Revision 1.31  2003/08/23 20:19:46  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp: Modify udp_socket_stream
 *       constructor so that it does not create the socket immediatly.
 *     - skstream/skstream.cpp: Re-write dgram_socketbuf::setTarget() so it
 *       uses getaddrinfo to create the socket, and get the address in
 *       a protocol independant way.
 *
 * Revision 1.30  2003/08/23 01:14:44  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp: Use getaddrinfo to handle the port number.
 *
 * Revision 1.29  2003/08/23 00:37:38  alriddoch
 * *** empty log message ***
 *
 * Revision 1.28  2003/08/08 23:56:26  alriddoch
 *  2003-08-08 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp, skstream/skstream_unix.h: Include skstream
 *       header with its fully qualified name for compatability, and move
 *       the unix system header out of the header into the cpp file.
 *     - skstream/skserver.cpp, skstream/skserver.h: Add new base class for ip
 *       socket types, and put in the ip address related functionality into it,
 *       so basic class can be used as a base for unix socket class.  Move
 *       can_accept() into the base class as it applies to any listen socket.
 *     - skstream/skserver_unix.h, skstream/skserver.cpp: Add a class for
 *       unix listen sockets.
 *
 * Revision 1.27  2003/07/30 23:17:55  alriddoch
 *  2003-07-30 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.cpp, skstream/skserver.h, skstream/skstream.cpp,
 *       skstream/skstream.h, skstream/skstream_unix.h: Move virtual
 *       function implementations into .cpp files.
 *
 * Revision 1.26  2003/07/23 17:00:29  alriddoch
 *  2003-07-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstreamconfig.h.pbx, skstream/skstreamconfig.h.in,
 *       skstream/skstreamconfig.h.windows: Removed some defines which
 *       are the same on all platforms, and put them in the main header.
 *       Avoid streambuf header as it varies on some systems, and rely
 *       on iostream instead.
 *     - Remove the streambuf header test.
 *     - skstream/skstream.h: Remove the old platform specific code,
 *       take on some changes from the skstreamconfig header, and clean
 *       up some comments.
 *     - skstream/skstream.cpp: Re-write the resolver code for platforms
 *       without getaddrinfo() so it is cleaner, and covers more cases.
 *
 * Revision 1.25  2003/05/06 21:53:11  alriddoch
 *  2003-05-06 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp, skstream_unix.h:
 *       Re-work basic_socket_stream so it can have either stream or datagram
 *       buffers.
 *     - ping/ping.cpp, ping/ping.h, test/basicskstreamtest.h,
 *       test/childskstreamtest.h, test/skservertest.h: Get the tests and examples
 *       building again.
 *
 * Revision 1.24  2003/05/04 01:14:07  alriddoch
 *  2003-05-04 Al Riddoch <alriddoch@zepler.org>,
 *     - skstream/skstream.cpp: Add a check to detect IPv6 sockaddr in
 *       headers, and treat address explicitly as an IPv6 address if
 *       it is.
 *
 * Revision 1.23  2003/05/04 00:34:30  alriddoch
 *  2003-05-04 Al Riddoch <alriddoch@zepler.org>,
 *     - Add a second pkgconfig file for apps that need unix socket support.
 *     - Rename sksystem.h as skstreamconfig.h, and put it in an architecture
 *       dependent subdir. Modify pkgconfig files to handle this.
 *     - Handle errors in a more general way to reduce the number
 *       of places where pre-processor conditionals are required.
 *     - Remove some redundant code for dealing with win32 that has
 *       now been superceded.
 *
 * Revision 1.22  2003/04/22 15:14:59  alriddoch
 *  2003-04-22 Al Riddoch <alriddoch@zepler.org>,
 *     - Add configure checks and support for protocol family independant
 *       socket address functions, so we can support IPv6 as well as IPv4.
 *
 * Revision 1.21  2003/04/16 14:02:35  alriddoch
 *  2003-04-16 Al Riddoch <alriddoch@zepler.org>,
 *     - Fix up includes so they work properlly
 *
 * Revision 1.20  2003/03/18 22:46:45  alriddoch
 *  2003-03-18 Al Riddoch <alriddoch@zepler.org>,
 *     - Improve safety of some constructors, and move IP (host and port)
 *       related functions into apropriate classes.
 *     - Add in unix sockets, based on a configure check.
 *
 * Revision 1.19  2003/03/16 12:27:40  alriddoch
 *  2003-03-16 Al Riddoch <alriddoch@zepler.org>,
 *     - skstream.cpp: Fix problem with virtual method being called
 *       after destructor.
 *
 * Revision 1.18  2003/03/14 19:33:11  alriddoch
 *  2003-03-14 Al Riddoch <alriddoch@zepler.org>,
 *     - skstream.h, skstream.cpp: Re-work sockbuf class so it is
 *       not specific to any one type of socket. Remove inet
 *       functionality from base classes. Derive new buffer
 *       classes for stream and datagram sockets, and use
 *       stream version in current socket classes. This should
 *       be considered work in progress. Do not port your code.
 *
 * Revision 1.17  2002/12/09 22:13:21  rsteinke
 *     - created basic_socket, a virtual base class
 *       for basic_socket_stream and basic_socket_server,
 *       so that the polling code has a common base
 *       with getSocket() to access
 *     - created basic_socket_poll polling class
 *     - changed tcp_socket_stream::is_ready() to isReady()
 *       to match standards for function naming, left is_ready()
 *       as a wrapper of isReady() for compatibility
 *
 * Revision 1.16  2002/11/04 22:49:15  alriddoch
 *  2002-11-04 Al Riddoch <alriddoch@zepler.org>,
 *     - Add doxygen docs.
 *
 * Revision 1.15  2002/10/14 05:05:45  malcolm
 * Added a Sleep(0) call during check for getsockopt() that appears to be needed
 * to circumvent a bug during nonblocking connect.  See MS KB Q165989.
 *
 * Revision 1.14  2002/07/22 19:33:48  rsteinke
 *     - changed sizeof(sockaddr) to sizeof(sockaddr_in) in
 *       several locations, so that we pass the correct size
 *       of in_peer and out_peer in socketbuf
 *     - reordered initializers in socketbuf constructors
 *       to match their declaration order in the class
 *
 * Revision 1.13  2002/07/16 21:10:06  malcolm
 * Had to add another tiny tweak for win32 nonblocking stuff.  ioctlsocket()
 * explicitly requires a pointer to an unsigned long, not just a 0 (NULL),
 * for the third argument, in windows.  Sheesh.
 *
 * Revision 1.12  2002/07/16 18:14:25  grimicus
 * 2002-07-16 Dan Tomalesky <grim@xynesis.com>
 *     * skstream.cpp: added a small fix to a bug with is_ready.  wasn't
 *       compiling on linux and seems code was wrong in the select because it was
 *       doing |= to the select() instead of != (i.e., if it doesn't return 1,
 *       there is no ready socket or an error)
 *
 * Revision 1.11  2002/07/16 01:12:19  malcolm
 * Fixed call for win32 cases with is_ready() - it was failing before with
 * WSAEFAULT - bad pointer.  Works now.  Tested with uclient; all is good.
 *
 * Revision 1.10  2002/07/15 21:21:17  alriddoch
 *  07/15/2002 Al Riddoch <alriddoch@zepler.org>,
 *     - skstream.cpp: Handle nonblocking connect in win32
 *     - skstream.h: Include correct streambuf header on Linux.
 *
 * Revision 1.9  2002/07/04 10:23:56  jmt
 *  07/03/2002 James Turner <james@worldforge.org>
 * 	-Added configuration case for Darwin / OS-X
 * 	-Fixed Rsteinke's changes to use typedefs for socklen / errnum
 *
 * Revision 1.8  2002/06/12 01:21:47  rsteinke
 *     -Added an optional "milliseconds" argument to
 *      tcp_socket_stream::is_ready(), to take advantage
 *      of the timeout in select()
 *
 * Revision 1.7  2002/06/12 00:27:40  rsteinke
 *     -Fixed many bugs in nonblocking tcp socket connect code
 *
 * Revision 1.6  2002/05/21 07:29:36  malcolm
 * Added rsteinke's nonblocking connect patch.  Works on linux; does not break API
 * (I bumped version to 0.2.3 anyway).  May not work on win32, though I did test it
 * and socket communication does happen.
 *
 * Revision 1.5  2002/04/08 20:02:00  xmp
 * Just a few fixes to MSVC support, removing a few unnessesary std::'s and shifting a default argument to a header.
 *
 * Revision 1.4  2002/04/08 19:47:12  malcolm
 * Changes to allow clean compilation on Microsoft Visual C++.  I must go and
 * boil my keyboard now.  Constructor code for basic_socket_stream moved out of
 * header and into .cpp.
 *
 * MS Project files are untested (I edited them!  Oh no!)
 *
 * Revision 1.3  2002/02/26 20:33:55  grimicus
 * 02/26/2002 Dan Tomalesky <grim@xynesis.com>
 *     * Added test cases for skserver and friends
 *
 *     * Adding .cvsignore files so that it doesn't mess with non-cvs files anymore
 *
 *     * In skserver.cpp and skstream.cpp, in the close() methods, I commented out
 *       the return; in the error part of the shutdown() call because it is
 *       possible that a socket can exist without it actually having been used,
 *       so this could error out on those cases and the socket is never actually
 *       closed.
 *
 *     * In skserver.h, added can_accept() to tcp_socket_server so that it can be
 *       checked to see if the server socket has any connections available, so that
 *       accept() can then be called. (if it returns false, if accept is called,
 *       it will block until a connection is made)
 *
 *     * Removed the #include <iostream> from skserver.h and skstream.h as they are
 *       not actually needed for any of the code. (else it comes in from some other
 *       include, I'm not positive)
 *
 *     * Made some formatting changes in skserver.h along the same lines as I have
 *       been doing throughout the code.
 *
 *     * Added testClose() to basicskstreamtest.
 *
 *     * Changed the socket created in basicskstreamtest from SOCK_STREAM to
 *       SOCK_DGRAM though it doesn't make any difference what so ever in the
 *       testing.
 *
 *     * Added the skservertests into the test runner.
 *
 * Revision 1.2  2002/02/24 03:15:41  grimicus
 * 02/23/2002 Dan Tomalesky <grim@xynesis.com>
 *
 *     * Added in CVS logging variable so that changes show up in modified files
 *       This will help keep changes made by worldforge in each file that is
 *       changed as required by the GPL.
 *
 *     * Changed some parameter variables to have better meaning.
 *       (ad -> address, etc.)
 *
 *     * Added some code into tcp_sk_stream::open so that it calls setLastError()
 *       when the connection fails.
 *
 *     * Added some comments into skstream.h to better describe SOCKET_TYPE as
 *       there can be some confusion between what it is actually for
 *       (pointer/file descriptor/windows cludge of the socket) and the various
 *       types of sockets (tcp, udp, raw, etc)
 *
 *     * Changed some more formatting for readability.
 *
 *     * Uncommented some commented out code in skstream.h so that the sync()
 *       method returns 0 on the else, rather than not returning anything.
 *
 *     * Added some code into setBroadcast() so that setLastError() is called
 *       if it fails to perform the getsocketopt().
 *
 *     * Modified the test/Makefile.am to remove the header files from the SOURCES
 *       as the .h files do not seem to affect the build.
 *
 *     * Updated all the current test so that they use a socket instead of the
 *       absolutely wrong stuff I was doing before.
 *
 *     * Added tests for tcp, udp, and raw skstreams child classes.
 *
 * Revision 1.1  2002/01/07 23:02:08  rykard
 * Adding the new version of skstream/FreeSockets to cvs.  
 * Note there are some API changes and new features in this version, so I 
 * didn't just commit over the older one.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <skstream/skstream.h>

#ifndef _WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#endif // _WIN32

#include <cstdio>
#include <cassert>

static inline int getSystemError()
{
  #ifdef _WIN32
    return WSAGetLastError();
  #else
    return errno;
  #endif
}

#ifndef HAVE_CLOSESOCKET
static inline int closesocket(SOCKET_TYPE sock)
{
    return ::close(sock);
}
#endif // HAVE_CLOSESOCKET

#ifndef HAVE_IN_ADDR_T
// This may cause problems on L64 systems, if they don't have in_addr_t
// but only windows does not have in_addr_t to my knowledge.
// AJR 2003-09-24
typedef unsigned long in_addr_t;
#endif // HAVE_IN_ADDR_T

/////////////////////////////////////////////////////////////////////////////
// class socketbuf implementation
/////////////////////////////////////////////////////////////////////////////
// Constructor
socketbuf::socketbuf(SOCKET_TYPE sock, unsigned insize, unsigned outsize)
    : std::streambuf(), _socket(sock),
      out_p_size(sizeof(out_peer)), in_p_size(sizeof(in_peer)),
      Timeout(false)
{
  _buffer = NULL;
  // allocate 16k buffer each for input and output
  const int bufsize = insize+outsize;
  // Allocate a new buffer
  char* buffer = new char[bufsize];
  ::memset(buffer,0,bufsize);
  // Setup the buffer
  if(this != setbuf(buffer, bufsize)) {
    _buffer = NULL;
  }
  _timeout.tv_sec  = 0;
  _timeout.tv_usec = 0;

  ::getpeername(sock,(sockaddr*)&out_peer, &out_p_size);
  in_peer = out_peer;
  in_p_size = out_p_size;
}

// Constructor
socketbuf::socketbuf(SOCKET_TYPE sock, char* buf, int length)
    : std::streambuf(), _socket(sock),
      out_p_size(sizeof(out_peer)), in_p_size(sizeof(in_peer)),
      Timeout(false)
{
  _buffer = NULL;
  if(this != setbuf(buf,length)) {
    _buffer = NULL;
  }
  _timeout.tv_sec  = 0;
  _timeout.tv_usec = 0;

  ::getpeername(sock,(sockaddr*)&out_peer, &out_p_size);
  in_peer = out_peer;
  in_p_size = out_p_size;
}

// Destructor
socketbuf::~socketbuf(){
  delete [] _buffer;
  _buffer = NULL;
}

void socketbuf::setSocket(SOCKET_TYPE sock)
{
  _socket = sock;
  out_p_size = sizeof(sockaddr);
  ::getpeername(sock, (sockaddr*)&out_peer, &out_p_size);
  in_peer = out_peer;
  in_p_size = out_p_size;
}


int socketbuf::sync()
{
    if(overflow() == EOF) // traits::eof()
      return EOF;	// ios will set the fail bit // traits::eof()
    else {
// This appeared to be causing the stream to lose data.
//      // empty put and get areas
//      setp(pbase(), epptr());
//      setg(eback(), egptr(), egptr());
//
      return 0; // traits::not_eof(0);
    }
}

std::streambuf* socketbuf::setbuf(char* buf, long len)
{
    if((buf != NULL) && (len > 0)) {
      _buffer = buf;
      setp(_buffer, _buffer+(len >> 1));
      setg(_buffer+(len >> 1), _buffer+len, _buffer+len);
    }
    return this;
}

stream_socketbuf::stream_socketbuf(SOCKET_TYPE sock,
                                   unsigned insize,
                                   unsigned outsize)
    : socketbuf(sock, insize, outsize) { }

stream_socketbuf::stream_socketbuf(SOCKET_TYPE sock, char* buf, int length)
    : socketbuf(sock, buf, length) { }

stream_socketbuf::~stream_socketbuf()
{
  sync();
}

dgram_socketbuf::dgram_socketbuf(SOCKET_TYPE sock,
                                 unsigned insize,
                                 unsigned outsize)
    : socketbuf(sock, insize, outsize) { }

dgram_socketbuf::dgram_socketbuf(SOCKET_TYPE sock, char* buf, int length)
    : socketbuf(sock, buf, length) { }

dgram_socketbuf::~dgram_socketbuf()
{
  sync();
}

// The next function are those who do the dirt work

// overflow() - handles output to a connected socket.
int stream_socketbuf::overflow(int nCh) { // traits::eof()
  // in case of error, user finds out by testing fail()
  if(_socket==INVALID_SOCKET)
    return EOF; // Invalid socket // traits::eof()

  if(pptr()-pbase() <= 0)
    return 0; // nothing to send

  int size;

  // prepare structure for detecting timeout
  #ifndef __BEOS__
    // if a timeout was specified, wait for it.
    {
      timeval _tv = _timeout;
      if((_tv.tv_sec+_tv.tv_usec) > 0) {
        int sr;
        fd_set socks;
        FD_ZERO(&socks); // zero fd_set
        FD_SET(_socket,&socks); // add buffer socket to fd_set
        sr = ::select(_socket+1,NULL,&socks,NULL,&_tv);
        if(/*(sr == 0) || */ !FD_ISSET(_socket,&socks)){
          Timeout = true;
          return EOF; // a timeout error should be set here! - RGJ
         } else if(sr < 0) return EOF; // error on select() // traits::eof()
      }
      Timeout = false;
    }
  #endif

  // send pending data or return EOF on error
  size=::send(_socket, pbase(),pptr()-pbase(),0);

  if(size < 0) {
    return EOF; // Socket Could not send // traits::eof()
  }

  if(size == 0)
    return EOF; // remote site has closed this connection // traits::eof()

  if(nCh != EOF) { // traits::eof()
    // size >= 1 at this point
    size--;
    *(pbase()+size)=nCh;
  }

  // move remaining pbase()+size .. pptr()-1 => pbase() .. pptr()-size-1
  for(char *p=pbase()+size; p<pptr(); p++)
    *(p-size)=*p;

  const int newlen=(pptr()-pbase())-size;

  setp(pbase(),epptr());

  pbump(newlen);

  return 0; // traits::not_eof(ch)
}

// underflow() - handles input from a connected socket.
int stream_socketbuf::underflow()
{
  if(_socket == INVALID_SOCKET)
    return EOF; // Invalid socket! // traits::eof()

  if((gptr()) && (egptr()-gptr() > 0))
    return (int)(unsigned char)(*gptr());

  // fill up from eback to egptr
  int size;

  // prepare structure for detecting timeout
  #ifndef _BEOS
    // if a timeout was specified, wait for it.
    if((_timeout.tv_sec+_timeout.tv_usec) > 0) {
      int sr;
      timeval _tv = _timeout;
      fd_set socks;
      FD_ZERO(&socks); // zero fd_set
      FD_SET(_socket,&socks); // add buffer socket to fd_set
      sr = ::select(_socket+1,&socks,NULL,NULL,&_tv);
      if((sr == 0) || !FD_ISSET(_socket,&socks)){
        Timeout = true;
        return EOF; // a timeout error should be set here! - RGJ
      } else if(sr < 0) return EOF;  // error on select()
    }
    Timeout = false;
  #endif

  // receive data or return EOF on error
  size = ::recv(_socket, eback(), egptr()-eback(), 0);

  if(size <= 0)
    return EOF; // remote site has closed connection or (TCP) Receive error
     // traits::eof()

  // move receivd data from eback() .. eback()+size to egptr()-size .. egptr()
  const int delta = egptr()-(eback()+size);
  for(char *p=eback()+size-1; p >= eback(); p--)
    *(p+delta) = *p;

  setg(eback(), egptr()-size, egptr());

  return (int)(unsigned char)(*gptr()); // traits::not_eof(...)
}

// setTarget() - set the target socket address
bool dgram_socketbuf::setTarget(const std::string& address, unsigned port,
                                int protocol)
{
  if (_socket != INVALID_SOCKET) {
    ::closesocket(_socket);
    _socket = INVALID_SOCKET;
  }

#ifdef HAVE_GETADDRINFO
  struct addrinfo req, *ans;
  char portName[32];

  ::sprintf(portName, "%d", port);

  req.ai_flags = 0;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = SOCK_DGRAM;
  req.ai_protocol = 0;
  req.ai_addrlen = 0;
  req.ai_addr = 0;
  req.ai_canonname = 0;
  req.ai_next = 0;

  int ret;
  if ((ret = ::getaddrinfo(address.c_str(), portName, &req, &ans)) != 0) {
    return false;
  }

  bool success = false;

  for(struct addrinfo * i = ans; success == false && i != 0; i = i->ai_next) {

    _socket = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);

    if (_socket != INVALID_SOCKET) {
      ::memcpy(&out_peer, i->ai_addr, i->ai_addrlen);
      out_p_size = i->ai_addrlen;
      success = true;
    }

  }

  ::freeaddrinfo(ans);

  return success;
#else // HAVE_GETADDRINFO

#warning Using deprecated resolver code because getaddrinfo() is not available

  hostent * he = ::gethostbyname(address.c_str());
  if (he != 0) {
    // If gethostbyname succeeded, we now have an address
    out_peer.ss_family = he->h_addrtype;
    ::memcpy(&((sockaddr_in&)out_peer).sin_addr, he->h_addr_list[0],
                                                 he->h_length);
    out_p_size = sizeof(sockaddr_in);
    ((sockaddr_in &)out_peer).sin_port = htons(port);
  } else {
    // If gethostbyname failed then our last chance is to assume
    // the string contains an IPv4 address in dotted quad form.
    // inet_aton is the cleaner way of reading this, but may
    // not be present on all systems.

    sockaddr_in & out_peer_in = (sockaddr_in &)out_peer;
 #ifdef HAVE_INET_ATON
    struct in_addr in_address;
    if (::inet_aton(address.c_str(), &in_address) == 0) {
      return false;
    }
    ::memcpy(&out_peer_in.sin_addr, &in_address, sizeof(in_address));
 #else // HAVE_INET_ATON
    in_addr_t iaddr;
    iaddr = ::inet_addr(address.c_str());
    if (iaddr == INADDR_NONE) {
      return false;
    }
    out_peer_in.sin_addr.s_addr = iaddr;
 #endif // HAVE_INET_ATON
    out_peer_in.sin_family = AF_INET;
    out_peer_in.sin_port = htons(port);
    out_p_size = sizeof(sockaddr_in);
  }

  SOCKET_TYPE newSock = ::socket(out_peer.ss_family, SOCK_DGRAM, protocol);
  if(newSock == INVALID_SOCKET) {
    return false;
  }
  _socket = newSock;

  return true;
#endif // HAVE_GETADDRINFO
}

// overflow() - handles output to a connected socket.
int dgram_socketbuf::overflow(int nCh) { // traits::eof()
  // in case of error, user finds out by testing fail()
  if(_socket==INVALID_SOCKET)
    return EOF; // Invalid socket // traits::eof()

  if(pptr()-pbase() <= 0)
    return 0; // nothing to send

  int size;

  // prepare structure for detecting timeout
  #ifndef __BEOS__
    // if a timeout was specified, wait for it.
    {
      timeval _tv = _timeout;
      if((_tv.tv_sec+_tv.tv_usec) > 0) {
        int sr;
        fd_set socks;
        FD_ZERO(&socks); // zero fd_set
        FD_SET(_socket,&socks); // add buffer socket to fd_set
        sr = ::select(_socket+1,NULL,&socks,NULL,&_tv);
        if(/*(sr == 0) || */ !FD_ISSET(_socket,&socks)){
          Timeout = true;
          return EOF; // a timeout error should be set here! - RGJ
         } else if(sr < 0) return EOF; // error on select() // traits::eof()
      }
      Timeout = false;
    }
  #endif

  // send pending data or return EOF on error
  size=::sendto(_socket, pbase(),pptr()-pbase(),0,(sockaddr*)&out_peer,out_p_size);

  if(size < 0) {
    return EOF; // Socket Could not send // traits::eof()
  }

  if(size == 0)
    return EOF; // remote site has closed this connection // traits::eof()

  if(nCh != EOF) { // traits::eof()
    // size >= 1 at this point
    size--;
    *(pbase()+size)=nCh;
  }

  // move remaining pbase()+size .. pptr()-1 => pbase() .. pptr()-size-1
  for(char *p=pbase()+size; p<pptr(); p++)
    *(p-size)=*p;

  const int newlen=(pptr()-pbase())-size;

  setp(pbase(),epptr());

  pbump(newlen);

  return 0; // traits::not_eof(ch)
}

// underflow() - handles input from a connected socket.
int dgram_socketbuf::underflow() {
  if(_socket == INVALID_SOCKET)
    return EOF; // Invalid socket! // traits::eof()

  if((gptr()) && (egptr()-gptr() > 0))
    return (int)(unsigned char)(*gptr());

  // fill up from eback to egptr
  int size;

  // prepare structure for detecting timeout
  #ifndef _BEOS
    // if a timeout was specified, wait for it.
    if((_timeout.tv_sec+_timeout.tv_usec) > 0) {
      int sr;
      timeval _tv = _timeout;
      fd_set socks;
      FD_ZERO(&socks); // zero fd_set
      FD_SET(_socket,&socks); // add buffer socket to fd_set
      sr = ::select(_socket+1,&socks,NULL,NULL,&_tv);
      if((sr == 0) || !FD_ISSET(_socket,&socks)){
        Timeout = true;
        return EOF; // a timeout error should be set here! - RGJ
      } else if(sr < 0) return EOF;  // error on select()
    }
    Timeout = false;
  #endif

  // receive data or return EOF on error
  in_p_size = sizeof(in_peer);
  size = ::recvfrom(_socket, eback(), egptr()-eback(), 0,
                    (sockaddr*)&in_peer, &in_p_size);

  if(size <= 0)
    return EOF; // remote site has closed connection or (TCP) Receive error
     // traits::eof()

  // move receivd data from eback() .. eback()+size to egptr()-size .. egptr()
  const int delta = egptr()-(eback()+size);
  for(char *p=eback()+size-1; p >= eback(); p--)
    *(p+delta) = *p;

  setg(eback(), egptr()-size, egptr());

  return (int)(unsigned char)(*gptr()); // traits::not_eof(...)
}

/////////////////////////////////////////////////////////////////////////////
// class basic_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

// Constructor

basic_socket_stream::basic_socket_stream(socketbuf & buffer,
                                                 int proto)
    : std::iostream(&_sockbuf), _sockbuf(buffer),
      protocol(proto), LastError(0)
{
  startup();
  init(&_sockbuf); // initialize underlying streambuf
}

basic_socket_stream::~basic_socket_stream()
{
  if(is_open()) {
    ::shutdown(_sockbuf.getSocket(), SHUT_RDWR);
    ::closesocket(_sockbuf.getSocket());
  }
  delete &_sockbuf;
}

SOCKET_TYPE basic_socket_stream::getSocket() const
{
  return _sockbuf.getSocket();
}

// System dependant initialization
bool basic_socket_stream::startup() {
#ifdef _WIN32
  const unsigned wMinVer = 0x0101;	// request WinSock v1.1 (at least)
  WSADATA wsaData;
  LastError = WSAStartup(wMinVer, &wsaData);
  return (LastError == 0);
#else // _WIN32
  return true;
#endif // _WIN32
}

// System dependant finalization
void basic_socket_stream::shutdown() {
  if(is_open()) {
    if(::shutdown(_sockbuf.getSocket(), SHUT_RDWR) == SOCKET_ERROR) {
      setLastError();
    }
  }
}

// private function that sets the internal variable LastError
void basic_socket_stream::setLastError() const {
    LastError = getSystemError();
}

// closes a socket connection
void basic_socket_stream::close() {
  if(is_open()) {
    if(::shutdown(_sockbuf.getSocket(), SHUT_RDWR) == SOCKET_ERROR) {
      setLastError();
      //not necessarily a returning offense because there could be a socket
      //open that has never connected to anything and hence, does not need
      //to be shutdown.
    }

    if(::closesocket(_sockbuf.getSocket()) == SOCKET_ERROR) {
      setLastError();
      return;
    }
    _sockbuf.setSocket(INVALID_SOCKET);
  }
}

// Check for failure condition
bool basic_socket_stream::fail() {
  if(timeout()) {
    clear();
    return false;
  }
  if(std::iostream::fail()) {
    setLastError();
    return true;
  }
  return false;
}

bool basic_socket_stream::setBroadcast(bool opt)
{
  int ok = opt?1:0;
  ok = ::setsockopt(_sockbuf.getSocket(),
                  SOL_SOCKET,SO_BROADCAST,(char*)&ok,sizeof(ok));
  bool ret = (ok != SOCKET_ERROR);
  if(!ret) setLastError();
  return ret;
}

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

tcp_socket_stream::tcp_socket_stream() :
                  basic_socket_stream(*new stream_socketbuf(INVALID_SOCKET)),
                  _connecting_socket(INVALID_SOCKET),
                  _connecting_address(0),
                  _connecting_addrlist(0),
                  stream_sockbuf((stream_socketbuf&)_sockbuf) {
  protocol = FreeSockets::proto_TCP;
}

tcp_socket_stream::tcp_socket_stream(SOCKET_TYPE socket) :
                  basic_socket_stream(*new stream_socketbuf(socket)),
                  _connecting_socket(INVALID_SOCKET),
                  _connecting_address(0),
                  _connecting_addrlist(0),
                  stream_sockbuf((stream_socketbuf&)_sockbuf) {
  protocol = FreeSockets::proto_TCP;
}

tcp_socket_stream::tcp_socket_stream(const std::string& address, int service,
                                     bool nonblock) :
                  basic_socket_stream(*new stream_socketbuf(INVALID_SOCKET)),
                  _connecting_socket(INVALID_SOCKET),
                  _connecting_address(0),
                  _connecting_addrlist(0),
                  stream_sockbuf((stream_socketbuf&)_sockbuf) {
  protocol = FreeSockets::proto_TCP;
  open(address, service, nonblock);
}

tcp_socket_stream::tcp_socket_stream(const std::string& address, int service,
                                     unsigned int milliseconds) :
                  basic_socket_stream(*new stream_socketbuf(INVALID_SOCKET)),
                  _connecting_socket(INVALID_SOCKET),
                  _connecting_address(0),
                  _connecting_addrlist(0),
                  stream_sockbuf((stream_socketbuf&)_sockbuf) {
  protocol = FreeSockets::proto_TCP;
  open(address, service, milliseconds);
}

tcp_socket_stream::~tcp_socket_stream() { 
  if(_connecting_socket != INVALID_SOCKET) {
    ::shutdown(_connecting_socket, SHUT_RDWR);
    ::closesocket(_connecting_socket);
#ifdef HAVE_GETADDRINFO
    ::freeaddrinfo(_connecting_addrlist);
#endif // HAVE_GETADDRINFO
  }
}

void tcp_socket_stream::open(const std::string & address,
                             int service, bool nonblock)
{
  if (is_open() || _connecting_socket != INVALID_SOCKET) {
    close();
  }

#ifdef HAVE_GETADDRINFO
  if (_connecting_addrlist != 0) {
    ::freeaddrinfo(_connecting_addrlist);
    _connecting_addrlist = 0;
  }

  struct addrinfo req, *ans;
  char serviceName[32];

  ::sprintf(serviceName, "%d", service);

  req.ai_flags = 0;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = SOCK_STREAM;
  req.ai_protocol = 0;
  req.ai_addrlen = 0;
  req.ai_addr = 0;
  req.ai_canonname = 0;
  req.ai_next = 0;

  int ret;
  if ((ret = ::getaddrinfo(address.c_str(), serviceName, &req, &ans)) != 0) {
    fail();
    return;
  }

  bool success = false;
  SOCKET_TYPE _socket = INVALID_SOCKET;

  for(struct addrinfo * i = ans; success == false && i != 0; i = i->ai_next) {

    _socket = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);
    if(_socket == INVALID_SOCKET) {
      continue;
    }

    if(nonblock) {
   #ifndef _WIN32
      int err_val = ::fcntl(_socket, F_SETFL, O_NONBLOCK);
   #else // _WIN32
      u_long nonblocking = 1;  // This flag may be set elsewhere,
                               // in a header ?
      int err_val = ::ioctlsocket(_socket, FIONBIO, &nonblocking);
   #endif // _WIN32
      if(err_val == -1) {
        setLastError();
        ::closesocket(_socket);
        continue;
      }
    }

    sockaddr_storage iaddr;
    ::memcpy(&iaddr, i->ai_addr, i->ai_addrlen);
    SOCKLEN iaddrlen = i->ai_addrlen;

    if(::connect(_socket, (sockaddr *)&iaddr, iaddrlen) < 0) {
      if(nonblock && getSystemError() == SOCKET_BLOCK_ERROR) {
        _connecting_socket = _socket;
        _connecting_address = i;
        _connecting_addrlist = ans;
        return;
      }
      setLastError();
      ::closesocket(_socket);
    } else {
      success = true;
    }

  }

  ::freeaddrinfo(ans);

  if (!success) {
    fail();
    return;
  }

#else // HAVE_GETADDRINFO

#warning Using deprecated resolver code because getaddrinfo() is not available

  // Create socket
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_STREAM, protocol);
  if(_socket == INVALID_SOCKET) {
    fail();
    return;
  }

  if(nonblock) {
 #ifndef _WIN32
    int err_val = ::fcntl(_socket, F_SETFL, O_NONBLOCK);
 #else // _WIN32
    u_long nonblocking = 1;  // This flag may be set elsewhere, in a header ?
    int err_val = ::ioctlsocket(_socket, FIONBIO, &nonblocking);
 #endif // _WIN32
    if(err_val == -1) {
      setLastError();
      ::closesocket(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
  }

  sockaddr_storage sa;
  SOCKLEN sa_size;
  hostent * he = ::gethostbyname(address.c_str());
  if (he != 0) {
    // If gethostbyname succeeded, we now have an address
    sa.ss_family = he->h_addrtype;
    ::memcpy(&((sockaddr_in&)sa).sin_addr, he->h_addr_list[0],
                                           he->h_length);
    sa_size = sizeof(sockaddr_in);
    ((sockaddr_in &)sa).sin_port = htons(service);
  } else {
    // If gethostbyname failed then our last chance is to assume
    // the string contains an IPv4 address in dotted quad form.
    // inet_aton is the cleaner way of reading this, but may
    // not be present on all systems.

    sockaddr_in & sa_in = (sockaddr_in &)sa;
 #ifdef HAVE_INET_ATON
    struct in_addr in_address;
    if (::inet_aton(address.c_str(), &in_address) == 0) {
      fail();
      ::closesocket(_socket);
      return;
    }
    ::memcpy(&sa_in.sin_addr, &in_address, sizeof(in_address));
 #else // HAVE_INET_ATON
    in_addr_t iaddr;
    iaddr = ::inet_addr(address.c_str());
    if (iaddr == INADDR_NONE) {
      fail();
      ::closesocket(_socket);
      return;
    }
    sa_in.sin_addr.s_addr = iaddr;
 #endif // HAVE_INET_ATON
    sa_in.sin_family = AF_INET;
    sa_in.sin_port = htons(service);
    sa_size = sizeof(sockaddr_in);
  }

  // Connect to host

  if(::connect(_socket,(sockaddr*)&sa, sa_size) == SOCKET_ERROR) {
    if(nonblock && getSystemError() == SOCKET_BLOCK_ERROR) {
      _connecting_socket = _socket;
      return;
    }
    setLastError();
    fail();
    ::closesocket(_socket);
    return;
  }
#endif // HAVE_GETADDRINFO

  // set the socket blocking again for io
  if(nonblock) {
#ifndef _WIN32
    int err_val = ::fcntl(_socket, F_SETFL, 0);
#else // _WIN32
    u_long blocking = 0;
    int err_val = ::ioctlsocket(_socket, FIONBIO, &blocking);
#endif // _WIN32
    if(err_val == -1) {
      setLastError();
      ::closesocket(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(_socket);
}

void tcp_socket_stream::open(const std::string & address, int service,
                             unsigned int milliseconds)
{
  open(address, service, true);
  if(!isReady(milliseconds)) {
    close();
    fail();
  }
}

void tcp_socket_stream::close()
{
  if(_connecting_socket != INVALID_SOCKET) {
    ::closesocket(_connecting_socket);
    _connecting_socket = INVALID_SOCKET;
  }

  basic_socket_stream::close();
}

SOCKET_TYPE tcp_socket_stream::getSocket() const
{
    return (_connecting_socket == INVALID_SOCKET)
            ? basic_socket_stream::getSocket() : _connecting_socket;
}

const std::string tcp_socket_stream::getRemoteHost(bool lookup) const
{
#ifdef HAVE_GETADDRINFO
  char hbuf[NI_MAXHOST];
  const int flags = lookup ? 0 : NI_NUMERICHOST;

  if (::getnameinfo((const sockaddr*)&getInpeer(),
                    stream_sockbuf.getInpeerSize(),
                    hbuf, sizeof(hbuf), 0, 0, flags) == 0) {
    return std::string(hbuf);
  }
  return "[unknown]";
#else // HAVE_GETADDRINFO
  return std::string(::inet_ntoa(((const sockaddr_in&)getInpeer()).sin_addr));
#endif // HAVE_GETADDRINFO
}

const std::string tcp_socket_stream::getRemoteService(bool lookup) const
{
  char sbuf[NI_MAXSERV];
#ifdef HAVE_GETADDRINFO
  const int flags = lookup ? 0 : NI_NUMERICSERV;

  if (::getnameinfo((const sockaddr*)&getInpeer(),
                    stream_sockbuf.getInpeerSize(),
                    0, 0, sbuf, sizeof(sbuf), flags) == 0) {
    return std::string(sbuf);
  }
  return "[unknown]";
#else // HAVE_GETADDRINFO

  unsigned short port = ntohs(((const sockaddr_in&)getInpeer()).sin_port);
  ::sprintf(sbuf, "%d", port);
  return std::string(sbuf);
#endif // HAVE_GETADDRINFO
}

bool tcp_socket_stream::isReady(unsigned int milliseconds)
{
  if(_connecting_socket == INVALID_SOCKET) {
    return true;
  }

  fd_set fds;
  struct timeval wait_time = {milliseconds / 1000, (milliseconds % 1000) * 1000};

  FD_ZERO(&fds);
  FD_SET(_connecting_socket, &fds);

  if (::select(_connecting_socket + 1, 0, &fds, 0, &wait_time) != 1
      || !FD_ISSET(_connecting_socket, &fds)) {
    return false;
  }

  // It's done connecting, check for error

  // We're no longer connecting, put the socket in a tmp variable
  SOCKET_TYPE _socket = _connecting_socket;
  _connecting_socket = INVALID_SOCKET;

  int errnum;
  SOCKLEN errsize = sizeof(errnum);
#ifndef _WIN32
  ::getsockopt(_socket, SOL_SOCKET, SO_ERROR, &errnum, &errsize);
#else // _WIN32
  Sleep(0);
  ::getsockopt(_socket, SOL_SOCKET, SO_ERROR, (LPSTR)&errnum, &errsize); 
#endif // _WIN32

#ifdef HAVE_GETADDRINFO
  // Check for failure, and if it has occured, we need to
  // revisit the address list we got from getaddrinfo.
  assert(_connecting_addrlist != 0);
  assert(_connecting_address != 0);

  if (errnum != 0) {
    ::closesocket(_socket);

    bool success = false;

    struct addrinfo * i = _connecting_address->ai_next;
    for (; success == false && i != 0; i = i->ai_next) {
      _socket = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);
      if(_socket == INVALID_SOCKET) {
        continue;
      }
   #ifndef _WIN32
      int err_val = ::fcntl(_socket, F_SETFL, O_NONBLOCK);
   #else // _WIN32
      u_long nonblocking = 1;  // This flag may be set elsewhere,
                               // in a header ?
      int err_val = ::ioctlsocket(_socket, FIONBIO, &nonblocking);
   #endif // _WIN32
      if(err_val == -1) {
        setLastError();
        ::closesocket(_socket);
        continue;
      }

      sockaddr_storage iaddr;
      ::memcpy(&iaddr, i->ai_addr, i->ai_addrlen);
      SOCKLEN iaddrlen = i->ai_addrlen;

      if(::connect(_socket, (sockaddr *)&iaddr, iaddrlen) < 0) {
        if(getSystemError() == SOCKET_BLOCK_ERROR) {
          _connecting_socket = _socket;
          _connecting_address = i;
          return false;
        }
        setLastError();
        ::closesocket(_socket);
      } else {
        success = true;
      }
    }

  }

  ::freeaddrinfo(_connecting_addrlist);
  _connecting_addrlist = 0;
  _connecting_address = 0;
#else // HAVE_GETADDRINFO
  if (errnum != 0) {
    // Can't use setLastError(), since errno doesn't have the error
    LastError = errnum;
    fail();
    ::closesocket(_socket);
    return true;
  }
#endif // HAVE_GETADDRINFO

  // set the socket blocking again for io
#ifndef _WIN32
  int err_val = ::fcntl(_socket, F_SETFL, 0);
#else // _WIN32
  u_long blocking = 0;
  int err_val = ::ioctlsocket(_socket, FIONBIO, &blocking);
#endif // _WIN32
  if(err_val == -1) {
    setLastError();
    ::closesocket(_socket);
    _socket = INVALID_SOCKET;
    fail();
    return true;
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(_socket);

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

udp_socket_stream::udp_socket_stream() :
                  basic_socket_stream(*new dgram_socketbuf(INVALID_SOCKET)),
                  dgram_sockbuf((dgram_socketbuf&)_sockbuf)
{
  protocol = FreeSockets::proto_UDP; 
}

udp_socket_stream::~udp_socket_stream()
{
  // Don't close the main socket, that is done in the basic_socket_stream
  // destructor
}

#ifdef SKSTREAM_UNIX_SOCKETS

#include <skstream/skstream_unix.h>

#include <sys/un.h>

/////////////////////////////////////////////////////////////////////////////
// class unix_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

unix_socket_stream::unix_socket_stream() :
                   basic_socket_stream(*new stream_socketbuf(INVALID_SOCKET)),
                   _connecting_socket(INVALID_SOCKET),
                   stream_sockbuf((stream_socketbuf&)_sockbuf) {
}

unix_socket_stream::unix_socket_stream(const std::string & address,
                                       bool nonblock) :
                   basic_socket_stream(*new stream_socketbuf(INVALID_SOCKET)),
                   _connecting_socket(INVALID_SOCKET),
                   stream_sockbuf((stream_socketbuf&)_sockbuf) {
  open(address, nonblock);
}

unix_socket_stream::unix_socket_stream(const std::string & address,
                                       unsigned int milliseconds) :
                   basic_socket_stream(stream_sockbuf),
                   _connecting_socket(INVALID_SOCKET),
                   stream_sockbuf((stream_socketbuf&)_sockbuf) {
  open(address, milliseconds);
}

unix_socket_stream::~unix_socket_stream()
{ 
  // Don't close the main socket, that is done in the basic_socket_stream
  // destructor
  if(_connecting_socket != INVALID_SOCKET) {
    ::shutdown(_connecting_socket, SHUT_RDWR);
    ::closesocket(_connecting_socket);
  }
}


void unix_socket_stream::open(const std::string& address, bool nonblock)
{
  if (address.size() >  107) {
    return;
  }

  if(is_open() || _connecting_socket != INVALID_SOCKET) close();

  // Create socket
  SOCKET_TYPE _socket = ::socket(AF_UNIX, SOCK_STREAM, protocol);
  if(_socket == INVALID_SOCKET) {
    fail();
    return;
  }

  if(nonblock) {
#ifndef _WIN32
    int err_val = ::fcntl(_socket, F_SETFL, O_NONBLOCK);
#else // _WIN32
    u_long nonblocking = 1;  // This flag may be set elsewhere,
                             // in a header ?
    int err_val = ::ioctlsocket(_socket, FIONBIO, &nonblocking);
#endif // _WIN32
    if(err_val == -1) {
      setLastError();
      ::closesocket(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
  }

  // Fill host information
  sockaddr_un sa;
  sa.sun_family = AF_UNIX;
  strncpy(sa.sun_path, address.c_str(), 108);

  if(::connect(_socket,(sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    if(nonblock && getSystemError() == SOCKET_BLOCK_ERROR) {
      _connecting_socket = _socket;
      return;
    }
    setLastError();
    fail();
    ::closesocket(_socket);
    return;
  }

  // set the socket blocking again for io
  if(nonblock) {
#ifndef _WIN32
    int err_val = ::fcntl(_socket, F_SETFL, 0);
#else // _WIN32
    u_long blocking = 0;
    int err_val = ::ioctlsocket(_socket, FIONBIO, &blocking);
#endif // _WIN32
    if(err_val == -1) {
      setLastError();
      ::closesocket(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(_socket);
}

void unix_socket_stream::close()
{
  if(_connecting_socket != INVALID_SOCKET) {
    ::closesocket(_connecting_socket);
    _connecting_socket = INVALID_SOCKET;
  }

  basic_socket_stream::close();
}

SOCKET_TYPE unix_socket_stream::getSocket() const
{
    return (_connecting_socket == INVALID_SOCKET)
            ? basic_socket_stream::getSocket() : _connecting_socket;
}

bool unix_socket_stream::isReady(unsigned int milliseconds)
{
  if(_connecting_socket == INVALID_SOCKET) {
    return true;
  }

  fd_set fds;
  struct timeval wait_time = {milliseconds / 1000, (milliseconds % 1000) * 1000};

  FD_ZERO(&fds);
  FD_SET(_connecting_socket, &fds);

  if (::select(_connecting_socket + 1, 0, &fds, 0, &wait_time) != 1
      || !FD_ISSET(_connecting_socket, &fds)) {
    return false;
  }

  // It's done connecting, check for error

  // We're no longer connecting, put the socket in a tmp variable
  SOCKET_TYPE _socket = _connecting_socket;
  _connecting_socket = INVALID_SOCKET;

  int errnum;
  SOCKLEN errsize = sizeof(errnum);
#ifndef _WIN32
  getsockopt(_socket, SOL_SOCKET, SO_ERROR, &errnum, &errsize);
#else // _WIN32
  Sleep(0);
  getsockopt(_socket, SOL_SOCKET, SO_ERROR, (LPSTR)&errnum, &errsize); 
#endif // _WIN32

  if(errnum != 0) {
    // Can't use setLastError(), since errno doesn't have the error
    LastError = errnum;
    fail();
    ::closesocket(_socket);
    return true;
  }

  // set the socket blocking again for io
#ifndef _WIN32
  int err_val = ::fcntl(_socket, F_SETFL, 0);
#else // _WIN32
  u_long blocking = 0;
  int err_val = ::ioctlsocket(_socket, FIONBIO, &blocking);
#endif // _WIN32
  if(err_val == -1) {
    setLastError();
    ::closesocket(_socket);
    _socket = INVALID_SOCKET;
    fail();
    return true;
  }

  // set socket for underlying socketbuf
    _sockbuf.setSocket(_socket);

  return true;
}

#endif // SKSTREAM_UNIX_SOCKETS

/////////////////////////////////////////////////////////////////////////////
// class raw_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////
#ifdef SOCK_RAW

raw_socket_stream::raw_socket_stream(FreeSockets::IP_Protocol proto) :
                  basic_socket_stream(*new dgram_socketbuf(INVALID_SOCKET)),
                  dgram_sockbuf((dgram_socketbuf&)_sockbuf)
{
  protocol = proto;
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
  _sockbuf.setSocket(_socket);
}

raw_socket_stream::raw_socket_stream(unsigned insize,unsigned outsize,
                                     FreeSockets::IP_Protocol proto) :
                  basic_socket_stream(*new dgram_socketbuf(INVALID_SOCKET,
                                                           insize,
                                                           outsize),
                                      proto),
                  dgram_sockbuf((dgram_socketbuf&)_sockbuf)
{
  protocol = proto;
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
  _sockbuf.setSocket(_socket);
}

void raw_socket_stream::setProtocol(FreeSockets::IP_Protocol proto) {
  if(is_open()) close();
  protocol = proto;
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
  _sockbuf.setSocket(_socket);
}

raw_socket_stream::~raw_socket_stream()
{
  // Don't close the main socket, that is done in the basic_socket_stream
  // destructor
}

#endif // SOCK_RAW
