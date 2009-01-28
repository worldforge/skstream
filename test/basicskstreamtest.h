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
//  Author: Alistair Riddoch
//  Created: 2002-02-19 by Dan Tomalesky
//

#ifndef BASICSKSTREAMTEST_H
#define BASICSKSTREAMTEST_H

#include <skstream/skstream.h>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

class basicskstreamtest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(basicskstreamtest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST(testConstructor_2);
    CPPUNIT_TEST(testConstructor_3);
    CPPUNIT_TEST(testConstructor_4);
    CPPUNIT_TEST(testSetSocket);
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
            stream_socketbuf *skb = new stream_socketbuf(INVALID_SOCKET);
            basic_socket_stream *sks = new basic_socket_stream(*skb);

            CPPUNIT_ASSERT(sks);

            delete sks;
        }

        void testConstructor_2()
        {
            stream_socketbuf *skb;
            basic_socket_stream *sks;
            unsigned insize, outsize;

            for(int i = FreeSockets::proto_IP; i <= FreeSockets::proto_RAW; ++i)
            {
                //maybe make them random? :-)
                insize = i * 3;
                outsize = i * 2;

                skb = new stream_socketbuf(INVALID_SOCKET, insize, outsize);
                sks = new basic_socket_stream(*skb, i);

                CPPUNIT_ASSERT(sks);
                CPPUNIT_ASSERT(sks->getProtocol() == i);

                delete sks;
            }
            
            //test with crumby settings...should this fail?
            skb = new stream_socketbuf(INVALID_SOCKET,
                                       (std::streamsize)0,
                                       (std::streamsize)0);
            sks = new basic_socket_stream(*skb, 0);
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

            stream_socketbuf * skb = new stream_socketbuf (sock, insize, outsize);
            basic_socket_stream sks(*skb);

            CPPUNIT_ASSERT(sks);

            CPPUNIT_ASSERT(sks.getSocket() == sock);
        }

        void testSetSocket()
        {
            CPPUNIT_ASSERT(skstream->getSocket() == socket);
            CPPUNIT_ASSERT(skstream->is_open());
        }

        void testClose()
        {
            skstream->close();

            /*
            if(skstream->is_open())
            {
                std::cout << endl << "Last Error: " << 
                     strerror(skstream->getLastError()) << std::endl;
            }
            */

            CPPUNIT_ASSERT(!skstream->is_open());
        }

        void setUp()
        {
            socket = ::socket(AF_INET, SOCK_DGRAM, FreeSockets::proto_UDP);
            stream_socketbuf * skb = new stream_socketbuf(socket);
            skstream = new basic_socket_stream(*skb);
            
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
