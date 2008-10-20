#include <dnsdatabasereader.h>

DNSDataBaseReader::DNSDataBaseReader() throw (FileNotFoundException)
{
  numberIPs = 0;
  strcpy(realName, "No name");
  resolvedIPs = NULL;

  //File opening: setting and returning exceptions
  ifsHosts.exceptions( ifstream::badbit | ifstream::failbit | ifstream::eofbit);
  try 
  {
    ifsHosts.open("hosts");
  }
  catch (ifstream::failure FileNotFound)
  {
    throw FileNotFoundException();
  }

}


DNSDataBaseReader::DNSDataBaseReader(char *hostFile) throw (FileNotFoundException)
{

  numberIPs = 0;
  strcpy(realName, "No name");
  resolvedIPs = NULL;

  //File opening: setting and returning exceptions
  ifsHosts.exceptions( ifstream::badbit | ifstream::failbit | ifstream::eofbit);
  try 
  {
    ifsHosts.open(hostFile);
  }
  catch (ifstream::failure FileNotFound)
  {
    throw FileNotFoundException();
  }

}

DNSDataBaseReader::~DNSDataBaseReader()
{
  ifsHosts.close();

  //Delete ip array of last search
  deleteIParray();
}

char *DNSDataBaseReader::getFoundIP(int ipIndex)
{
  return resolvedIPs[ipIndex];
}

char *DNSDataBaseReader::getRealName()
{
  return realName;
}

int DNSDataBaseReader::getNumberofIPs()
{
  return numberIPs;
}

void DNSDataBaseReader::findNumberIPinLine(string &line)
{
  size_t commaMark = 0, postCommaMark = 1;
  numberIPs = 1;

  //Number of ips is number of commas plus one
  while ((commaMark = line.find(',', postCommaMark)) != string::npos)
  {
    numberIPs++;
    postCommaMark = commaMark + 1;
  }
}

void DNSDataBaseReader::deleteIParray()
{

  for (int i = 0; i < numberIPs; i++)
  {
    delete[] resolvedIPs[i];
  }

  delete[] resolvedIPs;
  resolvedIPs = NULL;

}

//Search the name in local database. Returns:
// NAME_IS_AN_URL if the found URL is an URL 
// NAME_IS_AN_ALIAS if the found URL is an alias
// NAME_NOT_FOUND if URL is not found
int DNSDataBaseReader::searchIPbyURL(char *askedURL)
{

  size_t beginMark, endMark;
  string line, HostIP, HostName, HostAlias;
  numberIPs = 0;
   
  //Positioning in the begining of the buffer
  if (ifsHosts.eof() == true)
  {
    ifsHosts.clear();
  }

  ifsHosts.seekg(0, ios_base::beg);

  while (ifsHosts.eof() == false)
  {
    try
    {
      getline(ifsHosts, line);
    }
    catch (ifstream::failure)
    {
      if (ifsHosts.eof() == true)
      {
        //Not found URL (reached EOF)
        return NAME_NOT_FOUND;  
      }
    }

    //Is the URL in this line?
    if (line.find(askedURL) != string::npos)
    {
    
      if ((beginMark = line.find('#')) != string::npos)
      {
        //Erase everything after '#' in the line
        line.erase(beginMark, line.length() - beginMark);
      }

      if (((beginMark = line.find_first_not_of(FORBIDDEN_CHARS, 0)) != string::npos) && (line.length() > 0))
      {
        //OK, so we have found something
        //First delete ip array of last search
        if (resolvedIPs != NULL)
        {
          deleteIParray();
        }

        //Then count the number of IPs on the line
        findNumberIPinLine(line);

        //Dynamic reservation for IPs...
        resolvedIPs = new char *[numberIPs];

        //Copy of the IPs
        int i = 0;
        while ((endMark = line.find(',', beginMark)) != string::npos)
        {
          HostIP = line.substr(beginMark, endMark - beginMark);
          resolvedIPs[i] = new char[16];
          strcpy(resolvedIPs[i], HostIP.c_str());     
          i++;
          beginMark = line.find_first_not_of(FORBIDDEN_CHARS, endMark);
        }

        //Copy last of IPs
        endMark = line.find_first_of(FORBIDDEN_CHARS, beginMark);
        HostIP = line.substr(beginMark, endMark - beginMark);
        resolvedIPs[i] = new char[16];
        strcpy(resolvedIPs[i], HostIP.c_str());   

        //////////////////////////////////////////////////////////////////////////////////////
        ////////////    Copy of the URL & alias part of line
        if ((beginMark = line.find_first_of(ALPHABETIC_CHARS, endMark)) != string::npos)
        {
          endMark = line.find_first_of(FORBIDDEN_CHARS, beginMark);
          HostName = line.substr(beginMark, endMark - beginMark);
          //The found name is an URL           
          if (HostName.compare(askedURL) == 0)
          {  
            return NAME_IS_AN_URL;
          }
        }
 
        if ((beginMark = line.find_first_of(ALPHABETIC_CHARS, endMark)) != string::npos)
        {
          endMark = line.find_first_of(FORBIDDEN_CHARS, beginMark);
          HostAlias = line.substr(beginMark, endMark - beginMark);      
          //The found name is an alias
          if (HostAlias.compare(askedURL) == 0)
          {
            strcpy(realName, HostName.c_str());
            return NAME_IS_AN_ALIAS;
          }  
        }
        //////////////////////////////////////////////////////////////////////////////////////
      }
    }

  } //while


}

