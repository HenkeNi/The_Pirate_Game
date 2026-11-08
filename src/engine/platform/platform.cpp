#include "engine/platform/platform.h"

#include "engine/core/logger.h"
#include "engine/core/result.h"

#include "engine/resources/resource_loaders.h"
#include "engine/resources/texture/surface.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

namespace cursed_engine
{
	Platform::Platform(EventBus& eventBus)
		: m_window{}, m_cursor{}, m_inputHandler{ eventBus }, m_eventBus{ eventBus }, m_shouldExit{ false }
	{
	}

	Platform::~Platform()
	{
		shutdown();
	}

	bool Platform::init(const PlatformArgs& args)
	{
		if (!SDL_SetAppMetadata(args.name, args.version, args.identifier))
		{
			Logger::logError(std::format("{}Failed to set SDL_AppMetadata. Reason: {}", log_format::INDENT, SDL_GetError()));
			return false;
		}

		// SDL initialization
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			Logger::logError(std::format("{}Failed to initialize SDL. Reason: {}", log_format::INDENT, SDL_GetError()));
			return false;
		}

		// TTF initialization
		if (!TTF_Init())
		{
			Logger::logError(std::format("{}Failed to initialize TTF. Reason: {}", log_format::INDENT, SDL_GetError()));
			return false;
		}

		// Window initialization
		Result<void> winResult = m_window.init(args.wndArgs);
		if (!winResult)
		{
			Logger::logError(std::format("{}Failed to initialize Window. Reason: {}", log_format::INDENT, winResult.message()));
			return false;
		}

		SurfaceLoader surfaceLoader;
		Result<Surface> surfaceResult = surfaceLoader(args.iconPath);

		if (surfaceResult)
		{
			m_window.setIcon(surfaceResult.take());
		}
		else
		{
			Logger::logWarning(std::format("{}Failed to set Window icon.", log_format::INDENT));
		}

		// Input initialization
		Result<void> inputResult = m_inputHandler.init(args.inputArgs);
		if (!inputResult)
		{
			Logger::logError(std::format("{}Failed to initialize InputHandler. Reason: {}", log_format::INDENT, winResult.message()));
			return false;
		}

		Logger::logInfo("\'Platform\' successfully initialized!");

		int version = SDL_GetVersion();
		Logger::logInfo(std::format("{} ~ SDL version: {}.{}.{}",
			log_format::INDENT,
			SDL_VERSIONNUM_MAJOR(version),
			SDL_VERSIONNUM_MINOR(version),
			SDL_VERSIONNUM_MICRO(version)));

		const char* videoDriver = SDL_GetCurrentVideoDriver();
		Logger::logInfo(std::format("{} ~ SDL Video driver: {}", log_format::INDENT, videoDriver));

		return true;
	}

	void Platform::shutdown()
	{
		m_window.destroy();
		//m_input.destroy();

		SDL_Quit();
		TTF_Quit();
	}

	void Platform::beginFrame()
	{
		m_inputHandler.beginFrame();
	}

	void Platform::endFrame()
	{
		m_inputHandler.endFrame();
	}

	void Platform::processEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				m_shouldExit = true;
				break;

			default:
				m_window.processEvent(event);
				m_inputHandler.processInput(event);
				break;
			}
		}
	}

	bool Platform::exitRequested() const noexcept
	{
		return m_shouldExit;
	}

	Window& Platform::getWindow() noexcept
	{
		return m_window;
	}

	Cursor& Platform::getCursor() noexcept
	{
		return m_cursor;
	}

	InputHandler& Platform::getInputHandler() noexcept
	{
		return m_inputHandler;
	}
}