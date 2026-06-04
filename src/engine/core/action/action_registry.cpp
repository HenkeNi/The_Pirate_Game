#include "engine/core/action/action_registry.h"
#include "engine/ecs/entity/entity.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	void ActionRegistry::execute(const ActionID& id, const ActionArgs& args)
	{
		assert(isRegistered(id) && "Action is not registered!");

		if (auto it = m_actions.find(id); it != m_actions.end())
		{
			it->second(args);
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