#include <iostream>
#include <string>
#include "dnsdatabasereader.h"

int main(int argc, char argv[])
{
  string urlBuscada = string("www.hotmail.com");

  DNSDataBaseReader dbr;
  string *dirIP = dbr.searchIPbyURL(&urlBuscada);

  cout << "La IP correspondiente a " << urlBuscada << " es " << *dirIP << endl;
}
