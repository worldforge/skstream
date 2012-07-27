/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2000-2001 Rafael Guterres Jeffman
           (C) 2003-2006 Alistair Riddoch

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

/// \brief Base class for anything that encapsulates a listen socket.
class basic_socket_server : public basic_socket {
public:
  static const int SK_SRV_NONE = 0;
  static const int SK_SRV_PURE = 1 << 0;
  static const int SK_SRV_REUSE = 1 << 1;
protected:
  SOCKET_TYPE _socket;
  int _flags;
private:
  basic_socket_server(const basic_socket_server&);
  basic_socket_server& operator=(const basic_socket_server&);

protected:
  explicit basic_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET,
                               int flags = SK_SRV_NONE)
     : _socket(_sock), _flags(flags) { 
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

/// \brief Base class for anything that encapsulates an IP listen socket.
class ip_socket_server : public basic_socket_server {
protected:
  int bindToAddressInfo(struct addrinfo *);
  int bindToIpService(int service, int type, int protocol);

  explicit ip_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET,
                            int flags = SK_SRV_NONE) :
             basic_socket_server(_sock, flags) {
  }
public:
  virtual ~ip_socket_server();
};

/////////////////////////////////////////////////////////////////////////////
// class tcp_socket_server
/////////////////////////////////////////////////////////////////////////////

/// \brief Encapsulates a TCP/IP stream listen socket.
class tcp_socket_server : public ip_socket_server {
public:
  explicit tcp_socket_server(int flags = SK_SRV_PURE |
                                         SK_SRV_REUSE) :
      ip_socket_server(INVALID_SOCKET, flags) {
  }

  // Destructor
  virtual ~tcp_socket_server();

  SOCKET_TYPE accept();

  int open(int service);
  int open(struct addrinfo *);
};

/////////////////////////////////////////////////////////////////////////////
// class udp_socket_server
/////////////////////////////////////////////////////////////////////////////

/// \brief Encapsulates a UDP/IP datagram listen socket.
class udp_socket_server : public ip_socket_server {
public:
  explicit udp_socket_server(int service) { 
    open(service); 
  }

  // Destructor
  virtual ~udp_socket_server();

  int open(int service);

};

#endif // RGJ_FREE_THREADS_SERVER_H_
