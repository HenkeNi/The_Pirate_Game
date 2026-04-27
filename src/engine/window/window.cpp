#include "engine/window/window.h"
//#include "engine/config/config_types.h" or use this?!
#include "engine/config/config_manager.h"
#include "engine/core/logger.h"
#include "engine/resources/surface.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

namespace cursed_engine
{
	Window::Window()
		: m_window{ nullptr }, m_size{ 0, 0 }, m_isFullscreen{ false }, m_shouldClose{ false }
	{
	}

	Window::~Window()
	{
		if (m_window)
			shutdown();
	}

	void Window::create(const char* title, const WindowConfig& config)
	{
		if (m_window)
		{
			Logger::logWarning("Valid window already exists, closing down current");
			shutdown();
		}

		if (m_window = SDL_CreateWindow(title, config.width, config.height, SDL_WINDOW_RESIZABLE))
		{
			m_size.x = config.width;
			m_size.y = config.height;
		}
		else
		{
			Logger::logError(std::format("Failed to create window. Reason: {}", SDL_GetError()));
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
	
	void Window::setIcon(Surface surface)
	{
		SDL_SetWindowIcon(m_window, surface.surface);
	}

	void Window::handleResize(int width, int height)
	{
		m_size.x = width;
		m_size.y = height;

		// send event?
	}
}