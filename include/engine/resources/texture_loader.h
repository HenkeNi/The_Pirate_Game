#pragma once
#include <filesystem>
#include <memory>
//#include "engine/rendering/texture.h"

namespace cursed_engine
{
	class Texture;
	class Renderer;

	class TextureLoader
	{
	public:
		[[nodiscard]] std::unique_ptr<Texture> load(Renderer& renderer, const std::filesystem::path& path);
	};
}