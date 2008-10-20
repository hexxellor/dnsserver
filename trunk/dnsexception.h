#ifndef DNS_EXCEPTIONS_H
#define DNS_EXCEPTIONS_H

//#include <exception>
#include <stdexcept>

using std::exception;

/**
 *  @class  FileNotFoundException
 *  @brief  Exception launched when some asked file is not found.
 */
class FileNotFoundException : public exception
{ 
  public:
    FileNotFoundException() : exception()
    { 
    }
};

/**
 *  @class  SocketException
 *  @brief  Exception launched when there is some problem creating or binding the socket.
 */
class SocketException : public exception
{ 
  public:
    SocketException() : exception()
    {
    }
};

#endif /* DNS_EXCEPTIONS_H */
