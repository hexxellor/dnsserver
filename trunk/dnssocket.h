#ifndef DNS_SOCKET_H
#define DNS_SOCKET_H

extern "C"
{
  #include <sys/types.h>
  #include <sys/socket.h>

  #include <unistd.h>

  #include <string.h>
}

//#include <dnsdatabasereader.h>
#include <dnspacketizator.h>

// 5 es un buen numero... Comprobar!
#define NUM_MAX_CLIENTES 5

/**
 *  @class  DNSServerSocket
 *  @brief  Creates a listening socket and serves the requests.
 */
class DNSServerSocket
{

  public:

    /*
     * @brief Default constructor.
     */
    DNSServerSocket();

    /*
     * @brief Param constructor.
     * @param Port where the server will listen to incoming petitions.
     */
    DNSServerSocket(int dnsPort);

    /*
     * @brief Default destructor.
     */
    ~DNSServerSocket();

    /*
     * @brief Listen for incoming client. Every client must run in his own thread.
     */
    void listenSocket();

  private:

   /*
     * @brief Pointer to buffer for UDP incoming packet. 512 bytes is maximum for a UDP packet without truncation.
     */
     unsigned char receivedData[DNS_PKT_SIZE];

    /*
     * @brief File descriptor for the DNS server socket
     */
    int dnsServerSocket;

    int bytesRecibidos;

    /*
     * @brief Pointer to packetizator class
     */
    DNSPacketizator *dnsPacketizator;

    struct sockaddr_in dnsSocketAddr;

    /*
     * @brief Sockaddr struct for the dns client
     */
    struct sockaddr_in dnsClientInetAddr;

    int dnsClient;

    /*
     * @brief Create the server socket and initialize the corresponding structs
     * @param serverPort The port where the server wait for the client
     */
    void createSocket(int serverPort);

};

#endif /* DNS_SOCKET_H */
