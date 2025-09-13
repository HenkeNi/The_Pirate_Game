#pragma once
#include "action.h"
#include <unordered_map>
#include <SDL3/SDL.h>

namespace cursed_engine
{
	// TODO; forward declare SDL_Scancode?

	struct InputConfig
	{
		using KeyBindings = std::unordered_map<SDL_Scancode, Action>;

		InputConfig() = default;

		InputConfig(const KeyBindings& bindings, float sensitivity)
			: keyBindings{ bindings }, mouseSensitivity{ sensitivity }
		{
		}

		KeyBindings keyBindings;
		float mouseSensitivity = 1.f;
	};
}