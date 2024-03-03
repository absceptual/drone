#include "../headers/socket.h"

namespace network 
{
    socket::socket( std::string host, std::string port )
    {   
        addrinfo hints{ };
        addrinfo* servinfo{ nullptr };
        
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

    bool socket::is_connected( ) const
    {
        return this->m_connected;
    }

    int socket::send( const std::uint8_t* data, std::size_t size )
    {
        std::vector<uint8_t> frame{ };
        uint8_t mask[ 4 ];
        RAND_bytes( mask, 4 );

        size_t sent{ };
        while ( sent < size )
        {
            size_t fragment_size = min( size - sent, MAX_FRAGMENT_SIZE );
            bool is_final = ( sent + fragment_size ) == size;

            frame.clear( );

            // Prepare WebSocket frame header
            uint8_t header = is_final ? 0x80 : 0x00; // Set FIN bit accordingly
            if ( sent == 0 )
                header |= opcode; 
            else
                header |= 0x0; 
       

            // fin and opcode
            frame.push_back( header );

            // length
            frame.push_back( 126 | 0x80 ); // Payload length set to 127 for next 8 bytes length
            frame.push_back( ( fragment_size >> 8 ) & 0xFF ); // Extended payload length (high byte)
            frame.push_back( fragment_size & 0xFF );        // Extended payload length (low byte)

            // mask
            for ( int i = 0; i < 4; ++i )
                frame.push_back( mask[ i ] );

            // data
            for ( size_t i = 0; i < fragment_size; ++i )
                frame.push_back( data[ i + sent ] ^ mask[ i % 4 ] );

            ::send( m_socket, reinterpret_cast< const char* >( frame.data( ) ), frame.size( ), NULL );
            sent += fragment_size;
        }

        // fin and opcode
        frame.push_back( 0x00 );
        frame.clear( );
        ::send( m_socket, reinterpret_cast< const char* >( frame.data( ) ), frame.size( ), NULL );
        return sent;
    }

    void socket::recv( ) 
    {
        // Clear whatever data we had left from our old receive
        m_data.clear( );

        bool complete = false;
        while (!complete )
        {
            // Extract whether or not this is the last bit of the message
            // In reality, I should check the opcode but we only expect binary data anyway
            char header[ 2 ]{ };

            ::recv( m_socket, header, sizeof( header ), NULL );
            bool fin = ( header[ 0 ] & 0x80 ) != 0;
            if (fin)
                complete = true;
            
            // Get the (unextended) length of the payload and then adjust it accordingly
            uint64_t length{ };
            length = header[ 1 ] & 0x7F;
            switch ( length )
            {
            case 126:
                ::recv( m_socket, reinterpret_cast< char* >( &length ), sizeof( std::uint16_t ), NULL );
                length = ntohs( length );
                break;
            case 127:
                ::recv( m_socket, reinterpret_cast< char* >( &length ), sizeof( std::uint64_t ), NULL );
                length = ntohll( length );
                break;
            default:
                break;
            }

            // Add to our final product at the end
            std::vector<uint8_t> frame{ };
            frame.resize( length );

            ::recv( m_socket, reinterpret_cast< char* >( frame.data( ) ), length, NULL );
            m_data.insert( m_data.end( ), frame.begin( ), frame.end( ) );
        }

        m_data.shrink_to_fit( );
    }

    std::vector<std::uint8_t>& socket::get_data( )
    {
        return m_data;
    }
    
}




