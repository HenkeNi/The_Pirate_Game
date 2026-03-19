#pragma once
#include "engine/ecs/system/system.h"
#include "engine/resources/engine_resources.h"

namespace cursed_engine
{
	class Localization; 

	class TextSystem : public System
	{
	public:
		TextSystem(EngineResources& engineResources, Localization& localization);

		void update(SystemContext& context) override;

	private:
		void handleDynamicText();
		void handleStaticText();

		EngineResources& m_engineResources;
		Localization& m_localization;
	};
}