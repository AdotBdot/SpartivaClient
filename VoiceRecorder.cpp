#include "VoiceRecorder.h"

#include <iostream>

#include "Devs.hpp"

VoiceRecorder::VoiceRecorder( sf::TcpSocket* SocketPtr )
	:Socket( SocketPtr )
{

}

VoiceRecorder::~VoiceRecorder( )
{
	stop( );
}

bool VoiceRecorder::onProcessSamples( const INT16* samples, std::size_t sampleCount )
{
	sf::Packet packet;

	packet << ( INT8 ) PacketType::VoiceStart;

	packet.append( samples, sampleCount * sizeof( INT16 ) );

	std::cout << "sent" << std::endl;
	return Socket->send( packet ) == sf::Socket::Done;
}

void VoiceRecorder::onStop( )
{
	sf::Packet packet;
	packet << ( INT8 ) PacketType::VoiceEnd;
	Socket->send( packet );
	std::cout << "sent"<<std::endl;
}
