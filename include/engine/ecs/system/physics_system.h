#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class PhysicsWorld;

	// listen to component added (physics component) or entity created?
	class PhysicsSystem : public UpdateSystem
	{
	public:
		void update(SystemUpdateContext& context) override;

		void setPhysicsWorld(PhysicsWorld* world);

	private:
		PhysicsWorld* m_physicsWorld;
	};
}