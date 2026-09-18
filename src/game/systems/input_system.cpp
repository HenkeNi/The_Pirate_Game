#include "game/systems/input_system.h"
#include "game/components/components.h"
#include <engine/ecs/ecs_registry.h>
#include <engine/platform/Input_api.h>
#include <engine/platform/input.h> // include both? move key to input_types? platform_types?

InputSystem::InputSystem(cursed_engine::InputAPI input)
	: m_input{ input }
{
}

void InputSystem::update(cursed_engine::SystemContext& context)
{
	auto componentView = context.registry.view<InputComponent>();
	componentView.forEach([&](InputComponent& inputComponent)
		{
			inputComponent.up = m_input.isKeyHeld(cursed_engine::Key::W);
			inputComponent.down = m_input.isKeyHeld(cursed_engine::Key::S);
			inputComponent.left = m_input.isKeyHeld(cursed_engine::Key::A);
			inputComponent.right = m_input.isKeyHeld(cursed_engine::Key::D);

			inputComponent.interact = m_input.isKeyHeld(cursed_engine::Key::E);
		});

}