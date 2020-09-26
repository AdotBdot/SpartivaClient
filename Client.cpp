#include "Client.h"

Client::Client( int mainArgC, std::string mainArgs )
{
	init( );
}

Client::~Client( )
{
	delete Lgr;
}

void Client::init( )
{
	Logger::init( );
	Logger::LogTime( true );
	Logger::LogToFile( false );
	Logger::setLogLevel( LogLevel::DEBUG );

	Lgr = new Logger( "MainThread" );
	Lgr->log( LogLevel::INFO, "Initializiation..." );



	Lgr->log( LogLevel::INFO, "Initializiaed" );
}

void Client::run( )
{
}
