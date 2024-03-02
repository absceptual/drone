#pragma once
#include "includes.h"


namespace network 
{
    constexpr std::uint8_t text_opcode = 0x1;
    constexpr std::uint8_t binary_opcode = 0x2;
    constexpr size_t MAX_FRAGMENT_SIZE = 1024;

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

        // DATA HAS TO BE ATLEAST 1024
        int send(std::uint8_t* data, std::size_t size, std::uint8_t opcode );
        void send_frame_header( std::vector<uint8_t>& frame, std::uint8_t header );
        void send_frame_payload( std::vector<uint8_t>& frame, std::uint8_t* data, std::uint8_t size, std::uint8_t* mask );
        int recv(std::uint8_t* buffer, std::size_t size);
    };
}
