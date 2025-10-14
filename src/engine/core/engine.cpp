#include "engine/core/engine.h"
#include "engine/core/logger.h"
#include "engine/core/application.h"
#include "engine/audio/audio_controller.h"
#include "engine/rendering/renderer.h"
#include "engine/resources/resource_manager.h"
#include "engine/window/window.h"
#include "engine/utils/frame_timer.h"
#include "engine/input/input_handler.h"
#include "engine/scenes/scene_stack.h"
#include "engine/events/event_bus.h"
#include "engine/config/config_manager.h"
#include <SDL3/SDL.h>

//#include "engine/utils/json_utils.h"

#include "engine/config/config_types.h" // remove later...
#include "engine/config/config_loader.h"


// REMOEV
#include "engine/audio/audio.h"

namespace cursed_engine
{
	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }, resourceManager{ renderer }
		{
		}

		InputHandler inputHandler;
		FrameTimer timer;
		ResourceManager resourceManager;		
		Window window;
		Renderer renderer;
		SceneStack sceneStack;
		EventBus eventBus;
		ConfigManager configManager;
		AudioController audioController;
		Application& application;


		// Event? or make static?
		// Physics?
		// Audio
		// Task system
		// World/ECS?
		// SceneManager/scene graph? or keep in game?
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
		auto& configManager = m_impl->configManager;

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
		
		const auto& windowConfig = configManager.getWindowConfig();
		m_impl->window.init(appInfo.name.c_str(), windowConfig);
		m_impl->window.setIcon(windowConfig.iconPath);

		const auto& inputConfig = configManager.getInputConfig();	
		m_impl->inputHandler.init(inputConfig);

		m_impl->renderer.init(m_impl->window);
		m_impl->audioController.init();

		m_impl->application.onCreated({ m_impl->inputHandler, m_impl->window, m_impl->renderer });
		return true;
	}

	void Engine::shutdown()
	{
		m_impl->window.shutdown();
		//m_impl->inputHandler
		m_impl->sceneStack.clear();
		SDL_Quit();
	}

	void Engine::run()
	{
		bool running = true;

		// Test
		auto textureHandle = m_impl->resourceManager.getTexture("../assets/textures/test3.bmp");
		auto* texture = m_impl->resourceManager.resolve(textureHandle);


		auto audioHandle = m_impl->resourceManager.getAudio("../assets/sounds/707884__dave4884__pirates-song.wav");
		auto* audio = m_impl->resourceManager.resolve(audioHandle);

		auto audioHandle2 = m_impl->resourceManager.getAudio("../assets/sounds/249813__spookymodem__goblin-death.wav");
		auto* audio2 = m_impl->resourceManager.resolve(audioHandle2);

		m_impl->audioController.playSound(audio2->m_stream, audio2->m_buffer, audio2->m_length);

		//

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

				m_impl->inputHandler.processInput(event);
				m_impl->window.processEvent(event);
			}

			double deltaTime = timer.getDeltaTime();

			m_impl->inputHandler.update();
			m_impl->sceneStack.update(deltaTime);

			m_impl->eventBus.dispatchAll();

			m_impl->renderer.clearScreen();

			// TEST
			for (int i = 0; i < 128; ++i)
			{
				for (int j = 0; j < 172; ++j)
				{
					m_impl->renderer.renderTexture(i * 10, j * 10, *texture);
				}
			}

			m_impl->renderer.renderLine(0, 0, 100, 100);


			// END TEST....

			m_impl->renderer.present();

			
			Uint64 end = SDL_GetPerformanceCounter();
			float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
			float fps = 1.f / elapsed;
			// timer.getFPS();
			m_impl->window.setTitle(std::format("The Cursed Pirate - Fps: {}", (int)fps).c_str());
		}
	}

	void Engine::loadMedia()
	{
		
	}
}