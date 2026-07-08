#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class TransformSystem : public System
	{
	public:
		void update(SystemContext& context) override;
	};
}