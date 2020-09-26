#include "Logger.h"

#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

//STATIC
string Logger::CurrentFilePath = "logs/";
LogLevel Logger::_LogLevel = LogLevel::INFO;
bool Logger::_isLogToFile = false;
bool Logger::_isLogTime = true;

void Logger::splog( LogLevel MsgLevel, string ObjectName, string Message )
{
	if( _LogLevel == LogLevel::NO )
		return;

	if( _LogLevel != LogLevel::DEBUG )
	{
		if( MsgLevel == LogLevel::ERROR && _LogLevel != LogLevel::ERROR )
			return;
		if( MsgLevel == LogLevel::INFO && _LogLevel != LogLevel::INFO )
			return;
	}

	string output = "";

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
	cout << output;

	if( _isLogToFile && ObjectName != "Logger" )
		logToFile( output );
}

void Logger::logToFile( string Message )
{
	ofstream file;
	file.open( CurrentFilePath.c_str( ), ios::app );
	if( !file.good( ) )
	{
		splog( LogLevel::INFO, "Logger", "Something gone wrong with file" );
		return;
	}

	file << Message;
	file.close( );
}

string Logger::getPrefix( LogLevel MsgLevel )
{
	switch( MsgLevel )
	{
		case LogLevel::ERROR:
			return "[ERROR]";
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
		string sufix = to_string( i );

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
Logger::Logger( string ObjectName )
{
	ObjName = ObjectName;
}

void Logger::log( LogLevel MsgLevel, string Message )
{
	splog( MsgLevel, ObjName, Message );
}
