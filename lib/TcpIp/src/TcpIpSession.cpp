#include "TcpIp.h"

using namespace TcpIp;

void Server::Session::recv( SessionShptr self )
{
    /* If several thread will serve one 'io_service'
     * it will be concurent resource if it will be common.
     * So each receive have its own buffer. */
    BufferShPtr readBuf_shptr = ::std::make_shared< Buffer >(RECV_BUF_SIZE, 0);
    readBuf_shptr->resize( RECV_BUF_SIZE );
    m_socket.async_read_some( ::boost::asio::buffer( readBuf_shptr->data(), readBuf_shptr->size() ),
        [ &, readBuf_shptr, self ] ( const ErrCode& error, 
            ::std::size_t bytes_transferred ) //mutable
        {
            if( error )
            {
                PRINT_ERR( "Error when reading : %s\n", error.message().c_str() );
                m_socket.shutdown( Socket::shutdown_receive );
                return;
            }
            readBuf_shptr->resize(bytes_transferred);
            m_parent_ptr->getConfig().m_recvCallBack( getIp(), * readBuf_shptr );
            this->recv( self );
        } ); //end async_read_until
}