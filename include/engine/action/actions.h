#pragma once

namespace cursed_engine
{
	class ActionRegistry;
	class EventBus;

	namespace core_actions
	{
		void registerAll(ActionRegistry& actionRegistry, EventBus& eventBus);
	}
}