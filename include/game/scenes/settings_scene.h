#pragma once
#include "scene.h"

class SettingsScene : public Scene
{
public:
	SettingsScene(SceneContext context);
	void onUpdate(float deltaTime) override;

	void onEnter();
	void onExit();

private:

};
