#include "engine/core/engine.h"
#include "engine/core/application.h"
#include "engine/rendering/renderer.h"
#include "engine/window/window.h"
#include "engine/window/window_config.h"
#include "engine/utils/frame_timer.h"
#include "engine/input/input_handler.h"

//#include <assert.h>
//#include <iostream>
#include <SDL3/SDL.h>

#include "engine/resources/texture_loader.h" // TODO; remove later

namespace cursed_engine
{
	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }
		{
		}

		// TODO; or store sdl subsystems raw here?

		Renderer renderer;
		Window window;
		// Event? or make static?
		// Physics?
		// Audio
		// ResourceHolder(s) texture, audio, shaders?
		// Task system
		// SceneManager/scene graph? or keep in game?
		InputHandler inputHandler;
		FrameTimer timer;
		Application& application;
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
		assert(m_impl && "Failed to allocate memory for m_impl");

		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO))
		{
			SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
			return false;
		}

		// store in registry? or engine config class?
		WindowConfig wConfig{ "The Cursed Pirate", 1280, 720, false, true };

		if (!m_impl)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "m_impl was nullptr!");
			return false;
		}

		m_impl->window.init(wConfig);
		m_impl->renderer.init(m_impl->window);

		m_impl->application.init();
				
		return true;
	}

	void Engine::shutdown()
	{
		SDL_Quit();
	}

	void Engine::run()
	{
		bool running = true;

		// Test
		TextureLoader textureLoader;
		auto texture = textureLoader.loadTexture(m_impl->renderer, "../assets/textures/test3.bmp");
		//

		while (running)
		{
			Uint64 start = SDL_GetPerformanceCounter();

			auto& timer = m_impl->timer;
			timer.tick();

			double deltaTime = timer.getDeltaTime();



			SDL_Event event;
			//SDL_zero(event);

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT) // create engine function checkIfShouldQuit?
				{
					running = false;
				}

				m_impl->inputHandler.processInput(&event);
			}

			m_impl->renderer.clearScreen();

			int counter = 0;
			for (int i = 0; i < 128; ++i)
			{
				for (int j = 0; j < 172; ++j)
				{
					++counter;
					m_impl->renderer.renderTexture(i * 10, j * 10, texture.value());
				}
			}

			m_impl->renderer.present();


			Uint64 end = SDL_GetPerformanceCounter();
			float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
			float fps = 1.f / elapsed;

			m_impl->window.setTitle("The Cursed Pirate - Fps: " + std::to_string((int)fps));
		}
	}

	void Engine::loadMedia()
	{

	}
}



//struct EngineTest::Pimpl
//{
//	class Renderer* m_renderer;
//	class Window* m_window;
//
//
//};