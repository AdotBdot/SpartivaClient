#pragma once

enum class PacketType
{
	ClientsInfo,
	Message,
	VoiceStart,
	VoiceEnd,
	Whisper,
	Command,
	Disconect
};

enum class PacketRecipient
{
	Server,
	All,
	One,
	Me
};