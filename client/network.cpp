#include "headers/network.h"

bool initalize_client(SOCKET& server)
{
    addrinfo hints{ };
    addrinfo* servinfo{ nullptr };
    
    // IPV4 only
    hints.ai_family = AF_INET;
    
    // Required to use TCP protocol
    hints.ai_socktype = SOCK_STREAM; 

    // We are binding whatever IP is provided to a socket
    hints.ai_flags = AI_PASSIVE; 

    int result = find_server(constants::address, constants::port, &hints, &servinfo, server);

    freeaddrinfo(servinfo);
    return result;
}

bool handshake_socket(SOCKET& server)
{
    
}