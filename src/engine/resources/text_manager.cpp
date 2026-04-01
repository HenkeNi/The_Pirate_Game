#include "engine/resources/text_manager.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/render_types.h"
#include "engine/rendering/renderer.h"
#include "engine/core/logger.h"

//#include "engine/rendering/font.h"
//#include "engine/core/logger.h"
//#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	TextManager::TextManager(TextureManager& textureManager, FontManager& fontManager, Renderer& renderer)
		: m_textureManager{ textureManager }, m_fontManager{ fontManager }, m_renderer{ renderer }
	{
	}


	ResourceHandle<Texture> TextManager::getHandle(const std::string& id, int fontSize)
	{
		using TextureHandle = ResourceHandle<Texture>;

		TextureHandle textureHandle = m_textureManager.getHandle(TextureKey{ id + std::to_string(fontSize) });

		if (textureHandle.isValid())
		{
			return textureHandle;
		}

		return TextureHandle::invalid();
	}

	// Can get text from localization?!
	ResourceHandle<Texture> TextManager::create(const std::string& id, const std::string& text, ResourceHandle<Font> fontHandle, const Color& color, int fontSize)
	{
		using TextureHandle = ResourceHandle<Texture>;

		if (!fontHandle.isValid() || !m_fontManager.isValidHandle(fontHandle)) // check all in manager??
		{
			Logger::logError("Invalid font handle!");

			// TODO; load font....
			// m_fontManager.getHandle() // need font id for this...
		}


		auto font = m_fontManager.get(fontHandle);

		auto texture = createTexture(text.c_str(), font, color);

		auto textureHandle = m_textureManager.insertResource(TextureKey{ id + std::to_string(fontSize) }, std::move(texture));
		return textureHandle;

		// check if font is loaded => else load it... (should text manager handle this?)




		//TextureKey key{ id + std::to_string(fontSize) };
		//if (m_textureManager.isLoaded(key))
		//{
		//	int x = 20;
		//}
		//else
		//{
		//	int y = 20;
		//}
		//// if text(ure) doesnt exist => create and insert it...



		//return ResourceHandle<Texture>::invalid();
	}

	bool TextManager::isConstructed(const std::string& id, int fontSize) const noexcept
	{
		return m_textureManager.isLoaded(TextureKey{ id + std::to_string(fontSize) });
	}

	std::unique_ptr<Texture> TextManager::createTexture(const char* text, Font& font, const Color& color) const
	{
		SDL_Surface* surface = TTF_RenderText_Blended(font.get(), text, 0, { color.r, color.g, color.b, color.a });

		if (!surface)
		{
			Logger::logError(std::format("Failed to generate surface for text! Error: {}", SDL_GetError()));
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer.getRenderer(), surface);

		if (!texture)
		{
			Logger::logError(std::format("Failed to create texture from surface! Error: {}", SDL_GetError()));
			return nullptr;
		}

		return std::make_unique<Texture>(texture);
	}
}