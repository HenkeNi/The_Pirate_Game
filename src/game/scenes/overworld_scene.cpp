#include "game/scenes/overworld_scene.h"
#include "game/systems/map_render_system.h"
#include <engine/core/logger.h>
#include <engine/ecs/system/system_manager.h>

using namespace cursed_engine;

OverworldScene::OverworldScene(SceneContext context)
	: Scene{ std::move(context) }
{
}

void OverworldScene::onUpdate(float deltaTime)
{
	int x = 20;
}

void OverworldScene::onEnter()
{
	Logger::logInfo("Entering Overworld Scene..."); // put in scene stack?
	
	auto& mapRenderSystem = m_context.systemManager->getSystem<MapRenderSystem>();

	m_mapGenerator.generateStartArea(m_tileMap, 1);

	mapRenderSystem.setTileMap(&m_tileMap);
}

void OverworldScene::onExit()
{
}
