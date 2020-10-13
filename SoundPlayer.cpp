#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
	:offset( 0 )
{
	initialize( 1, 44100 );
}

void SoundPlayer::copy( const sf::Int16* _Samples, size_t _SampleCount )
{
	m_mutex.lock();
	std::copy( _Samples, _Samples + _SampleCount, std::back_inserter( samples ) );
	m_mutex.unlock();
}

bool SoundPlayer::onGetData( sf::SoundStream::Chunk& data )
{
	while( offset >= samples.size() )
		sf::sleep( sf::milliseconds( 10 ) );

	m_mutex.lock();
	tempBuffer.assign( samples.begin() + offset, samples.end() );
	samples.erase( samples.begin(), samples.begin() + offset );
	offset = 0;
	m_mutex.unlock();

	// zapelnianie streamu
	data.samples = &tempBuffer[ 0 ];
	data.sampleCount = tempBuffer.size();

	// Update offset
	offset += tempBuffer.size();

	return true;
}

void SoundPlayer::onSeek( sf::Time timeOffset )
{
	offset = timeOffset.asMilliseconds() * getSampleRate() * getChannelCount() / 1000;
}
