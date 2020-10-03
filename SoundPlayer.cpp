#include "SoundPlayer.h"

bool SoundPlayer::onGetData( sf::SoundStream::Chunk& data )
{
	while( ( offset >= samples.size( ) ) )
		sf::sleep( sf::milliseconds( 10 ) );

	{
		sf::Lock lock( m_mutex );
		tempBuffer.assign( samples.begin( ) + offset, samples.end( ) );
		samples.erase( samples.begin( ), samples.begin( ) + offset );
		offset = 0;
	}

	// zapelnianie streamu
	data.samples = &tempBuffer[ 0 ];
	data.sampleCount = tempBuffer.size( );

	// Update offset
	offset += tempBuffer.size( );

	return true;
}

void SoundPlayer::onSeek( sf::Time timeOffset )
{
	offset = timeOffset.asMilliseconds( ) * getSampleRate( ) * getChannelCount( ) / 1000;
}

SoundPlayer::SoundPlayer( )
	:offset( 0 )
{
	initialize( 1, 44100 );
}

std::vector<sf::Int16>* SoundPlayer::getSamplesPtr( )
{
	return &this->samples;
}

sf::Mutex* SoundPlayer::getMutex( )
{
	return &this->m_mutex;
}
