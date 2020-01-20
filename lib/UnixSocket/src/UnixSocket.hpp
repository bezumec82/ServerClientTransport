#ifndef UNIX_SOCKET_HPP
#define UNIX_SOCKET_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <atomic>
#include <sstream>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "Tools.h"



namespace UnixSocket
{
    namespace PropTree = ::boost::property_tree;

    class Server;
    class Client;

    using IoService     = ::boost::asio::io_service;
    using ErrCode       = ::boost::system::error_code;

    using EndPoint      = ::boost::asio::local::stream_protocol::endpoint;
    using EndPointUptr  = ::std::unique_ptr< EndPoint >;
    using Acceptor      = ::boost::asio::local::stream_protocol::acceptor;
    using AcceptorUptr  = ::std::unique_ptr< Acceptor >;

    using Tree = ::boost::property_tree::ptree;

    using Socket        = ::boost::asio::local::stream_protocol::socket;
    using SocketUptr    = ::std::unique_ptr< Socket >;
    using RecvCallBack  = ::std::function< void( ::std::string ) >;
    using SendCallBack  = ::std::function< void( ::std::size_t ) >;
}

namespace UnixSocket
{

    enum class Result //: int8_t
    {
        AUTH_FAILURE    = -3,
        NO_SUCH_ADDRESS = -2,
        CFG_ERROR       = -1,
        ALL_GOOD        = 0,
        SEND_SUCCESS    = 1,
        AUTH_SUCCESS    = 2,
    }; //end class Result

    class Server /* Default constructable */
    {
        class Session;
        using SessionShptr  = ::std::shared_ptr< Session >;
        using AuthSessions  = ::std::unordered_map< ::std::string, SessionShptr >;
        using Sessions      = ::std::vector< SessionShptr >;

    public :
        struct Config
        {
            RecvCallBack    m_recv_cb;
            SendCallBack    m_send_cb;
            ::std::string   m_address; //address of server in the file system
            ::std::string   m_delimiter;
                /* Each message should have some kind of start-end sequence :
                 *  <body>
                 *      ...
                 *  </body>
                 */
            ::std::string   m_authKey;
                /* for example XML is used for communication.
                 * Server will wait for key of type <'m_auth_key'>ClientName</'m_auth_key'>.
                 * For example :
                 * <auth>ClientName</auth>, or :
                 * <name>ClientName</name>,
                 * until that no transactions will pass through Session class.
                 */

        }; //end struct Config
    private : /* No access to the Sessions from outside */
        class Session
        {
        public :
            Session(IoService& io_service, Server * parent)
                : m_ioService_ref( io_service ),
                m_socket( io_service ),
                m_parent_ptr( parent )
            { }

            Socket& getSocket( void )
            {
                return m_socket;
            }
            void recv( SessionShptr );
            Result authentication( SessionShptr , const ::std::string& );
            template< typename Data >
            void send( Data&& );

        private :
            IoService& m_ioService_ref;
            Socket m_socket;
            Server * m_parent_ptr;
            ::std::string m_read_buf;
            ::std::atomic< bool > m_isAuthenicated{ false };
        }; /* end class Session */

    public : /* Interface */
        /* Getters and Setters */
        Result setConfig ( Config && cfg );
        Config& getConfig( void )
        {
            return m_config;
        }        
        AuthSessions& getAuthSessions( void )
        {
            return m_authSessions;
        }
        Result start( void );
        template< typename Data >
        Result send( const ::std::string& clientName, Data&& data );

    private :
        void accept( void );
    
    private : /* Variables */
        Config m_config;
        IoService m_ioService; /* Initialized with class creation */
        AcceptorUptr m_acceptor_uptr;
        Sessions m_sessions; /* Server should know about all opened sessions */
        AuthSessions m_authSessions;
        ::std::thread m_worker;
        ::std::atomic< bool > m_isConfigured { false };
    }; //end class Server



    class Client /* Default constructable */
    {
    public :
        enum class ConnectType //: uint8_t
        {
            ASYNC_CONNECT = 0,
            SYNC_CONNECT = 1
        };

        struct Config
        {
            RecvCallBack    m_recv_cb;
            SendCallBack    m_send_cb;
            ::std::string   m_address; //file to connect to
            ::std::string   m_delimiter; //look 'Server::Config'

            /* For authentification at server side */
            ::std::string   m_authKey; //look 'Server::Config'
            ::std::string   m_clientName;
            ConnectType     m_conType;
        };
        Result setConfig( Config&& );
        Result start( void );

        template< typename Data >
        void send( Data&& data );
        ~Client() { m_socket_uptr->close(); }
    private :
        void connect( ConnectType );
        void recv( void );
        void authenticate( void );
    private : /* Variables */
        Config m_config;
        SocketUptr m_socket_uptr;
        EndPointUptr m_endPoint_uptr;
        ::std::atomic< bool > m_isConfigured{ false };
        ::std::atomic<bool> m_isConnected{ false };
        IoService m_ioService;
        ::std::thread m_worker;
        ::std::string m_read_buf;
    }; //end class client
}

/* c-style helpers */
#define ERR_CHECK(val, msg) \
    if( val == "" ) \
    { \
        PRINT_ERR( "No %s provided.\n", msg ); \
        return Result::CFG_ERROR; \
    }


#include "UnixSocketClient.hpp"
#include "UnixSocketServer.hpp"
#include "UnixSocketSession.hpp"


#endif /* UNIX_SOCKET_HPP*/