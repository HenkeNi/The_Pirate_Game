#pragma once
#include "engine/core/subsystem.h"

typedef uint32_t SDL_AudioDeviceID;
struct SDL_AudioStream;

namespace cursed_engine
{
	class AudioController : public Subsystem
	{
	public:
		AudioController();

		bool init();

		void playSound(SDL_AudioStream* stream, uint8_t * buffer, uint32_t length); // or accept audio?
		//SDL_AudioSpec getSpecs();

	private:
		SDL_AudioDeviceID m_deviceID;
		SDL_AudioStream* m_audioStream;
	};
}