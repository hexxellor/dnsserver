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

#define COMPRESSED_MASK 0xc000;

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
     */
    int resolveQueryRequest(unsigned char *bufferRRs, unsigned int nQueries);

    //void setQueryBufferBegin(unsigned char *bufferQueries);

    /*
     * @brief Returns the number of answer RRs added
     * @return Number of RRs added to response
     */
    int getResponseAnRRsNumber();

  private:

    unsigned char *queriesPointer;

    int newRRsNumber;

    int readQueryRequest(unsigned char*queryBufferPointer, dnsQuery *query);

    /*
     * @brief Pointer to data base reader class
     */
    DNSDataBaseReader *dnsDataBaseReader;

    /*
     * @brief Add response record to response packet
     */
    int addResponseRecord(unsigned char *rrBufferPlace, char *resolvedIP);
};

#endif /* DNSRESOLVER_H */
