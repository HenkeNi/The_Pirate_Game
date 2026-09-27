#pragma once
#include "engine/platform/input.h"
//#include "engine/ecs/entity/entity.h"
#include "engine/ecs/entity/entity_handle.h"
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

	struct SettingsChangedEvent
	{

	};

	struct EntityCreatedEvent
	{
		Entity entity;
	};

	// GOOD or bad idea?
	struct EntitiesCreatedEvent
	{
		std::vector<Entity> entities;
	};
}