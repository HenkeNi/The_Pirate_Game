#include "engine/platform/window.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/result.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

namespace cursed_engine
{
#pragma region Window

	Result Window::create(const WindowConfig& config)
	{
		Result result = onCreate(config);

		if (result.succeeded)
		{
			m_preferences.title = config.title;
			m_preferences.position = { config.xPos, config.yPos };
			m_preferences.size = { config.width, config.height };
			m_preferences.fullscreen = config.fullscreen;
			m_preferences.vsync = config.vsync;
			m_preferences.alwaysOnTop = config.alwaysOnTop;

			setPreferences(std::move(m_preferences));
		}

		return result;
	}

	void Window::destroy()
	{
		onDestroy();
	}

	void Window::setPreferences(Preferences preferences)
	{
		m_preferences = std::move(preferences);

		setTitle(m_preferences.title.c_str());

		setPosition(m_preferences.position.x, m_preferences.position.y);

		setSize(m_preferences.size.x, m_preferences.size.y);

		setFullscreen(m_preferences.fullscreen);

		setVSync(m_preferences.vsync);

		setAlwaysOnTop(m_preferences.alwaysOnTop);
	}

	bool Window::setTitle(const char* title)
	{
		if (m_preferences.title == title)
			return true;

		if (!onSetTitle(title))
			return false;

		m_preferences.title = title;		
		return true;
	}

	bool Window::setAlwaysOnTop(bool enable)
	{
		if (m_preferences.alwaysOnTop = enable)
			return true;

		if (!onSetAlwaysOnTop(enable))
			return false;

		m_preferences.alwaysOnTop = enable;
		return true;
	}

	bool Window::setFullscreen(bool enable)
	{
		if (m_preferences.fullscreen == m_preferences.fullscreen)
			return true;

		if (!onSetFullscreen(enable))
			return false;

		m_preferences.fullscreen = enable;
		return true;
	}

	bool Window::setPosition(int x, int y)
	{
		if (m_preferences.position.x == x && m_preferences.position.y == y)
			return true;

		if (!onSetPosition(x, y))
			return false;

		m_preferences.position = { x, y };
		return true;
	}

	bool Window::setSize(int width, int height)
	{
		if (m_preferences.size.x == width && m_preferences.size.y == height)
			return true;

		if (!onSetSize(width, height))
			return false;

		m_preferences.size = { width, height };
		return true;
	}

	bool Window::setIcon(const Icon& icon)
	{
		return onSetIcon(icon);
	}

	bool Window::setVSync(bool enable)
	{
		if (m_preferences.vsync == enable)
			return true;

		if (!onSetVSync(enable))
			return false;

		m_preferences.vsync = enable;
		return true;
	}

#pragma endregion

#pragma region SDLWindow

	SDLWindow::SDLWindow()
		: m_window{ nullptr }
	{
	}

	SDLWindow::SDLWindow(SDLWindow&& other) noexcept
		: m_window{ other.m_window }
	{
		other.m_window = nullptr;
	}

	SDLWindow& SDLWindow::operator=(SDLWindow&& other) noexcept
	{
		m_window = other.m_window;
		other.m_window = nullptr;

		return *this;
	}

	void SDLWindow::processEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_WINDOW_RESIZED:
			setSize(event.window.data1, event.window.data2);
			break;

		case SDL_EVENT_WINDOW_MINIMIZED:
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			// TODO!
			break;
		}
	}

	void* SDLWindow::getNativeHandle() const noexcept
	{
		return m_window;
	}

	Result SDLWindow::onCreate(const WindowConfig& config)
	{
		if (m_window)
		{
			onDestroy();
		}

		m_window = SDL_CreateWindow(config.title.c_str(), config.width, config.height, SDL_WINDOW_RESIZABLE);

		return m_window ? Result::success() : Result::failure(SDL_GetError());
	}

	void SDLWindow::onDestroy()
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	bool SDLWindow::onSetTitle(const char* title)
	{
		return SDL_SetWindowTitle(m_window, title);
	}

	bool SDLWindow::onSetAlwaysOnTop(bool enable)
	{
		return SDL_SetWindowAlwaysOnTop(m_window, enable);
	}

	bool SDLWindow::onSetFullscreen(bool enable)
	{
		return SDL_SetWindowFullscreen(m_window, enable);
	}

	bool SDLWindow::onSetPosition(int x, int y)
	{
		return SDL_SetWindowPosition(m_window, x, y);
	}

	bool SDLWindow::onSetSize(int width, int height)
	{
		return SDL_SetWindowSize(m_window, width, height);
	}

	bool SDLWindow::onSetVSync(bool enable)
	{
		return SDL_SetWindowSurfaceVSync(m_window, enable ? 1 : 0);
	}

	bool SDLWindow::onSetIcon(const Icon& icon)
	{
		if (const Surface* surface = std::get_if<Surface>(&icon))
		{
			return SDL_SetWindowIcon(m_window, surface->surface);
		}

		return false;
	}

#pragma endregion
}