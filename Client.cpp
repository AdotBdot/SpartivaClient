#include "Client.h"

#include <iostream>

#include "Devs.hpp"

void Client::receive( )
{
	for( ;; )
	{
		sf::Packet ReceivedData;
		if( Socket->receive( ReceivedData ) )
		{
			std::cout << "Received: " << ReceivedData.getDataSize( ) << " bytes" << std::endl;
		}
	}
}

void Client::connect( sf::IpAddress ServerIp, unsigned short ServerPort )
{
	if( Socket->connect( ServerIp, ServerPort ) == sf::Socket::Done )
	{
		Lgr->log( LogLevel::INFO, "Connected with" + ServerIp.toString( ) + ":" + std::to_string( ServerPort ) );
	}
	else
	{
		Lgr->log( LogLevel::INFO, "Can't connect with: " + ServerIp.toString( ) + ":" + std::to_string( ServerPort ) );
	}
}

// TODO: add sending disconnection packet
void Client::disconnect( )
{
	Socket->disconnect( );
	Lgr->log( LogLevel::INFO, "Disconnected" );
}

Client::Client( int mainArgC, std::string mainArgs )
{
	init( );
}

Client::~Client( )
{
	if( Socket != nullptr )
		delete Socket;
	if( Lgr != nullptr )
		delete Lgr;
}

void Client::init( )
{
	//Logger init

	Lgr = new Logger( "MainThread" );
	Lgr->log( LogLevel::INFO, "Initializiation..." );

	//Client Socket init
	Socket = new sf::TcpSocket( );

	ReceiveThread = new sf::Thread( &Client::receive, this );

	Lgr->log( LogLevel::INFO, "Initialized" );
}

void Client::run( )
{
	sf::IpAddress ServerIp;
	std::cout << "Enter Server IP: ";
	std::cin >> ServerIp;

	short ServerPort;
	std::cout << "Enter Server port: ";
	std::cin >> ServerPort;

	connect( ServerIp, ServerPort );

	ReceiveThread->launch( );

	for( ;; )
	{
		std::string txt;
		std::cin >> txt;

		sf::Packet packet;
		packet << static_cast< UINT8 >( PacketType::Message ) << txt;

		Socket->send( packet );
	}
}
