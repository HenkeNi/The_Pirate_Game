#include "engine/core/engine.h"
#include "engine/core/application.h"
#include "engine/core/engine_context.h"
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
//#include "engine/core/config/config_loader.h"
#include <cassert>
#include <string_view>

namespace
{
	constexpr const char* initFailedMessage = "Engine Initialization Failed! Module: {}";
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
		Logger::logInfo("############################ Starting engine initialization ############################");
		assert(m_impl && "Engine::Impl is null!");

		auto& settings = m_impl->settings;

		const char* configPath = "../assets/config/engine_config.json"; // TODO: store elsewhere..

		if (!settings.loadConfig(configPath))
		{
			Logger::logError(std::format("Engine initialization aborted. Could not load configuration file {}", configPath));
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
		if (!rendering.init(platform.getWindow()))
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
		if (!physics.init())
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
		if (!resource.init(rendering.getRenderer(), configs.resource))
		{
			Logger::logError(std::format(initFailedMessage, "ResourceModule"));
			return false;
		}

		auto ctx = context();

		auto& ecs = m_impl->ecs;
		if (!ecs.init(ctx))
		{
			Logger::logError(std::format(initFailedMessage, "ECSModule"));
			return false;
		}

		auto& network = m_impl->network;
		if (!network.init())
		{
			Logger::logError(std::format(initFailedMessage, "NetworkModule"));
			return false;
		}

		m_impl->application.onCreated(ctx);

		Logger::logInfo("Engine initialization successful!\n\n");
		return true;
	}

	void Engine::shutdown()
	{
		Logger::logInfo("############################ Engine shutdown started ############################");
		assert(m_impl && "Engine::Impl is null!");

		m_impl->application.onDestroyed();

		m_impl->asset.shutdown();
		m_impl->audio.shutdown();
		m_impl->ecs.shutdown();
		m_impl->physics.shutdown();
		m_impl->resource.shutdown();
		m_impl->rendering.shutdown();
		m_impl->platform.shutdown(); // always last!

		Logger::logInfo("Engine shutdown complete!");
	}

	void Engine::run()
	{
		Logger::logInfo("############################ Engine Loop started ############################");
		assert(m_impl && "Engine::Impl is null!");

		bool running = true;

		while (running)
		{
			auto& platform = m_impl->platform;

			platform.beginFrame();
			platform.pollEvents();

			if (platform.shouldQuit())
			{
				running = false;
			}

			m_impl->rendering.beginFrame(); // rename clear screen?

			double deltaTime = platform.getDeltaTime();
			m_impl->application.onUpdate(deltaTime);

			// Update ecs systems here?
			//m_impl->systemManager.update(deltaTime); // After application update?

			m_impl->eventBus.dispatchAll();

			m_impl->resource.update(platform.getFrameCount(), deltaTime);

			//float currentTime = platform.getTime();
			//frameTimer.tick(currentTime);

			// Put in platfomr end...
			m_impl->rendering.endFrame();
			platform.endFrame();

			// timer.getFPS();
			// system sets fps?
			// window.setTitle(std::format("The Cursed Pirate - Fps: {}", (int)fps).c_str()); // render debug text instead?
		}
	}

	EngineContext Engine::context() const
	{
		return EngineContext{
			EngineContext::PlatformServices{
				&m_impl->platform.getInputHandler(),
				&m_impl->platform.getFrameTimer()
			},
			EngineContext::RenderingServices {
				m_impl->rendering.getRenderAPI()
			},
			EngineContext::AssetServices{
				&m_impl->asset.getAssetManager(),
				&m_impl->asset.getLocalization()
			},
			EngineContext::ResourceServices{
				&m_impl->resource.getAudioManager(),
				&m_impl->resource.getFontManager(),
				&m_impl->resource.getTextureManager(),
				&m_impl->resource.getTextManager(),
				&m_impl->resource.getTextFactory(),
			},
			EngineContext::ECSServices{
				&m_impl->ecs.getEntityFactory(),
				&m_impl->ecs.getComponentRegistry(),
				&m_impl->ecs.getSystemManager(),
			},
			EngineContext::PhysicsServices{
				&m_impl->physics.getPhysics(),
			},
			EngineContext::AudioServices{
				&m_impl->audio.getAudioController()
			},
			&m_impl->actionRegistry,
			&m_impl->eventBus,
			&m_impl->settings
		};
	}
}