#include <dnsdatabasereader.h>

DNSDataBaseReader::DNSDataBaseReader(char *hostFile)
{
  //File opening
  ifsHosts.open(hostFile);
  strcpy(resolvedIP, "No IP");
  strcpy(realName, "No name");
}

DNSDataBaseReader::~DNSDataBaseReader()
{
  ifsHosts.close();
}

char *DNSDataBaseReader::getFoundIP()
{
  return resolvedIP;
}

char *DNSDataBaseReader::getRealName()
{
  return realName;
}

//Search the name in local database. Returns:
// NAME_IS_AN_URL if the found URL is an URL 
// NAME_IS_AN_ALIAS if the found URL is an alias
// NAME_NOT_FOUND if URL is not found
int DNSDataBaseReader::searchIPbyURL(char *askedURL)
{

  size_t beginComment, beginHostIP, endHostIP, beginHostName, endHostName, beginHostAlias, endHostAlias;
  string line, HostIP, HostName, HostAlias;
   
  //Positioning in the begining of the buffer
  if (ifsHosts.eof() == true)
  {
    ifsHosts.clear();
  }

  ifsHosts.seekg(0, ios_base::beg);

  while (getline(ifsHosts, line))
  {
    if ((beginComment = line.find('#')) != string::npos)
    {
      //Erase everything after '#' in the line
      line.erase(beginComment, line.length() - beginComment);
    }

    if (((beginHostIP = line.find_first_not_of(FORBIDDEN_CHARS)) != string::npos) && (line.length() > 0))
    {
       endHostIP = line.find_first_of(FORBIDDEN_CHARS, beginHostIP);
       HostIP = line.substr(beginHostIP, endHostIP - beginHostIP);

       if ((beginHostName = line.find_first_of(ALPHABETIC_CHARS, endHostIP)) != string::npos)
       {
         endHostName = line.find_first_of(FORBIDDEN_CHARS, beginHostName);
         HostName = line.substr(beginHostName, endHostName - beginHostName);

         //The found name is an URL           
         if (HostName.compare(askedURL) == 0)
         {
           strcpy(resolvedIP, HostIP.c_str());       
           return NAME_IS_AN_URL;
         }
       }

       if ((beginHostAlias = line.find_first_not_of(FORBIDDEN_CHARS, endHostName)) != string::npos)
       {
         endHostAlias = line.find_first_of(FORBIDDEN_CHARS, beginHostAlias);
         HostAlias = line.substr(beginHostAlias, endHostAlias - beginHostAlias);         
         //The found name is an alias
         if (HostAlias.compare(askedURL) == 0)
         {
           strcpy(resolvedIP, HostIP.c_str());
           strcpy(realName, HostName.c_str());
           return NAME_IS_AN_ALIAS;
         }

       }
    }
  }

  //Not found URL
  return NAME_NOT_FOUND;  
}

