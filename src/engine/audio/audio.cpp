#include "engine/audio/audio.h"
#include <SDL3/SDL_audio.h>

namespace cursed_engine
{
	Audio::Audio()
		: m_stream{ nullptr }, m_buffer{ nullptr }, m_length{ 0 }
	{
	}

	Audio::Audio(SDL_AudioStream* stream, uint8_t* buffer, uint32_t length)
		: m_stream{ stream }, m_buffer{ buffer }, m_length{ length }
	{
	}

	Audio::~Audio()
	{
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
}