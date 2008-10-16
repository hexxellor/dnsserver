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

int DNSResolver::resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries)
{
  dnsQuery atomicQuery;
  int responseLength = 0;
  //int searchResponse = 0;

  newRRsNumber = 0;

  for (int i = 0; i < nQueries; i++)
  {
    readQueryRequest(queriesPointer, &atomicQuery);

    //searchResponse = dnsDataBaseReader->searchIPbyURL( atomicQuery.Name );
    switch (dnsDataBaseReader->searchIPbyURL( atomicQuery.Name ))
    {
      case NAME_IS_AN_URL:
      {
        responseLength = responseLength + addRDATARecordResponse(bufferRRs, &atomicQuery);
        break;
      }
      case NAME_IS_AN_ALIAS:
      {
        responseLength = responseLength + addCNAMERecordResponse(bufferRRs, &atomicQuery);
        responseLength = responseLength + addRDATARecordResponse(bufferRRs, &atomicQuery);
        break;
      }
      default:
      {
        return NAME_NOT_FOUND;      
      }
    }  //switch
  }  //for
  return responseLength;
}

int DNSResolver::readQueryRequest(unsigned char*queryBufferPointer, dnsQuery *query)
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

  return (nameCounter + 4);
}

//Add RDATA record
int DNSResolver::addRDATARecordResponse(unsigned char *rrBufferPlace, char *resolvedIP)
{
  int lengthRecord;

  in_addr intAddress;
  dnsRDataRecord *responseRecord = (dnsRDataRecord *)rrBufferPlace;

  responseRecord->rName = htons((uint16_t)(COMPRESSED_MASK + 12));
  responseRecord->rType = htons((uint16_t) A_TYPE);
  responseRecord->rClass = htons((uint16_t)0x0001);

  responseRecord->rTTL = (int32_t)ntohl((int32_t)0x00000000);

  //memcpy(rrBufferPlace[],0,4);

  responseRecord->rdLength = ntohs((uint16_t)0x0004);

  if (inet_aton(resolvedIP, &intAddress) == 0)
  {
    printf("Dir IP no valida\n");
  }

  responseRecord->rRDATA = (uint32_t)intAddress.s_addr;

  //memcpy(rrBufferPlace, dnsRecords, sizeof(dnsRecords));

  newRRsNumber++;

  return (sizeof(dnsRDataRecord));
}

//Add CNAME record
int DNSResolver::addCNAMERecordResponse(unsigned char *rrBufferPlace)
{
  unsigned int lengthRecord = 0;

  in_addr intAddress;
  dnsCNameRecord *responseRecord = (dnsCNameRecord *)rrBufferPlace;

  responseRecord->rName = htons((uint16_t) 0xc00c);
  responseRecord->rType = htons((uint16_t) CNAME_TYPE);
  responseRecord->rClass = htons((uint16_t) 0x0001);

  responseRecord->rTTL = (int32_t)ntohl((int32_t)0x00000000);

  lengthRecord = writeNamefromURL(dnsDataBaseReader->getRealName(), &rrBufferPlace[sizeof(dnsCNameRecord)]);

  responseRecord->rdLength = ntohs((uint16_t)lengthRecord);

  return (sizeof(dnsCNameRecord) + lengthRecord);
}

unsigned int DNSResolver::writeNamefromURL(char *URL, char *destinationBuffer)
{

  char *nextDot , *prevDot = URL;
  int numBytes = 1;
  
  nextDot = strchr(URL,'.');
  destinationBuffer[0] = abs(nextDot - prevStep);
  while (nextDot != NULL)
  {
    strncpy(&destinationBuffer[numBytes], prevDot, abs(prevDot - nextDot));
    numBytes = numBytes + abs(prevDot - nextDot);
    prevDot = nextDot + 1;
    nextDot = strchr(URL,'.');
    destinationBuffer[numBytes] = abs(nextDot - prevStep);
    numBytes++;
  }

  //Copy of the name last tag
  nextDot = strchr(prevDot,'\0');
  strncpy(&destinationBuffer[numBytes], prevDot, abs(prevDot - nextDot));
  numBytes = numBytes + abs(prevDot - nextDot);
  destinationBuffer[numBytes] = '\0';
  return (numBytes +1);
}

int DNSResolver::getResponseAnRRsNumber()
{
  return newRRsNumber;
}

