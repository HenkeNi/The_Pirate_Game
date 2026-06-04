#include "engine/modules/resource_module.h"
#include "engine/core/logger.h"

// test
#include "engine/resources/texture/texture.h"
#include "engine/resources/audio/audio.h"
#include "engine/resources/text/font.h"

namespace cursed_engine
{
	//ResourceModule::ResourceModule(RenderingServices rendering, const cursed_engine::ResourceConfig& config)
	//	: m_textureManager{ config, TextureLoader{ rendering.renderer } },
	//	m_audioManager{ config, AudioLoader{} },
	//	m_fontManager{ config, FontLoader{} },
	//	m_textManager{ resource.fontManager, rendering.renderer },
	//	m_textFactory{ resource.fontManager, rendering.textRenderer.getTextEngine() }
	//{
	//}

	bool ResourceModule::init(Renderer& renderer, const cursed_engine::ResourceConfig& config)
	{
		m_textureManager.init(&config, std::make_unique<TextureLoader>(&renderer));
		m_audioManager.init(&config, std::make_unique<AudioLoader>());
		m_fontManager.init(&config, std::make_unique<FontLoader>());
		m_textManager.init(&m_fontManager, renderer); // accept font manager in constructor?

		m_textFactory.init(&m_fontManager, renderer);

		Logger::logInfo("-> ResourceModule: Success");
		return true;
	}

	void ResourceModule::shutdown()
	{
		Logger::logInfo("ResourceModule shutdown complete!");
	}

	void ResourceModule::update(uint64_t currentFrame, float deltaTime)
	{
		m_audioManager.update(currentFrame, deltaTime);
		m_fontManager.update(currentFrame, deltaTime);
		m_textureManager.update(currentFrame, deltaTime);
	}

	//ResourceServices ResourceModule::getServices() noexcept
	//{
	//	return {
	//		&m_audioManager,
	//		&m_fontManager,
	//		&m_textureManager,
	//		&m_textManager,
	//		&m_textFactory
	//	};
	//}
}