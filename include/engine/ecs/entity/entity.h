#pragma once
#include "engine/ecs/ecs_types.h"
#include "engine/utils/utils.h"

// Consdier moviong Entity to ecs_types.h

namespace cursed_engine
{
	struct Entity
	{
		EntityID id = INVALID_ENTITY_ID;
		EntityVersion version = INVALID_ENTITY_VERSION; // rename generation?

		bool operator==(const Entity& other) const { return id == other.id && version == other.version; }
		bool isValid() const { return id != INVALID_ENTITY_ID && version != INVALID_ENTITY_VERSION; }
	};
}

template<>
struct std::hash<cursed_engine::Entity>
{
	std::size_t operator()(const cursed_engine::Entity& entity) const noexcept
	{
		using namespace cursed_engine::utils::hash;

		std::size_t h = 0;

		hashCombine(h, entity.id);
		hashCombine(h, entity.version);

		return h;
	}
};