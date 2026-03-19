#include "game/scenes/scene.h"
#include <engine/ecs/system/system_manager.h>
#include <cassert>

#include <engine/ecs/system/system.h>


Scene::Scene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData, cursed_engine::EventBus* eventBus, std::string id)
	: m_context{ systemManager, entityFactory, componentData, eventBus }, m_id{ std::move(id) }
{
}

void Scene::update(float deltaTime)
{
	assert(m_context.systemManager && "SystemManager is nullptr!");

	onUpdate(deltaTime);

	cursed_engine::SystemContext systemContext{ m_context.registry, *m_context.eventBus, deltaTime };
	m_context.systemManager->update(systemContext);
}