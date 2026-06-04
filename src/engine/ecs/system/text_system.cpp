#include "engine/ecs/system/text_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/core/localization/localization.h"
#include "engine/resources/text/text_manager.h"
#include "engine/resources/text/text_factory.h"

namespace cursed_engine
{
	TextSystem::TextSystem(TextManager* textManager, TextFactory* textFactory, Localization* localization)
		: m_textManager{ textManager }, m_textFactory{ textFactory }, m_localization{ localization }
	{
	}

	void TextSystem::update(SystemContext& context)
	{
		auto& registry = context.registry;

		// TODO; read static text's from json (in Scene)... 

		//auto view = registry.view<TextComponent>();
		//view.forEach([&](TextComponent& textComponent)
		//	{
		//		if (textComponent.isDirty)
		//		{
		//			auto fontHandle = textComponent.fontHandle;

		//			if (!fontHandle.isValid())
		//			{
		//				Logger::logWarning("Failed to find font");
		//				return;
		//			}

		//			const std::string& text = m_localization.getText(textComponent.textID);

		//			if (!m_textManager.isConstructed(textComponent.textID, textComponent.fontSize))
		//			{
		//				textComponent.textureHandle = m_textManager.create(textComponent.textID, text, fontHandle, textComponent.color, textComponent.fontSize);
		//			}
		//			else
		//			{
		//				textComponent.textureHandle = m_textManager.getHandle(textComponent.textID, textComponent.fontSize); //Y TODO pass in path?
		//			}

		//			textComponent.isDirty = false;
		//		}
		//	});

		auto textView = registry.view<TextComponent>();
		textView.forEach([&](TextComponent& textComponent)
			{
				if (!textComponent.textObj.isValid())
				{
					auto fontHandle = textComponent.fontHandle;


					if (!fontHandle.isValid())
					{
						Logger::logWarning("Failed to find font");
						return;
					}

					const std::string& text = m_localization->getText(textComponent.textID);

					m_textFactory->createText(text, fontHandle);
				}

				// try use text in text component... (no handle) store directly in component...
			});
	}
}


