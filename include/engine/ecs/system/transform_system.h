#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	struct WorldTransformComponent;
	class EntityHandle;

	class TransformSystem : public UpdateSystem
	{
	public:
		void update(SystemUpdateContext& context) override;
	
	private:
		void updateTransform(EntityHandle& handle, const WorldTransformComponent& parentTransformComponent);
	};
}