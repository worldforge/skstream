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
 * Revision 1.17  2002-06-12 00:38:24  rsteinke
 *     -Modified getSocket() so you can poll on the writability
 *      of a tcp_socket_stream during a nonblocking connect,
 *      instead of calling is_ready() in a timeout. You still
 *      have to call is_ready() when the poll returns.
 *
 * Revision 1.16  2002/06/12 00:27:40  rsteinke
 *     -Fixed many bugs in nonblocking tcp socket connect code
 *
 * Revision 1.15  2002/05/27 10:53:09  mkoch
 * 05/27/2002 Michael Koch <konqueror@gmx.de>
 *     -Add support for FreeBSD.
 *
 * Revision 1.14  2002/05/21 07:29:36  malcolm
 * Added rsteinke's nonblocking connect patch.  Works on linux; does not break API
 * (I bumped version to 0.2.3 anyway).  May not work on win32, though I did test it
 * and socket communication does happen.
 *
 * Revision 1.13  2002/05/16 18:10:54  SpeedBump
 * added detection code for Solaris.  tested with 2.6.  needs testing on further
 * platforms.
 * also added TODO item to use configure tests instead of preprocessor tests in
 * skstream.h
 *
 * Revision 1.12  2002/04/14 22:50:46  xmp
 * one liner change to define sorting out cygwin compiles
 *
 * Revision 1.11  2002/04/08 20:02:00  xmp
 * Just a few fixes to MSVC support, removing a few unnessesary std::'s and shifting a default argument to a header.
 *
 * Revision 1.10  2002/04/08 19:47:12  malcolm
 * Changes to allow clean compilation on Microsoft Visual C++.  I must go and
 * boil my keyboard now.  Constructor code for basic_socket_stream moved out of
 * header and into .cpp.
 *
 * MS Project files are untested (I edited them!  Oh no!)
 *
 * Revision 1.9  2002/03/18 07:36:25  malcolm
 * Add superclass constructor code to *_socket_stream classes
 * (Necessary for compilation with gcc3)
 *
 * Revision 1.8  2002/03/18 07:31:50  malcolm
 * Code necessary for mingw32 compilation.
 *
 * Revision 1.7  2002/02/26 20:33:55  grimicus
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
 * Revision 1.6  2002/02/24 03:15:41  grimicus
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
 * Revision 1.5  2002/02/21 05:11:15  grimicus
 * 2002-02-20 Dan Tomalesky <grim@xynesis.com>
 *    * Added a new test case header for basic_socket_streams
 *
 *    * Made a mod to basic_socket_stream::setOutpeer(sockaddr_in&)
 *      seemed to me it had an erroneous ! in the return
 *
 *    * Oh, and I reformatted skstream.h so its not so damn hard to look at
 *
 * Revision 1.4  2002/02/20 05:04:07  grimicus
 * 2002-02-19 Grimicus <grim@xynesis.com>
 *
 *    * updated socketbuf(SOCKET_TYPE, unsigned, unsigned) to have variable
 *      names in the declaration so it was a bit easier to look at. (and know
 *      what the heck they were suppose to be for)
 *
 *    * Added some tests in for socketbuf.  Not very good ones but its a start
 *
 * Revision 1.3  2002/02/19 22:04:31  grimicus
 * 2002-02-19 Grimicus <grim@xynesis.com>
 *
 *   * Added License header to skstream.h
 *
 *   * Added cppunit TestCase  classes in test/ and auto* support functionality
 *
 * Revision 1.2  2002/02/15 18:24:24  grimicus
 * 2002-02-15 Grimicus <grim@xynesis.com>
 *
 *   * Modified skstream.h so that it is in the correct format for CVS
 *     (was checked in as a dos file instead of *nix, so it had funky
 *     control characters on the end...plays havok with some editors and such)
 *
 * Revision 1.1  2002/01/07 23:02:08  rykard
 * Adding the new version of skstream/FreeSockets to cvs.  
 * Note there are some API changes and new features in this version, so I 
 * didn't just commit over the older one.
 */
#ifndef RGJ_FREE_SOCKET_H_
#define RGJ_FREE_SOCKET_H_

#include <iomanip>
#include <string>
#include <stdexcept>

#if defined( _WIN32 ) || defined( __CYGWIN32__ )
  #ifdef _MSC_VER
    #include <streambuf>
  #else
    #include <streambuf.h>
  #endif

  #include <winsock.h>

//SOCKET_TYPE is used to define the socket in each of the different OSes 
//that skstream supports.  This is because it is not guaranteed how any
//will handle the socket.  Currently, they all use an integer file descriptor
//type thing, but windows is not to be trusted.  SOCKET_TYPE does *NOT* stand
//for the various 'types' of socket connections (SOCK_STREAM, SOCK_DGRAM, etc)
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

//SOCKET_TYPE is used to define the socket in each of the different OSes 
//that skstream supports.  This is because it is not guaranteed how any
//will handle the socket.  Currently, they all use an integer file descriptor
//type thing, but windows is not to be trusted.  SOCKET_TYPE does *NOT* stand
//for the various 'types' of socket connections (SOCK_STREAM, SOCK_DGRAM, etc)
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

//SOCKET_TYPE is used to define the socket in each of the different OSes 
//that skstream supports.  This is because it is not guaranteed how any
//will handle the socket.  Currently, they all use an integer file descriptor
//type thing, but windows is not to be trusted.  SOCKET_TYPE does *NOT* stand
//for the various 'types' of socket connections (SOCK_STREAM, SOCK_DGRAM, etc)
  typedef int SOCKET_TYPE;
#endif

#ifdef __sun__
  #include <sys/socket.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <errno.h>
  #include <unistd.h>

  #define INVALID_SOCKET (SOCKET_TYPE)(~0)
  #define SOCKET_ERROR   -1

  #define SOCKLEN int

  #define IPPORT_RESERVED 1024
  #define INADDR_NONE	  0xFFFFFFFF

  #define closesocket(x) close(x)
  
//SOCKET_TYPE is used to define the socket in each of the different OSes 
//that skstream supports.  This is because it is not guaranteed how any
//will handle the socket.  Currently, they all use an integer file descriptor
//type thing, but windows is not to be trusted.  SOCKET_TYPE does *NOT* stand
//for the various 'types' of socket connections (SOCK_STREAM, SOCK_DGRAM, etc)
  typedef int SOCKET_TYPE;
#endif

#ifdef __FreeBSD__
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <errno.h>

  #define INVALID_SOCKET (SOCKET_TYPE)(~0)
  #define SOCKET_ERROR   -1

  #define SOCKLEN socklen_t

  #define IPPORT_RESERVED 1024

  #define closesocket(x) close(x)

  typedef int SOCKET_TYPE;
#endif

#ifndef SOCKLEN
  #error "no support for target os"
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

  bool setOutpeer(const sockaddr_in& peer) { 
    out_peer = peer; 
    return true; 
  }

  sockaddr_in getOutpeer() const {
    return out_peer; 
  }

  sockaddr_in getInpeer() const { 
    return in_peer; 
  }

  void setSocket(SOCKET_TYPE sock) {
    _socket = sock;
    int size = sizeof(sockaddr);
    ::getpeername(sock,(sockaddr*)&out_peer,(SOCKLEN*)&size);
    in_peer = out_peer;
  }

  SOCKET_TYPE getSocket() const {
      return _socket; 
  }

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
    else {
//      // empty put and get areas
//      setp(pbase(), epptr());
//      setg(eback(), egptr(), egptr());
//
      return 0; // traits::not_eof(0);
    }
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
  basic_socket_stream();
  basic_socket_stream(unsigned insize,unsigned outsize,
                      int proto=FreeSockets::proto_IP);
  basic_socket_stream(SOCKET_TYPE sock);
  basic_socket_stream(SOCKET_TYPE sock,
		  unsigned insize,unsigned outsize);

  // Destructor
  virtual ~basic_socket_stream() {
    shutdown();
  }

  virtual bool fail();

  virtual bool operator!() { 
    return fail(); 
  }

  virtual bool timeout() const {
    return _sockbuf.timeout();
  }

  bool setOutpeer(const std::string& address, unsigned port) { 
    return _sockbuf.setOutpeer(address,port); 
  }

  bool setOutpeer(const sockaddr_in& peer) { 
    /*%NOTE(Grimicus, skstream2, FYI)
     * removed a ! right before _socketbuf because it
     *didn't seem to be consistent with the other setOutpeer()
     */
    return _sockbuf.setOutpeer(peer); 
  }

  sockaddr_in getOutpeer() const { 
    return _sockbuf.getOutpeer(); 
  }

  sockaddr_in getInpeer() const { 
    return _sockbuf.getInpeer(); 
  }

  bool is_open() const { 
    return ( _sockbuf.getSocket() != INVALID_SOCKET); 
  }

  void setSocket(SOCKET_TYPE sock) { 
    _sockbuf.setSocket(sock); 
  }

  virtual SOCKET_TYPE getSocket() const { 
    return _sockbuf.getSocket(); 
  }

  std::string getRemoteHost() const {
    return std::string(::inet_ntoa(getInpeer().sin_addr));
  }

  unsigned short getRemotePort() const {
    return ntohs(getInpeer().sin_port);
  }

  int getLastError() const { 
    return LastError; 
  }

  // Needs to be virtual to handle in-progress connect()'s for
  // tcp sockets
  virtual void close();

  void setTimeout(unsigned sec, unsigned usec=0) { 
    _sockbuf.setTimeout(sec,usec); 
  }

  int getProtocol() const { 
    return protocol; 
  }

  bool setBroadcast(bool opt=false) {
    int ok = opt?1:0;
    ok = setsockopt(_sockbuf.getSocket(),
                    SOL_SOCKET,SO_BROADCAST,(char*)&ok,sizeof(ok));
    bool ret = (ok != SOCKET_ERROR);
    if(!ret) setLastError();
    return ret;
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
  tcp_socket_stream(SOCKET_TYPE socket);

  tcp_socket_stream& operator=(const tcp_socket_stream& socket);

  SOCKET_TYPE _connecting_socket;

public:
  tcp_socket_stream() : basic_socket_stream(), _connecting_socket(INVALID_SOCKET) {
    protocol = FreeSockets::proto_TCP;
  }

  tcp_socket_stream(const std::string& address, int service, bool nonblock = false) : 
      basic_socket_stream(), _connecting_socket(INVALID_SOCKET) {
    protocol = FreeSockets::proto_TCP;
    open(address, service, nonblock);
  }

  virtual ~tcp_socket_stream() { 
      shutdown(); 
      if(_connecting_socket != INVALID_SOCKET)
#ifndef _WIN32
        ::close(_connecting_socket);
#else
        ::closesocket(_connecting_socket);
#endif
  }

  void open(const std::string& address, int service, bool nonblock = false);
  virtual void close();
  virtual SOCKET_TYPE getSocket() {return (_connecting_socket == INVALID_SOCKET)
    ? basic_socket_stream::getSocket() : _connecting_socket;}

  bool is_ready();
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_stream
/////////////////////////////////////////////////////////////////////////////
class udp_socket_stream : public basic_socket_stream {
private:
  udp_socket_stream(const udp_socket_stream&);
  udp_socket_stream(SOCKET_TYPE socket);

  udp_socket_stream& operator=(const udp_socket_stream& socket);


public:
  udp_socket_stream(FreeSockets::IP_Protocol proto=FreeSockets::proto_UDP)
     : basic_socket_stream() {
    protocol = proto; 
    SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_DGRAM, protocol);
    _sockbuf.setSocket(_socket);
  }

  virtual ~udp_socket_stream() {
    shutdown(); 
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
  sockaddr_in local_host;

public:
  raw_socket_stream(FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW) 
    : basic_socket_stream() {
    protocol = proto;
    SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
    _sockbuf.setSocket(_socket);
  }

  raw_socket_stream(unsigned insize,unsigned outsize,
                    FreeSockets::IP_Protocol proto=FreeSockets::proto_RAW)
    : basic_socket_stream(insize,outsize) {
    protocol = proto;
    SOCKET_TYPE _socket = ::socket(AF_INET, SOCK_RAW, protocol);
    _sockbuf.setSocket(_socket);
  }

  virtual ~raw_socket_stream() {
      shutdown(); 
  }

  void setProtocol(FreeSockets::IP_Protocol proto);

  sockaddr_in getLocalHost() const { 
    return local_host; 
  }
};
#endif // SOCK_RAW

#endif

