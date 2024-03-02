#pragma once
#include "includes.h"

namespace utility 
{
    std::string base64_encode( std::uint8_t* buffer, size_t length );
    std::map<std::string, std::string> parse_header( const std::string& header );
}