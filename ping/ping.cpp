#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "ping.h"

#include <iostream>
#include <string>
#include <map>
#include <utility>

#include <cstdio>

#include <time.h>
#include <signal.h> // to trap Ctrl+C : SIGINT

#ifndef _WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#endif // _WIN32

//---------------------------------------------------------------------------
// globals
typedef std::map<std::string,std::pair<unsigned, unsigned> > stat_type;
stat_type statistics;
unsigned Ping_Count, transmitted_packets;
bool Terminate;
namespace FreeSockets {
  static const int echo = 7;
};
//---------------------------------------------------------------------------
// Prototypes
unsigned short in_cksum(unsigned short*, int);
void ping(const std::string&);
void send_request(raw_socket_stream&, const std::string&);
bool recv_reply(raw_socket_stream&, ECHO_REPLY&);
void print_statistics(const ECHO_REPLY&);
void print_final_statistics();
bool is_broadcast_address();
//---------------------------------------------------------------------------
// signal handler
void CTLRC(int);
//---------------------------------------------------------------------------
int main(int argc, char** argv) {
  Ping_Count = 4;
  std::string host("");

  if (argc < 2) {
    std::cerr << "Usage: ping [-n NUM] host" << std::endl;
    return 1;
  }

  Terminate = false;

  transmitted_packets=0;

  for(int i=1; i < argc; i++) {
    if(std::string(argv[i]).compare("-n")==0) {
      if(i == argc-1) {
        std::cerr << "Usage: ping [-n NUM] host" << std::endl;
        return 1;
      }
      i++;
      char **error = NULL;
      Ping_Count = strtoul(argv[i],error,0);
      if((Ping_Count==0)||(error != NULL)) {
        std::cerr << "Usage: ping [-n NUM] host" << std::endl;
        return 1;
      }
    } else {
      host.assign(argv[i]);
    }
  }
  if(host.size() == 0){
    std::cerr << "Usage: ping [-n NUM] host" << std::endl;
    return 1;
  }

  // register SIGINT handler
  signal(SIGINT, CTLRC);

  #ifdef __BORLANDC__
    #define CLOCKS_DIV 1.0/(CLK_TCK*1.0)
  #else
    #define CLOCKS_DIV 1.0/(CLOCKS_PER_SEC*1.0)
  #endif
  std::cout << "Time resolution: " << CLOCKS_DIV*1000.0 << " msec." << std::endl;

  ping(host);

  return 0;
}
//---------------------------------------------------------------------------
void ping(const std::string& host) {
  raw_socket_stream ping_socket(FreeSockets::proto_ICMP);
  if(!ping_socket) {
    std::cerr << "Could not create raw socket." << std::endl;
    exit(0);
  }

  if(is_broadcast_address()) {
    if(!ping_socket.setBroadcast(true)) {
      std::cerr << "Could not set broadcast socket." << std::endl;
      exit(0);
    }
  }

  ping_socket.setTarget(host,FreeSockets::echo);

  ECHO_REPLY reply;
  for(unsigned i=0; i < Ping_Count; i++) {
    send_request(ping_socket,host);
    transmitted_packets++;
    if(recv_reply(ping_socket,reply))
      print_statistics(reply);
  }

  while(!Terminate) {
    if(recv_reply(ping_socket,reply))
      print_statistics(reply);
    else break; // quit on timeout
  }

  print_final_statistics();
}
//---------------------------------------------------------------------------
void send_request(raw_socket_stream& sock, const std::string& host) {
  static ECHO_REQUEST echoReq;
  static int nId = 1;
  static int nSeq = 1;

  // Fill in echo request
  echoReq.icmpHdr.Type = ICMP_ECHOREQ;
  echoReq.icmpHdr.Code = 0;
  echoReq.icmpHdr.Checksum = 0;
  echoReq.icmpHdr.ID = nId++;
  echoReq.icmpHdr.Seq = nSeq++;
  echoReq.dwTime = clock();

  // Fill in some data to send
  for(int i=0; i < REQ_DATASIZE; i++)
    echoReq.cData[i] = ' '+i;

  // Put data in packet and compute checksum
  echoReq.icmpHdr.Checksum =
      in_cksum((unsigned short*)&echoReq,sizeof(ECHO_REQUEST));

  const sockaddr_storage & sst = sock.getOutpeer();

#ifndef HAVE_GETADDRINFO
#warning Legacy resolver code
  std::cout << "Pinging "<<host<<"["<<::inet_ntoa(((sockaddr_in &)sst).sin_addr);
#else // HAVE_GETADDRINFO
  char hbuf[NI_MAXHOST];

  if (::getnameinfo((const sockaddr*)&sst, sock.getOutpeerSize(),
                    hbuf, sizeof(hbuf), 0, 0, NI_NUMERICHOST) == 0) {
    std::cout << "Pinging " << host << "[" << hbuf;
  } else {
    std::cout << "Pinging [unknown";
  }
#endif // HAVE_GETADDRINFO
  std::cout << "] with " << REQ_DATASIZE << " bytes of data." << std::endl;

  sock.setTimeout(3,0);
  sock.write((char*)&echoReq,sizeof(ECHO_REQUEST));
  sock.flush();

  // Check for timeout
  if(sock.fail()) {
    if(sock.timeout()) {
      std::cerr << "Timeout sending ICMP request." << std::endl;
    } else {
      unsigned error = sock.getLastError();
      std::cerr << "SEND: Ping Error #" << error << std::endl;
      #ifdef __linux__
        std::cerr << strerror(error) << std::endl;
      #endif
    }
    exit(0);
  }
}

//---------------------------------------------------------------------------
bool recv_reply(raw_socket_stream& sock, ECHO_REPLY& reply)
{
  sock.setTimeout(3,0);
  sock.read((char*)&reply,sizeof(ECHO_REPLY));

  // Check for timeout
  if(sock.timeout()) return false;

  // Check for errors
  if(sock.fail()) {
    unsigned error = sock.getLastError();
    std::cerr << "RECV: Ping Error #" << error << std::endl;

    #ifdef __linux__
      std::cerr << strerror(error) << std::endl;
    #endif

    exit(0);
  }

  return true;
}

//---------------------------------------------------------------------------
void print_statistics(const ECHO_REPLY& reply) {
  // again: portability vs. precision
  clock_t elapsed = clock() - reply.echoRequest.dwTime;

  std::string replier;
#ifndef HAVE_GETADDRINFO
#warning Legacy resolver code
  replier = ::inet_ntoa(reply.ipHdr.iaSrc);
#else // HAVE_GETADDRINFO
  char hbuf[NI_MAXHOST];

  if (::getnameinfo((const sockaddr*)&reply.ipHdr.iaSrc,
                    sizeof(reply.ipHdr.iaSrc),
                    hbuf, sizeof(hbuf), 0, 0, NI_NUMERICHOST) == 0) {
    replier = hbuf;
  } else {
    replier = "[unknown]";
  }
#endif // HAVE_GETADDRINFO

  stat_type::iterator iter = statistics.find(replier);
  if(iter == statistics.end())
    iter = statistics.insert(std::pair<std::string, std::pair<unsigned, unsigned> >(replier,std::pair<unsigned, unsigned>(0,0))).first;

  (*iter).second.first++;
  (*iter).second.second += elapsed;

  std::cout << "Reply from: " << replier;
  std::cout << " : bytes=" << REQ_DATASIZE << " time=" << elapsed << "ms";
  std::cout << " TTL=" << int(reply.ipHdr.TTL) << std::endl;
}
//---------------------------------------------------------------------------
void print_final_statistics() {
  stat_type::iterator iter = statistics.begin();

  std::cout << "\nTransmitted Packets: " << transmitted_packets << std::endl;

  while(iter != statistics.end()) {
    std::cout << "\nStatistics for host " << (*iter).first << ":" << std::endl;
    std::cout << "\tPackets received: " << (*iter).second.first << std::endl;
    double mean = (*iter).second.second/((*iter).second.first*1.0);
    std::cout << "\tMean ping time: " << mean*CLOCKS_DIV << " msec." << std::endl;
    unsigned lost_packets = transmitted_packets - (*iter).second.first;
    std::cout << "\tLost Packets: " << lost_packets << std::endl;
    std::cout << "\tPacket loss: "<< double(lost_packets/(transmitted_packets*1.0))*100.0 << "%" << std::endl;
    iter++;
  }

}

//---------------------------------------------------------------------------
void CTLRC(int) {
  std::cout << "Stopping service..." << std::endl;
  Terminate=true;
}
//---------------------------------------------------------------------------
bool is_broadcast_address() {
  return true;
}
//---------------------------------------------------------------------------
// Mike Muuss' in_cksum() function
// and his comments from the original
// ping program
//
// * Author -
// *	Mike Muuss
// *	U. S. Army Ballistic Research Laboratory
// *	December, 1983
/*
 *			I N _ C K S U M
 *
 * Checksum routine for Internet Protocol family headers (C Version)
 *
 */
unsigned short in_cksum(unsigned short *addr, int len) {
  register int nleft = len;
  register unsigned short *w = addr;
  register unsigned short answer;
  register int sum = 0;

  /*
   *  Our algorithm is simple, using a 32 bit accumulator (sum),
   *  we add sequential 16 bit words to it, and at the end, fold
   *  back all the carry bits from the top 16 bits into the lower
   *  16 bits.
   */
  while( nleft > 1 )  {
    sum += *w++;
    nleft -= 2;
  }

  /* mop up an odd byte, if necessary */
  if( nleft == 1 ) {
    u_short	u = 0;

    *(u_char *)(&u) = *(u_char *)w ;
    sum += u;
  }

  /*
   * add back carry outs from top 16 bits to low 16 bits
   */
  sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
  sum += (sum >> 16);			/* add carry */
  answer = ~sum;			/* truncate to 16 bits */
  return (answer);
}
