#pragma once
#include "System.h"

namespace cursed_engine
{
	class RenderSystem : public System
	{
	public:
		void update(ECSRegistry& registry, float deltaTime) override;
	};
}