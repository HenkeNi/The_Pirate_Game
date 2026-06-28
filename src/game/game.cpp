#include "game/game.h"
#include "game/events/events.h"
#include "game/systems/scene_system.h"
#include <engine/core/engine_context.h>
#include <engine/core/action/action_registry.h>
#include <engine/ecs/system/system_manager.h>
#include <engine/core/events/event_bus.h>
#include <engine/core/events/events.h>
#include <iostream>

#include "game/scenes/title_scene.h"
#include "game/scenes/overworld_scene.h"
#include "game/scenes/settings_scene.h"

#include "game/systems/map_render_system.h"

#include <engine/ecs/system/render_system.h>
#include <engine/ecs/system/interaction_system.h> //? ?
#include <engine/ecs/system/ui_system.h>
#include <engine/ecs/system/text_system.h>
#include <engine/ecs/system/audio_system.h>

#include <engine/core/settings/settings.h>
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

	systemManager->emplace<MapRenderSystem>(context.rendering.rendererAPI, context.resources.textureManager, m_tileRegistry);
	systemManager->emplace<cursed_engine::RenderSystem>(context.resources.textureManager, context.assets.assetManager, context.rendering.rendererAPI);
	//m_systemManager.emplace<InputSystem>(inputHandler);
	systemManager->emplace<cursed_engine::InteractionSystem>();
	systemManager->emplace<cursed_engine::UISystem>(context.platform.inputHandler, context.actionRegistry); // OR Accept action registry (and event bus) by pointer?
	systemManager->emplace<cursed_engine::TextSystem>(context.resources.textManager, context.resources.textFactory, context.assets.localization);
	systemManager->emplace<cursed_engine::AudioSystem>(context.resources.audioManager, context.audio.audioController, context.eventBus); // FIX eventbus ptr

	systemManager->emplace<SceneSystem>(
		componentInitContext,
		context.eventBus,
		m_sceneStack,
		m_sceneFactory);



	m_sceneFactory.init({ context.ecs.entityFactory, context.ecs.componentRegistry, context.ecs.systemManager, context.eventBus });

	const auto& configs = context.settings->getEngineConfig();

	m_sceneFactory.registerScene("title_scene", configs.resource.assetRoot.string() + "scenes/title_scene.json", [](SceneContext context) { return std::make_unique<TitleScene>(std::move(context)); });
	m_sceneFactory.registerScene("settings_scene", configs.resource.assetRoot.string() + "scenes/settings_scene.json", [](SceneContext context) { return std::make_unique<SettingsScene>(std::move(context)); });
	m_sceneFactory.registerScene("overworld_scene", configs.resource.assetRoot.string() + "scenes/overworld_scene.json", [](SceneContext context) { return std::make_unique<OverworldScene>(std::move(context)); });

	//m_sceneStack.addPath("TitleScene", configs.resource.assetRoot.string() + "scenes/title_scene.json" ); // Force user to specify path?
	//m_sceneStack.addPath("OverworldScene", configs.resource.assetRoot.string() + "scenes/overworld_scene.json");

	m_tileRegistry.load(*context.assets.assetManager, "../assets/tiles/tile_types.json");

	//m_sceneFactory.init(&systemManager, &context.ecs.entityFactory, m_appContext.eventBus, &context.ecs.componentRegistry);

	// or register in events file
	context.actionRegistry->registerAction("NewGame",
		[eventBus = context.eventBus](const cursed_engine::ActionArgs& args)
		{
			eventBus->publishInstantly<cursed_engine::PlaySoundEvent>("ButtonClick");
			// eventBus.publishInstantly(PlaySoundEvent{ "ButtonClick" });

			// eventBus.publishInstantly(SceneTransitionEvent{ "GameScene" }); // should game know about scenes?

			eventBus->publishInstantly<NewGameEvent>(); // or handle direclty in game class or change scene

			eventBus->publishInstantly<SceneTransitionEvent>("overworld_scene", "push");

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

	//m_sceneStack.registerScene<TitleScene>("TitleScene", context.assetRoot.string() + "scenes/title_scene.json"); // Force user to specify path?

	// DONT HERE? creates / enters scene before engine is done initializing...
	//context.eventBus->publishInstantly<SceneTransitionEvent>("overworld_scene", "push");
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