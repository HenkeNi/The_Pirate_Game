#include "engine/window/window.h"
#include "engine/window/window_config.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

namespace cursed_engine
{
	Window::Window()
		: m_window{ nullptr }, m_width{ 0 }, m_height{ 0 }, m_isFullscreen{ false }, m_shouldClose{ false }
	{
	}

	Window::~Window()
	{
		if (m_window)
			shutdown();
	}

	void Window::init(WindowConfig config)
	{
		m_config = std::move(config);

		m_window = SDL_CreateWindow(m_config.title.c_str(), m_config.width, m_config.height, SDL_WINDOW_RESIZABLE);

		if (!m_window)
		{
			//Logger::logError("Failed to create window. Reason: " + SDL_GetError());
		}
		else
		{
			m_width = m_config.width;
			m_height = m_config.height;

			m_shouldClose = false;
		}
	}

	void Window::shutdown()
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	void Window::processEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_WINDOW_RESIZED:
			handleResize(event.window.data1, event.window.data2);
			break;

		case SDL_EVENT_WINDOW_MINIMIZED:
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			// TODO; send event!
			break;

		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			m_shouldClose = true;
			break;
		}
	}

	void Window::setFullscreen(bool enable)
	{
		m_isFullscreen = enable;
		SDL_SetWindowFullscreen(m_window, enable);
	}

	void Window::setTitle(const char* title)
	{
		SDL_SetWindowTitle(m_window, title);
	}
	
	void Window::setIcon(SDL_Surface* surface)
	{
		SDL_SetWindowIcon(m_window, surface);
	}

	void Window::setIcon(const std::filesystem::path& path)
	{
		auto* surface = SDL_LoadBMP(path.string().c_str());
		if (!surface)
		{
			Logger::logError("Failed to load icon: " + path.string());
			return;
		}

		SDL_SetWindowIcon(m_window, surface);
		SDL_DestroySurface(surface);
	}

	void Window::handleResize(int width, int height)
	{
		m_width = width;
		m_height = height;

		// send event?
	}
}