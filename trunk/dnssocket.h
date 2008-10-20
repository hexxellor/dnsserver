#ifndef DNS_SOCKET_H
#define DNS_SOCKET_H

extern "C"
{
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <unistd.h>
//  #include <string.h>
}

#include <dnspacketizator.h>

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
     * @brief Parameter constructor.
     * @param dnsPort Port where the server will listen to incoming petitions.
     * @param dnsPacketizatorObject Pointer to dnsPacketizator object in memory.
     * @throw SocketException If it was some error creating the socket.
     */
    DNSServerSocket(int dnsPort, DNSPacketizator *dnsPacketizatorObject) throw (SocketException);

    /*
     * @brief Default destructor.
     */
    ~DNSServerSocket();

    /*
     * @brief Infinite loop: Listen for incoming clients.
     */
    void listenSocket();

  private:

    /*
     * @brief File descriptor for the DNS server socket.
     */
    int dnsServerSocket;

    /*
     * @brief Pointer to buffer for UDP incoming packet. 
     *        512 bytes is maximum for a UDP packet without truncation.
     */
     unsigned char receivedData[DNS_PKT_SIZE];

    /*
     * @brief Pointer to packetizator class.
     */
    DNSPacketizator *dnsPacketizator;

    /*
     * @brief Create the server socket and initialize the corresponding structs.
     * @param serverPort The port where the server wait for the client.
     * @throw SocketException If it was some error creating the socket.
     */
    void createSocket(int serverPort) throw (SocketException);

};

#endif /* DNS_SOCKET_H */
