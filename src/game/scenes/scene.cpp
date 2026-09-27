#include "game/scenes/scene.h"
#include <engine/ecs/system/system_manager.h>
#include <cassert>

#include <engine/ecs/system/system.h>

Scene::Scene(SceneContext context)
	: m_context{ std::move(context) }
{
}

void Scene::update(float deltaTime)
{
	assert(m_context.systemManager && "SystemManager is nullptr!");

	onUpdate(deltaTime);

	cursed_engine::SystemUpdateContext context{ m_registry, *m_context.eventBus, deltaTime };
	m_context.systemManager->update(context);
}

void Scene::render()
{
	onRender();

	cursed_engine::SystemRenderContext context{ m_registry, m_context.renderAPI };
	m_context.systemManager->render(context); // or copy it?
}