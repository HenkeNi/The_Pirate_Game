#pragma once
#include <filesystem>

struct TTF_Font;

namespace cursed_engine
{
	class Font
	{
	public:
		struct Tag {};

		Font(TTF_Font* font, std::size_t size);

		[[nodiscard]] inline TTF_Font* get() { return m_font; }
		[[nodiscard]] inline const TTF_Font* get() const { return m_font; }

		// maybe find a better name than size?
		[[nodiscard]] inline std::size_t size() const { return m_size; }

	private:
		TTF_Font* m_font;
		std::size_t m_size;
	};
}