// socket_server test cases
// Copyright (C) 2002 Dan Tomalesky and The WorldForge Project
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//  For information about Worldforge and its authors, please contact
//  the Worldforge Web Site at http://www.wordforge.org.
//
//  Author: Dan Tomalesky
//  Created: 2002-02-24 by Dan Tomalesky
//
// $Log$
// Revision 1.1  2002-02-26 20:33:55  grimicus
// 02/26/2002 Dan Tomalesky <grim@xynesis.com>
//     * Added test cases for skserver and friends
//
//     * Adding .cvsignore files so that it doesn't mess with non-cvs files anymore
//
//     * In skserver.cpp and skstream.cpp, in the close() methods, I commented out
//       the return; in the error part of the shutdown() call because it is
//       possible that a socket can exist without it actually having been used,
//       so this could error out on those cases and the socket is never actually
//       closed.
//
//     * In skserver.h, added can_accept() to tcp_socket_server so that it can be
//       checked to see if the server socket has any connections available, so that
//       accept() can then be called. (if it returns false, if accept is called,
//       it will block until a connection is made)
//
//     * Removed the #include <iostream> from skserver.h and skstream.h as they are
//       not actually needed for any of the code. (else it comes in from some other
//       include, I'm not positive)
//
//     * Made some formatting changes in skserver.h along the same lines as I have
//       been doing throughout the code.
//
//     * Added testClose() to basicskstreamtest.
//
//     * Changed the socket created in basicskstreamtest from SOCK_STREAM to
//       SOCK_DGRAM though it doesn't make any difference what so ever in the
//       testing.
//
//     * Added the skservertests into the test runner.
//

#ifndef SKSERVERTEST_H
#define SKSERVERTEST_H

#include "skserver.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class tcpskservertest : public CppUnit::TestCase {

    //macros for setting up suite.
    CPPUNIT_TEST_SUITE(tcpskservertest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccept);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testClose);
    CPPUNIT_TEST_SUITE_END();

    private:
        tcp_socket_server *skserver;
        int port;

    public:
        tcpskservertest(std::string name) : TestCase(name) { }
        tcpskservertest() { }

        void testConstructor()
        {
            CPPUNIT_ASSERT(skserver);

            CPPUNIT_ASSERT(skserver->is_open());
        }

        void testAccept()
        {
            CPPUNIT_ASSERT(skserver->is_open());
            
            tcp_socket_stream tss(std::string("localhost"), port);

            CPPUNIT_ASSERT(tss.is_open());
            
            CPPUNIT_ASSERT(skserver->can_accept());

            SOCKET_TYPE socket = skserver->accept();

            CPPUNIT_ASSERT(socket != INVALID_SOCKET);
        }

        void testOpen()
        {
            skserver->open(7777);
            CPPUNIT_ASSERT(skserver->is_open());
        }

        void testClose()
        {
            skserver->close();

            CPPUNIT_ASSERT(!skserver->is_open());
        }

        void setUp()
        {
            port = 8888;
            skserver = new tcp_socket_server(port);
        }

        void tearDown()
        {
            delete skserver;
            port = 0;
        }
};

class udpskservertest : public CppUnit::TestCase {
    
    //macros for setting up suite.
    CPPUNIT_TEST_SUITE(udpskservertest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAccept);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testClose);
    CPPUNIT_TEST_SUITE_END();

    private:
        udp_socket_server *skserver;
        int port;

    public:
        udpskservertest(std::string name) : TestCase(name) { }
        udpskservertest() { }
        
        void testConstructor()
        {
            CPPUNIT_ASSERT(skserver);

            CPPUNIT_ASSERT(skserver->is_open());
        }

        void testAccept()
        {
            CPPUNIT_ASSERT(skserver->accept() != INVALID_SOCKET);
        }

        void testOpen()
        {
            skserver->open(7777);
            CPPUNIT_ASSERT(skserver->is_open());
        }

        void testClose()
        {
            skserver->close();

            CPPUNIT_ASSERT(!skserver->is_open());
        }

        void setUp()
        {
            port = 8888;
            skserver = new udp_socket_server(port);
        }

        void tearDown()
        {
            delete skserver;
            port = 0;
        }
};

#endif
