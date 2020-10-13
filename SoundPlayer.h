#pragma once

#include <vector>
#include <SFML/Audio.hpp>

class SoundPlayer
	:public sf::SoundStream
{
	size_t offset;
	sf::Mutex m_mutex;
	std::vector<sf::Int16> samples;
	std::vector<sf::Int16> tempBuffer;

	virtual bool onGetData( sf::SoundStream::Chunk& data );
	virtual void onSeek( sf::Time timeOffset );

public:
	SoundPlayer( );

	void copy( const sf::Int16* _Samples, size_t _SampleCount );
};
