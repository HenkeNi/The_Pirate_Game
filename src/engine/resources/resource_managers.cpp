#include "engine/resources/resource_managers.h"
#include "engine/resources/resource_loaders.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	ResourceManagers::ResourceManagers()
	{
	}

	bool ResourceManagers::init(const TextureCreator* textureCreator, const AudioCreator* audioCreator)
	{
		
		//m_textureManager.init(catalog); // texture manager holds the (resource) paths? or accept them (correct one) as args?
		m_textureManager.addLoader(std::make_unique<TextureLoader>(textureCreator));
		m_textureManager.scanDirectory("../assets/");

		//m_audioManager.init(catalog);
		m_audioManager.addLoader(std::make_unique<AudioLoader>(audioCreator));
		m_audioManager.scanDirectory("../assets/");

		//m_fontManager.init(catalog);
		m_fontManager.addLoader(std::make_unique<FontLoader>());
		m_fontManager.scanDirectory("../assets/");

		//m_textureManager.init(&config, std::make_unique<TextureLoader>(textureCreator));
		//m_audioManager.init(&config, std::make_unique<AudioLoader>(audioCreator));
		//m_fontManager.init(&config, std::make_unique<FontLoader>());

		Logger::logInfo(std::format("\'Resources\' successfully initialized!", log_format::INDENT));
		return true;
	}

	void ResourceManagers::shutdown()
	{
		Logger::logInfo("Resources shutdown complete!");
	}

	void ResourceManagers::update(uint64_t currentFrame, float deltaTime)
	{
		m_audioManager.update(currentFrame, deltaTime);
		m_fontManager.update(currentFrame, deltaTime);
		m_textureManager.update(currentFrame, deltaTime);
	}
}