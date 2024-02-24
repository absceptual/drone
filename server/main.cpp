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

    std::printf("listening to connections from 127.0.0.0:8080\n");
    while (!(GetAsyncKeyState(VK_END) & 1))
    {
        SOCKET connection{ };
        sockaddr_storage data{ };
        int size = sizeof(sockaddr_storage);

        connection = accept(socket, (sockaddr*)&data, &size);
        if (connection == -1)
            continue;

        char connected_address[INET_ADDRSTRLEN]{ };
        inet_ntop(data.ss_family,
            (&(((sockaddr_in*)(&data))->sin_addr)),
            connected_address,
            INET_ADDRSTRLEN);
        printf("server: got connection from %s\n", connected_address);
        if (send(connection, "Hello, world!", 13, 0) == -1) {
            std::printf("[!] failed to send packet lmaoooo\n");
        }
        closesocket(connection);
    }
    WSACleanup();
    return 0;
}   