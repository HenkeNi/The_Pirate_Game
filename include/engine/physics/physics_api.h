#pragma once
#include "engine/physics/physics_types.h"

namespace cursed_engine
{
	class PhysicsDebugDraw;

	// redundant class? add to config instead? (or something)
	class PhysicsAPI
	{
	public:
		PhysicsAPI(PhysicsDebugDraw* physicsDebugDraw = nullptr);

		void setDebugDrawEnabled(bool enabled);
		void setWorldId(WorldId worldId);

		[[nodiscard]] constexpr bool isDebugDrawEnabled() const noexcept;

		// [[nodiscard]] PhysicsWorld createWorld() const;

		//	[[nodiscard]] bool isValidWorld(PhysicsWorld world) const;

	private:
		PhysicsDebugDraw* m_physicsDebugDraw;
	};


}