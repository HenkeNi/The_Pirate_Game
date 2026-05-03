#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/events/event_bus.h"
#include "engine/events/events.h"
#include "engine/input/input_handler.h"
#include "engine/action/action_registry.h"

namespace cursed_engine
{
	UISystem::UISystem(InputHandler& inputHandler, ActionRegistry& actionRegistry)
		: m_inputHandler{ inputHandler }, m_actionRegistry{ actionRegistry }
	{
		// need to pull events, doesnt have access to registry class otherwise...

		//m_eventBus.subscribe<MouseBtnPressedEvent>(handleMouseBtnPressed);
		/*m_eventBus.subscribe<MouseBtnPressedEvent>([this](const MouseBtnPressedEvent& event)
			{
				handleMouseBtnPressed(event);
			});

		m_eventBus.subscribe<KeyPressedEvent>([this](const KeyPressedEvent& event)
			{
				handleKeyPressed(event);
			});*/
	}

	void UISystem::update(SystemContext& systemContext)
	{
		handleButtonInteractions(systemContext.registry); // here or in an interaction system? might make more sense here?
	}

	bool isInside(FVec2 min, FVec2 max, FVec2 point)
	{
		return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y;
	}

	void UISystem::handleButtonInteractions(ECSRegistry& registry)
	{
		FVec2 mousePosition = m_inputHandler.getMousePosition();

		auto view = registry.view<TransformComponent, ButtonComponent, BoundingBoxComponent>();
		view.forEach([&](Entity entity, TransformComponent& transformComponent, ButtonComponent& buttonComponent, BoundingBoxComponent& boundingBoxComponent)
			{
				buttonComponent.previousState = buttonComponent.currentState;

				// TODO, make into function? in component? static somewhere??
				FVec2 buttonPosition = transformComponent.position + boundingBoxComponent.offset;		
				FVec2 size = boundingBoxComponent.halfSize * 2.f;
				
				// TODO; make sure max is larger than min!

				if (isInside(buttonPosition, buttonPosition + size, mousePosition))
				{
					switch (m_inputHandler.getMouseInputState(MouseButton::Left))
					{
					case InputState::None:
						buttonComponent.currentState = ButtonComponent::State::Hovered;
						break;
					case InputState::Pressed:
						buttonComponent.currentState = ButtonComponent::State::Pressed;
						break;
					case InputState::Released:
						m_actionRegistry.execute("NewGame", entity);
						buttonComponent.currentState = ButtonComponent::State::Hovered;
						break;
					}


					// check if having hover color,pressed color
					//if (buttonComponent.hoverColor.has_value())
					//{
					//}

					//if (buttonComponent.pressedColor.has_value())
					//{
					//}

				}
				else
				{
					buttonComponent.currentState = ButtonComponent::State::Normal;
				}
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