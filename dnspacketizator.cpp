#include <dnspacketizator.h>

DNSPacketizator::DNSPacketizator()
{
  idDnsQuery = 0;
  qdCount = 0;
  dnsError = NO_ERROR;
}

DNSPacketizator::DNSPacketizator(DNSResolver *dnsResolverObject)
{
  idDnsQuery = 0;
  qdCount = 0;
  dnsError = NO_ERROR;

  dnsResolver = dnsResolverObject;
  dnsResolver->initializeQueryBufferBegin(dnsQueryResponse + DNS_HEADER_LENGTH);
}

DNSPacketizator::~DNSPacketizator()
{

}

void DNSPacketizator::initializeBufferPointer(unsigned char *msg)
{
  dnsQueryRequest = msg;
}

void DNSPacketizator::processDnsQuery(int dnsPktLength)
{

  unsigned int dnsResponseRRsLength = 0;

  //Set the error state to NO ERROR
  dnsError = NO_ERROR;
  dnsRequestLength = dnsPktLength;
  dnsResponseLength = dnsRequestLength;

  //Then study the request. Is it right?
  if (dnsRequestLength >= DNS_HEADER_LENGTH)
  {
    //First intialize answer packet
    initializeDnsQueryResponse();
    //Process header
    processDnsHeader(dnsQueryRequest);
  }
  else
  {
    //Create minimum info to generate a header and exit!
    qdCount = 0;
    idDnsQuery = ntohs(*(uint16_t *)dnsQueryRequest);
    dnsResponseLength = DNS_HEADER_LENGTH;    
    dnsError = FORMAT_ERROR;
  }

  if (dnsError == NO_ERROR)
  {
    switch (dnsResponseRRsLength = dnsResolver->resolveQueryRequest(dnsQueryResponseRRsPointer, qdCount, dnsRequestLength))
    {
      case NAME_NOT_FOUND:
      {
        dnsError = NAME_ERROR;   
        break;
      }
      case MALFORMED_QUERY:
      {
        dnsError = FORMAT_ERROR;
        //If there are a malformed query, we return only the header
        dnsResponseLength = DNS_HEADER_LENGTH;
        qdCount = 0;
        break;
      }
      case UNSUPPORTED_RRTYPE_RRCLASS:
      {
        dnsError = NOT_IMPLEMENTED;
        break;
      }
      default:
      {
        dnsResponseLength = dnsResponseLength + dnsResponseRRsLength;
      }
    }  //switch

  }

  generateDnsResponseHeader(dnsQueryResponse);

}

void DNSPacketizator::processDnsHeader(unsigned char *msgPointer)
{
  uint16_t anCount = 0;
  uint16_t nsCount = 0;
  uint16_t arCount = 0;

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

  if((anCount != 0)||(nsCount != 0)||(arCount != 0))
  {
    dnsError = FORMAT_ERROR;
  }

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

  //Set the header counters to their correct values
  responseHeader->qdCount = htons((uint16_t)qdCount);

  if ( dnsError == NO_ERROR )
  {
    responseHeader->anCount = htons( (uint16_t) dnsResolver->getResponseAnRRsNumber() );
  }
  else
  {
    responseHeader->anCount = 0;
  }

  responseHeader->nsCount = 0;
  responseHeader->arCount = 0;
}

void DNSPacketizator::initializeDnsQueryResponse()
{
  //Copy of query section from response to request.
  memcpy(&dnsQueryResponse[DNS_HEADER_LENGTH], &dnsQueryRequest[DNS_HEADER_LENGTH], (dnsRequestLength - DNS_HEADER_LENGTH));
  dnsQueryResponseRRsPointer = &dnsQueryResponse[dnsRequestLength];
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

  return headerFlags;
}

unsigned char *DNSPacketizator::getDnsResponse()
{
  return dnsQueryResponse;
}

int DNSPacketizator::getDnsResponseLength()
{
  return dnsResponseLength;
}

