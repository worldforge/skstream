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
 * Revision 1.1  2002-12-09 22:13:21  rsteinke
 *     - created basic_socket, a virtual base class
 *       for basic_socket_stream and basic_socket_server,
 *       so that the polling code has a common base
 *       with getSocket() to access
 *     - created basic_socket_poll polling class
 *     - changed tcp_socket_stream::is_ready() to isReady()
 *       to match standards for function naming, left is_ready()
 *       as a wrapper of isReady() for compatibility
 *
 */

#include "skpoll.h"

basic_socket_poll::basic_socket_poll() : maxfd_(0)
{
  FD_ZERO(&read_);
  FD_ZERO(&write_);
  FD_ZERO(&except_);
}

int basic_socket_poll::poll(const socket_map& map, unsigned long timeout = 0)
{
  FD_ZERO(&read_);
  FD_ZERO(&write_);
  FD_ZERO(&except_);
  maxfd_ = 0;

  for(socket_map::const_iterator I = map.begin(); I != map.end(); ++I) {
    SOCKET_TYPE socket;
    if(!(I->second & MASK) || !I->first ||
      (socket = I->first->getSocket()) == INVALID_SOCKET)
      continue;

    if(I->second & READ)
      FD_SET(socket, &read_);
    if(I->second & WRITE)
      FD_SET(socket, &write_);
    if(I->second & EXCEPT)
      FD_SET(socket, &except_);
    if(socket >= maxfd_)
      maxfd_ = socket + 1;
  }

  struct timeval timeout_val = {timeout / 1000, timeout % 1000};

  return select(maxfd_, &read_, &write_, &except_, &timeout_val);
}

basic_socket_poll::poll_type basic_socket_poll::isReady(const basic_socket* soc,
	poll_type mask = MASK)
{
  SOCKET_TYPE socket;
  if(!(mask & MASK) || !soc || (socket = soc->getSocket()) == INVALID_SOCKET
    || socket >= maxfd_)
    return (poll_type) 0;

  unsigned result = 0;
  if((mask & READ) && FD_ISSET(socket, &read_))
    result |= READ;
  if((mask & WRITE) && FD_ISSET(socket, &write_))
    result |= WRITE;
  if((mask & EXCEPT) && FD_ISSET(socket, &except_))
    result |= EXCEPT;

  return (poll_type) result;
}
