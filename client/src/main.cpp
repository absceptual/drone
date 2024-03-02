#include "../headers/includes.h"
#include "../headers/client.h"
#include "../headers/socket.h"
#include "../headers/utility.h"

int main()
{
    // OpenSSL initialization
    {
        SSL_library_init( );
        OpenSSL_add_all_algorithms( );
        SSL_load_error_strings( );
    }

    network::client client{ "127.0.0.1", "8080" };
    if ( !client.open_connection() ) 
    {
        std::fprintf( stderr, "[!] failed to initialize network::client!" );
        std::cin.get( );
        std::exit( -2 );
    }
    
    std::printf( "[+] client initialized!\n" );
    std::cin.get();
    WSACleanup();
    return 0;
}   
