#include "engine/action/action_registry.h"
#include "engine/ecs/entity/entity.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	void ActionRegistry::execute(const ActionID& id, Entity e)
	{
		assert(isRegistered(id) && "Action is not registered!");

		if (auto it = m_actions.find(id); it != m_actions.end())
		{
			it->second(e);
		}
		else
		{
			Logger::logError("Failed to find action with id: " + id);
		}
	}

	bool ActionRegistry::isRegistered(const ActionID& id) const noexcept
	{
		return m_actions.contains(id);
	}
}