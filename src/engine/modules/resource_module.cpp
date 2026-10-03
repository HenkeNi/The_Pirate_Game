#include "engine/modules/resource_module.h"
#include "engine/resources/resource_loaders.h"
#include "engine/core/logger.h"

//#include "engine/resources/resource_creator.h"
//#include "engine/resources/texture/texture.h"
//#include "engine/resources/audio/audio.h"
//#include "engine/resources/text/font.h"
//#include <format>

namespace cursed_engine
{
	bool ResourceModule::init(const TextureCreator* textureCreator, const TextCreator* textCreator, const AudioCreator* audioCreator, const ResourceConfig& config, Backend backend)
	{
		Logger::logInfo(std::format("{}[ResourceModule] - Initialization started...", log_format::INDENT));

		switch (backend)
		{
		case Backend::SDL:
			m_textureManager.init(&config, std::make_unique<SDLTextureLoader>(textureCreator));
			m_audioManager.init(&config, std::make_unique<SDLAudioLoader>(audioCreator));
			m_fontManager.init(&config, std::make_unique<SDLFontLoader>());
			break;

		default:
			Logger::logInfo(std::format("{}[RenderModule] - Unsupported backend {}", log_format::INDENT, (int)backend));
			return false;
		}
	
		m_textManager.init(&m_fontManager, textCreator); // accept font manager in constructor?
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
}