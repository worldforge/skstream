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
// Revision 1.2  2002-02-20 05:04:07  grimicus
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

#include "skstream.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class socketbuftest : public CppUnit::TestCase
{
    //some macros for building the suite() method
    CPPUNIT_TEST_SUITE(socketbuftest);
    CPPUNIT_TEST(testConstructor_1);
    CPPUNIT_TEST(testConstructor_2);
    CPPUNIT_TEST(testSetOutpeer);
    CPPUNIT_TEST_SUITE_END();

    private: 
        socketbuf *socketBuffer;
        std::string *hostname;
        unsigned port;

    public:
        socketbuftest(std::string name) : TestCase(name) { }
        socketbuftest() { }

        void testConstructor_1()
        {
            socketbuf *socketBuf;

            for(int i = FreeSockets::proto_IP; i <= FreeSockets::proto_RAW; ++i)
            {
                socketBuf = new socketbuf(i);

                CPPUNIT_ASSERT(socketBuf);

                //always clean up...
                delete socketBuf;
            }


            //what happens when you set the socketbuf(0, 0, 0);
            socketBuf = new socketbuf(0, 0, 0);
            CPPUNIT_ASSERT(socketBuf);
            delete socketBuf;
        }

        void testConstructor_2()
        {
            socketbuf *socketBuf;
            char * ch; 

            for(int i = FreeSockets::proto_IP; i <= FreeSockets::proto_RAW; ++i)
            {
                ch = new char [20];
                int length = sizeof(ch);
                socketBuf = new socketbuf(i, ch, length);

                CPPUNIT_ASSERT_MESSAGE("testing i", socketBuf);

                delete socketBuf;
            }
        }

        void testSetOutpeer()
        {
            CPPUNIT_ASSERT(socketBuffer->setOutpeer(*hostname, port));
        }

        void setUp()
        {
            socketBuffer = new socketbuf(FreeSockets::proto_TCP);
            hostname = new std::string("www.worldforge.org");
            port = 80;
        }

        void tearDown()
        {
            delete socketBuffer;
            delete hostname;
            port = 0;
        }

};

#endif
