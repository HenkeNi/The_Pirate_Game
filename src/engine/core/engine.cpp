#include "engine/core/engine.h"
#include "engine/core/logger.h"
#include "engine/core/application.h"
#include "engine/ecs/system/system_manager.h"
#include "engine/ecs/entity/entity_factory.h"
#include "engine/audio/audio_controller.h"
#include "engine/rendering/renderer.h"
#include "engine/assets/asset_manager.h"
#include "engine/window/window.h"
#include "engine/utils/frame_timer.h"
#include "engine/input/input_handler.h"
#include "engine/physics/physics.h"
#include "engine/events/event_bus.h"
#include "engine/config/config_manager.h"
#include "engine/core/subsystem_registry.h"
#include "engine/assets/asset_loaders.h"
#include "engine/resources/resource_manager.hpp"
#include "engine/resources/engine_resources.h"
#include "engine/core/type_registry.hpp"
#include <SDL3/SDL.h>

//#include "engine/utils/json_utils.h"

#include "engine/config/config_types.h" // remove later...
#include "engine/config/config_loader.h"

// REMOEV
#include "engine/audio/audio.h"
#include "engine/rendering/texture.h"
#include "engine/utils/data_structures/sparse_set.hpp"
#include <optional>

#include "engine/core/types.h"
#include "engine/utils/json/json_document.h"
#include "engine/ecs/component/core_components.h"


namespace cursed_engine
{
	struct MetaStorage // MOVE?
	{
		TypeRegistry<ComponentInfo> componentData;

	};

	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }, assetRoot{ "../assets/" }
		{
		}

		//struct MetaStorage
		//{
		//	TypeRegistry<ComponentInfo> componentData;

		//} metaStorage;

		SubsystemRegistry subsystemRegistry;
		SystemManager systemManager;
		FrameTimer timer;
		MetaStorage metaStorage;
		std::filesystem::path assetRoot;
		Application& application;

		//EntityFactory entityFactory; // dont store instnace? let game use if needed?

		// Task system/Thread pool
		// Profiler
	};

	void registerComponents(TypeRegistry<ComponentInfo>& registry);

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

		if (!configManager.loadAllConfigs("../assets/config/"))
		{
			Logger::logError("Failed to load one or more configuration files.");
			return false;
		}

		const auto& appInfo = configManager.getAppInfo();
		SDL_SetAppMetadata(appInfo.name.c_str(), appInfo.version.c_str(), appInfo.identifier.c_str());

		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			Logger::logError(std::format("SDL couldn't be be initialized! Error: {}", SDL_GetError()).c_str());
			return false;
		}

		auto& window = subsystemRegistry.add<Window>();

		const auto& windowConfig = configManager.getWindowConfig();
		window.init(appInfo.name.c_str(), windowConfig);
		window.setIcon(windowConfig.iconPath);

		auto& inputHandler = subsystemRegistry.add<InputHandler>();
		inputHandler.init(configManager.getInputConfig());

		auto& renderer = subsystemRegistry.add<Renderer>();
		renderer.init(window);

		auto& audioController = subsystemRegistry.add<AudioController>();
		audioController.init();

		auto& engineResources = subsystemRegistry.add<EngineResources>();
		engineResources.registerLoader<TextureLoader>(renderer);
		engineResources.registerLoader<AudioLoader>();
		subsystemRegistry.add<EventBus>();
		subsystemRegistry.add<Physics>();
		//subsystemRegistry.add<ECSRegistry>();

		auto& assetManager = subsystemRegistry.add<AssetManager>();
		//m_impl->entityFactory.initialize(&assetManager);

		loadMedia(); // HERE? Dont do in init? also maybe dont do in Engine?

		m_impl->application.onCreated({ m_impl->systemManager, inputHandler, assetManager, renderer, window, m_impl->assetRoot });
		return true;
	}

	void Engine::shutdown()
	{
		m_impl->subsystemRegistry.forEach([](auto& subsystem) { subsystem->shutdown(); });

		//m_impl->sceneStack.clear();
		SDL_Quit();
	}

	void Engine::run()
	{
		bool running = true;

		auto& subsystemRegistry = m_impl->subsystemRegistry;

		// Test
		auto& resourceManager = subsystemRegistry.get<EngineResources>();
		auto textureHandle = resourceManager.getHandle<Texture>("test3.bmp");
		auto& texture = resourceManager.get<Texture>(textureHandle);

		auto audioHandle = resourceManager.getHandle<Audio>("707884__dave4884__pirates-song.wav");
		const auto& audio = resourceManager.get<Audio>(audioHandle);

		auto audioHandle2 = resourceManager.getHandle<Audio>("249813__spookymodem__goblin-death.wav");
		const auto& audio2 = resourceManager.get<Audio>(audioHandle2);

		auto& audioController = subsystemRegistry.get<AudioController>();
		audioController.playSound(audio2.m_stream, audio2.m_buffer, audio2.m_length);

		while (running)
		{
			Uint64 start = SDL_GetPerformanceCounter();

			auto& timer = m_impl->timer;
			timer.tick();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					running = false;
				}

				subsystemRegistry.get<InputHandler>().processInput(event); // TODO; "prefetch" input handler?
				subsystemRegistry.get<Window>().processEvent(event);
			}

			double deltaTime = timer.getDeltaTime();

			subsystemRegistry.get<InputHandler>().update();
			m_impl->application.onUpdate(deltaTime);


			// or getactivesceenes.... then iterate and, check if should update next 
			//m_impl->systemManager.update(deltaTime); // After application update?

			subsystemRegistry.get<EventBus>().dispatchAll();
			subsystemRegistry.get<Renderer>().clearScreen();


			// TEST
			for (int i = 0; i < 128; ++i)
			{
				for (int j = 0; j < 172; ++j)
				{
					subsystemRegistry.get<Renderer>().renderTexture(i * 10, j * 10, texture);
				}
			}

			subsystemRegistry.get<Renderer>().renderLine(0, 0, 100, 100);

			// END TEST....

			subsystemRegistry.get<Renderer>().present();

			Uint64 end = SDL_GetPerformanceCounter();
			float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
			float fps = 1.f / elapsed;
			// timer.getFPS();
			subsystemRegistry.get<Window>().setTitle(std::format("The Cursed Pirate - Fps: {}", (int)fps).c_str());
		}
	}

	void Engine::loadMedia()
	{
		// TODO; do lazy loading later on!? -> maybe load core resources?

		auto& assetManager = m_impl->subsystemRegistry.get<AssetManager>();

		// Load prefabs
		assetManager.addLoader<PrefabLoader>();

		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/prefabs/"))
		{
			if (!entry.is_regular_file())
				continue;

			auto handle = assetManager.loadAsset<Prefab>(entry.path());
			const auto& prefab = assetManager.getAsset<Prefab>(handle); // Dont later...
		}

		// Load sprite sheets
		assetManager.addLoader<SpriteSheetLoader>(m_impl->subsystemRegistry.get<EngineResources>());
		auto& engineResources = m_impl->subsystemRegistry.get<EngineResources>();

		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/textures"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();

			std::string filename = path.string();
			if (filename.ends_with("sprite_sheet.json"))
			{
				auto handle = assetManager.loadAsset<SpriteSheet>(entry.path());
			}
			else if (filename.ends_with(".png") || filename.ends_with(".bmp")) // TODO; function? isValidTextureFormat
			{
				engineResources.insertPath<Texture>(path.filename().string(), path);
			}
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/sounds"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();
			std::string filename = path.string();

			if (filename.ends_with(".wav"))
			{
				engineResources.insertPath<Audio>(path.filename().string(), path);
			}
		}
	}

	void registerComponents(TypeRegistry<ComponentInfo>& registry)
	{
		registerComponent<TransformComponent>(registry, "Transform",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
				auto& transformComponent = handle.getComponent<TransformComponent>();
				//transformComponent.position = properties.at("position"); // WORKS???
			});



		//registry.emplace<TransformComponent>("Transform",
		//	[](EntityHandle& handle, const ComponentProperties& properties) 
		//	{

		//	},
		//	[](EntityHandle& handle) 
		//	{

		//	});

		// register components
		//ComponentID id;
		//std::string name;
		//std::size_t alignment;
		//std::size_t size;

		//// TODO; need to include ComponentProperties? or forward declare it...
		//std::function<void(EntityHandle& handle, const ComponentProperties& properties)> initialization;
		//std::function<void(EntityHandle& handle)> addComponent;

		// add constructor that accepts name id, and functions.... do size and alignment in constructor...		
	}
}