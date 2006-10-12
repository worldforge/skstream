/**************************************************************************
 FreeSockets - Portable C++ classes for IP(sockets) applications. (v0.3)
 Copyright (C) 2000-2001 Rafael Guterres Jeffman
           (C) 2002-2006 Alistair Riddoch

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
#ifndef RGJ_FREE_SOCKETS_PROTOCOL_H_
#define RGJ_FREE_SOCKETS_PROTOCOL_H_

#include <skstream/skstream.h> // FreeSockets classes are needed
#include <skstream/freethrd.h> // FreeThreads is needed for multithreading support

#include <string>
#include <iostream>

class basic_protocol : public basic_thread {
private:
  // Copy Constructor an d assignment operator - prevents from copying a thread
  // and a stream! - RGJ
  basic_protocol(const basic_protocol& copy);
  basic_protocol& operator=(const basic_protocol& copy);

protected:
  basic_socket_stream host;

public:
  typedef THREAD_RESULT protocol_code;

  // Other Constructors
  basic_protocol(SOCKET_TYPE sock) : basic_thread(), host(sock) {}

  // Destructor
  virtual ~basic_protocol() { host.close(); }

  // Set a new host
  void setHost(SOCKET_TYPE _sock) {
    if(host.is_open()) host.close();
    host.attach(_sock);
  }

  // Protocol function - must be implemented on derived classes
  virtual protocol_code run() { host.close(); return 0; }
};

#endif
