#include "TcpIp.h"

using namespace TcpIp;

Result TcpIp::Server::setConfig( Config&& cfg ) /* Save config */
{
    boost::system::error_code error;
    m_config = ::std::move( cfg ); //by-member move

    if( m_config.m_ipAddress == "" )
    {
        PRINT_ERR( "No IP address provided.\n" );
        return Result::CFG_ERROR;
    }
    if( ! m_config.m_recvCallBack ) //not contains function
    {
        PRINT_ERR( "No read callback provided.\n" );
        return Result::CFG_ERROR;
    }
    if( ! m_config.m_sendCallBack )
    {
        PRINT_ERR( "No send callback provided.\n" );
        return Result::CFG_ERROR;
    }
    m_address = 
        boost::asio::ip::make_address( m_config.m_ipAddress, error );
    if( error )
    {
        PRINT_ERR( "%s", error.message().c_str() );
        return Result::WRONG_IP_ADDRESS;
    }

    m_isConfigured.store(true);
    PRINTF( GRN, "Configuration is accepted.\n" );
    return Result::ALL_GOOD;
}

Result TcpIp::Server::start( void )
{
    if( ! m_isConfigured.load() )
    {
        PRINT_ERR( "Server has no configuration.\n" );
        return Result::CFG_ERROR;
    }
    m_endPoint_uptr = ::std::make_unique<EndPoint>( m_address, m_config.m_portNum );
    m_acceptor_uptr = ::std::make_unique<Acceptor>( m_ioService, * m_endPoint_uptr );

    accept();
    /* TO DO : make several threads for one 'io_service' */
    m_worker = ::std::move( ::std::thread(
                [&]()
                { 
                    m_ioService.run(); 
                } /* lambda */ )/* thread */ )/* move */;
    return Result::ALL_GOOD;
}

void TcpIp::Server::accept( void )
{
    SessionShptr session_shptr = ::std::make_shared< Session >( m_ioService, this );
    m_acceptor_uptr->async_accept(session_shptr->getSocket(),
        [&, session_shptr]( ErrCode error ) //mutable
        {
            if( !error )
            {
                PRINTF( GRN, "Client accepted.\n" );
                m_sessions.push_back( session_shptr ); //vector underneath
                session_shptr->recv( session_shptr );
                this->accept();
            }
            else
            {
                PRINT_ERR( "Error when accepting : %s\n", error.message().c_str());
            }
        } /* lambda */ )/* async_accept */;
}

Server::~Server()
{
    /* Stop accepting */
    m_acceptor_uptr->cancel();
    m_acceptor_uptr->close();
    closeAllSessions();
    m_ioService.stop();
    m_worker.join();
}

void Server::closeAllSessions( void )
{
    for( const auto & it : m_sessions )
    {
        /* All Sessions will exit recv with error and will be destroyed */
        it->getSocket().shutdown( Socket::shutdown_send );
        it->getSocket().shutdown( Socket::shutdown_receive );
        it->getSocket().close();
    }
}
/* EOF */