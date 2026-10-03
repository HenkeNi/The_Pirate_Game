#include "engine/rendering/text/text_creator.h"
#include "engine/rendering/text/text.h"
#include "engine/resources/font/font.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	void SDLTextCreator::init(TTF_TextEngine* textEngine)
	{
		m_textEngine = textEngine;
	}

	Result<TextPtr> SDLTextCreator::createText(const std::string& str, const Font& font) const
	{
		assert(m_textEngine && "[SDLTextCreator::createText] - Invalid text engine!");

		const SDLFont& sdlFont = static_cast<const SDLFont&>(font);

		TTF_Text* text = TTF_CreateText(m_textEngine, sdlFont.getInternal(), str.c_str(), str.size());

		if (!text)
		{
			return Result<TextPtr>::failure(std::format("Failed to create text! Reason: {}", SDL_GetError()));
		}

		return Result<TextPtr>::success(std::make_unique<SDLText>(text));
	}
}