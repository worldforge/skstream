// skstream - Portable C++ classes for IP(sockets) applications.
// Copyright (C) 2011 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <skstream/skstream.h>

#include <cstdio>
#include <cstdlib>

static void usage(const char * prgname)
{
    std::cout << "usage: " << prgname << " host [port]"
              << std::endl << std::flush;
}

int main(int argc, char ** argv)
{
    if (argc < 2 || argc > 3) {
        usage(argv[0]);
        return 1;
    }

    tcp_socket_stream * s = new tcp_socket_stream;
    int port = 80;

    if (argc == 3) {
        port = strtol(argv[2], 0, 10);
    }

    s->open(argv[1], port);

    if (!s->is_open()) {
        perror("connect");
        return 1;
    }

    return 0;
}
