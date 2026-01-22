#include "game/scenes/title_scene.h"


TitleScene::TitleScene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData)
	: Scene{ systemManager, entityFactory, componentData, "TitleScene"}
{
}

void TitleScene::onUpdate(float deltaTime)
{
}

void TitleScene::onEnter()
{
	int x = 20;
}

void TitleScene::onExit() 
{
}
