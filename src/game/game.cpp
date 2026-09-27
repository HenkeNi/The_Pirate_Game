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
#include "game/scenes/scene_types.h"


#include "game/systems/map_render_system.h"
#include "game/systems/player_controller_system.h"
#include "game/systems/input_system.h"
#include "game/systems/movement_system.h"
#include "game/systems/map_system.h"
#include "game/systems/camera_system.h"
#include "game/systems/map_decoration_system.h"
#include "game/systems/debug_system.h"
#include "game/systems/hud_system.h"

#include "game/map/tileset_loader.h"

#include <engine/ecs/system/render_system.h>
#include <engine/ecs/system/interaction_system.h> //? ?
#include <engine/ecs/system/ui_system.h>
#include <engine/ecs/system/transform_system.h>
#include <engine/ecs/system/hierarchy_system.h>
#include <engine/ecs/system/text_system.h>
#include <engine/ecs/system/audio_system.h>
#include <engine/ecs/system/animation_system.h>
#include <engine/ecs/system/physics_system.h>

#include <engine/core/settings/settings.h>
#include <engine/ecs/component/component_registry.h>

#include <engine/ecs/system/screen_space_render_system.h>
#include <engine/ecs/system/world_render_system.h>

#include <engine/rendering/render_pipeline.h>

#include "game/rendering/render_passes.h"

#include <engine/ecs/entity/entity_factory.h> // For setting context in factory... remove later!

#include <engine/assets/asset_manager.h>

using namespace cursed_engine;


Game::Game()
	: m_eventBus{ nullptr }, m_configs{ nullptr }
{
}

void Game::onUpdate(float deltaTime)
{
	//if (m_sceneStack.isEmpty()) [[unlikely]]
	//	m_eventBus->publishInstantly<SceneTransitionEvent>("title_scene", "push");

	m_sceneManager.update(deltaTime);

	// handle scene transition...
	// get current scene?
}

void Game::onRender(const cursed_engine::RenderContext& ctx)
{
	m_sceneManager.render();
}

void Game::onCreated(const cursed_engine::EngineContext& context)
{
	m_eventBus = context.eventBus;
	m_configs = &context.settings->getEngineConfig();

	context.assets.assetManager->addLoader<TilesetLoader>(*context.assets.assetManager);
	//context.assets.assetManager->preload<Tileset>("tile_types"); // any way of static_assert if havent specified tempalte type?

	// m_tileRegistry.load(*context.assets.assetManager, "../assets/map/tile_types.json");

	PhysicsAPI physicsAPI = context.physics.physics;
	physicsAPI.setDebugDrawEnabled(true);


	// just use engine ctx instead?
	ce::ComponentInitContext componentInitContext = ce::createComponentInitContext(context);

	//cursed_engine::ComponentInitContext componentInitContext{
	//	context.assets.assetManager,
	//	context.assets.localization,
	//	context.rendering.rendererAPI,
	//	context.resources.audioManager,
	//	context.resources.fontManager,
	//	context.resources.textureManager,
	//	context.resources.textManager,
	//	//context.resources.textFactory
	//};

	auto* systemManager = context.ecs.systemManager;

	context.ecs.entityFactory->setContext(componentInitContext); // ? or pass context when creating?

	//context.rendering.renderPipeline.emplace<WorldPass>(context.rendering.rendererAPI, context.resources.textureManager, context.assets.assetManager);
	//context.rendering.renderPipeline.emplace<UIPass>(context.);

	registerActions(context);
	registerScenes(context);

	registerComponents(context);
	setupSystems(context);

	// DONT HERE? creates / enters scene before engine is done initializing...
	//context.eventBus->publishInstantly<SceneTransitionEvent>("overworld_scene", "push");
	//context.eventBus->publishInstantly<SceneTransitionEvent>("title_scene", "push");
	
	// GAME COULD ALSO SET INITIAL SCENE!?
	context.eventBus->publish<SceneTransitionRequestEvent>("title_scene", SceneTransitionType::Push);
	//context.eventBus->publish<SceneTransitionRequestEvent>("overworld_scene", SceneTransitionType::Push);
	
	//context.eventBus->publish<SceneTransitionEvent>("title_scene", SceneTransitionType::Push);
}

void Game::onDestroyed()
{
	m_sceneManager.shutdown();
}

void Game::setupSystems(const ce::EngineContext& ctx)
{
	SystemManager* systemManager = ctx.ecs.systemManager;

	
	//systemManager->emplace<cursed_engine::RenderSystem>(context.resources.textureManager, context.assets.assetManager, context.rendering.rendererAPI);
	//m_systemManager.emplace<InputSystem>(inputHandler);

	systemManager->emplace<InteractionSystem>();
	systemManager->emplace<TransformSystem>(); // this or hierarchy system?
	systemManager->emplace<CameraSystem>(*ctx.settings); // run after TransformSystem -> sets final position (bounds, follow, etc)
	systemManager->emplace<UISystem>(ctx.platform.input, ctx.actionRegistry); // OR Accept action registry (and event bus) by pointer?
	systemManager->emplace<TextSystem>(ctx.resources.textManager/*, context.resources.textFactory*/, ctx.assets.localization);
	systemManager->emplace<AudioSystem>(ctx.resources.audioManager, ctx.audio.audioController, ctx.eventBus); // FIX eventbus ptr
	systemManager->emplace<PlayerControllerSystem>();
	systemManager->emplace<MovementSystem>();
	systemManager->emplace<HierarchySystem>();
	systemManager->emplace<InputSystem>(ctx.platform.input);
	systemManager->emplace<HUDSystem>(*ctx.eventBus, *ctx.ecs.entityFactory);




	ce::ComponentInitContext componentInitContext = ce::createComponentInitContext(ctx); // or pass it since already created in onCreated!
	//cursed_engine::ComponentInitContext componentInitContext{
	//	ctx.assets.assetManager,
	//	ctx.assets.localization,
	//	ctx.rendering.rendererAPI,
	//	ctx.resources.audioManager,
	//	ctx.resources.fontManager,
	//	ctx.resources.textureManager,
	//	ctx.resources.textManager,
	//	//context.resources.textFactory
	//};

	systemManager->emplace<SceneSystem>(
		componentInitContext,
		ctx.eventBus,
		m_sceneManager);
	systemManager->emplace<cursed_engine::AnimationSystem>(*ctx.assets.assetManager); // update or render?
	//systemManager->emplace<MapSystem>(m_mapGenerator); - currentyl done in overworld scene!
	systemManager->emplace<MapDecorationSystem>(*ctx.ecs.entityFactory, *ctx.eventBus);
	systemManager->emplace<DebugSystem>(*ctx.platform.timer); // only add in debug...
	systemManager->emplace<ce::PhysicsSystem>();


	systemManager->emplace<MapRenderSystem>(ctx.rendering.rendererAPI, ctx.resources.textureManager);
	systemManager->emplace<WorldRenderSystem>(ctx.resources.textureManager, ctx.assets.assetManager, ctx.rendering.rendererAPI, ctx.physics.physicsDebugDraw);
	systemManager->emplace<ScreenSpaceRenderSystem>(ctx.resources.textureManager, ctx.assets.assetManager, ctx.rendering.rendererAPI);
}

void Game::registerComponents(const ce::EngineContext& ctx)
{
	// No player controller component?
	/*componentRegistry->registerComponent<PlayerControllerComponent>("player_controller",
	[](EntityHandle& handle, const ComponentProperties& properties)
	{},
	[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
	{
		handle.attachComponent<PlayerControllerComponent>();
	});*/

	// Component registration
	ce::ComponentRegistry* componentRegistry = ctx.ecs.componentRegistry;

	ce::registerComponent<DebugComponent>(*componentRegistry, "debug",
		[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		{
			handle.attachComponent<DebugComponent>();
		},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<DebugComponent>();
		});

	ce::registerComponent<HealthComponent>(*componentRegistry, "health",
		[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		{
			handle.attachComponent<HealthComponent>();
		},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<HealthComponent>();
		});

	// or input in engine?
	ce::registerComponent<InputComponent>(*componentRegistry, "input",
		[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		{},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<InputComponent>();
		});


	ce::registerComponent<InteractionComponent>(*componentRegistry, "interaction",
		[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		{
			handle.attachComponent<InteractionComponent>();
		},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<InteractionComponent>();
		});

	ce::registerComponent<PlayerComponent>(*componentRegistry, "player",
		[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		{
			handle.attachComponent<PlayerComponent>();
		},
		[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
		{
			handle.attachComponent<PlayerComponent>();
		});
}

void Game::registerActions(const ce::EngineContext& ctx)
{
	ActionRegistry* registry = ctx.actionRegistry;

	assert(registry && "ActionRegistry is null!");

	// or register in events file
	registry->registerAction("NewGame",
		[eventBus = ctx.eventBus](const cursed_engine::ActionArgs& args)
		{
			eventBus->publishInstantly<cursed_engine::PlaySoundEvent>("ButtonClick");

			// eventBus.publishInstantly(PlaySoundEvent{ "ButtonClick" });
			// eventBus.publishInstantly(SceneTransitionEvent{ "GameScene" }); // should game know about scenes?

			// m_mapGenerator.generateStartArea(m_tilemap, 1);

			eventBus->publishInstantly<NewGameEvent>(); // or handle direclty in game class or change scene

			eventBus->publishInstantly<SceneTransitionRequestEvent>("overworld_scene", SceneTransitionType::Push);

		});

	registry->registerAction("SceneTransition",
		[eventBus = ctx.eventBus](const cursed_engine::ActionArgs& args)
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

			// FIX!
			static const std::unordered_map<std::string, SceneTransitionType> transitions
			{
				{ "push", SceneTransitionType::Push },
				{ "pop", SceneTransitionType::Pop },
				{ "replace", SceneTransitionType::Swap } // swpa or replace?
			};
			 
			eventBus->publishInstantly<SceneTransitionRequestEvent>(scene, transitions.at(transition));

			// send change 
			int x = 20; // Need to pass string value...
		});

	registry->registerAction("Board",
		[](const cursed_engine::ActionArgs& args)
		{
			// pass entity? both?
		});
}

void Game::registerScenes(const ce::EngineContext& ctx)
{
	const std::filesystem::path sceenPath = m_configs->resource.assetRoot / "scenes/";

	SceneRegistry sceneRegistry;

	// pass root path instead?
	registerScene<TitleScene>(sceneRegistry, "title_scene", sceenPath / "title_scene.json",
		[](SceneContext context)
		{
			return std::make_unique<TitleScene>(std::move(context));
		}); 

	registerScene<SettingsScene>(sceneRegistry, "settings_scene", sceenPath / "settings_scene.json",
		[](SceneContext context)
		{
			return std::make_unique<SettingsScene>(std::move(context));
		});
	
	registerScene<OverworldScene>(sceneRegistry, "overworld_scene", sceenPath / "overworld_scene.json",
		[physics = ctx.physics.physics](SceneContext context)
		{
			return std::make_unique<OverworldScene>(std::move(context), physics);
			//return std::make_unique<OverworldScene>(std::move(context), physics->createWorld());
		});

	m_sceneManager.init(ctx, std::move(sceneRegistry));
}