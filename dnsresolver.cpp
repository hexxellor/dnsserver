#include <dnsresolver.h>

DNSResolver::DNSResolver()
{
  newRRsNumber = 0;
}

DNSResolver::DNSResolver(unsigned char *bufferQueries)
{
  newRRsNumber = 0;
  queriesPointer = bufferQueries;
  dnsDataBaseReader = new DNSDataBaseReader();
}


DNSResolver::~DNSResolver()
{

}

int DNSResolver::resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries, unsigned int queryLength)
{
  dnsQuery atomicQuery;

  int responseLength = 0;
  int requestLength = 0;
  int bytesRequest = 0;
  int bytesResponse = 0;

  unsigned char *actualRR = bufferRRs;
  unsigned char *actualQuery = queriesPointer;

  newRRsNumber = 0;

  for (int i = 0; i < nQueries; i++)
  {
    bytesRequest = readQueryRequest(actualQuery, &atomicQuery, requestLength + 12);
    requestLength = requestLength + bytesRequest;
    actualQuery = actualQuery + bytesRequest;

    switch (dnsDataBaseReader->searchIPbyURL(atomicQuery.Name))
    {
      case NAME_IS_AN_URL:
      {
        //Add a RDATA record
        bytesResponse = addRDATARecordResponse(actualRR, dnsDataBaseReader->getFoundIP(), atomicQuery.tagPointer);          
        break;
      }
      case NAME_IS_AN_ALIAS:
      {
        //Add a CNAME record
        bytesResponse = addCNAMERecordResponse(actualRR, dnsDataBaseReader->getRealName(), atomicQuery.tagPointer);
        //Actualize the tag pointer to an actual value for RDATA
        atomicQuery.tagPointer = abs(actualRR - queriesPointer) + 12;
        actualRR = actualRR + bytesResponse;
        //Add a RDATA record
        bytesResponse = addRDATARecordResponse(bufferRRs, dnsDataBaseReader->getFoundIP(), atomicQuery.tagPointer);
        break;
      }
      default:
      {
        return NAME_NOT_FOUND;      
      }
    }  //switch

    newRRsNumber++;
    responseLength = responseLength + bytesResponse;
    actualRR = actualRR + bytesResponse;

  }  //for

  if (requestLength != queryLength)
  {
    return MALFORMED_QUERY;
  }

  return responseLength;
}

int DNSResolver::readQueryRequest(unsigned char*queryBufferPointer, dnsQuery *query, unsigned int tPointer)
{
  int nameCounter;

  uint8_t queryLength = (uint8_t)queryBufferPointer[0];

  for (nameCounter = 0; queryLength != 0; nameCounter++)
  {
    strncpy(&query->Name[nameCounter], (char *)&queryBufferPointer[nameCounter+1], queryLength);
    nameCounter = nameCounter + queryLength;
    queryLength = (uint8_t)queryBufferPointer[nameCounter + 1];
    query->Name[nameCounter] = '.';
  }

  query->Name[nameCounter - 1] = '\0';

  query->rrType = ntohs(*(uint16_t *)&queryBufferPointer[nameCounter+1]);
  query->rrClass = ntohs(*(uint16_t *)&queryBufferPointer[nameCounter+3]);

  query->tagPointer = tPointer;

  return (nameCounter + 4);
}

//Add RDATA record
int DNSResolver::addRDATARecordResponse(unsigned char *rrBufferPlace, char *resolvedIP, unsigned int tagPointer)
{
  int lengthRecord;

  in_addr intAddress;
  dnsRDataRecord *responseRecord = (dnsRDataRecord *)rrBufferPlace;

  responseRecord->rName = htons((uint16_t)(COMPRESSED_MASK + tagPointer));
  responseRecord->rType = htons((uint16_t) A_TYPE);
  responseRecord->rClass = htons((uint16_t)0x0001);

  responseRecord->rTTL = (int32_t)ntohl((int32_t)0x00000000);

  responseRecord->rdLength = ntohs((uint16_t)0x0004);

  if (inet_aton(resolvedIP, &intAddress) == 0)
  {
    printf("Dir IP no valida\n");
  }

  responseRecord->rRDATA = (uint32_t)intAddress.s_addr;


  return (sizeof(dnsRDataRecord));
}

//Add CNAME record
int DNSResolver::addCNAMERecordResponse(unsigned char *rrBufferPlace, char *realName, unsigned int tagPointer)
{
  unsigned int lengthRecord = 0;

  in_addr intAddress;
  dnsCNameRecord *responseRecord = (dnsCNameRecord *)rrBufferPlace;

  responseRecord->rName = htons((uint16_t)(COMPRESSED_MASK + tagPointer));
  responseRecord->rType = htons((uint16_t) CNAME_TYPE);
  responseRecord->rClass = htons((uint16_t) 0x0001);

  responseRecord->rTTL = (int32_t)ntohl((int32_t)0x00000000);

  lengthRecord = writeNamefromURL(realName, &rrBufferPlace[sizeof(dnsCNameRecord)]);

  responseRecord->rdLength = ntohs((uint16_t)lengthRecord);

  //We prepare the pointer for the RDATA response
  tagPointer = 12 + abs(&rrBufferPlace[sizeof(dnsCNameRecord)] - rrBufferPlace);

  return (sizeof(dnsCNameRecord) + lengthRecord);
}

unsigned int DNSResolver::writeNamefromURL(char *URL, unsigned char *destinationBuffer)
{
  char *nextDot , *prevDot = URL;
  int numBytes = 1;
  
  nextDot = strchr(URL,'.');
  destinationBuffer[0] = abs(nextDot - prevDot);
  while (nextDot != NULL)
  {
    strncpy(&(char)destinationBuffer[numBytes], prevDot, abs(prevDot - nextDot));
    numBytes = numBytes + abs(prevDot - nextDot);
    prevDot = nextDot + 1;
    nextDot = strchr(URL,'.');
    destinationBuffer[numBytes] = abs(nextDot - prevDot);
    numBytes++;
  }

  //Copy of the name last tag
  nextDot = strchr(prevDot,'\0');
  strncpy(&(char)destinationBuffer[numBytes], prevDot, abs(prevDot - nextDot));
  numBytes = numBytes + abs(prevDot - nextDot);
  destinationBuffer[numBytes] = '\0';
  return (numBytes +1);
}

int DNSResolver::getResponseAnRRsNumber()
{
  return newRRsNumber;
}

