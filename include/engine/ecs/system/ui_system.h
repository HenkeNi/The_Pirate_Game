#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class InputHandler;

	class UISystem : public System
	{
	public:
		UISystem(InputHandler& inputHandler);

		void update(ECSRegistry& registry, float deltaTime) override;

	private:
		void handleButtonInteractions(ECSRegistry& registry);

		InputHandler& m_inputHandler;
	};
}