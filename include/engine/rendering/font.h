#pragma once
#include "engine/resources/resource_manager.hpp"
#include "engine/utils/utils.h"
#include <filesystem>

struct TTF_Font;

// Maybe FontManager contains REsourceManager, maps fontsizes to map of id to paths ?
// getFont(id, font_size); 
/*
* Roboto16Regular
Roboto16Outline2
Roboto16Bold
Roboto24Title
*/

namespace cursed_engine
{
	enum class FontStyle : uint32_t
	{
		Normal = 0,
		Bold = 1 << 0,
		Italic = 1 << 1,
		Underline = 1 << 2,
		Strikethrough = 1 << 3
	};

	inline FontStyle operator|(FontStyle lhs, FontStyle rhs)
	{
		return static_cast<FontStyle>(
			static_cast<uint32_t>(lhs) |
			static_cast<uint32_t>(rhs)
			);
	}

	inline FontStyle& operator|=(FontStyle& lhs, FontStyle rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	struct FontDescriptor
	{
		std::string path;
		FontStyle style;
		int size;
		int outline;
		bool kerning; // spacing between characters

		bool operator==(const FontDescriptor& other) const noexcept
		{
			return path == other.path
				&& style == other.style
				&& size == other.size
				&& outline == other.outline
				&& kerning == other.kerning;
		}
	};

	class Font
	{
	public:
		struct Tag {};

		Font();
		Font(TTF_Font* font, FontDescriptor descriptor);

		[[nodiscard]] inline TTF_Font* getInternal() { return m_font; }
		[[nodiscard]] inline const TTF_Font* getInternal() const { return m_font; }

		// maybe find a better name than size?
		[[nodiscard]] inline const std::string& getPath() const { return m_descriptor.path; }
		[[nodiscard]] inline FontStyle getStyle() const { return m_descriptor.style; }
		[[nodiscard]] inline int getSize() const { return m_descriptor.size; }
		[[nodiscard]] inline int getOutline() const { return m_descriptor.outline; }
		[[nodiscard]] inline bool getKerning() const { return m_descriptor.kerning; }

		[[nodiscard]] bool isFixedWidth() const;
		[[nodiscard]] bool isScalable() const;

	private:
		TTF_Font* m_font;
		FontDescriptor m_descriptor;
	};

	struct FontLoader
	{
		[[nodiscard]] Font operator()(const FontDescriptor& descriptor) const;
	};

	using FontManager = ResourceManager<Font, FontDescriptor, FontLoader>;
}

template<>
struct std::hash<cursed_engine::FontDescriptor>
{
	std::size_t operator()(const cursed_engine::FontDescriptor& descriptor) const noexcept
	{
		using namespace cursed_engine::utils::hash;

		std::size_t h = 0;

		hashCombine(h, descriptor.path);
		hashCombine(h, static_cast<uint32_t>(descriptor.style));
		hashCombine(h, descriptor.size);
		hashCombine(h, descriptor.outline);
		hashCombine(h, descriptor.kerning);

		return h;
		//std::size_t h = std::hash<std::string>{}(descriptor.path);

		//h ^= std::hash<int>{}(descriptor.size)
		//	+ 0x9e3779b9 + (h << 6) + (h >> 2);

		//return h;
		//return std::hash<std::string>{}(descriptor.fontId) ^ std::hash<std::size_t>{}(descriptor.fontSize);	
	}
};