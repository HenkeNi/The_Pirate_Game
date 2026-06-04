#include "engine/resources/audio/audio.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_audio.h>
#include <filesystem>

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

	Audio AudioLoader::operator()(const AudioDescriptor& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			Logger::logError("Failed to load audio, invalid path: " + key.path);
			return Audio{};
		}

		SDL_AudioSpec spec;
		/*spec.freq = 48000;
		spec.format = SDL_AUDIO_F32;
		spec.channels = 2;*/

		Uint8* audioBuffer = nullptr;
		Uint32 audioLength = 0;

		if (!SDL_LoadWAV(key.path.c_str(), &spec, &audioBuffer, &audioLength))
		{
			Logger::logError("Failed to load audio, invalid path: " + key.path);
			return Audio{};
		}

		// Is this correct??
		auto* stream = SDL_CreateAudioStream(&spec, nullptr);
		if (!stream)
		{
			Logger::logError(std::format("Failed to create audio stream! Error: {}", SDL_GetError()).c_str());
			return Audio{};
		}

		return Audio{ stream, audioBuffer, audioLength };
	}
}