#include "UnixSocket.h"
#include <iostream>

int main (int, char**)
{
    ::std::cout << "Starting UnixSocket test.\n";
    ::boost::asio::io_service io_service;
    ::UnixSocket::Server server(io_service);
    ::UnixSocket::Client client(io_service);
    io_service.run();
    io_service.stop();
    ::std::cout << "UnixSocket test finished.\n";
    return 0;
}