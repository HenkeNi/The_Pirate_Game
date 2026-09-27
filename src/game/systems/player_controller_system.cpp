#include "game/systems/player_controller_system.h"
#include "game/components/components.h"
#include <engine/ecs/ecs_registry.h>
#include <engine/ecs/component/core_components.h>

PlayerControllerSystem::PlayerControllerSystem()
{
}

void PlayerControllerSystem::update(cursed_engine::SystemUpdateContext& context)
{
	auto componentView = context.registry.view<InputComponent, cursed_engine::VelocityComponent>();

	componentView.forEach([&](InputComponent& inputComponent, cursed_engine::VelocityComponent& velocityComponent)
		{
			float verticalVelocity = 0.f;

			if (inputComponent.up)
				verticalVelocity -= 1.0f;
			if (inputComponent.down)
				verticalVelocity += 1.0f;

			float horizontalVelocity = 0.f;

			if (inputComponent.left)
				horizontalVelocity -= 1.0f;
			if (inputComponent.right)
				horizontalVelocity += 1.0f;

			velocityComponent.velocity.x = horizontalVelocity;
			velocityComponent.velocity.y = verticalVelocity;


			if (inputComponent.interact)
			{

			}



			// clmap veloicty? up + left...
		});
}