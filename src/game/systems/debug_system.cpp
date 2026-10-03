#include "game/systems/debug_system.h"
#include "game/components/components.h"
#include <engine/utils/frame_timer.h>
#include <engine/ecs/ecs_registry.h>
#include <engine/ecs/component/core_components.h> 

DebugSystem::DebugSystem(cursed_engine::FrameTimer& timer)
	: m_timer{ timer }
{ 
}

void DebugSystem::update(cursed_engine::SystemUpdateContext& context)
{
	auto view = context.registry.view<cursed_engine::TextComponent, DebugComponent>();

	view.forEach([&](cursed_engine::TextComponent& textComponent, const DebugComponent& debugComponent)
		{
			if (!textComponent.text)
			{
				// log?
				return;
			}

			const int fps = (int)m_timer.getFPS();
			textComponent.text->setText(std::format("FPS: {}", fps));
			//textComponent.text.setText("FPS: " + std::to_string(m_timer.getFPS()));
			int x = 20;
		});

	// check if debug component:: id == "fps"?
}