#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/core/events/event_bus.h"
#include "engine/core/events/events.h"
#include "engine/core/action/action_registry.h"
#include "engine/platform/input_api.h"
#include "engine/math/vec2.hpp"
#include "engine/core/logger.h"
#include <format>
// TODO; move boundingbox checks to an interaction system?

namespace cursed_engine
{
	/*bool isInside(FVec2 min, FVec2 max, FVec2 point)
	{
		return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y;
	}*/

	UISystem::UISystem(InputAPI input, EventBus& eventBus, ActionRegistry* actionRegistry)
		: m_input{ input }, m_eventBus{ eventBus }, m_actionRegistry{ actionRegistry }, m_capturedEntity{ EntityHandle::invalid() }, m_hoveredEntity{ EntityHandle::invalid() }
	{
		m_eventBus.subscribe<WorldUnloadingEvent>([this](const WorldUnloadingEvent& e)
			{
				m_capturedEntity = EntityHandle::invalid();
				m_hoveredEntity = EntityHandle::invalid();
			});

		// need to pull events, doesnt have access to registry class otherwise...

		//m_eventBus.subscribe<MouseBtnPressedEvent>(handleMouseBtnPressed);
		//m_eventBus.subscribe<KeyPressedEvent>([this](const KeyPressedEvent& event)
	}

	UISystem::~UISystem()
	{
		// unsubscribe...
	}

	// onCapturedPressed
	void UISystem::onPressed(ECSRegistry& registry, EntityHandle handle)
	{
		// capture entity? or dont do from handleCaptuedEntity?
	}

	void UISystem::onDragged(ECSRegistry& registry, EntityHandle handle, FVec2 mousePos)
	{
		if (handle.hasComponents<SliderThumbComponent>())
		{
			LocalTransformComponent& transformComponent = handle.getComponent<LocalTransformComponent>(); // thumb's! rename...

			// can try; set transform position to mouse pos!!
			// does hierarchy system reset with parent position? (how does offset work? pivot? handled in sprite - oherwise need to offset both sprite and bounding box) 
			// try setting differnt transform in json... 

			if (auto* hierarchyComponent = handle.tryGetComponent<HierarchyComponent>())
			{
				// The slider itself (not thumb)
				EntityHandle parentHandle = hierarchyComponent->parent;

				SliderComponent& sliderComponent = parentHandle.getComponent<SliderComponent>();
				LocalTransformComponent& parentTransformComponent = parentHandle.getComponent<LocalTransformComponent>();
				
				float halfLength = sliderComponent.length * 0.5f;

				if (sliderComponent.orientation == Orientation::Horizontal)
				{
					// TODO; add mouse position?!!
					float currentThumbPosition = mousePos.x;
					
					//float currentThumbPosition = parentTransformComponent.position.x + sliderComponent.length * sliderComponent.value;

					// need to know if slider position is centered or upper left corner
					// thumb in world coords, or relative to parent??
					transformComponent.position.x = mousePos.x;
					//transformComponent.position.x = std::clamp(currentThumbPosition, parentTransformComponent.position.x - halfLength, parentTransformComponent.position.x + halfLength);
				}
				else
				{
					// 0 is up!!
					float currentThumbPosition = mousePos.y;
					Logger::logInfo(std::format("Mouse y: {}", currentThumbPosition));
					Logger::logInfo(std::format("pos y: {}", transformComponent.position.y));

					//float currentThumbPosition = parentTransformComponent.position.y + sliderComponent.length * sliderComponent.value;
					//transformComponent.position.y = std::clamp(currentThumbPosition, parentTransformComponent.position.y - halfLength, parentTransformComponent.position.y + halfLength);
					//transformComponent.position.y = mousePos.y;
					transformComponent.position.x = 640;
					transformComponent.position.y = 350;
					Logger::logInfo(std::format("pos y: {}", transformComponent.position.y));
				
					// "converrt" thumb position to normalized value (based on slider length)
				}
			}



			// check if vertical or horizontal... get parent?

		}
	}

	void UISystem::onReleased(ECSRegistry& registry, EntityHandle handle, FVec2 mousePosition)
	{
		if (ButtonComponent* buttonComponent = handle.tryGetComponent<ButtonComponent>())
		{
			// handle button press...
			m_actionRegistry->execute(buttonComponent->action, buttonComponent->actionArgs);
		}
		else if (CheckboxComponent* checkboxComponent = handle.tryGetComponent<CheckboxComponent>())
		{
			checkboxComponent->isChecked = !checkboxComponent->isChecked;

			if (SpriteComponent* spriteComponent = handle.tryGetComponent<SpriteComponent>())
			{
				spriteComponent->atlasRegion = checkboxComponent->isChecked ?
					checkboxComponent->checkedRegion : checkboxComponent->uncheckedRegion;
			}
		}
		else if (SliderThumbComponent* sliderThumbComponent = handle.tryGetComponent<SliderThumbComponent>())
		{
			int x = 20;
		}

		m_capturedEntity = EntityHandle::invalid();
	}

	void UISystem::onHoverBegan(EntityHandle handle)
	{
		UIInteractableComponent& uiInteractableComponent = handle.getComponent<UIInteractableComponent>();

		if (uiInteractableComponent.hoverColor)
		{
			SpriteComponent& spriteComponent = handle.getComponent<SpriteComponent>();
			spriteComponent.color = uiInteractableComponent.hoverColor.value();
		}
	}

	void UISystem::onHoverEnd(EntityHandle handle)
	{
		UIInteractableComponent& uiInteractableComponent = handle.getComponent<UIInteractableComponent>();
		SpriteComponent& spriteComponent = handle.getComponent<SpriteComponent>();

		spriteComponent.color = uiInteractableComponent.defaultColor;
	}

	void UISystem::update(SystemUpdateContext& systemContext)
	{
		const FVec2 mousePosition = m_input.getMousePosition();

		updateHover(systemContext.registry, mousePosition);
		handleMouseInput(systemContext.registry, m_input.getMouseInputState(MouseButton::Left), mousePosition);
	}

	void UISystem::updateHover(ECSRegistry& registry, FVec2 mousePosition)
	{
		EntityHandle hovered = findInteractableUnderMouse(registry, mousePosition);

		if (hovered == m_hoveredEntity)
			return;

		if (m_hoveredEntity.isValid())
			onHoverEnd(m_hoveredEntity);

		m_hoveredEntity = hovered;

		if (m_hoveredEntity.isValid())
			onHoverBegan(m_hoveredEntity);
	}

	void UISystem::handleMouseInput(ECSRegistry& registry, InputState mouseState, FVec2 mousePosition)
	{
		if (!m_capturedEntity.isValid())
		{
			if (mouseState == InputState::Pressed && m_hoveredEntity.isValid())
			{
				m_capturedEntity = m_hoveredEntity;
				onPressed(registry, m_capturedEntity);
			}

			return;
		}

		switch (mouseState)
		{
		case InputState::Held:
			onDragged(registry, m_capturedEntity, mousePosition);
			break;

		case InputState::Released:
			if (isMouseInside(m_capturedEntity, mousePosition))
				onReleased(registry, m_capturedEntity, mousePosition);

			// Always release capture, even if released outside.
			m_capturedEntity = EntityHandle::invalid();
			break;

		default:
			break;
		}
	}

	EntityHandle UISystem::findInteractableUnderMouse(ECSRegistry& registry, FVec2 mousePosition)
	{
		auto view = registry.view<UITransformComponent, UIInteractableComponent>();
		auto entity = view.findFirst([&](const UITransformComponent& uiTransformComponent, const UIInteractableComponent& uiInteractableComponent)
			{
				return isMouseInside(uiTransformComponent, mousePosition);
			});

		if (entity)
		{
			return EntityHandle{ entity.value(), &registry };
		}

		return EntityHandle::invalid();
	}

	/*bool UISystem::isMouseInside(const struct ScreenRectComponent& screenRectComponent, const FVec2& mousePos) const noexcept
	{
		auto rectToAABB = [](const FRect rect) -> FAABB
		{
			return FAABB{ rect.x, rect.y, rect.w, rect.h };
		};

		return rectToAABB(screenRectComponent.rect).contains(mousePos);
	}*/

	bool UISystem::isMouseInside(const UITransformComponent& transformComponent, const FVec2& mousePos) const noexcept
	{
		// This assumes that bounding box is "centered" around the transform position (rather than upper left corner)
		//const FVec2 halfSize = transformComponent.size * 0.5f;
		//const FVec2 center = transformComponent.position + halfSize;

		FAABB aabb{ transformComponent.position, transformComponent.position + transformComponent.size };
		return aabb.contains(mousePos);
	}

	bool UISystem::isMouseInside(const EntityHandle& handle, const FVec2& mousePos) const
	{
		const UITransformComponent& uiTransformComponent = handle.getComponent<UITransformComponent>();

		return isMouseInside(uiTransformComponent, mousePos);
	}
	//bool UISystem::isMouseInside(const EntityHandle& handle, const FVec2& mousePos) const
	//{
	//	const LocalTransformComponent& transformComponent = handle.getComponent<LocalTransformComponent>();
	//	const BoundingBoxComponent& boundingBoxComponent = handle.getComponent<BoundingBoxComponent>();

	//	return isMouseInside(transformComponent, boundingBoxComponent, mousePos);
	//}
}