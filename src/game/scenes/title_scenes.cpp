#include "game/scenes/title_scene.h"
#include <engine/core/logger.h>

using namespace cursed_engine;

TitleScene::TitleScene(SceneContext context)
	: Scene{ std::move(context) }
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
