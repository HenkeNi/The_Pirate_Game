#pragma once
#include "engine/ecs/system/system.h"
#include "engine/resources/engine_resources.h"

namespace cursed_engine
{
	class TextSystem : public System
	{
	public:
		TextSystem(EngineResources& engineResources);

		void update(ECSRegistry& registry, float deltaTime) override;

	private:
		void handleDynamicText();
		void handleStaticText();

		EngineResources& m_engineResources;
	};
}