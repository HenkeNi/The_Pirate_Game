#include "engine/platform/window/window.h"
#include "engine/resources/texture/surface.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

namespace cursed_engine
{
	Window::Window()
		: m_window{ nullptr }, m_title{ "" }, m_position{ 0, 0 }, m_size{ 0, 0 }, m_isVSyncEnabled{ false }, m_alwaysOnTop{ false }, m_isFullscreen{ false }, m_shouldClose{ false }
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

	bool Window::setAspectRatio(float width, float height)
	{
		bool success = SDL_SetWindowAspectRatio(m_window, width, height);

		if (success)
		{
			m_size.x = width;
			m_size.y = height;

			Logger::logDebug(std::format("Updated window aspect ratio to {}:{}", width, height));
		}
		else
		{
			Logger::logError(std::format("Failed to update window aspect ratio to {}:{}. Error: {}", width, height, SDL_GetError()));
		}

		return success;
	}

	bool Window::setAlwaysOnTop(bool enable)
	{
		bool success = SDL_SetWindowAlwaysOnTop(m_window, enable);

		if (success)
		{
			m_alwaysOnTop = enable;
			Logger::logDebug(std::format("{} always-on-top mode for window", enable ? "Enabled" : "Disabled"));
		}
		else
		{
			Logger::logError(std::format("Failed to {} always-on-top mode for window. Error: {}", enable ? "enable" : "disable", SDL_GetError()));
		}

		return success;
	}

	bool Window::setFullscreen(bool enable)
	{
		bool success = SDL_SetWindowFullscreen(m_window, enable);

		if (success)
		{
			m_isFullscreen = enable;
			Logger::logDebug(std::format("{} fullscreen mode", enable ? "Enabled" : "Disabled"));
		}
		else
		{
			Logger::logError(std::format("Failed to {} fullscreen mode. Error: {}", enable ? "enable" : "disable", SDL_GetError()));
		}

		return success;
	}

	bool Window::setPosition(int x, int y)
	{
		bool success = SDL_SetWindowPosition(m_window, x, y);

		if (success)
		{
			m_position.x = x;
			m_position.y = y;
			Logger::logDebug(std::format("Updated window position to ({}, {})", x, y));
		}
		else
		{
			Logger::logError(std::format("Failed to update window position to ({}, {}). Error: {}", x, y, SDL_GetError()));
		}

		return success;
	}

	bool Window::setVSync(bool enable)
	{
		bool success = SDL_SetWindowSurfaceVSync(m_window, enable ? 1 : 0);

		if (success)
		{
			m_isVSyncEnabled = enable;
			Logger::logDebug(std::format("{} VSync", enable ? "Enabled" : "Disabled"));
		}
		else
		{
			Logger::logError(std::format("Failed to {} VSync. Error: {}", enable ? "enable" : "disable", SDL_GetError()));
		}

		return success;
	}

	bool Window::setTitle(const char* title)
	{
		bool success = SDL_SetWindowTitle(m_window, title);

		if (success)
		{
			m_title = title;
			Logger::logDebug(std::format("Updated window title to \"{}\"", title));
		}
		else
		{
			Logger::logError(std::format("Failed to update window title to \"{}\". Error: {}", title, SDL_GetError()));
		}

		return success;
	}

	bool Window::setIcon(Surface surface)
	{
		bool success = SDL_SetWindowIcon(m_window, surface.surface);

		if (success)
		{
			Logger::logDebug("Updated window icon");
		}
		else
		{
			Logger::logError(std::format("Failed to update window icon. Error: {}", SDL_GetError()));
		}

		return success;
	}

	void Window::handleResize(int width, int height)
	{
		m_size.x = width;
		m_size.y = height;

		// send event?
	}
}