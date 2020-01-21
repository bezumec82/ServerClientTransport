#include <iostream>
#include <csignal>

#include "TcpIp.h"

bool keepRunning = true;

using byte = uint8_t;
void serverReadCallBack( ::std::string clientAddr, ::std::vector< byte > data )
{
    ::std::string strData( reinterpret_cast< char* >( data.data() ), data.size() ); //bad but tolerable
    ::std::cout << __func__ << " : "
                << "Server recieved data : " 
                << strData << '\n'
                << "From client : " << clientAddr << ::std::endl;
}

void serverSendCallBack( ::std::size_t sent_bytes )
{
    ::std::cout << __func__ << " : "
                << "Server sent " 
                << sent_bytes << "bytes." 
                << ::std::endl;
}

void sigIngHandler ( int signal )
{
    keepRunning = false;
}

int main( int, char** )
{

    ::TcpIp::Server server;
    ::TcpIp::Server::Config config = 
    {
        .m_recvCallBack = serverReadCallBack,
        .m_sendCallBack = serverSendCallBack,
        .m_ipAddress    = "127.0.0.1",
        .m_portNum      = 80,
    };
    server.setConfig( ::std::move( config ) );
    server.start();

    std::signal(SIGINT, sigIngHandler);

    while (keepRunning)
    {
        ::std::this_thread::sleep_for( ::std::chrono::milliseconds(1000) );
    }
}