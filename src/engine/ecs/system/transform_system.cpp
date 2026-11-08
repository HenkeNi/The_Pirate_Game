#include "engine/ecs/system/transform_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/entity/entity_handle.h"

namespace cursed_engine
{
	//void TransformSystem::update(SystemUpdateContext& context)
	//{
	//	// need to update parent first, then child...
	//	auto componentView = context.registry.view<HierarchyComponent>();
	//	componentView.forEach([&](HierarchyComponent& hierarchyComponent)
	//		{
	//			// want all root nodes.. (maybe with WorldTransfromComponent)
	//			// all with local transforms
	//			// - need to know if root or not, if player goes on boat, then it needs to swap to a local transform?

	//			
	//			// create a tree?
	//			
	//			auto parentHandle = hierarchyComponent.parent;
	//			if (!parentHandle.isValid())
	//				return;

	//			// if root...
	//			if (!parentHandle.hasComponents<HierarchyComponent>())
	//			{
	//				// updaet position...



	//			}

	//		});
	//}

	void TransformSystem::update(cursed_engine::SystemUpdateContext& context)
	{
		// update all entities... (if heirarchy component, check if has parent)...
		// Update parents... 

		// need to make sure entities are updated in correct order: root => entity1 => entity2 => entity3 
		// not good enough to just update 


		auto componentView = context.registry.view<LocalTransformComponent, WorldTransformComponent>();
		componentView.forEach([&](Entity entity, const LocalTransformComponent& localTransformComponent, WorldTransformComponent& worldTransformComponent)
			{
				// If not a child then world transform == local transform...

				const HierarchyComponent* hierarchyComponent = context.registry.tryGetComponent<HierarchyComponent>(entity);
				bool isRoot = hierarchyComponent && !hierarchyComponent->parent.isValid();
				if (!hierarchyComponent || isRoot)
				{
					worldTransformComponent.position = localTransformComponent.position;
					worldTransformComponent.scale = localTransformComponent.scale;
					worldTransformComponent.rotation = localTransformComponent.rotation;

					if (isRoot)
					{
						// assert child is valid?
						
						// traverse children... (since no parent -> no siblings?)
						EntityHandle firstChild = hierarchyComponent->firstChild;
						if (firstChild.isValid())
						{
							updateTransform(firstChild, worldTransformComponent);
						}
					
					}
				}
				

			});
	}

	void TransformSystem::updateTransform(EntityHandle& handle, const WorldTransformComponent& parentTransformComponent)
	{
		auto& localTransformComponent = handle.getComponent<LocalTransformComponent>();
		auto& worldTransformComponent = handle.getComponent<WorldTransformComponent>();
		auto& hierarchyComponent = handle.getComponent<HierarchyComponent>();

		// Child: World transform == local transform + parent World transform
		//worldTransformComponent.position = localTransformComponent.position + parentTransformComponent.position;
		worldTransformComponent.position = parentTransformComponent.position + 
			localTransformComponent.position.rotated(parentTransformComponent.rotation) * parentTransformComponent.scale;

		worldTransformComponent.rotation = parentTransformComponent.rotation + localTransformComponent.rotation;
		worldTransformComponent.scale = parentTransformComponent.scale * localTransformComponent.scale;

		EntityHandle& child = hierarchyComponent.firstChild;

		if (child.isValid())
		{
			child = hierarchyComponent.nextSibling;
			updateTransform(child, worldTransformComponent); 
		}
		else if (hierarchyComponent.nextSibling.isValid())
		{
			child = hierarchyComponent.nextSibling;
			updateTransform(child, parentTransformComponent);
		}
	}
}