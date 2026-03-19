#include "game/scenes/settings_scene.h"

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
}

void SettingsScene::onExit()
{
}
