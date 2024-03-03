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

        SOCKET& get_socket( );
        bool is_connected( ) const;

        // DATA HAS TO BE ATLEAST 1024
        int send( const std::uint8_t* data, std::size_t size );
        void recv( );

        std::vector<std::uint8_t>& get_data( );
    };
}
