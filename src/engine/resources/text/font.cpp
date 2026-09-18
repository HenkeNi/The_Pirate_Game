#include "engine/resources/text/font.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	// TODO, consider adding: 		
	// TTF_SetFontLineSkip();
	// TTF_SetFontWrapAlignment	
	// TTF_SetFontSDF	
	// TTF_SetFontHinting(); 

	Font::Font()
		: m_font{ nullptr }, m_descriptor{}
	{
	}

	Font::Font(TTF_Font* font, FontDescriptor descriptor)
		: m_font{ font }, m_descriptor{ descriptor }
	{
	}

	bool Font::isFixedWidth() const
	{
		return TTF_FontIsFixedWidth(m_font);
	}

	bool Font::isScalable() const
	{
		return TTF_FontIsScalable(m_font);
	}
}