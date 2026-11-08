#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	// or ui interaction system?

	class InteractionSystem : public UpdateSystem
	{
	public:
		void update(SystemUpdateContext& context) override;

	private:
		void handleMouseInput();
		void handleControllerInput();
	};
}