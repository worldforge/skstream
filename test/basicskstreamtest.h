// basic_socket_stream test case
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
//  Created: 2002-02-19 by Dan Tomalesky
//
// $Log$
// Revision 1.3  2002-02-26 20:33:55  grimicus
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
// Revision 1.2  2002/02/24 03:15:41  grimicus
// 02/23/2002 Dan Tomalesky <grim@xynesis.com>
//
//     * Added in CVS logging variable so that changes show up in modified files
//       This will help keep changes made by worldforge in each file that is
//       changed as required by the GPL.
//
//     * Changed some parameter variables to have better meaning.
//       (ad -> address, etc.)
//
//     * Added some code into tcp_sk_stream::open so that it calls setLastError()
//       when the connection fails.
//
//     * Added some comments into skstream.h to better describe SOCKET_TYPE as
//       there can be some confusion between what it is actually for
//       (pointer/file descriptor/windows cludge of the socket) and the various
//       types of sockets (tcp, udp, raw, etc)
//
//     * Changed some more formatting for readability.
//
//     * Uncommented some commented out code in skstream.h so that the sync()
//       method returns 0 on the else, rather than not returning anything.
//
//     * Added some code into setBroadcast() so that setLastError() is called
//       if it fails to perform the getsocketopt().
//
//     * Modified the test/Makefile.am to remove the header files from the SOURCES
//       as the .h files do not seem to affect the build.
//
//     * Updated all the current test so that they use a socket instead of the
//       absolutely wrong stuff I was doing before.
//
//     * Added tests for tcp, udp, and raw skstreams child classes.
//
// Revision 1.1  2002/02/21 05:11:15  grimicus
// 2002-02-20 Dan Tomalesky <grim@xynesis.com>
//     * Added a new test case header for basic_socket_streams
//
//     * Made a mod to basic_socket_stream::setOutpeer(sockaddr_in&)
//       seemed to me it had an erroneous ! in the return
//
//     * Oh, and I reformatted skstream.h so its not so damn hard to look at
//
//

#ifndef BASICSKSTREAMTEST_H
#define BASICSKSTREAMTEST_H

#include "skstream.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class basicskstreamtest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(basicskstreamtest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST(testConstructor_2);
    CPPUNIT_TEST(testConstructor_3);
    CPPUNIT_TEST(testConstructor_4);
    CPPUNIT_TEST(testSetOutpeer);
    CPPUNIT_TEST(testSetSocket);
    CPPUNIT_TEST(testSetBroadcast);
    CPPUNIT_TEST(testRemoteHost);
    CPPUNIT_TEST(testClose);
    CPPUNIT_TEST_SUITE_END();

    private: 
        basic_socket_stream *skstream;
        std::string *hostname;
        SOCKET_TYPE socket;
        int port;

    public:
        basicskstreamtest(std::string name) : TestCase(name) { }
        basicskstreamtest() { }

        void testConstructor_1()
        {
            basic_socket_stream *sks = new basic_socket_stream();

            CPPUNIT_ASSERT(sks);

            delete sks;
        }

        void testConstructor_2()
        {
            basic_socket_stream *sks;
            unsigned insize, outsize;

            for(int i = FreeSockets::proto_IP; i <= FreeSockets::proto_RAW; ++i)
            {
                //maybe make them random? :-)
                insize = i * 3;
                outsize = i * 2;

                sks = new basic_socket_stream(insize, outsize, i);

                CPPUNIT_ASSERT(sks);
                CPPUNIT_ASSERT(sks->getProtocol() == i);

                delete sks;
            }
            
            //test with crumby settings...should this fail?
            sks = new basic_socket_stream((unsigned)0, 0, 0);
            CPPUNIT_ASSERT(sks);
            delete sks;
        }

        void testConstructor_3()
        {
            CPPUNIT_ASSERT(skstream);
        }

        void testConstructor_4()
        {
            unsigned insize = 5, outsize = 6;
            SOCKET_TYPE sock = skstream->getSocket();

            basic_socket_stream sks(sock, insize, outsize);

            CPPUNIT_ASSERT(sks);

            CPPUNIT_ASSERT(sks.getSocket() == sock);
        }

        void testSetOutpeer()
        {
            CPPUNIT_ASSERT(skstream->setOutpeer(*hostname, port));
            sockaddr_in sain = skstream->getOutpeer();

            //check the port was set
            CPPUNIT_ASSERT(sain.sin_port);

            //check the address was set
            CPPUNIT_ASSERT(sain.sin_addr.s_addr);

            //now test other setOutpeer method
            CPPUNIT_ASSERT(skstream->setOutpeer(sain));
        }

        void testSetSocket()
        {
            CPPUNIT_ASSERT(skstream->getSocket() == socket);
            CPPUNIT_ASSERT(skstream->is_open());
        }

        void testSetBroadcast()
        {
            if(!skstream->setBroadcast(true))
            {
                cout << endl;
                switch(skstream->getLastError()) 
                {
                    case EBADF:
                       cout << "Error with broadcast: EBADF" << endl;
                       break;
                    case ENOTSOCK:
                       cout << "Error with broadcast: ENOTSOCK" << endl;
                       break;
                    case ENOPROTOOPT:
                       cout << "Error with broadcast: ENOPROTOOPT" << endl;
                       break;
                    case EFAULT:
                       cout << "Error with broadcast: EFAULT" << endl;
                       break;
                }

                CPPUNIT_ASSERT(false);
            }
            
            CPPUNIT_ASSERT(skstream->setBroadcast(false));
        }

        void testRemoteHost()
        {
            remote_host rh(*hostname, port);

            *skstream << rh;
            sockaddr_in sain = skstream->getOutpeer();
            CPPUNIT_ASSERT(sain.sin_port);
            CPPUNIT_ASSERT(sain.sin_addr.s_addr);
        }

        void testClose()
        {
            skstream->close();

            /*
            if(skstream->is_open())
            {
                cout << endl << "Last Error: " << 
                     skstream->getLastError() << endl;
            }
            */

            CPPUNIT_ASSERT(!skstream->is_open());
        }

        void setUp()
        {
            socket = ::socket(AF_INET, SOCK_DGRAM, FreeSockets::proto_UDP);
            skstream = new basic_socket_stream(socket);
            
            //echo service must be running (check inetd settings or if you
            //are a winders user, you have to install it)
            hostname = new std::string("127.0.0.1");
                //new std::string("localhost");
            port = 7;
        }

        void tearDown()
        {
            delete skstream;
            delete hostname;
            port = 0;
            socket = INVALID_SOCKET;
        }

};

#endif
