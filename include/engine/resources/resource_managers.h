#pragma once
#include "engine/resources/audio/audio_manager.h"
#include "engine/resources/font/font_manager.h"
#include "engine/resources/texture/texture_manager.h"

namespace cursed_engine
{
	class AudioCreator;
	class TextureCreator;

	// Rename "Resources"? or ResourecRegistry
	class ResourceManagers final
	{
	public:		
		ResourceManagers();
		~ResourceManagers() = default;

		ResourceManagers(const ResourceManagers&) = delete;
		ResourceManagers(ResourceManagers&&) = delete;

		ResourceManagers& operator=(const ResourceManagers&) = delete;
		ResourceManagers& operator=(ResourceManagers&&) = delete;

		bool init(const TextureCreator* textureCreator, const AudioCreator* audioCreator);
		void shutdown();

		void update(uint64_t currentFrame, float deltaTime); // rename? handle offloading...

		[[nodiscard]] inline AudioManager& getAudioManager() noexcept { return m_audioManager; }
		[[nodiscard]] inline FontManager& getFontManager() noexcept { return m_fontManager; }
		[[nodiscard]] inline TextureManager& getTextureManager() noexcept { return m_textureManager; }

	private:
		AudioManager m_audioManager;
		FontManager m_fontManager;
		TextureManager m_textureManager;
	};
}