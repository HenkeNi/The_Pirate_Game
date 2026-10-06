#include "game/scenes/scene_manager.h"
#include "game/scenes/scene.h"
#include "game/events/events.h"
#include <engine/core/events/event_bus.h>
#include <engine/core/engine_context.h>
#include <engine/core/logger.h>

void SceneManager::init(const ce::EngineContext& context, SceneRegistry registry)
{
	//m_context = context; // Needed????
	m_registry = std::move(registry);
	m_eventBus = context.eventBus;

	SceneContext sceneContext
	{
		context.ecs.entityFactory,
		context.ecs.componentRegistry,
		context.ecs.systemManager,
		context.eventBus,
		context.assets.assetManager,
		context.rendering.rendererAPI,
		context.resources.audioManager,
		context.audio.audioController
	};

	ce::ComponentInitContext componentInitContext = ce::createComponentInitContext(context);

	/*{
		context.assets.assetManager,
		context.assets.localization,
		context.rendering.rendererAPI,
		context.resources.audioManager,
		context.resources.fontManager,
		context.resources.textureManager,
		context.resources.textManager
	};
	*/
	m_loader.init(sceneContext, componentInitContext, m_eventBus);
}

void SceneManager::shutdown()
{
	m_stack.clear();
}

void SceneManager::applyPendingTransition()
{
	if (!m_pendingTransition.has_value())
		return;

	const SceneTransition& transition = m_pendingTransition.value();

	switch (transition.transition)
	{
	case SceneTransitionType::Push:
		push(transition.name);
		break;
	case SceneTransitionType::Pop:
		pop();
		break;
	case SceneTransitionType::Swap:
		swap(transition.name);
		break;
	}

	m_pendingTransition = std::nullopt;
}

void SceneManager::update(float deltaTime)
{
	if (!m_stack.empty()) [[unlikely]]
	{
		std::unique_ptr<Scene>& top = m_stack.back();
		top->update(deltaTime);
	}

	applyPendingTransition();
}

void SceneManager::render()
{
	if (!m_stack.empty()) [[unlikely]]
	{
		std::unique_ptr<Scene>& top = m_stack.back();
		top->render();
	}
}

void SceneManager::requestTransition(SceneName name, SceneTransitionType type)
{
	m_pendingTransition = { std::move(name), type };
}

void SceneManager::push(SceneName name)
{
	const SceneMeta& meta = m_registry.get(name);

	if (std::unique_ptr<Scene> scene = m_loader.load(meta))
	{
		if (!m_stack.empty())
		{
			std::unique_ptr<Scene>& top = m_stack.back();
			top->onExit();
		}

		scene->onCreated();
		scene->onEnter();

		m_stack.push_back(std::move(scene));
		m_eventBus->publishInstantly<SceneTransitionEvent>(name);
	}
	else
	{
		ce::Logger::logError("[SceneManager::push] - Failed to create new scene");
	}
}

void SceneManager::swap(SceneName name)
{
	pop();
	push(name);
}

void SceneManager::pop()
{
	if (!m_stack.empty()) [[likely]]
	{
		std::unique_ptr<Scene>& top = m_stack.back();
		top->onExit();
		top->onDestroyed();

		m_stack.pop_back();

		if (!m_stack.empty()) [[likely]]
		{
			std::unique_ptr<Scene>& top = m_stack.back();
			top->onEnter();
		}
	}
}