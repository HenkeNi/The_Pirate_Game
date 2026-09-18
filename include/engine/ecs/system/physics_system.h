#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	// listen to component added (physics component) or entity created?
	class PhysicsSystem : public System
	{
	public:
		void update(SystemContext& context) override;

	private:
	};
}