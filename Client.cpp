#include "Client.h"

#include <iostream>
#include <string>
#include <vector>

#include "Devs.hpp"

Client::Client( int mainArgC, char** mainArgv )
{
	std::vector<std::string> argList;
	if( mainArgC > 1 )
	{
		for( int i = 0; i < mainArgC; i++ )
			argList.push_back( mainArgv[ i ] );
	}
	for( int i = 0; i < argList.size( ); i++ )
		std::cout << argList[ i ] << std::endl;
	init( argList );
}

Client::~Client( )
{
	if( Socket != nullptr )
		delete Socket;
	if( ReceiveThread != nullptr )
		delete ReceiveThread;
	if( Recorder != nullptr )
		delete Recorder;
	if( Player != nullptr )
		delete Player;
	if( Lgr != nullptr )
		delete Lgr;
}

void Client::init( std::vector<std::string> CMDArguments )
{
	//Logger init
	Lgr = new Logger( "MainThread" );
	Lgr->log( LogLevel::DEBUG, "Initializiation..." );

	//Client Socket init
	Socket = new sf::TcpSocket( );
	ReceiveThread = new sf::Thread( &Client::receive, this );

	//init Voice and Sound
	Recorder = new VoiceRecorder( Socket );
	Player = new SoundPlayer( );

	Player->setVolume( 100 );

	Lgr->log( LogLevel::DEBUG, "Initialized!" );
}

void Client::connect( sf::IpAddress ServerIp, unsigned int ServerPort )
{
	sf::Socket::Status Status = Socket->connect( ServerIp, ServerPort );
	if( Status == sf::Socket::Done )
	{
		Lgr->log( LogLevel::INFO, "Connected with: " + ServerIp.toString( ) + ":" + std::to_string( ServerPort ) );
	}
	else
	{
		Lgr->log( LogLevel::ERROR, "Can't connect with: " + ServerIp.toString( ) + ":" + std::to_string( ServerPort ) );
	}
}

//works in other thread
void Client::receive( )
{
	while( true )
	{
		sf::Packet ReceivedData;
		sf::Socket::Status Status = Socket->receive( ReceivedData );
		switch( Status )
		{
			case sf::Socket::Done:
			{
				sf::Int8 PType, trash;
				ReceivedData >> PType >>trash;
				switch( static_cast< PacketType >( PType ) )
				{
					case PacketType::Message:
					{
						std::string text;
						ReceivedData >> text;
						std::cout << "Message: " << text << std::endl;
					}
					break;
					case PacketType::VoiceStart:
					{
						const sf::Int16* samples = reinterpret_cast< const sf::Int16* >( static_cast< const char* >( ReceivedData.getData( ) ) + 1 );
						size_t sampleCount = ( ReceivedData.getDataSize( ) - 1 ) / sizeof( sf::Int16 );
						Player->copy( samples, sampleCount );
					}
					break;
					case PacketType::VoiceEnd:
					{

					}
					break;
				}
			}
			break;
			case sf::Socket::Error:
			{
				Lgr->log( LogLevel::ERROR, "Error" );
			}
			break;
		}
	}
}

void Client::run( )
{
	sf::IpAddress ServerIp;
	std::cout << "Enter ServerIp: ";
	std::cin >> ServerIp;

	connect( ServerIp, 50001 );

	ReceiveThread->launch( );

	//Recorder->start( );
	//Player->play( );

	while( true )
	{
		char input[ 150 ];
		//std::cin.getline( input, 150 );
		std::cin >> input;

		sf::Packet packet;
		packet << static_cast< sf::Int8 >( PacketType::Message ) << /*static_cast< sf::Int8 >( PacketRecipient::All )<<*/ input;

		sf::Socket::Status Status = Socket->send( packet );
		if( Status == sf::Socket::Status::Done )
			Lgr->log( LogLevel::DEBUG, "Packet sent" );
	}
}
