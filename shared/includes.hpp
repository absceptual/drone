#ifndef INCLUDES_HPP
#define INCLUDES_HPP


#define WIN32_LEAN_AND_MEAN
#define WINSOCK_VERSION MAKEWORD(2, 2)

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>

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

#endif