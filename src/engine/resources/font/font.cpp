#include "engine/resources/font/font.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	SDLFont::SDLFont()
		: m_font{ nullptr }
	{
	}

	SDLFont::SDLFont(TTF_Font* font, FontDescriptor descriptor)
		: m_font{ font }
	{
		TTF_SetFontKerning(m_font, descriptor.kerning);
		TTF_SetFontOutline(m_font, descriptor.outline);
		TTF_SetFontSize(m_font, descriptor.size);
		TTF_SetFontStyle(font, static_cast<TTF_FontStyleFlags>(descriptor.style)); //  make sure works...
	}

	SDLFont::~SDLFont()
	{
	}

	SDLFont::SDLFont(SDLFont&& other)
		: m_font{ other.m_font }
	{
		other.m_font = nullptr;
	}

	SDLFont& SDLFont::operator=(SDLFont&& other)
	{
		m_font = other.m_font;
		other.m_font = nullptr;

		return *this;
	}

	/*FontStyle SDLFont::getStyle() const noexcept
	{
		return TTF_GetFontStyle(m_font);
	}*/

	bool SDLFont::getKerning() const noexcept
	{
		return TTF_GetFontKerning(m_font);
	}

	int SDLFont::getOutline() const noexcept
	{
		return TTF_GetFontOutline(m_font);
	}

	int SDLFont::getWidth() const noexcept
	{
		return TTF_GetFontWeight(m_font);
	}

	int SDLFont::getHeight() const noexcept
	{
		return TTF_GetFontHeight(m_font);
	}

	bool SDLFont::isFixedWidth() const noexcept
	{
		return TTF_FontIsFixedWidth(m_font);
	}

	bool SDLFont::isScalable() const noexcept
	{
		return TTF_FontIsScalable(m_font);
	}
}