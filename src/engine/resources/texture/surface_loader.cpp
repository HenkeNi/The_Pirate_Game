#include "engine/resources/texture/surface_loader.h"
#include "engine/resources/texture/surface.h"
#include "engine/core/logger.h"
#include <SDL3_image/SDL_image.h>

namespace cursed_engine
{
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