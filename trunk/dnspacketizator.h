#ifndef DNSPACKETIZATOR_H
#define DNSPACKETIZATOR_H

extern "C"
{
  #include <stdio.h>
  #include <netinet/in.h>
  #include <stdlib.h>  
  #include <string.h>
}

#include <defs.h>
#include <dnsresolver.h>

class DNSPacketizator
{

  public:

    /*
     * @brief Default constructor
     */
    DNSPacketizator();

    /*
     * @brief Parameter constructor.
     * @param dnsResolverObject Pointer to dnsResolver object in memory.
     */
    DNSPacketizator(DNSResolver *dnsResolverObject);

    /*
     * @brief Default destructor
     */
    ~DNSPacketizator();

    /*
     * @brief Initializator function of message pointer
     * @param msg Pointer to received DNS request
     */
    void initializeBufferPointer(unsigned char *msg);

    /*
     * @brief Read a DNS query and set the object variables
     * @param dnsPktLength Lenght of dns query request in bytes.
     */
    void processDnsQuery(int dnsPktLength);

    /*
     * @brief Returns generated query response
     * @return A pointer to generated query response
     */
    unsigned char *getDnsResponse();

    /*
     * @brief Returns length of generated query response
     * @return Length of generated DNS response
     */
    int getDnsResponseLength();

  private:

    /*
     * @brief Length in bytes of DNS request
     */
    int dnsRequestLength;

    /*
     * @brief Length in bytes of DNS response
     */
    int dnsResponseLength;

    /*
     * @brief Pointer to beginning of DNS query
     */
    unsigned char *dnsQueryRequest;

    /*
     * @brief Buffer to fill with dns response
     */
    unsigned char dnsQueryResponse[DNS_PKT_SIZE];

    /*
     * @brief Pointer to rr zone in dns response buffer
     */
    unsigned char *dnsQueryResponseRRsPointer;

    /*
     * @brief Header variables
     */
    uint16_t idDnsQuery;
    uint16_t headerFlags;
    uint16_t qdCount;

    /*
     * @brief Dns server error status
     */
    RCODE dnsError;

    /*
     * @brief Procces the query request header.
     * @param msgPointer Pointer to header.
     */
    void processDnsHeader(unsigned char *msgPointer);

    /*
     * @brief Procces the query request header flags.
     * @param headerFlags Two words with the flags part of the header (3rd and 4th bytes).
     */
    void processDnsHeaderFlags(unsigned int headerFlags);

    /*
     * @brief Generate header for response.
     * @param msgPointer Pointer to dnsQueryResponse header zone.
     */
    void generateDnsResponseHeader(unsigned char *msgPointer);

    /*
     * @brief Initialize the response (pointers ans query part).
     */
    void initializeDnsQueryResponse();

    /*
     * @brief Generate the header flags for response.
     * @return A 16 bits word with the response flags.
     */
    uint16_t generateResponseHeaderFlags();

    /*
     * @brief Pointer to DNSResolver object.
     */
    DNSResolver *dnsResolver;
};
#endif /*DNSPACKETIZATOR_H*/

