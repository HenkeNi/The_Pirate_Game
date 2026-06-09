#include "engine/platform/window/window.h"
#include "engine/resources/texture/surface.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

namespace cursed_engine
{
	Window::Window()
		: m_window{ nullptr }, m_size{ 0, 0 }, m_isVSyncEnabled{ false }, m_isFullscreen{ false }, m_shouldClose{ false }
	{
	}

	Window::~Window()
	{
		if (m_window)
			destroy();
	}

	bool Window::create(const char* title, const WindowConfig& config)
	{
		if (m_window)
		{
			Logger::logWarning("Valid window already exists, closing down current");
			destroy();
		}

		if (m_window = SDL_CreateWindow(title, config.width, config.height, SDL_WINDOW_RESIZABLE))
		{
			m_size.x = config.width;
			m_size.y = config.height;
		
			return true;
		}
		else
		{
			Logger::logError(std::format("Window creation failed! Reason: {}", SDL_GetError()));
			return false;
		}
	}

	void Window::destroy()
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;

		Logger::logInfo("Window destroyed");
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

	void Window::setAspectRatio(float width, float height)
	{
		SDL_SetWindowAspectRatio(m_window, width, height);
		Logger::logDebub(std::format("Changed aspect ratio: {} x {}", width, height));
	}

	void Window::setAlwaysOnTop(bool enable)
	{
		SDL_SetWindowAlwaysOnTop(m_window, enable);
		Logger::logDebub(std::format("Window always on top changed to: {}", enable));
	}

	void Window::setFullscreen(bool enable)
	{
		m_isFullscreen = enable;
		SDL_SetWindowFullscreen(m_window, enable);

		Logger::logDebub(std::format("Fullscreen: {}", enable ? "enabled" : "disabled"));
	}

	void Window::setPosition(float x, float y)
	{
		SDL_SetWindowPosition(m_window, x, y);
		Logger::logDebub(std::format("Window position: {} {}", x, y));
	}

	void Window::setVSync(bool enable)
	{
		SDL_SetWindowSurfaceVSync(m_window, enable ? 1 : 0);
		Logger::logDebub(std::format("Vsync: {}", enable ? "enabled" : "disabled"));
	}

	void Window::setTitle(const char* title)
	{
		SDL_SetWindowTitle(m_window, title);
		Logger::logDebub(std::format("Window title updated: {}", title));
	}
	
	void Window::setIcon(Surface surface)
	{
		SDL_SetWindowIcon(m_window, surface.surface);
		Logger::logDebub("New window icon set!");
	}

	void Window::handleResize(int width, int height)
	{
		m_size.x = width;
		m_size.y = height;

		// send event?
	}
}