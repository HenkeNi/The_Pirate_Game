#pragma once
#include <string>
#include "engine/input/action.h"
#include <unordered_map>
#include <SDL3/SDL.h>

namespace cursed_engine
{
    struct WindowConfig
    {
        std::string title = "The Cursed Pirate";
        int width = 1280;
        int height = 720;
        bool fullscreen = false;
        bool vsync = true;
    };

	// shouldnt know about actions??
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

	struct AudioConfig
	{
		float masterVolume = 1.f;
		float musicVolume = 0.8f;
		float sfxVolume = 0.9f;
	};

	struct RenderConfig
	{
		float brightness = 1.f;
		float contrast = 1.f;
	};

	struct EngineConfig
	{
		WindowConfig windowConfig;
		InputConfig inputConfig;
		AudioConfig audioConfig;
		RenderConfig renderConfig;
	};
}