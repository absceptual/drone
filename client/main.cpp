#include <includes.hpp>

int main()
{
    setup_network();
    std::string domain{ };
    std::printf("[?] Enter the domain you wish to query: ");
    std::cin >> domain;

    addrinfo hints{ };
    addrinfo* servinfo{ nullptr };
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(domain.c_str(), NULL, &hints, &servinfo);
    if (status != S_OK) {
        std::printf("[!] Failed to query %s\n", domain.c_str());
        std::cin.get();
        WSACleanup();

        return -1;
    }

    std::printf("[+] Domains available for %s:\n", domain.c_str());
    std::cin.get();
    addrinfo current{ };
    for (auto current = servinfo; current != nullptr; current = current->ai_next) {
        char ip[INET_ADDRSTRLEN]{ };
        inet_ntop(AF_INET, &(current->ai_addr), ip, INET_ADDRSTRLEN);

        std::printf("\t[+] IPv4: %s\n", ip);
    }

    freeaddrinfo(servinfo);
    std::cin.get();
    WSACleanup();
    return 0;
}   