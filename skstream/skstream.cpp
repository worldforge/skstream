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
 * Revision 1.6  2002-05-21 07:29:36  malcolm
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
#include "skstream.h"

#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#else
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef socklen_t
#define socklen_t int
#endif
#endif

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
    : _socket(sock),streambuf(), Timeout(false)
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
// Constructors
  basic_socket_stream::basic_socket_stream()
      : iostream(&_sockbuf), _sockbuf(INVALID_SOCKET), protocol(FreeSockets::proto_IP), LastError(0)
  {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }

  basic_socket_stream::basic_socket_stream(unsigned insize,unsigned outsize,
                      int proto)
      : iostream(&_sockbuf), _sockbuf(INVALID_SOCKET,insize,outsize),
        protocol(proto), LastError(0)
  {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }

  basic_socket_stream::basic_socket_stream(SOCKET_TYPE sock)
      : iostream(&_sockbuf), _sockbuf(sock), protocol(FreeSockets::proto_IP), LastError(0) {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }

  basic_socket_stream::basic_socket_stream(SOCKET_TYPE sock,
                      unsigned insize,unsigned outsize)
      : iostream(&_sockbuf), _sockbuf(sock,insize,outsize),
        protocol(FreeSockets::proto_IP), LastError(0) {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }

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
      //not necessarily a returning offense because there could be a socket
      //open that has never connected to anything and hence, does not need
      //to be shutdown.
      //return;
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
    if(iostream::fail()) {
      setLastError();
      return true;
    }
    return false;
  }



/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////
void tcp_socket_stream::open(const std::string& address, int service, bool nonblock) {
  if(is_open() || _connecting_socket) close();

  // Create socket
  SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_STREAM, protocol);
  if(_socket == INVALID_SOCKET) {
    fail();
    return;
  }

  if(nonblock) {
#ifndef _WIN32
    int err_val = fcntl(_socket, F_SETFL, O_NONBLOCK);
    if(err_val == -1) {
      setLastError();
      ::close(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
#else
    unsigned long nonblocking = 1;  // This flag may be set elsewhere,
                                    // in a header ?
    int err_val = ioctlsocket(_socket, FIONBIO, &nonblocking);
    if(err_val == -1) {
      setLastError();
      ::closesocket(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
#endif
  }

  // find host name
  unsigned long iaddr;
  // try to resolve DNS for host
  hostent *he = ::gethostbyname(address.c_str());
  if(he!=NULL) {
    iaddr = *(unsigned long *)(he->h_addr_list[0]);
  } 
  else {
    // if it could not resolve DNS, host name can be in dot address already
    iaddr = ::inet_addr(address.c_str());
  }

  if(iaddr == INADDR_NONE) {
    fail();
#ifndef _WIN32
    ::close(_socket);
#else
    ::closesocket(_socket);
#endif
    return;
  }

  // Fill host information
  sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = iaddr;
  sa.sin_port = htons(service);
  // Connect to host

  if(::connect(_socket,(sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    if(nonblock && errno == EINPROGRESS) {
      _connecting_socket = _socket;
      return;
    }
    setLastError();
    fail();
#ifndef _WIN32
    ::close(_socket);
#else
    ::closesocket(_socket);
#endif
    return;
  }

  // set the socket blocking again for io
  if(nonblock) {
#ifndef _WIN32
    int err_val = fcntl(_socket, F_SETFL, 0);
    if(err_val == -1) {
      setLastError();
      ::close(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
#else
    int err_val = ioctlsocket(_socket, FIONBIO, 0);
    if(err_val == -1) {
      setLastError();
      ::closesocket(_socket);
      _socket = INVALID_SOCKET;
      fail();
      return;
    }
#endif
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(_socket);
}

void tcp_socket_stream::close()
{
  if(_connecting_socket) {
#ifndef _WIN32
    ::close(_connecting_socket);
#else
		::closesocket(_connecting_socket);
#endif
    _connecting_socket = 0;
  }

  basic_socket_stream::close();
}

bool tcp_socket_stream::is_ready()
{
  if(!_connecting_socket)
    return true;

  fd_set fds;
  struct timeval zero_time = {0, 0};

  FD_ZERO(&fds);
  FD_SET(_connecting_socket, &fds);

  if(select(_connecting_socket + 1, 0, &fds, 0, &zero_time) != 1
    || !FD_ISSET(_connecting_socket, &fds))
    return false;

  // It's done connecting, check for error

  // We're no longer connecting, put the socket in a tmp variable
  SOCKET_TYPE _socket = _connecting_socket;
  _connecting_socket = 0;

  int errnum;
  socklen_t errsize = sizeof(errnum);
#ifndef _WIN32
  getsockopt(_socket, SOL_SOCKET, SO_ERROR, &errnum, &errsize);
#else
  getsockopt(_socket, SOL_SOCKET, SO_ERROR, (char *)&errnum, &errsize); 
#endif

  if(errnum != 0) {
    // Can't use setLastError(), since errno doesn't have the error
    LastError = errnum;
    fail();
#ifndef _WIN32
    ::close(_socket);
#else
		::closesocket(_socket);
#endif
    return true;
  }

  // set the socket blocking again for io
#ifndef _WIN32
  int err_val = fcntl(_socket, F_SETFL, 0);
  if(err_val == -1) {
    setLastError();
    ::close(_socket);
    _socket = INVALID_SOCKET;
    fail();
    return true;
  }
#else
  int err_val = ioctlsocket(_socket, FIONBIO, 0);
  if(err_val == -1) {
    setLastError();
    ::closesocket(_socket);
    _socket = INVALID_SOCKET;
    fail();
    return true;
  }
#endif

  // set socket for underlying socketbuf
    _sockbuf.setSocket(_connecting_socket); 
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
