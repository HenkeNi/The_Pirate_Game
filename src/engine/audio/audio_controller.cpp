#include "engine/audio/audio_controller.h"
#include <SDL3/SDL_audio.h>

namespace cursed_engine
{
	void AudioController::init()
	{
		SDL_AudioSpec spec;
		SDL_zero(spec);
		spec.freq = 48000;
		spec.format = SDL_AUDIO_F32;
		spec.channels = 2;

		m_deviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

		//SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(m_deviceID, &spec, nullptr);
		//SDL_ResumeAudioDevice(m_deviceID); // start playback
	}
}