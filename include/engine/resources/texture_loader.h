#pragma once
#include <filesystem>
#include <optional>
#include "engine/rendering/texture.h"

struct SDL_Surface;

namespace cursed_engine
{
	//class Texture;
	class Renderer;

	// use textureFactory instead? store refernece to renderer?
	// rename Image loader?
	// static or not?
	class TextureLoader
	{
	public:
		std::optional<Texture> loadTexture(Renderer& renderer, const std::filesystem::path& path);
	};
}