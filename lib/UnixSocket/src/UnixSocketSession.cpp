#include "UnixSocket.h"

using namespace UnixSocket;

void Server::Session::recv( SessionShptr self )
{

    ::std::string delimiter = "";
    if( ! m_isAuthenticated.load() )
    {
        delimiter = ::std::string{ "</" + m_parent_ptr->getConfig().m_authKey + ">" };
    }
    else
    {
        delimiter = ::std::string{ "</" + m_parent_ptr->getConfig().m_delimiter + ">" };
    }

    BufferShPtr readBuf_shptr = ::std::make_shared< Buffer >("");
    readBuf_shptr->reserve( RECV_BUF_SIZE ); //don't trust boost dynamic allocation
    ::boost::asio::async_read_until( m_socket,
    ::boost::asio::dynamic_buffer( * readBuf_shptr ),
    delimiter,
    [&, self, readBuf_shptr] ( const ErrCode& error, 
    ::std::size_t bytes_transferred ) //mutable
    {
        if( error )
        {
            PRINT_ERR( "Error when reading : %s\n", error.message().c_str());
            m_socket.shutdown( Socket::shutdown_receive );
            return;
        }
        if( ! m_isAuthenticated.load() )
        {
            authentication( self, * readBuf_shptr );
        } else { /* Give access to data after authentication. */
            m_parent_ptr->getConfig().m_recvCallBack( m_remoteName, * readBuf_shptr);
        }
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
        m_remoteName = xmlTree.get<std::string>( m_parent_ptr->getConfig().m_authKey );
        m_parent_ptr->getAuthSessions().emplace(
            ::std::make_pair(
                m_remoteName, 
                self 
        ) );
        m_isAuthenticated.store(true);
        PRINTF( GRN, "Client '%s' successfully authenticated.\n", m_remoteName.c_str() );
        return Result::AUTH_SUCCESS;
    } catch( const ::std::exception& e )
    {
        PRINT_ERR( "%s\n", e.what());
        return Result::AUTH_FAILURE;
    }
}

/* EOF */