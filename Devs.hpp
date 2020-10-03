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