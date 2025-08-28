#include "engine/window/window.h"
#include <SDL3/SDL_video.h>
#include <iostream>

namespace cursed_engine
{
	Window::Window()
	{
	}

	void Window::init()
	{
		m_window = SDL_CreateWindow("The Cursed Pirate", 1200, 800, SDL_WINDOW_RESIZABLE);

		if (!m_window)
		{
			const char* error = SDL_GetError();
			std::cerr << error << "\n";

			// SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		}
	}

	void Window::shutdown()
	{
		SDL_DestroyWindow(m_window);
	}

	SDL_Window* Window::getWindow()
	{
		return m_window;
	}
}