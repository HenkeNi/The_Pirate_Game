#include "engine/rendering/text.h"
#include "engine/core/logger.h"
#include "engine/rendering/font.h"
#include "engine/rendering/render_types.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	Text::Text()
		: m_text{ nullptr }
	{
	}

	Text::Text(TTF_Text* text)
		: m_text{ text }
	{
	}

	Text::~Text()
	{
		if (m_text)
		{
			TTF_DestroyText(m_text);
			m_text = nullptr;
		}
	}

	Text::Text(Text&& other) noexcept
	{
		m_text = other.m_text;
		other.m_text = nullptr;
	}

	Text& Text::operator=(Text&& other) noexcept
	{
		m_text = other.m_text;
		other.m_text = nullptr;
	
		return *this;
	}

	IVec2 Text::getSize() const noexcept
	{
		IVec2 size;
		TTF_GetTextSize(m_text, &size.x, &size.y);

		return size;
	}

	bool Text::insertText(const std::string& text, int offset)
	{
		return TTF_InsertTextString(m_text, offset, text.c_str(), text.length());
	}

	bool Text::appendText(const std::string& text)
	{
		return TTF_AppendTextString(m_text, text.c_str(), text.length());
	}

	bool Text::deleteText(int offset, int length)
	{
		return TTF_DeleteTextString(m_text, offset, length);
	}

	bool Text::setPosition(int x, int y)
	{
		return TTF_SetTextPosition(m_text, x, y);
	}

	bool Text::setTextScript(uint32_t script)
	{
		return TTF_SetTextScript(m_text, script);
	}

	bool Text::setTextColor(const Color& color)
	{
		return TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
	}

	bool Text::setText(const std::string& text)
	{
		return TTF_SetTextString(m_text, text.c_str(), text.length());
	}

	bool Text::setFont(Font& font)
	{
		return TTF_SetTextFont(m_text, font.getInternal());
	}

	bool Text::setSetDirection(TextDirection direction)
	{
		return TTF_SetTextDirection(m_text, static_cast<TTF_Direction>(direction)); // SAFE To cast?
	}

	bool Text::setWrapWidth(int width)
	{
		return TTF_SetTextWrapWidth(m_text, width);
	}

	bool Text::setWrapWhitespaceVisibility(bool visible)
	{
		return TTF_SetTextWrapWhitespaceVisible(m_text, visible);
	}
}