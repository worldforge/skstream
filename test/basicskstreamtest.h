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
// Revision 1.1  2002-02-21 05:11:15  grimicus
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
    CPPUNIT_TEST_SUITE_END();

    private: 
        basic_socket_stream *skstream;
        std::string *hostname;
        int port;

        SOCKET_TYPE *socket_types; 
        int st_length;

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
            basic_socket_stream *sks;

            for(int i = 0; i < st_length; ++i)
            {
                sks = new basic_socket_stream(socket_types[i]);

                CPPUNIT_ASSERT(sks);

                CPPUNIT_ASSERT(sks->getSocket() == socket_types[i]);

                delete sks;
            }
        }

        void testConstructor_4()
        {
            basic_socket_stream *sks;
            unsigned insize, outsize;

            for(int i = 0; i < st_length; ++i)
            {
                //maybe make them random? :-)
                insize = i * 3;
                outsize = i * 2;

                sks = new basic_socket_stream(socket_types[i], insize, outsize);

                CPPUNIT_ASSERT(sks);

                CPPUNIT_ASSERT(sks->getSocket() == socket_types[i]);

                delete sks;
            }
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

//            this test no worky, need to connect to see the values...
//            std::cout << endl << "remote host: " << 
 //                        skstream->getRemoteHost() << endl;
  //          CPPUNIT_ASSERT(*hostname == skstream->getRemoteHost());
   //         CPPUNIT_ASSERT(port == skstream->getRemotePort());

        }

        void testSetSocket()
        {
            for(int i = 0; i < st_length ; ++i)
            {
                int value = socket_types[i];
                skstream->setSocket(value);

                CPPUNIT_ASSERT(skstream->getSocket() == value);
                CPPUNIT_ASSERT(skstream->is_open());
            }
        }

        void testSetBroadcast()
        {
            CPPUNIT_ASSERT(skstream->setBroadcast(true));
            CPPUNIT_ASSERT(skstream->setBroadcast(false));
        }

        void setUp()
        {
            skstream = new basic_socket_stream();
            
            //echo service must be running (check inetd settings or if you
            //are a winders user, you have to install it)
            hostname = new std::string("127.0.0.1");
                //new std::string("localhost");
            port = 7;

            //these are the potential values for SOCKET_TYPE
            //it is assumed the sockets are using AF_INET, and
            //hence SOCK_SEQPACKET should fail at least in a GNU 
            //implementation of sockets...?
            st_length = 6;
            socket_types = new int [st_length];
            socket_types[0] = SOCK_STREAM;
            socket_types[1] = SOCK_DGRAM;
            socket_types[2] = SOCK_RAW;
            socket_types[3] = SOCK_RDM;
            socket_types[4] = SOCK_SEQPACKET;
            socket_types[5] = SOCK_PACKET;
        }

        void tearDown()
        {
            delete skstream;
            delete hostname;
            port = 0;

            delete [] socket_types;
            st_length = 0;
        }

};

#endif
