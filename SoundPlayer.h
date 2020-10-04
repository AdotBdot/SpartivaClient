#pragma once

#include<vector>
#include <basetsd.h>
#include <SFML/Audio.hpp>

class SoundPlayer
	:public sf::SoundStream
{
	size_t offset;
	sf::Mutex m_mutex;
	std::vector<INT16> samples;
	std::vector<INT16> tempBuffer;

	virtual bool onGetData( sf::SoundStream::Chunk& data );
	virtual void onSeek( sf::Time timeOffset );

public:
	SoundPlayer( );

	std::vector<sf::Int16>* getSamplesPtr( );
	sf::Mutex* getMutex( );
};
