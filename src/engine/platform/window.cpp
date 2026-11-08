#include "engine/platform/window.h"

#include "engine/core/result.h"

#include "engine/resources/texture/surface.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include <cassert>

namespace cursed_engine
{
	Window::Window()
		: m_window{ nullptr }, m_state{}
	{
	}

	Window::~Window()
	{
		destroy();
	}

	Result<void> Window::init(const WindowArgs& args)
	{
		assert(!m_window && "Window is already initialized");

		if (m_window = SDL_CreateWindow(args.title.c_str(), args.width, args.height, SDL_WINDOW_RESIZABLE))
		{
			const SDL_WindowFlags flags = SDL_GetWindowFlags(m_window);

			SDL_GetWindowPosition(m_window, &m_state.position.x, &m_state.position.y);
			SDL_GetWindowSize(m_window, &m_state.size.x, &m_state.size.y);

			m_state.alwaysOnTop = (flags & SDL_WINDOW_ALWAYS_ON_TOP) != 0;
			m_state.fullscreen = (flags & SDL_WINDOW_FULLSCREEN) != 0;
			m_state.minimized = (flags & SDL_WINDOW_MINIMIZED) != 0;
			m_state.focused = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;

			return Result<void>::success();
		}
		else
		{
			return Result<void>::failure(SDL_GetError());
		}
	}

	void Window::destroy()
	{
		if (m_window)
		{
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}
	}

	void Window::processEvent(const SDL_Event& event)
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

	void* Window::getNativeHandle() const noexcept
	{
		return m_window;
	}
	
	bool Window::setTitle(const char* title)
	{
		return SDL_SetWindowTitle(m_window, title);
	}

	bool Window::setIcon(const Surface& surface)
	{
		return SDL_SetWindowIcon(m_window, surface.surface);
	}

	bool Window::setAlwaysOnTop(bool enable)
	{
		if (m_state.alwaysOnTop == enable)
			return true;

		if (!SDL_SetWindowAlwaysOnTop(m_window, enable))
			return false;

		return (m_state.alwaysOnTop = (SDL_GetWindowFlags(m_window) & SDL_WINDOW_ALWAYS_ON_TOP) != 0);
	}

	bool Window::setFullscreen(bool enable)
	{
		if (m_state.fullscreen == enable)
			return true;

		if (!SDL_SetWindowFullscreen(m_window, enable))
			return false;

		return (m_state.fullscreen = (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0);
	}

	bool Window::setPosition(int x, int y)
	{
		if (m_state.position.x == x && m_state.position.y == y)
			return true;

		if (!SDL_SetWindowPosition(m_window, x, y))
			return false;

		return SDL_GetWindowPosition(m_window, &m_state.position.x, &m_state.position.y);
	}

	bool Window::setSize(int width, int height)
	{
		if (m_state.size.x == width && m_state.size.y == height)
			return true;

		if (!SDL_SetWindowSize(m_window, width, height))
			return false;

		return SDL_GetWindowSize(m_window, &m_state.size.x, &m_state.size.y);
	}
}