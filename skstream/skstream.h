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
#ifndef RGJ_FREE_STREAM_H_
#define RGJ_FREE_STREAM_H_

#include <iostream>

#include <skstream/sksocket.h>

/////////////////////////////////////////////////////////////////////////////
// class socketbuf
/////////////////////////////////////////////////////////////////////////////

/// A base class for stream buffers that handle sockets
class socketbuf : public std::streambuf {
private:
  std::streambuf::char_type *_buffer;

protected:
  SOCKET_TYPE _socket;

  timeval _underflow_timeout;
  timeval _overflow_timeout;

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
  explicit socketbuf(SOCKET_TYPE sock, std::streamsize insize = 0x8000,
                                       std::streamsize outsize = 0x8000);
  /** Make a new socket buffer from an existing socket, with an existing
   *  buffer.
   */
  socketbuf(SOCKET_TYPE sock, std::streambuf::char_type * buf,
                              std::streamsize length);

  /// Destroy the socket buffer.
  virtual ~socketbuf();

  /// Set the existing socket that this buffer should use.
  void setSocket(SOCKET_TYPE sock);

  /// Get the socket that this buffer uses.
  SOCKET_TYPE getSocket() const {
      return _socket; 
  }

  /** Set up a timeout value after which an error flag is set if the socket
   *  is not ready for a read.
   */
  void setReadTimeout(unsigned sec, unsigned usec=0) {
    _underflow_timeout.tv_sec  = sec;
    _underflow_timeout.tv_usec = usec;
  }

  /** Set up a timeout value after which an error flag is set if the socket
   *  is not ready for a write.
   */
  void setWriteTimeout(unsigned sec, unsigned usec=0) {
    _overflow_timeout.tv_sec  = sec;
    _overflow_timeout.tv_usec = usec;
  }

  void setTimeout(unsigned sec, unsigned usec=0) {
    setWriteTimeout(sec, usec);
  }

  /// Return the flag indicating whether a timeout has occured.
  bool timeout() const {
    return Timeout;
  }

protected:
  /// Handle writing data from the buffer to the socket.
  virtual int_type overflow(int_type nCh = traits_type::eof()) = 0;
  /// Handle reading data from the socket to the buffer.
  virtual int_type underflow() = 0;

  /// Flush the output buffer.
  virtual int sync();

  /** Set the buffer area this stream buffer uses. Only works if not already
   *  set.
   */
  std::streambuf * setbuf(std::streambuf::char_type * buf, std::streamsize len);
};

/// A stream buffer class that handles stream sockets
class stream_socketbuf : public socketbuf {
public:
  /** Make a new socket buffer from an existing socket, with optional
   *  buffer sizes.
   */
  explicit stream_socketbuf(SOCKET_TYPE sock,
                            std::streamsize insize = 0x8000,
                            std::streamsize outsize = 0x8000);
  /** Make a new socket buffer from an existing socket, with an existing
   *  buffer.
   */
  stream_socketbuf(SOCKET_TYPE sock, std::streambuf::char_type * buf,
                                     std::streamsize length);

  /// Destroy the socket buffer.
  virtual ~stream_socketbuf();

protected:
  /// Handle writing data from the buffer to the socket.
  virtual int_type overflow(int_type nCh = traits_type::eof());
  /// Handle reading data from the socket to the buffer.
  virtual int_type underflow();

};

/// A stream buffer class that handles datagram sockets
class dgram_socketbuf : public socketbuf {
public:
  /** Make a new socket buffer from an existing socket, with optional
   *  buffer sizes.
   */
  explicit dgram_socketbuf(SOCKET_TYPE sock, std::streamsize insize = 0x8000,
                                             std::streamsize outsize = 0x8000);
  /** Make a new socket buffer from an existing socket, with an existing
   *  buffer.
   */
  dgram_socketbuf(SOCKET_TYPE sock,
                  std::streambuf::char_type * buf,
                  std::streamsize length);

  /// Destroy the socket buffer.
  virtual ~dgram_socketbuf();

  bool setTarget(const std::string& address, unsigned port, int proto);

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

protected:
  /// Target address of datagrams sent via this stream
  sockaddr_storage out_peer;
  /// Source address of last datagram received by this stream
  sockaddr_storage in_peer;
  /// Size of target address
  SOCKLEN out_p_size;
  /// Size of source address
  SOCKLEN in_p_size;

  /// Handle writing data from the buffer to the socket.
  virtual int_type overflow(int_type nCh = traits_type::eof());
  /// Handle reading data from the socket to the buffer.
  virtual int_type underflow();

};

/////////////////////////////////////////////////////////////////////////////
// class socket_stream
/////////////////////////////////////////////////////////////////////////////

/// A base class for iostreams that handles stream sockets
class basic_socket_stream : public basic_socket, public std::iostream {
protected:
  socketbuf & _sockbuf;
  int m_protocol;

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

  virtual SOCKET_TYPE getSocket() const;

  // Needs to be virtual to handle in-progress connect()'s for
  // tcp sockets
  virtual void close();

  void shutdown();

  void setSocket(SOCKET_TYPE sock) {
    _sockbuf.setSocket(sock);
  }

  void setTimeout(unsigned sec, unsigned usec=0) { 
    _sockbuf.setTimeout(sec,usec); 
  }

  int getProtocol() const { 
    return m_protocol; 
  }
};

/////////////////////////////////////////////////////////////////////////////
// class stream_socket_stream
/////////////////////////////////////////////////////////////////////////////

class stream_socket_stream : public basic_socket_stream {
private:
  stream_socket_stream(const stream_socket_stream&);
  stream_socket_stream& operator=(const stream_socket_stream& socket);

protected:
  SOCKET_TYPE _connecting_socket;

  stream_socketbuf & stream_sockbuf;

  stream_socket_stream();
  stream_socket_stream(SOCKET_TYPE socket);
public:
  virtual ~stream_socket_stream();
  
  virtual void close();
  virtual SOCKET_TYPE getSocket() const;

  bool connect_pending() const {
    return (_connecting_socket != INVALID_SOCKET);
  }
};

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream
/////////////////////////////////////////////////////////////////////////////

struct addrinfo;

/// An iostream class that handle TCP sockets
class tcp_socket_stream : public stream_socket_stream {
private:
  tcp_socket_stream(const tcp_socket_stream&);

  tcp_socket_stream& operator=(const tcp_socket_stream& socket);

  struct addrinfo * _connecting_address;
  struct addrinfo * _connecting_addrlist;

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
  int open_next();

  const std::string getRemoteHost(bool lookup = false) const;
  const std::string getRemoteService(bool lookup = false) const;
  bool isReady(unsigned int milliseconds = 0);
};

/// An iostream class that handle IP datagram sockets
class dgram_socket_stream : public basic_socket_stream {
private:
  dgram_socket_stream(const dgram_socket_stream&);

  dgram_socket_stream& operator=(const dgram_socket_stream& socket);

protected:
  dgram_socketbuf & dgram_sockbuf;

  int bindToIpService(int service, int type, int protocol);

public:
  dgram_socket_stream();

  virtual ~dgram_socket_stream();

  bool setTarget(const std::string& address, unsigned port) { 
    return dgram_sockbuf.setTarget(address, port, m_protocol); 
  }

  void setOutpeer(const sockaddr_storage& peer) { 
    return dgram_sockbuf.setOutpeer(peer); 
  }

  const sockaddr_storage & getOutpeer() const { 
    return dgram_sockbuf.getOutpeer(); 
  }

  const sockaddr_storage & getInpeer() const { 
    return dgram_sockbuf.getInpeer(); 
  }

  SOCKLEN getOutpeerSize() const {
    return dgram_sockbuf.getOutpeerSize();
  }

  SOCKLEN getInpeerSize() const {
    return dgram_sockbuf.getInpeerSize();
  }
};


/////////////////////////////////////////////////////////////////////////////
// class udp_socket_stream
/////////////////////////////////////////////////////////////////////////////

/// An iostream class that handle UDP sockets
class udp_socket_stream : public dgram_socket_stream {
private:
  udp_socket_stream(const udp_socket_stream&);

  udp_socket_stream& operator=(const udp_socket_stream& socket);

public:
  udp_socket_stream();

  virtual ~udp_socket_stream();

  int open(int service);
};

#ifdef SOCK_RAW

/////////////////////////////////////////////////////////////////////////////
// class raw_socket_stream
/////////////////////////////////////////////////////////////////////////////

/// An iostream class that handle raw IP sockets
class raw_socket_stream : public dgram_socket_stream {
private:
  raw_socket_stream(const raw_socket_stream&);

  raw_socket_stream& operator=(const raw_socket_stream& socket);

public:
  raw_socket_stream(FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW);

  virtual ~raw_socket_stream();

  void setProtocol(FreeSockets::IP_Protocol proto);

  bool setTarget(const std::string& address, unsigned port) { 
    return dgram_sockbuf.setTarget(address, port, m_protocol); 
  }

  bool setBroadcast(bool opt=false);
};

#endif // SOCK_RAW

#endif // RGJ_FREE_STREAM_H_

