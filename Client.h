#pragma once

#include <string>

#include "Logger.h"

class Client
{
	Logger* Lgr;
public:
	Client( int mainArgC, std::string mainArgs );
	~Client( );

	void init( );

	void run( );

};
