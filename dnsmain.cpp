//#include <dnsdatabasereader.h>
//#include <dnsresolver.h>
//#include <dnspacketizator.h>
#include <dnssocket.h>

#include <iostream>

#define SERVER_PORT 53

int main (int argv, char argc[])
{

   //Procesar argumentos!!


  //Create the listening server socket
//  DNSDataBaseReader dnsDataBaseReader();

//  DNSResolver dnsResolver();
//  cout << "OK" << endl;

//  DNSPacketizator dnsPacketizator(dnsResolver);
//  cout << "OK" << endl;

  //Create the listening UDP socket
  DNSServerSocket dnsServerSocket(SERVER_PORT);
//  cout << "OK" << endl;

  //Wait for clients & process them  
  //SEGURAMENTE PASEMOS COMO PARÁMETRO EL HASH DE SERVIDORES!
  dnsServerSocket.listenSocket();

}
