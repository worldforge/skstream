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
#include "skserver.h"

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
      return;
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

