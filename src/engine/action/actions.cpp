#include "engine/action/actions.h"
#include "engine/action/action_registry.h"
#include "engine/events/event_bus.h"
#include "engine/events/events.h"

namespace cursed_engine
{
	namespace core_actions
	{
		void registerAll(ActionRegistry& actionRegistry, EventBus& eventBus)
		{
			actionRegistry.registerAction("NewGame", 
				[&](Entity entity) 
				{
					eventBus.publishInstantly(SceneChangeEvent{ "GameScene" }); // should game know about scenes?
					eventBus.publishInstantly(PlaySoundEvent{ "ButtonClick" });
				});
		}
	}
}

