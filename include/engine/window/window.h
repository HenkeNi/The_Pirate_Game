#pragma once
#include "engine/window/window_config.h"

typedef struct SDL_Window SDL_Window;
struct SDL_Surface;

namespace cursed_engine
{
	struct WindowConfig;

	class Window
	{
	public:
		Window();

		void init(const WindowConfig& config);
		void shutdown();

		// "update" function that accepts config, or is that init?

		//void applyConfig();

		SDL_Surface* getSurface(); // Remove? just use returned window?

		[[nodiscard]] inline SDL_Window* getWindow() { return m_window; } // rename, or dont use?

		void setTitle(const char* title);
		void setIcon(SDL_Surface* surface);

	private:
		//WindowConfig m_config;
		SDL_Window* m_window;
	};
}