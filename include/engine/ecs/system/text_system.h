#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
//	class TextFactory;
	class TextManager;
	class Localization;
	
	class TextSystem : public UpdateSystem
	{
	public:
		TextSystem(TextManager* textManager/*,TextFactory* textFactory*/, Localization* localization);

		void update(SystemUpdateContext& context) override;

	private:
		//void handleDynamicText();
		//void handleStaticText();

		TextManager* m_textManager;
		//TextFactory* m_textFactory;
		Localization* m_localization;
	};
}