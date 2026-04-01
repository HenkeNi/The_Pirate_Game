#pragma once
#include "engine/ecs/system/system.h"
//#include "engine/resources/texture_manager.h"
//#include "engine/resources/font_manager.h"

namespace cursed_engine
{
	class TextManager;
	class Localization; 
	//class Font;
	
	class TextSystem : public System
	{
	public:
		TextSystem(TextManager& textManager, Localization& localization);
		//TextSystem(TextureManager& textureManager, FontManager& fontManager, Localization& localization);

		void update(SystemContext& context) override;

	private:
		//void handleDynamicText();
		//void handleStaticText();

		TextManager& m_textManager;
		//TextureManager& m_textureManager;
		//FontManager& m_fontManager;
		Localization& m_localization;
	};
}