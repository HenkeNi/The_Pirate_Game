#pragma once
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/font.h"
#include "engine/resources/resource_types.h"
#include "engine/resources/text/text.h"
//#include "engine/resources/texture_manager.h"
//#include "engine/resources/font_manager.h"

//#include "engine/resources/resource_cache.hpp"
//#include "engine/rendering/render_types.h"

namespace cursed_engine
{
	struct TextKey // move to text file...
	{
		std::string id;
		int fontSize;

		// font??

		bool operator==(const TextKey& other) const noexcept
		{
			return id == other.id && fontSize == other.fontSize;
		}

	};
}

template<>
struct std::hash<cursed_engine::TextKey>
{
	std::size_t operator()(const cursed_engine::TextKey& key) const noexcept
	{
		std::size_t h = std::hash<std::string>{}(key.id);

		h ^= std::hash<int>{}(key.fontSize)
			+ 0x9e3779b9 + (h << 6) + (h >> 2);

		return h;
		//return std::hash<std::string>{}(key.fontId) ^ std::hash<std::size_t>{}(key.fontSize);	
	}
};

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

	

	

	//class FontManager;
	class Renderer;
	class Texture;
	struct Color;
	// TODO; currently text's are stored by id (not path), maybe should be separate storage?

	class TextManager
	{
	public:
		TextManager();
		TextManager(FontManager* fontManager, Renderer* renderer);
		//TextManager(TextureManager& textureManager, FontManager& fontManager, Renderer& renderer);

		void init(FontManager* fontManager, Renderer& renderer);

		// TextParams?
		[[nodiscard]] ResourceHandle<Texture> getHandle(const std::string& id, int fontSize);
		
		[[nodiscard]] ResourceHandle<Texture> create(const std::string& id, const std::string& text, ResourceHandle<Font> fontHandle, const Color& color, int fontSize); // get font size from font instead??

		[[nodiscard]] inline const Texture* get(ResourceHandle<Texture> handle) const { return m_cache.retrieve(handle); }
		[[nodiscard]] inline Texture* get(ResourceHandle<Texture> handle) { return m_cache.retrieve(handle); }


		// or string view+
		[[nodiscard]] bool isConstructed(const std::string& id, int fontSize) const noexcept;

		// ResourceHandle<Texture> acquireOrCreate(const std::string& id, ResourceHandle<Font> fontHandle, const Color& color, int fontSize);

	private:
		Texture createTexture(const char* text, Font& font, const Color& color) const;

	
		using HandleMap = std::unordered_map<TextKey, ResourceHandle<Texture>>;

		// create a storage class for runtime resources??
		ResourceCache<Texture> m_cache; 
		HandleMap m_keyToHandle;

		// TextureManager& m_textureManager;
		FontManager* m_fontManager;
		
		Renderer* m_renderer;

	};
}

//template<>
//struct std::hash<cursed_engine::TextKey>
//{
//	std::size_t operator()(const cursed_engine::TextKey& key) const noexcept
//	{
//		std::size_t h = std::hash<std::string>{}(key.id);
//
//		h ^= std::hash<int>{}(key.fontSize)
//			+ 0x9e3779b9 + (h << 6) + (h >> 2);
//
//		return h;
//		//return std::hash<std::string>{}(key.fontId) ^ std::hash<std::size_t>{}(key.fontSize);	
//	}
//};