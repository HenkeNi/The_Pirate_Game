#include "engine/ecs/system/transform_system.h"
#include "engine/ecs/component/core_components.h"

namespace cursed_engine
{
	void TransformSystem::update(SystemContext& context)
	{
		// need to update parent first, then child...
		auto componentView = context.registry.view<ParentComponent>();
		componentView.forEach([&](ParentComponent& parentComponent)
			{
				// want all root nodes.. (maybe with WorldTransfromComponent)
				// all with local transforms
				// - need to know if root or not, if player goes on boat, then it needs to swap to a local transform?

				
				// create a tree?
				
				auto parentHandle = parentComponent.parent;
				if (!parentHandle.isValid())
					return;

				// if root...
				if (!parentHandle.hasComponents<ParentComponent>())
				{
					// updaet position...



				}

			});
	}
}