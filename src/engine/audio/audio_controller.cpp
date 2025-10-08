#include "engine/audio/audio_controller.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_audio.h>
#include <format>

namespace cursed_engine
{
	AudioController::AudioController()
	{
	}

	bool AudioController::init()
	{
		static SDL_AudioDeviceID audio_device = 0;

		// TODO; use same spec for audio controller, and audio's?
		SDL_AudioSpec spec;
		SDL_zero(spec);
		spec.freq = 48000;
		spec.format = SDL_AUDIO_S16LE;
		spec.channels = 2;

		//m_deviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

		m_audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

		if (!m_audioStream)
		{
			Logger::logError(std::format("Failed to create audio stream! Error: {}", SDL_GetError()).c_str());
			return false;
		}

		m_deviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

		SDL_ResumeAudioStreamDevice(m_audioStream);
	}

	void AudioController::playSound(SDL_AudioStream* stream, uint8_t* buffer, uint32_t length)
	{
		static bool test = false;

		if (!test)
		{

			// DO in init sound?
			if (!SDL_BindAudioStream(m_deviceID, stream))
			{
				Logger::logError(std::format("Failed to bind stream! Error: {}", SDL_GetError()).c_str());

			}

			test = true;
		}




		// THIS Checks if sound can be queued??? -> have own queue if fails?
		if (SDL_GetAudioStreamQueued(m_audioStream) < (int)length) {
			/* feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
			SDL_PutAudioStreamData(m_audioStream, buffer, length);
		}


		//if (!SDL_PutAudioStreamData(m_audioStream, buffer, length))
		//{
		//	Logger::logWarning("Failed to put audio stream"); // TOOD; log error to
		//}


		//SDL_BindAudioStream(m_deviceID, m_audioStream);
		//SDL_ResumeAudioDevice(m_deviceID);
	}

	/*SDL_AudioSpec AudioController::getSpecs()
	{
		return SDL_AudioSpec();
	}*/
}