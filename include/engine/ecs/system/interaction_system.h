#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class InteractionSystem : public System
	{
	public:
		void update(SystemContext& context) override;

	private:
		void handleMouseInput();
		void handleControllerInput();
	};
}