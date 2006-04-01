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
#ifndef RGJ_FREE_THREADS_SERVER_H_
#define RGJ_FREE_THREADS_SERVER_H_

#include <skstream/sksocket.h> // FreeSockets are needed

/////////////////////////////////////////////////////////////////////////////
// class basic_socket_server
/////////////////////////////////////////////////////////////////////////////
class basic_socket_server : public basic_socket {
protected:
  SOCKET_TYPE _socket;

private:
  basic_socket_server(const basic_socket_server&);
  basic_socket_server& operator=(const basic_socket_server&);

protected:
  explicit basic_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET)
     : _socket(_sock) { 
    startup(); 
  }

public:
  // Destructor
  virtual ~basic_socket_server();

  virtual SOCKET_TYPE getSocket() const;

  void close();
  void shutdown();

  /// See if accept() can be called without blocking on it.
  bool can_accept();

};

/////////////////////////////////////////////////////////////////////////////
// class ip_socket_server
/////////////////////////////////////////////////////////////////////////////
class ip_socket_server : public basic_socket_server {
protected:
  int bindToIpService(int service, int type, int protocol);

  explicit ip_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET) :
             basic_socket_server(_sock) {
  }
public:
  virtual ~ip_socket_server();
};

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_server
/////////////////////////////////////////////////////////////////////////////
class tcp_socket_server : public ip_socket_server {
public:
  tcp_socket_server() {
  }

  explicit tcp_socket_server(int service) { 
    open(service); 
  }

  // Destructor
  virtual ~tcp_socket_server();

  SOCKET_TYPE accept();

  bool open(int service);
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server
/////////////////////////////////////////////////////////////////////////////
class udp_socket_server : public ip_socket_server {
public:
  explicit udp_socket_server(int service) { 
    open(service); 
  }

  // Destructor
  virtual ~udp_socket_server();

  bool open(int service);

};

#endif // RGJ_FREE_THREADS_SERVER_H_
