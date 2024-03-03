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
        socket m_socket{ };

    public:
        client( std::string host, std::string port );
        ~client( );

        bool open_connection( );
        void close_connection( );

        int send( std::uint8_t* buffer, size_t size, std::uint8_t opcode = network::binary_opcode );
    };
}