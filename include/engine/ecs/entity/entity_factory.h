#pragma once
#include "entity_handle.h"
#include <optional>

namespace cursed_engine
{
	//class EntityHandle;
	class ECSRegistry;

	class EntityFactory
	{
	public:
		EntityFactory(ECSRegistry& registry);

		std::optional<EntityHandle> createFromPrefab(std::string_view prefab);

	private:
		ECSRegistry& m_registry;
	};
}