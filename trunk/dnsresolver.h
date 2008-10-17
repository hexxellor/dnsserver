#ifndef DNSRESOLVER_H
#define DNSRESOLVER_H

extern "C"
{
  #include <netinet/in.h>
  #include <arpa/inet.h>
}

#include <defs.h>
#include <dnsdatabasereader.h>

#include <string>

#define COMPRESSED_MASK 0xc000

/*
 * @class DNSResolver
 * @brief Generate the response RRs from query part of request
 */
class DNSResolver
{
  public:

    DNSResolver();

    DNSResolver(unsigned char *bufferQueries);

    ~DNSResolver();
   
    /*
     * @brief Pointer to the search class in data base
     * @param bufferRRs Number of RRs added to response
     * @param nQueries Place where the name must be wroten
     * @param queryLength Place where the name must be wroten
     * @return Number bytes wroten to destinationBuffer
     */
    int resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries, unsigned int queryLength);

    /*
     * @brief Returns the number of answer RRs added
     * @return Number of RRs added to response
     */
    int getResponseAnRRsNumber();

  private:

    unsigned char *queriesPointer;

    int newRRsNumber;

    /*
     * @brief Pointer to data base reader class
     */
    DNSDataBaseReader *dnsDataBaseReader;

    /*
     * @brief Read and process queries in request; generate the RRs response
     * @param URL Number of RRs added to response
     * @param destinationBuffer Place where the name must be wroten
     * @param destinationBuffer Place where the name must be wroten
     * @return Number bytes wroten to destinationBuffer
     */
    int readQueryRequest(unsigned char*queryBufferPointer, dnsQuery *query, unsigned int tPointer);

    /*
     * @brief Add RDATA record to response packet
     * @param URL Number of RRs added to response
     * @param destinationBuffer Place where the name must be wroten
     * @return Number bytes wroten to destinationBuffer
     */
    int addRDATARecordResponse(unsigned char *rrBufferPlace, char *resolvedIP, unsigned int tagPointer);

    /*
     * @brief Add CNAME record to response packet
     * @param URL Number of RRs added to response
     * @param destinationBuffer Place where the name must be wroten
     * @return Number bytes wroten to destinationBuffer
     */
    int addCNAMERecordResponse(unsigned char *rrBufferPlace, char *realName, unsigned int tagPointer);

    /*
     * @brief Add response record to response packet
     * @param URL Number of RRs added to response
     * @param destinationBuffer Place where the name must be wroten
     * @return Number bytes wroten to destinationBuffer
     */
    unsigned int writeNamefromURL(char *URL, unsigned char *destinationBuffer);
};

#endif /* DNSRESOLVER_H */
