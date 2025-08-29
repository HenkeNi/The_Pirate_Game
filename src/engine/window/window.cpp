#include "engine/window/window.h"
#include "engine/window/window_config.h"
#include <SDL3/SDL_video.h>
#include <iostream>

namespace cursed_engine
{
	Window::Window()
		: m_window{ nullptr }
	{
	}

	void Window::init(const WindowConfig& config)
	{
		m_window = SDL_CreateWindow(config.title.c_str(), config.width, config.height, SDL_WINDOW_RESIZABLE);

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