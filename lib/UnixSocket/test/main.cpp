#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "UnixSocket.h"

void serverReadCallBack( ::std::string clientName, ::std::string data )
{
    ::std::cout << __func__ << " : "
                << "Server received data : " 
                << data << '\n'
                << "From client : " << clientName << ::std::endl;
}

void serverSendCallBack( ::std::size_t sent_bytes )
{
    ::std::cout << __func__ << " : "
                << "Server sent " 
                << sent_bytes << "bytes." 
                << ::std::endl;
}

/*                       Unification : */
void clientReadCallBack( ::std::string , ::std::string data )
{
    ::std::cout << __func__ << " : "
                << "Client received data : " 
                << data << ::std::endl;
}

void clientSendCallBack( ::std::size_t sent_bytes )
{
    ::std::cout << __func__ << " : " 
                << "Client sent " 
                << sent_bytes << "bytes." 
                << ::std::endl;
}

/* "Sometimes old methods are better." Mr.World. American Gods. */
#define FILE_TO_USE         "/tmp/UnixSocketServer"
#define BODY_DELIMITER      "body"
#define AUTH_DELIMITER      "auth"
#define SYNC_CLIENT_NAME    "syncClient"
#define ASYNC_CLIENT_NAME   "asyncClient"
#define TRANSACTION_DELAY   ::std::chrono::milliseconds(1000)


int main( int , char** )
{
    /* Create and start server */
    ::UnixSocket::Server server;
    ::UnixSocket::Server::Config config = 
    {
        .m_recvCallBack = serverReadCallBack,
        .m_sendCallBack = serverSendCallBack,
        .m_address      = FILE_TO_USE,
        .m_delimiter    = BODY_DELIMITER,
        .m_authKey      = AUTH_DELIMITER
    };
    server.setConfig( ::std::move( config ) ); /* No way to change config. */
    server.start();

    /* Create and start ASYNC client */
    ::UnixSocket::Client asyncClient;
    ::UnixSocket::Client::Config asyncConfig =
    {
        .m_recvCallBack = clientReadCallBack,
        .m_sendCallBack = clientSendCallBack,
        .m_address      = FILE_TO_USE,
        .m_delimiter    = BODY_DELIMITER,
        .m_authKey      = AUTH_DELIMITER,
        .m_clientName   = ASYNC_CLIENT_NAME,
        /* Client without connection seems useless */
        .m_conType = ::UnixSocket::Client::ConnectType::ASYNC_CONNECT
    };
    asyncClient.setConfig( ::std::move( asyncConfig ) ); /* No way to change config. */
    asyncClient.start();

    /* Create and start SYNC client */
    ::UnixSocket::Client syncClient;
    ::UnixSocket::Client::Config syncConfig =
    {
        .m_recvCallBack = clientReadCallBack,
        .m_sendCallBack = clientSendCallBack,
        .m_address      = FILE_TO_USE,
        .m_delimiter    = BODY_DELIMITER,
        .m_authKey      = AUTH_DELIMITER,
        .m_clientName   = SYNC_CLIENT_NAME,
        /* Client without connection seems useless */
        .m_conType = ::UnixSocket::Client::ConnectType::SYNC_CONNECT
    };
    syncClient.setConfig( ::std::move( syncConfig ) ); /* No way to change config. */
    syncClient.start();

    while( true )
    {
        ::std::this_thread::sleep_for( TRANSACTION_DELAY );
        asyncClient.send( ::std::string{ "<body>Data from " ASYNC_CLIENT_NAME "</body>" } );
        ::std::cout << "--" << ::std::endl;

        ::std::this_thread::sleep_for( TRANSACTION_DELAY );
        syncClient.send( ::std::string{ "<body>Data from " SYNC_CLIENT_NAME "</body>" } );
        ::std::cout << "--" << ::std::endl;

        ::std::this_thread::sleep_for( TRANSACTION_DELAY );
        server.send< ::std::string >( 
                    ::std::string{ ASYNC_CLIENT_NAME },
                    ::std::string{ "<body>Data for " ASYNC_CLIENT_NAME "</body>" } );
        ::std::cout << "--" << ::std::endl;

        ::std::this_thread::sleep_for( TRANSACTION_DELAY );
        server.send< ::std::string >( 
                    ::std::string{ SYNC_CLIENT_NAME },
                    ::std::string{ "<body>Data for " SYNC_CLIENT_NAME "</body>" } );
        ::std::cout << "--" << ::std::endl;
    }
    return 0;
}