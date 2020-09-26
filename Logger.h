#pragma once

#include <string>

enum class LogLevel
{
	NO = 0,
	ERROR = 1,
	INFO = 2,
	DEBUG = 3
};

class Logger
{
	//STATIC
private:
	static std::string CurrentFilePath;
	static LogLevel _LogLevel;
	static bool _isLogToFile;
	static bool _isLogTime;

	static void splog( LogLevel MsgLevel, std::string ObjectName, std::string Message );
	static void logToFile( std::string Message );
	static std::string getPrefix( LogLevel MsgLevel );

public:
	static void init( );
	static void setLogLevel( LogLevel logLevel );
	static void LogToFile( bool is );
	static void LogTime( bool is );
	static Logger & getInstance( std::string Name );

	//OBJECT
private:
	std::string ObjName;

public:
	Logger( std::string ObjectName );

	void log( LogLevel MsgLevel, std::string Message );

};
