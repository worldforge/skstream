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
 * Revision 1.2  2002-02-24 03:15:41  grimicus
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

#include <string>
#include <iostream>

#include "skstream.h" // FreeSockets are needed

/////////////////////////////////////////////////////////////////////////////
// class basic_socket_server
/////////////////////////////////////////////////////////////////////////////
class basic_socket_server {
protected:
  SOCKET_TYPE _socket;
  int _service;
  int LastError;

  void setLastError();

  void setService(unsigned service) {
    if(is_open())
      close();
    _service = service;
    open(_service);
  }

private:
  basic_socket_server(const basic_socket_server&);
  basic_socket_server& operator=(const basic_socket_server&);

  bool startup();
  bool shutdown();

protected:
  basic_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET)
     : _socket(_sock), _service(0), LastError(0) { startup(); };

public:
  // Destructor
  virtual ~basic_socket_server() { close(); shutdown(); }

  bool is_open() const
    { return ( _socket != INVALID_SOCKET); }

  SOCKET_TYPE getSocket() const
    { return _socket; }

  int getLastError() { setLastError(); return LastError; }

  void close();

  virtual SOCKET_TYPE accept() = 0;
  virtual void open(int service) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_server
/////////////////////////////////////////////////////////////////////////////
class tcp_socket_server : public basic_socket_server {
public:
  tcp_socket_server(int service) : basic_socket_server()
    { setService(service); }

  // Destructor
  virtual ~tcp_socket_server() { close(); }

  virtual SOCKET_TYPE accept();

  virtual void open(int service);
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server
/////////////////////////////////////////////////////////////////////////////
class udp_socket_server : public basic_socket_server {
public:
  udp_socket_server(int service) : basic_socket_server()
    { setService(service); }

  // Destructor
  virtual ~udp_socket_server() { close(); }

  // return the socket used to send/recv UDP packets
  virtual SOCKET_TYPE accept() { return _socket; };

  virtual void open(int service);
};

#endif
