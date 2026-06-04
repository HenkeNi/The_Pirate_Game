#include "engine/resources/texture/texture_loader.h"
#include "engine/resources/texture/texture.h"
#include "engine/rendering/renderer.h"
#include "engine/core/logger.h"
#include <SDL3_image/SDL_image.h>
#include <filesystem>

namespace cursed_engine
{
	TextureLoader::TextureLoader(Renderer* renderer)
		: m_renderer{ renderer }
	{
	}

	Texture TextureLoader::operator()(const TextureDescriptor& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			Logger::logError("Failed to load texture, invalid path: " + key.path); // stirng format!
			return Texture{ nullptr };
		}

		// use this if not modifying the texture:
		//auto* texture = IMG_LoadTexture(m_renderer.getRenderer(), path.string().c_str());
		auto* surface = IMG_Load(key.path.c_str()); // pass const char* instead?

		if (!surface)
		{
			Logger::logError("Unable to load image, path: " + key.path + ", error: " + SDL_GetError());
			return Texture{ nullptr };
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer->getRenderer(), surface);

		if (!texture)
		{
			Logger::logError("Unable to create texture from surface, path: " + key.path + ", error: " + SDL_GetError());
			return Texture{ nullptr };
		}

		SDL_DestroySurface(surface);
		return Texture{ texture };
	}
}