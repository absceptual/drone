#include "../headers/client.h"

namespace network
{
    client::client( std::string host, std::string port )
        : m_host{ host }, m_port{ port }
    {
        WSADATA data{ };
        HRESULT status{ WSAStartup( WINSOCK_VERSION, &data ) };
        if ( status != S_OK )
        {
            std::fprintf( stderr, "[!] client::client( %s, %s ): failed to initialize WSA\n", host.c_str( ), port.c_str( ) );
            std::cin.get( );
            std::exit( -1 );
        }

        if ( LOBYTE( data.wVersion ) != 2 || HIBYTE( data.wVersion ) != 2 )
        {
            std::fprintf( stderr, "[!] client::client( %s, %s ): requested version 2.2 is not available", host.c_str( ), port.c_str( ) );
            WSACleanup( );
            std::cin.get( );
            std::exit( -1 );
        }
    }

    client::~client( )
    {
        close_connection( );
        if ( m_data )
            delete[ ] m_data;
    }

    bool client::open_connection( )
    {
        m_socket = socket{ m_host, m_port };
        if ( !m_socket.is_connected( ) )
            return false;

        std::string request{ };
        request += "GET http://";
        request += m_host + ":";
        request += m_port + " HTTP/1.1\r\n";
        request += "Host: localhost:8080\r\n";
        request += "Upgrade: websocket\r\n";
        request += "Connection: Upgrade\r\n";

        std::uint8_t bytes[ 16 ]{ };
        RAND_bytes( bytes, 16 );

        std::string key = utility::base64_encode( bytes, 16 );
        request += "Sec-WebSocket-Key: " + key + "\r\n";
        request += "Sec-WebSocket-Version: 13\r\n\r\n";

        char buffer[ 512 ];
        ::send( m_socket.get_socket( ), request.c_str( ), request.size( ), NULL );
        ::recv( m_socket.get_socket( ), reinterpret_cast< char* >( buffer ), sizeof( buffer ), NULL );

        std::string raw_data( buffer );
        if ( raw_data.find( "101 Switching Protocols" ) == std::string::npos )
            return false;

        auto response = utility::parse_header( raw_data );
        if ( response.find( "Upgrade" ) == response.end( ) || response.find( "Upgrade" )->second.compare( "websocket" ) != 0 )
            return false;

        std::string raw_hash, hash;
        char sha[ 20 ]{ };

        raw_hash = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        SHA1( reinterpret_cast< const std::uint8_t* >( raw_hash.c_str( ) ), raw_hash.length( ), reinterpret_cast< std::uint8_t* >( sha ) );
        hash = utility::base64_encode( reinterpret_cast< std::uint8_t* >( sha ), 20 );

        if ( response.find( "Sec-WebSocket-Accept" ) == response.end( ) || response.find( "Sec-WebSocket-Accept" )->second.compare( hash ) )
            return false;

        return true;
    }

    void client::close_connection( )
    {
        WSACleanup( );
        if ( m_socket.is_connected( ) )
            closesocket( m_socket.get_socket( ) );
    }

    int client::send( const std::uint8_t* buffer, size_t size )
    {
        return m_socket.send( buffer, size );
    }

    std::vector<std::uint8_t> client::recv( )
    {
        m_socket.recv( );
        return m_socket.get_data( );
    }

    std::string client::recv_string( )
    {
        return utility::string_from_recv( recv( ) );
    }

}


