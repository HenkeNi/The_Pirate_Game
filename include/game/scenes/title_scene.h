#pragma once
#include "game/scenes/scene.h"

class TitleScene : public Scene
{
public:
	TitleScene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData);
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:

};