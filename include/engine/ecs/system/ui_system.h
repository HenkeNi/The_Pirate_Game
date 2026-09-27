#pragma once
#include "engine/ecs/system/system.h"
#include "engine/platform/input_api.h"

namespace cursed_engine
{
	// Repsonsibilities; should determine/calculate final position, anchors, etc...
	// set state?
	// update visual state...

	class ActionRegistry;
	
	class UISystem : public UpdateSystem
	{
	public:
		UISystem(InputAPI input, ActionRegistry* actionRegistry);

		void update(SystemUpdateContext& context) override;

	private:
		void handleButtonInteractions(ECSRegistry& registry); // renaeme func?
		//void updateButtonColor(ButtonComponent::State buttonState, SpriteComponent& spriteComponent); // decide if should be func
		void handleCheckboxInteractions(ECSRegistry& registry);
		void handleSlidersInteractions(ECSRegistry& registry);

		void handleMouseBtnPressed(const struct MouseBtnPressedEvent& event);
		void handleKeyPressed(const struct KeyPressedEvent& event);
		//void handleMouseBtnPressed(MouseButton button);

		[[nodiscard]] bool isMouseInsideBoundingBox(struct TransformComponent& transformComponent, struct BoundingBoxComponent& boundingBoxComponent, float mousePosX, float mousePosY) const noexcept;

		InputAPI m_input;
		ActionRegistry* m_actionRegistry;
		//EventBus& m_eventBus;
	};
}