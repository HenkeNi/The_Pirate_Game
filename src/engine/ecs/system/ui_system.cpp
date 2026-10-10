#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/core/events/event_bus.h"
#include "engine/core/events/events.h"
#include "engine/core/action/action_registry.h"
#include "engine/platform/input_api.h"
#include "engine/math/vec2.hpp"
// TODO; move boundingbox checks to an interaction system?

namespace cursed_engine
{
	bool isInside(FVec2 min, FVec2 max, FVec2 point)
	{
		return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y;
	}

	UISystem::UISystem(InputAPI input, ActionRegistry* actionRegistry)
		: m_input{ input }, m_actionRegistry{ actionRegistry }
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

	void UISystem::update(SystemUpdateContext& systemContext)
	{
		handleButtonInteractions(systemContext.registry); // here or in an interaction system? might make more sense here?
		handleCheckboxInteractions(systemContext.registry);
		handleSlidersInteractions(systemContext.registry);
	}

	// TODO; pass mouse pos to each function?
	void UISystem::handleButtonInteractions(ECSRegistry& registry)
	{
		FVec2 mousePosition = m_input.getMousePosition();

		auto view = registry.view<TransformComponent, ButtonComponent, BoundingBoxComponent>();
		view.forEach([&](Entity entity, TransformComponent& transformComponent, ButtonComponent& buttonComponent, BoundingBoxComponent& boundingBoxComponent)
			{
				buttonComponent.previousState = buttonComponent.currentState;


				// TODO, make into function? in component? static somewhere??
				/*FVec2 buttonPosition = transformComponent.position + boundingBoxComponent.offset;
				FVec2 size = boundingBoxComponent.halfSize * 2.f;
				buttonPosition -= size * transformComponent.pivot;*/

				auto& spriteComponent = registry.getComponent<SpriteComponent>(entity);

				// TODO; make sure max is larger than min! use intersections...
				bool isInside = isMouseInsideBoundingBox(transformComponent, boundingBoxComponent, mousePosition.x, mousePosition.y);

				if (isInside)
				{

					switch (m_input.getMouseInputState(MouseButton::Left))
					{
					case InputState::None:
						buttonComponent.currentState = ButtonComponent::State::Hovered; // func? handleButtonHoverState

						if (buttonComponent.hoverColor.has_value())
							spriteComponent.color = buttonComponent.hoverColor.value();

						break;

					case InputState::Pressed:
					{
						buttonComponent.currentState = ButtonComponent::State::Pressed;
						if (buttonComponent.pressedColor.has_value())
							spriteComponent.color = buttonComponent.pressedColor.value();

						break;
					}
					case InputState::Released:
						//m_actionRegistry.execute(buttonComponent.action, entity);
						m_actionRegistry->execute(buttonComponent.action, buttonComponent.actionArgs);
						buttonComponent.currentState = ButtonComponent::State::Hovered;
						// spriteComponent.color = buttonComponent.hoverColor;

						break;
					}
				}
				else
				{
					buttonComponent.currentState = ButtonComponent::State::Normal; // or just set before if?
					spriteComponent.color = buttonComponent.defaultColor;
				}
			});
	}

	void UISystem::handleCheckboxInteractions(ECSRegistry& registry)
	{
		// TODO; always set correct region?

		// pass in mouse pos instead?
		FVec2 mousePosition = m_input.getMousePosition();

		// TODO; handle bounding box in physics or collision system?
		auto view = registry.view<TransformComponent, CheckboxComponent, BoundingBoxComponent>();
		view.forEach([&](Entity entity, TransformComponent& transformComponent, CheckboxComponent& checkboxComponent, BoundingBoxComponent& boundingBoxComponent)
			{
				bool isInside = isMouseInsideBoundingBox(transformComponent, boundingBoxComponent, mousePosition.x, mousePosition.y);

				if (isInside)
				{
					switch (m_input.getMouseInputState(MouseButton::Left))
					{
					case InputState::Released:
						checkboxComponent.isChecked = !checkboxComponent.isChecked;

						if (auto* spriteComponent = registry.tryGetComponent<SpriteComponent>(entity))
						{
							spriteComponent->atlasRegion = checkboxComponent.isChecked ? 
								checkboxComponent.checkedRegion : checkboxComponent.uncheckedRegion;
						}
						break;
					}
				}
			});
	}

	void UISystem::handleSlidersInteractions(ECSRegistry& registry)
	{
		// TODO; get all entities with a parent component, where parent has slider component?

		auto view = registry.view<SliderComponent>();
		view.forEach([&](Entity entity, SliderComponent& sliderComponent) 
			{				
				// "Thumb"...
				//if (registry.hasComponents<HierarchyComponent>(entity))
				if (auto* hierarchyComponent = registry.tryGetComponent<HierarchyComponent>(entity))
				{
					int x = 20; 
					auto& thumb = hierarchyComponent->firstChild;

					// if thumb has been pressed (check if true) && input == held (or listen to release)

				}
			});
	}

	/*void UISystem::updateButtonColor(ButtonComponent::State buttonState, SpriteComponent& spriteComponent)
	{
		if (buttonState == ButtonComponent::State::Normal)
			spriteComponent.color = but

		switch (buttonState)
		{
		case InputState::None:
			buttonComponent.currentState = ButtonComponent::State::Hovered;
			break;
		case InputState::Pressed:
			buttonComponent.currentState = ButtonComponent::State::Pressed;
			break;
		case InputState::Released:
		}
	}*/

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

	bool UISystem::isMouseInsideBoundingBox(TransformComponent& transformComponent, BoundingBoxComponent& boundingBoxComponent, float mousePosX, float mousePosY) const noexcept
	{
		const FVec2 center = transformComponent.position + boundingBoxComponent.offset;
		const FVec2 halfSize = boundingBoxComponent.size * 0.5f;

		return isInside(center - halfSize, center + halfSize, FVec2{ mousePosX, mousePosY });
	}
}