#pragma once
#include "game/scenes/scene_stack.h"
#include "game/scenes/scene_factory.h"
//#include "game/map/map_generator.h"
#include "game/map/tile_registry.h"
#include <engine/core/application.h>

namespace cursed_engine
{
	class EventBus;
}

class Game : public cursed_engine::Application
{
public:
	Game();
	~Game() = default;

	void onUpdate(float deltaTime) override;
	void onRender(const cursed_engine::RenderContext& ctx) override;
	
	void onCreated(const cursed_engine::EngineContext& ctx) override; // pass by value?
	void onDestroyed() override;

private:
	void setupScenes();

	// Pimpl??
	SceneFactory m_sceneFactory;
	SceneStack m_sceneStack;
	//cursed_engine::EventBus* m_eventBus;

	TileRegistry m_tileRegistry;
	//MapGenerator m_mapGenerator; // put in GameScene? (base)
};