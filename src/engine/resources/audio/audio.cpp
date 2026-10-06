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
		if (m_audio)
		{
			MIX_DestroyAudio(m_audio);
		}
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

	float SDLAudio::getDuration() const
	{
		return MIX_GetAudioDuration(m_audio);
	}
}