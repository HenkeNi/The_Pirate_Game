#include "engine/ecs/system/text_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/core/localization/localization.h"
#include "engine/rendering/text/text_creator.h"

namespace cursed_engine
{
	TextSystem::TextSystem(const TextCreator* textCreator, Localization* localization)
		: m_textCreator{ textCreator }, m_localization{ localization }
	{
	}

	void TextSystem::update(SystemUpdateContext& context)
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

		//			const std::string& text = m_localization.getText(textComponent.textId);

		//			if (!m_textManager.isConstructed(textComponent.textId, textComponent.fontSize))
		//			{
		//				textComponent.textureHandle = m_textManager.create(textComponent.textId, text, fontHandle, textComponent.color, textComponent.fontSize);
		//			}
		//			else
		//			{
		//				textComponent.textureHandle = m_textManager.getHandle(textComponent.textId, textComponent.fontSize); //Y TODO pass in path?
		//			}

		//			textComponent.isDirty = false;
		//		}
		//	});

		auto textView = registry.view<TextComponent>();
		textView.forEach([&](TextComponent& textComponent)
			{
				if (!textComponent.text)
				{
					/*auto fontHandle = textComponent.fontHandle;


					if (!fontHandle.isValid())
					{
						Logger::logWarning("Failed to find font");
						return;
					}

					const std::string& text = m_localization->getText(textComponent.textID);*/

					//m_textFactory->createText(text, fontHandle);
				}

				// try use text in text component... (no handle) store directly in component...
			});
	}
}


