#ifndef UTILITY_H
#define UTILITY_H

#include <chrono>
#include <random>

#include "../shared/includes.hpp"

// Creates a socket on the localhost
int create_socket( addrinfo *hints, addrinfo **servinfo ) {

    // Kindly ask for a list of sockets on our localhost (port 8080)
    int status = getaddrinfo( "127.0.0.1", "8080", hints, servinfo );
    if ( status != S_OK ) {
        std::fprintf( stderr, "getaddrinfo error: %s\n",
                      gai_strerror( status ) );
        std::exit( -1 );
    }

    // Just grab the first available address
    SOCKET sockfd{ };
    addrinfo *current{ };
    for ( current = *servinfo; current != nullptr;
          current = current->ai_next ) {
        sockfd = socket( current->ai_family, current->ai_socktype,
                         current->ai_protocol );
        if ( sockfd == INVALID_SOCKET )
            std::fprintf( stderr, "socket error: 0x%x\n", WSAGetLastError( ) );
        else
            break;
    }

    if ( sockfd == INVALID_SOCKET ) {
        std::fprintf( stderr, "create_socket error (are any available?)\n" );
        std::exit( -1 );
    }

    // inshallah we will bind our port
    bind( sockfd, current->ai_addr, current->ai_addrlen );
    return sockfd;
}

// Returns a newly created socket on 127.0.0.1:8080
int initalize_server( ) {
    addrinfo hints{ };
    addrinfo *servinfo{ nullptr };

    hints.ai_family = AF_INET;       // IPV4 only
    hints.ai_socktype = SOCK_STREAM; // Required to use TCP protocol
    hints.ai_flags =
        AI_PASSIVE; // We are binding whatever IP is provided to a socket

    freeaddrinfo( &hints );
    freeaddrinfo( servinfo );
    return create_socket( &hints, &servinfo );
}

#endif