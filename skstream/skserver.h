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
 * Revision 1.9  2003-08-23 14:01:57  alriddoch
 *  2003-08-23 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.h, skstream/skserver_unix.h: Add default constructor
 *       to tcp_socket_server, and make single argument constructors explicit.
 *     - skstream/skserver.cpp: Use AI_PASSIVE with getaddrinfo() to make listen
 *       sockets accept any connection.
 *
 * Revision 1.8  2003/08/08 23:56:26  alriddoch
 *  2003-08-08 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skstream.cpp, skstream/skstream_unix.h: Include skstream
 *       header with its fully qualified name for compatability, and move
 *       the unix system header out of the header into the cpp file.
 *     - skstream/skserver.cpp, skstream/skserver.h: Add new base class for ip
 *       socket types, and put in the ip address related functionality into it,
 *       so basic class can be used as a base for unix socket class.  Move
 *       can_accept() into the base class as it applies to any listen socket.
 *     - skstream/skserver_unix.h, skstream/skserver.cpp: Add a class for
 *       unix listen sockets.
 *
 * Revision 1.7  2003/07/30 23:17:55  alriddoch
 *  2003-07-30 Al Riddoch <alriddoch@zepler.org>
 *     - skstream/skserver.cpp, skstream/skserver.h, skstream/skstream.cpp,
 *       skstream/skstream.h, skstream/skstream_unix.h: Move virtual
 *       function implementations into .cpp files.
 *
 * Revision 1.6  2003/04/16 14:02:35  alriddoch
 *  2003-04-16 Al Riddoch <alriddoch@zepler.org>,
 *     - Fix up includes so they work properlly
 *
 * Revision 1.5  2002/12/09 22:13:21  rsteinke
 *     - created basic_socket, a virtual base class
 *       for basic_socket_stream and basic_socket_server,
 *       so that the polling code has a common base
 *       with getSocket() to access
 *     - created basic_socket_poll polling class
 *     - changed tcp_socket_stream::is_ready() to isReady()
 *       to match standards for function naming, left is_ready()
 *       as a wrapper of isReady() for compatibility
 *
 * Revision 1.4  2002/04/08 20:02:00  xmp
 * Just a few fixes to MSVC support, removing a few unnessesary std::'s and shifting a default argument to a header.
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
#ifndef RGJ_FREE_THREADS_SERVER_H_
#define RGJ_FREE_THREADS_SERVER_H_

#include <skstream/skstream.h> // FreeSockets are needed

#include <string>

/////////////////////////////////////////////////////////////////////////////
// class basic_socket_server
/////////////////////////////////////////////////////////////////////////////
class basic_socket_server : public basic_socket {
protected:
  SOCKET_TYPE _socket;
  int LastError;

  void setLastError();

private:
  basic_socket_server(const basic_socket_server&);
  basic_socket_server& operator=(const basic_socket_server&);

  bool startup();
  bool shutdown();

protected:
  explicit basic_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET)
     : _socket(_sock), LastError(0) { 
    startup(); 
  }

public:
  // Destructor
  virtual ~basic_socket_server();

  bool is_open() const { 
    return (_socket != INVALID_SOCKET); 
  }

  virtual SOCKET_TYPE getSocket() const;

  int getLastError() { 
    setLastError(); 
    return LastError; 
  }

  void close();

  /**
   * See if accept() can be called without blocking on it.
   */
  bool can_accept();

  virtual SOCKET_TYPE accept() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// class ip_socket_server
/////////////////////////////////////////////////////////////////////////////
class ip_socket_server : public basic_socket_server {
protected:
  int _service;

  void setService(unsigned service) {
    if(is_open())
      close();

    _service = service;
    open(_service);
  }

  explicit ip_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET) :
             basic_socket_server(_sock), _service(0) {
  }
public:
  virtual ~ip_socket_server();

  int getService() {
      return _service;
  }

  virtual void open(int service) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_server
/////////////////////////////////////////////////////////////////////////////
class tcp_socket_server : public ip_socket_server {
public:
  tcp_socket_server() {
  }

  explicit tcp_socket_server(int service) : ip_socket_server() { 
    setService(service); 
  }

  // Destructor
  virtual ~tcp_socket_server();

  virtual SOCKET_TYPE accept();

  virtual void open(int service);
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server
/////////////////////////////////////////////////////////////////////////////
class udp_socket_server : public ip_socket_server {
public:
  explicit udp_socket_server(int service) : ip_socket_server() { 
    setService(service); 
  }

  // Destructor
  virtual ~udp_socket_server();

  // return the socket used to send/recv UDP packets
  virtual SOCKET_TYPE accept();

  virtual void open(int service);
};

#endif
