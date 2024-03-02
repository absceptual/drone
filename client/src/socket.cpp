#include "../headers/socket.h"

namespace network 
{
    socket::socket( std::string host, std::string port )
    {   
        addrinfo hints{ };
        addrinfo* servinfo{ nullptr };
        
        // IPV4 only
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        int status{ getaddrinfo( host.c_str( ), port.c_str( ), &hints, &servinfo ) };
        if ( status != S_OK ) 
        {
            std::fprintf( stderr, "[!] sock::sock( %s, %s ): getaddrinfo error 0x%ls\n", host.c_str( ), port.c_str( ), gai_strerror( status ) );
            std::cin.get( );
            std::exit( -1 );
        }

        addrinfo* current{ nullptr };
        for (current = servinfo; servinfo != nullptr; servinfo = servinfo->ai_next) {
            
            int target_port = ( servinfo->ai_family == AF_INET ) 
                                ? reinterpret_cast< sockaddr_in* >( current->ai_addr )->sin_port
                                : reinterpret_cast< sockaddr_in6* >( current->ai_addr )->sin6_port;
            
            if ( htons( std::stoi( port ) ) != target_port )
                continue;

            m_socket = ::socket( current->ai_family, current->ai_socktype, current->ai_protocol );
            if ( m_socket == INVALID_SOCKET )
                std::fprintf( stderr, "[!] sock::sock( %s, %s ): socket error 0x%x\n", host.c_str( ), port.c_str( ), WSAGetLastError( ) );
            else
                break;

        }
        m_connected = !connect( m_socket, current->ai_addr, static_cast< int >( current->ai_addrlen ) );
        freeaddrinfo( servinfo );
    }

    SOCKET& socket::get_socket( )
    {
        return m_socket;
    }

    bool network::socket::is_connected( ) const
    {
        return this->m_connected;
    }

    int send( std::uint8_t* data, std::size_t size )
    {
        return 0;
    }
}




