#pragma once
#include "game/scenes/scene_types.h"
#include <engine/ecs/ecs_registry.h>

class SceneLoader;

class Scene
{
public:
	Scene(SceneContext context);
	virtual ~Scene() = default;

	void update(float deltaTime); // or base scene updates ecs?
	virtual void onUpdate(float deltaTime) = 0;

	virtual void onEnter() {};
	virtual void onExit() {};

	virtual void onCreated() {};
	virtual void onDestroyed() {};

protected:
	friend class SceneLoader;

	SceneContext m_context;
	cursed_engine::ECSRegistry m_registry;
};