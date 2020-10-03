#include "Client.h"

#include <iostream>

#include "Devs.hpp"

void Client::receive( )
{
	for( ;; )
	{
		sf::Packet ReceivedData;
		if( Socket->receive( ReceivedData ) == sf::Socket::Done )
		{
			std::cout << "Received: " << ReceivedData.getDataSize( ) << " bytes" << std::endl;
		}
	}
}

void Client::connect( sf::IpAddress ServerIp, int ServerPort )
{
	sf::TcpSocket::Status Status = Socket->connect( ServerIp, ServerPort );
	if( Status == sf::Socket::Done )
	{
		Lgr->log( LogLevel::INFO, "Connected with: " + ServerIp.toString( ) + ":" + std::to_string( ServerPort ) );
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
	if( ReceiveThread != nullptr )
		delete ReceiveThread;
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

	int ServerPort;
	std::cout << "Enter Server port: ";
	std::cin >> ServerPort;

	connect( ServerIp, ServerPort );

	ReceiveThread->launch( );

	for( ;; )
	{
		std::string txt;
		std::cin >> txt;

		sf::Packet packet;
		packet << ( UINT8 ) PacketType::Message << ( UINT8 ) PacketReceiver::All << txt;

		Socket->send( packet );
	}
}
