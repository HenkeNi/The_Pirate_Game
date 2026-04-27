#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class TextManager;
	class Localization;
	
	class TextSystem : public System
	{
	public:
		TextSystem(TextManager& textManager, Localization& localization);

		void update(SystemContext& context) override;

	private:
		//void handleDynamicText();
		//void handleStaticText();

		TextManager& m_textManager;
		Localization& m_localization;
	};
}