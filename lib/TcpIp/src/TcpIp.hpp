#ifndef TCP_IP_HPP
#define TCP_IP_HPP

#include <functional>

#include <boost/asio.hpp>

#include "Tools.h"

namespace TcpIp
{
    class Session;
    class Server;

    using IoService = ::boost::asio::io_service;
    using ErrCode = boost::system::error_code;
    using Socket = ::boost::asio::ip::tcp::socket;

    using Endpoint = ::boost::asio::ip::tcp::endpoint;
    using EndpointUptr = ::std::unique_ptr<Endpoint>;

    using Acceptor = ::boost::asio::ip::tcp::acceptor;
    using AcceptorUptr = ::std::unique_ptr<Acceptor>;

    using Callable = 
        ::std::function< void( const ::boost::system::error_code , ::std::size_t ) >;
    
    using SessionShptr = ::std::shared_ptr< Session >;
    using Sessions = ::std::vector< SessionShptr >;
    class Session
    {
    public:
        Session(IoService& io_service, Server* parent)
            : m_ioService_ref( io_service ),
            m_socket( io_service )
        { }

        Socket& getSocket( void )
        {
            return m_socket;
        }
        void recv( SessionShptr self )
        {

        }
        void start( void ) {}
    private:
        IoService& m_ioService_ref;
        Socket m_socket;

    };

    class Server
    { /* Default constractible */
    public :
        enum class Result : int8_t
        {
            WRONG_IP_ADDRESS = -1,
            ALL_GOOD = 0,

        };

        struct Config 
        {
            Callable m_recvCallbak;
            Callable m_sendCallbak;
            ::std::string m_ipAddress;
            uint16_t m_portNum;
        };

        Result setConfig( Config&& );
        void start( void );
        void stop( void );
        ~Server();
    private :
        void accept( void );
        void closeAllSessions( void );
        Config m_config;
        AcceptorUptr m_acceptor_uptr;
        EndpointUptr m_endpoint_uptr;
        IoService m_ioService; /* Created with class */
        ::std::thread m_worker;
        Sessions m_sessions;
    };


}; //end namespace LocalHost

#endif /* TCP_IP_HPP */