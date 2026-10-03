#pragma once
#include "engine/resources/audio/audio_manager.h"
#include "engine/resources/font/font_manager.h"
#include "engine/resources/texture/texture_manager.h"

namespace cursed_engine
{
	class AudioCreator;
	class TextureCreator;
	struct ResourceConfig; 

	enum class Backend;

	class ResourceModule
	{
	public:		
		bool init(const TextureCreator* textureCreator, const AudioCreator* audioCreator, const ResourceConfig& config, Backend backend);
		void shutdown();

		void update(uint64_t currentFrame, float deltaTime); // rename? handle offloading...

		// use facades or interfaces instead?
		[[nodiscard]] inline TextureManager& getTextureManager() noexcept { return m_textureManager; }
		[[nodiscard]] inline AudioManager& getAudioManager() noexcept { return m_audioManager; }
		[[nodiscard]] inline FontManager& getFontManager() noexcept { return m_fontManager; }

	private:
		TextureManager m_textureManager;
		AudioManager m_audioManager;
		FontManager m_fontManager;
	};
}