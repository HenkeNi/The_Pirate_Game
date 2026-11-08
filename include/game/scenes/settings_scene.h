#pragma once
#include "scene.h"

class SettingsScene : public Scene
{
public:
	SettingsScene(SceneContext context);
	void onUpdate(const ce::UpdateContext& ctx) override;

	void onEnter();
	void onExit();

private:

};
