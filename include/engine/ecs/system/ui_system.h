#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class InputHandler;
	class EventBus;

	class UISystem : public System
	{
	public:
		UISystem(InputHandler& inputHandler, EventBus& eventBus);

		void update(SystemContext& context) override;

	private:
		void handleButtonInteractions(ECSRegistry& registry);
		void handleMouseBtnPressed(const struct MouseBtnPressedEvent& event);
		void handleKeyPressed(const struct KeyPressedEvent& event);
		//void handleMouseBtnPressed(MouseButton button);

		InputHandler& m_inputHandler;
		EventBus& m_eventBus;
	};
}