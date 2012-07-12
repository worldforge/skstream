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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <skstream/sksocket.h>

#ifndef _WIN32
#include <errno.h>
#endif

#include <cstdio>

static inline int getSystemError()
{
  #ifdef _WIN32
    return WSAGetLastError();
  #else
    return errno;
  #endif
}

/////////////////////////////////////////////////////////////////////////////
// class basic_socket implementation
/////////////////////////////////////////////////////////////////////////////

int basic_socket::startup_count = 0;

// private function that sets the internal variable LastError
void basic_socket::setLastError() const {
    LastError = getSystemError();
}

basic_socket::basic_socket() throw () : LastError(0)
{
  startup();
}


basic_socket::~basic_socket()
{
}

// System dependant initialization
bool basic_socket::startup() {
#ifdef _WIN32
  if (++startup_count == 1) {
      static const unsigned wMinVer = 0x0101;// request WinSock v1.1 (at least)
      WSADATA wsaData;
      int error = WSAStartup(wMinVer, &wsaData);
      return (error == 0);
  }
#else // _WIN32
  return true;
#endif // _WIN32
}
