#include <includes.hpp>
#include "utility.h"

int main()
{
    // boring wsa setup...
    setup_network();
    SOCKET socket = initalize_server();
    
    int result = listen(socket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::printf("listen error: 0x%d\n", result);
        std::exit(-1);
    }
    std::printf("!");

    sockaddr_in name{ };
    int size = sizeof(sockaddr_in);
    getsockname(socket, (sockaddr*)(&name), &size);

    char address[INET_ADDRSTRLEN]{};
    inet_ntop(AF_INET, (LPCVOID*)(&name.sin_addr), address, INET_ADDRSTRLEN);
    std::printf("listening to connections from %s on port %d", address, name.sin_port);
    std::cin.get();
    WSACleanup();
    return 0;
}   