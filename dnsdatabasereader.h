#ifndef DNS_DBR_H
#define DNS_DBR_H

#include <fstream>
#include <iostream>
#include <string>

#include <defs.h>

//using namespace std;

#define HOSTS_FILE "./hosts"
#define FORBIDDEN_CHARS "\t "
#define NUMERIC_CHARS "1234567890"
#define ALPHABETIC_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

class DNSDataBaseReader
{
  public:

    /*
     * @brief Default constructor.
     */
    DNSDataBaseReader();

    /*
     * @brief Default destructor.
     *
     */
    ~DNSDataBaseReader();

    /*
     * @brief Function to search in /etc/hosts
     */
    int searchIPbyURL(char *askedURL);

    /*
     * @brief Function to search in /etc/hosts
     */
    char *getFoundIP();

    /*
     * @brief Function to search in /etc/hosts
     */
    char *getRealName();

  private:

    /*
     * @brief Descriptor of the host file
     */
    ifstream ifsHosts;

    string line;

    char resolvedIP[16];

    char realName[255];

};

#endif /* DNS_DBR_H */