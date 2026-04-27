#include "engine/rendering/font.h"
#include "engine/core/logger.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	Font::Font(TTF_Font* font, int size)
		: m_font{ font }, m_size{ size }
	{
	}

	Font FontLoader::operator()(const FontKey& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			Logger::logError("Failed to load font, invalid path: " + key.path);
			assert(false && "Trying to load font with invalid path!");

			return Font{};
		}

		TTF_Font* font = TTF_OpenFont(key.path.c_str(), (float)key.fontSize);

		if (!font)
		{
			Logger::logError("Unable to load font, path: " + key.path + ", error: " + SDL_GetError());
			return Font{};
		}

		//SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer.getRenderer(), surface);

		//if (!texture)
		//{
		//	Logger::logError("Unable to create texture from surface, path: " + path.string() + ", error: " + SDL_GetError());
		//	return nullptr;
		//}

		return Font{ font, key.fontSize };
	}
}