# Skstream
<http://github.com/worldforge/skstream.git> Status: [![Build Status](https://secure.travis-ci.org/worldforge/skstream.png?branch=master)](http://travis-ci.org/worldforge/skstream)

Skstream is an iostream based C++ socket library. It is ideally suited for use
as the underlying transport for Atlas-C++, and has been well tested and
debugged. It was first discovered for WorldForge when it was used in UClient
and has since been packaged up as its own library for use on other WorldForge
projects. The code was originally written by Rafael Guterres Jeffman who is not
involved with the project but we have extensively rewritten it, and added new
features such as IPv6 support and unix domain sockets. It now maintained for
WorldForge by Al Riddoch and others.

This is version 0.3.2.

## Installation

Skstream uses GNU autotools and CPPUnit for building and testing, respectively.
In Ubuntu (tested with 12.10) dependencies can be installed with:

    $ sudo apt-get -y install make automake g++ autoconf libtool libcppunit-dev

You can then configure and build Skstream with:

    $ ./autogen.sh
    $ make

To allow GNU Autotools/Libtool to attempt to install the library for you, so
that it can be used by other programs or libraries (Including the rest of the
WorldForge suite) use:

    $ sudo make install

