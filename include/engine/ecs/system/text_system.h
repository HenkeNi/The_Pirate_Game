#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class TextCreator;
	class Localization;
	
	class TextSystem : public UpdateSystem
	{
	public:
		TextSystem(const TextCreator* textCreator, Localization* localization);

		void update(SystemUpdateContext& context) override;

	private:
		//void handleDynamicText();
		//void handleStaticText();

		const TextCreator* m_textCreator;
		//TextFactory* m_textFactory;
		Localization* m_localization;
	};
}