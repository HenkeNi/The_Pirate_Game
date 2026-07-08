#include "engine/modules/resource_module.h"
#include "engine/core/logger.h"

// test
#include "engine/resources/texture/texture.h"
#include "engine/resources/audio/audio.h"
#include "engine/resources/text/font.h"

namespace cursed_engine
{
	bool ResourceModule::init(ResourceCreator* creator, const cursed_engine::ResourceConfig& config)
	{
		Logger::logInfo(std::format("{}[ResourceModule] - Initialization started...", log_format::INDENT));

		m_textureManager.init(&config, std::make_unique<TextureLoader>(creator));
		m_audioManager.init(&config, std::make_unique<AudioLoader>());
		m_fontManager.init(&config, std::make_unique<FontLoader>());

		// why both?
		m_textManager.init(&m_fontManager, creator); // accept font manager in constructor?
		//m_textFactory.init(&m_fontManager, renderer);

		Logger::logInfo(std::format("{}[ResourceModule] - Initialization successful!", log_format::INDENT));
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