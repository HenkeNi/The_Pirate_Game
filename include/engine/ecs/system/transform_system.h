#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class TransformSystem : public UpdateSystem
	{
	public:
		void update(SystemUpdateContext& context) override;
	};
}