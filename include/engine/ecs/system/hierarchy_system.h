#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	// listen for entity created? destroyed? remoev component<HeiarachyComppnent>?

	//class HierarchyManager
	//{
	//public:
	//	// getRoots();

	//	// getChildren(Parent)

	//	// 

	//};

	// extend entity handle with functions??

	class EntityHandle;
	struct TransformComponent;

	class HierarchySystem : public System
	{
	public:
		void update(cursed_engine::SystemContext& context) override;
		//void update()
		//{
		//	// listen to entity created.... -> need to use string id...
		//}

	private:
		void updateTransform(EntityHandle& handle, const TransformComponent& parentTransformComponent); // or pass metrix or osmething...
	};
}