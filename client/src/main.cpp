#include "../headers/includes.h"
#include "../headers/client.h"
#include "../headers/socket.h"
#include "../headers/utility.h"
#include "../headers/packet.h"

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
    std::printf( "[+] client attempts to say hello server!\n" );

    const char* data = "hello server!";
    client.send( data, strlen(data) );
    std::printf( "[+] server responds and says %s\n", client.recv_string( ).c_str( ) );

    std::printf( "[+] client sends the server something in binary..\n" );

    unsigned char secret = 0x90;
    client.send( &secret, sizeof( std::uint8_t ) );

    auto response_string = client.recv_string( );
    if ( response_string.empty() )
    {
        std::printf( "[+] server responded with nothing..." );
        std::cin.get( );

        return -3;
    }

    std::printf( "[+] server responds and says: 0x%s\n", response_string.c_str( ) );

    auto response = client.recv( );
    bool correct = *response.data( ) == secret;
    std::printf( "[+] did the server get it right? %s\n", correct ? "yes!" : "no" );
    std::cin.get( );
    return 0;
}   
