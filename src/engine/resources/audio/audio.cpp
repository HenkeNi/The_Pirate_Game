#include "engine/resources/audio/audio.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_audio.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <filesystem>

namespace cursed_engine
{
	SDLAudio::SDLAudio(MIX_Audio* audio)
		: m_audio{ audio }
	{ 
	}

	SDLAudio::~SDLAudio()
	{
	}

	SDLAudio::SDLAudio(SDLAudio&& other) noexcept
		: m_audio{ other.m_audio }
	{
		other.m_audio = nullptr;
	}

	SDLAudio& SDLAudio::operator=(SDLAudio&& other) noexcept
	{
		m_audio = other.m_audio;
		other.m_audio = nullptr;
	
		return *this;
	}

	//Audio::Audio(const SDL_AudioSpec& spec, uint8_t* buffer, uint32_t length)
	//	: m_stream{ spec }, m_buffer{ buffer }, m_length{ length }
	//{
	//}

	//void Audio::init(const SDL_AudioSpec& spec, uint8_t* buffer, uint32_t length)
	//{
	//	m_spec = spec;
	//	m_buffer = buffer;
	//	m_length = length;
	//}

	/*SDLAudio::SDLAudio()
		: m_stream{ nullptr }, m_buffer{ nullptr }, m_length{ 0 }
	{
	}

	SDLAudio::SDLAudio(SDL_AudioStream* stream, uint8_t* buffer, uint32_t length)
		: m_stream{ stream }, m_buffer{ buffer }, m_length{ length }
	{
	}*/

}