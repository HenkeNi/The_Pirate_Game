#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/events/event_bus.h"
#include "engine/input/input_handler.h"

namespace cursed_engine
{
	UISystem::UISystem(InputHandler& inputHandler, EventBus& eventBus)
		: m_inputHandler{ inputHandler }, m_eventBus{ eventBus }
	{
		//m_eventBus.subscribe<MouseBtnPressedEvent>(handleMouseBtnPressed);
		m_eventBus.subscribe<MouseBtnPressedEvent>([this](const MouseBtnPressedEvent& event)
			{
				handleMouseBtnPressed(event);
			});

		m_eventBus.subscribe<KeyPressedEvent>([this](const KeyPressedEvent& event)
			{
				handleKeyPressed(event);
			});
	}

	void UISystem::update(SystemContext& systemContext)
	{
		//handleButtonInteractions(systemContext.registry);
	}

	bool isInside(FVec2 min, FVec2 max, FVec2 point)
	{
		return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y;
	}

	void UISystem::handleButtonInteractions(ECSRegistry& registry)
	{
		FVec2 mousePosition = m_inputHandler.getMousePosition();

		auto view = registry.view<TransformComponent, ButtonComponent, BoundingBox>();
		view.forEach([&](TransformComponent& transformComponent, ButtonComponent& buttonComponent, BoundingBox& boundingBox)
			{
				FVec2 buttonPosition = transformComponent.position;
				buttonPosition.x += boundingBox.offset.x;
				buttonPosition.y += boundingBox.offset.y;

				// TODO; make sure max is larger than min!

				FVec2 max = buttonPosition + boundingBox.halfExtents;

				if (isInside(buttonPosition, max, mousePosition))
				{

					std::cout << "inside\n";
					
					auto mouseState = m_inputHandler.getMouseInputState(MouseButton::Left);

					if (mouseState == InputState::Pressed || m_inputHandler.isMouseBtnPressed(MouseButton::Left))
					{
						int x = 20;
						std::cout << "Pressed !!!";

					}
					else
					{
						// just hovered...
					}

				}
				else
					std::cout << "\n";

			});
	}

	void UISystem::handleMouseBtnPressed(const MouseBtnPressedEvent& event)
	{
		// OR polls eevnt itself in update loop...
		// store evnets?

		int x = 20;
	}
	void UISystem::handleKeyPressed(const KeyPressedEvent& event)
	{
		int x = 20;
	}
}