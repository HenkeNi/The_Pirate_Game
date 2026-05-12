#include "engine/core/engine.h"
#include "engine/core/application.h"
#include "engine/core/logger.h"
#include "engine/config/config_manager.h"
#include "engine/localization/localization.h"
#include "engine/input/input_handler.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/text_renderer.h"
#include "engine/resources/surface.h"
#include "engine/window/window.h"
#include <SDL3/SDL.h>


#include <SDL3_image/SDL_image.h>

#include "engine/assets/asset_loader.h"
#include "engine/audio/audio_controller.h"

#include "engine/action/actions.h"

#include "engine/ecs/system/system_manager.h"
#include "engine/ecs/entity/entity_factory.h"
#include "engine/events/event_bus.h"
#include "engine/assets/asset_manager.h"
#include "engine/utils/frame_timer.h"
#include "engine/physics/physics.h"
#include "engine/core/subsystem_registry.h"
#include "engine/resources/resource_manager.hpp"
#include "engine/resources/engine_resources.h"
//#include "engine/core/type_registry.hpp"


#include "engine/config/config_types.h" // remove later...
#include "engine/config/config_loader.h"

#include "engine/action/action_registry.h"

// REMOEV

#include "engine/ecs/component/core_components.h"
#include "engine/ecs/system/render_system.h"
#include "engine/ecs/system/interaction_system.h"
#include "engine/ecs/system/input_system.h"
#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/system/text_system.h"
#include "engine/ecs/system/audio_system.h"
#include "engine/ecs/component/component_registry.h"


namespace cursed_engine
{
	struct MetaStorage // MOVE to own file? rename registries?
	{
		//	TypeRegistry<ComponentInfo> componentData;
		ComponentRegistry componentData; // rename?
		// ResourceRegistry?
	};

	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }, assetRoot{ "../assets/" }
		{
		}
	
		// A GameContext in applicaiton (or Applicaiton Context, that stores certain "systems")

		SubsystemRegistry subsystemRegistry;
		SystemManager systemManager;
		FrameTimer timer;
		MetaStorage metaStorage;
		EntityFactory entityFactory; // pass in assetmanager, set ecs registry...
		ActionRegistry actionRegistry;
		std::filesystem::path assetRoot; // ??

		Application& application;
		uint64_t currentFrame = 0; // store current "stats", fps, etc
		// store an ECSContext here?

		//ResourceRegistry resourceRegistry; // Todo; figure out a better way !!

		// Task system/Thread pool
		// Profiler
	};

	Engine::Engine(Application& app)
		: m_impl{ std::make_unique<Engine::Impl>(app) }
	{
	}

	Engine::~Engine()
	{
	}

	bool Engine::init()
	{
		auto& subsystemRegistry = m_impl->subsystemRegistry;
		auto& configManager = subsystemRegistry.add<ConfigManager>();

		if (!configManager.loadConfig("../assets/config/engine_config.json"))
		{
			Logger::logError("Engine startup failed: could not load configuration file {}");
			return false;
		}

		const auto& appInfo = configManager.getAppInfo();
		SDL_SetAppMetadata(appInfo.name.c_str(), appInfo.version.c_str(), appInfo.identifier.c_str());

		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			Logger::logError(std::format("SDL couldn't be be initialized! Error: {}", SDL_GetError()).c_str());
			return false;
		}

		auto& eventBus = subsystemRegistry.add<EventBus>();

		//  Window initialization
		auto& window = subsystemRegistry.add<Window>();

		const auto& windowConfig = configManager.getWindowConfig();
		window.create(appInfo.name.c_str(), windowConfig);
		
		{
			SurfaceLoader surfaceLoader;
			Surface surface = surfaceLoader(windowConfig.iconPath);

			window.setIcon(surface);
		}

		// Input initialization
		auto& inputHandler = subsystemRegistry.add<InputHandler>(eventBus);
		inputHandler.init(configManager.getInputConfig());

		auto& renderer = subsystemRegistry.add<Renderer>();
		renderer.init(window);

		auto& textRenderer = subsystemRegistry.add<TextRenderer>();
		textRenderer.init(renderer);

		auto& audioController = subsystemRegistry.add<AudioController>();
		audioController.init();

		auto& engineResources = subsystemRegistry.add<EngineResources>(configManager.getResourceConfig(), renderer, textRenderer);
		
		subsystemRegistry.add<Physics>();
		//subsystemRegistry.add<ECSRegistry>();

		auto& assetManager = subsystemRegistry.add<AssetManager>();
		m_impl->entityFactory.init(&assetManager);

		// TODO; iterate through all files in folder...
		auto& localization = subsystemRegistry.add<Localization>();
		localization.registerLanguage("english", "../assets/localization/en.json"); // Dont here? read start language from config...
		localization.setLanguage("english");

		loadAssets(); // HERE? Dont do in init? also maybe dont do in Engine?

		core_components::registerAll(m_impl->metaStorage.componentData, assetManager, engineResources, configManager.getResourceConfig(), engineResources.textFactory, localization);
		core_actions::registerAll(m_impl->actionRegistry, eventBus);

		// Move to other part of code base?
		auto& systemManager = m_impl->systemManager;
		systemManager.emplace<RenderSystem>(engineResources, assetManager, renderer, textRenderer);
		//systemManager.emplace<InputSystem>(inputHandler);
		systemManager.emplace<InteractionSystem>();
		systemManager.emplace<UISystem>(inputHandler, m_impl->actionRegistry);
		systemManager.emplace<TextSystem>(engineResources.textManager, engineResources.textFactory, localization);
		systemManager.emplace<AudioSystem>(engineResources.audioManager, audioController, eventBus);

		m_impl->application.onCreated({
			m_impl->systemManager,
			inputHandler, assetManager,
			m_impl->entityFactory,
			renderer,
			window,
			eventBus,
			m_impl->metaStorage.componentData,
			m_impl->assetRoot
			});

		Logger::logInfo("Engine startup successful!");
		return true;
	}

	void Engine::shutdown()
	{
		m_impl->application.onDestroyed();
		m_impl->subsystemRegistry.forEach([](auto& subsystem) { subsystem->shutdown(); });
		SDL_Quit();

		Logger::logInfo("Engine shutdown complete");
	}

	void Engine::run()
	{
		bool running = true;

		auto& subsystemRegistry = m_impl->subsystemRegistry;

		while (running)
		{
			++m_impl->currentFrame;

			Uint64 start = SDL_GetPerformanceCounter();

			auto& timer = m_impl->timer;
			timer.tick();

			auto& inputHandler = subsystemRegistry.get<InputHandler>();
			inputHandler.beginFrame();

			auto& window = subsystemRegistry.get<Window>();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					running = false;
				}

				inputHandler.processInput(event); // TODO; "prefetch" input handler?
				window.processEvent(event);
			}

			double deltaTime = timer.getDeltaTime();

			subsystemRegistry.get<InputHandler>().update();
			m_impl->application.onUpdate(deltaTime);


			//m_impl->systemManager.update(deltaTime); // After application update?

			subsystemRegistry.get<EventBus>().dispatchAll();

			subsystemRegistry.get<EngineResources>().update(m_impl->currentFrame, deltaTime);

			// subsystemRegistry.get<Renderer>().clearScreen();

			Uint64 end = SDL_GetPerformanceCounter(); // hide in class?
			float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
			float fps = 1.f / elapsed;


			auto& renderer = subsystemRegistry.get<Renderer>();
			//renderer.renderDebugText(220, 220, "Fps: " + (int)fps);
			//renderer.renderDebugText(350, 10, std::to_string(fps).c_str());

			renderer.present(); // or in render system? if here, also clear!
			// timer.getFPS();
			window.setTitle(std::format("The Cursed Pirate - Fps: {}", (int)fps).c_str()); // render debug text instead?
		}
	}

	void Engine::loadAssets()
	{
		// TODO; use a single loop for all files in asset
		// TODO; create an AssetRegistry? or handle loading elsewhere? in Assets.h?

		// TODO; do lazy loading later on!? -> maybe load core resources?

		auto& assetManager = m_impl->subsystemRegistry.get<AssetManager>();
		auto& engineResources = m_impl->subsystemRegistry.get<EngineResources>();

		assetManager.addLoader<SpriteSheetLoader>(engineResources); // TOOD; pass texturemanager instead?
		assetManager.addLoader<TextureAtlasLoader>(engineResources);
		assetManager.addLoader<PrefabLoader>();


		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/prefabs/"))
		{
			if (!entry.is_regular_file())
				continue;

			auto handle = assetManager.loadAsset<Prefab>(entry.path());
			const auto& prefab = assetManager.getAsset<Prefab>(handle); // Dont later...
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/textures"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();
			std::string filename = path.string();

			if (filename.ends_with("sprite_sheet.json"))
			{
				auto handle = assetManager.loadAsset<SpriteSheet>(path);
			}
			else if (filename.ends_with("texture_atlas.json"))
			{
				auto handle = assetManager.loadAsset<TextureAtlas>(path);
			}
		}

		//for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/sounds"))
		//{
		//	if (!entry.is_regular_file())
		//		continue;

		//	const auto& path = entry.path();
		//	std::string filename = path.string();

		//	if (filename.ends_with(".wav")) // TODO; or check if file.extension() == ???
		//	{
		//		auto id = extractResourceID(path);
		//		registry.audioIdToPath.insert_or_assign(std::move(id), path); // move here?
		//	}
		//}

		//for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/fonts"))
		//{
		//	if (!entry.is_regular_file())
		//		continue;

		//	const auto& path = entry.path();
		//	std::string filename = path.string();

		//	if (filename.ends_with(".ttf"))
		//	{
		//		auto id = extractResourceID(path);
		//		registry.fontIdToPath.insert_or_assign(std::move(id), path); // move here?

		//		//engineResources.fontManager.insertPath(FontDescriptor{ id, 20 }, path);
		//		//engineResources.insertPath<Font>(path.filename().stem().string(), path); // auto generate id?
		//	}
		//}

		// TODO; load texture atlas...

	}
}