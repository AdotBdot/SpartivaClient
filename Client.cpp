#include "Client.h"

#include <iostream>

#include "Devs.hpp"

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
	if( Recorder != nullptr )
		delete Recorder;
	if( Player != nullptr )
		delete Player;
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

	//Voice Recorder
	if( sf::SoundRecorder::isAvailable( ) )
		Recorder = new VoiceRecorder( Socket );
	else
		Lgr->log( LogLevel::ERROR, "Audio capture is not avaible on your system!" );

	Player = new SoundPlayer( );
	Player->setVolume( 100 );

	Lgr->log( LogLevel::INFO, "Initialized" );
}

void Client::connect( sf::IpAddress ServerIp, unsigned int ServerPort )
{
	auto Status = Socket->connect( ServerIp, ServerPort );
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

void Client::receive( )
{
	for( ;; )
	{
		sf::Packet ReceivedData;
		if( Socket->receive( ReceivedData ) == sf::Socket::Done )
		{
			std::cout << "Received: " << ReceivedData.getDataSize( ) << " bytes" << std::endl;
			UINT8 type;
			ReceivedData >> type;

			switch( ( PacketType ) type )
			{
				case PacketType::Message:
				{
					std::string message;
					ReceivedData >> message;
					std::cout << message;

				}
				case PacketType::VoiceStart:
				{
					const sf::Int16* samples = reinterpret_cast< const sf::Int16* >( ( const char* ) ReceivedData.getData( ) + 1 );
					std::size_t sampleCount = ( ReceivedData.getDataSize( ) - 1 ) / sizeof( sf::Int16 );
					{
						sf::Lock lock( *Player->getMutex( ) );
						std::copy( samples, samples + sampleCount, std::back_inserter( *Player->getSamplesPtr( ) ) );
					}
				}
				default:
					Lgr->log( LogLevel::ERROR, "Invalid packet! ID: " + ( int ) type );
					break;
			}
			ReceivedData.clear( );
		}
	}
}

void Client::run( )
{
	sf::IpAddress ServerIp;
	std::cout << "Enter Server IP: ";
	std::cin >> ServerIp;

	unsigned int ServerPort;
	std::cout << "Enter Server port: ";
	std::cin >> ServerPort;

	connect( ServerIp, ServerPort );

	ReceiveThread->launch( );

	//Recorder->start( );

	for( ;; )
	{
		std::string txt;
		getline( std::cin, txt );

		sf::Packet packet;
		packet << ( INT8 ) PacketType::Message << ( INT8 ) PacketReceiver::All << txt;

		sf::TcpSocket::Status Status = Socket->send( packet );
		if( Status == sf::TcpSocket::Status::Done )
			std::cout << "Packet sent" << std::endl;
	}
}
