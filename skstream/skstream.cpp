/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2000-2001 Rafael Guterres Jeffman
           (C) 2002-2006 Alistair Riddoch

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

#include <skstream/skstream.h>

#ifndef _WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#endif // _WIN32

#include <cstdio>
#include <cstring>
#include <cassert>

#ifdef _WIN32
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
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

#ifndef HAVE_GAI_STRERROR
const char * skstream_gai_strerror(int errcode)
{
    switch (errcode) {
#ifdef EAI_BADFLAGS
      case EAI_BADFLAGS:
        return "Invalid value for `ai_flags' field.";
#endif
#ifdef EAI_NONAME
      case EAI_NONAME:
        return "NAME or SERVICE is unknown.";
#endif
#ifdef EAI_AGAIN
      case EAI_AGAIN:
        return "Temporary failure in name resolution.";
#endif
#ifdef EAI_FAIL
      case EAI_FAIL:
        return "Non-recoverable failure in name res.";
#endif
#ifdef EAI_NODATA
      case EAI_NODATA:
        return "No address associated with NAME.";
#endif
#ifdef EAI_FAMILY
      case EAI_FAMILY:
        return "`ai_family' not supported.";
#endif
#ifdef EAI_SOCKTYPE
      case EAI_SOCKTYPE:
        return "`ai_socktype' not supported.";
#endif
#ifdef EAI_SERVICE
      case EAI_SERVICE:
        return "SERVICE not supported for `ai_socktype'.";
#endif
#ifdef EAI_ADDRFAMILY
      case EAI_ADDRFAMILY:
        return "Address family for NAME not supported.";
#endif
#ifdef EAI_MEMORY
      case EAI_MEMORY:
        return "Memory allocation failure.";
#endif
#ifdef EAI_SYSTEM
      case EAI_SYSTEM:
        return "System error returned in `errno'.";
#endif
#ifdef EAI_OVERFLOW
      case EAI_OVERFLOW:
        return "Argument buffer overflow.";
#endif
      default:
        return "Unknown error.";
    }
}

static const char * gai_strerror(int errcode)
{
    return skstream_gai_strerror(errcode);
}
#endif // HAVE_GAI_STRERROR

/////////////////////////////////////////////////////////////////////////////
// class socketbuf implementation
/////////////////////////////////////////////////////////////////////////////
// Constructor
socketbuf::socketbuf(SOCKET_TYPE sock, std::streamsize insize,
                                       std::streamsize outsize)
    : _buffer(0), _socket(sock), Timeout(false)
{
  // allocate 16k buffer each for input and output
  const std::streamsize bufsize = insize + outsize;
  // Allocate a new buffer
  std::streambuf::char_type * buffer = new std::streambuf::char_type[bufsize];
  ::memset(buffer,0,bufsize);
  // Setup the buffer
  setbuf(buffer, bufsize);

  _underflow_timeout.tv_sec  = 0;
  _underflow_timeout.tv_usec = 0;

  _overflow_timeout.tv_sec  = 0;
  _overflow_timeout.tv_usec = 0;

  setSocket(sock);
}

// Constructor
socketbuf::socketbuf(SOCKET_TYPE sock, std::streambuf::char_type * buf,
                                       std::streamsize length)
    : _buffer(0), _socket(sock), Timeout(false)
{
  setbuf(buf, length);

  _underflow_timeout.tv_sec  = 0;
  _underflow_timeout.tv_usec = 0;

  _overflow_timeout.tv_sec  = 0;
  _overflow_timeout.tv_usec = 0;

  setSocket(sock);
}

// Destructor
socketbuf::~socketbuf()
{
  delete [] _buffer;
  if(_socket != INVALID_SOCKET){
    ::closesocket(_socket);
  }
}

void socketbuf::setSocket(SOCKET_TYPE sock)
{
  _socket = sock;
}


int socketbuf::sync()
{
    if(overflow() == traits_type::eof())
      return traits_type::eof();         // ios will set the fail bit
    else {
// This appeared to be causing the stream to lose data.
//      // empty put and get areas
//      setp(pbase(), epptr());
//      setg(eback(), egptr(), egptr());
//
      return 0; // traits::not_eof(0);
    }
}

std::streambuf * socketbuf::setbuf(std::streambuf::char_type * buf,
                                   std::streamsize len)
{
    if((buf != NULL) && (len > 0)) {
      _buffer = buf;
      setp(_buffer, _buffer+(len >> 1));
      setg(_buffer+(len >> 1), _buffer+len, _buffer+len);
    }

    return this;
}

stream_socketbuf::stream_socketbuf(SOCKET_TYPE sock,
                                   std::streamsize insize,
                                   std::streamsize outsize)
    : socketbuf(sock, insize, outsize) { }

stream_socketbuf::stream_socketbuf(SOCKET_TYPE sock,
                                   std::streambuf::char_type * buf,
                                   std::streamsize length)
    : socketbuf(sock, buf, length) { }

stream_socketbuf::~stream_socketbuf()
{
}

dgram_socketbuf::dgram_socketbuf(SOCKET_TYPE sock,
                                 std::streamsize insize,
                                 std::streamsize outsize)
    : socketbuf(sock, insize, outsize),
      out_p_size(sizeof(out_peer)), in_p_size(sizeof(in_peer))
{
}

dgram_socketbuf::dgram_socketbuf(SOCKET_TYPE sock,
                                 std::streambuf::char_type * buf,
                                 std::streamsize length)
    : socketbuf(sock, buf, length),
      out_p_size(sizeof(out_peer)), in_p_size(sizeof(in_peer))
{
}

dgram_socketbuf::~dgram_socketbuf()
{
  sync();
}

// The next function are those who do the dirt work

// overflow() - handles output to a connected socket.
std::streambuf::int_type stream_socketbuf::overflow(std::streambuf::int_type nCh) {
  // in case of error, user finds out by testing fail()
  if(_socket==INVALID_SOCKET) {
    return traits_type::eof(); // Invalid socket
  }

  if(pptr()-pbase() <= 0) {
    return 0; // nothing to send
  }

  // prepare structure for detecting timeout

  // if a timeout was specified, wait for it.

  if((_overflow_timeout.tv_sec+_overflow_timeout.tv_usec) > 0) {
    int sr;
    timeval tv = _overflow_timeout;
    fd_set socks;
    FD_ZERO(&socks); // zero fd_set
    FD_SET(_socket,&socks); // add buffer socket to fd_set
    sr = ::select(_socket+1,NULL,&socks,NULL,&tv);
    if(sr == 0){
      Timeout = true;
      return traits_type::eof(); // a timeout error should be set here! - RGJ
    } else if(sr < 0) {
      return traits_type::eof(); // error on select()
    }
    assert(FD_ISSET(_socket,&socks));
  }
  Timeout = false;

  // send pending data or return eof() on error
  int size=::send(_socket, pbase(),pptr()-pbase(),0);

  if(size < 0) {
    return traits_type::eof(); // Socket Could not send
  }

  if(size == 0) {
    return traits_type::eof(); // remote site has closed this connection
  }

  if(nCh != traits_type::eof()) {
    // size >= 1 at this point
    size--;
    *(pbase()+size)=nCh;
  }

  // move remaining pbase()+size .. pptr()-1 => pbase() .. pptr()-size-1
  for(std::streambuf::char_type * p=pbase() + size; p < pptr(); p++)
    *(p - size) = *p;

  const int newlen = (pptr() - pbase()) - size;

  setp(pbase(),epptr());

  pbump(newlen);

  return 0; // traits::not_eof(ch)
}

// underflow() - handles input from a connected socket.
std::streambuf::int_type stream_socketbuf::underflow()
{
  if(_socket == INVALID_SOCKET) {
    return traits_type::eof(); // Invalid socket!
  }

  assert(gptr());
  if(gptr() < egptr()) {
    return traits_type::to_int_type(*this->gptr());
  }

  // prepare structure for detecting timeout

  // if a timeout was specified, wait for it.
  if((_underflow_timeout.tv_sec+_underflow_timeout.tv_usec) > 0) {
    int sr;
    timeval tv = _underflow_timeout;
    fd_set socks;
    FD_ZERO(&socks); // zero fd_set
    FD_SET(_socket,&socks); // add buffer socket to fd_set
    sr = ::select(_socket+1,&socks,NULL,NULL,&tv);
    if(sr == 0){
      Timeout = true;
      return traits_type::eof(); // a timeout error should be set here! - RGJ
    } else if(sr < 0) {
      return traits_type::eof(); // error on select()
    }
    assert(FD_ISSET(_socket,&socks));
  }
  Timeout = false;


  // fill up from eback to egptr
  // receive data or return eof() on error
  int size = ::recv(_socket, eback(), egptr()-eback(), 0);

  if(size <= 0) {
    return traits_type::eof(); // remote site has closed connection or (TCP) Receive error
  }

  // move receivd data from eback() .. eback()+size to egptr()-size .. egptr()
  const std::streamsize delta = egptr()-(eback()+size);
  for(std::streambuf::char_type * p = eback() + size - 1; p >= eback(); p--)
    *(p + delta) = *p;

  setg(eback(), egptr() - size, egptr());

  return traits_type::to_int_type(*this->gptr()); // traits::not_eof(...)
}

// setTarget() - set the target socket address
bool dgram_socketbuf::setTarget(const std::string& address, unsigned port,
                                int proto)
{
  if (_socket != INVALID_SOCKET) {
    ::closesocket(_socket);
    _socket = INVALID_SOCKET;
  }

  struct addrinfo req, *ans;
  char portName[32];

  ::sprintf(portName, "%d", port);

  req.ai_flags = 0;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = SOCK_DGRAM;
  req.ai_protocol = proto;
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
}

/// Handle output to a connected socket.
std::streambuf::int_type dgram_socketbuf::overflow(std::streambuf::int_type nCh)
{
  // in case of error, user finds out by testing fail()
  if(_socket==INVALID_SOCKET) {
    return traits_type::eof(); // Invalid socket
  }

  if(pptr()-pbase() <= 0) {
    return 0; // nothing to send
  }

  int size;

  // prepare structure for detecting timeout

  // if a timeout was specified, wait for it.

  if((_overflow_timeout.tv_sec+_overflow_timeout.tv_usec) > 0) {
    int sr;
    timeval tv = _overflow_timeout;
    fd_set socks;
    FD_ZERO(&socks); // zero fd_set
    FD_SET(_socket,&socks); // add buffer socket to fd_set
    sr = ::select(_socket+1,NULL,&socks,NULL,&tv);
    if(sr == 0){
      Timeout = true;
      return traits_type::eof(); // a timeout error should be set here! - RGJ
    } else if(sr < 0) {
      return traits_type::eof(); // error on select()
    }
    assert(FD_ISSET(_socket,&socks));
  }
  Timeout = false;

  // send pending data or return eof() on error
  size=::sendto(_socket, pbase(),pptr()-pbase(),0,(sockaddr*)&out_peer,out_p_size);

  if(size < 0) {
    return traits_type::eof(); // Socket Could not send
  }

  if(size == 0) {
    return traits_type::eof(); // remote site has closed this connection
  }

  if(nCh != traits_type::eof()) {
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
  if(_socket == INVALID_SOCKET) {
    return traits_type::eof(); // Invalid socket!
  }

  if(gptr() < egptr()) {
    return traits_type::to_int_type(*this->gptr());
  }

  // fill up from eback to egptr
  int size;

  // prepare structure for detecting timeout

  // if a timeout was specified, wait for it.
  if((_underflow_timeout.tv_sec+_underflow_timeout.tv_usec) > 0) {
    int sr;
    timeval tv = _underflow_timeout;
    fd_set socks;
    FD_ZERO(&socks); // zero fd_set
    FD_SET(_socket,&socks); // add buffer socket to fd_set
    sr = ::select(_socket+1,&socks,NULL,NULL,&tv);
    if(sr == 0){
      Timeout = true;
      return traits_type::eof(); // a timeout error should be set here! - RGJ
    } else if(sr < 0) {
      return traits_type::eof(); // error on select()
    }
    assert(FD_ISSET(_socket,&socks));
  }
  Timeout = false;


  // receive data or return eof() on error
  in_p_size = sizeof(in_peer);
  size = ::recvfrom(_socket, eback(), egptr()-eback(), 0,
                    (sockaddr*)&in_peer, &in_p_size);

  if(size <= 0) {
    return traits_type::eof(); // remote site has closed connection or (TCP) Receive error
  }

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

basic_socket_stream::basic_socket_stream(socketbuf & buffer, int proto)
    : std::iostream(&buffer), _sockbuf(buffer),
      m_protocol(proto)
{
  init(&_sockbuf); // initialize underlying streambuf
}

basic_socket_stream::~basic_socket_stream()
{
  delete &_sockbuf;
}

SOCKET_TYPE basic_socket_stream::getSocket() const
{
  return _sockbuf.getSocket();
}

void basic_socket_stream::shutdown() {
  SOCKET_TYPE fd = getSocket();
  if(fd != INVALID_SOCKET){
    if(::shutdown(fd, SHUT_RDWR) == SOCKET_ERROR) {
      setLastError();
    }
  }
}

// closes a socket connection
void basic_socket_stream::close() {
  SOCKET_TYPE fd = getSocket();
  if(fd != INVALID_SOCKET){
    if(::closesocket(fd) == SOCKET_ERROR) {
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

static int set_nonblock(int sfd)
{
#ifndef _WIN32
  int flags = ::fcntl(sfd, F_GETFL, 0);
  if (flags == -1) {
    flags = 0;
  }
  return ::fcntl(sfd, F_SETFL, flags | O_NONBLOCK);
#else // _WIN32
  u_long nonblocking = 1;  // This flag may be set elsewhere,
                           // in a header ?
  return ::ioctlsocket(sfd, FIONBIO, &nonblocking);
#endif // _WIN32
}

static int reset_nonblock(int sfd)
{
#ifndef _WIN32
  int flags = ::fcntl(sfd, F_GETFL, 0);
  if (flags == -1) {
    flags = 0;
  }
  return ::fcntl(sfd, F_SETFL, flags & ~O_NONBLOCK);
#else // _WIN32
  u_long blocking = 0;
  return ::ioctlsocket(sfd, FIONBIO, &blocking);
#endif // _WIN32
}

/////////////////////////////////////////////////////////////////////////////
// class stream_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

stream_socket_stream::stream_socket_stream()
    : basic_socket_stream(*new stream_socketbuf(INVALID_SOCKET)),
      _connecting_socket(INVALID_SOCKET),
      stream_sockbuf((stream_socketbuf&)_sockbuf)
{
}

stream_socket_stream::stream_socket_stream(SOCKET_TYPE socket)
    : basic_socket_stream(*new stream_socketbuf(socket)),
      _connecting_socket(INVALID_SOCKET),
      stream_sockbuf((stream_socketbuf&)_sockbuf)
{
}

stream_socket_stream::~stream_socket_stream()
{
  if(_connecting_socket != INVALID_SOCKET) {
    ::closesocket(_connecting_socket);
  }
}

void stream_socket_stream::close()
{
  if(_connecting_socket != INVALID_SOCKET) {
    ::closesocket(_connecting_socket);
    _connecting_socket = INVALID_SOCKET;
  }

  basic_socket_stream::close();
}

SOCKET_TYPE stream_socket_stream::getSocket() const
{
    return (_connecting_socket == INVALID_SOCKET)
            ? basic_socket_stream::getSocket() : _connecting_socket;
}

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

tcp_socket_stream::tcp_socket_stream() :
      _connecting_address(0),
      _connecting_addrlist(0)
{
  m_protocol = FreeSockets::proto_TCP;
}

tcp_socket_stream::tcp_socket_stream(SOCKET_TYPE socket)
    : stream_socket_stream(socket),
      _connecting_address(0),
      _connecting_addrlist(0)
{
  m_protocol = FreeSockets::proto_TCP;
}

tcp_socket_stream::tcp_socket_stream(const std::string& address, int service,
                                     bool nonblock) :
      _connecting_address(0),
      _connecting_addrlist(0)
{
  m_protocol = FreeSockets::proto_TCP;
  open(address, service, nonblock);
}

tcp_socket_stream::tcp_socket_stream(const std::string& address, int service,
                                     unsigned int milliseconds) :
      _connecting_address(0),
      _connecting_addrlist(0)
{
  m_protocol = FreeSockets::proto_TCP;
  open(address, service, milliseconds);
}

tcp_socket_stream::~tcp_socket_stream()
{
  if(_connecting_socket != INVALID_SOCKET) {
    ::freeaddrinfo(_connecting_addrlist);
  }
}

void tcp_socket_stream::open(const std::string & address,
                             int service, bool nonblock)
{
  if (is_open() || _connecting_socket != INVALID_SOCKET) {
    close();
  }

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
  req.ai_protocol = m_protocol;
  req.ai_addrlen = 0;
  req.ai_addr = 0;
  req.ai_canonname = 0;
  req.ai_next = 0;

  int ret;
  if ((ret = ::getaddrinfo(address.c_str(), serviceName, &req, &ans)) != 0) {
    setLastError();
    return;
  }

  bool success = false;
  SOCKET_TYPE sfd = INVALID_SOCKET;

  for(struct addrinfo * i = ans; success == false && i != 0; i = i->ai_next) {

    sfd = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);
    if(sfd == INVALID_SOCKET) {
      continue;
    }

    if(nonblock) {
      int err_val = set_nonblock(sfd);
      if(err_val == -1) {
        setLastError();
        ::closesocket(sfd);
        continue;
      }
    }

    if(::connect(sfd, i->ai_addr, i->ai_addrlen) < 0) {
      if(nonblock && getSystemError() == SOCKET_BLOCK_ERROR) {
        _connecting_socket = sfd;
        _connecting_address = i;
        _connecting_addrlist = ans;
        return;
      }
      setLastError();
      ::closesocket(sfd);
    } else {
      success = true;
    }

  }

  ::freeaddrinfo(ans);

  if (!success) {
    return;
  }

  // set the socket blocking again for io
  if(nonblock) {
    int err_val = reset_nonblock(sfd);
    if(err_val == -1) {
      setLastError();
      ::closesocket(sfd);
      return;
    }
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(sfd);
}

void tcp_socket_stream::open(const std::string & address, int service,
                             unsigned int milliseconds)
{
  open(address, service, true);
  // FIXME add retry logic here
  if(!isReady(milliseconds)) {
    close();
  }
}

int tcp_socket_stream::open_next()
{
  if(_connecting_socket == INVALID_SOCKET ||
     _connecting_addrlist == 0 ||
     _connecting_address == 0) {
    // We can only go on if we are in non-blocking connect already
    return -1;
  }

  ::closesocket(_connecting_socket);
  _connecting_socket = INVALID_SOCKET;

  bool success = false;
  SOCKET_TYPE sfd = INVALID_SOCKET;

  struct addrinfo * i = _connecting_address->ai_next;
  for (; success == false && i != 0; i = i->ai_next) {
    sfd = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);
    if(sfd == INVALID_SOCKET) {
      setLastError();
      continue;
    }
    int err_val = set_nonblock(sfd);
    if(err_val == -1) {
      setLastError();
      ::closesocket(sfd);
      continue;
    }

    if(::connect(sfd, i->ai_addr, i->ai_addrlen) < 0) {
      if(getSystemError() == SOCKET_BLOCK_ERROR) {
        _connecting_socket = sfd;
        _connecting_address = i;
        return 1;
      }
      setLastError();
      ::closesocket(sfd);
    } else {
      success = true;
    }
  }

  assert(_connecting_socket == INVALID_SOCKET);

  ::freeaddrinfo(_connecting_addrlist);
  _connecting_addrlist = 0;
  _connecting_address = 0;

  if (!success) {
    return -1;
  }

  assert(sfd == INVALID_SOCKET);

  // Socket is connected, complete the process

  // set the socket blocking again for io
  int err_val = reset_nonblock(sfd);
  if(err_val == -1) {
    setLastError();
    ::closesocket(sfd);
    return true;
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(sfd);

  return 0;
}

const std::string tcp_socket_stream::getRemoteHost(bool lookup) const
{
  sockaddr_storage peer;
  SOCKLEN peer_size;
  char hbuf[NI_MAXHOST];
  const int flags = lookup ? 0 : NI_NUMERICHOST;

  if (::getpeername(getSocket(), (sockaddr*)&peer, &peer_size) != 0) {
    return "[unconnected]";
  }

  if (::getnameinfo((const sockaddr*)&peer, peer_size,
                    hbuf, sizeof(hbuf), 0, 0, flags) == 0) {
    return std::string(hbuf);
  }
  return "[unknown]";
}

const std::string tcp_socket_stream::getRemoteService(bool lookup) const
{
  char sbuf[NI_MAXSERV];
  sockaddr_storage peer;
  SOCKLEN peer_size;
  const int flags = lookup ? 0 : NI_NUMERICSERV;

  if (::getpeername(getSocket(), (sockaddr*)&peer, &peer_size) != 0) {
    return "[unconnected]";
  }

  if (::getnameinfo((const sockaddr*)&peer, peer_size,
                    0, 0, sbuf, sizeof(sbuf), flags) == 0) {
    return std::string(sbuf);
  }
  return "[unknown]";
}

bool tcp_socket_stream::isReady(unsigned int milliseconds)
{
  if(_connecting_socket == INVALID_SOCKET) {
    return true;
  }

  fd_set wfds;
  fd_set * efdsp = 0;
  struct timeval wait_time = {milliseconds / 1000, (milliseconds % 1000) * 1000};

  FD_ZERO(&wfds);
  FD_SET(_connecting_socket, &wfds);

#ifdef _WIN32
  fd_set efds;
  FD_ZERO(&efds);
  FD_SET(_connecting_socket, &efds);
  
  efdsp = &efds;
#endif // _WIN32

  if (::select(_connecting_socket + 1, 0, &wfds, efdsp, &wait_time) != 1
#ifdef _WIN32
      || !FD_ISSET(_connecting_socket, &efds) && !FD_ISSET(_connecting_socket, &wfds)) {
#else // _WIN32
      || !FD_ISSET(_connecting_socket, &wfds)) {
#endif // _WIN32
    return false;
  }

  // It's done connecting, check for error

  int errnum;
  SOCKLEN errsize = sizeof(errnum);
#ifndef _WIN32
  ::getsockopt(_connecting_socket, SOL_SOCKET, SO_ERROR, &errnum, &errsize);
#else // _WIN32
  ::getsockopt(_connecting_socket, SOL_SOCKET, SO_ERROR, (LPSTR)&errnum, &errsize);
#endif // _WIN32

  // Check for failure, and if it has occured, we need to
  // revisit the address list we got from getaddrinfo.
  assert(_connecting_addrlist != 0);
  assert(_connecting_address != 0);

  if (errnum != 0) {
    return false;
  }

  ::freeaddrinfo(_connecting_addrlist);
  _connecting_addrlist = 0;
  _connecting_address = 0;

  // set the socket blocking again for io
  int err_val = reset_nonblock(_connecting_socket);
  if(err_val == -1) {
    setLastError();
    ::closesocket(_connecting_socket);
    _connecting_socket = INVALID_SOCKET;
    return true;
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(_connecting_socket);
  _connecting_socket = INVALID_SOCKET;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// class dgram_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

dgram_socket_stream::dgram_socket_stream() :
                     basic_socket_stream(*new dgram_socketbuf(INVALID_SOCKET)),
                     dgram_sockbuf((dgram_socketbuf&)_sockbuf)
{
}

dgram_socket_stream::~dgram_socket_stream()
{
}

int dgram_socket_stream::bindToIpService(int service, int type, int protocol)
{
  struct addrinfo req, *ans;
  char serviceName[32];

  ::sprintf(serviceName, "%d", service);

  req.ai_flags = AI_PASSIVE;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = type;
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
    return -1;
  }

  int success = -1;

  for(struct addrinfo * i = ans; success == -1 && i != 0; i = i->ai_next) {
    SOCKET_TYPE socket = ::socket(i->ai_family, i->ai_socktype, i->ai_protocol);
    if (socket == INVALID_SOCKET) {
      setLastError();
      continue;
    }
    dgram_sockbuf.setSocket(socket);

    if (::bind(socket, i->ai_addr, i->ai_addrlen) == SOCKET_ERROR) {
      setLastError();
      close();
    } else {
      success = 0;
    }
  }

  ::freeaddrinfo(ans);

  return success;
}

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

udp_socket_stream::udp_socket_stream()
{
  m_protocol = FreeSockets::proto_UDP;
}

udp_socket_stream::~udp_socket_stream()
{
  // Don't close the main socket, that is done in the basic_socket_stream
  // destructor
}

int udp_socket_stream::open(int service)
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

#include <skstream/skstream_unix.h>

#include <sys/un.h>

/////////////////////////////////////////////////////////////////////////////
// class unix_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////

unix_socket_stream::unix_socket_stream()
{
}

unix_socket_stream::unix_socket_stream(const std::string & address,
                                       bool nonblock)
{
  open(address, nonblock);
}

unix_socket_stream::unix_socket_stream(const std::string & address,
                                       unsigned int milliseconds)
{
  open(address, milliseconds);
}

unix_socket_stream::unix_socket_stream(unix_socket_stream & other,
                                       bool nonblock)
{
  open(other, nonblock);
}

unix_socket_stream::~unix_socket_stream()
{
  // Don't close the main socket, that is done in the basic_socket_stream
  // destructor
  if(_connecting_socket != INVALID_SOCKET) {
    ::closesocket(_connecting_socket);
  }
}

void unix_socket_stream::open(const std::string & address, bool nonblock)
{
  if (address.size() >  107) {
    return;
  }

  if(is_open() || _connecting_socket != INVALID_SOCKET) close();

  // Create socket
  SOCKET_TYPE sfd = ::socket(AF_UNIX, SOCK_STREAM, m_protocol);
  if(sfd == INVALID_SOCKET) {
    setLastError();
    return;
  }

  if(nonblock) {
    int err_val = set_nonblock(sfd);
    if(err_val == -1) {
      setLastError();
      ::closesocket(sfd);
      return;
    }
  }

  // Fill host information
  sockaddr_un sa;
  sa.sun_family = AF_UNIX;
  strncpy(sa.sun_path, address.c_str(), sizeof(sa.sun_path));

  if(::connect(sfd,(sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
    if(nonblock && getSystemError() == SOCKET_BLOCK_ERROR) {
      _connecting_socket = sfd;
      return;
    }
    setLastError();
    ::closesocket(sfd);
    return;
  }

  // set the socket blocking again for io
  if(nonblock) {
    int err_val = reset_nonblock(sfd);
    if(err_val == -1) {
      setLastError();
      ::closesocket(sfd);
      return;
    }
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(sfd);
}

void unix_socket_stream::open(const std::string & address,
                              unsigned int milliseconds)
{
  open(address, true);
  if(!isReady(milliseconds)) {
    close();
  }
}

void unix_socket_stream::open(unix_socket_stream & other, bool nonblock)
{
  // FIXME Create a socketpair connecting the two stream together.
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
  SOCKET_TYPE sfd = _connecting_socket;
  _connecting_socket = INVALID_SOCKET;

  int errnum;
  SOCKLEN errsize = sizeof(errnum);
#ifndef _WIN32
  getsockopt(sfd, SOL_SOCKET, SO_ERROR, &errnum, &errsize);
#else // _WIN32
  Sleep(0);
  getsockopt(sfd, SOL_SOCKET, SO_ERROR, (LPSTR)&errnum, &errsize);
#endif // _WIN32

  if(errnum != 0) {
    // Can't use setLastError(), since errno doesn't have the error
    LastError = errnum;
    ::closesocket(sfd);
    return true;
  }

  // set the socket blocking again for io
  int err_val = reset_nonblock(sfd);
  if(err_val == -1) {
    setLastError();
    ::closesocket(sfd);
    return true;
  }

  // set socket for underlying socketbuf
  _sockbuf.setSocket(sfd);

  return true;
}

#endif // SKSTREAM_UNIX_SOCKETS

/////////////////////////////////////////////////////////////////////////////
// class raw_socket_stream implementation
/////////////////////////////////////////////////////////////////////////////
#ifdef SOCK_RAW

raw_socket_stream::raw_socket_stream(FreeSockets::IP_Protocol proto)
{
  m_protocol = proto;
  SOCKET_TYPE sfd = ::socket(AF_INET, SOCK_RAW, m_protocol);
  _sockbuf.setSocket(sfd);
}

void raw_socket_stream::setProtocol(FreeSockets::IP_Protocol proto) {
  if(is_open()) close();
  m_protocol = proto;
  SOCKET_TYPE sfd = ::socket(AF_INET, SOCK_RAW, m_protocol);
  _sockbuf.setSocket(sfd);
}

raw_socket_stream::~raw_socket_stream()
{
  // Don't close the main socket, that is done in the basic_socket_stream
  // destructor
}

bool raw_socket_stream::setBroadcast(bool opt)
{
  int ok = opt?1:0;
  ok = ::setsockopt(_sockbuf.getSocket(),
                  SOL_SOCKET,SO_BROADCAST,(char*)&ok,sizeof(ok));
  bool ret = (ok != SOCKET_ERROR);
  if(!ret) setLastError();
  return ret;
}

#endif // SOCK_RAW
