#include "UnixSocket.h"

using namespace UnixSocket;

Result Client::setConfig( Config&& cfg )
{
    m_config = cfg;
    ERR_CHECK( m_config.m_address,      "file name" );
    ERR_CHECK( m_config.m_authKey,      "authentication" );
    ERR_CHECK( m_config.m_delimiter,    "delimiter" );
    ERR_CHECK( m_config.m_clientName,   "client name" );
    if( ! m_config.m_recvCallBack )
    {
        PRINT_ERR( "No read callback provided.\n" );
        return Result::CFG_ERROR;
    }
    if( ! m_config.m_sendCallBack )
    {
        PRINT_ERR( "No send callback provided.\n" );
        return Result::CFG_ERROR;
    }
    m_isConfigured.store( true );    
    PRINTF( GRN, "Configuration is accepted.\n" );
    return Result::ALL_GOOD;
}

Result Client::start( void )
{
    if( ! m_isConfigured.load() )
    {
        PRINT_ERR( "Server has no configuration.\n" );
        return Result::CFG_ERROR;
    }
    ::std::cout << "Starting Unix client. Server address : " 
                << m_config.m_address <<::std::endl;
    
    m_socket_uptr = ::std::make_unique< Socket >(m_ioService);
    m_endPoint_uptr = ::std::make_unique< EndPoint >( m_config.m_address );
    connect();
    m_worker = ::std::move( 
        ::std::thread( [&](){ m_ioService.run(); } )
    );
    return Result::ALL_GOOD;
}

#define CON_RETRY_DELAY ::std::chrono::milliseconds( 500 )
void Client::connect( void )
{
    switch( m_config.m_conType )
    {
        case ConnectType::ASYNC_CONNECT :
        {
            m_socket_uptr->async_connect( * m_endPoint_uptr, 
            [&] ( const ::boost::system::error_code& error )
            {
                if( !error )
                {
                    PRINTF( GRN, "Successfully connected to the server '%s'.\n", 
                            m_endPoint_uptr->path().c_str() );
                    authenticate();
                    m_isConnected.store( true ); //let send after authentication
                    this->recv();
                } else {
                    PRINT_ERR( "Can't connect to the server '%s'.\n",
                        m_endPoint_uptr->path().c_str() );
                }
            } );
            break;
        } //end ASYNC_CONNECT
        case ConnectType::SYNC_CONNECT : //Usually before connection client is useless
        {
            try {
                m_socket_uptr->connect( * m_endPoint_uptr );
                PRINTF( GRN, "Successfully connected to the server '%s'.\n", 
                        m_endPoint_uptr->path().c_str() );
                authenticate();
                m_isConnected.store( true );
                this->recv();
            } catch( const ::std::exception& e ) {
                PRINT_ERR( "%s.\n", e.what() );
            }
            if( ! m_isConnected.load() ) //try again
            {
                /* Delay */
                ::std::this_thread::sleep_for( CON_RETRY_DELAY );
                this->connect();
            }
            break;
        } //end SYNC_CONNECT
        default :
        {
            throw std::runtime_error( "Undefined connection type.\n" );
        }
    } //end switch
}

void Client::authenticate( void )
{
    Tree xmlTree;
    xmlTree.put( m_config.m_authKey, m_config.m_clientName );
    ::std::ostringstream xmlStream;
    PropTree::write_xml( xmlStream, xmlTree );
    PRINTF( YEL, "Sending authentication : %s.\n", xmlStream.str().c_str() );
    send( xmlStream.str() );
}

void Client::recv( void )
{
    BufferShPtr readBuf_shptr = ::std::make_shared< Buffer >();
    readBuf_shptr->reserve( RECV_BUF_SIZE );
    ::boost::asio::async_read_until( * m_socket_uptr,
        ::boost::asio::dynamic_buffer( * readBuf_shptr ),
        ::std::string{ "</" + m_config.m_delimiter + ">" },
        [ &, readBuf_shptr ] ( const ErrCode& error, 
            ::std::size_t bytes_transferred ) //mutable
        {
            if( error )
            {
                PRINT_ERR( "Error when reading : %s\n", error.message().c_str());
                m_socket_uptr->shutdown(Socket::shutdown_receive);
                return;
            }
            m_config.m_recvCallBack( "", * readBuf_shptr );
            this->recv();
        } ); //end async_read_until
}

/* EOF */