#include "engine/ecs/system/system_manager.h"

namespace cursed_engine
{
	void SystemManager::update(ECSRegistry& registry, float deltaTime)
	{
		for (auto& system : m_systems)
		{
			system->update(registry, deltaTime);
		}
	}
}