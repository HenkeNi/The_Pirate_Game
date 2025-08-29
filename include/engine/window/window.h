#pragma once
#include "../subsystems/subsystem.h"
#include "engine/window/window_config.h"

typedef struct SDL_Window SDL_Window;

namespace cursed_engine
{
	struct WindowConfig;

	class Window : public Subsystem
	{
	public:
		Window();

		void init(const WindowConfig& config);
		void shutdown();
		
		//void applyConfig();

		SDL_Window* getWindow(); // rename, or dont use?

	private:
		//WindowConfig m_config;
		SDL_Window* m_window;
	};
}