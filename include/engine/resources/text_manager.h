#pragma once
#include "engine/resources/texture_manager.h"
#include "engine/resources/font_manager.h"

//#include "engine/resources/resource_cache.hpp"
//#include "engine/rendering/render_types.h"

namespace cursed_engine
{
	//struct Text
	//{
	//	std::unique_ptr<Texture> texture;
	//	Font& font;
	//	// size?
	//};

	//	struct DynamicText
	//	{ };

	class Renderer;
	class Texture;
	struct Color;

	class TextManager
	{
	public:
		TextManager(TextureManager& textureManager, FontManager& fontManager, Renderer& renderer);

		// TextParams?
		[[nodiscard]] ResourceHandle<Texture> getHandle(const std::string& id, int fontSize);
		
		[[nodiscard]] ResourceHandle<Texture> create(const std::string& id, const std::string& text, ResourceHandle<Font> fontHandle, const Color& color, int fontSize);

		// or string view+
		[[nodiscard]] bool isConstructed(const std::string& id, int fontSize) const noexcept;

		// ResourceHandle<Texture> acquireOrCreate(const std::string& id, ResourceHandle<Font> fontHandle, const Color& color, int fontSize);

	private:
		std::unique_ptr<Texture> createTexture(const char* text, Font& font, const Color& color) const;

		TextureManager& m_textureManager;
		FontManager& m_fontManager;
		
		Renderer& m_renderer;
	};
}