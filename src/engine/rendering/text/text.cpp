#include "engine/rendering/text/text.h"
#include "engine/resources/font/font.h"
#include "engine/rendering/render_types.h"
#include "engine/core/logger.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace
{
	using namespace cursed_engine;

	TTF_Direction toTTFDirection(TextDirection direction)
	{
		switch (direction)
		{
		case TextDirection::LeftToRight:
			return TTF_DIRECTION_LTR;
		case TextDirection::TopToBottom:
			return TTF_DIRECTION_TTB;
		case TextDirection::RightToLeft:
			return TTF_DIRECTION_RTL;
		case TextDirection::BottomToTop:
			return TTF_DIRECTION_BTT;
		}
		return TTF_DIRECTION_INVALID;
	}
}

namespace cursed_engine
{
	SDLText::SDLText(TTF_Text* text)
		: m_text{ text }
	{
	}

	SDLText::~SDLText()
	{
		if (m_text)
		{
			TTF_DestroyText(m_text);
			m_text = nullptr;
		}
	}

	SDLText::SDLText(SDLText&& other) noexcept
	{
		m_text = other.m_text;
		other.m_text = nullptr;
	}

	SDLText& SDLText::operator=(SDLText&& other) noexcept
	{
		m_text = other.m_text;
		other.m_text = nullptr;

		return *this;
	}

	IVec2 SDLText::getSize() const noexcept
	{
		IVec2 size;
		TTF_GetTextSize(m_text, &size.x, &size.y);

		return size;
	}

	bool SDLText::insertText(const std::string& text, int offset)
	{
		return TTF_InsertTextString(m_text, offset, text.c_str(), text.length());
	}

	bool SDLText::appendText(const std::string& text)
	{
		return TTF_AppendTextString(m_text, text.c_str(), text.length());
	}

	bool SDLText::deleteText(int offset, int length)
	{
		return TTF_DeleteTextString(m_text, offset, length);
	}

	bool SDLText::setPosition(int x, int y)
	{
		return TTF_SetTextPosition(m_text, x, y);
	}

	bool SDLText::setTextScript(uint32_t script)
	{
		return TTF_SetTextScript(m_text, script);
	}

	bool SDLText::setTextColor(const Color& color)
	{
		return TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
	}

	bool SDLText::setText(const std::string& text)
	{
		return TTF_SetTextString(m_text, text.c_str(), text.length());
	}

	bool SDLText::setFont(const Font& font)
	{
		const SDLFont& sdlFont = static_cast<const SDLFont&>(font);
		return TTF_SetTextFont(m_text, sdlFont.getInternal());
	}

	bool SDLText::setSetDirection(TextDirection direction)
	{
		return TTF_SetTextDirection(m_text, toTTFDirection(direction));
	}

	bool SDLText::setWrapWhitespaceVisibility(bool visible)
	{
		return TTF_SetTextWrapWhitespaceVisible(m_text, visible);
	}

	bool SDLText::setWrapWidth(int width)
	{
		return TTF_SetTextWrapWidth(m_text, width);
	}
}