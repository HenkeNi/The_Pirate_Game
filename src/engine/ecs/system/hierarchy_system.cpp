#include "engine/ecs/system/hierarchy_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"

namespace cursed_engine
{
	void HierarchySystem::update(cursed_engine::SystemContext& context)
	{
		auto componentView = context.registry.view<HierarchyComponent>();
		componentView.forEach([&](Entity entity, HierarchyComponent& hierarchyComponent)
			{
				auto& child = hierarchyComponent.firstChild;
				auto& parent = hierarchyComponent.parent;

				if (child.isValid() && !parent.isValid())
				{
					const auto& parentTransformComponent = context.registry.getComponent<TransformComponent>(entity);
					updateTransform(child, parentTransformComponent);
				}
			});
	}

	void HierarchySystem::updateTransform(EntityHandle& handle, const TransformComponent& parentTransformComponent)
	{
		auto& transformComponent = handle.getComponent<TransformComponent>();
		auto& hierarchyComponent = handle.getComponent<HierarchyComponent>();


		transformComponent.position = parentTransformComponent.position; // add offset!!!!!!

		EntityHandle& child = hierarchyComponent.firstChild;
		
		if (child.isValid())
		{
			child = hierarchyComponent.nextSibling;
			updateTransform(child, parentTransformComponent); // dont pass in parent world, instead use transform of current child
		}
		else if (hierarchyComponent.nextSibling.isValid())
		{
			child = hierarchyComponent.nextSibling;
			updateTransform(child, parentTransformComponent);
		}

		//for (EntityHandle& child = hierarchyComponent.firstChild; child.isValid(); child = hierarchyComponent.nextSibling)
		//{
		//	updateTransform(child, parentTransformComponent); // dont pass in parent world, instead use transform of current child
		//}
	}
}