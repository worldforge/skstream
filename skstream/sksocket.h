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
#ifndef RGJ_FREE_SOCKET_H_
#define RGJ_FREE_SOCKET_H_

#include <skstream/skstreamconfig.h>

// This constant is defined in windows, but not in most other systems
#ifndef SOCKET_ERROR
static const int SOCKET_ERROR = -1;
#endif

// This constant is defined in windows, but not in most other systems
#ifndef INVALID_SOCKET
 #define INVALID_SOCKET   (SOCKET_TYPE)~0
#endif // INVALID_SOCKET

// All systems should define this, but it is here just in case
#ifndef INADDR_NONE
 #warning System headers do not define INADDR_NONE
 #define INADDR_NONE   0xFFFFFFFF
#endif // INADDR_NONE

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
// Supported Protocols
namespace FreeSockets {
  enum IP_Protocol {
    proto_IP   = IPPROTO_IP,
    proto_ICMP = IPPROTO_ICMP,
#ifndef _WIN32 
    proto_IGMP = IPPROTO_IGMP, 
#else 
    proto_IGMP = IPPROTO_GGP, 
#endif 
    proto_TCP  = IPPROTO_TCP,
    proto_PUP  = IPPROTO_PUP,
    proto_UDP  = IPPROTO_UDP,
    proto_IDP  = IPPROTO_IDP,
#ifdef IPPROTO_SCTP
    proto_SCTP = IPPROTO_SCTP,
#endif
    proto_RAW  = IPPROTO_RAW
  };
};

/////////////////////////////////////////////////////////////////////////////
// class basic_socket, a virtual base class for use in polling
/////////////////////////////////////////////////////////////////////////////
class basic_socket {
protected:
  mutable int LastError;

  void setLastError() const;

  basic_socket() throw ();
public:
  virtual ~basic_socket();

  virtual SOCKET_TYPE getSocket() const = 0;

  int getLastError() const { 
    return LastError; 
  }

  bool is_open() const { 
    return (getSocket() != INVALID_SOCKET); 
  }

  static bool startup();

};

#endif // RGJ_FREE_SOCKET_H_
