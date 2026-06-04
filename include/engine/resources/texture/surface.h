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
}