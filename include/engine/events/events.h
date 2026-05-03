#pragma once
#include "engine/ecs/entity/entity.h"
#include "engine/input/input_handler.h"
#include <string>
// TODO; create dedicated UIEvents, InputEvents, etc if file grows to large

namespace cursed_engine
{
	struct ButtonPressedEvent
	{
		Entity entity;
	};

	struct MouseBtnPressedEvent // Shared events mouse btn and key?
	{
		MouseButton button;
	};

	struct MouseBtnReleasedEvent
	{
		MouseButton button;
	};

	struct KeyPressedEvent
	{
		SDL_Scancode key;
	};

	struct KeyReleasedEvent
	{
		SDL_Scancode key;
	};

	struct WindowResizeEvent
	{

	};

	// Should engine know about scenes? 
	struct SceneChangeEvent 
	{
		std::string scene;
	};

	struct PlaySoundEvent
	{
		std::string sound;
		// entity?
	};
}