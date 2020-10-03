#pragma once

#include <string>
#include <SFML/Network.hpp>

#include "Logger.h"
#include "Devs.hpp"

class Client
{
	Logger* Lgr;
	sf::TcpSocket* Socket;
	sf::Thread* ReceiveThread;

	void connect( sf::IpAddress ServerIp, int ServerPort );
	void disconnect( );
	void receive( );

public:
	Client( int mainArgC, std::string mainArgs );
	~Client( );

	void init( );
	void run( );
};
