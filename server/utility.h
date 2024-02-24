#ifndef UTILITY_H
#define UTILITY_H

#include <random>
#include <chrono>

#include "../shared/includes.hpp"

// Creates a socket and returns the port it is binded to
int create_socket(addrinfo* hints, addrinfo** servinfo) {
    int status = getaddrinfo("127.0.0.1", "8080", hints, servinfo);
    if (status != S_OK) {
        std::fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        std::exit(-1);
    }
 
    // Just grab the first available address
    SOCKET sockfd{ };  
    addrinfo* current{ };
    for (current = *servinfo; current != nullptr; current = current->ai_next) {
        sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (sockfd == INVALID_SOCKET)
            std::fprintf(stderr, "socket error: 0x%x\n", WSAGetLastError());
        else
            break;
    }

    if (sockfd == INVALID_SOCKET) {
        std::fprintf(stderr, "failed to create a socket (are any available?)\n");
        std::exit(-1);
    }

    bind(sockfd, current->ai_addr, current->ai_addrlen);
    return sockfd;
}

int initalize_server()
{
    addrinfo hints{ };
    addrinfo* servinfo{ nullptr };
    
    // IPV4 only
    hints.ai_family = AF_INET;
    
    // Required to use TCP protocol
    hints.ai_socktype = SOCK_STREAM; 

    // We are binding whatever IP is provided to a socket
    hints.ai_flags = AI_PASSIVE; 

    freeaddrinfo(&hints);
    freeaddrinfo(servinfo);
    return create_socket(&hints, &servinfo);
    
}
#endif