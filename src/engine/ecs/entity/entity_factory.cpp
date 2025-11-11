#include "engine/ecs/entity/entity_factory.h"

namespace cursed_engine
{
	EntityFactory::EntityFactory(ECSRegistry& registry)
		: m_registry{ registry }
	{
	}

	std::optional<EntityHandle> EntityFactory::createFromPrefab(std::string_view prefab)
	{
		return std::nullopt;
	}
}