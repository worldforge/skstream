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
// Revision 1.1  2002-02-19 22:04:31  grimicus
// 2002-02-19 Grimicus <grim@xynesis.com>
//
//     * Added License header to skstream.h
//
//     * Added cppunit TestCase  classes in test/ and auto* support functionality
//

#ifndef SOCKETBUFTEST_H
#define SOCKETBUFTEST_H

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

class socketbuftest;

typedef CppUnit::TestCaller<socketbuftest> socketbuftestcaller;

class socketbuftest : CppUnit::TestCase
{
    private:
        socketbuf *sbuf;

    public:
        socketbuftest(std::string name) : TestCase(name) { }
        socketbuftest() { }

        void testConstructor()
        {
            CPPUNIT_ASSERT(true);
        }

        static CppUnit::TestSuite *suite()
        {
            CppUnit::TestSuite *suite = new CppUnit::TestSuite();
            suite->addTest(new socketbuftestcaller("testStuff", 
                                                   &socketbuftest::testStuff));
            return suite;
        }

        void setUp()
        {
            sbuf = new socketbuf();
        }

        void tearDown()
        {
        }
};

#endif
