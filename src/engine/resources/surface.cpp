#include "engine/resources/surface.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_surface.h>

namespace cursed_engine
{
	Surface::~Surface()
	{
		SDL_DestroySurface(surface);
	}

	Surface SurfaceLoader::operator()(const std::filesystem::path& path) const
	{
		auto* surface = SDL_LoadBMP(path.string().c_str());
		if (!surface)
		{
			Logger::logError(std::format("Failed to load window icon, path: {}", path.string()));
			return Surface{ nullptr };
		}

		return Surface{ surface };
	}
}