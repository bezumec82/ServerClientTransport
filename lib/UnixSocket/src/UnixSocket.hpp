#ifndef UNIX_SOCKET_HPP
#define UNIX_SOCKET_HPP

#include <boost/asio.hpp>

#include "Tools.h"

namespace UnixSocket
{
    using IoService = ::boost::asio::io_service;
class Server
{
public :
    Server()=delete;
    explicit Server( IoService& io_service )
    { 
        PRINTF(GRN, "Server started.\n");
    }
};

class Client
{
public :
    Client()=delete;
    explicit Client( IoService& io_service )
    { 
        PRINTF(GRN, "Client started.\n");
    }
};

}; //end namespace UnixSocket

#endif /* UNIX_SOCKET_HPP */