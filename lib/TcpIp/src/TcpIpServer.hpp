#ifndef TCP_IP_SERVER_HPP
#define TCP_IP_SERVER_HPP

#include "TcpIp.h"

namespace TcpIp
{
/* Wrapper around 'Session.send' */
template< typename Data >
Result Server::send( const ::std::string& address, Data&& data )
{
    /* Hash will be better here : use 'unordered_map' */
    for( const auto & it : m_sessions )
    {
        if ( it->getIp() == address )
        {
            it->send( ::std::forward<Data>(data) ); //perfect forwarding
            return Result::SEND_SUCCESS; 
        }
    } //end for
    return Result::NO_SUCH_CLIENT;
}//end send

} //end namespace TcpIp

#endif /* TCP_IP_HPP */