#pragma once
#include <engine/core/application.h>

using cursed_engine::EngineSystems;

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
	void onCreated(const EngineSystems& systems) override;

private:
	void setupScenes();

	cursed_engine::SceneStack* m_sceneStack;
};