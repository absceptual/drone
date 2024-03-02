#include "../headers/utility.h"

namespace utility 
{
	std::string base64_encode( std::uint8_t* buffer, size_t length )
	{
		BIO* bio, * b64;
		BUF_MEM* bufferPtr;

		b64 = BIO_new( BIO_f_base64( ) );
		bio = BIO_new( BIO_s_mem( ) );
		bio = BIO_push( b64, bio );

		BIO_set_flags( bio, BIO_FLAGS_BASE64_NO_NL ); //Ignore newlines - write everything in one line
		BIO_write( bio, buffer, length );
		BIO_flush( bio );
		BIO_get_mem_ptr( bio, &bufferPtr );
		BIO_set_close( bio, BIO_NOCLOSE );

		std::string text( (*bufferPtr ).data, ( *bufferPtr ).length );
		BIO_free_all( bio );

		return text;
	}

	std::map<std::string, std::string> parse_header( const std::string& header )
	{
		std::map<std::string, std::string> headers{ };
		std::istringstream stream( header );
		std::string line;

		while ( std::getline( stream, line ) && line != "\r" )
		{
			auto pos = line.find( ':' );
			if ( pos != std::string::npos )
			{
				std::string key = line.substr( 0, pos );
				std::string value = line.substr( pos + 2, line.length( ) - pos - 3 ); // +2 and -3 adjust for colon and \r\n
				headers[ key ] = value;
			}
		}

		return headers;
	}
}
