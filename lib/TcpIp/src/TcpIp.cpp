#include "TcpIp.hpp"


::TcpIp::Server::Result \
TcpIp::Server::setConfig( Config&& cfg ) /* Save config */
{
    boost::system::error_code error;
    m_config = ::std::move(cfg);
    auto const address = 
        boost::asio::ip::make_address( m_config.m_ipAddress, error );
    if (error)
    {
        PRINT_ERR( "%s", error.message().c_str()) ;
        return Result::WRONG_IP_ADDRESS;
    }
    m_endpoint_uptr = ::std::make_unique<Endpoint>( address, m_config.m_portNum );
    m_acceptor_uptr = ::std::make_unique<Acceptor>( m_ioService, * m_endpoint_uptr );
    return Result::ALL_GOOD;
}

void TcpIp::Server::start( void )
{
    accept();
    m_worker = \
        ::std::move(
            ::std::thread(
                [&]()
                { 
                    m_ioService.run(); 
                } )
        );
}

void TcpIp::Server::accept( void )
{
#if (1)
    SessionShptr session_shptr = ::std::make_shared< Session >(m_ioService, this); 
    m_acceptor_uptr->async_accept(session_shptr->getSocket(),
        [&, session_shptr]( ErrCode error )
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
        }
    );
#else
    m_acceptor_uptr->async_accept(
        [this]( ErrCode error, Socket socket)
        {
            if (!error)
            {
            std::make_shared<Session>(std::move(socket))->start();
            }

            accept();
        } );
#endif
} 


TcpIp::Server::~Server()
{
    m_acceptor_uptr->cancel();
    m_acceptor_uptr->close();
    closeAllSessions();
    m_ioService.stop();
    m_worker.join();
}