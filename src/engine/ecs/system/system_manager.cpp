#include "engine/ecs/system/system_manager.h"

namespace cursed_engine
{
	void SystemManager::update(SystemContext& context)
	{
		for (auto& system : m_systems)
		{
			system->update(context);
		}
	}
}