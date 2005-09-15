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
#ifndef RGJ_FREE_SOCKET_POLL_H_
#define RGJ_FREE_SOCKET_POLL_H_

#include <skstream/skstream.h>

#include <map>

class basic_socket_poll
{
public:
  basic_socket_poll();

  enum poll_type {
    READ = 1 << 0,
    WRITE = 1 << 1,
    EXCEPT = 1 << 2,
    MASK = (1 << 3) - 1
  };
  typedef std::map<const basic_socket*,poll_type> socket_map;

  int poll(const socket_map& sockets, unsigned long timeout = 0);

  poll_type isReady(const basic_socket* soc, poll_type mask = MASK);
  poll_type isReady(const socket_map::value_type& val)
	{return isReady(val.first, val.second);}
  poll_type isReady(const socket_map::iterator& I)
	{return isReady(I->first, I->second);}
  poll_type isReady(const socket_map::const_iterator& I)
	{return isReady(I->first, I->second);}
  poll_type isReady(const socket_map::reverse_iterator& I)
	{return isReady(I->first, I->second);}
  poll_type isReady(const socket_map::const_reverse_iterator& I)
	{return isReady(I->first, I->second);}

private:
  basic_socket_poll(const basic_socket_poll&);
  basic_socket_poll& operator=(const basic_socket_poll&);

  fd_set read_, write_, except_;
  SOCKET_TYPE maxfd_;
};

#endif

