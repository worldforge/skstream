/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2012 Alistair Riddoch

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
#ifndef RGJ_FREE_ADDRESS_H_
#define RGJ_FREE_ADDRESS_H_

#include <skstream/sksocket.h>

#include <string>

struct addrinfo;

// I am making this inherit from basic_socket, even though it does not
// at this time appear to be a socket. This is so that it can ensure
// basic_socket::startup is called in the standard way, and so that if in
// future we need a custom, non blocking resolver, we can have one.

class basic_address : public basic_socket {
protected:
  struct addrinfo * _addrlist;

  int _type;
  int _protocol;

  basic_address(int, int);

  int resolve(int, const char *, const char *);

  // FIXME some data structures for non-getaddrinfo legacy systems
public:
  virtual ~basic_address();

  /// Check if an address has been resolved
  bool isReady() const {
    return _addrlist != 0;
  }

  int resolveListener(const std::string & service);

  int resolveConnector(const std::string & host, const std::string & service);
  
  // FIXME - perhaps we could do this like an iterator, c++11 style

  /// Check the number of network address resolved
  std::size_t size() const;

  class const_iterator;

  const_iterator begin() const;
  const_iterator end() const;

  /// Get one of the resolved address info records
  struct addrinfo * getAddrinfo(std::size_t c) const;

  virtual SOCKET_TYPE getSocket() const;
  
};

class basic_address::const_iterator {
private:
  friend class basic_address;

  struct addrinfo * _info;

  explicit const_iterator(struct addrinfo * i) : _info(i) {
  }

public:
  // FIXME Add move stuff

  const_iterator(const const_iterator & rhs) : _info(rhs._info) {
  }

  bool operator==(const const_iterator& rhs) {
    return _info == rhs._info;
  }

  bool operator!=(const const_iterator& rhs) {
    return !this->operator==(rhs);
  }

  const_iterator& operator=(const const_iterator& rhs) {
    _info = rhs._info;
    return *this;
  }

  struct addrinfo * operator*() const {
    return _info;
  }

  const_iterator& operator++();
};

inline basic_address::const_iterator basic_address::begin() const
{
  return basic_address::const_iterator(_addrlist);
}

inline basic_address::const_iterator basic_address::end() const
{
  return basic_address::const_iterator(0);
}

class tcp_address : public basic_address {
public:
  tcp_address();

};

#endif // RGJ_FREE_SOCKET_H_
