#include "engine/core/engine.h"
#include "engine/core/engine_context.h"
#include "engine/core/application.h"
#include "engine/core/logger.h"
#include "engine/modules/asset_module.h"
#include "engine/modules/audio_module.h"
#include "engine/modules/ecs_module.h"
#include "engine/modules/network_module.h"
#include "engine/modules/platform_module.h"
#include "engine/modules/render_module.h"
#include "engine/modules/resource_module.h"
#include "engine/modules/physics_module.h"
#include "engine/core/events/event_bus.h" 
#include "engine/core/settings/settings.h"
#include "engine/core/action/action_registry.h"
#include <cassert>

namespace
{
	constexpr const char* initFailedMessage = "[Engine] - Initialization Failed! Module: {}";
}

namespace cursed_engine
{
	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }, platform{ eventBus }, settings{ eventBus }
		{
		}

		// Core 
		PlatformModule platform;
		EventBus eventBus;
		Settings settings;

		// Resource
		AssetModule asset;
		ResourceModule resource;

		// Output
		RenderModule rendering;
		AudioModule audio;

		// Simulation
		ECSModule ecs;
		PhysicsModule physics;
		ActionRegistry actionRegistry;

		// Network
		NetworkModule network;

		Application& application;

		// LayerStack??? ImGuilayer? debug layer? ui layer? game layer?

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
		Logger::logInfo("[Engine] - Began initialization...\n");
		assert(m_impl && "Engine::Impl is null!");

		auto& settings = m_impl->settings;

		Result result = settings.loadConfig(Settings::getConfigPath());

		if (!result.ok())
		{
			Logger::logError(std::format("[Engine] - Error occured trying to read engine configs. Reason: {}", result.message()));
			return false;
		}

		const auto& configs = settings.getEngineConfig();

		auto& platform = m_impl->platform;
		if (!platform.init(configs))
		{
			Logger::logError(std::format(initFailedMessage, "PlatformModule"));
			return false;
		}

		auto& rendering = m_impl->rendering;
		if (!rendering.init(platform.getWindow(), configs.render, configs.platform.backend))
		{
			Logger::logError(std::format(initFailedMessage, "RenderModule"));
			return false;
		}

		auto& audio = m_impl->audio;
		if (!audio.init())
		{
			Logger::logError(std::format(initFailedMessage, "AudioModule"));
			return false;
		}

		auto& physics = m_impl->physics;
		if (!physics.init(rendering.getRenderAPI()))
		{
			Logger::logError(std::format(initFailedMessage, "PhysicsModule"));
			return false;
		}

		auto& asset = m_impl->asset;
		if (!asset.init())
		{
			Logger::logError(std::format(initFailedMessage, "AssetModule"));
			return false;
		}

		auto& resource = m_impl->resource;
		if (!resource.init(rendering.getResourceCreator(), configs.resource, configs.platform.backend))
		{
			Logger::logError(std::format(initFailedMessage, "ResourceModule"));
			return false;
		}

		auto& network = m_impl->network;
		if (!network.init())
		{
			Logger::logError(std::format(initFailedMessage, "NetworkModule"));
			return false;
		}

		auto ctx = context();

		auto& ecs = m_impl->ecs;
		if (!ecs.init(ctx))
		{
			Logger::logError(std::format(initFailedMessage, "ECSModule"));
			return false;
		}

		m_impl->application.onCreated(ctx);

		asset.scanAssets();

		Logger::logInfo("[Engine] - Initialization successful!");
		return true;
	}

	void Engine::shutdown()
	{
		Logger::logInfo("[Engine] - Shutdown began...");
		assert(m_impl && "Engine::Impl is null!");

		auto& impl = *m_impl;

		impl.application.onDestroyed();
		impl.asset.shutdown();
		impl.audio.shutdown();
		impl.ecs.shutdown();
		impl.physics.shutdown();
		impl.resource.shutdown();
		impl.rendering.shutdown();
		impl.platform.shutdown();

		Logger::logInfo("[Engine] - Shutdown complete!");
	}

	void Engine::run()
	{
		Logger::logInfo("[Engine] - Starting game loop...");
		assert(m_impl && "Engine::Impl is null!");

		auto& impl = *m_impl;

		bool running = true; // EngineState struct that contains paused, minimized, hasFocus?

		while (running)
		{
			impl.platform.beginFrame();
			impl.platform.processEvents();

			if (impl.platform.exitRequested())
			{
				running = false;
			}

			impl.rendering.beginFrame();

			double deltaTime = impl.platform.getDeltaTime();

			impl.application.onUpdate(deltaTime);
			impl.application.onRender(RenderContext{ impl.rendering.getRenderPipeline() }); // DONT PASS rendering api and pipeline?
			
			// Update ecs systems here?
			//m_impl->systemManager.update(deltaTime); // After application update?

			impl.eventBus.dispatchAll();
			impl.resource.update(impl.platform.getFrameCount(), deltaTime);

			//float currentTime = platform.getTime();
			//frameTimer.tick(currentTime);
			// timer.getFPS();
			// system sets fps?
			// window.setTitle(std::format("The Cursed Pirate - Fps: {}", (int)fps).c_str()); // render debug text instead?

			impl.rendering.endFrame();
			impl.platform.endFrame();
		}
	}

	EngineContext Engine::context() const
	{
		auto& impl = *m_impl;

		return EngineContext{
			EngineContext::PlatformServices{
				impl.platform.getInputAPI(),
				&impl.platform.getFrameTimer()
			},
			EngineContext::RenderingServices {
				impl.rendering.getRenderAPI(),
				impl.rendering.getRenderPipeline()
			},
			EngineContext::AssetServices{
				&impl.asset.getAssetManager(),
				&impl.asset.getLocalization()
			},
			EngineContext::ResourceServices{
				&impl.resource.getAudioManager(),
				&impl.resource.getFontManager(),
				&impl.resource.getTextureManager(),
				&impl.resource.getTextManager()
			},
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
				&impl.audio.getAudioController()
			},
			&impl.actionRegistry,
			&impl.eventBus,
			&impl.settings
		};
	}
}