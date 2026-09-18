#include "engine/resources/resource_creator.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/text.h"
#include "engine/resources/text/font.h"
#include "engine/core/result.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <cassert>
#include <format>

namespace cursed_engine
{
	void SDLResourceCreator::init(TTF_TextEngine* textEngine, SDL_Renderer* renderer)
	{
		m_textEngine = textEngine; 
		m_renderer = renderer;
	}

	Result<Texture> SDLResourceCreator::createTextureFromSurface(Surface surface) const
	{
		assert(m_renderer && "[SDLResourceCreator::createTextureFromSurface] - Invalid renderer!");

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface.surface);
		
		if (!texture)
		{
			return Result<Texture>::failure(std::format("Failed to generate texture! Reason: {}", SDL_GetError()));
		}

		SDL_DestroySurface(surface.surface);

		return Result<Texture>::success(texture);
	}

	Result<Text> SDLResourceCreator::createText(const std::string& text, Font& font) const
	{
		assert(m_renderer && "[SDLResourceCreator::createText] - Invalid text engine!");

		TTF_Text* textObject = TTF_CreateText(m_textEngine, font.getInternal(), text.c_str(), text.size());

		if (!textObject)
		{
			return Result<Text>::failure(std::format("Failed to create text! Reason: {}", SDL_GetError()));
		}

		return Result<Text>::success(textObject);
	}
}