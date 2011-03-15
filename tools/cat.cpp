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

#include <getopt.h>

static const std::streamsize BUF_SIZE = 8192;

static void usage(const char * prgname)
{
    std::cout << "usage: " << prgname << " [-n] host [port]"
              << std::endl << std::flush;
}

static bool copy(std::istream & i, std::ostream & o)
{
    static char buffer[BUF_SIZE];
    if (i.fail() ||
        i.peek() == std::iostream::traits_type::eof()) {
        return true;
    } else {
        std::streamsize count = i.rdbuf()->in_avail();
        for (; count > 0;) {
            std::streamsize len = std::min(BUF_SIZE - 1, count);
            i.rdbuf()->sgetn(&buffer[0], len);
            buffer[len] = '\0';
            o << &buffer[0] << std::flush;
            count -= len;
        }
    }
    return false;
}

int main(int argc, char ** argv)
{
    bool option_verbose = false;
    bool option_nonblock = false;

    while (1) {
        int c = getopt(argc, argv, "nv");
        if (c == -1) {
            break;
        } else if (c == '?') {
            usage(argv[0]);
            return 1;
        } else if (c == 'v') {
            option_verbose = true;
        } else if (c == 'n') {
            option_nonblock = true;
        }
    }

    if (option_verbose && option_nonblock) {
        std::cerr << "Connecting non blocking"
                  << std::endl << std::flush;
    }

    int arg_left = argc - optind;

    if (arg_left < 1 || arg_left > 2) {
        usage(argv[0]);
        return 1;
    }

    tcp_socket_stream * s = new tcp_socket_stream;
    int port = 80;

    if (arg_left == 2) {
        port = strtol(argv[optind + 1], 0, 10);
    }

    s->open(argv[optind], port, option_nonblock);

    if (option_verbose) {
        std::cerr << "Open called" << std::endl << std::flush;
    }

    if(s->connect_pending()) {
        for (int i = 0; i < 4; ++i) {
            std::cerr << "Connection pending"
                      << std::endl << std::flush;
            if (s->isReady(1000)) {
                break;
            }
        }
    } else if (s->is_open()) {
        std::cerr << "Connected without blocking"
                  << std::endl << std::flush;
    } else {
        std::cerr << "Connection failed"
                  << std::endl << std::flush;
        return 1;
    }

    if (s->is_open()) {
        std::cerr << "Connection complete"
                  << std::endl << std::flush;
    } else {
        perror("connect");
        return 1;
    }

    SOCKET_TYPE sfd = s->getSocket();
#ifdef _WIN32
    int nfds = sfd + 1;
#else
    int nfds = std::max(STDIN_FILENO, sfd) + 1;
#endif

    bool done = false;

    while (!done) {
        fd_set rfds;
        FD_ZERO(&rfds);
#ifndef _WIN32
        FD_SET(STDIN_FILENO, &rfds);
#endif
        FD_SET(sfd, &rfds);

        int ret = select(nfds, &rfds, 0, 0, 0);

        if (ret == -1) {
            perror("select");
            done = true;
        } else if (ret) {
#ifndef _WIN32
            static char buffer[BUF_SIZE];
            if (FD_ISSET(STDIN_FILENO, &rfds)) {
                if (fgets(&buffer[0], BUF_SIZE, stdin) == 0) {
                    done = true;
                } else {
                    (*s) << &buffer[0] << std::flush;
                }
            }
#endif
            if (FD_ISSET(sfd, &rfds)) {
                done = copy(*s, std::cout);
            }
        } else {
            perror("unknown");
        }
    }

    return 0;
}
