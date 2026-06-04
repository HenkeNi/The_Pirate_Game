#include "game/scenes/settings_scene.h"
#include <engine/core/logger.h>

using namespace cursed_engine;

SettingsScene::SettingsScene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData, cursed_engine::EventBus* eventBus)
	: Scene{ systemManager, entityFactory, componentData, eventBus, "SettingsScene" }
{
}

void SettingsScene::onUpdate(float deltaTime)
{
	int x = 20;
}

void SettingsScene::onEnter()
{
	Logger::logInfo("Entering Settings Scene...");
}

void SettingsScene::onExit()
{
}
