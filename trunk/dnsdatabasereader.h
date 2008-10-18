#ifndef DNS_DBR_H
#define DNS_DBR_H

#include <fstream>
#include <iostream>
#include <string>

#include <defs.h>

using namespace std;

#define HOSTS_FILE "./hosts"
#define FORBIDDEN_CHARS "\t "
#define NUMERIC_CHARS "1234567890"
#define ALPHABETIC_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*
 * @class DNSDataBaseReader
 * @brief 
 */
class DNSDataBaseReader
{
  public:

    /*
     * @brief Default constructor.
     */
    DNSDataBaseReader();

    /*
     * @brief Default destructor.     
     */
    ~DNSDataBaseReader();

    /*
     * @brief Function to search an URL in /etc/hosts
     * @param askedURL The URL we are searching for
     * @return A value depending on if the found is an URL, an Alias or not found
     */
    int searchIPbyURL(char *askedURL);

    /*
     * @brief Returns the last resolved IP
     * @return Pointer to char buffer with the IP
     */
    char *getFoundIP();

    /*
     * @brief Function to search in /etc/hosts
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
