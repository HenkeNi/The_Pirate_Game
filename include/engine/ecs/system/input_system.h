#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class InputHandler;

	class InputSystem : public System
	{
	public:
		InputSystem(InputHandler& inputHandler);

		void update(SystemContext& context) override;
		
	private:
		InputHandler& m_inputHandler;
	};
}