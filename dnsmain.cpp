#include <dnsdatabasereader.h>
#include <dnsresolver.h>
#include <dnspacketizator.h>
#include <dnssocket.h>

#include <iostream>

extern "C"
{
  #include <getopt.h>
}

#define SERVER_PORT 53
#define HOSTS_FILE "./hosts"


char optString[] = "p:f:h";

static struct option longOptions[] =
{
  {"port",       required_argument,  0,  'p' },
  {"hostfile",   required_argument,  0,  'f' },
  {"help",       no_argument,        0,  'h' },
  {0,            0,                  0,  0   }
};

char hostFileName[100];

void printOptions()
{
  cout << "Posible options:" << endl;
  cout << "   -p, --port=PORT                 Listening port." << endl;
  cout << "   -f, --hostfile=HOST_FILE_NAME   File to read the pairs URL - IP" << endl;
  cout << "   -h, --help:                     Print this." << endl;
}

int main (int argc, char *argv[])
{

  int dnsServerPort = SERVER_PORT;
  strcpy(hostFileName, HOSTS_FILE);

  int option = -1;
  int optionIndex = 0;

  //Process the command options...
  while ((option = getopt_long (argc, argv, optString, longOptions, &optionIndex)) != -1)
  {
    switch (option)
    {
      case 'f':
        if (strlen(optarg) > 100)
        {
          printf("The file name %s is too long, takin the default value \'%s\'\n", optarg, hostFileName);
        }
        else
        {
          strcpy(hostFileName, optarg);
        }
        break;

      case 'p':
        //In god we trust!
        dnsServerPort = atoi(optarg);
        break;

     case 'h':
        printOptions();
        exit(-1);
    }
  }

  DNSDataBaseReader *dnsDBR = new DNSDataBaseReader(hostFileName);
  cout << "OK" << endl;

  DNSResolver *dnsResolver = new DNSResolver(dnsDBR);
  cout << "OK" << endl;

  DNSPacketizator *dnsPacketizator = new DNSPacketizator(dnsResolver);
  cout << "OK" << endl;

  //Create the listening UDP socket
  DNSServerSocket dnsServerSocket(dnsServerPort, dnsPacketizator);
  cout << "OK" << endl;

  //Wait for clients & process them  
  dnsServerSocket.listenSocket();

}
