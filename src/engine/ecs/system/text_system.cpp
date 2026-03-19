#include "engine/ecs/system/text_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"

namespace cursed_engine
{
	TextSystem::TextSystem(EngineResources& engineResources, Localization& localization)
		: m_engineResources{ engineResources }, m_localization{ localization }
	{
	}

	void TextSystem::update(SystemContext& context)
	{
		auto& registry = context.registry;

		// TODO; read static text's from json (in Scene)... 

		auto view = registry.view<TextComponent>();
		view.forEach([](TextComponent& textComponent)
			{
				if (textComponent.isDirty)
				{
					

					// create new texture... (factory / static method?)
					// place function here?
					// make into a resource? - already has a texture resource and texture loader
					// could store a loader here... but the lazy loading might not work for the text 
					// 


				}


				// Need to get or create texture... (if text has changed, it should create)


				// TODO; check if text is loaded...

				int x = 20;
			});
	}
}


