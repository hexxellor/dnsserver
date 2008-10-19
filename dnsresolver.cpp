#include <dnsresolver.h>

DNSResolver::DNSResolver()
{
  newRRsNumber = 0;
  queriesPointer = NULL;
}

DNSResolver::DNSResolver(DNSDataBaseReader *dnsDataBaseReaderObject)
{
  newRRsNumber = 0;
  dnsDataBaseReader = dnsDataBaseReaderObject;
  queriesPointer = NULL;
}

DNSResolver::~DNSResolver()
{

}

void DNSResolver::initializeQueryBufferBegin(unsigned char *queriesBuffer)
{
  queriesPointer = queriesBuffer;
}

int DNSResolver::resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries, unsigned int queryLength)
{
  dnsQuery atomicQuery;

  int responseLength = 0;
  int requestLength = 0;
  int bytesRequest = 0;
  int bytesResponse = 0;

  int numberIPsResolved;

  int compressedPointer;

  unsigned char *actualRR = bufferRRs;
  unsigned char *actualQuery = queriesPointer;

  newRRsNumber = 0;

  for (int i = 0; i < nQueries; i++)
  {
    bytesRequest = readQueryRequest(actualQuery, &atomicQuery,   queryLength - DNS_HEADER_LENGTH  );

    if (bytesRequest == MALFORMED_QUERY)
    {
      return MALFORMED_QUERY;
    }

    compressedPointer = abs(queriesPointer - actualQuery) + DNS_HEADER_LENGTH;

    requestLength = requestLength + bytesRequest;
    actualQuery = actualQuery + bytesRequest;

    switch (dnsDataBaseReader->searchIPbyURL(atomicQuery.Name))
    {
      case NAME_IS_AN_URL:
      {
        //Number of IPs asociated with the URL
        numberIPsResolved = dnsDataBaseReader->getNumberofIPs();
        //Add a RDATA record for every IP found
        for (int jRR = 0; jRR < numberIPsResolved; jRR++)
        {
          bytesResponse = addRDATARecordResponse(actualRR, dnsDataBaseReader->getFoundIP(jRR), compressedPointer);
          //Actualize counters
          responseLength = responseLength + bytesResponse;
          actualRR = actualRR + bytesResponse;
          newRRsNumber++;
        }
        break;
      }
      case NAME_IS_AN_ALIAS:
      {
        //Add a CNAME record
        bytesResponse = addCNAMERecordResponse(actualRR, dnsDataBaseReader->getRealName(), compressedPointer);

        //Actualize some values to add an RDATA record     
        responseLength = responseLength + bytesResponse;
        compressedPointer = abs(actualRR - bufferRRs) + queryLength + sizeof(dnsCNameRecord);
        actualRR = actualRR + bytesResponse;
        newRRsNumber++;

        //Number of IPs asociated with the URL
        numberIPsResolved = dnsDataBaseReader->getNumberofIPs();
        //Add a RDATA record for every IP found
        for (int jRR = 0; jRR < numberIPsResolved; jRR++)
        {
          bytesResponse = addRDATARecordResponse(actualRR, dnsDataBaseReader->getFoundIP(jRR), compressedPointer);
          //Actualize counters
          responseLength = responseLength + bytesResponse;
          actualRR = actualRR + bytesResponse;
          newRRsNumber++;
        }

        break;
      }
      default:
      {
        return NAME_NOT_FOUND;      
      }
    }  //switch

  }  //for

  if (requestLength != (queryLength - DNS_HEADER_LENGTH))
  {
    return MALFORMED_QUERY;
  }

  return responseLength;
}

int DNSResolver::readQueryRequest(unsigned char*queryBufferPointer, dnsQuery *query, unsigned int bufferSize)
{
  int nameCounter;

  uint8_t labelLength = (uint8_t)queryBufferPointer[0];
  for (nameCounter = 0; labelLength != 0; nameCounter++)
  {
    strncpy(&query->Name[nameCounter], (char *)&queryBufferPointer[nameCounter+1], labelLength);
    nameCounter = nameCounter + labelLength;
    labelLength = (uint8_t)queryBufferPointer[nameCounter + 1];
    query->Name[nameCounter] = '.';
    //Maximum size for labels and FQDN (RFC1035)
    //If the recepction buffer is over-readed => Malformed query
    if ((labelLength > 63)||(nameCounter > 255)|| (nameCounter +4 > bufferSize))
    {
      return MALFORMED_QUERY;
    }
  }

  query->Name[nameCounter - 1] = '\0';

  query->rrType = ntohs(*(uint16_t *)&queryBufferPointer[nameCounter+1]);
  query->rrClass = ntohs(*(uint16_t *)&queryBufferPointer[nameCounter+3]);

  return (nameCounter + 5);
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

  //Tell the client not to cache the IP
  responseRecord->rTTL = htonl(0);

  responseRecord->rdLength = htons((uint16_t)sizeof(intAddress.s_addr));

  if (inet_aton(resolvedIP, &intAddress) == 0)
  {
    printf("DataBase returned a not valid IP\n");
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

  //Tell the client not to cache the IP
  responseRecord->rTTL = htonl((uint32_t)0);

  lengthRecord = writeNamefromURL(realName, (rrBufferPlace + sizeof(dnsCNameRecord)));

  responseRecord->rdLength = htons((uint16_t)lengthRecord);

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
    strncpy((char *)&destinationBuffer[numBytes], prevDot, abs(prevDot - nextDot));
    numBytes = numBytes + abs(prevDot - nextDot);
    prevDot = nextDot + 1;
    nextDot = strchr(prevDot,'.');
    destinationBuffer[numBytes] = abs(nextDot - prevDot);
    numBytes++;
  }

  //Copy of the name last tag
  nextDot = strchr(prevDot,'\0');
  strncpy((char *)&destinationBuffer[numBytes], prevDot, abs(prevDot - nextDot));
  destinationBuffer[numBytes - 1] = abs(nextDot - prevDot);
  numBytes = numBytes + abs(prevDot - nextDot);
  destinationBuffer[numBytes] = '\0';

  return (numBytes + 1);
}

int DNSResolver::getResponseAnRRsNumber()
{
  return newRRsNumber;
}

