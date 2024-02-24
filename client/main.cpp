#include "headers/includes.h"
#include "headers/utility.h"
#include "headers/network.h"

int main()
{
    // boring wsa setup...
    setup_network();

    SOCKET server{};
    if (!initalize_client(server))
    {
        std::printf("[!] failed to initalize client! %d\n", WSAGetLastError());
        std::cin.get();

        std::exit(-1);
    }
    std::string request = "GET https://localhost:8080\r\n"
                          "Host: localhost:8080\r\n"
                          "Connection: Upgrade\r\n"
                          "Upgrade: websocket\r\n"
                          "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDf==\r\n"
                          "Sec-WebSocket-Version: 13\r\n\r\n";
    
    int result = send(server, request.c_str(), request.length(), 0);
    std::printf("%d\n", result);
    if (result == SOCKET_ERROR) {
        std::printf("[!] failed to upgrade client! %d\n", WSAGetLastError());
        std::cin.get();

        std::exit(-1);
        // Handle error
    }
    std::printf("[+] connected!\n");
    char data[1024]{};
    recv(server, data, sizeof(data), 0);

    std::printf("[data] sent?\n");
    std::printf("%s\n", data);
    std::cin.get();
    WSACleanup();
    return 0;
}   