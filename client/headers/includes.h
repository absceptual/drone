#pragma once

#define WIN32_LEAN_AND_MEAN
#define WINSOCK_VERSION MAKEWORD(2, 2)

#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
