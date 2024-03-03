#pragma once
#include "includes.h"
#include "socket.h"
#include "utility.h"


namespace network
{
    class client 
    {
    private:
        const std::string m_host{ };
        const std::string m_port{ };
        char* m_data{ };
        socket m_socket;

    public:
        client( std::string host, std::string port );
        ~client( );

        bool open_connection( );
        void close_connection( );

        int send( const void* buffer, size_t size );
        std::vector<std::uint8_t> recv( );
        std::string recv_string( );
    };
}
