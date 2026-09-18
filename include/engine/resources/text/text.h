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

	struct TextDescriptor
	{
		std::string id;
		int fontSize;

		// font??

		bool operator==(const TextDescriptor& other) const noexcept
		{
			return id == other.id && fontSize == other.fontSize;
		}

	};

	// TODO; creat TExt base class
	// SDLText
	class Text
	{
	public:
		Text(TTF_Text* text = nullptr);
		~Text();

		Text(const Text&) = delete;
		Text(Text&& other) noexcept;

		Text& operator=(const Text&) = delete;
		Text& operator=(Text&& other) noexcept;

		[[nodiscard]] IVec2 getSize() const noexcept;
		[[nodiscard]] inline bool isValid() const noexcept { return m_text != nullptr; }

		[[nodiscard]] inline const TTF_Text* get() const noexcept { return m_text; }
		[[nodiscard]] inline TTF_Text* get() noexcept { return m_text; } // get or getRaw? getInternal getHandle?

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

template<>
struct std::hash<cursed_engine::TextDescriptor>
{
	std::size_t operator()(const cursed_engine::TextDescriptor& descriptor) const noexcept
	{
		std::size_t h = std::hash<std::string>{}(descriptor.id);

		h ^= std::hash<int>{}(descriptor.fontSize)
			+ 0x9e3779b9 + (h << 6) + (h >> 2);

		return h;
		//return std::hash<std::string>{}(key.fontId) ^ std::hash<std::size_t>{}(key.fontSize);	
	}
};