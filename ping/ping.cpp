#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>

#include <map>
#include <utility>

#include <signal.h> // to trap Ctrl+C : SIGINT

#include "skstream.h"

#include "ping.h"

using namespace std;

//---------------------------------------------------------------------------
// globals
typedef map<string,pair<unsigned, unsigned> > stat_type;
stat_type statistics;
unsigned Ping_Count, transmitted_packets;
bool Terminate;
//---------------------------------------------------------------------------
// Prototypes
unsigned short in_cksum(unsigned short*, int);
void ping(const string&);
void send_request(basic_socket_stream&, const string&);
bool recv_reply(basic_socket_stream&, ECHO_REPLY&);
void print_statistics(const ECHO_REPLY&);
void print_final_statistics();
bool is_broadcast_address();
//---------------------------------------------------------------------------
// signal handler
void CTLRC(int);
//---------------------------------------------------------------------------
int main(int argc, char** argv) {
  Ping_Count = 4;
  string host("");

  if (argc < 2) {
    cerr << "Usage: ping [-n NUM] host" << endl;
    return 1;
  }

  Terminate = false;

  transmitted_packets=0;

  for(int i=1; i < argc; i++) {
    if(string(argv[i]).compare("-n")==0) {
      if(i == argc-1) {
        cerr << "Usage: ping [-n NUM] host" << endl;
        return 1;
      }
      i++;
      char **error = NULL;
      Ping_Count = strtoul(argv[i],error,0);
      if((Ping_Count==0)||(error != NULL)) {
        cerr << "Usage: ping [-n NUM] host" << endl;
        return 1;
      }
    } else {
      host.assign(argv[i]);
    }
  }
  if(host.size() == 0){
    cerr << "Usage: ping [-n NUM] host" << endl;
    return 1;
  }

  // register SIGINT handler
  signal(SIGINT, CTLRC);

  #ifdef __BORLANDC__
    #define CLOCKS_DIV 1.0/(CLK_TCK*1.0)
  #else
    #define CLOCKS_DIV 1.0/(CLOCKS_PER_SEC*1.0)
  #endif
  cout << "Time resolution: " << CLOCKS_DIV*1000.0 << " msec." << endl;

  ping(host);

  return 0;
}
//---------------------------------------------------------------------------
void ping(const string& host) {
  raw_socket_stream ping_socket(FreeSockets::proto_ICMP);
  if(!ping_socket) {
    cerr << "Could not create raw socket." << endl;
    exit(0);
  }

  if(is_broadcast_address()) {
    if(!ping_socket.setBroadcast(true)) {
      cerr << "Could not set broadcast socket." << endl;
      exit(0);
    }
  }

  ping_socket.setOutpeer(host,FreeSockets::echo);

  unsigned nRet;

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
void send_request(basic_socket_stream& sock, const string& host) {
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

  cout << "Pinging "<<host<<"["<<inet_ntoa(sock.getOutpeer().sin_addr);
  cout << "] with " << REQ_DATASIZE << " bytes of data." << endl;

  sock.setTimeout(3,0);
  sock.write((char*)&echoReq,sizeof(ECHO_REQUEST));
  sock.flush();

  // Check for timeout
  if(sock.fail()) {
    if(sock.timeout()) {
      cerr << "Timeout sending ICMP request." << endl;
    } else {
      unsigned error = sock.getLastError();
      cerr << "SEND: Ping Error #" << error << endl;
      #ifdef __linux__
        cerr << strerror(error) << endl;
      #endif
    }
    exit(0);
  }
}

//---------------------------------------------------------------------------
bool recv_reply(basic_socket_stream& sock, ECHO_REPLY& reply)
{
  sock.setTimeout(3,0);
  sock.read((char*)&reply,sizeof(ECHO_REPLY));

  // Check for timeout
  if(sock.timeout()) return false;

  // Check for errors
  if(sock.fail()) {
    unsigned error = sock.getLastError();
    cerr << "RECV: Ping Error #" << error << endl;

    #ifdef __linux__
      cerr << strerror(error) << endl;
    #endif

    exit(0);
  }

  return true;
}

//---------------------------------------------------------------------------
void print_statistics(const ECHO_REPLY& reply) {
  // again: portability vs. precision
  clock_t elapsed = clock() - reply.echoRequest.dwTime;

  string replier(inet_ntoa(reply.ipHdr.iaSrc));

  stat_type::iterator iter = statistics.find(replier);
  if(iter == statistics.end())
    iter = statistics.insert(make_pair(replier,make_pair(0,0))).first;

  (*iter).second.first++;
  (*iter).second.second += elapsed;

  cout << "Reply from: " << replier;
  cout << " : bytes=" << REQ_DATASIZE << " time=" << elapsed << "ms";
  cout << " TTL=" << int(reply.ipHdr.TTL) << endl;
}
//---------------------------------------------------------------------------
void print_final_statistics() {
  stat_type::iterator iter = statistics.begin();

  cout << "\nTransmitted Packets: " << transmitted_packets << endl;

  while(iter != statistics.end()) {
    cout << "\nStatistics for host " << (*iter).first << ":" << endl;
    cout << "\tPackets received: " << (*iter).second.first << endl;
    double mean = (*iter).second.second/((*iter).second.first*1.0);
    cout << "\tMean ping time: " << mean*CLOCKS_DIV << " msec." << endl;
    unsigned lost_packets = transmitted_packets - (*iter).second.first;
    cout << "\tLost Packets: " << lost_packets << endl;
    cout << "\tPacket loss: "<< double(lost_packets/(transmitted_packets*1.0))*100.0 << "%" << endl;
    iter++;
  }

}

//---------------------------------------------------------------------------
void CTLRC(int) {
  cout << "Stopping service..." << endl;
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