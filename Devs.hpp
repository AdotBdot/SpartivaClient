#pragma once

enum class PacketType
{
	Message,
	VoiceStart,
	VoiceEnd
};

enum class PacketRecipient
{
	Server,
	All,
	One,
	Me
};