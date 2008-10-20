#include <iostream>

#include <dnsdatabasereader.h>
#include <dnsresolver.h>
#include <dnspacketizator.h>
#include <dnssocket.h>
#include <dnsexception.h>

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

  //Pointers for the objects
  DNSDataBaseReader *dnsDBR;
  DNSResolver *dnsResolver;
  DNSPacketizator *dnsPacketizator;
  DNSServerSocket *dnsServerSocket;

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

  //Creating the objects
  cout << "Creating data base reader object...   " ;
  try 
  {
    dnsDBR = new DNSDataBaseReader (hostFileName);
  }
  catch (FileNotFoundException)
  {
    cout << "Fatal error: couldn't open file " << hostFileName << endl;
    cout << "Exit!" << endl; 
    exit(-1);
  }
  cout << "OK!" << endl;

  cout << "Creating resolver object...   " ;
  dnsResolver = new DNSResolver(dnsDBR);
  cout << "OK!" << endl;

  cout << "Creating packetizator object...   " ;
  dnsPacketizator = new DNSPacketizator(dnsResolver);
  cout << "OK!" << endl;

  cout << "Creating listening socket...   " ;
  //Create the listening UDP socket
  try
  {
    dnsServerSocket = new DNSServerSocket(dnsServerPort, dnsPacketizator);
  }
  catch (SocketException)
  {   
    cout << "Fatal error: couldn't listen on port " << dnsServerPort << endl;
    cout << "Do you have rights for doing it?" << endl; 
    cout << "Exit!" << endl; 
    exit(-1);
  }
  cout << "OK!" << endl;

   cout << "Listening for incoming petitions" << endl,
  //Wait for clients & process them  
  dnsServerSocket->listenSocket();

}
