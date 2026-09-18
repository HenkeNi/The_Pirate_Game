#include "game/systems/debug_system.h"
#include "game/components/components.h"
#include <engine/utils/frame_timer.h>
#include <engine/ecs/ecs_registry.h>
#include <engine/ecs/component/core_components.h> 

DebugSystem::DebugSystem(cursed_engine::FrameTimer& timer)
	: m_timer{ timer }
{ 
}

void DebugSystem::update(cursed_engine::SystemContext& context)
{
	auto view = context.registry.view<cursed_engine::TextComponent, DebugComponent>();

	view.forEach([&](cursed_engine::TextComponent& textComponent, const DebugComponent& debugComponent)
		{
			const int fps = (int)m_timer.getFPS();
			textComponent.textObj.setText(std::format("FPS: {}", fps));
			//textComponent.textObj.setText("FPS: " + std::to_string(m_timer.getFPS()));
			int x = 20;
		});

	// check if debug component:: id == "fps"?
}