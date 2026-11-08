#pragma once
#include "engine/physics/physics_api.h"
#include "engine/physics/physics_debug_draw.h"
#include "engine/physics/physics_world.h"

namespace cursed_engine
{
	class Physics
	{
	public:
		bool init(PhysicsDebugDrawContext context);
		void shutdown();

		//void debugDraw();

		[[nodiscard]] inline PhysicsAPI getPhysicsAPI() noexcept { return PhysicsAPI{ &m_physicsDebugDraw }; }
		[[nodiscard]] inline PhysicsDebugDraw& getPhysicsDebugDraw() noexcept { return m_physicsDebugDraw; }

	private:
		PhysicsDebugDraw m_physicsDebugDraw;
	};
}