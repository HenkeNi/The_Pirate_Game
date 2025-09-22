#pragma once

typedef uint32_t SDL_AudioDeviceID;

namespace cursed_engine
{
	class AudioController
	{
	public:
		void init();

	private:
		SDL_AudioDeviceID m_deviceID;
	};
}