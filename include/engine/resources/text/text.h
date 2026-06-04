#pragma once
#include "engine/math/vec2.hpp"
#include <string>

struct TTF_Text;
struct TTF_TextEngine;

// store in AssetRegustry (two containers, runtime resources (or assets), 

namespace cursed_engine
{
	struct Color;
	class Font;

	enum class TextDirection
	{
		Invalid = 0,
		LeftToRight = 4,
		TopToBottom,
		RightToLeft,
		BottomToTop
	};

	class Text
	{
	public:
		Text();
		Text(TTF_Text* text);
		~Text();

		Text(const Text&) = delete;
		Text(Text&& other) noexcept;

		Text& operator=(const Text&) = delete;
		Text& operator=(Text&& other) noexcept;

		[[nodiscard]] IVec2 getSize() const noexcept;
		[[nodiscard]] inline TTF_Text* get() noexcept { return m_text; } // get or getRaw?
		[[nodiscard]] inline const TTF_Text* get() const noexcept { return m_text; }
		[[nodiscard]] inline bool isValid() const noexcept { return m_text != nullptr; }

		bool insertText(const std::string& text, int offset);
		bool appendText(const std::string& text);
		bool deleteText(int offset, int length);

		bool setPosition(int x, int y);
		bool setTextScript(uint32_t script);
		bool setTextColor(const Color& color);
		bool setText(const std::string& text);
		bool setFont(Font& font);
		bool setSetDirection(TextDirection direction);
		bool setWrapWidth(int width);
		bool setWrapWhitespaceVisibility(bool visible);


	private:
		TTF_Text* m_text;
		//std::string m_text; // create a TextDescriptor
	};
}