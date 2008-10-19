#ifndef DNS_DBR_H
#define DNS_DBR_H

#include <fstream>
#include <iostream>
#include <string>

#include <defs.h>

using namespace std;

#define FORBIDDEN_CHARS "\t ,"
#define NUMERIC_CHARS "1234567890"
#define ALPHABETIC_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*
 * @class DNSDataBaseReader
 * @brief Read the name - IP correspondeces from a 'hosts like' local file
 */
class DNSDataBaseReader
{
  public:

    /*
     * @brief Default constructor.
     */
    DNSDataBaseReader();

    /*
     * @brief Param constructor.
     * @param hostFile Name of the 'hosts like' text file with the DNS local data.
     */
    DNSDataBaseReader(char *hostFile);

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
     * @param ipIndex The URL we are searching for
     * @return Pointer to char buffer with the IP
     */
    char *getFoundIP(int ipIndex);

    /*
     * @brief Returns the last resolved IP
     * @return Number of IPs asociated with the target URL
     */
    int getNumberofIPs();

    /*
     * @brief Returns the URL of last resolution
     * @return Pointer to char array with the URL
     */
    char *getRealName();

  private:

    /*
     * @brief Descriptor of the host file
     */
    ifstream ifsHosts;

    /*
     * @brief IP of last resolution done
     */
    char **resolvedIPs;

    /*
     * @brief URL of last resolution done
     */
    char realName[255];

    /*
     * @brief Number of IPs asociated with the target URL
     */
    int numberIPs;

    /*
     * @brief Calculate number of IPs in a line
     * @param line The line to study
     */
    void findNumberIPinLine(string &line);

    /*
     * @brief Number of IPs asociated with the target URL
     */
    void deleteIParray();

};

#endif /* DNS_DBR_H */
