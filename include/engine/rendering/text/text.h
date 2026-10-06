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

	class Text
	{
	public:
		Text() = default;
		virtual ~Text() = default;
		
		Text(const Text&) = delete;
		Text(Text&&) = default;

		Text& operator=(const Text&) = delete;
		Text& operator=(Text&&) = default;

		[[nodiscard]] virtual IVec2 getSize() const noexcept = 0;
		[[nodiscard]] virtual bool isValid() const noexcept = 0;

		virtual bool insertText(const std::string& text, int offset) = 0;
		virtual bool appendText(const std::string& text) = 0;
		virtual bool deleteText(int offset, int length) = 0;

		virtual bool setPosition(int x, int y) = 0; 
		virtual bool setTextScript(uint32_t script) = 0; 

		virtual bool setTextColor(const Color& color) = 0;
		virtual bool setText(const std::string& text) = 0;

		virtual bool setFont(const Font& font) = 0;
		virtual bool setSetDirection(TextDirection direction) = 0;

		virtual bool setWrapWhitespaceVisibility(bool visible) = 0;
		virtual bool setWrapWidth(int width) = 0;
	};

	class SDLText final : public Text
	{
	public:
		SDLText(TTF_Text* text = nullptr);
		~SDLText();

		SDLText(SDLText&& other) noexcept;
		SDLText& operator=(SDLText&& other) noexcept;

		[[nodiscard]] inline TTF_Text* getInternal() const noexcept { return m_text; }

		[[nodiscard]] IVec2 getSize() const noexcept override;
		[[nodiscard]] inline bool isValid() const noexcept override { return m_text != nullptr; }

		bool insertText(const std::string& text, int offset) override;
		bool appendText(const std::string& text) override;
		bool deleteText(int offset, int length) override;

		bool setPosition(int x, int y) override;
		bool setTextScript(uint32_t script) override;

		bool setTextColor(const Color& color) override;
		bool setText(const std::string& text) override;

		bool setFont(const Font& font) override;
		bool setSetDirection(TextDirection direction) override;

		bool setWrapWhitespaceVisibility(bool visible) override;
		bool setWrapWidth(int width) override;
		
	private:
		TTF_Text* m_text;

		//std::string m_text; // create a TextDescriptor?
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