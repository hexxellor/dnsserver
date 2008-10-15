#include <dnsdatabasereader.h>

DNSDataBaseReader::DNSDataBaseReader()
{
  //Apertura del fichero. 
  //COMPROBAR SI EXISTE PRIMERO!!
  ifsHosts.open(HOSTS_FILE);
}

DNSDataBaseReader::~DNSDataBaseReader()
{
  ifsHosts.close();
}

string DNSDataBaseReader::getFoundIP()
{

}

//Search the name in local database. Returns:
//1 - 


int DNSDataBaseReader::searchIPbyURL(string &askedURL)
{
  
  size_t beginComment, beginHostIP, endHostIP, beginHostName, endHostName, beginHostAlias, endHostAlias;
  string HostIP, HostName, HostAlias;

  if (ifsHosts)
  {
    //Positioning in the begining of the buffer
    ifsHosts.seek(0, std::beg);

    while (getline(ifsHosts, line))
    {
      if ((beginComment = line.find('#')) != string::npos)
      {
        //Recortamos todo lo que hay por detras de #
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
           //The found name is an URL => return 1
           if (HostName.compare(askedURL) == 0)
           {
             resolvedIP = HostIP;
             return (1);
           }
         }

         if ((beginHostAlias = line.find_first_not_of(FORBIDDEN_CHARS, endHostName)) != string::npos)
         {
           endHostAlias = line.find_first_of(FORBIDDEN_CHARS, beginHostAlias);
           HostAlias = line.substr(beginHostAlias, endHostAlias - beginHostAlias);         
           //The found name is an alias => return 2
           if (HostName.compare(askedURL) == 0)
           {
             resolvedIP = HostIP;
             return (2);
           }

         }

      }
    }
  //Not found UURL => return -1
  printf("Not found!\n");
  return (-1);
  }

}

