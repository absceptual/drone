#pragma once

#define WIN32_LEAN_AND_MEAN
#define WINSOCK_VERSION MAKEWORD(2, 2)

#include <iostream>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "network.h"
#include "utility.h"
