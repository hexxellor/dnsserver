#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

#include <errno.h>

#include "defs.h"

uint16_t nQueries;
unsigned char pktDns[512]; 

void writeHeader(struct dnsHeader *header)
{

  header->idDnsQuery = htons((uint16_t)0xa987);
  header->flagsHeader = htons((uint16_t)0x0100);
  header->qdCount = htons((uint16_t)nQueries);
  header->anCount = htons((uint16_t)0x0000);
  header->nsCount = htons((uint16_t)0x0000);
  header->arCount = htons((uint16_t)0x0000);

}

unsigned int addQuery(char *URL, char *creaPkt)
{  

  char *punto = URL, *prevStep = creaPkt;
  uint8_t numChar = 0, numBytes = 1;

  uint16_t rr_Type, rr_Class; 

  int i;
  creaPkt++;
  for (i = 0; i < strlen(URL); i++, punto++) 
  {
    if (*punto == '.')
       { 
         *prevStep = numChar;
         numChar = 0;
         prevStep = creaPkt;
         creaPkt = creaPkt + 1;
       }
    else
    {
      *creaPkt = *punto;
      creaPkt++;
      numChar++;
    }
    numBytes++;
  }

  //Escritura ultimos datos para acabar el paquete
 *prevStep = numChar;
  creaPkt[0] = 0x00;
  numBytes++;

  rr_Type = htons((uint16_t)0x0001);
  memcpy(&creaPkt[1],&rr_Type, 2);

  rr_Class = htons((uint16_t)0x0001);
  memcpy(&creaPkt[3],&rr_Class, 2);

  numBytes = numBytes + 4;

  nQueries++;

  return(numBytes);
}

void sendPkt(int longDatos)
{
  int dnsServerSocket;
  struct sockaddr_in dnsSocketAddr;

  struct hostent *dnsRemoteServer;

  if ((dnsServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    printf("Error creando socket, clase DNSServerSocket\n");
    //throw SomeException;
  }

  //Fill fields of the inet address struct
  memset(&dnsSocketAddr, 0, sizeof(struct sockaddr_in));

  dnsRemoteServer = gethostbyname("199.7.83.42");

  dnsSocketAddr.sin_family = AF_INET;
  dnsSocketAddr.sin_addr.s_addr = ((struct in_addr *)(dnsRemoteServer->h_addr))->s_addr;
  dnsSocketAddr.sin_port = htons(53);

  if (bind(dnsServerSocket,(struct sockaddr *)&dnsSocketAddr, sizeof (dnsSocketAddr)) == -1)
  {    
    printf("Error haciendo el bind: %s\n", strerror(errno));
  }

  if (sendto(dnsServerSocket, pktDns, longDatos, 0, (struct sockaddr *)&dnsSocketAddr, sizeof(struct sockaddr_in)) == -1)
  {
     printf("No se pudo enviar nada!\n");
  }
}

int main (int argc, char argv[])
{
  unsigned char *creaPkt;
  unsigned int nBytes = 0;
  unsigned int bytesQuery = 0;

  nQueries = 0;
  creaPkt = pktDns + 12;
  
  bytesQuery = addQuery("www.yahoo.com", creaPkt);
  nBytes = nBytes + bytesQuery;

//  creaPkt = creaPkt + bytesQuery; 
//  bytesQuery = addQuery("www.yahoo.com", creaPkt);
//  nBytes = nBytes + bytesQuery;

  writeHeader((struct dnsHeader *)pktDns);

  //Cabecera...
  nBytes = nBytes + 12;

  printf("Enviando %i\n", nBytes);

  sendPkt(nBytes);

  printf("Ue!\n");

}
