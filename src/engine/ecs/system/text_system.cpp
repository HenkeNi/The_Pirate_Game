#include "engine/ecs/system/text_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"

namespace cursed_engine
{
	TextSystem::TextSystem(EngineResources& engineResources)
		: m_engineResources{ engineResources }
	{
	}

	void TextSystem::update(ECSRegistry& registry, float deltaTime)
	{
		// TODO; read static text's from json (in Scene)... 

		auto view = registry.view<TextComponent>();
		view.forEach([](TextComponent& textComponent)
			{

				// Need to get or create texture... (if text has changed, it should create)


				// TODO; check if text is loaded...

				int x = 20;
			});
	}
}


