#include "engine/modules/platform_module.h"
#include "engine/platform/platform.h"

#include "engine/core/settings/engine_config.h"
#include "engine/core/logger.h"
#include "engine/core/result.h"
#include "engine/resources/texture/surface_loader.h"
#include "engine/resources/texture/surface.h"
#include "engine/core/events/event_bus.h"

#include <SDL3/SDL.h>

namespace cursed_engine
{
	PlatformModule::PlatformModule(EventBus& eventBus)
		: m_eventBus{ eventBus }, m_platform{ nullptr }
	{
	}

	PlatformModule::~PlatformModule()
	{
	}

	bool PlatformModule::init(const EngineConfig& config)
	{
		Logger::logInfo(std::format("{}[PlatformModule] - Initialization started...", log_format::INDENT));

		const auto& backend = config.platform.backend;

		switch (backend)
		{
		case Backend::SDL:
			m_platform = std::make_unique<SDLPlatform>(m_eventBus);
			Logger::logInfo(std::format("{}[PlatformModule] - Selected platform: SDL", log_format::INDENT));
			break;

		default:
			Logger::logInfo(std::format("{}[PlatformModule] - Unsupported backend {}", log_format::INDENT, (int)backend));
			return false;
		}

		const Result result = m_platform->init(config);

		if (!result.ok())
		{
			Logger::logInfo(std::format("{}[PlatformModule] - Initialization failed! Reason: {}", log_format::INDENT, result.message()));
			return false;
		}

		Logger::logInfo(std::format("{}[PlatformModule] - Created window \'{}\' ({}x{})", log_format::INDENT, config.appInfo.name, config.window.width, config.window.height));
		Logger::logInfo(std::format("{}[PlatformModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void PlatformModule::shutdown()
	{
		if (m_platform)
		{
			m_platform->shutdown();
			m_platform = nullptr;
		}
	}

	void PlatformModule::beginFrame()
	{
		m_frameBeginCounter = SDL_GetPerformanceCounter();

		m_platform->beginFrame();

		m_timer.tick();

		//assert(m_platform && "Platform uninitialized");
	//	m_platform->pollEvents();
	}

	void PlatformModule::endFrame()
	{
		// TODO; do in FrameTimer?

		// do in beginning of frame? do in class?
		uint64_t end = SDL_GetPerformanceCounter(); // hide in class?
		float elapsed = (end - m_frameBeginCounter) / (float)SDL_GetPerformanceFrequency();
		m_fps = 1.f / elapsed;

		m_platform->endFrame();
	}

	void PlatformModule::processEvents()
	{
		m_platform->processEvents();
	}

	bool PlatformModule::exitRequested() const noexcept
	{
		assert(m_platform && "Platform uninitialized");
		return m_platform->exitRequested();
	}  

	double PlatformModule::getDeltaTime() const noexcept
	{
		return m_timer.getDeltaTime();
	}

	uint64_t PlatformModule::getFrameCount() const noexcept
	{
		return m_timer.frameCount();
	}

	Window& PlatformModule::getWindow() noexcept
	{
		return m_platform->getWindow();
	}

	Cursor& PlatformModule::getCursor() noexcept
	{
		return m_platform->getCursor();
	}

	InputAPI PlatformModule::getInputAPI() noexcept 
	{ 
		return &m_platform->getInput(); 
	}
}