#include "game/game.h"
#include "game/scenes/title_scene.h"
#include "game/events/events.h"
#include "game/systems/scene_system.h"
#include <engine/core/engine_context.h>
#include <engine/core/action/action_registry.h>
#include <engine/ecs/system/system_manager.h>
#include <engine/core/events/event_bus.h>
#include <engine/core/events/events.h>
#include <iostream>

#include <engine/ecs/component/component_registry.h>

Game::Game()
{
}

void Game::onUpdate(float deltaTime)
{
	m_sceneStack.update(deltaTime);
	m_sceneStack.applyPendingChanges();
	// handle scene transition...
	// get current scene?
}

void Game::onCreated(const EngineContext& context)
{
	cursed_engine::ComponentInitContext componentInitContext{
		context.assets.assetManager,
		context.assets.localization,
		context.rendering.rendererAPI,
		context.resources.audioManager,
		context.resources.fontManager,
		context.resources.textureManager,
		context.resources.textManager,
		context.resources.textFactory
	};

	auto* systemManager = context.ecs.systemManager;
	systemManager->emplace<SceneSystem>(
		componentInitContext,
		context.eventBus,
		m_sceneStack,
		m_sceneFactory);

	m_sceneFactory.init(componentInitContext, context.ecs.systemManager, context.ecs.entityFactory, context.ecs.componentRegistry, context.eventBus);
	//m_sceneFactory.init(&systemManager, &context.ecs.entityFactory, m_appContext.eventBus, &context.ecs.componentRegistry);

	// or register in events file
	context.actionRegistry->registerAction("NewGame",
		[eventBus = context.eventBus](const cursed_engine::ActionArgs& args)
		{
			eventBus->publishInstantly<cursed_engine::PlaySoundEvent>("ButtonClick");
			// eventBus.publishInstantly(PlaySoundEvent{ "ButtonClick" });

			// eventBus.publishInstantly(SceneTransitionEvent{ "GameScene" }); // should game know about scenes?

			eventBus->publishInstantly<NewGameEvent>(); // or handle direclty in game class or change scene
			int x = 20;
		}); //  "NewGame"

	context.actionRegistry->registerAction("SceneTransition",
		[eventBus = context.eventBus](const cursed_engine::ActionArgs& args)
		{
			std::string scene;
			if (auto it = args.find("scene"); it != args.end())
			{
				scene = std::get<std::string>(it->second);

				// or handle direclty in game class or change scene
			}

			std::string transition;
			if (auto it = args.find("transition"); it != args.end())
			{
				transition = std::get<std::string>(it->second);
			}

			eventBus->publishInstantly<SceneTransitionEvent>(scene, transition);

			// send change 
			int x = 20; // Need to pass string value...
		});

	//setupScenes();

	m_sceneStack.addPath("TitleScene", context.assetRoot.string() + "scenes/title_scene.json"); // Force user to specify path?
	//m_sceneStack.registerScene<TitleScene>("TitleScene", context.assetRoot.string() + "scenes/title_scene.json"); // Force user to specify path?

	// DONT HERE? creates / enters scene before engine is done initializing...
	context.eventBus->publishInstantly<SceneTransitionEvent>("title_scene", "push");
	//m_sceneStack.push(std::make_unique<TitleScene>(&context.systemManager, &context.entityFactory, &context.componentRegistry, &context.eventBus)); // NOTE; (maybe problem) but every scene will need to accept systemmanager!

}

void Game::onDestroyed()
{
	m_sceneStack.clear();
}

void Game::setupScenes()
{
}