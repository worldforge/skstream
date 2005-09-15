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
 * This software package has been extensively modified by members of the
 * Worldforge Project. See the file ChangeLog for details.
 *
 * $Id$
 *
 */
#ifndef RGJ_FREE_SOCKET_H_
#define RGJ_FREE_SOCKET_H_

#include <iomanip>
#include <string>
#include <stdexcept>
#include <iostream>

#include <skstream/skstreamconfig.h>

// This constant is defined in windows, but not in most other systems
#ifndef SOCKET_ERROR
static const int SOCKET_ERROR = -1;
#endif

// This constant is defined in windows, but not in most other systems
#ifndef INVALID_SOCKET
 #define INVALID_SOCKET   (SOCKET_TYPE)~0
#endif // INVALID_SOCKET

// All systems should define this, but it is here just in case
#ifndef INADDR_NONE
 #warning System headers do not define INADDR_NONE
 #define INADDR_NONE   0xFFFFFFFF
#endif // INADDR_NONE

/////////////////////////////////////////////////////////////////////////////
// class socketbuf
/////////////////////////////////////////////////////////////////////////////
class socketbuf : public std::streambuf {
private:
  char *_buffer;

protected:
  SOCKET_TYPE _socket;

  timeval _timeout;

  sockaddr_storage out_peer, in_peer;
  SOCKLEN out_p_size, in_p_size;

private:
  /// Not implemented. Copying a socket buffer is not permited.
  socketbuf(const socketbuf&);
  /// Not implemented. Copying a socket buffer is not permited.
  socketbuf& operator=(const socketbuf&);

protected:
  bool Timeout;

public:
  /** Make a new socket buffer from an existing socket, with optional
   *  buffer sizes.
   */
  explicit socketbuf(SOCKET_TYPE sock, unsigned insize=0x8000,
                                       unsigned outsize=0x8000);
  /** Make a new socket buffer from an existing socket, with an existing
   *  buffer.
   */
  socketbuf(SOCKET_TYPE sock, char* buf, int length);

  /// Destroy the socket buffer.
  virtual ~socketbuf();

  void setOutpeer(const sockaddr_storage & peer) { 
    out_peer = peer; 
  }

  const sockaddr_storage & getOutpeer() const {
    return out_peer; 
  }

  const sockaddr_storage & getInpeer() const { 
    return in_peer; 
  }

  SOCKLEN getOutpeerSize() const {
    return out_p_size;
  }

  SOCKLEN getInpeerSize() const {
    return in_p_size;
  }

  /// Set the existing socket that this buffer should use.
  void setSocket(SOCKET_TYPE sock);

  /// Get the socket that this buffer uses.
  SOCKET_TYPE getSocket() const {
      return _socket; 
  }

  /** Set up a timeout value after which an error flag is set if the socket
   *  is not ready for a read or write.
   */
  void setTimeout(unsigned sec, unsigned usec=0) {
    _timeout.tv_sec  = sec;
    _timeout.tv_usec = usec;
  }

  /// Return the flag indicating whether a timeout has occured.
  bool timeout() const {
    return Timeout;
  }

protected:
  /// Handle writing data from the buffer to the socket.
  virtual int overflow(int nCh=EOF) = 0;
  /// Handle reading data from the socket to the buffer.
  virtual int underflow() = 0;

  /// Flush the output buffer.
  int sync();

  /** Set the buffer area this stream buffer uses. Only works if not already
   *  set.
   */
  std::streambuf * setbuf(std::streambuf::char_type * buf, std::streamsize len);
};

class stream_socketbuf : public socketbuf {
public:
  /** Make a new socket buffer from an existing socket, with optional
   *  buffer sizes.
   */
  explicit stream_socketbuf(SOCKET_TYPE sock, unsigned insize=0x8000,
                                              unsigned outsize=0x8000);
  /** Make a new socket buffer from an existing socket, with an existing
   *  buffer.
   */
  stream_socketbuf(SOCKET_TYPE sock, char* buf, int length);

  /// Destroy the socket buffer.
  virtual ~stream_socketbuf();

protected:
  /// Handle writing data from the buffer to the socket.
  virtual int overflow(int nCh=EOF);
  /// Handle reading data from the socket to the buffer.
  virtual int underflow();

};

class dgram_socketbuf : public socketbuf {
public:
  /** Make a new socket buffer from an existing socket, with optional
   *  buffer sizes.
   */
  explicit dgram_socketbuf(SOCKET_TYPE sock, unsigned insize=0x8000,
                                             unsigned outsize=0x8000);
  /** Make a new socket buffer from an existing socket, with an existing
   *  buffer.
   */
  dgram_socketbuf(SOCKET_TYPE sock, char* buf, int length);

  /// Destroy the socket buffer.
  virtual ~dgram_socketbuf();

  bool setTarget(const std::string& address, unsigned port, int protocol);

protected:
  /// Handle writing data from the buffer to the socket.
  virtual int overflow(int nCh=EOF);
  /// Handle reading data from the socket to the buffer.
  virtual int underflow();

};

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
// Supported Protocols
namespace FreeSockets {
  enum IP_Protocol {
    proto_IP   = IPPROTO_IP,
    proto_ICMP = IPPROTO_ICMP,
#ifndef _WIN32 
    proto_IGMP = IPPROTO_IGMP, 
#else 
    proto_IGMP = IPPROTO_GGP, 
#endif 
    proto_TCP  = IPPROTO_TCP,
    proto_PUP  = IPPROTO_PUP,
    proto_UDP  = IPPROTO_UDP,
    proto_IDP  = IPPROTO_IDP,
    proto_RAW  = IPPROTO_RAW
  };
  // Well known ports
  enum IP_Service {
    echo        =       7, //
    daytime     =      13, //
    ftp         =      21, //
    ssh         =      22, //
    telnet      =      23, //
    smtp        =      25, //       mail
    time        =      37, //       timserver
    name        =      42, //       nameserver
    nameserver  =      53, //       domain        # name-domain server
    finger      =      79, //
    http        =      80, //
    pop         =     109, //       postoffice
    pop2        =     109, //                     # Post Office
    pop3        =     110, //       postoffice
    nntp        =     119  //       usenet        # Network News Transfer
  };
};

/////////////////////////////////////////////////////////////////////////////
// class basic_socket, a virtual base class for use in polling
/////////////////////////////////////////////////////////////////////////////

class basic_socket {
public:
  virtual ~basic_socket();

  virtual SOCKET_TYPE getSocket() const = 0;
};

/////////////////////////////////////////////////////////////////////////////
// class socket_stream
/////////////////////////////////////////////////////////////////////////////
class basic_socket_stream : public basic_socket, public std::iostream {
protected:
  socketbuf & _sockbuf;
  int protocol;

  mutable int LastError;

  bool startup();

  void setLastError() const;

public:
  /// Make a socket stream.
  basic_socket_stream(socketbuf & buffer, int proto = FreeSockets::proto_IP);

  // Destructor
  virtual ~basic_socket_stream();

  bool fail();

  bool operator!() {
    return fail();
  }

  bool timeout() const {
    return _sockbuf.timeout();
  }

  void setOutpeer(const sockaddr_storage& peer) { 
    return _sockbuf.setOutpeer(peer); 
  }

  const sockaddr_storage & getOutpeer() const { 
    return _sockbuf.getOutpeer(); 
  }

  const sockaddr_storage & getInpeer() const { 
    return _sockbuf.getInpeer(); 
  }

  SOCKLEN getOutpeerSize() const {
    return _sockbuf.getOutpeerSize();
  }

  SOCKLEN getInpeerSize() const {
    return _sockbuf.getInpeerSize();
  }

  bool is_open() const { 
    return ( _sockbuf.getSocket() != INVALID_SOCKET); 
  }

  void setSocket(SOCKET_TYPE sock) { 
    _sockbuf.setSocket(sock); 
  }

  virtual SOCKET_TYPE getSocket() const;

  int getLastError() const { 
    return LastError; 
  }

  // Needs to be virtual to handle in-progress connect()'s for
  // tcp sockets
  virtual void close();

  void shutdown();

  void setTimeout(unsigned sec, unsigned usec=0) { 
    _sockbuf.setTimeout(sec,usec); 
  }

  int getProtocol() const { 
    return protocol; 
  }

  bool setBroadcast(bool opt=false);
};

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream
/////////////////////////////////////////////////////////////////////////////

struct addrinfo;

class tcp_socket_stream : public basic_socket_stream {
private:
  tcp_socket_stream(const tcp_socket_stream&);

  tcp_socket_stream& operator=(const tcp_socket_stream& socket);

  SOCKET_TYPE _connecting_socket;
  struct addrinfo * _connecting_address;
  struct addrinfo * _connecting_addrlist;

  stream_socketbuf & stream_sockbuf;

public:
  tcp_socket_stream();
  tcp_socket_stream(SOCKET_TYPE socket);

  tcp_socket_stream(const std::string& address, int service,
                    bool nonblock = false);

  tcp_socket_stream(const std::string& address, int service,
                    unsigned int milliseconds);

  virtual ~tcp_socket_stream();

  void open(const std::string& address, int service, bool nonblock = false);
  void open(const std::string& address, int service, unsigned int milliseconds);

  virtual void close();
  virtual SOCKET_TYPE getSocket() const;

  const std::string getRemoteHost(bool lookup = false) const;
  const std::string getRemoteService(bool lookup = false) const;
  bool isReady(unsigned int milliseconds = 0);
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_stream
/////////////////////////////////////////////////////////////////////////////
class udp_socket_stream : public basic_socket_stream {
private:
  udp_socket_stream(const udp_socket_stream&);
  udp_socket_stream(SOCKET_TYPE socket);

  udp_socket_stream& operator=(const udp_socket_stream& socket);

  dgram_socketbuf & dgram_sockbuf;

public:
  udp_socket_stream();

  virtual ~udp_socket_stream();

  bool setTarget(const std::string& address, unsigned port) { 
    return dgram_sockbuf.setTarget(address, port, protocol); 
  }
};

/////////////////////////////////////////////////////////////////////////////
// class raw_socket_stream
/////////////////////////////////////////////////////////////////////////////
#ifdef SOCK_RAW
class raw_socket_stream : public basic_socket_stream {
private:
  raw_socket_stream(const raw_socket_stream&);
  raw_socket_stream(SOCKET_TYPE socket);

  raw_socket_stream& operator=(const raw_socket_stream& socket);

protected:
  sockaddr_storage local_host;
  dgram_socketbuf & dgram_sockbuf;

public:
  raw_socket_stream(FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW);

  raw_socket_stream(unsigned insize,unsigned outsize,
                    FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW);

  virtual ~raw_socket_stream();

  void setProtocol(FreeSockets::IP_Protocol proto);

  bool setTarget(const std::string& address, unsigned port) { 
    return dgram_sockbuf.setTarget(address, port, protocol); 
  }

  sockaddr_storage getLocalHost() const { 
    return local_host; 
  }
};
#endif // SOCK_RAW

#endif // RGJ_FREE_SOCKET_H_

