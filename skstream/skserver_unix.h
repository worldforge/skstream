/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2000-2001 Rafael Guterres Jeffman
 Copyright (C) 2003 Alistair Riddoch

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

#ifndef RGJ_FREE_THREADS_SERVER_UNIX_H_
#define RGJ_FREE_THREADS_SERVER_UNIX_H_

#include <skstream/skserver.h> // FreeSockets are needed

/////////////////////////////////////////////////////////////////////////////
// class unix_socket_server
/////////////////////////////////////////////////////////////////////////////
class unix_socket_server : public basic_socket_server {
protected:
  std::string _service;

  void setService(const std::string & service) {
    if(is_open())
      close();

    _service = service;
    open(_service);
  }

public:
  unix_socket_server() {
  }

  explicit unix_socket_server(const std::string & service) :
                                     basic_socket_server() { 
    setService(service); 
  }

  // Destructor
  virtual ~unix_socket_server();

  virtual SOCKET_TYPE accept();

  const std::string & getService() {
      return _service;
  }

  virtual void open(const std::string & service);
};

#endif // RGJ_FREE_THREADS_SERVER_UNIX_H_
