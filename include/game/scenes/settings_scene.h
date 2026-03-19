#pragma once
#include "scene.h"

class SettingsScene : public Scene
{
public:
	SettingsScene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData, cursed_engine::EventBus* eventBus);
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:

};
