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

/**
 * This software package has been modified by the Worldforge Project 
 * in the following ways:
 *
 * $Log$
 * Revision 1.12  2003-08-23 22:13:55  alriddoch
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
 * Revision 1.11  2003/08/23 14:01:57  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.h, skstream/skserver_unix.h: Add default constructor
 *       to tcp_socket_server, and make single argument constructors explicit.
 *     - skstream/skserver.cpp: Use AI_PASSIVE with getaddrinfo() to make listen
 *       sockets accept any connection.
 *
 * Revision 1.10  2003/08/23 12:14:39  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.cpp: Use getaddrinfo to create server sockets
 *       in a protocol independant way.
 *
 * Revision 1.9  2003/08/08 23:56:26  alriddoch
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
 * Revision 1.8  2003/07/30 23:17:55  alriddoch
 *  2003-07-30 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.cpp, skstream/skserver.h, skstream/skstream.cpp,
 *       skstream/skstream.h, skstream/skstream_unix.h: Move virtual
 *       function implementations into .cpp files.
 *
 * Revision 1.7  2003/05/04 00:34:29  alriddoch
 *  2003-05-04 Al Riddoch <alriddoch@zepler.org>,
 *     - Add a second pkgconfig file for apps that need unix socket support.
 *     - Rename sksystem.h as skstreamconfig.h, and put it in an architecture
 *       dependent subdir. Modify pkgconfig files to handle this.
 *     - Handle errors in a more general way to reduce the number
 *       of places where pre-processor conditionals are required.
 *     - Remove some redundant code for dealing with win32 that has
 *       now been superceded.
 *
 * Revision 1.6  2003/04/16 14:02:35  alriddoch
 *  2003-04-16 Al Riddoch <alriddoch@zepler.org>,
 *     - Fix up includes so they work properlly
 *
 * Revision 1.5  2002/04/08 20:02:00  xmp
 * Just a few fixes to MSVC support, removing a few unnessesary std::'s and shifting a default argument to a header.
 *
 * Revision 1.4  2002/03/01 14:00:09  grimicus
 * 03/01/2002 Dan Tomalesky <grim@xynesis.com>
 *     * Changed can_accept's timeval to be all 0, so that it is just a quick
 *       poll and returns immediately (Thanks to alriddoch for the suggestion)
 *
 *     * Modified the ugly switch block I was using to print out error message
 *       names to use strerror (Thanks to James for that one)
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

#include <skstream/skserver.h>

#ifndef _WIN32
#include <sys/types.h>
#include <netdb.h>
#endif // _WIN32

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

/////////////////////////////////////////////////////////////////////////////
// class basic_socket_server implementation
/////////////////////////////////////////////////////////////////////////////
// sistem dependant initialization
bool basic_socket_server::startup(){
  #ifdef _WIN32
    const unsigned wMinVer = 0x0101;	// request WinSock v1.1 (at least)
    WSADATA wsaData;
    LastError = WSAStartup(wMinVer, &wsaData);
    return (LastError == 0);
  #else
    return true;
  #endif
}

// sistem dependant finalization
bool basic_socket_server::shutdown() {
  if(is_open()) close();
  #ifdef _WIN32
    LastError = WSACleanup();
    return (LastError == 0);
  #else
    return true;
  #endif
}

// sistem dependant error verification
void basic_socket_server::setLastError() {
    LastError = getSystemError();
}

basic_socket_server::~basic_socket_server()
{
    close();
    shutdown();
}

SOCKET_TYPE basic_socket_server::getSocket() const
{
    return _socket;
}

// close server's underlying socket
//   The shutdown is a little rude... -  RGJ
void basic_socket_server::close() {
  if(is_open()) {
    if(::shutdown(_socket,0) == INVALID_SOCKET) {
      setLastError();
      //not necessarily a returning offense because there could be a socket
      //open that has never connected to anything and hence, does not need
      //to be shutdown.
      //return;
    }

    if(::closesocket(_socket) == INVALID_SOCKET) {
      setLastError();
      return;
    }
    _socket = INVALID_SOCKET;
  }
}

bool basic_socket_server::can_accept() {
  if(_socket == INVALID_SOCKET) return false;

  fd_set sock_fds;
  struct timeval tv;

  tv.tv_sec=0;
  tv.tv_usec=0;

  FD_ZERO(&sock_fds);
  FD_SET(_socket, &sock_fds);

  int ret = ::select((_socket + 1), &sock_fds, NULL, NULL, &tv);

  if( ret > 0) {
      return true;
  }
  else if (ret < 0) {
      setLastError();
  }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
// class ip_socket_server implementation
/////////////////////////////////////////////////////////////////////////////

ip_socket_server::~ip_socket_server()
{
}

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_server implementation
/////////////////////////////////////////////////////////////////////////////

tcp_socket_server::~tcp_socket_server()
{
}

// handles tcp connections request
SOCKET_TYPE tcp_socket_server::accept() {
  if(_socket==INVALID_SOCKET) return INVALID_SOCKET;
  SOCKET_TYPE commsock = ::accept(_socket, NULL, NULL);
  if(commsock == INVALID_SOCKET) {
    setLastError();
    close();
    return INVALID_SOCKET;
  }
  return commsock;
}

// start tcp server and put it in listen state
void tcp_socket_server::open(int service)
{
  if (is_open()) {
    close();
  }

#ifdef HAVE_GETADDRINFO
  struct addrinfo req, *ans;
  char serviceName[32];

  sprintf(serviceName, "%d", service);

  req.ai_flags = AI_PASSIVE;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = SOCK_STREAM;
  req.ai_protocol = 0;
  req.ai_addrlen = 0;
  req.ai_addr = 0;
  req.ai_canonname = 0;
  req.ai_next = 0;

  int ret;
  if ((ret = ::getaddrinfo(0, serviceName, &req, &ans)) != 0) {
    std::cout << "skstream: " << gai_strerror(ret)
              << std::endl << std::flush;
    setLastError();
    return;
  }

  _socket = ::socket(ans->ai_family, ans->ai_socktype, ans->ai_protocol);
  if (_socket == INVALID_SOCKET) {
    setLastError();
    return;
  }

  sockaddr_storage iaddr;
  memcpy(&iaddr, ans->ai_addr, ans->ai_addrlen);
  SOCKLEN iaddrlen = ans->ai_addrlen;
  ::freeaddrinfo(ans);

  if (::bind(_socket, (sockaddr*)&iaddr, iaddrlen) == SOCKET_ERROR) {
    setLastError();
    close();
    return;
  }

  // Listen
  if(::listen(_socket, 5) == SOCKET_ERROR) { // max backlog
    setLastError();
    close();
    return;
  }
#else
  // create socket
  _socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(_socket == INVALID_SOCKET) {
    setLastError();
    return;
  }

  // Bind Socket
  sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = INADDR_ANY; // we want to connect to ANY client!
  sa.sin_port = htons((unsigned short)service); // define service port
  if(::bind(_socket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    setLastError();
    close();
    return;
  }

  // Listen
  if(::listen(_socket, 5) == SOCKET_ERROR) { // max backlog
    setLastError();
    close();
    return;
  }
#endif // HAVE_GETADDRINFO
}

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server implementation
/////////////////////////////////////////////////////////////////////////////

udp_socket_server::~udp_socket_server()
{
}

SOCKET_TYPE udp_socket_server::accept()
{
    return _socket;
}

// create a UDP socket binded to a given port
void udp_socket_server::open(int service)
{
  if (is_open()) {
    close();
  }

#ifdef HAVE_GETADDRINFO
  struct addrinfo req, *ans;
  char serviceName[32];

  sprintf(serviceName, "%d", service);

  req.ai_flags = AI_PASSIVE;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = SOCK_DGRAM;
  req.ai_protocol = 0;
  req.ai_addrlen = 0;
  req.ai_addr = 0;
  req.ai_canonname = 0;
  req.ai_next = 0;

  int ret;
  if ((ret = ::getaddrinfo(0, serviceName, &req, &ans)) != 0) {
    std::cout << "skstream: " << gai_strerror(ret)
              << std::endl << std::flush;
    setLastError();
    return;
  }

  _socket = ::socket(ans->ai_family, ans->ai_socktype, ans->ai_protocol);
  if (_socket == INVALID_SOCKET) {
    setLastError();
    return;
  }

  sockaddr_storage iaddr;
  memcpy(&iaddr, ans->ai_addr, ans->ai_addrlen);
  SOCKLEN iaddrlen = ans->ai_addrlen;
  ::freeaddrinfo(ans);

  if (::bind(_socket, (sockaddr*)&iaddr, iaddrlen) == SOCKET_ERROR) {
    setLastError();
    close();
    return;
  }
#else // HAVE_GETADDRINFO
  // Create socket
  _socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(_socket == INVALID_SOCKET) {
    setLastError();
    return;
  }
  // Bind Socket
  sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = INADDR_ANY; // we want to connect to ANY client!
  //sa.sin_addr.s_addr = inet_addr("10.10.100.1");

  sa.sin_port = htons((unsigned short)service); // define service port
  if(::bind(_socket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    setLastError();
    close();
    return;
  }
#endif // HAVE_GETADDRINFO
}

#ifdef SKSTREAM_UNIX_SOCKETS

#include <skstream/skserver_unix.h>

#include <sys/un.h>

/////////////////////////////////////////////////////////////////////////////
// class unix_socket_server implementation
/////////////////////////////////////////////////////////////////////////////

unix_socket_server::~unix_socket_server()
{
}

// handles unix connections request
SOCKET_TYPE unix_socket_server::accept() {
  if(_socket==INVALID_SOCKET) return INVALID_SOCKET;
  SOCKET_TYPE commsock = ::accept(_socket, NULL, NULL);
  if(commsock == INVALID_SOCKET) {
    setLastError();
    close();
    return INVALID_SOCKET;
  }
  return commsock;
}

// start unix server and put it in listen state
void unix_socket_server::open(const std::string & service) {
  if(is_open()) close();

  if (service.size() > 107) {
    return;
  }

  // create socket
  _socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
  if(_socket == INVALID_SOCKET) {
    setLastError();
    return;
  }

  // Bind Socket
  sockaddr_un sa;
  sa.sun_family = AF_UNIX;
  strncpy(sa.sun_path, service.c_str(), 108);
  if(::bind(_socket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    setLastError();
    close();
    return;
  }

  // Listen
  if(::listen(_socket, 5) == SOCKET_ERROR) { // max backlog
    setLastError();
    close();
    return;
  }
}


#endif // SKSTREAM_UNIX_SOCKETS
