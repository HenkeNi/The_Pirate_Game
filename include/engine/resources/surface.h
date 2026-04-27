#pragma once
#include <filesystem>

struct SDL_Surface;

namespace cursed_engine
{
	struct Surface
	{
		~Surface();

		SDL_Surface* surface = nullptr;
	};

	struct SurfaceLoader
	{
		[[nodiscard]] Surface operator()(const std::filesystem::path& path) const;
	};
}