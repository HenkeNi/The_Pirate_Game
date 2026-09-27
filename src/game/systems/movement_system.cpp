#include "game/systems/movement_system.h"
#include <engine/ecs/component/core_components.h>
#include <engine/ecs/ecs_registry.h>

void MovementSystem::update(ce::SystemUpdateContext& context)
{
	// TODO; handle both entiteis with physics compoent and without...

	auto componentView = context.registry.view<ce::TransformComponent, ce::VelocityComponent>();
	componentView.forEach([&](ce::TransformComponent& transformComponent, const ce::VelocityComponent& velocityComponent)
		{
			//auto& position = transformComponent.position;
			//position.x += velocityComponent.velocity.x * context.deltaTime * 300;
			//position.y += velocityComponent.velocity.y * context.deltaTime * 300;
			
			//position.x += velocityComponent.velocity.x * context.deltaTime * 300;
			//position.y += velocityComponent.velocity.y * context.deltaTime * 300;
		});
}
