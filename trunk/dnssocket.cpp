#include <dnssocket.h>

DNSServerSocket::DNSServerSocket()
{
  dnsServerSocket = -1;

  //Cleaning data buffer
  memset(receivedData, 0, DNS_PKT_SIZE);
}

DNSServerSocket::DNSServerSocket(int dnsPort, DNSPacketizator *dnsPacketizatorObject) throw (SocketException)
{
  dnsServerSocket = -1;

  memset(receivedData, 0, DNS_PKT_SIZE);
  try
  {
    createSocket(dnsPort);
  }
  catch(SocketException)
  {
    throw SocketException();
  }
  dnsPacketizator = dnsPacketizatorObject;
  dnsPacketizator->initializeBufferPointer(receivedData);
}

DNSServerSocket::~DNSServerSocket()
{
  if (dnsServerSocket > 0)
  {
    close(dnsServerSocket);
  }

}

void DNSServerSocket::createSocket(int serverPort) throw (SocketException)
{
  //Sockaddr struct for the socket
  struct sockaddr_in dnsSocketAddr;

  if ((dnsServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    throw SocketException();
  }

  //Fill fields of the inet address struct
  memset(&dnsSocketAddr, 0, sizeof(struct sockaddr_in));
  dnsSocketAddr.sin_family = AF_INET;
  dnsSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  dnsSocketAddr.sin_port = htons(serverPort);

  if (bind(dnsServerSocket,(struct sockaddr *)&dnsSocketAddr, sizeof (sockaddr_in)) == -1)
  {
    throw SocketException();
  }

}

void DNSServerSocket::listenSocket()
{
  int receivedBytes = 0;

  //Sockaddr struct for the dns client
  struct sockaddr_in dnsClientInetAddr;

  //Creation of the class for searching the data base
  int clientInetAddrSize = sizeof(sockaddr_in);

  //Infinite loop to receive incoming packets
  while (true)
  {
    if ( (receivedBytes = recvfrom(dnsServerSocket, receivedData, sizeof(receivedData), MSG_WAITALL, \
          (struct sockaddr *)&dnsClientInetAddr, (socklen_t *)&clientInetAddrSize) ) == -1)
    {
      printf("Problem with UDP reception, aborting petition\n");      
    }
    else
    {
      //If the incoming packet is a request then forget it!
      if((receivedData[2] & 0x80) == 0)
      {
        dnsPacketizator->processDnsQuery(receivedBytes);

        if ( sendto(dnsServerSocket, dnsPacketizator->getDnsResponse(), dnsPacketizator->getDnsResponseLength(), 0, \
            (struct sockaddr *)&dnsClientInetAddr, (socklen_t)clientInetAddrSize) == -1)
        { 
          printf("Error sending DNS response\n");
        }
      }
    }
  } //while
}
