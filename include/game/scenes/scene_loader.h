#pragma once
#include "game/scenes/scene_types.h"
#include <engine/ecs/component/component_registry.h>
#include <filesystem>
#include <memory>

namespace cursed_engine
{
//	struct ComponentInitContext; // why not working?
	class EventBus;
}

namespace ce = cursed_engine;

class Scene;

class SceneLoader
{
public:
	void init(SceneContext sceneCtx, ce::ComponentInitContext initCtx, ce::EventBus* eventBus);

	// rreturn future?
	std::unique_ptr<Scene> load(const SceneMeta& meta); // return result?

private:
	ce::ComponentInitContext m_componentInitContext;
	//ce::ComponentPostInitContext m_componentPostInitContext;

	SceneContext m_sceneContext; // are all 3 context necessary? or can be combined?  (remove scene context?)
	ce::EventBus* m_eventBus; // maybe not?? dont send event?
};