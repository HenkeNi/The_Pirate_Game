#pragma once
#include "game/scenes/scene_types.h"
#include <engine/ecs/ecs_registry.h>
#include <engine/utils/utils.h>
#include <any>
#include <typeindex>
#include <unordered_map>

namespace cursed_engine
{
	class PhysicsWorld;
}

namespace ce = cursed_engine;

class SceneLoader;

class Scene
{
public:
	Scene(SceneContext context);
	virtual ~Scene() = default;

	void update(float deltaTime); // or base scene updates ecs?
	virtual void onUpdate(float deltaTime) = 0;

	void render();
	virtual void onRender() {}; // make const?

	virtual void onEnter() {};
	virtual void onExit() {};

	virtual void onCreated() {};
	virtual void onDestroyed() {};
	
	virtual ce::PhysicsWorld* getPhysicsWorld() noexcept { return nullptr; } // maybe find a better idea?

protected:
	friend class SceneLoader;

	SceneContext m_context;
	cursed_engine::ECSRegistry m_registry;
};