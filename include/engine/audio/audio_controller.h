#pragma once

typedef uint32_t SDL_AudioDeviceID;
struct SDL_AudioStream;

namespace cursed_engine
{
	template <typename T>
	class Result;

	class AudioController
	{
	public:
		AudioController();

		Result<void> init();
		void shutdown();

		void playSound(SDL_AudioStream* stream, uint8_t * buffer, uint32_t length); // or accept audio?
		//SDL_AudioSpec getSpecs();

	private:
		SDL_AudioDeviceID m_deviceId;
		SDL_AudioStream* m_audioStream;
	};
}