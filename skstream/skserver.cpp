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
 * Revision 1.6  2003-04-16 14:02:35  alriddoch
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
  #ifdef _WIN32
    LastError = WSAGetLastError();
  #else
    LastError = errno;
  #endif
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

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_server implementation
/////////////////////////////////////////////////////////////////////////////
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

bool tcp_socket_server::can_accept() {
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

// start tcp server and put it in listen state
void tcp_socket_server::open(int service) {
  if(is_open()) close();

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
}

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server implementation
/////////////////////////////////////////////////////////////////////////////
// create a UDP socket binded to a given port
void udp_socket_server::open(int service) {
  if(is_open()) close();
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
}

