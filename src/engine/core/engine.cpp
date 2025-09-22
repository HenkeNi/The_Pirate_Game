#include "engine/core/engine.h"
#include "engine/core/logger.h"
#include "engine/core/application.h"
#include "engine/rendering/renderer.h"
#include "engine/resources/resource_manager.h"
#include "engine/window/window.h"
#include "engine/window/window_config.h"
#include "engine/utils/frame_timer.h"
#include "engine/input/input_handler.h"
#include "engine/scenes/scene_stack.h"
#include "engine/events/event_bus.h"
#include <SDL3/SDL.h>


#include "engine/input/input_config.h" // remove later...

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
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			Logger::logError(std::format("SDL couldn't be be initialized! Error: {}", SDL_GetError()).c_str());
			return false;
		}
		
		// store in registry? or engine config class?
		WindowConfig windowCfg{ "The Cursed Pirate", 1280, 720, false, true };
		m_impl->window.init(windowCfg);
		m_impl->window.setIcon("../assets/textures/test3.bmp");

		InputConfig inputCfg;
		inputCfg.keyBindings.insert({ (SDL_Scancode)26, Action::MoveUp });
		inputCfg.keyBindings.insert({ (SDL_Scancode)4, Action::MoveLeft });
		inputCfg.keyBindings.insert({ (SDL_Scancode)22, Action::MoveDown });
		inputCfg.keyBindings.insert({ (SDL_Scancode)7, Action::MoveRight });
		m_impl->inputHandler.init(inputCfg);

		m_impl->renderer.init(m_impl->window);
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
		auto handle = m_impl->resourceManager.getTexture("../assets/textures/test3.bmp");
		auto* texture = m_impl->resourceManager.resolve(handle);
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

			m_impl->eventBus.process();

			m_impl->renderer.clearScreen();

			int counter = 0;
			for (int i = 0; i < 128; ++i)
			{
				for (int j = 0; j < 172; ++j)
				{
					++counter;
					m_impl->renderer.renderTexture(i * 10, j * 10, *texture);
				}
			}

			m_impl->renderer.renderLine(0, 0, 100, 100);
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