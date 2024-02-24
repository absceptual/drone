#pragma once
#include "includes.h"

namespace constants {
    static const char* address = "127.0.0.1";
    static const char* port    = "8080";
}

bool initalize_client(SOCKET& server);
bool handshake_socket(SOCKET& server);

