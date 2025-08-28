#pragma once
#include "../subsystems/subsystem.h"

typedef struct SDL_Window SDL_Window;

namespace cursed_engine
{
	class Window : public Subsystem
	{
	public:
		Window();

		void init(); // take in windowConfig
		void shutdown();

		SDL_Window* getWindow(); // rename, or dont use?

	private:
		SDL_Window* m_window;
	};
}