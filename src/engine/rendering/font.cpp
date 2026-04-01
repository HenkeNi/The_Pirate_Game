#include "engine/rendering/font.h"

namespace cursed_engine
{
	Font::Font(TTF_Font* font, std::size_t size)
		: m_font{ font }, m_size{ size }
	{
	}
}