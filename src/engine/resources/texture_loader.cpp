#include "engine/pch.h"
#include "engine/resources/texture_loader.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/renderer.h"
#include <SDL3_image/SDL_image.h>

namespace cursed_engine
{
	std::unique_ptr<Texture> TextureLoader::load(Renderer& renderer, const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			SDL_Log("Couldn't load Texture: invalid path");
			return nullptr;
		}

 		auto* surface = IMG_Load(path.string().c_str()); // pass const char* instead?

 		if (!surface)
		{
			SDL_Log("Unable to load image %s!, SDL Error: %s", path.string().c_str(), SDL_GetError());
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);

		if (!texture)
		{
			SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
			return nullptr;
		}

		SDL_DestroySurface(surface);
		return std::make_unique<Texture>(texture);
	}
}