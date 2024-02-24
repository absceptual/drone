#ifndef UTILITY_H
#define UTILITY_H

#include <random>
#include <chrono>

#include "../shared/includes.hpp"

// Creates a socket and returns the port it is binded to
int create_socket(addrinfo* hints, addrinfo** servinfo) {

    // Generate a random port instead of hardcoding
    std::mt19937 mt{ static_cast<std::mt19937::result_type>(
		std::chrono::steady_clock::now().time_since_epoch().count()
	) };
    std::uniform_int_distribution range{ 1024, 65536 };
    
    char port[5]{};
    itoa(range(mt), port, 10);

    int status = getaddrinfo(NULL, port, hints, servinfo);
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