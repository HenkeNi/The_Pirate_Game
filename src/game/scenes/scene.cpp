#include "game/scenes/scene.h"
#include "engine/ecs/system/system_manager.h"
#include <cassert>

Scene::Scene(cursed_engine::SystemManager* systemManager)
	: m_context{ systemManager }
{
}

void Scene::update(float deltaTime)
{
	assert(m_context.systemManager && "SystemManager is nullptr!");

	onUpdate(deltaTime);
	m_context.systemManager->update(m_context.registry, deltaTime);
}
