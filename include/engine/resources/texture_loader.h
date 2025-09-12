#pragma once
#include <filesystem>
#include "engine/rendering/texture.h"

struct SDL_Surface;

namespace cursed_engine
{
	//class Texture;
	class Renderer;

	// use textureFactory instead? store refernece to renderer?
	// store all loaders in same file?

	// TODO; try to just forward declare!
	// TODO; make static?
	class TextureLoader
	{
	public:
		// Change name to just 'load'?
		[[nodiscard]] std::unique_ptr<Texture> loadTexture(Renderer& renderer, const std::filesystem::path& path);
	};
}