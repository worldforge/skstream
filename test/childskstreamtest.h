// test cases for children of basic_socket_stream
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
//  Created: 2002-02-23 by Dan Tomalesky
//
// $Log$
// Revision 1.2  2002-05-21 07:29:37  malcolm
// Added rsteinke's nonblocking connect patch.  Works on linux; does not break API
// (I bumped version to 0.2.3 anyway).  May not work on win32, though I did test it
// and socket communication does happen.
//
// Revision 1.1  2002/02/24 03:15:41  grimicus
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

#ifndef CHILDSKSTREAMTEST_H
#define CHILDSKSTREAMTEST_H

#include "skstream.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class tcpskstreamtest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(tcpskstreamtest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST(testConstructor_2);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testOpenNonblock);
    CPPUNIT_TEST_SUITE_END();

    private: 
        tcp_socket_stream *skstream;
        std::string *hostname;
        int port;

    public:
        tcpskstreamtest(std::string name) : TestCase(name) { }
        tcpskstreamtest() { }

        void testConstructor_1()
        {
            CPPUNIT_ASSERT(skstream);
        }

        void testConstructor_2()
        {
            tcp_socket_stream *sks = new tcp_socket_stream(*hostname, port);

            CPPUNIT_ASSERT(sks);

            CPPUNIT_ASSERT(sks->is_open());

            delete sks;
        }

        void testOpen()
        {
            skstream->open(*hostname, port);
            
            if(!skstream->is_open())
            {
                if(skstream->getLastError() == ECONNREFUSED)
                {
                    CPPUNIT_ASSERT_MESSAGE("Check that echo service is running on local machine", skstream->is_open());
                }
                else
                {
                    CPPUNIT_ASSERT(skstream->is_open());
                }
            }
                    
        }

	void testOpenNonblock()
	{
	    skstream->open(*hostname, port, true);

	    int waitcount = 0;
	    while(!skstream->is_ready()) // wait
		++waitcount;

	    // FIXME print waitcount somehow, to verify we're not connecting
	    // immediately
         
            if(!skstream->is_open())
            {
                if(skstream->getLastError() == ECONNREFUSED)
                {
                    CPPUNIT_ASSERT_MESSAGE("Check that echo service is running on local machine", skstream->is_open());
                }
                else
                {
                    CPPUNIT_ASSERT(skstream->is_open());
                }
            }
   
	}

        void setUp()
        {
            skstream = new tcp_socket_stream();
            
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
        }

};

class udpskstreamtest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(udpskstreamtest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST_SUITE_END();

    public:
        udpskstreamtest(std::string name) : TestCase(name) { }
        udpskstreamtest() { }

        void testConstructor_1()
        {
            udp_socket_stream skstream;
            CPPUNIT_ASSERT(skstream);

            CPPUNIT_ASSERT(skstream.is_open());
        }

        void setUp()
        {
        }

        void tearDown()
        {
        }

};

#ifdef SOCK_RAW
class rawskstreamtest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(rawskstreamtest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST_SUITE_END();

    public:
        rawskstreamtest(std::string name) : TestCase(name) { }
        rawskstreamtest() { }

        void testConstructor_1()
        {
            raw_socket_stream skstream;
            CPPUNIT_ASSERT(skstream);

            CPPUNIT_ASSERT_MESSAGE(
                    "Raw only works on GNU/Linux and you must be root", 
                    skstream.is_open());
        }

        void setUp()
        {
        }

        void tearDown()
        {
        }

};
#endif

#endif
