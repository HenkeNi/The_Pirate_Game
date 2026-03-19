#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/system/input_system.h"
#include "engine/input/input_handler.h"

namespace cursed_engine
{
	InputSystem::InputSystem(InputHandler& inputHandler)
		: m_inputHandler{ inputHandler }
	{
	}

	void InputSystem::update(SystemContext& context)
	{
		context.eventBus;

		//m_inputHandler.

		// TODO; create input events!



		// m_eventBus.publishInstantly(MouseBtnPressedEvent{ (MouseButton)event.button.button });

		/*auto componentView = registry.view<InputComponent>();
		componentView.forEach([&](InputComponent& inputComponent)
			{
				auto mousePosition = m_inputHandler.getMousePosition();
				inputComponent.mousePosition.x = mousePosition.first;
				inputComponent.mousePosition.y = mousePosition.second;

				std::cout << inputComponent.mousePosition.x << " - " << inputComponent.mousePosition.y << "\n";
			});*/
	}
}