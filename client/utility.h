#ifndef UTILITY_H
#define UTILITY_H

#include <random>
#include <chrono>

#include "../shared/includes.hpp"

bool find_server(const std::string& address, const std::string& port, addrinfo* hints, addrinfo** servinfo, SOCKET& server) 
{
    int status = getaddrinfo(address.c_str(), port.c_str(), hints, servinfo);
    if (status != S_OK) {
        std::printf("getaddrinfo error: %s\n", gai_strerror(status));
        std::cin.get();
        std::exit(-1);
    }

    // Just grab the first available address
    SOCKET sockfd{ };  
    addrinfo* current{ };

    int port_number = std::stoi(port.c_str());
    for (current = *servinfo; current != nullptr; current = current->ai_next) {
        if (port_number != ((sockaddr_in*)(&(current->ai_addr)))->sin_port)
            continue;

        sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (sockfd == INVALID_SOCKET)
            std::fprintf(stderr, "socket error: 0x%x\n", WSAGetLastError());
        else
            break;
    }

    sockfd = socket((*servinfo)->ai_family, (*servinfo)->ai_socktype, (*servinfo)->ai_protocol);
    if (sockfd == INVALID_SOCKET) {
        std::printf("socket error: 0x%x\n", WSAGetLastError());
        std::cin.get();
        std::exit(-1);
    }

    server = sockfd;
    status = connect(sockfd, (*servinfo)->ai_addr, (*servinfo)->ai_addrlen);
    return status == 0;
}

bool initalize_client(const std::string& address, const std::string& port, SOCKET& server)
{
    addrinfo hints{ };
    addrinfo* servinfo{ nullptr };
    
    // IPV4 only
    hints.ai_family = AF_INET;
    
    // Required to use TCP protocol
    hints.ai_socktype = SOCK_STREAM; 

    // We are binding whatever IP is provided to a socket
    hints.ai_flags = AI_PASSIVE; 

    int result = find_server(address, port, &hints, &servinfo, server);

    freeaddrinfo(&hints);
    freeaddrinfo(servinfo);
    return result;
}

#endif