#ifndef TCP_IP_H
#define TCP_IP_H

#include <functional>

#include <boost/asio.hpp>

#include "Tools.h"

namespace TcpIp
{
    /* Used by both : Server and Client */
    enum class Result // : int8_t
    {
        NO_SUCH_CLIENT      = -3,
        CFG_ERROR           = -2,
        WRONG_IP_ADDRESS    = -1,
        ALL_GOOD            = 0,
        SEND_SUCCESS        = 1,
    };

    class Server; //forward

    using byte = uint8_t;

    using IoService = ::boost::asio::io_service;
    using ErrCode = boost::system::error_code;

    using Socket = ::boost::asio::ip::tcp::socket;
    using IpAddress = ::boost::asio::ip::address;
    using EndPoint = ::boost::asio::ip::tcp::endpoint;
    using EndPointUptr = ::std::unique_ptr<EndPoint>;

    using Acceptor = ::boost::asio::ip::tcp::acceptor;
    using AcceptorUptr = ::std::unique_ptr<Acceptor>;

    using Buffer = ::std::vector< byte >;
    using BufferShPtr = ::std::shared_ptr< Buffer >;
    
    /* Who and what */
    using RecvCallBack  = ::std::function< void( ::std::string, Buffer ) >;
    /* How many was really send - POSIX style */
    using SendCallBack  = ::std::function< void( ::std::size_t ) >;


    class Server
    { /* Default constructable */
    class Session;
    using SessionShptr = ::std::shared_ptr< Session >;
    using Sessions = ::std::vector< SessionShptr >;

    /*--- Structures/Classes/Enums ---*/
    public :    
        struct Config
        {
            /* Can be used directly in async calls */
            RecvCallBack m_recvCallBack; 
            SendCallBack m_sendCallBack;
            ::std::string m_ipAddress;
            uint16_t m_portNum;
        }; //end struct Config

    private :
        class Session
        {
        public:
            /* Session knows about its parent - class Server */
            Session(IoService& io_service, Server * parent)
                : m_socket( io_service ),
                m_parent_ptr( parent )
            { }

            Socket& getSocket( void )
            {
                return m_socket;
            }
            ::std::string getIp( void )
            {
                /* TO DO : find better way. Copy ellision not work here. */
                ::std::ostringstream address;
                address << m_socket.remote_endpoint().address();
                return address.str();
            }
            /* Taking self to prevent destruction shared ptr */
            void recv( SessionShptr self );
            template< typename Data >
            Result send( Data&& );
        private:
            Socket m_socket;
            Server * m_parent_ptr;
            #define BUF_SIZE    1024
        }; //end class Session

        /*--- Methods ---*/
    public:
        Result setConfig( Config&& );
        Config& getConfig( void )
        {
            return m_config;
        }
        Result start( void );
        /* Send function is callable, recv is event. */
        template< typename Data >
        Result send( const ::std::string&, Data&& );
        ~Server();
    private :
        void accept( void );
        /* Maybe useful */
        void closeAllSessions( void );

        /*--- Variables ---*/
    private :
        IoService m_ioService; //Server has its own io_service
        ::std::thread m_worker;
        Config m_config;
        AcceptorUptr m_acceptor_uptr;
        EndPointUptr m_endPoint_uptr;
        Sessions m_sessions;
        ::std::atomic<bool> m_isConfigured{false};
        IpAddress m_address;
    };
}; //end namespace TcpIp

#include "TcpIpServer.hpp"
#include "TcpIpSession.hpp"

#endif /* TCP_IP_H */