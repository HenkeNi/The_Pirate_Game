#pragma once
//#include "engine/rendering/font.h" // find better way! forward_declarations.h
#include "engine/resources/resource_types.h"

//struct TTF_TextEngine;

namespace cursed_engine
{
	class Font;
	//class FontManager;
	class Text;
	class Renderer;

	template <typename T>
	class ResourceHandle;


	class TextFactory
	{
	public:
		TextFactory() = default;
		TextFactory(FontManager* fontManager, Renderer& renderer); // acecpet all by ref?

		void init(FontManager* fontManager, Renderer& renderer);

		Text createText(const std::string& text, ResourceHandle<Font> fontHandle); // accept a font or hold fontmanager?

		// create text with properties? (return a builder)

	private:
		FontManager* m_fontManager;
		Renderer* m_renderer;
		//TTF_TextEngine* m_textEngine; // pointer or ref?
	};
}