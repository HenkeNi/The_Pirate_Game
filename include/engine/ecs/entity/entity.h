#pragma once
#include "engine/ecs/ecs_types.h"

// Consdier moviong Entity to ecs_types.h

namespace cursed_engine
{
	struct Entity
	{
		EntityID id = INVALID_ENTITY_ID;
		EntityVersion version = 0;

		bool operator==(const Entity& other) const { return id == other.id && version == other.version; }
		bool isValid() const { return id != INVALID_ENTITY_ID; }

	};

	// TODO; add hash!
}