#pragma once

enum class PacketType
{
	ClientsInfo,
	Message,
	VoiceStart,
	VoiceEnd,
	Whisper,
	Command,
	Disconnect
};

enum class PacketReceiver
{
	Server,
	All,
	One,
	Me
};