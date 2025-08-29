#include "engine/core/engine.h"
#include "engine/core/application.h"
#include "engine/renderer/renderer.h"
#include "engine/window/window.h"
#include "engine/window/window_config.h"

#include <assert.h>
#include <iostream>
#include <SDL3/SDL.h>

namespace cursed_engine
{
	struct Engine::Impl
	{
		Impl(Application& app)
			: application{ app }
		{
		}

		Renderer renderer;
		Window window;
		//SubsystemRegistry SubsystemRegistry;
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
			std::cerr << "Failed to initialize SDL\n";
			return false;
		}

		// store in registry? or engine config class?
		WindowConfig wConfig{ "The Cursed Pirate", 1280, 720, false, true };

		if (!m_impl)
		{
			std::cerr << "Error: Impl not initialized!\n";
			return false;
		}

		m_impl->window.init(wConfig);
		m_impl->renderer.init(m_impl->window);

		//auto& [registry, app] = *m_impl;
		//registry.emplace<Window>();
		//registry.emplace<Renderer>();
		// TODO; init subsystems

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

		while (running)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					running = false;
				}

				m_impl->renderer.clearScreen();
			}
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