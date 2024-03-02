#pragma once
#include "includes.h"

namespace network 
{
    class socket
    {
    private:
        SOCKET m_socket{ };
        bool m_connected{ };

    public:
        socket( ) = default;
        socket( std::string host, std::string port );

        SOCKET& get_socket( );
        bool is_connected( ) const;

        int send(std::uint8_t* data, std::size_t size);
        int recv(std::uint8_t* buffer, std::size_t size);
    };
}
