#pragma once
#include "game/scenes/scene_types.h"
#include <engine/ecs/component/component_registry.h>
#include <filesystem>
#include <memory>

//namespace cursed_engine
//{
//	struct ComponentInitContext; // why not working?
//}

namespace ce = cursed_engine;

class Scene;

class SceneLoader
{
public:
	void init(SceneContext sceneCtx, ce::ComponentInitContext componentCtx);

	// rreturn future?
	std::unique_ptr<Scene> load(const SceneMeta& meta); // return result?

private:
	ce::ComponentInitContext m_componentInitContext;
	SceneContext m_sceneContext;
};