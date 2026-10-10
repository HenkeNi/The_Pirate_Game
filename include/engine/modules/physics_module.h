#pragma once
#include "engine/physics/physics.h"
#include "engine/physics/physics_debug_draw.h"

namespace cursed_engine
{
	class PhysicsModule
	{
	public:
		bool init(PhysicsDebugDrawContext context);
		void shutdown();

		//void debugDraw();

		[[nodiscard]] inline PhysicsAPI getPhysicsAPI() noexcept { return PhysicsAPI{ &m_physicsDebugDraw }; }
		[[nodiscard]] inline PhysicsDebugDraw& getPhysicsDebugDraw() noexcept { return m_physicsDebugDraw; }

	private:
		//PhysicsAPI m_physics; // create on the fly??
		PhysicsDebugDraw m_physicsDebugDraw;
	};
}