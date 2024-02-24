#include "headers/utility.h"

bool find_server(const char* address, const char* port, addrinfo* hints, addrinfo** servinfo, SOCKET& server) 
{
    int status = getaddrinfo(address, port, hints, servinfo);
    if (status != S_OK) {
        std::printf("getaddrinfo error: %s\n", gai_strerror(status));
        std::cin.get();
        std::exit(-1);
    }

    // Just grab the first available address
    SOCKET sockfd{ };  
    addrinfo* current{ };

    int port_number = std::stoi(port);
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

void setup_network()
{
    WSADATA data{ };
    HRESULT status{ WSAStartup(WINSOCK_VERSION, &data) };
    if (status != S_OK)
    {
        std::fprintf(stderr, "[!] Something went wrong trying to initalize the WSA\n");
        std::exit(-1);
    }

    if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2) {
        std::fprintf(stderr, "[!] Version 2.2 of WinSock is not available\n");
        WSACleanup();
        std::exit(-1);
    }
}