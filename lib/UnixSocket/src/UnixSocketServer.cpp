#include "UnixSocket.hpp"

using namespace UnixSocket;

Result Server::setConfig( Config&& cfg )
{
    m_config = cfg;
    ERR_CHECK( m_config.m_address,      "file name" );
    ERR_CHECK( m_config.m_authKey,      "authentication");
    ERR_CHECK( m_config.m_delimiter,    "delimiter");

    if( ! m_config.m_recv_cb )
    {
        PRINT_ERR( "No read callback provided.\n" );
        return Result::CFG_ERROR;
    }
    if( ! m_config.m_send_cb )
    {
        PRINT_ERR( "No send callback provided.\n" );
        return Result::CFG_ERROR;
    }
    unlink( m_config.m_address.c_str() ); //prepare address upfront
    m_isConfigured.store( true );
    PRINTF( GRN, "Configuration is accepted.\n" );
    return Result::ALL_GOOD;
}

Result Server::start( void )
{
    if( ! m_isConfigured.load() )
    {
        PRINT_ERR( "Server has no configuration.\n" );
        return Result::CFG_ERROR;
    }
    ::std::cout << "Starting Unix server : " << m_config.m_address <<::std::endl;
    // PRINTF( RED, "Starting Unix server '%s'", m_config.m_address.c_str() );
    m_acceptor_uptr = ::std::make_unique< Acceptor >( m_ioService , EndPoint{ m_config.m_address } );
    accept(); /* Recursive async call inside */
    m_worker = ::std::move(
        ::std::thread( [&](){ m_ioService.run(); } )
    );
    return Result::ALL_GOOD;
}

void Server::accept ( void )
{
    SessionShptr session_shptr = ::std::make_shared< Session >( m_ioService, this);
    m_acceptor_uptr->async_accept( session_shptr->getSocket(),
    [&, session_shptr ] ( const ErrCode& error ) mutable
    {
        if (!error)
        {
            PRINTF( GRN, "Client accepted.\n" );
            m_sessions.push_back( session_shptr );
            session_shptr->recv( session_shptr );
            this->accept();
        }
        else
        {
            PRINT_ERR( "Error when accepting : %s\n", error.message().c_str());
        }
    } );
}

/* EOF */