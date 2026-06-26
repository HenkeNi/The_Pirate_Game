#pragma once
#include "game/scenes/scene.h"

class TitleScene : public Scene
{
public:
	TitleScene(SceneContext context);
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:

};