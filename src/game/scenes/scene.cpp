#include "game/scenes/scene.h"
#include <engine/core/contexts.h>
#include <engine/ecs/system/system_manager.h>
#include <cassert>

#include <engine/ecs/system/system.h>

Scene::Scene(SceneContext context)
	: m_context{ std::move(context) }
{
}

void Scene::update(const ce::UpdateContext& ctx)
{
	assert(m_context.systemManager && "SystemManager is nullptr!");

	onUpdate(ctx);

	cursed_engine::SystemUpdateContext context{ m_registry, *m_context.eventBus, ctx.windowSize, ctx.deltaTime };
	m_context.systemManager->update(context);
}

void Scene::render()
{
	onRender();

	cursed_engine::SystemRenderContext context{ m_registry, m_context.renderAPI };
	m_context.systemManager->render(context); // or copy it?
}