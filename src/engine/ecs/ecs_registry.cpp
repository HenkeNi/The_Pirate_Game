#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/entity/entity_handle.h"

// TODO; mark noexcept where possible

namespace cursed_engine
{
	EntityHandle ECSRegistry::createEntity()
	{
		auto entityOpt = m_entityManager.create();

		if (entityOpt.has_value())
		{
			return EntityHandle{ entityOpt.value(), this };
		}

		return EntityHandle(Entity{ INVALID_ENTITY_ID, 0 }, this);
	}

	bool ECSRegistry::destroyEntity(Entity entity)
	{
		if (m_entityManager.destroy(entity))
		{
			std::for_each(m_componentManagers.begin(), m_componentManagers.end(), 
				[=](auto& componentManager) 
				{ 
					componentManager->remove(entity.id); 
				});

			return true;
		}

		return false;
	}

	void ECSRegistry::destroyAllEntities()
	{
		m_entityManager.destroyAll();

		std::for_each(m_componentManagers.begin(), m_componentManagers.end(),
			[=](auto& componentManager)
			{
				componentManager->removeAll();
			});
	}

	std::optional<EntityHandle> ECSRegistry::FindEntityByName(std::string_view name)
	{
		return std::optional<EntityHandle>();
	}

	std::vector<EntityHandle> ECSRegistry::FindAllEntitiesWithTag(std::string_view tag)
	{
		return std::vector<EntityHandle>();
	}
}