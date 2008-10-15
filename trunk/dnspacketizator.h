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

    DNSPacketizator(unsigned char *msg);

    ~DNSPacketizator();

    /*
     * @brief Read a DNS query and set the correct flags
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

//    void setQueryRequestBuffer(unsigned char *msg);

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
    unsigned char *dnsQueryRRsPointer;

    /*
     * @brief Header variables
     */
    uint16_t idDnsQuery;
    uint16_t headerFlags;
    uint16_t qdCount;
    uint16_t anCount;
    uint16_t nsCount;
    uint16_t arCount;

    /*
     * @brief Store the dns message error status
     */
    RCODE dnsError;

    /*
     * @brief Procces the query request header 
     */
    void processDnsHeader(unsigned char *msgPointer);

    /*
     * @brief Procces the query request header flags
     */
    void processDnsHeaderFlags(unsigned int headerFlags);

    /*
     * @brief Generate header for response
     */
    void generateDnsResponseHeader(unsigned char *msgPointer);

    /*
     * @brief Actualize the RRs counter for response
     */
    void addNewRRstoCounters();

    /*
     * @brief Initialize the response (pointers ans query part)
     */
    void initializeDnsQueryResponse();

    /*
     * @brief Generate the header flags for response
     * @return A 16 bits word with the response flags
     */
    uint16_t generateResponseHeaderFlags();

    /*
     * @brief Object to resolve the query
     */
    DNSResolver *dnsResolver;
};
#endif /*DNSPACKETIZATOR_H*/

