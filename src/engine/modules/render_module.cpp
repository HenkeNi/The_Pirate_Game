#include "engine/modules/render_module.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/rendering/render_backend.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/logger.h"
#include <cassert>
#include <format>

namespace cursed_engine
{
	RenderModule::RenderModule()
		: m_backend{ nullptr }
	{
	}

	RenderModule::~RenderModule()
	{
	}

	bool RenderModule::init(Window& window, const RenderConfig& config, Backend backend)
	{
		Logger::logInfo(std::format("{}[RenderModule] - Initialization started...", log_format::INDENT));

		switch (backend)
		{
		case Backend::SDL:
			m_backend = std::make_unique<SDLRenderBackend>();
			Logger::logInfo(std::format("{}[RenderModule] - Selected backend: SDL", log_format::INDENT));
			break;

		default:
			Logger::logInfo(std::format("{}[RenderModule] - Unsupported backend {}", log_format::INDENT, (int)backend));
			return false;
		}

		const Result<void> result = m_backend->init(window);

		if (!result.ok())
		{
			Logger::logError(std::format("{}[RenderModule] - Initialization failed! Reason: {}", log_format::INDENT, result.message()));
			return false;
		}

		Logger::logInfo(std::format("{}[RenderModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void RenderModule::shutdown()
	{
		m_backend->shutdown();
		m_backend = nullptr;
	}

	void RenderModule::beginFrame()
	{
		m_backend->beginFrame();
	}

	void RenderModule::endFrame()
	{
		m_backend->endFrame();
	}

	const TextureCreator* RenderModule::getTextureCreator() const noexcept
	{
		assert(m_backend && "Backend not initialized!");
		return m_backend->getTextureCreator();
	}
	
	const TextCreator* RenderModule::getTextCreator() const noexcept
	{
		assert(m_backend && "Backend not initialized!");
		return m_backend->getTextCreator();
	}
}