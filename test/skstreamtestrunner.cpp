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
// Revision 1.6  2003-09-27 21:15:03  alriddoch
//  2003-09-27 Al Riddoch <alriddoch@zepler.org>
//     - test/childskstreamtest.h: Get rid of some of the java-isms, and
//       handle some failures a little more cleanly. Fix the udp test.
//     - test/skstreamtestrunner.cpp: Ensure that the tests exit status
//       reflects whether it passes or fails.
//     - Fix Makefile.am so tests are actually run.
//
// Revision 1.5  2002/02/26 20:33:55  grimicus
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

#include <cppunit/TextTestRunner.h>

#include "socketbuftest.h"
#include "basicskstreamtest.h"
#include "childskstreamtest.h"
#include "skservertest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(socketbuftest);
CPPUNIT_TEST_SUITE_REGISTRATION(basicskstreamtest);

CPPUNIT_TEST_SUITE_REGISTRATION(tcpskstreamtest);
CPPUNIT_TEST_SUITE_REGISTRATION(udpskstreamtest);

CPPUNIT_TEST_SUITE_REGISTRATION(tcpskservertest);
CPPUNIT_TEST_SUITE_REGISTRATION(udpskservertest);

#ifdef SOCK_RAW
CPPUNIT_TEST_SUITE_REGISTRATION(rawskstreamtest);
#endif

int main(int argc, char **argv)
{
    CppUnit::TextTestRunner runner;
    CppUnit::Test* tp =
            CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    runner.addTest(tp);

    if (runner.run()) {
        return 0;
    } else {
        return 1;
    }
}
