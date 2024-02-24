#pragma once
#include <random>
#include <chrono>

#include "includes.h"

void setup_network();
bool find_server(const char* address, const char* port, addrinfo* hints, addrinfo** servinfo, SOCKET& server);

