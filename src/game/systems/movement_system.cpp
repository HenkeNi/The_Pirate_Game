#include "game/systems/movement_system.h"
#include <engine/ecs/component/core_components.h>
#include <engine/ecs/ecs_registry.h>

void MovementSystem::update(cursed_engine::SystemContext& context)
{
	auto componentView = context.registry.view<cursed_engine::TransformComponent, cursed_engine::VelocityComponent>();
	componentView.forEach([&](cursed_engine::TransformComponent& transformComponent, cursed_engine::VelocityComponent& velocityComponent)
		{
			auto& position = transformComponent.position;
			position.x += velocityComponent.velocity.x * context.deltaTime * 300;
			position.y += velocityComponent.velocity.y * context.deltaTime * 300;
		});
}
