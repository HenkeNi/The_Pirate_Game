#include "engine/pch.h"
#include "engine/resources/texture_loader.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/renderer.h"
#include "engine/core/logger.h"
#include <SDL3_image/SDL_image.h>

namespace cursed_engine
{
	std::unique_ptr<Texture> TextureLoader::load(Renderer& renderer, const std::filesystem::path& path) const
	{
		if (!std::filesystem::exists(path))
		{
			Logger::logError("Failed to load texture, invalid path: " + path.string());
			return nullptr;
		}

 		auto* surface = IMG_Load(path.string().c_str()); // pass const char* instead?

 		if (!surface)
		{
			Logger::logError("Unable to load image, path: " + path.string() + ", error: " + SDL_GetError());
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);

		if (!texture)
		{
			Logger::logError("Unable to create texture from surface, path: " + path.string() + ", error: " + SDL_GetError());
			return nullptr;
		}

		SDL_DestroySurface(surface);
		return std::make_unique<Texture>(texture);
	}
}