/****************************************************************************
 Copyright (C) 2003 Al Riddoch

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

****************************************************************************/

#ifndef SKSTREAM_UNIX_H_
#define SKSTREAM_UNIX_H_

#include <skstream/skstream.h>

/////////////////////////////////////////////////////////////////////////////
// class unix_socket_stream
/////////////////////////////////////////////////////////////////////////////
class unix_socket_stream : public basic_socket_stream {
private:
  explicit unix_socket_stream(const unix_socket_stream&);
  explicit unix_socket_stream(SOCKET_TYPE socket);

  unix_socket_stream& operator=(const unix_socket_stream& socket);

  SOCKET_TYPE _connecting_socket;
  stream_socketbuf stream_sockbuf;

public:
  unix_socket_stream() : basic_socket_stream(stream_sockbuf),
                         _connecting_socket(INVALID_SOCKET),
                         stream_sockbuf(INVALID_SOCKET) {
  }

  explicit unix_socket_stream(const std::string& address,
                              bool nonblock = false) : 
          basic_socket_stream(stream_sockbuf),
          _connecting_socket(INVALID_SOCKET),
          stream_sockbuf(INVALID_SOCKET) {
    open(address, nonblock);
  }

  unix_socket_stream(const std::string& address, unsigned int milliseconds) :
              basic_socket_stream(stream_sockbuf),
              _connecting_socket(INVALID_SOCKET),
              stream_sockbuf(INVALID_SOCKET) {
    open(address, milliseconds);
  }

  virtual ~unix_socket_stream();

  void open(const std::string& address, bool nonblock = false);
  void open(const std::string& address, unsigned int milliseconds) {
    open(address, true);
    if(!isReady(milliseconds)) {
      close();
      fail();
    }
  }

  virtual void close();
  virtual SOCKET_TYPE getSocket() const;

  bool isReady(unsigned int milliseconds = 0);
};

#endif // SKSTREAM_UNIX_H_
