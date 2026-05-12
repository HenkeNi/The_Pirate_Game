#pragma once
#include "engine/rendering/font.h" // find better way! forward_declarations.h

struct TTF_TextEngine;

namespace cursed_engine
{
	class Font;
	//class FontManager;
	class Text;

	template <typename T>
	class ResourceHandle;


	class TextFactory
	{
	public:
		TextFactory(FontManager& fontManager, TTF_TextEngine* engine);

		Text createText(const std::string& text, ResourceHandle<Font> fontHandle); // accept a font or hold fontmanager?

		// create text with properties? (return a builder)

	private:
		FontManager& m_fontManager;
		TTF_TextEngine* m_textEngine; // pointer or ref?
	};
}