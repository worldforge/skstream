// socketbuf test case
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
// Revision 1.5  2003-05-04 21:30:16  alriddoch
//  2003-05-04 Al Riddoch <alriddoch@zepler.org>
//     - Sort out nasty hard tabs from address checks.
//     - Start work on updating the checks.
//
// Revision 1.4  2002/02/24 03:15:41  grimicus
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
// Revision 1.3  2002/02/21 05:11:15  grimicus
// 2002-02-20 Dan Tomalesky <grim@xynesis.com>
//     * Added a new test case header for basic_socket_streams
//
//     * Made a mod to basic_socket_stream::setOutpeer(sockaddr_in&)
//       seemed to me it had an erroneous ! in the return
//
//     * Oh, and I reformatted skstream.h so its not so damn hard to look at
//
// Revision 1.2  2002/02/20 05:04:07  grimicus
// 2002-02-19 Grimicus <grim@xynesis.com>
//
//     * updated socketbuf(SOCKET_TYPE, unsigned, unsigned) to have variable
//       names in the declaration so it was a bit easier to look at. (and know
//       what the heck they were suppose to be for)
//
//     * Added some tests in for socketbuf.  Not very good ones but its a start
//
// Revision 1.1  2002/02/19 22:04:31  grimicus
// 2002-02-19 Grimicus <grim@xynesis.com>
//
//     * Added License header to skstream.h
//
//     * Added cppunit TestCase  classes in test/ and auto* support functionality
//

#ifndef SOCKETBUFTEST_H
#define SOCKETBUFTEST_H

#include <skstream/skstream.h>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class socketbuftest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(socketbuftest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST(testConstructor_2);
    CPPUNIT_TEST(testGetOutpeer);
    CPPUNIT_TEST(testGetInpeer);
    CPPUNIT_TEST(testSetSocket);
    CPPUNIT_TEST_SUITE_END();

    private: 
        stream_socketbuf *socketBuffer;
        std::string *hostname;
        SOCKET_TYPE socket;
        unsigned port;

    public:
        socketbuftest(std::string name) : TestCase(name) { }
        socketbuftest() { }

        void testConstructor_1()
        {
            stream_socketbuf *sb = new stream_socketbuf(socket);

            CPPUNIT_ASSERT(sb);

            delete sb;
        }

        void testConstructor_2()
        {
            char * ch = new char [20];
            int length = sizeof(ch);

            stream_socketbuf * socketBuf = new stream_socketbuf(socket, ch, length);

            CPPUNIT_ASSERT(socketBuf);

            delete socketBuf;
        }

        void testGetOutpeer()
        {
            sockaddr_storage sain = socketBuffer->getOutpeer();

            //these tests are no longer useful

            //check the port was set
            // CPPUNIT_ASSERT(sain.sin_port);

            //check the address was set
            // CPPUNIT_ASSERT(sain.sin_addr.s_addr);
        }

        void testGetInpeer()
        {
            sockaddr_storage sain = socketBuffer->getInpeer();

            //these tests don't do anything it seems. hopefully some better
            //ones will surface at some point.
            
            //check the address was set
            //  CPPUNIT_ASSERT(sain.sin_addr.s_addr);

            //check the port was set
            //  CPPUNIT_ASSERT(sain.sin_port);

        }

        void testSetSocket()
        {
            stream_socketbuf socketBuf(INVALID_SOCKET);

            CPPUNIT_ASSERT(socketBuf.getSocket() == INVALID_SOCKET);

            socketBuf.setSocket(socket);

            CPPUNIT_ASSERT(socketBuf.getSocket() == socket);
        }

        void setUp()
        {
            socket = ::socket(AF_INET, SOCK_STREAM, FreeSockets::proto_TCP);
            socketBuffer = new stream_socketbuf(socket);
            
            //echo service must be running (check inetd settings or if you
            //are a winders user, you have to install it)
            hostname = new std::string("127.0.0.1");
            port = 7;
        }

        void tearDown()
        {
            delete socketBuffer;
            delete hostname;
            port = 0;
            socket = INVALID_SOCKET;
        }

};

#endif
