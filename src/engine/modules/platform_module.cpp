#include "engine/modules/platform_module.h"
#include "engine/core/config/config_manager.h"
#include "engine/core/logger.h"
#include "engine/resources/texture/surface_loader.h"
#include "engine/resources/texture/surface.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	PlatformModule::PlatformModule(EventBus& eventBus)
		: m_inputHandler{ eventBus }, m_isRunning{ true }
	{
	}

	bool PlatformModule::init(const ConfigManager& config)
	{
		const auto& appInfo = config.getAppInfo();
		if (!SDL_SetAppMetadata(appInfo.name.c_str(), appInfo.version.c_str(), appInfo.identifier.c_str()))
		{
			Logger::logError(std::format("Failed to set app metadata. Error: {}", SDL_GetError()).c_str());
			return false;
		}

		// SDL initialization
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			Logger::logError(std::format("Failed to initialize SDL! Error: {}", SDL_GetError()).c_str());
			return false;
		}

		// Window creation
		const auto& windowConfig = config.getWindowConfig();

		if (!m_window.create(appInfo.name.c_str(), windowConfig))
		{
			Logger::logError("Failed to create new window!");
			return false;
		}
		else
		{
			SurfaceLoader surfaceLoader;
			Surface surface = surfaceLoader(windowConfig.iconPath);

			m_window.setIcon(surface);
		}

		Logger::logInfo("-> PlatformModule: Success");
		return true;
	}

	void PlatformModule::shutdown()
	{
		m_window.destroy();
		SDL_Quit();
	}

	void PlatformModule::beginFrame()
	{
		m_frameBeginCounter = SDL_GetPerformanceCounter();

		m_inputHandler.beginFrame();

		m_timer.tick();
	}

	void PlatformModule::endFrame()
	{
		// do in beginning of frame? do in class?
		uint64_t end = SDL_GetPerformanceCounter(); // hide in class?
		float elapsed = (end - m_frameBeginCounter) / (float)SDL_GetPerformanceFrequency();
		m_fps = 1.f / elapsed;

		m_inputHandler.endFrame();
	}

	void PlatformModule::pollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				m_isRunning = false;
			}

			m_inputHandler.processInput(event);
			m_window.processEvent(event);
		}
	}

	double PlatformModule::getDeltaTime() const noexcept
	{
		return m_timer.getDeltaTime();
	}

	uint64_t PlatformModule::getFrameCount() const noexcept
	{
		return m_timer.frameCount();
	}

	//PlatformServices PlatformModule::getServices() noexcept
	//{
	//	return { &m_inputHandler, &m_window, &m_timer };
	//}
}