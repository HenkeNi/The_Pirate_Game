#include "game/scenes/overworld_scene.h"
#include "game/systems/map_render_system.h"
#include <engine/core/logger.h>
#include <engine/ecs/system/system_manager.h>

#include "game/systems/map_decoration_system.h"

#include "game/systems/map_system.h"

#include "engine/ecs/entity/entity_factory.h"

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
	
	auto& mapSystem = m_context.systemManager->emplace<MapSystem>(m_mapGenerator, m_context.eventBus);
	mapSystem.setMap(&m_tileMap); // NOTE! need to set map every time changing scene!

	auto& mapDecorationSystem = m_context.systemManager->getSystem<MapDecorationSystem>();
	mapDecorationSystem.setTileMap(&m_tileMap);

	auto& mapRenderSystem = m_context.systemManager->getSystem<MapRenderSystem>();

	m_mapGenerator.generateStartArea(m_tileMap, 1);

	mapRenderSystem.setTileMap(&m_tileMap);
	
	// TEMP; - print fps
	/*auto entity = m_context.entityFactory->createFromPrefab("DefaultText");
	if (entity.has_value())
	{
		int x = 20;
	}*/
}

void OverworldScene::onExit()
{
}
