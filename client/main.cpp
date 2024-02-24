#include <includes.hpp>
#include "utility.h"

int main()
{
    // boring wsa setup...
    setup_network();

    std::string address, port;
    std::cin >> address >> port;
    std::cin.get();

    SOCKET server{};
    if (!initalize_client(address, port, server))
    {
        std::printf("[!] failed to initalize client! %d\n", WSAGetLastError());
        std::cin.get();

        std::exit(-1);
    }
    
    std::printf("[+] connected!\n");
    std::printf("%d\n", WSAGetLastError());
    char data[256]{};
    recv(server, data, sizeof(data), 0);

    std::printf("[data] %s\n", data);
    std::cin.get();
    WSACleanup();
    return 0;
}   