#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "UnixSocket.h"

void serverReadCallBack( ::std::string data )
{
    ::std::cout << __func__ << " : "
                << "Server recieved data : " 
                << data << ::std::endl;
}

void serverSendCallBack( ::std::size_t sent_bytes )
{
    ::std::cout << __func__ << " : "
                << "Server sent " 
                << sent_bytes << "bytes." 
                << ::std::endl;
}

void clientReadCallBack( ::std::string data )
{
    ::std::cout << __func__ << " : "
                << "Client recieved data : " 
                << data << ::std::endl;
}

void clientSendCallBack( ::std::size_t sent_bytes )
{
    ::std::cout << __func__ << " : " 
                << "Client sent " 
                << sent_bytes << "bytes." 
                << ::std::endl;
}

int main( int , char** )
{
    /* Create and start server */
    ::UnixSocket::Server server;
    ::UnixSocket::Server::Config config = 
    {
        .m_recv_cb = serverReadCallBack,
        .m_send_cb = serverSendCallBack,
        .m_address = "/tmp/UnixSocketServer",
        .m_delimiter = "body",
        .m_authKey = "auth"
    };
    server.setConfig( ::std::move( config ) ); /* No way to change config. */
    server.start();

    /* Create and start ASYNC client */
    ::UnixSocket::Client asyncClient;
    ::UnixSocket::Client::Config asyncConfig =
    {
        .m_recv_cb = clientReadCallBack,
        .m_send_cb = clientSendCallBack,
        .m_address = "/tmp/UnixSocketServer",
        .m_delimiter = "body",
        .m_authKey = "auth",
        .m_clientName = "asyncClient",
        /* Client without connection seems useless */
        .m_conType = ::UnixSocket::Client::ConnectType::ASYNC_CONNECT
    };
    asyncClient.setConfig( ::std::move( asyncConfig ) ); /* No way to change config. */
    asyncClient.start();

    /* Create and start SYNC client */
    ::UnixSocket::Client syncClient;
    ::UnixSocket::Client::Config syncConfig =
    {
        .m_recv_cb = clientReadCallBack,
        .m_send_cb = clientSendCallBack,
        .m_address = "/tmp/UnixSocketServer",
        .m_delimiter = "body",
        .m_authKey = "auth",
        .m_clientName = "syncClient",
        /* Client without connection seems useless */
        .m_conType = ::UnixSocket::Client::ConnectType::SYNC_CONNECT
    };
    syncClient.setConfig( ::std::move( syncConfig ) ); /* No way to change config. */
    syncClient.start();

    while( true )
    {
        ::std::this_thread::sleep_for( ::std::chrono::milliseconds(1000) );
        asyncClient.send( ::std::string{ "<body>Data from asyncClient</body>" } );
        ::std::cout << "--" << ::std::endl;

        ::std::this_thread::sleep_for( ::std::chrono::milliseconds(1000) );
        syncClient.send( ::std::string{ "<body>Data from syncClient</body>" } );
        ::std::cout << "--" << ::std::endl;

        ::std::this_thread::sleep_for( ::std::chrono::milliseconds(1000) );
        server.send< ::std::string >( 
                    ::std::string{ "asyncClient" },
                    ::std::string{ "<body>Data for syncClient</body>" } );
        ::std::cout << "--" << ::std::endl;

        ::std::this_thread::sleep_for( ::std::chrono::milliseconds(1000) );
        server.send< ::std::string >( 
                    ::std::string{ "syncClient" },  
                    ::std::string{ "<body>Data for asyncClient</body>" } );
        ::std::cout << "--" << ::std::endl;

    }
    return 0;
}