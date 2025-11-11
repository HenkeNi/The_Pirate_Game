#pragma once
#include <engine/core/application.h>
#include "game/scenes/scene_stack.h"

using cursed_engine::AppContext;

namespace cursed_engine
{
	class SceneStack;
}

class Game : public cursed_engine::Application
{
public:
	Game();
	~Game() = default;

	void onUpdate(float deltaTime) override;
	
	void onCreated(const AppContext& context) override;
	void onDestroyed() override;

private:
	void setupScenes();

	SceneStack m_sceneStack;
};