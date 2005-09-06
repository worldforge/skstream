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
 * Revision 1.49  2005-09-06 16:51:20  alriddoch
 * 2005-09-06  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.h, skstream/skstream.cpp: Fix arguments to
 * 	  socketbuf::setbuf so they really match the function it should
 * 	  be overloading. Add the required return value, and return this.
 *
 * Thanks to Jonathan Phenix for originally reporting the bug, and for
 * providing the initial patch to fix it.
 *
 * Revision 1.48  2005/07/01 23:35:54  alriddoch
 * 2005-07-01  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.h, skstream/skstream.cpp: Make functions
 * 	  non-virtual if they don't require it. Get rid of unused return
 * 	  value for setbuf(). Move some functionality into
 * 	  socketbuf::setSocket(). Make sure socket is valid before getting
 * 	  its peer name.
 *
 * Revision 1.47  2005/03/03 15:46:05  alriddoch
 * 2004-03-03  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.h, skstream/skstream.cpp: Add a virtual
 * 	  destructor to basic_socket interface.
 *
 * Revision 1.46  2004/11/29 22:47:30  alriddoch
 * 2004-11-29  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.h, skstream/skstream.cpp: Inline operator!
 * 	  and timeout as they don't need to be virtual. Don't inline
 * 	  open() as its too big. Make setOutpeer return void, as the returned
 * 	  bool value was useless and ignored.
 *
 * Revision 1.45  2004/11/24 00:50:37  alriddoch
 * 2004-11-24  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skstream.cpp, skstream/skstream.h, skstream/skstream_unix.h:
 * 	  Re-work the way stream buffer objects are created so they are
 * 	  allocated in the super class using new, and passed to the base
 * 	  where they are deleted in the destructor. This ensures that the
 * 	  stream buffer is never used un-initialised, or after destruction.
 * 	  Move some lengthy constructors into the .cpp file.
 *
 * 	* configure.ac: Increment version and interface so applications can
 * 	  ensure they get the updated API.
 *
 * Revision 1.44  2004/11/23 01:22:24  alriddoch
 * 2004-11-23  Al Riddoch  <alriddoch@zepler.org>
 *
 * 	* skstream/skserver.cpp, skstream/skserver.h,
 * 	  skstream/skstream.cpp, skstream/skstream.h:
 * 	  Re-purpose the shutdown() method of various classes
 * 	  so it no longer closes the socket. This makes it more consistent
 * 	  and sorts out some issues with using epoll() with skstream.
 * 	  Make sure close() is called in the right places in destructors.
 * 	  Fix a bug in the Win32 build where WSACleanup() was getting
 * 	  called at utterly the wrong time.
 *
 * Revision 1.43  2003/09/28 00:53:54  alriddoch
 *  2003-09-27 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h: Add accessors so address sizes in buffer class
 *       are accessible from outside stream class.
 *     - ping/ping.cpp: Re-work code for handling conversion of addresses
 *       to presentation format for protocol independance.
 *
 * Revision 1.42  2003/09/26 22:26:44  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - Add option to get streams remote details as reverse lookup, rather
 *       than just presentation form.
 *
 * Revision 1.41  2003/09/26 10:49:03  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp: Add code that handles converting service to
 *       presentation format in a protocol independant way.
 *
 * Revision 1.40  2003/09/26 01:29:41  alriddoch
 *  2003-09-26 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h: Add accessors for address size members.
 *     - ping/ping.cpp: Clean up use of libc and system calls.
 *     - skstream/skstream.cpp: Clean up handling socket address sizes
 *       when setting socket by descriptor in the buffer objects.
 *       Add code that handles converting address to presentation format
 *       in a protocol independant way.
 *
 * Revision 1.39  2003/09/24 01:05:46  alriddoch
 *  2003-09-24 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp: Re-write connecting
 *       code for IP datagram and stream sockets so they try each of the
 *       results from getaddrinfo in turn. Needs work on non-blocking
 *       sockets.
 *
 * Revision 1.38  2003/09/08 17:05:36  xmp
 *  2003-09-06 Martin Pollard <circlemaster@blueyonder.co.uk>
 *     - configure.in: changed win32 code to use winsock2.h instead of winsock.h.
 *     - skstream/skstream.h: added a guard #ifndef SOCKET_ERROR to it's definition
 * .
 *
 * Revision 1.37  2003/08/25 17:18:28  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp: Add extra argument
 *       to dgram_streambuf::setTarget() so protocol can be passed in.
 *       Add #warnings if the old non getaddrinfo code is being used.
 *
 * Revision 1.36  2003/08/23 22:13:55  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstreamconfig.h.in, skstream/skstream_unix.h,
 *       skstream/skstream.h, skstream/skstream.cpp,
 *       skstream/skserver_unix.h, skstream/skserver.h, skstream/skserver.cpp:
 *       Remove as much platform sensitive code from the headers as possible,
 *       and simplify the generated header. Improve formatting and header
 *       guards for readability.
 *     - skstream/skserver_unix.h: Add a new constructor for unix_socket_server
 *       so it can be instanced without already knowing the details of the
 *       service.
 *
 * Revision 1.35  2003/08/23 20:19:46  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp: Modify udp_socket_stream
 *       constructor so that it does not create the socket immediatly.
 *     - skstream/skstream.cpp: Re-write dgram_socketbuf::setTarget() so it
 *       uses getaddrinfo to create the socket, and get the address in
 *       a protocol independant way.
 *
 * Revision 1.34  2003/07/30 23:17:55  alriddoch
 *  2003-07-30 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.cpp, skstream/skserver.h, skstream/skstream.cpp,
 *       skstream/skstream.h, skstream/skstream_unix.h: Move virtual
 *       function implementations into .cpp files.
 *
 * Revision 1.33  2003/07/23 22:55:43  alriddoch
 *  2003-07-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h: Fix header for windows build.
 *
 * Revision 1.32  2003/07/23 17:00:29  alriddoch
 *  2003-07-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstreamconfig.h.pbx, skstream/skstreamconfig.h.in,
 *       skstream/skstreamconfig.h.windows: Removed some defines which
 *       are the same on all platforms, and put them in the main header.
 *       Avoid streambuf header as it varies on some systems, and rely
 *       on iostream instead.
 *     - Remove the streambuf header test.
 *     - skstream/skstream.h: Remove the old platform specific code,
 *       take on some changes from the skstreamconfig header, and clean
 *       up some comments.
 *     - skstream/skstream.cpp: Re-write the resolver code for platforms
 *       without getaddrinfo() so it is cleaner, and covers more cases.
 *
 * Revision 1.31  2003/05/06 21:53:11  alriddoch
 *  2003-05-06 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.h, skstream/skstream.cpp, skstream_unix.h:
 *       Re-work basic_socket_stream so it can have either stream or datagram
 *       buffers.
 *     - ping/ping.cpp, ping/ping.h, test/basicskstreamtest.h,
 *       test/childskstreamtest.h, test/skservertest.h: Get the tests and examples
 *       building again.
 *
 * Revision 1.30  2003/05/04 00:34:30  alriddoch
 *  2003-05-04 Al Riddoch <alriddoch@zepler.org>,
 *     - Add a second pkgconfig file for apps that need unix socket support.
 *     - Rename sksystem.h as skstreamconfig.h, and put it in an architecture
 *       dependent subdir. Modify pkgconfig files to handle this.
 *     - Handle errors in a more general way to reduce the number
 *       of places where pre-processor conditionals are required.
 *     - Remove some redundant code for dealing with win32 that has
 *       now been superceded.
 *
 * Revision 1.29  2003/04/16 14:02:35  alriddoch
 *  2003-04-16 Al Riddoch <alriddoch@zepler.org>,
 *     - Fix up includes so they work properlly
 *
 * Revision 1.28  2003/03/18 22:46:45  alriddoch
 *  2003-03-18 Al Riddoch <alriddoch@zepler.org>,
 *     - Improve safety of some constructors, and move IP (host and port)
 *       related functions into apropriate classes.
 *     - Add in unix sockets, based on a configure check.
 *
 * Revision 1.27  2003/03/17 23:37:04  alriddoch
 *  2003-03-17 Al Riddoch <alriddoch@zepler.org>,
 *     - Start handling portability with configure checks.
 *
 * Revision 1.26  2003/03/14 19:33:11  alriddoch
 *  2003-03-14 Al Riddoch <alriddoch@zepler.org>,
 *     - skstream.h, skstream.cpp: Re-work sockbuf class so it is
 *       not specific to any one type of socket. Remove inet
 *       functionality from base classes. Derive new buffer
 *       classes for stream and datagram sockets, and use
 *       stream version in current socket classes. This should
 *       be considered work in progress. Do not port your code.
 *
 * Revision 1.25  2002/12/09 22:13:21  rsteinke
 *     - created basic_socket, a virtual base class
 *       for basic_socket_stream and basic_socket_server,
 *       so that the polling code has a common base
 *       with getSocket() to access
 *     - created basic_socket_poll polling class
 *     - changed tcp_socket_stream::is_ready() to isReady()
 *       to match standards for function naming, left is_ready()
 *       as a wrapper of isReady() for compatibility
 *
 * Revision 1.24  2002/11/04 22:49:15  alriddoch
 *  2002-11-04 Al Riddoch <alriddoch@zepler.org>,
 *     - Add doxygen docs.
 *
 * Revision 1.23  2002/10/28 10:57:24  mkoch
 *  20/28/2002 Michael Koch <konqueror@gmx.de>
 *     - Added support for GNU/Hurd.
 *     - Added man page for skstream-config.
 *     - Added support for NOCONFIGURE in autogen.sh
 *
 * Revision 1.22  2002/07/15 21:21:17  alriddoch
 *  07/15/2002 Al Riddoch <alriddoch@zepler.org>,
 *     - skstream.cpp: Handle nonblocking connect in win32
 *     - skstream.h: Include correct streambuf header on Linux.
 *
 * Revision 1.21  2002/07/04 10:23:56  jmt
 *  07/03/2002 James Turner <james@worldforge.org>
 * 	-Added configuration case for Darwin / OS-X
 * 	-Fixed Rsteinke's changes to use typedefs for socklen / errnum
 *
 * Revision 1.20  2002/06/30 11:01:11  rsteinke
 * Missing const in prototype to override virtual function
 *
 * Revision 1.19  2002/06/12 16:06:58  rsteinke
 *     -Added a timeout version of the tcp_socket_stream
 *      constructor, which wraps the timeout version
 *      of open()
 *
 * Revision 1.18  2002/06/12 01:21:47  rsteinke
 *     -Added an optional "milliseconds" argument to
 *      tcp_socket_stream::is_ready(), to take advantage
 *      of the timeout in select()
 *
 * Revision 1.17  2002/06/12 00:38:24  rsteinke
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

