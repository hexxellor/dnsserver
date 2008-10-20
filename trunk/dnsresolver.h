#ifndef DNSRESOLVER_H
#define DNSRESOLVER_H

extern "C"
{
  #include <netinet/in.h>
  #include <arpa/inet.h>
}

#include <defs.h>
#include <dnsdatabasereader.h>

#define COMPRESSED_MASK 0xc000

/*
 * @class DNSResolver
 * @brief Generate the response RRs from query part of request
 */
class DNSResolver
{
  public:

    /*
     * @brief Default constructor.
     */
    DNSResolver();

    /*
     * @brief Parameter constructor.
     * @param dnsDataBaseReaderObject Pointer to dnsDataBaseReader object in memory.
     */
    DNSResolver(DNSDataBaseReader *dnsDataBaseReaderObject);

    /*
     * @brief Default destructor.
     */
    ~DNSResolver();
   
    /*
     * @brief Initializator function of queries pointer
     * @param bufferQueries Pointer to the position of queries in dnsQueryResponse buffer.
     */
    void initializeQueryBufferBegin(unsigned char *queriesBuffer);


    /*
     * @brief Pointer to the search class in data base.
     * @param bufferRRs Pointer to position in dnsQueryResponse where the RRs must be added.
     * @param nQueries Number of queries found in header.
     * @param queryLength Length of the query section.
     * @return Number bytes wroten to dnsQueryResponse buffer.
     */
    int resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries, unsigned int queryLength);

    /*
     * @brief Returns the number of answer RRs added.
     * @return Number of RRs added to response by the object.
     */
    int getResponseAnRRsNumber();

  private:
    /*
     * @brief Pointer to queries in dnsQueryResponse buffer
     */
    unsigned char *queriesPointer;

    /*
     * @brief Number of RRs written.
     */
    int newRRsNumber;

    /*
     * @brief Pointer to Databasereader object
     */
    DNSDataBaseReader *dnsDataBaseReader;

    /*
     * @brief Read and process queries in request; generate the RRs response.
     * @param queryBufferPointer Pointer to query zone in request to read data from.
     * @param query Pointer to struct where the query data must be left.
     * @param tagPointer Pointer to first place of dns name in query (for compressed labels).
     * @return Number bytes read from queryBufferPointer.
     */
    int readQueryRequest(unsigned char *queryBufferPointer, dnsQuery *query, unsigned int tagPointer);

    /*
     * @brief Add RDATA record to response packet.
     * @param rrBufferPlace Pointer to zone in dnsQueryResponse where the RDATA RR must be written.
     * @param resolvedIP Pointer to IP address to write in the RR.
     * @param tagPointer Pointer to label place of dns name in response packet (for compressed labels).
     * @return Number bytes written to destination buffer.
     */
    int addRDATARecordResponse(unsigned char *rrBufferPlace, char *resolvedIP, unsigned int tagPointer);

    /*
     * @brief Add CNAME record to response packet.
     * @param rrBufferPlace Pointer to zone in dnsQueryResponse where the CNAME RR must be written.
     * @param realName Pointer to the real name of the host.
     * @param tagPointer Pointer to label place of dns name in response packet (for compressed labels).
     * @return Number bytes written to destination buffer.
     */
    int addCNAMERecordResponse(unsigned char *rrBufferPlace, char *realName, unsigned int tagPointer);

    /*
     * @brief Convert a name from human readable string to DNS packet format.
     * @param URL Name in human readable format.
     * @param destinationBuffer Place where the name must be written.
     * @return Number bytes written to destinationBuffer.
     */
    unsigned int writeNamefromURL(char *URL, unsigned char *destinationBuffer);
};

#endif /* DNSRESOLVER_H */
