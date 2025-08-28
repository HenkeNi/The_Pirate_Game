#include "engine/core/engine.h"
#include "engine/core/application.h"
#include "engine/renderer/renderer.h"
#include "engine/window/window.h"

#include <iostream>
#include <SDL3/SDL.h>

namespace cursed_engine
{
	struct Engine::Impl
	{
		Impl(Application& app)
			: App{ app }
		{
		}

		Renderer Renderer;
		Window Window;
		//SubsystemRegistry SubsystemRegistry;
		Application& App;
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
		if (!m_impl)
		{
			std::cerr << "Error: Impl not initialized!\n";
			return false;
		}

		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_VIDEO);

		m_impl->Window.init();
		m_impl->Renderer.init(m_impl->Window);

		//auto& [registry, app] = *m_impl;

		//registry.emplace<Window>();
		//registry.emplace<Renderer>();

		// TODO; init subsystems

		m_impl->App.init();
				
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
			}
		}

	}
}



//struct EngineTest::Pimpl
//{
//	class Renderer* m_renderer;
//	class Window* m_window;
//
//
//};