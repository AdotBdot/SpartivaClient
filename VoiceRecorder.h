#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class VoiceRecorder
    :public sf::SoundRecorder
{
    sf::TcpSocket* Socket;

    virtual bool onProcessSamples( const sf::Int16* samples, std::size_t sampleCount );
    virtual void onStop( );

public:
    VoiceRecorder( sf::TcpSocket* SocketPtr );
    ~VoiceRecorder( );
};

