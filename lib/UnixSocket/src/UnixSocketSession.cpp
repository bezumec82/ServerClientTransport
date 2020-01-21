#include "UnixSocket.h"

using namespace UnixSocket;

void Server::Session::recv( SessionShptr self )
{
    ::std::string delimiter = "";
    if( ! m_isAuthenicated.load() )
    {
        delimiter = ::std::string{ "</" + m_parent_ptr->getConfig().m_authKey + ">" };
    }
    else
    {
        delimiter = ::std::string{ "</" + m_parent_ptr->getConfig().m_delimiter + ">" };
    }

    ::boost::asio::async_read_until( m_socket,
    ::boost::asio::dynamic_buffer( m_read_buf ),
    delimiter,
    [&, self] ( const ErrCode& error, 
    ::std::size_t bytes_transferred ) //mutable
    {
        if( error )
        {
            PRINT_ERR( "Error when reading : %s\n", error.message().c_str());
            m_socket.shutdown( Socket::shutdown_receive );
            return;
        }
        if( ! m_isAuthenicated.load() )
        {
            authentication( self, m_read_buf );
        } else { /* Give access to data after authentication. */
            m_parent_ptr->getConfig().m_recv_cb(m_read_buf);
        }
        m_read_buf.clear();
        this->recv( self );
    } ); //end async_read_until
}

Result Server::Session::authentication( 
    SessionShptr self,
    const ::std::string& inData )
{
    /* Actual parsing here */
    Tree xmlTree;
    ::std::istringstream xmlStream( inData );
    PropTree::read_xml( xmlStream, xmlTree );
    try {
        ::std::string key = xmlTree.get<std::string>( m_parent_ptr->getConfig().m_authKey );
        m_parent_ptr->getAuthSessions().emplace(
            ::std::make_pair(
                key, 
                self 
        ) );
        m_isAuthenicated.store(true);
        PRINTF( GRN, "Client '%s' successfully authenticated.\n", key.c_str() );
        return Result::AUTH_SUCCESS;
    } catch( const ::std::exception& e )
    {
        PRINT_ERR( "%s\n", e.what());
        return Result::AUTH_FAILURE;
    }
}

/* EOF */