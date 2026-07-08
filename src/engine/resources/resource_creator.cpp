#include "engine/resources/resource_creator.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/text.h"
#include "engine/resources/text/font.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <cassert>

namespace cursed_engine
{
	void SDLResourceCreator::init(TTF_TextEngine* textEngine, SDL_Renderer* renderer)
	{
		m_textEngine = textEngine; 
		m_renderer = renderer;
	}

	Texture SDLResourceCreator::createTextureFromSurface(Surface surface) const noexcept
	{
		assert(m_renderer && "Trying to create texture with an invalid renderer!");

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface.surface);
		SDL_DestroySurface(surface.surface); // destroy where?

		return Texture(texture);
	}

	Text SDLResourceCreator::createText(const std::string& text, Font& font) const noexcept
	{
		assert(m_textEngine && "Trying to create text with an invalid text engine!");

		TTF_Text* textObject = TTF_CreateText(m_textEngine, font.getInternal(), text.c_str(), text.size());

		if (!textObject)
		{
			Logger::logError("Unable to generate text: " + text + ", error: " + SDL_GetError());
			return Text{ nullptr };
		}

		return Text{ textObject };
	}
}