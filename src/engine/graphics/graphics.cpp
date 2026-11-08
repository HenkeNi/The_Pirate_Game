#include "engine/graphics/graphics.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/core/logger.h"
#include <cassert>
#include <format>

namespace cursed_engine
{
	Graphics::Graphics()
		: m_backend{}
	{
	}

	Graphics::~Graphics()
	{
	}

	bool Graphics::init(Window& window, const GraphicsArgs& args)
	{
		const Result<void> result = m_backend.init(window);

		if (!result)
		{
			Logger::logError(std::format("{}Failed to initialize Renderer! Reason: {}", log_format::INDENT, result.message()));
			return false;
		}

		if (!m_backend.setVSync(args.vsync))
		{
			Logger::logWarning(std::format("Failed to set VSync! Reason: {}", result.message()));
		}

		Logger::logInfo(std::format("\'Graphics\' successfully initialized!", log_format::INDENT));
		return true;
	}

	void Graphics::shutdown()
	{
		m_backend.shutdown();
	}

	void Graphics::beginFrame()
	{
		m_backend.beginFrame();
	}

	void Graphics::endFrame()
	{
		m_backend.endFrame();
	}

	const TextureCreator* Graphics::getTextureCreator() const noexcept
	{
		return m_backend.getTextureCreator();
	}
	
	const TextCreator* Graphics::getTextCreator() const noexcept
	{
		return m_backend.getTextCreator();
	}
}