#include "game/scenes/settings_scene.h"
#include <engine/core/logger.h>

using namespace cursed_engine;

SettingsScene::SettingsScene(SceneContext context)
	: Scene{ std::move(context) }
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
