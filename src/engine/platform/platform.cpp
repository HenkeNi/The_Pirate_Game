#include "engine/platform/platform.h"
#include "engine/core/settings/engine_config.h"
#include "engine/resources/texture/surface_loader.h"
#include "engine/resources/texture/surface.h"
#include "engine/platform/window.h"
#include "engine/core/result.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

namespace cursed_engine
{
#pragma region SDL_Platform

	SDLPlatform::SDLPlatform(EventBus& eventBus)
		: m_eventBus{ eventBus }, m_input{ eventBus }, m_initialized{ false }, m_shouldExit{ false }
	{
	}

	SDLPlatform::~SDLPlatform()
	{
		shutdown();
	}

	Result SDLPlatform::init(const EngineConfig& config)
	{
		const auto& appInfo = config.appInfo;

		if (!SDL_SetAppMetadata(appInfo.name.c_str(), appInfo.version.c_str(), appInfo.identifier.c_str()))
		{
			return Result::failure(std::format("Failed to set SDL_AppMetadata. Reason: {}", SDL_GetError()));
		}

		// SDL initialization
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			return Result::failure(std::format("Failed to initialize SDL. Reason: {}", SDL_GetError()));
		}

		// TTF initialization
		if (!TTF_Init())
		{
			return Result::failure(std::format("Failed to initialize TTF. Reason: {}", SDL_GetError()));
		}

		// Window creation
		Result result = m_window.create(config.window);

		if (!result.succeeded)
		{
			return result;
		}

		SurfaceLoader surfaceLoader;
		Surface surface = surfaceLoader(config.window.iconPath);

		m_window.setIcon(surface);

		// Input 
		return m_input.init(config.input);		
	}

	void SDLPlatform::shutdown()
	{
		if (!m_initialized)
			return;

		m_window.destroy();
		//m_input.destroy();

		SDL_Quit();
		TTF_Quit();

		m_initialized = false;
	}

	void SDLPlatform::beginFrame()
	{
		m_input.beginFrame();
	}

	void SDLPlatform::endFrame()
	{
		m_input.endFrame();
	}

	void SDLPlatform::processEvents()
	{
		pollEvents();
	}

	void SDLPlatform::pollEvents()
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
				m_input.processInput(event); // TODO- name both processEvents?
				break;
			}
		}
	}

	bool SDLPlatform::exitRequested() const noexcept
	{
		return m_shouldExit;
	}

	Window& SDLPlatform::getWindow() noexcept
	{
		return m_window;
	}

	Cursor& SDLPlatform::getCursor() noexcept
	{
		return m_cursor;
	}

	Input& SDLPlatform::getInput() noexcept
	{
		return m_input;
	}

#pragma endregion
}