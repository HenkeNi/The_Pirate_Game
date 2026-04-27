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
		// need to pull events, doesnt have access to registry class otherwise...

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

		auto view = registry.view<TransformComponent, ButtonComponent, BoundingBox>();
		view.forEach([&](TransformComponent& transformComponent, ButtonComponent& buttonComponent, BoundingBox& boundingBox)
			{
				buttonComponent.previousState = buttonComponent.currentState;

				// TODO, make into function? in component? static somewhere??
				FVec2 buttonPosition = transformComponent.position + boundingBox.offset;		
				FVec2 size = boundingBox.halfSize * 2.f;
				
				// TODO; make sure max is larger than min!

				if (isInside(buttonPosition, buttonPosition + size, mousePosition))
				{
					// check current button state... call on state changed? (pass in state?) or just onHovered
					// onButtonStateChanged();

					std::cout << "inside\n";

					buttonComponent.currentState = ButtonComponent::State::Hovered;

					bool mouseReleased = m_inputHandler.isMouseBtnReleased(MouseButton::Left);

					// TODO; differentiate between enter hover and hovering..? (store current state)

					if (mouseReleased)
					{
						int x = 20;
					}

					bool mousePressed = m_inputHandler.isMouseBtnPressed(MouseButton::Left);
					if (mousePressed)
					{
						buttonComponent.currentState = ButtonComponent::State::Pressed;
						int x = 20;
					}

					//std::cout << "Is presed: " << mouseReleased << "\n";

					auto mouseState = m_inputHandler.getMouseInputState(MouseButton::Left);

					if (mouseState != InputState::None)
					{
						int x = 20;
					}

					////if (mouseState == InputState::Pressed || m_inputHandler.isMouseBtnPressed(MouseButton::Left))
					//if (m_inputHandler.isMouseBtnPressed(MouseButton::Left))
					//{
					//	int x = 20;
					//	std::cout << "Pressed !!!";

					//}
					//else
					//{
						// just hovered...
					//}

					// check if having hover color,pressed color
					if (buttonComponent.hoverColor.has_value())
					{

					}

					if (buttonComponent.pressedColor.has_value())
					{

					}

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