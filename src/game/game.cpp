#include "game/game.h"
#include "game/events/events.h"
#include "game/systems/scene_system.h"
#include <engine/core/engine_context.h>
#include <engine/core/action/action_registry.h>
#include <engine/ecs/system/system_manager.h>
#include <engine/core/events/event_bus.h>
#include <engine/core/events/events.h>
#include <iostream>

#include "game/components/components.h"

#include "game/scenes/title_scene.h"
#include "game/scenes/overworld_scene.h"
#include "game/scenes/settings_scene.h"

#include "game/systems/map_render_system.h"
#include "game/systems/player_controller_system.h"
#include "game/systems/input_system.h"
#include "game/systems/movement_system.h"
#include "game/systems/map_system.h"
#include "game/systems/map_decoration_system.h"

#include <engine/ecs/system/render_system.h>
#include <engine/ecs/system/interaction_system.h> //? ?
#include <engine/ecs/system/ui_system.h>
#include <engine/ecs/system/transform_system.h>
#include <engine/ecs/system/hierarchy_system.h>
#include <engine/ecs/system/text_system.h>
#include <engine/ecs/system/audio_system.h>
#include <engine/ecs/system/animation_system.h>

#include <engine/core/settings/settings.h>
#include <engine/ecs/component/component_registry.h>

#include <engine/ecs/system/screen_space_render_system.h>
#include <engine/ecs/system/world_render_system.h>

#include <engine/rendering/render_pipeline.h>

#include "game/rendering/render_passes.h"

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

void Game::onRender(const cursed_engine::RenderContext& ctx)
{

}

void Game::onCreated(const cursed_engine::EngineContext& context)
{
	cursed_engine::ComponentInitContext componentInitContext{
		context.assets.assetManager,
		context.assets.localization,
		context.rendering.rendererAPI,
		context.resources.audioManager,
		context.resources.fontManager,
		context.resources.textureManager,
		context.resources.textManager,
		//context.resources.textFactory
	};

	auto* systemManager = context.ecs.systemManager;


	systemManager->emplace<MapRenderSystem>(context.rendering.rendererAPI, context.resources.textureManager, m_tileRegistry);
	systemManager->emplace<cursed_engine::WorldRenderSystem>(context.resources.textureManager, context.assets.assetManager, context.rendering.rendererAPI);
	systemManager->emplace<cursed_engine::ScreenSpaceRenderSystem>(context.resources.textureManager, context.assets.assetManager, context.rendering.rendererAPI);
	//systemManager->emplace<cursed_engine::RenderSystem>(context.resources.textureManager, context.assets.assetManager, context.rendering.rendererAPI);
	
	
	//m_systemManager.emplace<InputSystem>(inputHandler);
	systemManager->emplace<cursed_engine::InteractionSystem>();
	systemManager->emplace<cursed_engine::TransformSystem>(); // this or hierarchy system?
	systemManager->emplace<cursed_engine::UISystem>(context.platform.input, context.actionRegistry); // OR Accept action registry (and event bus) by pointer?
	systemManager->emplace<cursed_engine::TextSystem>(context.resources.textManager/*, context.resources.textFactory*/, context.assets.localization);
	systemManager->emplace<cursed_engine::AudioSystem>(context.resources.audioManager, context.audio.audioController, context.eventBus); // FIX eventbus ptr
	systemManager->emplace<PlayerControllerSystem>();
	systemManager->emplace<MovementSystem>();
	systemManager->emplace<cursed_engine::HierarchySystem>();
	systemManager->emplace<InputSystem>(context.platform.input);
	systemManager->emplace<SceneSystem>(
		componentInitContext,
		context.eventBus,
		m_sceneStack,
		m_sceneFactory);
	systemManager->emplace<cursed_engine::AnimationSystem>(*context.assets.assetManager);
	//systemManager->emplace<MapSystem>(m_mapGenerator); - currentyl done in overworld scene!
	systemManager->emplace<MapDecorationSystem>(m_tileRegistry, *context.ecs.entityFactory, *context.eventBus);

	context.rendering.renderPipeline.emplace<WorldPass>(context.rendering.rendererAPI, context.resources.textureManager, context.assets.assetManager, m_tileRegistry);
	//context.rendering.renderPipeline.emplace<UIPass>(context.);

	using namespace cursed_engine; // why here and not at top?

	auto* componentRegistry = context.ecs.componentRegistry;

	// No player controller component?
	/*componentRegistry->registerComponent<PlayerControllerComponent>("player_controller",
		[](EntityHandle& handle, const ComponentProperties& properties)
		{},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<PlayerControllerComponent>();
		});*/

	// or engine?
	componentRegistry->registerComponent<InputComponent>("input",
		[](EntityHandle& handle, const ComponentProperties& properties)
		{},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<InputComponent>();
		});

	m_sceneFactory.init({ context.ecs.entityFactory, context.ecs.componentRegistry, context.ecs.systemManager, context.eventBus });

	const auto& configs = context.settings->getEngineConfig();

	m_sceneFactory.registerScene("title_scene", configs.resource.assetRoot.string() + "scenes/title_scene.json", [](SceneContext context) { return std::make_unique<TitleScene>(std::move(context)); });
	m_sceneFactory.registerScene("settings_scene", configs.resource.assetRoot.string() + "scenes/settings_scene.json", [](SceneContext context) { return std::make_unique<SettingsScene>(std::move(context)); });
	m_sceneFactory.registerScene("overworld_scene", configs.resource.assetRoot.string() + "scenes/overworld_scene.json", [](SceneContext context) { return std::make_unique<OverworldScene>(std::move(context)); });

	//m_sceneStack.addPath("TitleScene", configs.resource.assetRoot.string() + "scenes/title_scene.json" ); // Force user to specify path?
	//m_sceneStack.addPath("OverworldScene", configs.resource.assetRoot.string() + "scenes/overworld_scene.json");

	m_tileRegistry.load(*context.assets.assetManager, "../assets/map/tile_types.json");

	//m_sceneFactory.init(&systemManager, &context.ecs.entityFactory, m_appContext.eventBus, &context.ecs.componentRegistry);

	// or register in events file
	context.actionRegistry->registerAction("NewGame",
		[eventBus = context.eventBus](const cursed_engine::ActionArgs& args)
		{
			eventBus->publishInstantly<cursed_engine::PlaySoundEvent>("ButtonClick");
			// eventBus.publishInstantly(PlaySoundEvent{ "ButtonClick" });

			// eventBus.publishInstantly(SceneTransitionEvent{ "GameScene" }); // should game know about scenes?

			//m_mapGenerator.generateStartArea(m_tileMap, 1);


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