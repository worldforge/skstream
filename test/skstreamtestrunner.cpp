// skstream test runner
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
//  Created: 2000-10-03 by Bryce
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

#include <cppunit/TextTestRunner.h>

#include "socketbuftest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(socketbuftest);

int main(int argc, char **argv)
{
    CppUnit::TextTestRunner runner;
    CppUnit::Test* tp =
            CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    runner.addTest(tp);

    runner.run();
    return 0;
}
