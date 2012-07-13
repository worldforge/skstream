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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <skstream/skaddress.h>

#ifndef _WIN32
#include <netdb.h>
#endif // _WIN32


/////////////////////////////////////////////////////////////////////////////
// class basic_address implementation
/////////////////////////////////////////////////////////////////////////////

basic_address::basic_address(int type, int protocol) :
    _addrlist(0), _type(type), _protocol(protocol)
{
}

basic_address::~basic_address()
{
  if(_addrlist != 0) {
    ::freeaddrinfo(_addrlist);
  }
}

int basic_address::resolve(int flags,
                           const char * node,
                           const char * service)
{
  struct addrinfo req;

  req.ai_flags = flags;
  req.ai_family = PF_UNSPEC;
  req.ai_socktype = _type;
  req.ai_protocol = _protocol;
  req.ai_addrlen = 0;
  req.ai_addr = 0;
  req.ai_canonname = 0;
  req.ai_next = 0;

  if (::getaddrinfo(0, service, &req, &_addrlist) != 0) {
    setLastError();
    return -1;
  }
  return 0;
}

int basic_address::resolveListener(const std::string & service)
{
  int ret = this->resolve(AI_PASSIVE, 0, service.c_str());

  return ret;
}

int basic_address::resolveConnector(const std::string & host,
                                    const std::string & service)
{
  int ret = this->resolve(0, host.c_str(), service.c_str());

  return ret;
}

std::size_t basic_address::size() const
{
  std::size_t len = 0;
  struct addrinfo * i = _addrlist;

  for(; i != 0; ++len, i = i->ai_next) {
  }

  return len;
}

SOCKET_TYPE basic_address::getSocket() const
{
  return INVALID_SOCKET;
}

basic_address::const_iterator& basic_address::const_iterator::operator++()
{
  _info = _info->ai_next;
  return *this;
}


/////////////////////////////////////////////////////////////////////////////
// class tcp_address implementation
/////////////////////////////////////////////////////////////////////////////

tcp_address::tcp_address() : basic_address(SOCK_STREAM, IPPROTO_TCP)
{
}

/////////////////////////////////////////////////////////////////////////////
// class ip_datagram_address implementation
/////////////////////////////////////////////////////////////////////////////

ip_datagram_address::ip_datagram_address() : basic_address(SOCK_DGRAM, 0)
{
}
