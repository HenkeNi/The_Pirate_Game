#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	// Repsonsibilities; should determine/calculate final position, anchors, etc...
	// set state?
	// update visual state...

	class InputHandler;
	class ActionRegistry;

	class UISystem : public System
	{
	public:
		UISystem(InputHandler& inputHandler, ActionRegistry& actionRegistry);

		void update(SystemContext& context) override;

	private:
		void handleButtonInteractions(ECSRegistry& registry);
		void handleMouseBtnPressed(const struct MouseBtnPressedEvent& event);
		void handleKeyPressed(const struct KeyPressedEvent& event);
		//void handleMouseBtnPressed(MouseButton button);

		InputHandler& m_inputHandler;
		ActionRegistry& m_actionRegistry;
		//EventBus& m_eventBus;
	};
}