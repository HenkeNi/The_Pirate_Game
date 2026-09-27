#include "engine/ecs/system/physics_system.h"
#include "engine/core/events/event_bus.h"
#include "engine/core/events/events.h"
#include "engine/ecs/component/core_components.h"

namespace cursed_engine
{	
	void PhysicsSystem::update(SystemUpdateContext& context)
	{
		if (!m_physicsWorld)
			return;
		//assert(m_physicsWorld && "PhysicsWorld is nullptr!");

		{
			auto componentView = context.registry.view<VelocityComponent, PhysicsComponent>();
			componentView.forEach([](const VelocityComponent& velocityComponent, PhysicsComponent& physicsComponent)
				{
					physicsComponent.physicsBody.setLinearVelocity(velocityComponent.velocity * 300); // or calculate speed / velocity in movemnt system?
				});
		}

		m_physicsWorld->step();

		// update transforms...
		{
			auto componentView = context.registry.view<TransformComponent, PhysicsComponent>();
			componentView.forEach([](TransformComponent& transformComponent, const PhysicsComponent& physicsComponent)
				{
					transformComponent.position = physicsComponent.physicsBody.getPosition();
				});
		}
	}

	void PhysicsSystem::setPhysicsWorld(PhysicsWorld* world)
	{
		m_physicsWorld = world;
	}
}