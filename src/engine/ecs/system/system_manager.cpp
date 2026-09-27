#include "engine/ecs/system/system_manager.h"

namespace cursed_engine
{
	void SystemManager::update(SystemUpdateContext& context)
	{
		for (auto& system : m_updateSystems)
		{
			system->update(context);
		}
	}

	void SystemManager::render(SystemRenderContext& context)
	{
		for (auto& system : m_renderSystems)
		{
			system->render(context);
		}
	}

	void SystemManager::clear()
	{
		m_updateSystems.clear();
		m_renderSystems.clear();
	}
}