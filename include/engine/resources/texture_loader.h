#pragma once
#include <filesystem>
#include <memory>

namespace cursed_engine
{
	class Texture;
	class Renderer;

	class TextureLoader
	{
	public:
		[[nodiscard]] std::unique_ptr<Texture> load(Renderer& renderer, const std::filesystem::path& path) const;
	};
}