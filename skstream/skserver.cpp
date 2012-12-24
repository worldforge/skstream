/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2000-2001 Rafael Guterres Jeffman
           (C) 2003-2006 Alistair Riddoch

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
 * This software package has been extensively modified by members of the
 * Worldforge Project. See the file ChangeLog for details.
 *
 * $Id$
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <skstream/skserver.h>

#include <skstream/skaddress.h>

#ifndef _WIN32
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#endif // _WIN32

#include <iostream>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#endif

//IPV6_V6ONLY isn't defined on some versions of Windows, as well as mingw.
//Note that this value would only come into play if the system supports IPv6, and then it's assumed that the socket library also understand how to handle this flag.
//However, I'm not completely certain that this always will work. But since we don't really support running servers on win32 anyways, and the
//unsupported system being winxp which by now is deprecated I think doing it this way isn't totally crazy. /erik 20130515
#ifndef IPV6_V6ONLY
#define IPV6_V6ONLY 27
#endif

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

const int basic_socket_server::SK_SRV_NONE;
const int basic_socket_server::SK_SRV_PURE;
const int basic_socket_server::SK_SRV_REUSE;

basic_socket_server::~basic_socket_server() {
  if(_socket != INVALID_SOCKET) {
    ::closesocket(_socket);
  }
}

SOCKET_TYPE basic_socket_server::getSocket() const {
  return _socket;
}

// close server's underlying socket
//   The shutdown is a little rude... -  RGJ
void basic_socket_server::close() {
  if(_socket != INVALID_SOCKET) {
    if(::closesocket(_socket) == SOCKET_ERROR) {
      setLastError();
      return;
    }
    _socket = INVALID_SOCKET;
  }
}

void basic_socket_server::shutdown() {
  if(_socket != INVALID_SOCKET) {
    if(::shutdown(_socket, SHUT_RDWR) == SOCKET_ERROR) {
      setLastError();
    }
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

int ip_socket_server::bindToAddressInfo(struct addrinfo * i)
{
  _socket = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);
  if (_socket == INVALID_SOCKET) {
    setLastError();
    return -1;
  }

  if (_flags & SK_SRV_PURE && i->ai_family == AF_INET6) {
    int flag = 1;
    ::setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&flag, sizeof(flag));
  }

  if (_flags & SK_SRV_REUSE) {
    int flag = 1;
    ::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
  }

  sockaddr_storage iaddr;
  ::memcpy(&iaddr, i->ai_addr, i->ai_addrlen);
  SOCKLEN iaddrlen = i->ai_addrlen;

  if (::bind(_socket, (sockaddr*)&iaddr, iaddrlen) == SOCKET_ERROR) {
    setLastError();
    close();
    return -1;
  }


  return 0;
}

int ip_socket_server::bindToIpService(int service, int type, int protocol)
{
  char serviceName[32];

  ::sprintf(serviceName, "%d", service);

  tcp_address l;

  if (l.resolveListener(serviceName) != 0) {
    return -1;
  }

  int success = -1;

  tcp_address::const_iterator I = l.begin();
  for(; success == -1 && I != l.end(); ++I) {
    success = bindToAddressInfo(*I);
  }

  return success;
}

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
SOCKET_TYPE tcp_socket_server::accept()
{
  if(_socket==INVALID_SOCKET) return INVALID_SOCKET;
  SOCKET_TYPE commsock = ::accept(_socket, NULL, NULL);
  if(commsock == INVALID_SOCKET) {
    setLastError();
  }
  return commsock;
}

// start tcp server and put it in listen state
int tcp_socket_server::open(int service)
{
  if (is_open()) {
    close();
  }

  if (bindToIpService(service, SOCK_STREAM, IPPROTO_TCP) != 0) {
    return -1;
  }

  // Listen
  if(::listen(_socket, 5) == SOCKET_ERROR) { // max backlog
    setLastError();
    close();
    return -1;
  }

  return 0;
}

int tcp_socket_server::open(struct addrinfo * i)
{
  if (is_open()) {
    close();
  }

  if (bindToAddressInfo(i) != 0) {
    return -1;
  }

  // Listen
  if(::listen(_socket, 5) == SOCKET_ERROR) { // max backlog
    setLastError();
    close();
    return -1;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server implementation
/////////////////////////////////////////////////////////////////////////////

udp_socket_server::~udp_socket_server()
{
}

// create a UDP socket binded to a given port
int udp_socket_server::open(int service)
{
  if (is_open()) {
    close();
  }

  if (bindToIpService(service, SOCK_DGRAM, IPPROTO_UDP) != 0) {
    return -1;
  }

  return 0;
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
  }
  return commsock;
}

// start unix server and put it in listen state
int unix_socket_server::open(const std::string & service) {
  if(is_open()) close();

  if (service.size() > 107) {
    return -1;
  }

  // create socket
  _socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
  if(_socket == INVALID_SOCKET) {
    setLastError();
    return -1;
  }

  // Bind Socket
  sockaddr_un sa;
  sa.sun_family = AF_UNIX;
  strncpy(sa.sun_path, service.c_str(), 108);
  if(::bind(_socket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    setLastError();
    close();
    return -1;
  }

  // Listen
  if(::listen(_socket, 5) == SOCKET_ERROR) { // max backlog
    setLastError();
    close();
    return -1;
  }
  return 0;
}


#endif // SKSTREAM_UNIX_SOCKETS
