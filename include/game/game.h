#pragma once
#include "game/scenes/scene_stack.h"
#include "game/scenes/scene_factory.h"
#include "game/map/tile_registry.h"
#include <engine/core/application.h>

namespace cursed_engine
{
	class EventBus;
}

using cursed_engine::EngineContext;

class Game : public cursed_engine::Application
{
public:
	Game();
	~Game() = default;

	void onUpdate(float deltaTime) override;
	
	void onCreated(const EngineContext& context) override; // pass by value?
	void onDestroyed() override;

private:
	void setupScenes();

	// Pimpl??
	SceneFactory m_sceneFactory;
	SceneStack m_sceneStack;
	//cursed_engine::EventBus* m_eventBus;

	TileRegistry m_tileRegistry;
};