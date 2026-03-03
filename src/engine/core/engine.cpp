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

#include "engine/core/subsystem.h"
#include "engine/localization/localization.h"

#include "engine/config/config_types.h" // remove later...
#include "engine/config/config_loader.h"

// REMOEV
#include "engine/audio/audio.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/font.h"
#include "engine/utils/containers/sparse_set.hpp"
#include <optional>

#include "engine/core/types.h"
#include "engine/utils/json/json_document.h"
#include "engine/core/registry_aliases.h"


#include "engine/utils/json/json_value.h" // include this or not?

#include "engine/ecs/component/core_components.h"
#include "engine/ecs/system/render_system.h"
#include "engine/ecs/system/interaction_system.h"
#include "engine/ecs/system/input_system.h"
#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/system/text_system.h"

namespace cursed_engine
{
	struct MetaStorage // MOVE to own file?
	{
		ComponentRegistry componentData;
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
		EntityFactory entityFactory; // pass in assetmanager, set ecs registry...
		std::filesystem::path assetRoot;
		Application& application;

		// store an ECSContext here?

		//EntityFactory entityFactory; // dont store instnace? let game use if needed?

		// Task system/Thread pool
		// Profiler
	};

	void registerComponents(ComponentRegistry& registry, AssetManager& assetManager, EngineResources& engineResources);
	void registerSystems(SystemManager& systemManager, Renderer& renderer, EngineResources& resources, AssetManager& assetManager, InputHandler& inputHandler);

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
		m_impl->entityFactory.init(&assetManager);

		// TODO; iterate through all files in folder...
		auto& localization = subsystemRegistry.add<Localization>();
		localization.registerLanguage("english", "../assets/localization/en.json"); // Dont here? read start language from config...
		localization.setLanguage("english");

		loadMedia(); // HERE? Dont do in init? also maybe dont do in Engine?

		registerComponents(m_impl->metaStorage.componentData, assetManager, engineResources);
		registerSystems(m_impl->systemManager, renderer, engineResources, assetManager, inputHandler);

		m_impl->application.onCreated({
			m_impl->systemManager,
			inputHandler, assetManager,
			m_impl->entityFactory,
			renderer,
			window,
			m_impl->metaStorage.componentData,
			m_impl->assetRoot
			});

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
		auto textureHandle = resourceManager.getHandle<Texture>("test3");
		//auto textureHandle = resourceManager.getHandle<Texture>("test3.bmp");
		auto& texture = resourceManager.get<Texture>(textureHandle);

		auto audioHandle = resourceManager.getHandle<Audio>("707884__dave4884__pirates-song");
		//auto audioHandle = resourceManager.getHandle<Audio>("707884__dave4884__pirates-song.wav");
		const auto& audio = resourceManager.get<Audio>(audioHandle);

		//auto audioHandle2 = resourceManager.getHandle<Audio>("249813__spookymodem__goblin-death.wav");
		auto audioHandle2 = resourceManager.getHandle<Audio>("249813__spookymodem__goblin-death");
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

			// subsystemRegistry.get<Renderer>().clearScreen();

			// TEST
			/*for (int i = 0; i < 128; ++i)
			{
				for (int j = 0; j < 172; ++j)
				{
					subsystemRegistry.get<Renderer>().renderTexture(i * 10, j * 10, texture);
				}
			}*/

			//subsystemRegistry.get<Renderer>().renderLine(0, 0, 100, 100);

			// END TEST....

			// subsystemRegistry.get<Renderer>().present();

			Uint64 end = SDL_GetPerformanceCounter();
			float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
			float fps = 1.f / elapsed;
			// timer.getFPS();
			subsystemRegistry.get<Window>().setTitle(std::format("The Cursed Pirate - Fps: {}", (int)fps).c_str());
		}
	}

	void Engine::loadMedia()
	{
		// TODO; use a single loop for all files in asset

		//Not loading textuer atlases? 

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

		auto& engineResources = m_impl->subsystemRegistry.get<EngineResources>();

		// Load sprite sheets
		assetManager.addLoader<SpriteSheetLoader>(engineResources);
		assetManager.addLoader<TextureAtlasLoader>(engineResources);

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
			else if (filename.ends_with("texture_atlas.json"))
			{
				auto handle = assetManager.loadAsset<TextureAtlas>(entry.path());
			}
			else if (filename.ends_with(".png") || filename.ends_with(".bmp") || filename.ends_with(".jpeg")) // TODO; function? isValidTextureFormat
			{
				engineResources.insertPath<Texture>(path.filename().stem().string(), path);
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
				engineResources.insertPath<Audio>(path.filename().stem().string(), path); // TODO; utiltyFunction getStem()?
			}
		}



		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/fonts"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();
			std::string filename = path.string();

			if (filename.ends_with(".ttf"))
			{
				//engineResources.insertPath<Font>(path.filename().stem().string(), path); // auto generate id?
			}
		}




		// TODO; load texture atlas...



	}

	void registerComponents(ComponentRegistry& registry, AssetManager& assetManager, EngineResources& engineResources)
	{
		// TODO; use config to check which subsystems are active, only register if active? (physics -> physicsComponent)

		registerComponent<TransformComponent>(registry, "transform",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
				auto& transformComponent = handle.getComponent<TransformComponent>();
				//transformComponent.position = properties.at("position"); // WORKS???
			},
			[](EntityHandle& handle, const JsonValue& value)
			{
				float x = (float)value["x"].asDouble();
				float y = (float)value["y"].asDouble();

				// TODO; also valid check it contains the data?

				float width = (float)value["width"].asDouble();
				float height = (float)value["height"].asDouble();

				FVec2 pivot{ 0.f, 0.f };

				if (value.has("pivot")) {
					pivot.x = (float)value["pivot"]["x"].asDouble();
					pivot.y = (float)value["pivot"]["y"].asDouble();
				}

				float rotation = 0.f;

				if (value.has("rotation"))
					float rotation = (float)value["rotation"].asDouble();

				handle.attachComponent<TransformComponent>(FVec2{ x, y }, FVec2{ width, height }, pivot, rotation);
			});

		registerComponent<SpriteComponent>(registry, "sprite",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
			},
			[&](EntityHandle& handle, const JsonValue& value)
			{
				std::string id = value["id"].asString();

				const auto atlasHandle = assetManager.getAssetHandle<TextureAtlas>(id); // pass in id?
				AtlasRegion region; // TODO; fix!
				region.x = 0;
				region.y = 0;
				region.w = 700;
				region.h = 700;

				std::array<float, 4> color{ 1.f, 1.f, 1.f, 1.f };

				float zOrder = 1.f; 

				// Figue out...
				// Treat background images and texture alias the same? or solve with union/variant?


				/*
					AssetHandle atlasHandle; // Handle to texture atlas
					AtlasRegion region;
					float colors[4];
					float zOrder;
				*/

				handle.attachComponent<SpriteComponent>(atlasHandle, region, color, zOrder);
			});


		/*registerComponent<InputComponent>(registry, "input",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{},
			[](EntityHandle& handle, const JsonValue& value)
			{
			});*/

		// TODO; interaction component instead???
		registerComponent<ButtonComponent>(registry, "button",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
			},
			[](EntityHandle& handle, const JsonValue& value)
			{
				handle.attachComponent<ButtonComponent>();
			});


		registerComponent<BoundingBox>(registry, "bounding_box",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{

			},
			[](EntityHandle& handle, const JsonValue& value) 
			{
				int xOffset = value["offset"]["x"].asInt();
				int yOffset = value["offset"]["y"].asDouble();

				int width = value["size"]["width"].asInt(); // TODO; use halfextent instead!
				int height = value["size"]["height"].asInt(); // TODO; decide if float or int...

				handle.attachComponent<BoundingBox>(FVec2{ (float)xOffset, (float)yOffset }, FVec2{ (float)width, (float)height });
			});

		registerComponent<TextComponent>(registry, "text",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{

			},
			[&](EntityHandle& handle, const JsonValue& value)
			{
				std::string id; // no way of knowing the id...

				// store lockup table id to text (and perhaps text to id)

				//auto handle = engineResources.getHandle<Texture>(id); // WIll this load the resource? probably not

				handle.attachComponent<TextComponent>();
			});
	}

	void registerSystems(SystemManager& systemManager, Renderer& renderer, EngineResources& resources, AssetManager& assetManager, InputHandler& inputHandler)
	{
		systemManager.emplace<RenderSystem>(renderer, resources, assetManager);
		//systemManager.emplace<InputSystem>(inputHandler);
		systemManager.emplace<InteractionSystem>();
		systemManager.emplace<UISystem>(inputHandler);
		systemManager.emplace<TextSystem>(resources);
	}
}