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
#include "skstream.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// class socketbuf implementation
/////////////////////////////////////////////////////////////////////////////
// Constructor
socketbuf::socketbuf(SOCKET_TYPE sock, unsigned insize, unsigned outsize)
    : _socket(sock), streambuf(), Timeout(false) {
  _buffer = NULL;
  // allocate 16k buffer each for input and output
  const int bufsize = insize+outsize;
  // Allocate a new buffer
  char* buffer = new char[bufsize];
  memset(buffer,0,bufsize);
  // Setup the buffer
  if(this != setbuf(buffer, bufsize)) {
    _buffer = NULL;
  }
  _timeout.tv_sec  = 0;
  _timeout.tv_usec = 0;

  int size = sizeof(sockaddr);
  ::getpeername(sock,(sockaddr*)&out_peer,(SOCKLEN*)&size);
  in_peer = out_peer;
}

// Constructor
socketbuf::socketbuf(SOCKET_TYPE sock, char* buf, int length)
    : _socket(sock),std::streambuf(), Timeout(false)
{
  _buffer = NULL;
  if(this != setbuf(buf,length)) {
    _buffer = NULL;
  }
  _timeout.tv_sec  = 0;
  _timeout.tv_usec = 0;

  int size = sizeof(sockaddr);
  getpeername(sock,(sockaddr*)&out_peer,(SOCKLEN*)&size);
  in_peer = out_peer;
}
// Destructor
socketbuf::~socketbuf(){
  sync();
  delete [] _buffer;
  _buffer = NULL;
}

// set the other side host
bool socketbuf::setOutpeer(const string& address, unsigned port) {
  unsigned long iaddr;
  hostent *he = ::gethostbyname(address.c_str());
  if(he!=NULL) {
    iaddr = *(unsigned long *)(he->h_addr_list[0]);
  } else {
    iaddr = ::inet_addr(address.c_str());
  }
  if(iaddr == INADDR_NONE)
    return false;
  // Fill host information
  out_peer.sin_family = AF_INET;
  out_peer.sin_addr.s_addr = iaddr;
  out_peer.sin_port = htons(port);
  return true;
}

// The next function are those who do the dirt work

// overflow() - handles output to a connected socket.
int socketbuf::overflow(int nCh) { // traits::eof()
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
        sr = select(_socket+1,NULL,&socks,NULL,&_tv);
        if(/*(sr == 0) || */ !FD_ISSET(_socket,&socks)){
          Timeout = true;
          return EOF; // a timeout error should be set here! - RGJ
         } else if(sr < 0) return EOF; // error on select() // traits::eof()
      }
      Timeout = false;
    }
  #endif

  // send pending data or return EOF on error
  int insize = sizeof(sockaddr);
  size=sendto(_socket, pbase(),pptr()-pbase(),0,(sockaddr*)&out_peer,insize);

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
int socketbuf::underflow() {
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
      sr = select(_socket+1,&socks,NULL,NULL,&_tv);
      if((sr == 0) || !FD_ISSET(_socket,&socks)){
        Timeout = true;
        return EOF; // a timeout error should be set here! - RGJ
      } else if(sr < 0) return EOF;  // error on select()
    }
    Timeout = false;
  #endif

  // receive data or return EOF on error
  int insize = sizeof(sockaddr);
  size = ::recvfrom(_socket, eback(), egptr()-eback(), 0,
                    (sockaddr*)&in_peer,(SOCKLEN*)&insize);

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
// System dependant initialization
bool basic_socket_stream::startup() {
  #ifdef _WIN32
    const unsigned wMinVer = 0x0101;	// request WinSock v1.1 (at least)
    WSADATA wsaData;
    LastError = WSAStartup(wMinVer, &wsaData);
    return (LastError == 0);
  #else
    return true;
  #endif
}

// System dependant finalization
bool basic_socket_stream::shutdown() {
  if(is_open()) close();
  #ifdef _WIN32
    LastError = WSACleanup();
    return (LastError == 0);
  #else
    return true;
  #endif
}

// private function that sets the internal variable LastError
void basic_socket_stream::setLastError() const {
  #ifdef _WIN32
    LastError = WSAGetLastError();
  #else
    LastError = errno;
  #endif
}

// closes a socket connection
void basic_socket_stream::close() {
  if(is_open()) {
    if(::shutdown(_sockbuf.getSocket(),0) == SOCKET_ERROR) {
      setLastError();
      return ;
    }
    if(::closesocket(_sockbuf.getSocket()) == SOCKET_ERROR) {
      setLastError();
      return;
    }
    _sockbuf.setSocket(INVALID_SOCKET);
  }
}

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////
void tcp_socket_stream::open(const std::string& addr, int service) {
  if(is_open()) close();

  // Create socket
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_STREAM, protocol);
  if(_socket == INVALID_SOCKET) {
    fail();
    return;
  }

  // find host name
  unsigned long iaddr;
  // try to resolve DNS for host
  hostent *he = ::gethostbyname(addr.c_str());
  if(he!=NULL) {
    iaddr = *(unsigned long *)(he->h_addr_list[0]);
  } else {
    // if it could not resolve DNS, host name can be in dot address already
    iaddr = ::inet_addr(addr.c_str());
  }
  if(iaddr == INADDR_NONE) {
    fail();
    close();
    return;
  }

  // Fill host information
  sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = iaddr;
  sa.sin_port = htons(service);
  // Connect to host
  if(::connect(_socket,(sockaddr*)&sa,sizeof(sa)) == SOCKET_ERROR) {
    fail();
    close();
    return;
  }
  // set socket for underlying socketbuf
  _sockbuf.setSocket(_socket);
}

/////////////////////////////////////////////////////////////////////////////
// class raw_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////
#ifdef SOCK_RAW
void raw_socket_stream::setProtocol(FreeSockets::IP_Protocol proto) {
  if(is_open()) close();
  protocol = proto;
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
  _sockbuf.setSocket(_socket);
}

#endif
