#ifndef UNIX_SOCKET_SERVER_HPP
#define UNIX_SOCKET_SERVER_HPP

#include "UnixSocket.h"

namespace UnixSocket
{
/* Wrapper around 'Session.send' */
template< typename Data >
Result Server::send( const ::std::string& clientName, Data&& data )
{
    /* Not sure that this is necessary */
    if( ! m_isStarted.load() )
    {
    #if (0)
        PRINT_ERR( "Trying to use not started server.\n" );
        return Result::SEND_ERROR;
    #else
        throw ::std::runtime_error( "Trying to use not started server.\n" );
    #endif
    }
    /* Client should provide some kind recognition. */
    auto found = m_authSessions.find(clientName);
    if( found != m_authSessions.end() )
    {
        /* Underlaying class is Session */
        found->second->send(::std::forward<Data>(data) );
        return Result::SEND_SUCCESS;
    }
    else
    {
        PRINT_ERR( "No such client : %s.\n", clientName.c_str() );
        return Result::NO_SUCH_ADDRESS;
    }
}
} //end namespace UnixSocket

#endif /* UNXI_SOCKET_SERVER_HPP */