#include "engine/core/engine.h"

#include "engine/core/action/action_registry.h"
#include "engine/core/application.h"
#include "engine/core/config.h"
#include "engine/core/contexts.h"
//#include "engine/core/file_catalog.h"
#include "engine/core/logger.h"
#include "engine/core/events/event_bus.h" 
#include "engine/core/settings/settings.h"

#include "engine/assets/asset_store.h"
#include "engine/audio/audio_backend.h"
#include "engine/ecs/ecs.h"
#include "engine/graphics/graphics.h"
#include "engine/network/network.h"
#include "engine/physics/physics.h"
#include "engine/platform/platform.h"
#include "engine/resources/resource_managers.h"

#include "engine/utils/frame_timer.h"

#include <cassert>

#include "engine/platform/window.h" // remove later...

namespace
{
	using namespace cursed_engine;

	constexpr const char* errorMessage = "Engine initialization aborted.";

	[[nodiscard]] AudioBackendArgs createAudioBackendArgs(const Config::Audio& config, const AudioSettings& settings)
	{
		return AudioBackendArgs
		{
			{
				AudioBackendArgs::Options{ settings.sfxVolume, config.sfxTrackCount },
				AudioBackendArgs::Options{ settings.musicVolume, config.musicTrackCount },
				AudioBackendArgs::Options{ settings.ambienceVolume, config.ambienceTrackCount },
				AudioBackendArgs::Options{ settings.voiceVolume, config.voiceTrackCount },
				AudioBackendArgs::Options{ settings.uiVolume, config.uiTrackCount }
			},
			settings.masterVolume
		};
	}

	[[nodiscard]] GraphicsArgs createGraphicsArgs(const GraphicsSettings& settings) noexcept
	{
		return GraphicsArgs
		{
			settings.vsync
		};
	}

	[[nodiscard]] PlatformArgs createPlatformArgs(const Config& config, const Settings& settings) noexcept
	{
		const WindowSettings& wndSettings = settings.getWindowSettings();
		const InputSettings& inputSettings = settings.getInputSettings();
		const Config::Window& wndConfig = config.window;

		return PlatformArgs
		{
			WindowArgs
			{
				config.appInfo.name,
				IVec2{ wndConfig.xPos, wndConfig.yPos },
				wndSettings.size.x,
				wndSettings.size.y,
				wndSettings.fullscreen,
				wndConfig.alwaysOnTop,
			},
			InputArgs
			{
				inputSettings.keyBindings,
				inputSettings.mouseSensitivity
			},
			wndConfig.iconPath,
			config.appInfo.name.c_str(),
			config.appInfo.version.c_str(),
			config.appInfo.identifier.c_str()
		};
	}

	void registerAssetLoaders(AssetManager& assetManager)
	{
		assetManager.addLoader<AnimationLoader>();
		assetManager.addLoader<TextureAtlasLoader>();
		assetManager.addLoader<PrefabLoader>();
	}
}

namespace cursed_engine
{
	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }, platform{ eventBus }, settings{ eventBus }
		{
		}

		EventBus eventBus;
		//FileCatalog fileCatalog;
		Settings settings;
		Platform platform;
		Graphics graphics;
		AudioBackend audio; //rename AudioPlayback AudioPlayer; or simply Audio
		ResourceManagers resources; // ResourceRegistry => Resources
		Physics physics;
		Network network;
		AssetManager assets;  // AssetRegistry? => Assets (or AssetStore?)
		ECS ecs;
		Localization localization;
		ActionRegistry actionRegistry;

		std::uint64_t frameCount{}; // put in frame timer?
		Application& application;

		// Task system/Thread pool
		// Profiler
		// FIlesystem I/O

		// World manager?
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
		Logger::logInfo("Engine initializing...\n");
		assert(m_impl && "Engine::Impl is null!");

		Config config;
		if (Result<void> result = config.load(Config::getDefaultPath()); !result)
		{
			Logger::logError(std::format("{}. {}", errorMessage, result.message()));
			return false;
		}

		Settings& settings = m_impl->settings;
		if (Result<void> result = settings.load(Settings::getDefaultPath()); !result)
		{
			Logger::logError(std::format("Failed to read settings. Reason: {}", result.message()));
			return false;
		}

		Platform& platform = m_impl->platform;
		if (!platform.init(createPlatformArgs(config, settings)))
		{
			Logger::logError(errorMessage);
			return false;
		}

		Graphics& graphics = m_impl->graphics;
		if (!graphics.init(platform.getWindow(), createGraphicsArgs(settings.getGraphicsSettings())))
		{
			Logger::logError(errorMessage);
			return false;
		}

		AudioBackend& audio = m_impl->audio;
		if (!audio.init(createAudioBackendArgs(config.audio, settings.getAudioSettings())))
		{
			Logger::logError(errorMessage);
			return false;
		}
		
		ResourceManagers& resources = m_impl->resources;
		if (!resources.init(graphics.getTextureCreator(), audio.getAudioCreator()))
		{
			Logger::logError(errorMessage);
			return false;
		}

		Physics& physics = m_impl->physics;
		if (!physics.init({ graphics.getRenderAPI(), &resources.getFontManager(), graphics.getTextCreator() }))
		{
			Logger::logError(errorMessage);
			return false;
		}

		Network& network = m_impl->network;
		if (!network.init())
		{
			Logger::logError(errorMessage);
			return false;
		}

		AssetManager& assets = m_impl->assets;
		registerAssetLoaders(assets);

		auto ctx = context();

		ECS& ecs = m_impl->ecs;
		if (!ecs.init(&assets))
		{
			Logger::logError(errorMessage);
			return false;
		}

		m_impl->application.onCreated(ctx);

		Localization& localization = m_impl->localization;
		if (!localization.setLanguage(settings.getLocalizationSettings().language))
		{
			Logger::logError(errorMessage);
			return false;
		}

		assets.scanAssets("../assets/");

		Logger::logInfo("[Engine initialization completed]");
		return true;
	}

	void Engine::shutdown()
	{
		Logger::logInfo("[Engine shutdown began]");
		assert(m_impl && "Engine::Impl is null!");

		auto& impl = *m_impl;

		impl.application.onDestroyed();
		impl.ecs.shutdown();
		impl.resources.shutdown();
		// impl.assets.shutdown();
		impl.assets.unloadAll();
		impl.physics.shutdown();
		impl.audio.shutdown();
		impl.graphics.shutdown();
		impl.platform.shutdown();

		Logger::logInfo("[Engine shutdown completed!]");
	}

	void Engine::run()
	{
		//Logger::logInfo("[Engine] - Starting game loop...");
		assert(m_impl && "Engine::Impl is null!");

		auto& impl = *m_impl;

		bool running = true; // EngineState struct that contains paused, minimized, hasFocus?
		FrameTimer timer;

		constexpr float fixedTimeStep = 1.0f / 60.0f;
		double accumulator = 0.0;

		while (running)
		{
			const double deltaTime = timer.getDeltaTime(); // Do here (first)?

			//do this inside inner loop? also do endFramE?
			timer.tick();
			impl.platform.beginFrame();
			impl.platform.processEvents(); // tick delta time first of all?

			if (impl.platform.exitRequested())
			{
				running = false;
				continue;
			}

			accumulator += deltaTime;

			while (accumulator >= fixedTimeStep)
			{
				impl.eventBus.dispatchAll();

				impl.application.onUpdate(UpdateContext{ fixedTimeStep, impl.platform.getWindow().getState().size }); // have platform return preferences instead??

				// update game
				// update physics (physics.step())

				// dispatch events again?
				impl.platform.endFrame();

				accumulator -= fixedTimeStep;
			}

			impl.resources.update(impl.frameCount, deltaTime);

			impl.graphics.beginFrame();
			impl.application.onRender(RenderContext{}); // DONT PASS rendering api and pipeline?

			// Update ecs systems here?
			//m_impl->systemManager.update(deltaTime); // After application update?

			//Logger::logInfo("[Engine] - End frame..");

			impl.graphics.endFrame();
			//impl.platform.endFrame();
			++impl.frameCount;
		}
	}

	EngineContext Engine::context() const
	{
		auto& impl = *m_impl;

		return EngineContext{
			EngineContext::PlatformServices{
				InputAPI{ &impl.platform.getInputHandler() }
				//&impl.platform.getFrameTimer()
			},
			EngineContext::RenderingServices {
				impl.graphics.getRenderAPI(),
				&impl.graphics.getRenderPipeline(),
				impl.graphics.getTextCreator()
			},
			EngineContext::AssetServices{
				&impl.assets
			},
			/*EngineContext::ResourceServices{
				&impl.resources.getAudioManager(),
				&impl.resources.getFontManager(),
				&impl.resources.getTextureManager(),
			},*/
			EngineContext::ECSServices{
				&impl.ecs.getEntityFactory(),
				&impl.ecs.getComponentRegistry(),
				&impl.ecs.getSystemManager(),
			},
			EngineContext::PhysicsServices{
				impl.physics.getPhysicsAPI(),
				&impl.physics.getPhysicsDebugDraw()
			},
			EngineContext::AudioServices{
				AudioController{ &impl.audio }
			},
			Resources
			{ 
				&impl.resources.getAudioManager(),
				&impl.resources.getFontManager(),
				&impl.resources.getTextureManager()
			},
			&impl.actionRegistry,
			&impl.localization,
			&impl.eventBus,
			&impl.settings
		};
	}
}