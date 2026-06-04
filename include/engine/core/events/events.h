#pragma once
#include "engine/platform/input/input_handler.h"
#include "engine/ecs/entity/entity.h"
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
		Key key;
	};

	struct KeyReleasedEvent
	{
		Key key;
	};

	struct WindowResizeEvent
	{

	};

	struct PlaySoundEvent
	{
		std::string sound;
		// entity?
	};
}