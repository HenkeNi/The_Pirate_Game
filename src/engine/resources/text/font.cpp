#include "engine/resources/text/font.h"
#include "engine/core/logger.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	// TODO, consider adding: 		
	// TTF_SetFontLineSkip();
	// TTF_SetFontWrapAlignment	
	// TTF_SetFontSDF	
	// TTF_SetFontHinting(); 

	Font::Font()
		: m_font{ nullptr }, m_descriptor{}
	{
	}

	Font::Font(TTF_Font* font, FontDescriptor descriptor)
		: m_font{ font }, m_descriptor{ descriptor }
	{
	}

	bool Font::isFixedWidth() const
	{
		return TTF_FontIsFixedWidth(m_font);
	}

	bool Font::isScalable() const
	{
		return TTF_FontIsScalable(m_font);
	}

	Font FontLoader::operator()(const FontDescriptor& descriptor) const
	{
		if (!std::filesystem::exists(descriptor.path))
		{
			Logger::logError("Failed to load font, invalid path: " + descriptor.path);
			assert(false && "Trying to load font with invalid path!");

			return Font{};
		}

		TTF_Font* font = TTF_OpenFont(descriptor.path.c_str(), (float)descriptor.size);

		if (!font)
		{
			Logger::logError("Unable to load font, path: " + descriptor.path + ", error: " + SDL_GetError());
			return Font{};
		}

		TTF_SetFontStyle(font, static_cast<TTF_FontStyleFlags>(descriptor.style));
		TTF_SetFontOutline(font, descriptor.outline);
		TTF_SetFontKerning(font, descriptor.kerning);

		return Font{ font, descriptor };
	}
}