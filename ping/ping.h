#ifndef RGJ_FREE_SOCKETS_PING_EXAMPLE
#define RGJ_FREE_SOCKETS_PING_EXAMPLE

#include "skstream.h"

#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

// IP Header -- RFC 791
struct IP_HEADER
{
  unsigned char  VIHL;          // Version and IHL
  unsigned char  TOS;           // Type Of Service
  short          TotLen;        // Total Length
  short          ID;            // Identification
  short          FlagOff;       // Flags and Fragment Offset
  unsigned char  TTL;           // Time To Live
  unsigned char  Protocol;      // Protocol
  unsigned short Checksum;      // Checksum
  in_addr        iaSrc;         // Internet Address - Source
  in_addr        iaDst;         // Internet Address - Destination
};


// ICMP Header - RFC 792
struct ICMP_HEADER
{
  unsigned char         Type;           // Type
  unsigned char         Code;           // Code
  unsigned short        Checksum;       // Checksum
  unsigned short        ID;             // Identification
  unsigned short        Seq;            // Sequence
  char                  Data;           // Data
};


// ICMP Echo Request
#define REQ_DATASIZE 32         // Echo Request Data size

struct ECHO_REQUEST {
  ICMP_HEADER   icmpHdr;
  unsigned long dwTime;
  char          cData[REQ_DATASIZE];
};


// ICMP Echo Reply
struct ECHO_REPLY {
  IP_HEADER     ipHdr;
  ECHO_REQUEST  echoRequest;
};

#endif

