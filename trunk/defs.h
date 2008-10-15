#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

//Maximum size for DNS UDP is 512 bytes (RFC1035)
#define DNS_PKT_SIZE 512

struct dnsHeader
{
  uint16_t idDnsQuery;
  uint16_t headerFlags;
  uint16_t qdCount;
  uint16_t anCount;
  uint16_t nsCount;
  uint16_t arCount;
};

struct dnsQuery
{
  char Name[255];
  uint16_t rrType;
  uint16_t rrClass;
};

//For struct alignment
#pragma pack (2)
struct dnsRecords
{
  uint16_t rName;
  uint16_t rType;
  uint16_t rClass;
  uint32_t rTTL;
  uint16_t rdLength;
  uint32_t rRDATA;
};

struct dnsPkt
{
  struct dnsHeader header;
  struct dnsQuery *queries;
  struct dnsRecords *rrs;
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

//RR request types
enum RR_TYPE
{
  A_TYPE = 1,
  NS_TYPE = 2,
  MD_TYPE = 3,
  MF_TYPE = 4,
  CNAME_TYPE = 5,
  SOA_TYPE = 6,
  MB_TYPE = 7,
  MG_TYPE = 8,
  MR_TYPE = 9,
  NULL_TYPE = 10,
  WKS_TYPE = 11,
  PTR_TYPE = 12,
  HINFO_TYPE = 13,
  MINFO_TYPE = 14,
  MX_TYPE = 15,
  TXT_TYPE = 16
};

//Queries classes
enum QCLASS
{
  IN_CLASS = 1,
  CS_CLASS = 2,
  CH_CLASS = 3,
  HS_CLASS = 4
};

#define NAME_NOT_FOUND -1
#define NAME_IS_URL 0
#define NAME_IS_ALIAS 1

#endif /* DEFS_H */
