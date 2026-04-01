#include "engine/ecs/system/text_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/resources/text_manager.h"

namespace cursed_engine
{

	TextSystem::TextSystem(TextManager& textManager, Localization& localization)
		: m_textManager{ textManager }, m_localization{ localization }
	{
	}
	//TextSystem::TextSystem(TextManager& textManager, Localization& localization)
	//	: m_textureManager{ textureManager }, m_fontManager{ fontManager }, m_localization{ localization }
	//{
	//}

	void TextSystem::update(SystemContext& context)
	{
		auto& registry = context.registry;

		// TODO; read static text's from json (in Scene)... 

		auto view = registry.view<TextComponent>();
		view.forEach([&](TextComponent& textComponent)
			{
				if (textComponent.isDirty)
				{
					auto fontHandle = textComponent.fontHandle;

					if (!fontHandle.isValid())
						return;

					// TODO; get text from localization class...

					if (!m_textManager.isConstructed(textComponent.textID, textComponent.fontSize))
					{
						textComponent.textureHandle = m_textManager.create(textComponent.textID, "title_text", fontHandle, textComponent.color, textComponent.fontSize);
					}
					else
					{
						textComponent.textureHandle = m_textManager.getHandle("title_text", textComponent.fontSize);
					}

					textComponent.isDirty = false;
				}
			});
	}
}


