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

	// TODO, consider adding:
	// TTF_SetFontLineSkip();
	// TTF_SetFontWrapAlignment
	// TTF_SetFontSDF
	// TTF_SetFontHinting();


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

	/*struct FontParams
	{
		FontStyle style;
		int size;
		int outline;
		bool kerning;
	};*/


	class Font
	{
	public:
		Font() = default;
		virtual ~Font() = default;

		Font(const Font&) = delete;
		Font(Font&&) = default;

		Font& operator=(const Font&) = delete;
		Font& operator=(Font&&) = default;

		//[[nodiscard]] virtual FontStyle getStyle() const noexcept = 0; change to bitset? (allow multiple styles)
		[[nodiscard]] virtual bool getKerning() const noexcept = 0;

		[[nodiscard]] virtual int getOutline() const noexcept = 0;
		[[nodiscard]] virtual int getWidth() const noexcept = 0;
		[[nodiscard]] virtual int getHeight() const noexcept = 0;

		[[nodiscard]] virtual bool isFixedWidth() const noexcept = 0;
		[[nodiscard]] virtual bool isScalable() const noexcept = 0;

		//[[nodiscard]] inline const std::string& getPath() const { return m_descriptor.path; } // USED?
	};

	class SDLFont final : public Font
	{
	public:
		SDLFont();
		SDLFont(TTF_Font* font, FontDescriptor params);
		~SDLFont();

		SDLFont(SDLFont&& other);
		SDLFont& operator=(SDLFont&& other);

		[[nodiscard]] inline TTF_Font* getInternal() const noexcept { return m_font; }

		//[[nodiscard]] FontStyle getStyle() const noexcept override;
		[[nodiscard]] bool getKerning() const noexcept override;

		[[nodiscard]] int getOutline() const noexcept override;
		[[nodiscard]] int getWidth() const noexcept override;
		[[nodiscard]] int getHeight() const noexcept override;

		[[nodiscard]] bool isFixedWidth() const noexcept override;
		[[nodiscard]] bool isScalable() const  noexcept override;

	private:
		TTF_Font* m_font;
	};
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