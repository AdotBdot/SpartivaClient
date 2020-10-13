#include "VoiceRecorder.h"

#include "Devs.hpp"

VoiceRecorder::VoiceRecorder( sf::TcpSocket* SocketPtr )
	:Socket( SocketPtr )
{
}

VoiceRecorder::~VoiceRecorder( )
{
	stop( );
}

bool VoiceRecorder::onProcessSamples( const sf::Int16* samples, std::size_t sampleCount )
{
	sf::Packet packet;

	packet << static_cast< sf::Int8 > ( PacketType::VoiceStart );

	packet.append( samples, sampleCount * sizeof( sf::Int16 ) );

	return Socket->send( packet ) == sf::Socket::Done;
}

void VoiceRecorder::onStop( )
{
	sf::Packet packet;
	packet << static_cast< sf::Int8 > ( PacketType::VoiceEnd );
	Socket->send( packet );
}
