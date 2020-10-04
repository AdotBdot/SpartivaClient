#include "Logger.h"

#include <iostream>
#include <fstream>
#include <time.h>

//STATIC
std::string Logger::CurrentFilePath = "logs/";
LogLevel Logger::_LogLevel = LogLevel::INFO;
bool Logger::_isLogToFile = false;
bool Logger::_isLogTime = true;

void Logger::splog( LogLevel MsgLevel, std::string ObjectName, std::string Message )
{
	if( MsgLevel == LogLevel::NO )
		return;

	if( _LogLevel < MsgLevel )
		return;

	std::string output = "";

	if( _isLogTime )
	{
		char stime[ 10 ];
		time_t timet;
		struct tm* data;

		time( &timet );
		data = localtime( &timet );
		strftime( stime, 10, "%H:%M:%S ", data );

		output += stime;
	}

	output += getPrefix( MsgLevel ) + " " + ObjectName + ": " + Message + "\n";

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), getColor( MsgLevel ) );
	std::cout << output;
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 7 );

	if( _isLogToFile && ObjectName != "Logger" )
		logToFile( output );
}

void Logger::logToFile( std::string Message )
{
	std::ofstream file;
	file.open( CurrentFilePath.c_str( ), std::ios::app );
	if( !file.good( ) )
	{
		splog( LogLevel::INFO, "Logger", "Something gone wrong with file" );
		return;
	}

	file << Message;
	file.close( );
}

std::string Logger::getPrefix( LogLevel MsgLevel )
{
	switch( MsgLevel )
	{
		case LogLevel::NO:
			return "";
			break;
		case LogLevel::ERROR:
			return "[ERROR]";
			break;
		case LogLevel::WARNING:
			return "[WARNING]";
			break;
		case LogLevel::INFO:
			return "[INFO]";
			break;
		case LogLevel::DEBUG:
			return "[DEBUG]";
			break;
		default:
			break;
	}
}

int Logger::getColor( LogLevel MsgLevel )
{
	switch( MsgLevel )
	{
		case LogLevel::NO:
			return 0;
			break;
		case LogLevel::ERROR:
			return 12;
			break;
		case LogLevel::WARNING:
			return 14;
			break;
		case LogLevel::INFO:
			return 7;
			break;
		case LogLevel::DEBUG:
			return 3;
			break;
		default:
			break;
	}
}

void Logger::init( )
{
	char stime[ 10 ];
	{
		time_t timet;
		struct tm* data;

		time( &timet );
		data = localtime( &timet );
		strftime( stime, 10, "%d-%m-%y-", data );
	}

	CurrentFilePath += stime;

	for( size_t i = 1;; i++ )
	{
		std::string sufix = std::to_string( i );

		CurrentFilePath += sufix + ".txt";

		std::ifstream file( CurrentFilePath.c_str( ) );
		if( file.good( ) )
		{
			CurrentFilePath.erase( 14, sufix.length( ) + 4 );
			continue;
		}
		else
			break;
	}
}

void Logger::setLogLevel( LogLevel logLevel )
{
	_LogLevel = logLevel;
}

void Logger::LogToFile( bool is )
{
	_isLogToFile = is;
}

void Logger::LogTime( bool is )
{
	_isLogTime = is;
}

Logger& Logger::getInstance( std::string Name )
{
	static Logger instance( Name );
	return instance;
}

//OBJECT
Logger::Logger( std::string ObjectName )
{
	ObjName = ObjectName;
}

void Logger::log( LogLevel MsgLevel, std::string Message )
{
	splog( MsgLevel, ObjName, Message );
}
