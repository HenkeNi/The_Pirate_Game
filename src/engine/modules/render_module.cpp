#include "engine/modules/render_module.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/core/logger.h"
#include <format>

namespace cursed_engine
{
	//RenderModule::RenderModule()
	//	: m_renderAPI{ m_renderer }
	//{
	//}

	bool RenderModule::init(Window& window)
	{
		if (!m_renderer.init(window))
		{
			Logger::logError("Error occured while trying to initialize Renderer!");
			return false;
		}

		//m_capabilities.createTexture = [](Renderer& renderer, Surface surface)
		//	{
		//		return Texture{nullptr};
		//	};

		Logger::logInfo("-> RenderModule: Success");
		return true;
	}

	void RenderModule::shutdown()
	{
		m_renderer.shutdown();
	}

	void RenderModule::beginFrame()
	{
		m_renderer.beginFrame();
	}
	
	void RenderModule::endFrame()
	{
		m_renderer.endFrame();
	}

	/*RenderCapabilities RenderModule::getCapabilities() const noexcept
	{
		return RenderCapabilities();
	}*/
}