#pragma once
#include "game/scenes/scene.h"

class TitleScene : public Scene
{
public:
	TitleScene(SceneContext context);
	void onUpdate(const ce::UpdateContext& ctx) override;

	void onEnter();
	void onExit();

private:

};