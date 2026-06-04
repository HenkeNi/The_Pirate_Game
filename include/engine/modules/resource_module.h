#pragma once
#include "engine/resources/text/text_factory.h"
#include "engine/resources/text/text_manager.h"
#include "engine/resources/resource_types.h"

namespace cursed_engine
{
	class Renderer;

	class ResourceModule
	{
	public:		
		bool init(Renderer& renderer, const cursed_engine::ResourceConfig& config);
		void shutdown();

		void update(uint64_t currentFrame, float deltaTime); // rename? handle offloading...

		// use facades or interfaces instead?
		[[nodiscard]] inline TextureManager& getTextureManager() noexcept { return m_textureManager; }
		[[nodiscard]] inline AudioManager& getAudioManager() noexcept { return m_audioManager; }
		[[nodiscard]] inline FontManager& getFontManager() noexcept { return m_fontManager; }
		[[nodiscard]] inline TextManager& getTextManager() noexcept { return m_textManager; }
		[[nodiscard]] inline TextFactory& getTextFactory() noexcept { return m_textFactory; }

	private:
		TextureManager m_textureManager;
		AudioManager m_audioManager; // AudioAccessor?

		FontManager m_fontManager; // Combine font manager and text manager?
		TextManager m_textManager;
		TextFactory m_textFactory;
	};
}