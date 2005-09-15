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

#include <skstream/skstream.h> // FreeSockets are needed

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
  void shutdown();

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

  void setService(int service) {
    if(is_open())
      close();

    _service = service;
    open(_service);
  }

  bool bindToIpService(int service, int type, int protocol);

  explicit ip_socket_server(SOCKET_TYPE _sock = INVALID_SOCKET) :
             basic_socket_server(_sock), _service(0) {
  }
public:
  virtual ~ip_socket_server();

  int getService() {
      return _service;
  }

  virtual bool open(int service) = 0;
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

  virtual bool open(int service);
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

  virtual bool open(int service);
};

#endif // RGJ_FREE_THREADS_SERVER_H_
