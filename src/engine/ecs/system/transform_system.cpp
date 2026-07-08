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
				// create a tree?
				auto parentHandle = parentComponent.parent;
				if (!parentHandle.isValid())
					return;

				

			});
	}
}