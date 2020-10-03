#pragma once

#include <string>
#include <SFML/Network.hpp>

#include "Logger.h"
#include "VoiceRecorder.h"
#include "SoundPlayer.h"
#include "Devs.hpp"

class Client
{
	Logger* Lgr;
	sf::TcpSocket* Socket;
	sf::Thread* ReceiveThread;
	VoiceRecorder* Recorder;
	SoundPlayer* Player;

	void connect( sf::IpAddress ServerIp, unsigned int ServerPort );
	void disconnect( );
	void receive( );

public:
	Client( int mainArgC, std::string mainArgs );
	~Client( );

	void init( );
	void run( );
};
