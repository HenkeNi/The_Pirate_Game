#pragma once
#include "engine/ecs/system/system.h"
#include "engine/platform/input_api.h"
//#include "engine/ecs/entity/entity.h"
#include "engine/ecs/entity/entity_handle.h"

namespace cursed_engine
{
	// Repsonsibilities; should determine/calculate final position, anchors, etc...
	// set state?
	// update visual state...

	class ActionRegistry;
	
	class UISystem : public UpdateSystem
	{
	public:
		UISystem(InputAPI input, EventBus& eventBus, ActionRegistry* actionRegistry);
		~UISystem();

		void update(SystemUpdateContext& context) override;

	private:
		//void handleButtonInteractions(ECSRegistry& registry); // renaeme func?
		//void updateButtonColor(ButtonComponent::State buttonState, SpriteComponent& spriteComponent); // decide if should be func
		//void handleCheckboxInteractions(ECSRegistry& registry);
		//void handleSlidersInteractions(ECSRegistry& registry);

		void onPressed(ECSRegistry& registry, EntityHandle handle);
		void onDragged(ECSRegistry& registry, EntityHandle handle, FVec2 mousePosition);
		void onReleased(ECSRegistry& registry, EntityHandle handle, FVec2 mousePosition); // onReleasedInsideCaptured

		void onHoverBegan(EntityHandle handle);
		void onHoverEnd(EntityHandle handle);

		void updateHover(ECSRegistry& registry, FVec2 mousePosition);
		void handleMouseInput(ECSRegistry& registry, InputState mouseState, FVec2 mousePosition);

		EntityHandle findInteractableUnderMouse(ECSRegistry& registry, FVec2 mousePosition);

		//[[nodiscard]] bool isMouseInside(const struct ScreenRectComponent& screenRectComponent, const FVec2& mousePos) const noexcept;

		[[nodiscard]] bool isMouseInside(const struct UITransformComponent& transformComponent, const FVec2& mousePos) const noexcept;
		[[nodiscard]] bool isMouseInside(const EntityHandle& handle, const FVec2& mousePos) const;

		InputAPI m_input;
		EventBus& m_eventBus;		
		ActionRegistry* m_actionRegistry;
		EntityHandle m_capturedEntity;
		EntityHandle m_hoveredEntity;
	};
}