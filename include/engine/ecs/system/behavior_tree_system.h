#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class BehaviorTreeSystem : public UpdateSystem
	{
	public:
		void update(SystemUpdateContext& context) override;
	};
}