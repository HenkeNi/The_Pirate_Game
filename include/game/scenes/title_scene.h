#pragma once
#include "game/scenes/scene.h"

class TitleScene : public Scene
{
public:
	TitleScene(cursed_engine::SystemManager* systemManager);
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:

};