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

//void DNSResolver::setQueryBufferBegin(unsigned char *bufferQueries)
//{
//  queriesPointer = bufferQueries;
//}

int DNSResolver::resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries)
{
  dnsQuery atomicQuery;
  int responseLength = 0;
  int searchResponse = 0;

  newRRsNumber = 0;

  for (int i = 0; i < nQueries; i++)
  {
    readQueryRequest(queriesPointer, &atomicQuery);

    //SEARCHDATABASE atomicQuery.Name;!!

    printf("Name: %s, length %i",  atomicQuery.Name, strlen(atomicQuery.Name));

    searchResponse = dnsDataBaseReader->searchIPbyURL( atomicQuery.Name );

    if (searchResponse > 0 )
    {
      responseLength = responseLength + addResponseRecord(bufferRRs, dnsDataBaseReader->getFoundIP());
      printf("%s : %i\n responseLength %i\n", __FUNCTION__, __LINE__, responseLength);
    }
    else
    {
       //URL not found in data base
       return -1;
    }

  }

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

  //Return length of the query ?!?!?!?!?!?!?
  return (nameCounter + 4);
}

int DNSResolver::addResponseRecord(unsigned char *rrBufferPlace, char *resolvedIP)
{

  int lengthRecord;

  in_addr intAddress;
  dnsRecords *responseRecord = (dnsRecords *)rrBufferPlace;

  responseRecord->rName = htons((uint16_t)0xc00c);
  responseRecord->rType = htons((uint16_t)0x0001);
  responseRecord->rClass = htons((uint16_t)0x0001);

  responseRecord->rTTL = (int32_t)ntohl((int32_t)0x00000000);

  //memcpy(rrBufferPlace[],0,4);

  responseRecord->rdLength = ntohs((uint16_t)0x0004);

  if (inet_aton(resolvedIP, &intAddress) == 0)
  {
    printf("Dir IP no valida\n");
  }

  responseRecord->rRDATA = ntohl((uint32_t)intAddress.s_addr);

  //memcpy(rrBufferPlace, dnsRecords, sizeof(dnsRecords));

  newRRsNumber++;

  printf("%s : %i, sizeof %i\n", __FUNCTION__, __LINE__, sizeof(dnsRecords));

  return (sizeof(dnsRecords));
}

int DNSResolver::getResponseAnRRsNumber()
{
  return newRRsNumber;
}

