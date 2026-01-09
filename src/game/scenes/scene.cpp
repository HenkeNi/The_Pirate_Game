#include "game/scenes/scene.h"
#include "engine/ecs/system/system_manager.h"
#include <cassert>

Scene::Scene(cursed_engine::SystemManager* systemManager, std::string id)
	: m_context{ systemManager }, m_id{ std::move(id) }
{
}

void Scene::update(float deltaTime)
{
	assert(m_context.systemManager && "SystemManager is nullptr!");

	onUpdate(deltaTime);
	m_context.systemManager->update(m_context.registry, deltaTime);
}