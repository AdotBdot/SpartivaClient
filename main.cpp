#include "Client.h"

#include "Logger.h"

int main( int argc, char **argv )
{
	Logger::init( );
	Logger::LogTime( true );
	Logger::LogToFile( false );
	Logger::setLogLevel( LogLevel::DEBUG );

	Client c( argc, *argv );
	c.run( );

	return 0;
}