#pragma once

#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#undef ERROR
#endif // _WIN32

enum class LogLevel
{
	NO = 0,
	ERROR = 1,
	WARNING = 2,
	INFO = 3,
	DEBUG = 4
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
	static int getColor( LogLevel MsgLevel );

public:
	static void init( );
	static void setLogLevel( LogLevel logLevel );
	static void LogToFile( bool is );
	static void LogTime( bool is );
	static Logger& getInstance( std::string Name );

	//OBJECT
private:
	std::string ObjName;

public:
	Logger( std::string ObjectName );

	void log( LogLevel MsgLevel, std::string Message );

};
