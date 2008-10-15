#include <dnspacketizator.h>

DNSPacketizator::DNSPacketizator()
{
  idDnsQuery = 0;
  dnsError = NO_ERROR;
}

DNSPacketizator::DNSPacketizator(unsigned char *msg)
{
  idDnsQuery = 0;
  dnsError = NO_ERROR;

  dnsQueryRequest = msg;

  dnsResolver = new DNSResolver(dnsQueryResponse + 12 );

  //dnsResolver->setQueryBufferBegin(dnsQueryResponse + 12);
}

DNSPacketizator::~DNSPacketizator()
{

}

void DNSPacketizator::processDnsQuery(int dnsPktLength)
{
  //Set the error state to NO ERROR
  dnsError = NO_ERROR;
  dnsRequestLength = dnsPktLength;
  dnsResponseLength = dnsRequestLength;

  unsigned int dnsResponseRRsLength = 0;

  //First intialize answer packet
  initializeDnsQueryResponse();

  //Then study the request. Is it right?
  processDnsHeader(dnsQueryRequest);

  if (dnsError == NO_ERROR)
  {

    if ( (dnsResponseRRsLength = dnsResolver->resolveQueryRequest(dnsQueryRRsPointer, qdCount)) == -1 )
    {
       // URL/Alias not found!
       dnsError = NAME_ERROR;
    }
    else
    {
      dnsResponseLength = dnsResponseLength + dnsResponseRRsLength;
    }

  }

  generateDnsResponseHeader(dnsQueryResponse);

  printf("%s : %i\n", __FUNCTION__, __LINE__);

}

void DNSPacketizator::processDnsHeader(unsigned char *msgPointer)
{
  //Cast to header struct
  dnsHeader *headerPointer = (dnsHeader *)msgPointer;

  //Setting of DNS variables
  idDnsQuery = ntohs(headerPointer->idDnsQuery);

  headerFlags = ntohs(headerPointer->headerFlags);

  processDnsHeaderFlags(headerFlags);

  qdCount = ntohs(headerPointer->qdCount);
  anCount = ntohs(headerPointer->anCount);
  nsCount = ntohs(headerPointer->nsCount);
  arCount = ntohs(headerPointer->arCount);

}

void DNSPacketizator::processDnsHeaderFlags(unsigned int headerFlags)
{
  //If is not a standard query => not implemented function
  if ((headerFlags & 0x7800) != 0)
  {
    dnsError = NOT_IMPLEMENTED;
  }
}

void DNSPacketizator::generateDnsResponseHeader(unsigned char *msgPointer)
{
  //Cast to header struct
  dnsHeader *responseHeader = (dnsHeader *)msgPointer;

  responseHeader->idDnsQuery = htons((uint16_t)idDnsQuery);

  //Write header flags  
  responseHeader->headerFlags = htons(generateResponseHeaderFlags());

  responseHeader->qdCount = htons((uint16_t)qdCount);

  addNewRRstoCounters();

  printf("Nuevos valores: \n anCount: %i\n", anCount);

  responseHeader->anCount = htons((uint16_t)anCount);
  responseHeader->nsCount = htons((uint16_t)nsCount);
  responseHeader->arCount = htons((uint16_t)arCount);
}

void DNSPacketizator::addNewRRstoCounters()
{
  anCount = anCount + dnsResolver->getResponseAnRRsNumber();
}

void DNSPacketizator::initializeDnsQueryResponse()
{
  //Copy of query section from response to request.
  memcpy(&dnsQueryResponse[12], &dnsQueryRequest[12], (dnsRequestLength - 12));
  dnsQueryRRsPointer = &dnsQueryResponse[dnsRequestLength];
}

uint16_t DNSPacketizator::generateResponseHeaderFlags()
{
  //This is the response mask for:
  // -Response message
  // -Standard query
  // -Server not authoritative
  // -Recursive answer 
  uint16_t headerFlags = 0x8180;
  //To set the error condition in server
  headerFlags = headerFlags + dnsError;
}

unsigned char *DNSPacketizator::getDnsResponse()
{
  return dnsQueryResponse;
}

int DNSPacketizator::getDnsResponseLength()
{

  printf("%s : %i, dnsResponseLength %i\n", __FUNCTION__, __LINE__, dnsResponseLength);

  return dnsResponseLength;
}

