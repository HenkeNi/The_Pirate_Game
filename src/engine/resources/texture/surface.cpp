#include "engine/resources/texture/surface.h"
#include <SDL3/SDL_surface.h>

namespace cursed_engine
{
	Surface::~Surface()
	{
		SDL_DestroySurface(surface);
	}
}