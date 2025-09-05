#include "engine/input/input_handler.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	void InputHandler::processInput(SDL_Event* inputEvent)
	{
		if (inputEvent->type == SDL_EVENT_KEY_DOWN)
		{
			if (inputEvent->key.key == SDLK_W)
				std::cout << "UP";
		}
	}

	bool InputHandler::isKeyPressed() const
	{
		return false;
	}

	bool InputHandler::isKeyHeld() const
	{
		return false;
	}
}