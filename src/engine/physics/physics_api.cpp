#include "engine/physics/physics_api.h"
#include "engine/physics/physics_debug_draw.h"

namespace cursed_engine
{
	PhysicsAPI::PhysicsAPI(PhysicsDebugDraw* physicsDebugDraw)
		: m_physicsDebugDraw{ physicsDebugDraw }
	{
	}

	void PhysicsAPI::setDebugDrawEnabled(bool enabled)
	{
		m_physicsDebugDraw->setEnabled(enabled);
	}

	void PhysicsAPI::setWorldId(WorldId worldId)
	{
		m_physicsDebugDraw->setWorldId(worldId);
	}

	constexpr bool PhysicsAPI::isDebugDrawEnabled() const noexcept
	{
		return m_physicsDebugDraw->isEnabled();
	}
}