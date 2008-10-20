#ifndef DEFS_H
#define DEFS_H

extern "C"
{
  #include <stdint.h>
  //To include in all files
  #include <string.h>
  #include <stdlib.h>
}

//Maximum size for DNS UDP is 512 bytes (RFC1035)
#define DNS_PKT_SIZE 512
#define DNS_HEADER_LENGTH 12

#pragma pack (2)
struct dnsHeader
{
  uint16_t idDnsQuery;
  uint16_t headerFlags;
  uint16_t qdCount;
  uint16_t anCount;
  uint16_t nsCount;
  uint16_t arCount;
};

//For struct packing
#pragma pack (2)
struct dnsRDataRecord
{
  uint16_t rName;
  uint16_t rType;
  uint16_t rClass;
  uint32_t rTTL;
  uint16_t rdLength;
  uint32_t rRDATA;
};

//For struct packing
#pragma pack (2)
struct dnsCNameRecord
{
  uint16_t rName;
  uint16_t rType;
  uint16_t rClass;
  uint32_t rTTL;
  uint16_t rdLength;
  //RDATA will be generated in buffer in running time
};

struct dnsQuery
{
  char Name[255];
  uint16_t rrType;
  uint16_t rrClass;
};

//DNS Header error codes
enum RCODE
{
  NO_ERROR = 0,
  FORMAT_ERROR = 1,
  SERVER_FAILURE = 2,
  NAME_ERROR = 3,
  NOT_IMPLEMENTED = 4,
  REFUSED = 5
};

#define A_TYPE 1
#define CNAME_TYPE 5

#define IN_CLASS 1

//Possible returns of 
#define NAME_IS_AN_URL 1
#define NAME_IS_AN_ALIAS 2
#define NAME_NOT_FOUND -1

//Errors processing the query
#define MALFORMED_QUERY -2
#define UNSUPPORTED_RRTYPE_RRCLASS -3
#define SERVER_INTERNAL_ERROR -4

#endif /* DEFS_H */
