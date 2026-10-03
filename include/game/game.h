#pragma once
#include "game/scenes/scene_manager.h"
#include <engine/core/application.h>
#include <engine/utils/containers/registry.hpp>

namespace cursed_engine
{
	class EventBus;
	struct EngineConfig;
}

namespace ce = cursed_engine;

class Game final : public ce::Application
{
public:
	Game();
	~Game() = default;

	void onUpdate(float deltaTime) override;
	void onRender(const ce::RenderContext& ctx) override;
	
	void onCreated(const ce::EngineContext& ctx) override; // pass by value?
	void onDestroyed() override;

private:	
	void registerActions(const ce::EngineContext& ctx);
	void registerScenes(const ce::EngineContext& ctx);

	void registerComponents(const ce::EngineContext& ctx);
	void setupSystems(const ce::EngineContext& ctx);
	
	// Pimpl??
	SceneManager m_sceneManager;
	
	ce::EventBus* m_eventBus;
	const ce::EngineConfig* m_configs;
	//MapGenerator m_mapGenerator; // put in GameScene? (base)
};