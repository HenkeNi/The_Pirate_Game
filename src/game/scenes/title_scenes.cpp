#include "game/scenes/title_scene.h"
#include <engine/core/logger.h>

using namespace cursed_engine;

TitleScene::TitleScene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData, cursed_engine::EventBus* eventBus)
	: Scene{ systemManager, entityFactory, componentData, eventBus, "TitleScene" }
{
}

void TitleScene::onUpdate(float deltaTime)
{
	int x = 20;
}

void TitleScene::onEnter()
{
	Logger::logInfo("Entering Title Scene...");
}

void TitleScene::onExit() 
{
}
