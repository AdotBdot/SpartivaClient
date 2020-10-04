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
	/*	Player->play( );

		while (Player->getStatus() != sf::SoundStream::Stopped)
		{
			sf::sleep(sf::milliseconds(10));
		}*/

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

void Client::receive( )//works in other thread
{
	while( true )
	{
		sf::Packet ReceivedData;
		SMutex.lock( );
		sf::TcpSocket::Status Status = Socket->receive( ReceivedData );
		SMutex.unlock( );
		if( Status == sf::Socket::Done )
		{
			sf::Uint8 type, trash;
			ReceivedData >> type >> trash;

			switch( static_cast< PacketType >( type ) )
			{
				case PacketType::Message:
				{
					std::string message;
					ReceivedData >> message;
					std::cout << message << std::endl;
				}
				case PacketType::VoiceStart:
				{
					const sf::Int16* samples = reinterpret_cast< const sf::Int16* >( static_cast< const char* >( ReceivedData.getData( ) ) + 1 );
					size_t sampleCount = ( ReceivedData.getDataSize( ) - 1 ) / sizeof( sf::Int16 );
					{
						Player->getMutex( )->lock( );
						std::copy( samples, samples + sampleCount, std::back_inserter( *Player->getSamplesPtr( ) ) );
						Player->getMutex( )->unlock( );
					}
				}
				default:
				{
					Lgr->log( LogLevel::ERROR, " Invalid packet! ID: " + type );
				}

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

	while( true )
	{
		std::string txt;
		getline( std::cin, txt );

		sf::Packet packet;
		packet << static_cast< sf::Uint8 >( PacketType::Message ) << static_cast< sf::Uint8 >( PacketReceiver::All ) << txt;

		SMutex.lock( );
		sf::TcpSocket::Status Status = Socket->send( packet );
		SMutex.unlock( );
		if( Status == sf::TcpSocket::Status::Done )
			std::cout << "Packet sent" << std::endl;
	}
}
