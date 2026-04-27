#pragma once
#include "engine/resources/resource_manager.hpp"
#include <filesystem>

struct TTF_Font;

// Maybe FontManager contains REsourceManager, maps fontsizes to map of id to paths ?
// getFont(id, font_size); 

namespace cursed_engine
{
	class Font
	{
	public:
		struct Tag {};

		Font() = default;
		Font(TTF_Font* font, int size);

		[[nodiscard]] inline TTF_Font* get() { return m_font; }
		[[nodiscard]] inline const TTF_Font* get() const { return m_font; }

		// maybe find a better name than size?
		[[nodiscard]] inline int size() const { return m_size; }

	private:
		TTF_Font* m_font;
		int m_size;
	};

	struct FontKey
	{
		std::string path;
		int fontSize;
		
		bool operator==(const FontKey& other) const noexcept
		{
			return path == other.path && fontSize == other.fontSize;
		}
	};

	struct FontLoader
	{
		[[nodiscard]] Font operator()(const FontKey& key) const;
	};


	using FontManager = ResourceManager<Font, FontKey, FontLoader>;
}


template<>
struct std::hash<cursed_engine::FontKey>
{
	std::size_t operator()(const cursed_engine::FontKey& key) const noexcept
	{
		std::size_t h = std::hash<std::string>{}(key.path);

		h ^= std::hash<int>{}(key.fontSize)
			+ 0x9e3779b9 + (h << 6) + (h >> 2);

		return h;
		//return std::hash<std::string>{}(key.fontId) ^ std::hash<std::size_t>{}(key.fontSize);	
	}
};