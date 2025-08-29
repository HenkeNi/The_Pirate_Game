#pragma once
#include <string>

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
}