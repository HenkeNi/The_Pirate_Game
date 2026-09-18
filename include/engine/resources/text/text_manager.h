#pragma once
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/font.h"
#include "engine/resources/text/text.h"
#include "engine/resources/resource_types.h"

namespace cursed_engine
{
	class ResourceCreator;
	class Texture;
	struct Color;
	// TODO; currently text's are stored by id (not path), maybe should be separate storage?

	// SDLTextManager?

	class TextManager
	{
	public:
		TextManager();
		//TextManager(FontManager* fontManager, Renderer* renderer);
		//TextManager(TextureManager& textureManager, FontManager& fontManager, Renderer& renderer);

		void init(FontManager* fontManager, ResourceCreator* creator);

		// Dont return the actual text instance? -> return ptr?
		// TEST - insert text as well? or do lazy loading?
		[[nodiscard]] Text createText(const std::string& text, ResourceHandle<Font> fontHandle) const; // accept a font or hold fontmanager?


		// TextParams?
		//[[nodiscard]] ResourceHandle<Texture> getHandle(const std::string& id, int fontSize);
		
		//[[nodiscard]] ResourceHandle<Texture> create(const std::string& id, const std::string& text, ResourceHandle<Font> fontHandle, const Color& color, int fontSize); // get font size from font instead??

		//[[nodiscard]] inline const Texture* get(ResourceHandle<Texture> handle) const { return m_cache.retrieve(handle); }
		//[[nodiscard]] inline Texture* get(ResourceHandle<Texture> handle) { return m_cache.retrieve(handle); }


		// or string view+
		//[[nodiscard]] bool isConstructed(const std::string& id, int fontSize) const noexcept;

		// ResourceHandle<Texture> acquireOrCreate(const std::string& id, ResourceHandle<Font> fontHandle, const Color& color, int fontSize);

	private:
		//Texture createTexture(const char* text, Font& font, const Color& color) const;

	
		using HandleMap = std::unordered_map<TextDescriptor, ResourceHandle<Texture>>;

		// create a storage class for runtime resources??
		//ResourceCache<Texture> m_cache; 
		//HandleMap m_keyToHandle;

		// TextureManager& m_textureManager;
		FontManager* m_fontManager;
		
		//Renderer* m_renderer;
		ResourceCreator* m_creator;
	};
}