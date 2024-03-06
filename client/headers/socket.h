#pragma once
#include "includes.h"


namespace network
{
    constexpr std::uint8_t opcode = 0x2; // binary opcode for ws data
    constexpr size_t MAX_FRAGMENT_SIZE = 1024;

    class socket
    {
    private:
        SOCKET m_socket{ };
        bool m_connected{ };
        std::vector<std::uint8_t> m_data{ };

    public:
        socket( ) = default;
        socket( std::string host, std::string port );

        SOCKET& get_socket( ) const;
        bool is_connected( ) const;

        int send( const void* data, std::size_t size );
        void recv( );

        std::vector<std::uint8_t>& get_data( );
    };
}
