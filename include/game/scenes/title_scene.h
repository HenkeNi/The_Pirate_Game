#pragma once
#include <engine/scenes/scene.h>

class TitleScene : public cursed_engine::Scene
{
public:
	void update(float deltaTime) override;

	void onEnter();
	void onExit();

private:

};