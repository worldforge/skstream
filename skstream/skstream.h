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
#ifndef RGJ_FREE_SOCKET_H_
#define RGJ_FREE_SOCKET_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

#ifdef _WIN32
  #include <streambuf>

  #include <winsock.h>

  typedef SOCKET SOCKET_TYPE;

  #define SOCKLEN int 

#endif

#ifdef __linux__
  #include <streambuf.h>

  #include <sys/time.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
  #include <errno.h>

  #define INVALID_SOCKET (SOCKET_TYPE)(~0)
  #define SOCKET_ERROR   -1

  #define SOCKLEN socklen_t

  #define IPPORT_RESERVED 1024

  #define closesocket(x) close(x)

  typedef int SOCKET_TYPE;
#endif

#ifdef __BEOS__
  #include <net/socket.h>
  #include <net/netdb.h>
  #include <errno.h>

  #define INVALID_SOCKET (SOCKET_TYPE)(~0)
  #define SOCKET_ERROR   -1

  #define SOCKLEN socklen_t

  #define IPPORT_RESERVED 1024
  #define INADDR_NONE	  0xFFFFFF

  typedef int SOCKET_TYPE;
#endif

/////////////////////////////////////////////////////////////////////////////
// class socketbuf
/////////////////////////////////////////////////////////////////////////////
class socketbuf : public std::streambuf {
private:
  char *_buffer;

  SOCKET_TYPE _socket;

  timeval _timeout;

  sockaddr_in out_peer, in_peer;

  socketbuf(const socketbuf&);
  socketbuf& operator=(const socketbuf&);

  bool Timeout;

public:
  // Contructors
  socketbuf(SOCKET_TYPE sock, unsigned insize=0x8000, unsigned outsize=0x8000);
  socketbuf(SOCKET_TYPE sock, char* buf, int length);

  virtual ~socketbuf();

  bool setOutpeer(const std::string& address, unsigned port);
  bool setOutpeer(const sockaddr_in& peer)
    { out_peer = peer; return true; }
  sockaddr_in getOutpeer() const
    { return out_peer; }

  sockaddr_in getInpeer() const
    { return in_peer; }

  void setSocket(SOCKET_TYPE sock) {
    _socket = sock;
    int size = sizeof(sockaddr);
    ::getpeername(sock,(sockaddr*)&out_peer,(SOCKLEN*)&size);
    in_peer = out_peer;
  }

  SOCKET_TYPE getSocket() const { return _socket; }

  void setTimeout(unsigned sec, unsigned usec=0) {
    _timeout.tv_sec  = sec;
    _timeout.tv_usec = usec;
  }

  bool timeout() const {
    return Timeout;
  }

protected:
  virtual int overflow(int nCh=EOF);
  virtual int underflow();

  virtual int sync() {
    if(overflow() == EOF) // traits::eof()
      return EOF;	// ios will set the fail bit // traits::eof()
//    else {
//      // empty put and get areas
//      setp(pbase(), epptr());
//      setg(eback(), egptr(), egptr());
//
//      return 0; // traits::not_eof(0);
//    }
  }

  virtual std::streambuf* setbuf(char* buf, long len) {
    if((buf != NULL) && (len > 0)) {
      _buffer = buf;
      setp(_buffer, _buffer+(len >> 1));
      setg(_buffer+(len >> 1), _buffer+len, _buffer+len);
    }
    return this;
  }
};


/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
// Supported Protocols
namespace FreeSockets {
  enum IP_Protocol {
    proto_IP   = IPPROTO_IP,
    proto_ICMP = IPPROTO_ICMP,
    proto_IGMP = IPPROTO_IGMP,
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
// class socket_stream
/////////////////////////////////////////////////////////////////////////////
class basic_socket_stream : public std::iostream {
protected:
  socketbuf _sockbuf;
  int protocol;

  mutable int LastError;

  bool startup();
  bool shutdown();

  void setLastError() const;

public:
  // Constructors
  basic_socket_stream()
      : _sockbuf(INVALID_SOCKET), protocol(FreeSockets::proto_IP), LastError(0)
  {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }
  basic_socket_stream(unsigned insize,unsigned outsize,
                      int proto=FreeSockets::proto_IP)
      : _sockbuf(INVALID_SOCKET,insize,outsize),
        protocol(proto), LastError(0)
  {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }
  basic_socket_stream(SOCKET_TYPE sock)
      : _sockbuf(sock), protocol(FreeSockets::proto_IP), LastError(0) {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }
  basic_socket_stream(SOCKET_TYPE sock,
                      unsigned insize,unsigned outsize)
      : _sockbuf(sock,insize,outsize),
        protocol(FreeSockets::proto_IP), LastError(0) {
    startup();
    init(&_sockbuf); // initialize underlying streambuf
  }
  // Destructor
  virtual ~basic_socket_stream() {
    close();
    shutdown();
  }

  virtual bool operator!()
   { return fail(); }

  virtual bool fail() {
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

  virtual bool timeout() const {
    return _sockbuf.timeout();
  }

  bool setOutpeer(const std::string& address, unsigned port)
    { return _sockbuf.setOutpeer(address,port); }
  bool setOutpeer(const sockaddr_in& peer)
    { return !_sockbuf.setOutpeer(peer); }
  sockaddr_in getOutpeer() const
    { return _sockbuf.getOutpeer(); }

  sockaddr_in getInpeer() const
    { return _sockbuf.getInpeer(); }

  bool is_open() const
    { return ( getSocket() != INVALID_SOCKET); }

  void setSocket(SOCKET_TYPE sock)
    { _sockbuf.setSocket(sock); }

  SOCKET_TYPE getSocket() const
    { return _sockbuf.getSocket(); }

  std::string getRemoteHost() const {
    return std::string(::inet_ntoa(getInpeer().sin_addr));
  }

  unsigned short getRemotePort() const {
    return ntohs(getInpeer().sin_port);
  }

  int getLastError() const { return LastError; }

  void close();

  void setTimeout(unsigned sec, unsigned usec=0)
    { _sockbuf.setTimeout(sec,usec); }

  int getProtocol() const
    { return protocol; }

  bool setBroadcast(bool opt=false) {
    int ok = opt?1:0;
    ok = setsockopt(_sockbuf.getSocket(),
                    SOL_SOCKET,SO_BROADCAST,(char*)&ok,sizeof(ok));
    return (ok != SOCKET_ERROR);
  }
};

/////////////////////////////////////////////////////////////////////////////
// manipulator set_host
/////////////////////////////////////////////////////////////////////////////
class remote_host {
protected:
  std::string _host;
  unsigned _port;
public:
  remote_host(const std::string& host,unsigned port)
    : _host(host), _port(port) {}
  friend
    basic_socket_stream& operator<<(basic_socket_stream&, const remote_host&);
};

inline
basic_socket_stream& operator<<(basic_socket_stream& out,const remote_host& host)
{
  out.setOutpeer(host._host,host._port);
  return out;
}

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_stream
/////////////////////////////////////////////////////////////////////////////
class tcp_socket_stream : public basic_socket_stream {
private:
  tcp_socket_stream(const tcp_socket_stream&);
  tcp_socket_stream& operator=(const tcp_socket_stream& socket);

  tcp_socket_stream(SOCKET_TYPE socket);
public:
  tcp_socket_stream() : basic_socket_stream() {
    protocol = FreeSockets::proto_TCP;
  }
  tcp_socket_stream(const std::string& ad, int s) : basic_socket_stream() {
    protocol = FreeSockets::proto_TCP;
    open(ad, s);
  }

  virtual ~tcp_socket_stream() { shutdown(); }

  void open(const std::string& addr, int service);
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_stream
/////////////////////////////////////////////////////////////////////////////
class udp_socket_stream : public basic_socket_stream {
private:
  udp_socket_stream(const udp_socket_stream&);
  udp_socket_stream& operator=(const udp_socket_stream& socket);

  udp_socket_stream(SOCKET_TYPE socket);

public:
  udp_socket_stream(FreeSockets::IP_Protocol proto=FreeSockets::proto_UDP)
     : basic_socket_stream()
  {
    protocol = proto;
    SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_DGRAM, protocol);
    _sockbuf.setSocket(_socket);
  }

  virtual ~udp_socket_stream() { shutdown(); }
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
  sockaddr_in local_host;

public:
  raw_socket_stream(FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW)
    : basic_socket_stream()
  {
    protocol = proto;
    SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
    _sockbuf.setSocket(_socket);
  }

  raw_socket_stream(unsigned insize,unsigned outsize,
                    FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW)
    : basic_socket_stream(insize,outsize)
  {
    protocol = proto;
    SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
    _sockbuf.setSocket(_socket);
  }

  virtual ~raw_socket_stream() { shutdown(); }

  void setProtocol(FreeSockets::IP_Protocol proto);

  sockaddr_in getLocalHost() const { return local_host; }
};
#endif // SOCK_RAW

#endif

